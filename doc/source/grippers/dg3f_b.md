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
| `dg3f_b_moveit_config` | MoveIt 2 configuration (SRDF, planners, mock hardware) |

## Before You Control: Developer Mode

```{important}
The ros2_control driver speaks a custom protocol over Ethernet that is only available in **Developer Mode**. On the DG-3F button panel, set ② **Developer Mode** and ④ **EtherNET** before launching the driver, otherwise the connection will fail. See the `dg3f_b_driver` README for the switch/LED panel image.
```

## Launch

```bash
ros2 launch dg3f_b_driver dg3f_b_driver.launch.py
```

A multi-gripper launch file is also available:

```bash
ros2 launch dg3f_b_driver dg3f_b_driver_multi.launch.py
```

## MoveIt Integration

The `dg3f_b_moveit_config` package provides MoveIt 2 motion planning for the DG-3F-B with mock hardware support.

### Mock Hardware (No Device Required)

```bash
ros2 launch dg3f_b_moveit_config dg3f_b_moveit.launch.py use_mock:=true
```

### Real Hardware with MoveIt

```bash
ros2 launch dg3f_b_moveit_config dg3f_b_moveit.launch.py use_mock:=false delto_ip:=169.254.186.72
```

### Planning Groups

| Group | Joints |
|-------|--------|
| `finger_1` | `j_dg_1_1` ~ `j_dg_1_4` (chain to `l_dg_1_tip`) |
| `finger_2` | `j_dg_2_1` ~ `j_dg_2_4` (chain to `l_dg_2_tip`) |
| `finger_3` | `j_dg_3_1` ~ `j_dg_3_4` (chain to `l_dg_3_tip`) |
| `all_fingers` | All 12 joints |

## Firmware Note

Motor direction was revised in firmware **v3.6+**. If your firmware is older, the hardware interface automatically detects this and applies the legacy motor direction mapping. No manual configuration is required.
