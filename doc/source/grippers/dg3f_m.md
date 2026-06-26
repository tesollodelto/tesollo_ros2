# DG-3F (M Type)

[![CI](https://github.com/tesollodelto/dg3f_m_ros2/actions/workflows/ci.yml/badge.svg)](https://github.com/tesollodelto/dg3f_m_ros2/actions/workflows/ci.yml)

3-finger gripper with fingertip force/torque sensors.

## Specifications

| Parameter | Value |
|-----------|-------|
| Model ID | `0x3F02` |
| Fingers | 3 |
| Joints | 12 (4 per finger) |
| F/T Sensor | Yes |
| GPIO | Yes (3 outputs, 1 input) |

## Packages

| Package | Description |
|---------|-------------|
| `dg3f_m_description` | URDF/xacro model, meshes, and RViz display launch files |
| `dg3f_m_driver` | ros2_control hardware driver, controller configs, and launch files |
| `dg3f_m_gz` | Gazebo simulation launch files |
| `dg3f_m_moveit_config` | MoveIt 2 configuration (SRDF, planners, mock hardware) |

## Joint Naming Convention

Each joint is named `j_dg_F_J` where:

- `F` = finger number (1-3)
- `J` = joint number within the finger (1 = base, 4 = tip)

For example, `j_dg_2_3` is finger 2, joint 3.

## Before You Control: Developer Mode

```{important}
The ros2_control driver speaks a custom protocol over Ethernet that is only available in **Developer Mode**. On the DG-3F button panel, set ② **Developer Mode** and ④ **EtherNET** before launching the driver, otherwise the connection will fail. See the `dg3f_m_driver` README for the switch/LED panel image.
```

## Launch Files

### Driver Launch Files (Hardware Connection)

| Launch File | Controller Type | Description |
|-------------|----------------|-------------|
| `dg3f_m_driver.launch.py` | Joint Trajectory Controller | Default driver with JTC (position control with PID) |
| `dg3f_m_effort_controller.launch.py` | JointGroupEffortController | Direct effort (current) control |
| `dg3f_m_pid_controller.launch.py` | PidController (per joint) | Individual PID controllers, one per joint (`<joint>_pospid`) |
| `dg3f_m_pid_all_controller.launch.py` | PidController (grouped) | Single PID controller for all joints (`j_dg_pospid`) |

### Visualization and Simulation

| Launch File | Package | Description |
|-------------|---------|-------------|
| `dg3f_m_display.launch.py` | `dg3f_m_description` | RViz visualization with joint sliders |
| `dg3f_m_gz.launch.py` | `dg3f_m_gz` | Gazebo simulation |

## Launch Arguments

All driver launch files accept these arguments:

| Argument | Default | Description |
|----------|---------|-------------|
| `delto_ip` | `169.254.186.72` | IP address of the gripper |
| `delto_port` | `502` | TCP port of the gripper |
| `fingertip_sensor` | `false` | Enable reading F/T sensor data from the hardware |
| `ft_broadcaster` | `false` | Enable broadcasting F/T data as ROS 2 topics |
| `io` | `false` | Enable GPIO interfaces |

Example:

```bash
ros2 launch dg3f_m_driver dg3f_m_driver.launch.py delto_ip:=192.168.1.100 fingertip_sensor:=true ft_broadcaster:=true
```

## Controller Types

### Joint Trajectory Controller (JTC) -- Default

```bash
ros2 launch dg3f_m_driver dg3f_m_driver.launch.py
```

**Send a position command:**

```bash
ros2 action send_goal /dg3f_m/dg3f_m_controller/follow_joint_trajectory control_msgs/action/FollowJointTrajectory "{
  trajectory: {
    joint_names: [j_dg_1_1, j_dg_1_2, j_dg_1_3, j_dg_1_4],
    points: [
      {positions: [0.5, 0.5, 0.5, 0.5], time_from_start: {sec: 2, nanosec: 0}}
    ]
  }
}"
```

```{note}
The JTC supports `allow_partial_joints_goal: true`, so you can command a subset of joints without specifying all 12.
```

**Config file:** `config/dg3f_m_controller.yaml`

**Key config values:**
- Update rate: 100 Hz
- PID gains: `p=1.5, i=0.0, d=0.0` (per joint)

### Effort Controller (Direct Current)

```bash
ros2 launch dg3f_m_driver dg3f_m_effort_controller.launch.py
```

**Send effort commands (12 values, one per joint):**

```bash
ros2 topic pub /dg3f_m/effort_controller/commands std_msgs/msg/Float64MultiArray "{data: [0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0]}" --once
```

```{warning}
The effort controller has no position feedback loop. Start with small values to avoid damage.
```

### PID Controller (Per-Joint)

```bash
ros2 launch dg3f_m_driver dg3f_m_pid_controller.launch.py
```

**Send a position reference to a single joint:**

```bash
ros2 topic pub /dg3f_m/j_dg_1_1_pospid/reference std_msgs/msg/Float64 "{data: 0.5}" --once
```

Each joint has its own topic: `/dg3f_m/j_dg_X_Y_pospid/reference`.

## MoveIt Integration

The `dg3f_m_moveit_config` package provides MoveIt 2 motion planning for the DG-3F-M with mock hardware support.

```{raw} html
<video width="100%" autoplay loop muted playsinline>
  <source src="../_static/dg3f_m_moveit.webm" type="video/webm">
</video>
```

### Mock Hardware (No Device Required)

```bash
ros2 launch dg3f_m_moveit_config dg3f_m_moveit.launch.py use_mock:=true
```

### Real Hardware with MoveIt

```bash
ros2 launch dg3f_m_moveit_config dg3f_m_moveit.launch.py use_mock:=false delto_ip:=169.254.186.72
```

### Planning Groups

| Group | Joints |
|-------|--------|
| `finger_1` | `j_dg_1_1` ~ `j_dg_1_4` (chain to `l_dg_1_tip`) |
| `finger_2` | `j_dg_2_1` ~ `j_dg_2_4` (chain to `l_dg_2_tip`) |
| `finger_3` | `j_dg_3_1` ~ `j_dg_3_4` (chain to `l_dg_3_tip`) |
| `all_fingers` | All 12 joints |

## F/T Sensor Setup

The DG-3F-M has 3 fingertip F/T sensors. To enable them:

```bash
ros2 launch dg3f_m_driver dg3f_m_driver.launch.py fingertip_sensor:=true ft_broadcaster:=true
```

```{warning}
Both `fingertip_sensor:=true` AND `ft_broadcaster:=true` must be set. `fingertip_sensor` enables reading the sensor data from the hardware; `ft_broadcaster` starts the ROS 2 broadcasters that publish it as topics.
```

Check the F/T topics:

```bash
ros2 topic list | grep fingertip
```

```
/dg3f_m/fingertip_1_broadcaster/wrench
/dg3f_m/fingertip_2_broadcaster/wrench
/dg3f_m/fingertip_3_broadcaster/wrench
```

Calibrate the sensors:

```bash
ros2 service call /dg3f_m/delto_hardware_interface_node/set_ft_sensor_offset std_srvs/srv/Trigger
```

## Topics Published

Namespace: `/dg3f_m/`

| Topic | Type | Description |
|-------|------|-------------|
| `joint_states` | `sensor_msgs/msg/JointState` | Position, velocity, and effort for all 12 joints |
| `robot_description` | `std_msgs/msg/String` | URDF robot description |
| `dg3f_m_controller/state` | `control_msgs/msg/JointTrajectoryControllerState` | Controller state (JTC only) |
| `fingertip_N_broadcaster/wrench` | `geometry_msgs/msg/WrenchStamped` | F/T sensor data for finger N (if enabled) |

## Services

Replace `<ns>` with `dg3f_m`.

`/<ns>/delto_hardware_interface_node/set_ft_sensor_offset`
: **Type:** `std_srvs/srv/Trigger`
: Zero-calibrate all fingertip F/T sensors. Call when nothing is touching the fingertips.

`/<ns>/delto_hardware_interface_node/set_gpio_output1`
: **Type:** `std_srvs/srv/SetBool`
: Set GPIO output 1 (`true` = high, `false` = low)

`/<ns>/delto_hardware_interface_node/set_gpio_output2`
: **Type:** `std_srvs/srv/SetBool`
: Set GPIO output 2

`/<ns>/delto_hardware_interface_node/set_gpio_output3`
: **Type:** `std_srvs/srv/SetBool`
: Set GPIO output 3

## Verifying the Connection

```bash
# Check hardware interfaces
ros2 control list_hardware_interfaces

# Check active controllers
ros2 control list_controllers

# Read joint states
ros2 topic echo /dg3f_m/joint_states --once
```

## Firmware Note

Motor direction was revised in firmware **v2.8+**. The hardware interface automatically detects the firmware version and applies the correct motor direction mapping. No manual configuration is required.
