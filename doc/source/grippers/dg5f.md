# DG-5F

[![CI](https://github.com/tesollodelto/dg5f_ros2/actions/workflows/ci.yml/badge.svg)](https://github.com/tesollodelto/dg5f_ros2/actions/workflows/ci.yml)

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

| Package | Description |
|---------|-------------|
| `dg5f_description` | URDF/xacro model, meshes, and RViz display launch |
| `dg5f_driver` | ros2_control hardware driver and controller launch files |
| `dg5f_gz` | Gazebo simulation |

## Launch

**Hardware driver**

```bash
ros2 launch dg5f_driver dg5f_right_driver.launch.py
```

```bash
ros2 launch dg5f_driver dg5f_left_driver.launch.py
```

With custom IP/port:

```bash
ros2 launch dg5f_driver dg5f_right_driver.launch.py delto_ip:=192.168.1.100 delto_port:=502
```

**Effort controller**

```bash
ros2 launch dg5f_driver dg5f_right_effort_controller.launch.py
```

```bash
ros2 launch dg5f_driver dg5f_left_effort_controller.launch.py
```

**PID controller**

```bash
ros2 launch dg5f_driver dg5f_right_pid_controller.launch.py
```

```bash
ros2 launch dg5f_driver dg5f_left_pid_controller.launch.py
```

**RViz display**

```bash
ros2 launch dg5f_description dg5f_right_display.launch.py
```

```bash
ros2 launch dg5f_description dg5f_left_display.launch.py
```

**Gazebo simulation**

```bash
ros2 launch dg5f_gz dg5f_right_gz.launch.py
```

```bash
ros2 launch dg5f_gz dg5f_left_gz.launch.py
```

```bash
ros2 launch dg5f_gz dg5f_both_gz.launch.py
```

## Hand Type

Left and right hands have different motor direction arrays and separate launch files. The hand type is determined by the Model ID in the URDF hardware parameters.

## Firmware Note

Motor direction was revised in firmware **v2.8+**. The hardware interface automatically detects the firmware version and applies the correct motor direction mapping.
