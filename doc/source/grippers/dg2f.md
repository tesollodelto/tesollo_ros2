# DG-2F (M Type)

2-finger gripper with 6 actuated joints (3 per finger).

## Specifications

| Parameter | Value |
|-----------|-------|
| Model ID | `0x2F02` |
| Fingers | 2 |
| Joints | 6 actuated (`j_1_1~3`, `j_2_1~3`; the `j_X_4` tips are fixed) |
| F/T Sensor | No |
| GPIO | Yes (3 outputs, 1 input) |

## Packages

| Package | Description |
|---------|-------------|
| `dg2f_description` | URDF model, meshes, and RViz display launch |
| `dg2f_driver` | ros2_control hardware driver, controller configs, and launch files |

## Before You Control: Developer Mode

```{important}
The ros2_control driver speaks a custom protocol over Ethernet that is only available in **Developer Mode**. Set the gripper to **Developer Mode** with the communication mode set to **EtherNET** before launching the driver (check the switch/LED settings in your DG-2F product manual), otherwise the connection will fail.
```

## Launch

```bash
# JointTrajectoryController (default)
ros2 launch dg2f_driver dg2f_driver.launch.py delto_ip:=169.254.186.72

# Direct effort control
ros2 launch dg2f_driver dg2f_effort_controller.launch.py delto_ip:=169.254.186.72

# Mock hardware (no device required)
ros2 launch dg2f_driver dg2f_mock.launch.py
```

PID controllers (position reference → effort) are also provided:

```bash
# Individual: one PidController per joint (/dg2f/<joint>_pospid/reference)
ros2 launch dg2f_driver dg2f_pid_controller.launch.py delto_ip:=169.254.186.72

# All-in-one: a single grouped PidController (/dg2f/j_pospid/reference)
ros2 launch dg2f_driver dg2f_pid_all_controller.launch.py delto_ip:=169.254.186.72
```

## Controllers

| Controller | Config | Topic / Action |
|------------|--------|----------------|
| JointTrajectoryController | `dg2f_controller.yaml` | `/dg2f/dg2f_controller/joint_trajectory` (+ `follow_joint_trajectory` action) |
| JointGroupEffortController | `dg2f_effort_controller.yaml` | `/dg2f/effort_controller/commands` |
| PidController — individual | `dg2f_pid_controller.yaml` | `/dg2f/<joint>_pospid/reference` |
| PidController — grouped (`pid_all`) | `dg2f_pid_all_controller.yaml` | `/dg2f/j_pospid/reference` |

Test scripts: `dg2f_jtc_test.py`, `dg2f_jtc_action_test.py`, `dg2f_pid_test.py`, `dg2f_pid_all_test.py`.

## Firmware Note

The DG-2F-M is an **M-series** gripper. Motor direction was revised in firmware **v2.8+**. If your firmware is older, the hardware interface automatically detects this and applies the legacy motor direction mapping. No manual configuration is required.
