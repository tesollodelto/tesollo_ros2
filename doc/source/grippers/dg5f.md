# DG-5F

5-finger robotic hand with fingertip force/torque sensors. Available in left and right hand variants.

## Specifications

| Parameter | Value |
|-----------|-------|
| Model ID (Left) | `0x5F12` |
| Model ID (Right) | `0x5F22` |
| Fingers | 5 |
| Joints | 20 (4 per finger) |
| F/T Sensor | Yes |
| GPIO | Yes (3 outputs, 1 input) |

## Packages

- `dg5f_description` — URDF model
- `dg5f_driver` — Launch files and configuration
- `dg5f_gz` — Gazebo simulation

## Launch

```bash
# Left hand
ros2 launch dg5f_driver dg5f_left_driver.launch.py

# Right hand
ros2 launch dg5f_driver dg5f_right_driver.launch.py
```

## Hand Type

Left and right hands have different motor direction arrays and separate launch files.

## Firmware Note

Motor direction was revised in firmware **v2.8+**.
