# Communication Protocol

This page explains how the Delto gripper communicates with your computer over the network. Understanding this is helpful for debugging connection issues and for advanced users who want to know what happens at the network level.

## Overview

All Delto grippers communicate over **TCP/IP** using a custom binary protocol implemented in the `delto_tcp_comm` library (package name: `delto_tcp_comm`).

## Connection Details

| Parameter | Default Value | Description |
|-----------|---------------|-------------|
| IP Address | `169.254.186.72` | Link-local address (configurable) |
| Port | `502` | TCP port the gripper listens on |
| Protocol | TCP | Reliable, ordered byte stream |

```{warning}
The gripper accepts only **one TCP connection** at a time. If another program (or another instance of the driver) is already connected, your connection attempt will fail. Make sure to stop any existing connections before starting a new one.
```

## Network Configuration

The gripper uses a **link-local** IP address (`169.254.x.x`) by default. This means:

- No DHCP server or router is needed.
- You connect an Ethernet cable directly between your computer and the gripper.
- Your computer's Ethernet interface must be configured with an IP address on the same `169.254.x.x/16` subnet.

**Setting up the connection:**

```bash
# Find your Ethernet interface name
ip link show

# Assign a static IP on the same subnet (replace enp3s0 with your interface)
sudo ip addr add 169.254.186.1/16 dev enp3s0

# Verify connectivity
ping 169.254.186.72
```

## Data Flow

Here is what happens during each communication cycle:

```
delto_hardware::read()
  --> DeltoTCP::Communication::GetData()
    --> TCP send: data request packet
    --> TCP recv: response packet (via poll() with 500ms timeout)
    --> Parse binary response into DeltoReceivedData struct

delto_hardware::write()
  --> CurrentControl() computes duty values from effort commands
  --> ConvertDuty() applies motor direction mapping
  --> DeltoTCP::Communication::SendDuty()
    --> TCP send: duty command packet
```

## Socket Configuration

The TCP socket is configured for low-latency real-time communication:

| Setting | Value | Why |
|---------|-------|-----|
| `TCP_NODELAY` | Enabled | Disables Nagle's algorithm. Sends each packet immediately instead of buffering, reducing latency |
| Receive timeout | 500ms (via `poll()`) | Maximum time to wait for a response. If no response arrives within 500ms, the read is considered failed |
| TCP Keepalive idle | 1 second | Time before the first keepalive probe is sent after the connection goes idle |
| TCP Keepalive interval | 1 second | Time between subsequent keepalive probes |
| TCP Keepalive probes | 3 | Number of failed probes before the connection is considered dead |

With these keepalive settings, a cable disconnection is detected within approximately **4 seconds** (1s idle + up to 3 probes at 1s intervals).

## Sensor Types

During the initial connection, the gripper firmware reports its installed sensor type. The communication library automatically uses the correct data format based on this:

| Type | Enum Value | Description | Data per Finger | Raw Units / Range |
|------|------------|-------------|-----------------|-------------------|
| `NONE` | `0x00` | No sensor installed | -- | -- |
| `FT_6AXIS` | `0x01` | 6-axis force/torque sensor | 12 bytes (6 x int16: fx, fy, fz, tx, ty, tz) | Force: 0.1 N, Torque: 1 mNm |
| `FT_3AXIS` | `0x02` | 3-axis force/torque sensor | 12 bytes (6 x int16, 3 values unused) | Force: 0.1 N |
| `FT_4AXIS` | `0x04` | 4-axis force/torque sensor | 12 bytes (6 x int16, 2 values unused) | Force: 0.1 N, Torque: 1 mNm |
| `TACTILE_M` | `0x03` | Tactile matrix sensor (3x5 grid) | 15 bytes (uint8 per cell) | 0–255 per cell |
| `TACTILE_S` | `0x05` | Tactile matrix sensor (3x6 grid) | 36 bytes (18 x uint16) | 0–4095 per cell |

Multi-byte sensor values are transmitted in **big-endian** byte order. Tactile cells are
ordered top-left to bottom-right (row-major). The library converts F/T values to SI units
(N, Nm) before exposing them in `DeltoReceivedData`.

```{note}
The sensor type is detected automatically from the firmware. You do not need to configure it manually. The `delto_tcp_comm` library parses the correct data format based on the reported type.
```

## Received Data Structure

Each `GetData()` call returns a `DeltoReceivedData` structure containing:

| Field | Type | Description |
|-------|------|-------------|
| `joint` | `vector<double>` | Joint positions in radians |
| `velocity` | `vector<double>` | Joint velocities in radians/second |
| `current` | `vector<double>` | Motor currents in amperes |
| `temperature` | `vector<double>` | Motor temperatures in degrees Celsius |
| `fingertip_sensor` | `vector<double>` | F/T sensor data (N fingers x 6 axes) -- populated for FT sensor types |
| `gpio` | `vector<bool>` | GPIO states: [output1, output2, output3, input1] |
| `tactile_m` | `vector<vector<uint8_t>>` | Tactile M data: per finger, 3x5=15 cells -- populated for TACTILE_M type |
| `tactile_s` | `vector<vector<uint16_t>>` | Tactile S data: per finger, 3x6=18 cells -- populated for TACTILE_S type |

Only the sensor field matching the detected sensor type is populated. The other sensor fields remain empty.

## Troubleshooting Connection Issues

### Cannot ping the gripper

1. Verify the Ethernet cable is plugged in at both ends.
2. Check that the gripper is powered on.
3. Verify your interface is configured correctly: `ip addr show <interface>`.
4. Make sure the IP address is on the correct subnet: `169.254.x.x/16`.

### Connection refused

The gripper may already have an active connection from another program. Check for other ROS 2 nodes or applications that might be connected, and stop them first.

### Connection drops intermittently

- Check the Ethernet cable for damage or loose connections.
- The hardware interface will automatically attempt to reconnect (up to 10 retries at 1-second intervals).
- Monitor the connection status: `ros2 control list_hardware_interfaces | grep connection_status`.

### High latency or missed commands

- Ensure `TCP_NODELAY` is working (it is enabled by default in the library).
- Check for network congestion if the gripper is on a shared network instead of a direct connection.
- A direct Ethernet connection (computer to gripper) is always recommended for the lowest latency.
