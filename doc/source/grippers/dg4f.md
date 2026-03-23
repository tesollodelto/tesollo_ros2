# DG-4F

[![CI](https://github.com/tesollodelto/dg4f_ros2/actions/workflows/ci.yml/badge.svg)](https://github.com/tesollodelto/dg4f_ros2/actions/workflows/ci.yml)

4-finger gripper with fingertip force/torque sensors.

## Specifications

| Parameter | Value |
|-----------|-------|
| Model ID | `0x4F02` |
| Fingers | 4 |
| Joints | 18 |
| F/T Sensor | Yes |
| GPIO | Yes (3 outputs, 1 input) |

## Packages

| Package | Description |
|---------|-------------|
| `dg4f_description` | URDF/xacro model, meshes, and RViz display launch |
| `dg4f_driver` | ros2_control hardware driver and controller launch files |
| `dg4f_gz` | Gazebo simulation |

## Launch

```bash
# RViz display
ros2 launch dg4f_description dg4f_display.launch.py

# Hardware driver
ros2 launch dg4f_driver dg4f_driver.launch.py

# Effort controller
ros2 launch dg4f_driver dg4f_effort_controller.launch.py

# Gazebo simulation
ros2 launch dg4f_gz dg4f_gz.launch.py
```

## Note

DG-4F motor direction is always standard (no firmware version dependency).
