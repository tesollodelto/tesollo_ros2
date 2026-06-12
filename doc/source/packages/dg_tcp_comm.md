# delto_tcp_comm

[![CI](https://github.com/tesollodelto/dg_tcp_comm/actions/workflows/ci.yml/badge.svg)](https://github.com/tesollodelto/dg_tcp_comm/actions/workflows/ci.yml)

Unified TCP communication library for all Delto grippers.

```{note}
**For most users:** You do not need to interact with this package directly. It is used internally by the `delto_hardware` package to communicate with the gripper firmware. This page is provided for developers who want to understand how the communication layer works.
```

## Overview

The `delto_tcp_comm` package provides the `DeltoTCP::Communication` class, which handles all TCP communication between your computer and the Delto gripper firmware. It uses **POSIX sockets** (standard Linux system calls) for networking -- no external libraries like Boost are required.

## How It Works

When the gripper driver starts, the hardware interface creates a `DeltoTCP::Communication` instance and calls `Connect()` to establish a TCP connection to the gripper. Once connected, the communication loop works like this:

1. **Read cycle:** `GetData()` sends a data request to the gripper and receives a response containing joint positions, velocities, currents, temperatures, sensor data, and GPIO states.
2. **Write cycle:** `SendDuty()` sends motor duty (current) commands to the gripper.

This cycle repeats at the controller manager's update rate (typically 100-300 Hz).

## Network Configuration

| Setting | Value | Description |
|---------|-------|-------------|
| Socket type | POSIX (no Boost) | Uses direct Linux system calls (`socket`, `connect`, `send`, `recv`) |
| Receive timeout | 500ms (`poll()`) | Maximum wait time for a response from the gripper |
| TCP Keepalive | 1s idle, 1s interval, 3 probes | Detects cable disconnection within approximately 4 seconds |
| TCP_NODELAY | Enabled | Disables Nagle's algorithm for lower latency |

```{note}
**What is TCP_NODELAY?** By default, TCP buffers small packets and sends them together for efficiency (Nagle's algorithm). For real-time robot control, this adds unacceptable latency. `TCP_NODELAY` disables this buffering so each command is sent immediately.
```

## Key APIs

These are the main methods provided by the `DeltoTCP::Communication` class:

| Method | Description |
|--------|-------------|
| `Connect()` / `Disconnect()` | Open or close the TCP connection to the gripper |
| `GetData()` | Read all sensor data from the gripper (joints, F/T sensors, GPIO, etc.) |
| `SendDuty(std::vector<int>)` | Send motor duty commands to the gripper |
| `GetFirmwareVersion()` | Query the gripper's firmware version |
| `SetFTSensorOffset()` | Zero-calibrate the fingertip F/T sensors |
| `SetGPIO(bool, bool, bool)` | Set the three GPIO output states |
| `GetSensorType()` | Query what type of sensor is installed on the gripper |
| `GetFingerSensorMask()` | Query which fingers have sensors installed (as a bitmask) |

## Sensor Types

The gripper firmware reports its sensor type during the initial handshake. The communication library automatically parses the sensor data in the correct format:

| Type | Description | Data per finger | Units / Range |
|------|-------------|-----------------|---------------|
| `FT_6AXIS` | 6-axis force/torque sensor | 6 doubles (fx, fy, fz, tx, ty, tz) | Force: N, Torque: Nm |
| `FT_3AXIS` | 3-axis force/torque sensor | 6 doubles (3 used, 3 unused) | Force: N |
| `FT_4AXIS` | 4-axis force/torque sensor | 6 doubles (4 used, 2 unused) | Force: N, Torque: Nm |
| `TACTILE_M` | 3x5 tactile matrix sensor | 15 uint8 values per finger | 0–255 per cell |
| `TACTILE_S` | 3x6 tactile matrix sensor | 18 uint16 values per finger | 0–4095 per cell |

The sensor type is detected automatically -- you do not need to configure it manually.

```{note}
On the wire, F/T values are transmitted as signed 16-bit big-endian integers in units of
0.1 N (force) and 1 mNm (torque); the library converts them to SI units (N, Nm).
Tactile cells are ordered top-left to bottom-right (row-major): `TACTILE_M` uses 1 byte
per cell, `TACTILE_S` uses 2 bytes per cell (big-endian, 12-bit range 0–4095).
```

## Data Structure

The `GetData()` method returns a `DeltoReceivedData` structure containing:

| Field | Type | Description |
|-------|------|-------------|
| `joint` | `vector<double>` | Joint positions (radians) |
| `velocity` | `vector<double>` | Joint velocities (radians/second) |
| `current` | `vector<double>` | Motor currents (amperes) |
| `temperature` | `vector<double>` | Motor temperatures (Celsius) |
| `fingertip_sensor` | `vector<double>` | F/T sensor data (N fingers x 6 values) |
| `gpio` | `vector<bool>` | GPIO states (3 outputs + 1 input) |
| `tactile_m` | `vector<vector<uint8_t>>` | Tactile M sensor data |
| `tactile_s` | `vector<vector<uint16_t>>` | Tactile S sensor data |

Only the sensor field matching the detected `SensorType` is populated. For example, if the gripper has `FT_6AXIS` sensors, the `fingertip_sensor` field contains data and the `tactile_m` and `tactile_s` fields are empty.
