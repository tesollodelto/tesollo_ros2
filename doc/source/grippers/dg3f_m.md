# DG-3F (M Type)

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

- `dg3f_m_description` — URDF model
- `dg3f_m_driver` — Launch files and configuration
- `dg3f_m_gz` — Gazebo simulation

## Launch

```bash
ros2 launch dg3f_m_driver dg3f_m_driver.launch.py
```

## Firmware Note

Motor direction was revised in firmware **v2.8+**.
