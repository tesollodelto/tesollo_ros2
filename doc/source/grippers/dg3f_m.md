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
| `dg3f_m_description` | URDF/xacro model, meshes, and RViz display launch |
| `dg3f_m_driver` | ros2_control hardware driver and controller launch files |
| `dg3f_m_gz` | Gazebo simulation |

## Launch

```bash
# RViz display
ros2 launch dg3f_m_description dg3f_m_display.launch.py

# Hardware driver
ros2 launch dg3f_m_driver dg3f_m_driver.launch.py

# Effort controller
ros2 launch dg3f_m_driver dg3f_m_effort_controller.launch.py

# Gazebo simulation
ros2 launch dg3f_m_gz dg3f_m_gz.launch.py
```

## Firmware Note

Motor direction was revised in firmware **v2.8+**. The hardware interface automatically detects the firmware version and applies the correct motor direction mapping.
