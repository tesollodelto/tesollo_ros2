# Message Definitions

This page documents all custom message types used in the Tesollo ROS 2 packages. These messages are defined in the `dg_msgs` package (part of the `dg_sdk_ros2_bridge` repository) and are used by the SDK bridge node.

```{note}
**For ros2_control users:** The ros2_control driver (`delto_hardware`) uses standard ROS 2 message types only -- you do not need the `dg_msgs` package. Standard types used include `sensor_msgs/msg/JointState`, `geometry_msgs/msg/WrenchStamped`, `std_srvs/srv/Trigger`, and `std_srvs/srv/SetBool`.
```

## ReceivedGripperData

Gripper feedback data published continuously on `/dg/gripper_data`.

```
float64[] joint          # Joint positions (radians), one per joint
float64[] velocity       # Joint velocities (radians/second), one per joint
float64[] current        # Motor currents (amperes), one per joint
float64[] temperature    # Motor temperatures (Celsius), one per joint
```

**How to read this topic:**

```bash
ros2 topic echo /dg/gripper_data
```

## ReceivedFingertipSensorData

F/T sensor readings published continuously on `/dg/fingertip_sensor_data`.

```
float64[] data    # 6 values per finger: [fx, fy, fz, tx, ty, tz]
```

The array is organized as `[finger1_fx, finger1_fy, finger1_fz, finger1_tx, finger1_ty, finger1_tz, finger2_fx, ...]`. For a 5-finger gripper, this contains 30 values (5 fingers x 6 axes).

**How to read this topic:**

```bash
ros2 topic echo /dg/fingertip_sensor_data
```

## ReceivedGPIOData

GPIO states published continuously on `/dg/gpio_data`.

```
bool[] data    # [output1, output2, output3, input1]
```

- `data[0]` through `data[2]`: Current state of the 3 GPIO outputs
- `data[3]`: Current state of the GPIO input

## DiagnosisSystem

System diagnostic information published on `/dg/diagnosis_system`.

```{note}
See `dg_msgs/msg/DiagnosisSystem.msg` for the complete field definitions. This message contains internal system status information for debugging and monitoring.
```

## Recipe Messages

These messages are used with the recipe management services to save and load gripper configurations:

| Message | Description |
|---------|-------------|
| `RecipePoseData` | Joint pose recipe -- stores target joint positions |
| `RecipeGainData` | PID gain recipe -- stores PID gain values per joint |
| `RecipeGraspData` | Grasp recipe -- stores grasp force and motion parameters |
| `RecipeBlendData` | Blend motion recipe -- stores waypoints for smooth multi-point motions |

```{note}
See the corresponding `dg_msgs/msg/Recipe*.msg` files for complete field definitions.
```

## Configuration Messages

| Message | Description |
|---------|-------------|
| `GripperSystemSetting` | System-level configuration (used with `SetGripperSystem` service) |
| `GripperSetting` | Gripper-specific settings (used with `SetGripperOption` service) |

```{note}
See the corresponding `dg_msgs/msg/GripperSystemSetting.msg` and `dg_msgs/msg/GripperSetting.msg` files for complete field definitions.
```

## Standard ROS 2 Messages Used by ros2_control

When using the `delto_hardware` driver (ros2_control approach), the system uses these standard message types:

```{list-table}
:header-rows: 1
:widths: 25 35 40

* - Type
  - Where Used
  - Description
* - `sensor_msgs/msg/JointState`
  - `/<ns>/joint_states`
  - Joint positions, velocities, and efforts
* - `geometry_msgs/msg/WrenchStamped`
  - `/<ns>/fingertip_N_broadcaster/wrench`
  - F/T sensor force and torque readings
* - `control_msgs/action/FollowJointTrajectory`
  - `/<ns>/<controller>/follow_joint_trajectory`
  - Trajectory commands for the JTC
* - `std_msgs/msg/Float64MultiArray`
  - `/<ns>/effort_controller/commands`
  - Direct effort commands
* - `std_msgs/msg/Float64`
  - `/<ns>/<joint>_pospid/reference`
  - PID controller position reference
* - `std_msgs/msg/Float64MultiArray`
  - `/<ns>/rj_dg_pospid/reference`
  - Grouped PID controller position references
```
