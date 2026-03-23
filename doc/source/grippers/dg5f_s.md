# DG-5F-S

[![CI](https://github.com/tesollodelto/dg5f_s_ros2/actions/workflows/ci.yml/badge.svg)](https://github.com/tesollodelto/dg5f_s_ros2/actions/workflows/ci.yml)

5-finger robotic hand, compact version with fingertip force/torque sensors. Available in left and right hand variants.

Supports both **20-DOF** and **15-DOF** configurations.

## Specifications

### 20-DOF

| Parameter | Value |
|-----------|-------|
| Model ID (Left) | `0x5F14` |
| Model ID (Right) | `0x5F24` |
| Fingers | 5 |
| Joints | 20 (4 per finger) |
| F/T Sensor | Yes |
| GPIO | Yes (3 outputs, 1 input) |

### 15-DOF

| Parameter | Value |
|-----------|-------|
| Model ID (Left) | `0x5F34` |
| Model ID (Right) | `0x5F44` |
| Fingers | 5 |
| Joints | 15 (3 per finger) |
| F/T Sensor | Yes |
| GPIO | Yes (3 outputs, 1 input) |

## Packages

- `dg5f_s_description` -- URDF model, meshes, RViz display launch
- `dg5f_s_driver` -- ros2_control driver and controller launch files
- `dg5f_s_gz` -- Gazebo simulation

## Launch

### 20-DOF

```bash
# Hardware driver
ros2 launch dg5f_s_driver dg5f_s_right_driver.launch.py
ros2 launch dg5f_s_driver dg5f_s_left_driver.launch.py

# Effort controller
ros2 launch dg5f_s_driver dg5f_s_right_effort_controller.launch.py
ros2 launch dg5f_s_driver dg5f_s_left_effort_controller.launch.py

# RViz display
ros2 launch dg5f_s_description dg5f_s_right_display.launch.py
ros2 launch dg5f_s_description dg5f_s_left_display.launch.py

# Gazebo simulation
ros2 launch dg5f_s_gz dg5f_s_right_gz.launch.py
ros2 launch dg5f_s_gz dg5f_s_left_gz.launch.py
```

### 15-DOF

```bash
# Hardware driver
ros2 launch dg5f_s_driver dg5f_s_15dof_right_driver.launch.py
ros2 launch dg5f_s_driver dg5f_s_15dof_left_driver.launch.py

# Effort controller
ros2 launch dg5f_s_driver dg5f_s_15dof_right_effort_controller.launch.py
ros2 launch dg5f_s_driver dg5f_s_15dof_left_effort_controller.launch.py

# RViz display
ros2 launch dg5f_s_description dg5f_s_15dof_right_display.launch.py
ros2 launch dg5f_s_description dg5f_s_15dof_left_display.launch.py

# Gazebo simulation
ros2 launch dg5f_s_gz dg5f_s_15dof_right_gz.launch.py
ros2 launch dg5f_s_gz dg5f_s_15dof_left_gz.launch.py
```

## Note

DG-5F-S motor direction is always standard (no firmware version dependency).
