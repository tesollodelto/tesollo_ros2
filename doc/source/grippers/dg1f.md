# DG-1F (M Type)

1-finger gripper with 3 actuated joints. Smallest model in the Delto lineup.

## Specifications

| Parameter | Value |
|-----------|-------|
| Model ID | `0x1F02` |
| Fingers | 1 |
| Joints | 3 actuated (`j_1_1`, `j_1_2`, `j_1_3`; `j_1_4` is a fixed tip) |
| F/T Sensor | No |
| GPIO | Yes (3 outputs, 1 input) |

## Packages

| Package | Description |
|---------|-------------|
| `dg1f_description` | URDF model, meshes, and RViz display launch |
| `dg1f_driver` | ros2_control hardware driver, controller configs, and launch files |

## Before You Control: Developer Mode

```{important}
The ros2_control driver speaks a custom protocol over Ethernet that is only available in **Developer Mode**. Set the gripper to **Developer Mode** with the communication mode set to **EtherNET** before launching the driver (check the switch/LED settings in your DG-1F product manual), otherwise the connection will fail.
```

## Launch

```bash
# JointTrajectoryController (default)
ros2 launch dg1f_driver dg1f_driver.launch.py delto_ip:=169.254.186.72

# Direct effort control
ros2 launch dg1f_driver dg1f_effort_controller.launch.py delto_ip:=169.254.186.72

# Mock hardware (no device required)
ros2 launch dg1f_driver dg1f_mock.launch.py
```

PID controllers (position reference → effort) are also provided:

```bash
# Individual: one PidController per joint (/dg1f/<joint>_pospid/reference)
ros2 launch dg1f_driver dg1f_pid_controller.launch.py delto_ip:=169.254.186.72

# All-in-one: a single grouped PidController (/dg1f/j_pospid/reference)
ros2 launch dg1f_driver dg1f_pid_all_controller.launch.py delto_ip:=169.254.186.72
```

## Controllers

| Controller | Config | Topic / Action |
|------------|--------|----------------|
| JointTrajectoryController | `dg1f_controller.yaml` | `/dg1f/dg1f_controller/joint_trajectory` (+ `follow_joint_trajectory` action) |
| JointGroupEffortController | `dg1f_effort_controller.yaml` | `/dg1f/effort_controller/commands` |
| PidController — individual | `dg1f_pid_controller.yaml` | `/dg1f/<joint>_pospid/reference` |
| PidController — grouped (`pid_all`) | `dg1f_pid_all_controller.yaml` | `/dg1f/j_pospid/reference` |

Test scripts: `dg1f_jtc_test.py`, `dg1f_jtc_action_test.py`, `dg1f_pid_test.py`, `dg1f_pid_all_test.py`.

## Firmware Note

The DG-1F-M is an **M-series** gripper. Motor direction was revised in firmware **v2.8+**. If your firmware is older, the hardware interface automatically detects this and applies the legacy motor direction mapping. No manual configuration is required.
