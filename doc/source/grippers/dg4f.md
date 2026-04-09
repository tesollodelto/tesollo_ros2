# DG-4F

[![CI](https://github.com/tesollodelto/dg4f_ros2/actions/workflows/ci.yml/badge.svg)](https://github.com/tesollodelto/dg4f_ros2/actions/workflows/ci.yml)

4-finger gripper with fingertip force/torque sensors. This gripper has 4 main fingers with 4 joints each, plus 2 inner joints for a total of 18 joints.

## Specifications

| Parameter | Value |
|-----------|-------|
| Model ID | `0x4F02` |
| Fingers | 4 |
| Joints | 18 (4 per finger + 2 inner joints) |
| F/T Sensor | Yes |
| GPIO | Yes (3 outputs, 1 input) |

## Packages

| Package | Description |
|---------|-------------|
| `dg4f_description` | URDF/xacro model, meshes, and RViz display launch files |
| `dg4f_driver` | ros2_control hardware driver, controller configs, and launch files |
| `dg4f_gz` | Gazebo simulation launch files |
| `dg4f_moveit_config` | MoveIt 2 configuration (SRDF, planners, mock hardware) |

## Joint Naming Convention

Each joint is named `j_dg_F_J` where:

- `F` = finger number (1-4)
- `J` = joint number within the finger (1 = base, 4 = tip)

Additionally, there are two inner joints: `j_dg_1_inner` and `j_dg_4_inner`.

## Launch Files

### Driver Launch Files (Hardware Connection)

| Launch File | Controller Type | Description |
|-------------|----------------|-------------|
| `dg4f_driver.launch.py` | Joint Trajectory Controller | Default driver with JTC (position control with PID) |
| `dg4f_effort_controller.launch.py` | JointGroupEffortController | Direct effort (current) control |

### Visualization and Simulation

| Launch File | Package | Description |
|-------------|---------|-------------|
| `dg4f_display.launch.py` | `dg4f_description` | RViz visualization with joint sliders |
| `dg4f_gz.launch.py` | `dg4f_gz` | Gazebo simulation |

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
ros2 launch dg4f_driver dg4f_driver.launch.py delto_ip:=192.168.1.100 fingertip_sensor:=true ft_broadcaster:=true
```

## Controller Types

### Joint Trajectory Controller (JTC) -- Default

```bash
ros2 launch dg4f_driver dg4f_driver.launch.py
```

The JTC accepts trajectory goals and uses a PID loop to move joints to target positions.

**Send a position command:**

```bash
ros2 action send_goal /dg4f/dg4f_controller/follow_joint_trajectory control_msgs/action/FollowJointTrajectory "{
  trajectory: {
    joint_names: [j_dg_1_1, j_dg_1_2, j_dg_1_3, j_dg_1_4],
    points: [
      {positions: [0.5, 0.5, 0.5, 0.5], time_from_start: {sec: 2, nanosec: 0}}
    ]
  }
}"
```

```{note}
The DG-4F JTC supports `allow_partial_joints_goal: true`, so you can command a subset of joints without specifying all 18.
```

**Config file:** `config/dg4f_controller.yaml`

**Key config values:**
- Update rate: 300 Hz
- PID gains: `p=1.5, i=0.0, d=0.0` (per joint)

### Effort Controller (Direct Current)

```bash
ros2 launch dg4f_driver dg4f_effort_controller.launch.py
```

**Send effort commands (18 values, one per joint):**

```bash
ros2 topic pub /dg4f/effort_controller/commands std_msgs/msg/Float64MultiArray "{data: [0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0]}" --once
```

```{warning}
The effort controller has no position feedback loop. Start with small values to avoid damage.
```

## MoveIt Integration

The `dg4f_moveit_config` package provides MoveIt 2 motion planning for the DG-4F with mock hardware support.

### Mock Hardware (No Device Required)

```bash
ros2 launch dg4f_moveit_config dg4f_moveit.launch.py use_mock:=true
```

### Real Hardware with MoveIt

```bash
ros2 launch dg4f_moveit_config dg4f_moveit.launch.py use_mock:=false delto_ip:=169.254.186.72
```

### Planning Groups

| Group | Joints |
|-------|--------|
| `finger_1` | `j_dg_1_1` ~ `j_dg_1_4` + `j_dg_1_inner` (chain to `l_dg_1_tip`) |
| `finger_2` | `j_dg_2_1` ~ `j_dg_2_4` (chain to `l_dg_2_tip`) |
| `finger_3` | `j_dg_3_1` ~ `j_dg_3_4` (chain to `l_dg_3_tip`) |
| `finger_4` | `j_dg_4_1` ~ `j_dg_4_4` + `j_dg_4_inner` (chain to `l_dg_4_tip`) |
| `all_fingers` | All 18 joints |

## F/T Sensor Setup

The DG-4F has 4 fingertip F/T sensors. To enable them:

```bash
ros2 launch dg4f_driver dg4f_driver.launch.py fingertip_sensor:=true ft_broadcaster:=true
```

Check the F/T topics:

```bash
ros2 topic list | grep fingertip
```

```
/dg4f/fingertip_1_broadcaster/wrench
/dg4f/fingertip_2_broadcaster/wrench
/dg4f/fingertip_3_broadcaster/wrench
/dg4f/fingertip_4_broadcaster/wrench
```

Calibrate the sensors:

```bash
ros2 service call /dg4f/delto_hardware_interface_node/set_ft_sensor_offset std_srvs/srv/Trigger
```

## Topics Published

Namespace: `/dg4f/`

| Topic | Type | Description |
|-------|------|-------------|
| `joint_states` | `sensor_msgs/msg/JointState` | Position, velocity, and effort for all 18 joints |
| `robot_description` | `std_msgs/msg/String` | URDF robot description |
| `dg4f_controller/state` | `control_msgs/msg/JointTrajectoryControllerState` | Controller state (JTC only) |
| `fingertip_N_broadcaster/wrench` | `geometry_msgs/msg/WrenchStamped` | F/T sensor data for finger N (if enabled) |

## Services

| Service | Type | Description |
|---------|------|-------------|
| `/dg4f/delto_hardware_interface_node/set_ft_sensor_offset` | `std_srvs/srv/Trigger` | Zero-calibrate all F/T sensors |
| `/dg4f/set_gpio_output1` | `std_srvs/srv/SetBool` | Set GPIO output 1 |
| `/dg4f/set_gpio_output2` | `std_srvs/srv/SetBool` | Set GPIO output 2 |
| `/dg4f/set_gpio_output3` | `std_srvs/srv/SetBool` | Set GPIO output 3 |

## Verifying the Connection

```bash
# Check hardware interfaces
ros2 control list_hardware_interfaces

# Check active controllers
ros2 control list_controllers

# Read joint states
ros2 topic echo /dg4f/joint_states --once
```

## Note

DG-4F motor direction is always standard (no firmware version dependency). No special configuration is needed regardless of your firmware version.
