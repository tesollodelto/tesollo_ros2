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

**Hardware driver**

```bash
ros2 launch dg4f_driver dg4f_driver.launch.py
```

With custom IP/port:

```bash
ros2 launch dg4f_driver dg4f_driver.launch.py delto_ip:=192.168.1.100 delto_port:=502
```

**Effort controller**

```bash
ros2 launch dg4f_driver dg4f_effort_controller.launch.py
```

**RViz display**

```bash
ros2 launch dg4f_description dg4f_display.launch.py
```

**Gazebo simulation**

```bash
ros2 launch dg4f_gz dg4f_gz.launch.py
```

## Note

DG-4F motor direction is always standard (no firmware version dependency).
