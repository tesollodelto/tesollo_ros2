# DG-3F (B Type)

```{warning}
This model has been **discontinued**. This page is kept for reference only.
```

3-finger gripper, basic model.

## Specifications

| Parameter | Value |
|-----------|-------|
| Model ID | `0x3F01` |
| Fingers | 3 |
| Joints | 12 (4 per finger) |
| F/T Sensor | No |
| GPIO | Yes (3 outputs, 1 input) |

## Packages

- `dg3f_b_description` — URDF model
- `dg3f_b_driver` — Launch files and configuration

## Launch

```bash
ros2 launch dg3f_b_driver dg3f_b.launch.py
```

## Firmware Note

Motor direction was revised in firmware **v3.6+**. If your firmware is older, the hardware interface automatically applies legacy motor direction mapping.
