# delto_hardware

[![CI](https://github.com/tesollodelto/dg_hardware/actions/workflows/ci.yml/badge.svg)](https://github.com/tesollodelto/dg_hardware/actions/workflows/ci.yml)

Unified `ros2_control` hardware interface for all Delto gripper models.

```{note}
**What is ros2_control?** `ros2_control` is a ROS 2 framework that provides a standardized way to interface with robot hardware. It separates the **hardware driver** (how to talk to the motors) from the **controller** (what commands to send). This means you can switch between different control strategies (position control, effort control, etc.) without changing the hardware driver code. Think of it as a universal adapter between your software and the gripper hardware.
```

## Overview

The `delto_hardware` package provides a single `SystemInterface` plugin called `delto_hardware/SystemInterface`. This one plugin supports **all** Delto gripper models (DG-3F-B, DG-3F-M, DG-4F, DG-5F, DG-5F-S, DG-5F-S15) through model-aware configuration. The model is automatically detected from the gripper firmware when the connection is established.

### How It Fits in the System

```
Your Application
    |
    v
ros2_control Controller  (e.g., JointTrajectoryController)
    |
    v
delto_hardware/SystemInterface  <-- this package
    |
    v
delto_tcp_comm  (TCP communication library)
    |
    v
Gripper Firmware  (physical hardware)
```

The hardware interface reads sensor data from the gripper (position, velocity, current, temperature, F/T sensors, GPIO) and sends motor current commands back to the gripper. Controllers sit on top of this interface and translate your high-level commands (e.g., "move to position 0.5 radians") into the low-level effort commands that the hardware interface sends to the gripper.

## Hardware Parameters (URDF/XACRO)

These parameters are configured in the URDF/XACRO file that describes the gripper hardware. They are passed to the hardware interface when it initializes.

| Parameter | Description | Default |
|-----------|-------------|---------|
| `delto_ip` | IP address of the gripper on the network | `169.254.186.72` |
| `delto_port` | TCP port the gripper listens on | `502` |
| `delto_model` | Model ID (hexadecimal). Determines joint count and motor direction | Auto-detected from firmware |
| `hand_type` | `left` or `right` (only for DG-5F and DG-5F-S) | `left` |
| `fingertip_sensor` | Enable F/T sensor data reading | `false` |
| `IO` | Enable GPIO reading and control | `false` |

In the XACRO file, these appear like this:

```xml
<ros2_control name="DeltoGripperSystem" type="system">
  <hardware>
    <plugin>delto_hardware/SystemInterface</plugin>
    <param name="delto_ip">169.254.186.72</param>
    <param name="delto_port">502</param>
    <param name="fingertip_sensor">false</param>
    <param name="IO">false</param>
  </hardware>
  <!-- joint definitions ... -->
</ros2_control>
```

You typically do not need to edit these files directly. Instead, pass values as launch arguments (e.g., `delto_ip:=192.168.1.100`), and the launch file forwards them to the XACRO processor.

## Connection Behavior

The hardware interface implements **automatic background reconnection**:

- If the TCP connection to the gripper is lost (cable disconnected, gripper rebooted, etc.), a background thread attempts to reconnect at 1-second intervals, for up to 10 retries.
- During reconnection, the `read()` and `write()` functions return OK with the last known data. This prevents ros2_control from deactivating the hardware interface during temporary disconnections.
- You can monitor the connection status programmatically using the `system/connection_status` state interface:
  - `1.0` = connected
  - `0.0` = disconnected

To check the connection status from the command line:

```bash
ros2 control list_hardware_interfaces | grep connection_status
```

## State Interfaces (Data You Can Read)

State interfaces are read-only data published by the hardware interface. Controllers and broadcasters use these to report the gripper's current state.

| Interface | Per | Description |
|-----------|-----|-------------|
| `position` | Joint | Current joint position in radians |
| `velocity` | Joint | Current joint velocity in radians/second |
| `effort` | Joint | Current motor current in amperes |
| `temperature` | Joint | Motor temperature in degrees Celsius |
| `force.x`, `force.y`, `force.z` | Finger | Fingertip force (if `fingertip_sensor` enabled) |
| `torque.x`, `torque.y`, `torque.z` | Finger | Fingertip torque (if `fingertip_sensor` enabled) |
| `gpio/output_1`, `gpio/output_2`, `gpio/output_3` | System | GPIO output states (if `IO` enabled) |
| `gpio/input_1` | System | GPIO input state (if `IO` enabled) |
| `system/connection_status` | System | TCP connection status (1.0 = connected, 0.0 = disconnected) |

## Command Interfaces (Data You Can Write)

| Interface | Per | Description |
|-----------|-----|-------------|
| `effort` | Joint | Motor current command. This is the only command interface -- all controllers ultimately output effort commands |

```{note}
**Why effort only?** The Delto gripper firmware accepts motor current (effort) commands directly. Position control is implemented by the ros2_control controllers (like JointTrajectoryController) which read the current position, compute the error, and output an effort command through a PID loop. This design gives you maximum flexibility to choose or implement your own control strategy.
```

## Services

The hardware interface creates several ROS 2 services for calibration and GPIO control. The service names include the gripper's namespace.

| Service | Type | Description |
|---------|------|-------------|
| `/<ns>/delto_hardware_interface_node/set_ft_sensor_offset` | `std_srvs/srv/Trigger` | Zero-calibrate all fingertip F/T sensors. Call this when nothing is touching the fingertips to set the current readings as zero |
| `/<ns>/set_gpio_output1` | `std_srvs/srv/SetBool` | Set GPIO output 1 (true = high, false = low) |
| `/<ns>/set_gpio_output2` | `std_srvs/srv/SetBool` | Set GPIO output 2 |
| `/<ns>/set_gpio_output3` | `std_srvs/srv/SetBool` | Set GPIO output 3 |

Replace `<ns>` with your gripper's namespace (e.g., `dg5f_right`, `dg4f`, `dg3f_m`, `dg5f_s_right`).

**Example -- calibrate F/T sensors:**

```bash
ros2 service call /dg5f_right/delto_hardware_interface_node/set_ft_sensor_offset std_srvs/srv/Trigger
```

**Example -- set GPIO output 1 to high:**

```bash
ros2 service call /dg5f_right/set_gpio_output1 std_srvs/srv/SetBool "{data: true}"
```

## Supported Models

The hardware interface supports all Delto gripper models through a unified codebase:

| Model | ID | Fingers | Joints | F/T Sensor | Motor Direction |
|-------|----|---------|--------|------------|-----------------|
| ~~DG-3F-B~~ | `0x3F01` | 3 | 12 | No | Firmware-dependent (v3.6+) |
| DG-3F-M | `0x3F02` | 3 | 12 | Yes | Firmware-dependent (v2.8+) |
| DG-4F | `0x4F02` | 4 | 18 | Yes | Always standard |
| DG-5F-L | `0x5F12` | 5 | 20 | Yes | Firmware-dependent (v2.8+) |
| DG-5F-R | `0x5F22` | 5 | 20 | Yes | Firmware-dependent (v2.8+) |
| DG-5F-S-L | `0x5F14` | 5 | 20 | Yes | Always standard |
| DG-5F-S-R | `0x5F24` | 5 | 20 | Yes | Always standard |
| DG-5F-S15-L | `0x5F34` | 5 | 15 | Yes | Always standard |
| DG-5F-S15-R | `0x5F44` | 5 | 15 | Yes | Always standard |

## Control Flow

For those interested in the internal workings, here is the lifecycle of the hardware interface:

```
on_init()       -> Parse URDF parameters, establish TCP connection, create ROS 2 services
on_activate()   -> Verify connection is active
read()          -> Receive joint/sensor/GPIO data from gripper, process service callbacks
write()         -> Convert controller effort commands to motor duty values, send to gripper
on_deactivate() -> Disconnect from gripper
```

The `read()` and `write()` functions are called by the controller manager at the configured update rate (typically 100-300 Hz).
