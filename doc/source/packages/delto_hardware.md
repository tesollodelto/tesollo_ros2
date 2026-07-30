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
| `delto_model` | Model ID. Determines actuator count, frame layout and motor direction | `0x5F12` (DG-5F-L) |
| `hand_type` | `left` or `right` (only for DG-5F and DG-5F-S) | `left` |
| `fingertip_sensor` | Enable F/T sensor data reading | `false` |
| `IO` | Enable GPIO reading and control | `false` |

```{warning}
`delto_model` is **not** auto-detected -- it must match your gripper. The driver
reads the Product ID from the device only to cross-check it, and refuses to
start if the configured and reported models imply different actuator counts.
Omitting the parameter silently selects the DG-5F-L default.
```

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

- If the TCP connection to the gripper is lost (cable disconnected, gripper rebooted, etc.), a background thread retries indefinitely, starting at 1-second intervals and backing off to a maximum of 8 seconds.
- During reconnection, the `read()` and `write()` functions return OK with the last known data. This prevents ros2_control from deactivating the hardware interface during temporary disconnections.
- Because the feedback is frozen while the link is down, the current-control integrator is held at zero for the whole outage, so nothing accumulates to be dumped into the motors when the link returns.
- On a successful reconnect the driver sends a zero-duty frame and clears the integrator before resuming normal writes.
- You can monitor the connection status programmatically using the `system/connection_status` state interface:
  - `1.0` = connected
  - `0.0` = disconnected

```{warning}
The gripper firmware keeps applying the last PWM duty it received. While the link
is down the motors therefore stay energized at the pre-disconnect torque -- the
driver cannot change that, because the socket it would send zero duty on is the
one that went away. Treat a `connection_status` of `0.0` as "the hand is still
holding whatever it was holding", not as "the hand is safe".
```

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
| `effort` | Joint | Measured motor current in **milliamperes** -- see the note below |
| `temperature` | Joint | Motor temperature in degrees Celsius |
| `force.x`, `force.y`, `force.z` | Finger | Fingertip force (if `fingertip_sensor` enabled) |
| `torque.x`, `torque.y`, `torque.z` | Finger | Fingertip torque (if `fingertip_sensor` enabled) |
| `gpio/output_1`, `gpio/output_2`, `gpio/output_3` | System | GPIO output states (if `IO` enabled) |

```{warning}
The `effort` interfaces carry **motor current in mA**, not torque in N·m, even
though `hardware_interface::HW_IF_EFFORT` is defined as N or N·m. A generic
effort controller will therefore misread the value by roughly three orders of
magnitude and in the wrong dimension. Use it only with the Delto-specific
controllers in this repository, or convert with
`delto_gripper_helper::ConvertEffort()`. The same applies to the `effort`
command interface, which the driver feeds into its current-control loop rather
than treating as a torque.
```
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

Replace `<ns>` with your gripper's namespace (e.g., `dg5f_right`, `dg4f`, `dg3f_m`, `dg5f_s_right`).

All services are advertised under the hardware interface node `delto_hardware_interface_node`.

`/<ns>/delto_hardware_interface_node/set_ft_sensor_offset`
: **Type:** `std_srvs/srv/Trigger`
: Zero-calibrate all fingertip F/T sensors. Call when nothing is touching the fingertips to set the current readings as zero. Available only when the gripper carries an F/T sensor and `fingertip_sensor:=true`.

`/<ns>/delto_hardware_interface_node/set_gpio_output1`
: **Type:** `std_srvs/srv/SetBool`
: Set GPIO output 1 (`true` = high, `false` = low). Available only when `IO:=true` on a model that supports GPIO.

`/<ns>/delto_hardware_interface_node/set_gpio_output2`
: **Type:** `std_srvs/srv/SetBool`
: Set GPIO output 2.

`/<ns>/delto_hardware_interface_node/set_gpio_output3`
: **Type:** `std_srvs/srv/SetBool`
: Set GPIO output 3.

**Example -- calibrate F/T sensors:**

```bash
ros2 service call /dg5f_right/delto_hardware_interface_node/set_ft_sensor_offset std_srvs/srv/Trigger
```

**Example -- set GPIO output 1 to high:**

```bash
ros2 service call /dg5f_right/delto_hardware_interface_node/set_gpio_output1 std_srvs/srv/SetBool "{data: true}"
```

## Published Topics

When the gripper carries a tactile sensor and `fingertip_sensor:=true`, the hardware interface publishes one `sensor_msgs/msg/Image` per finger, named `tactile/finger_N` where `N` runs from `1` to the gripper's finger count. The topics are advertised on the controller-manager namespace (no node-name prefix), so the full path is `/<ns>/tactile/finger_N`.

```{list-table}
:header-rows: 1
:widths: 30 22 18 15 15

* - Topic
  - Type
  - Encoding
  - Resolution
  - Sensor type
* - `/<ns>/tactile/finger_N`
  - `sensor_msgs/msg/Image`
  - `mono8`
  - 3 x 5
  - TACTILE_M (DG-3F-M, DG-4F, DG-5F)
* - `/<ns>/tactile/finger_N`
  - `sensor_msgs/msg/Image`
  - `mono16` (little-endian)
  - 3 x 6
  - TACTILE_S (DG-5F-S, DG-5F-S15)
```

```{note}
The image encoding is selected automatically from the sensor type reported by the gripper firmware. F/T-only models (no tactile pad) do not publish these topics.
```

**Example -- view a tactile pad in `rqt_image_view`:**

```bash
ros2 run rqt_image_view rqt_image_view /dg5f_right/tactile/finger_1
```

## Supported Models

The hardware interface supports all Delto gripper models through a unified codebase:

| Model ID | Model Name | Motors | Bytes/Motor | GPIO | Sensor |
|----------|------------|--------|-------------|------|--------|
| `0x3F01` | ~~DG-3F-B~~ | 12 | 5 | Yes | -- |
| `0x3F02` | DG-3F-M | 12 | 8 | Yes | F/T, TACTILE_M |
| `0x4F02` | DG-4F | 18 | 8 | Yes | F/T, TACTILE_M |
| `0x5F12` | DG-5F-L | 20 | 8 | Yes | F/T, TACTILE_M |
| `0x5F22` | DG-5F-R | 20 | 8 | Yes | F/T, TACTILE_M |
| `0x5F14` | DG-5F-S-L | 20 | 8 | -- | F/T, TACTILE_S |
| `0x5F24` | DG-5F-S-R | 20 | 8 | -- | F/T, TACTILE_S |
| `0x5F34` | DG-5F-S15-L | 15 | 8 | -- | TACTILE_S |
| `0x5F44` | DG-5F-S15-R | 15 | 8 | -- | TACTILE_S |

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
