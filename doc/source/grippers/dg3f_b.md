# DG-3F (B Type)

[![CI](https://github.com/tesollodelto/dg3f_b_ros2/actions/workflows/ci.yml/badge.svg)](https://github.com/tesollodelto/dg3f_b_ros2/actions/workflows/ci.yml)

```{warning}
**This model has been discontinued.** It is no longer manufactured or sold, but the ROS 2 driver packages are still maintained for existing users. If you are purchasing a new gripper, consider the {doc}`dg3f_m` (M type) instead, which includes fingertip force/torque sensors.
```

3-finger gripper, basic model. This was the original 3-finger design without fingertip sensors.

## Specifications

| Parameter | Value |
|-----------|-------|
| Model ID | `0x3F01` |
| Fingers | 3 |
| Joints | 12 (4 per finger) |
| F/T Sensor | No |
| GPIO | Yes (3 outputs, 1 input) |

## Packages

| Package | Description |
|---------|-------------|
| `dg3f_b_description` | URDF/xacro model and meshes |
| `dg3f_b_driver` | ros2_control hardware driver and launch files |

## Launch

```bash
ros2 launch dg3f_b_driver dg3f_b_driver.launch.py
```

A multi-gripper launch file is also available:

```bash
ros2 launch dg3f_b_driver dg3f_b_driver_multi.launch.py
```

## Firmware Note

Motor direction was revised in firmware **v3.6+**. If your firmware is older, the hardware interface automatically detects this and applies the legacy motor direction mapping. No manual configuration is required.
