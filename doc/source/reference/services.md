# Service Definitions

All custom services are defined in the `dg_msgs` package (part of `dg_sdk_ros2_bridge` repository). Used by the SDK bridge node.

## System Setting

| Service | Description |
|---------|-------------|
| `SetGripperSystem` | Configure gripper system parameters |
| `SetGripperOption` | Set gripper options |
| `ConnectToGripper` | Establish connection |
| `DisconnectToGripper` | Close connection |
| `SystemStart` | Start gripper system |
| `SystemStop` | Stop gripper system |
| `SetIp` | Configure gripper IP |

## System Configuration

| Service | Description |
|---------|-------------|
| `SetLowPassFilterAlpha` | Set low-pass filter coefficient |
| `SetBootMode` | Configure boot mode |
| `SetGPIOOutput` | Set individual GPIO output |
| `SetGPIOOutputAll` | Set all GPIO outputs |
| `SetTorqueLimitMode` | Configure torque limiting |
| `SetBootRecipe` | Set boot recipe |
| `EEPROMWrite` | Write to EEPROM |
| `SystemDiagnosis` | Run system diagnostics |
| `BackupRecipeData` | Backup all recipes |
| `RestoreRecipeData` | Restore recipes from backup |

## Joint Gain Settings

| Service | Description |
|---------|-------------|
| `SetJointGainP` / `PFinger` / `PAll` | Proportional gain |
| `SetJointGainD` / `DFinger` / `DAll` | Derivative gain |
| `SetJointGainI` / `IFinger` / `IAll` | Integral gain |
| `SetJointGainPID` / `PIDFinger` / `PIDAll` / `PIDAllEqual` | Combined PID |
| `SetControlPIDMode` | Set PID control mode |

## Motion Time

| Service | Description |
|---------|-------------|
| `SetMotionTimeJoint` / `Finger` / `All` / `AllEqual` | Set motion duration |

## Position & Current Control

| Service | Description |
|---------|-------------|
| `SetPositionModeJoint` / `Finger` / `All` | Set position mode |
| `SetCurrentControlMode` | Enable current control |
| `SetTargetCurrentJoint` / `Finger` / `All` | Set target current |

## Motion Commands

| Service | Description |
|---------|-------------|
| `MoveJoint` / `MoveJointFinger` / `MoveJointAll` | Joint space motion |
| `MoveServoJoint` | Servo (real-time) joint control |
| `ManualTeachMode` | Enable/disable manual teaching |
| `StartGraspMotion` | Execute grasp motion |
| `SetGraspData` / `SetGraspForce` | Configure grasp parameters |

## TCP Control (Experimental)

| Service | Description |
|---------|-------------|
| `SetTCPGainP/D/I/PID` (Finger/All) | TCP space PID gains |
| `SetTCPMotionTime` (Finger/All/AllEqual) | TCP motion duration |
| `MoveTCPFinger` / `MoveTCPAll` | TCP space motion |
| `GetCurrentTcpPose` | Get current TCP pose |
| `SetManipulationGainPIDAll` | In-hand manipulation gains |
| `InHandManipulation` | Execute in-hand manipulation |
| `SetFingerTipDataZero` | Zero fingertip sensor |

## Recipe Management

| Service | Description |
|---------|-------------|
| `UpdateRecipeCurrentPoseData` | Save current pose to recipe |
| `UpdateRecipeTargetPoseData` | Save target pose to recipe |
| `LoadRecipePoseData` | Load pose recipe |
| `UpdateRecipeGainData` | Save gain recipe |
| `LoadRecipeGainData` | Load gain recipe |
| `UpdateRecipeGraspData` | Save grasp recipe |
| `LoadRecipeGraspData` | Load grasp recipe |

## Blend Motion

| Service | Description |
|---------|-------------|
| `UpdateBlendJoint` | Update blend motion waypoint |
| `AddMoveBlendJoint` | Add waypoint to blend queue |
| `ClearMoveBlendJoint` | Clear blend queue |
| `SetMoveBlendJoint` | Configure blend motion |
| `StartMoveBlendJoint` | Execute blend motion |
| `StopMoveBlendJoint` | Stop blend motion |

## Getters

| Service | Description |
|---------|-------------|
| `GetReceivedGripperData` | Get latest gripper data |
| `GetReceivedFingertipSensorData` | Get latest F/T data |
| `GetReceivedGPIOData` | Get latest GPIO data |
| `GetCommunicationPeriod` | Get comm cycle time |
| `GetDataProcessing` | Get processing status |
| `GetDiagnosisSystem` | Get diagnostics |
