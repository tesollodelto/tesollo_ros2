# Service Definitions

This page lists all services available in the Tesollo ROS 2 packages. Services are divided into two categories:

1. **Hardware interface services** -- provided by `delto_hardware` (available when using ros2_control)
2. **SDK bridge services** -- provided by `dg_sdk_ros2_bridge` (available when using the SDK bridge)

## Hardware Interface Services (delto_hardware)

These services are created by the `delto_hardware` hardware interface and are available under your gripper's namespace (e.g., `/dg5f_right/`, `/dg4f/`, `/dg3f_m/`).

```{list-table}
:header-rows: 1
:widths: 40 25 35

* - Service
  - Type
  - Description
* - `/<ns>/delto_hardware_interface_node/set_ft_sensor_offset`
  - `std_srvs/srv/Trigger`
  - Zero-calibrate all fingertip F/T sensors. Call when nothing is touching the fingertips
* - `/<ns>/set_gpio_output1`
  - `std_srvs/srv/SetBool`
  - Set GPIO output 1 (true = high, false = low)
* - `/<ns>/set_gpio_output2`
  - `std_srvs/srv/SetBool`
  - Set GPIO output 2
* - `/<ns>/set_gpio_output3`
  - `std_srvs/srv/SetBool`
  - Set GPIO output 3
```

Replace `<ns>` with your gripper's namespace. Common namespaces:
- `dg5f_right`, `dg5f_left` -- DG-5F
- `dg5f_both` -- DG-5F dual-hand
- `dg5f_s_right`, `dg5f_s_left` -- DG-5F-S
- `dg4f` -- DG-4F
- `dg3f_m` -- DG-3F-M

**Example usage:**

```bash
# Calibrate F/T sensors on a DG-5F Right
ros2 service call /dg5f_right/delto_hardware_interface_node/set_ft_sensor_offset std_srvs/srv/Trigger

# Enable GPIO output 1 on a DG-4F
ros2 service call /dg4f/set_gpio_output1 std_srvs/srv/SetBool "{data: true}"
```

```{note}
The F/T sensor calibration and GPIO services use standard ROS 2 message types (`std_srvs/srv/Trigger` and `std_srvs/srv/SetBool`), which are included with every ROS 2 installation. No custom messages are needed.
```

## SDK Bridge Services (dg_sdk_ros2_bridge)

All custom services below are defined in the `dg_msgs` package (part of the `dg_sdk_ros2_bridge` repository). They are available when running the SDK bridge node.

### System Setting

| Service | Description |
|---------|-------------|
| `SetGripperSystem` | Configure gripper system parameters |
| `SetGripperOption` | Set gripper options |
| `ConnectToGripper` | Establish connection to the gripper |
| `DisconnectToGripper` | Close connection |
| `SystemStart` | Start the gripper system |
| `SystemStop` | Stop the gripper system |
| `SetIp` | Configure the gripper's network IP address |

### System Configuration

| Service | Description |
|---------|-------------|
| `SetLowPassFilterAlpha` | Set low-pass filter coefficient for sensor smoothing |
| `SetBootMode` | Configure power-on behavior |
| `SetGPIOOutput` | Set individual GPIO output |
| `SetGPIOOutputAll` | Set all GPIO outputs at once |
| `SetTorqueLimitMode` | Configure torque limiting |
| `SetBootRecipe` | Set boot-up recipe |
| `EEPROMWrite` | Write to persistent storage |
| `SystemDiagnosis` | Run system diagnostics |
| `BackupRecipeData` | Backup all recipes |
| `RestoreRecipeData` | Restore recipes from backup |

### Joint Gain Settings

| Service | Description |
|---------|-------------|
| `SetJointGainP` / `PFinger` / `PAll` | Proportional gain (per joint / per finger / all) |
| `SetJointGainD` / `DFinger` / `DAll` | Derivative gain |
| `SetJointGainI` / `IFinger` / `IAll` | Integral gain |
| `SetJointGainPID` / `PIDFinger` / `PIDAll` / `PIDAllEqual` | Combined PID gains |
| `SetControlPIDMode` | Set PID control mode |

### Motion Time

| Service | Description |
|---------|-------------|
| `SetMotionTimeJoint` / `Finger` / `All` / `AllEqual` | Set motion duration |

### Position and Current Control

| Service | Description |
|---------|-------------|
| `SetPositionModeJoint` / `Finger` / `All` / `Base` | Set position control mode |
| `SetCurrentControlMode` | Enable current control |
| `SetTargetCurrentJoint` / `Finger` / `All` / `Base` | Set target current |
| `SetJointCurrentGainP` / `PFinger` / `PAll` / `PBase` | Current-loop P gain (per joint, per finger, all, or base) |
| `SetJointCurrentGainI` / `IFinger` / `IAll` / `IBase` | Current-loop I gain (per joint, per finger, all, or base) [^1] |

[^1]: I-gain services also accept an integral limit (`current_i_limit`). The `Base` variants apply only to the base joints of DG-4F.

### Motion Commands

| Service | Description |
|---------|-------------|
| `MoveJoint` / `MoveJointFinger` / `MoveJointAll` | Joint space motion |
| `MoveServoJoint` | Servo (real-time) joint control |
| `ManualTeachMode` | Enable/disable manual teaching |
| `StartGraspMotion` | Execute grasp motion |
| `SetGraspData` / `SetGraspForce` | Configure grasp parameters |

### TCP Control (Experimental)

| Service | Description |
|---------|-------------|
| `SetTCPGainP/D/I/PID` (Finger/All) | TCP space PID gains |
| `SetTCPMotionTime` (Finger/All/AllEqual) | TCP motion duration |
| `MoveTCPFinger` / `MoveTCPAll` | TCP space motion |
| `GetCurrentTcpPose` | Get current TCP pose |
| `SetManipulationGainPIDAll` | In-hand manipulation gains |
| `InHandManipulation` | Execute in-hand manipulation |
| `SetFingerTipDataZero` | Zero fingertip sensor |

### Recipe Management

| Service | Description |
|---------|-------------|
| `UpdateRecipeCurrentPoseData` | Save current pose to recipe |
| `UpdateRecipeTargetPoseData` | Save target pose to recipe |
| `LoadRecipePoseData` | Load pose recipe |
| `UpdateRecipeGainData` | Save gain recipe |
| `LoadRecipeGainData` | Load gain recipe |
| `UpdateRecipeGraspData` | Save grasp recipe |
| `LoadRecipeGraspData` | Load grasp recipe |

### Blend Motion

| Service | Description |
|---------|-------------|
| `UpdateBlendJoint` | Update blend motion waypoint |
| `AddMoveBlendJoint` | Add waypoint to blend queue |
| `ClearMoveBlendJoint` | Clear blend queue |
| `SetMoveBlendJoint` | Configure blend motion |
| `StartMoveBlendJoint` | Execute blend motion |
| `StopMoveBlendJoint` | Stop blend motion |

### Getters

| Service | Description |
|---------|-------------|
| `GetReceivedGripperData` | Get latest gripper data |
| `GetReceivedFingertipSensorData` | Get latest F/T data |
| `GetReceivedGPIOData` | Get latest GPIO data |
| `GetCommunicationPeriod` | Get communication cycle time |
| `GetDataProcessing` | Get processing status |
| `GetDiagnosisSystem` | Get diagnostics |
