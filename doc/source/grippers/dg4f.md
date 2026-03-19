# DG-4F

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

- `dg4f_description` — URDF model
- `dg4f_driver` — Launch files and configuration
- `dg4f_gz` — Gazebo simulation

## Launch

```bash
ros2 launch dg4f_driver dg4f_driver.launch.py
```

## Note

DG-4F motor direction is always standard (no firmware version dependency).
