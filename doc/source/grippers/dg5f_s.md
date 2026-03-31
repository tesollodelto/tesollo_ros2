# DG-5F-S

[![CI](https://github.com/tesollodelto/dg5f_s_ros2/actions/workflows/ci.yml/badge.svg)](https://github.com/tesollodelto/dg5f_s_ros2/actions/workflows/ci.yml)

5-finger robotic hand, small version with fingertip force/torque sensors. Available in **left** and **right** hand variants, with both **20-DOF** and **15-DOF** configurations.

```{note}
**What is DOF?** DOF stands for "Degrees of Freedom" -- the number of independently controllable joints. The 20-DOF version has 4 joints per finger (like the DG-5F), while the 15-DOF version has 3 joints per finger.
```

## Specifications

### 20-DOF Configuration

| Parameter | Value |
|-----------|-------|
| Model ID (Left) | `0x5F14` |
| Model ID (Right) | `0x5F24` |
| Fingers | 5 |
| Joints | 20 (4 per finger) |
| F/T Sensor | Yes |
| GPIO | Yes (3 outputs, 1 input) |

### 15-DOF Configuration

| Parameter | Value |
|-----------|-------|
| Model ID (Left) | `0x5F34` |
| Model ID (Right) | `0x5F44` |
| Fingers | 5 |
| Joints | 15 (3 per finger) |
| F/T Sensor | Yes |
| GPIO | Yes (3 outputs, 1 input) |

## Packages

| Package | Description |
|---------|-------------|
| `dg5f_s_description` | URDF/xacro model, meshes, and RViz display launch files |
| `dg5f_s_driver` | ros2_control hardware driver, controller configs, and launch files |
| `dg5f_s_gz` | Gazebo simulation launch files |

## Launch Files

### 20-DOF Driver Launch Files

| Launch File | Controller Type | Description |
|-------------|----------------|-------------|
| `dg5f_s_right_driver.launch.py` | Joint Trajectory Controller | Right hand with JTC |
| `dg5f_s_left_driver.launch.py` | Joint Trajectory Controller | Left hand with JTC |
| `dg5f_s_right_effort_controller.launch.py` | JointGroupEffortController | Right hand with direct effort control |
| `dg5f_s_left_effort_controller.launch.py` | JointGroupEffortController | Left hand with direct effort control |

### 15-DOF Driver Launch Files

| Launch File | Controller Type | Description |
|-------------|----------------|-------------|
| `dg5f_s_15dof_right_driver.launch.py` | Joint Trajectory Controller | Right hand 15-DOF with JTC |
| `dg5f_s_15dof_left_driver.launch.py` | Joint Trajectory Controller | Left hand 15-DOF with JTC |
| `dg5f_s_15dof_right_effort_controller.launch.py` | JointGroupEffortController | Right hand 15-DOF with effort control |
| `dg5f_s_15dof_left_effort_controller.launch.py` | JointGroupEffortController | Left hand 15-DOF with effort control |

### Visualization and Simulation

| Launch File | Package | Description |
|-------------|---------|-------------|
| `dg5f_s_right_display.launch.py` | `dg5f_s_description` | RViz visualization (right, 20-DOF) |
| `dg5f_s_left_display.launch.py` | `dg5f_s_description` | RViz visualization (left, 20-DOF) |
| `dg5f_s_15dof_right_display.launch.py` | `dg5f_s_description` | RViz visualization (right, 15-DOF) |
| `dg5f_s_15dof_left_display.launch.py` | `dg5f_s_description` | RViz visualization (left, 15-DOF) |
| `dg5f_s_right_gz.launch.py` | `dg5f_s_gz` | Gazebo simulation (right, 20-DOF) |
| `dg5f_s_left_gz.launch.py` | `dg5f_s_gz` | Gazebo simulation (left, 20-DOF) |
| `dg5f_s_15dof_right_gz.launch.py` | `dg5f_s_gz` | Gazebo simulation (right, 15-DOF) |
| `dg5f_s_15dof_left_gz.launch.py` | `dg5f_s_gz` | Gazebo simulation (left, 15-DOF) |

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
ros2 launch dg5f_s_driver dg5f_s_right_driver.launch.py delto_ip:=192.168.1.100 fingertip_sensor:=true ft_broadcaster:=true
```

## Controller Types

### Joint Trajectory Controller (JTC) -- Default

**20-DOF Right Hand:**

```bash
ros2 launch dg5f_s_driver dg5f_s_right_driver.launch.py
```

**15-DOF Right Hand:**

```bash
ros2 launch dg5f_s_driver dg5f_s_15dof_right_driver.launch.py
```

**Send a position command (20-DOF example, namespace `dg5f_s_right`):**

```bash
ros2 action send_goal /dg5f_s_right/dg5f_s_right_controller/follow_joint_trajectory control_msgs/action/FollowJointTrajectory "{
  trajectory: {
    joint_names: [rj_dg_1_1, rj_dg_1_2, rj_dg_1_3, rj_dg_1_4],
    points: [
      {positions: [0.5, 0.5, 0.5, 0.5], time_from_start: {sec: 2, nanosec: 0}}
    ]
  }
}"
```

### Effort Controller (Direct Current)

**20-DOF Right Hand:**

```bash
ros2 launch dg5f_s_driver dg5f_s_right_effort_controller.launch.py
```

**Send effort commands (20 values for 20-DOF, 15 values for 15-DOF):**

```bash
ros2 topic pub /dg5f_s_right/effort_controller/commands std_msgs/msg/Float64MultiArray "{data: [0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0]}" --once
```

## F/T Sensor Setup

The DG-5F-S has 5 fingertip F/T sensors. Enable them with both flags:

```bash
ros2 launch dg5f_s_driver dg5f_s_right_driver.launch.py fingertip_sensor:=true ft_broadcaster:=true
```

Check the F/T topics:

```bash
ros2 topic list | grep fingertip
```

```
/dg5f_s_right/fingertip_1_broadcaster/wrench
/dg5f_s_right/fingertip_2_broadcaster/wrench
/dg5f_s_right/fingertip_3_broadcaster/wrench
/dg5f_s_right/fingertip_4_broadcaster/wrench
/dg5f_s_right/fingertip_5_broadcaster/wrench
```

Calibrate the sensors:

```bash
ros2 service call /dg5f_s_right/delto_hardware_interface_node/set_ft_sensor_offset std_srvs/srv/Trigger
```

## Topics Published

Namespace varies by variant: `/dg5f_s_right/`, `/dg5f_s_left/`

| Topic | Type | Description |
|-------|------|-------------|
| `joint_states` | `sensor_msgs/msg/JointState` | Position, velocity, and effort for all joints |
| `robot_description` | `std_msgs/msg/String` | URDF robot description |
| `fingertip_N_broadcaster/wrench` | `geometry_msgs/msg/WrenchStamped` | F/T sensor data for finger N (if enabled) |

## Services

| Service | Type | Description |
|---------|------|-------------|
| `/<ns>/delto_hardware_interface_node/set_ft_sensor_offset` | `std_srvs/srv/Trigger` | Zero-calibrate all F/T sensors |
| `/<ns>/set_gpio_output1` | `std_srvs/srv/SetBool` | Set GPIO output 1 |
| `/<ns>/set_gpio_output2` | `std_srvs/srv/SetBool` | Set GPIO output 2 |
| `/<ns>/set_gpio_output3` | `std_srvs/srv/SetBool` | Set GPIO output 3 |

Replace `<ns>` with your gripper's namespace (e.g., `dg5f_s_right`, `dg5f_s_left`).

## Verifying the Connection

```bash
# Check hardware interfaces
ros2 control list_hardware_interfaces

# Check active controllers
ros2 control list_controllers

# Read joint states (adjust namespace for your variant)
ros2 topic echo /dg5f_s_right/joint_states --once
```

## Note

DG-5F-S motor direction is always standard (no firmware version dependency). No special configuration is needed regardless of your firmware version.
