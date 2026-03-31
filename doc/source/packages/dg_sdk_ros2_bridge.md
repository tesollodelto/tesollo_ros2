# dg_sdk_ros2_bridge

[![CI](https://github.com/tesollodelto/dg_sdk_ros2_bridge/actions/workflows/ci.yml/badge.svg)](https://github.com/tesollodelto/dg_sdk_ros2_bridge/actions/workflows/ci.yml)

ROS 2 bridge for the Delto Gripper SDK (DG SDK).

```{warning}
The DG SDK (`libDGSDK.so`) is only available for **x86_64** architectures. This package cannot be used on ARM-based platforms (such as NVIDIA Jetson or Raspberry Pi).
```

```{note}
Ensure the bundled SDK version matches your gripper firmware. Check the SDK release notes for compatibility information.
```

## Overview

The `dg_sdk_ros2_bridge` package provides a ROS 2 node that wraps the proprietary DG SDK library, exposing all gripper functions as ROS 2 **topics** and **services**. This is an alternative to using `delto_hardware` with `ros2_control`.

```{note}
**What is the difference between the SDK bridge and ros2_control?** The ros2_control approach (`delto_hardware`) gives you a standardized hardware interface that works with standard ROS 2 controllers, MoveIt, and the broader ROS 2 ecosystem. The SDK bridge gives you direct access to all proprietary SDK functions -- including recipe management, advanced gain tuning, blend motion, TCP space control, and more. Choose based on your needs.
```

## When to Use Which

| Use Case | Recommended Approach |
|----------|---------------------|
| Standard position/effort control | `delto_hardware` (ros2_control) |
| MoveIt integration | `delto_hardware` (ros2_control) |
| Trajectory following | `delto_hardware` (ros2_control) |
| Recipe management | `dg_sdk_ros2_bridge` |
| Advanced PID gain tuning per joint/finger | `dg_sdk_ros2_bridge` |
| Blend motion (multi-waypoint smooth paths) | `dg_sdk_ros2_bridge` |
| TCP (tool center point) space control | `dg_sdk_ros2_bridge` |
| In-hand manipulation | `dg_sdk_ros2_bridge` |
| System diagnostics and configuration | `dg_sdk_ros2_bridge` |

```{warning}
**Important:** The gripper only accepts **one TCP connection** at a time. You cannot run `delto_hardware` and `dg_sdk_ros2_bridge` simultaneously for the same gripper. Choose one approach per gripper instance.
```

## Architecture

```
Your Application
    |
    v  (ROS 2 topics & services)
dg_sdk_ros2_bridge node
    |
    v  (C++ function calls)
DG SDK (libDGSDK.so)
    |
    v  (TCP/IP)
Gripper Firmware
```

The bridge node acts as a translator: it receives ROS 2 service calls from your application and forwards them to the proprietary DG SDK library, which handles the actual communication with the gripper.

## Published Topics

The bridge node continuously publishes gripper data on these topics:

| Topic | Type | Rate | Description |
|-------|------|------|-------------|
| `/dg/gripper_data` | `dg_msgs/ReceivedGripperData` | Continuous | Joint positions, velocities, currents, and temperatures |
| `/dg/fingertip_sensor_data` | `dg_msgs/ReceivedFingertipSensorData` | Continuous | F/T sensor readings (6 values per finger) |
| `/dg/gpio_data` | `dg_msgs/ReceivedGPIOData` | Continuous | GPIO input/output states |
| `/dg/diagnosis_system` | `dg_msgs/DiagnosisSystem` | Continuous | System diagnostic information |

## Services by Category

The bridge exposes 86+ services for full SDK functionality. Here they are organized by category:

### System Settings

| Service | Description |
|---------|-------------|
| `SetGripperSystem` | Configure gripper system parameters |
| `SetGripperOption` | Set gripper options |
| `ConnectToGripper` | Establish connection to the gripper |
| `DisconnectToGripper` | Close connection |
| `SystemStart` | Start the gripper system |
| `SystemStop` | Stop the gripper system |
| `SetIp` | Configure the gripper's IP address |

### System Configuration

| Service | Description |
|---------|-------------|
| `SetLowPassFilterAlpha` | Set low-pass filter coefficient for sensor data smoothing |
| `SetBootMode` | Configure how the gripper behaves on power-up |
| `SetGPIOOutput` | Set a single GPIO output |
| `SetGPIOOutputAll` | Set all GPIO outputs at once |
| `SetTorqueLimitMode` | Configure motor torque limiting |
| `SetBootRecipe` | Set which recipe runs automatically on boot |
| `EEPROMWrite` | Write configuration to the gripper's persistent storage |
| `SystemDiagnosis` | Run a system diagnostic check |
| `BackupRecipeData` | Backup all recipes from the gripper |
| `RestoreRecipeData` | Restore recipes to the gripper |

### Joint Gain Settings (PID Tuning)

| Service | Description |
|---------|-------------|
| `SetJointGainP` / `PFinger` / `PAll` | Set proportional gain (per joint, per finger, or all joints) |
| `SetJointGainD` / `DFinger` / `DAll` | Set derivative gain |
| `SetJointGainI` / `IFinger` / `IAll` | Set integral gain |
| `SetJointGainPID` / `PIDFinger` / `PIDAll` / `PIDAllEqual` | Set all PID gains together |
| `SetControlPIDMode` | Switch between PID control modes |

### Motion Time

| Service | Description |
|---------|-------------|
| `SetMotionTimeJoint` / `Finger` / `All` / `AllEqual` | Set how long a motion should take (per joint, per finger, or all) |

### Position and Current Control

| Service | Description |
|---------|-------------|
| `SetPositionModeJoint` / `Finger` / `All` | Enable position control mode |
| `SetCurrentControlMode` | Enable direct current control mode |
| `SetTargetCurrentJoint` / `Finger` / `All` | Set target motor current |

### Motion Commands

| Service | Description |
|---------|-------------|
| `MoveJoint` / `MoveJointFinger` / `MoveJointAll` | Move joints to target positions |
| `MoveServoJoint` | Real-time servo mode for smooth continuous motion |
| `ManualTeachMode` | Enable/disable manual teach mode (freely move joints by hand) |
| `StartGraspMotion` | Execute a pre-configured grasp motion |
| `SetGraspData` / `SetGraspForce` | Configure grasp parameters and force limits |

### TCP (Tool Center Point) Control -- Experimental

| Service | Description |
|---------|-------------|
| `SetTCPGainP/D/I/PID` (Finger/All) | Set PID gains for TCP space control |
| `SetTCPMotionTime` (Finger/All/AllEqual) | Set motion time for TCP space motions |
| `MoveTCPFinger` / `MoveTCPAll` | Move fingertips to target poses in Cartesian space |
| `GetCurrentTcpPose` | Get current TCP (fingertip) pose |
| `SetManipulationGainPIDAll` | Set PID gains for in-hand manipulation |
| `InHandManipulation` | Execute in-hand manipulation |
| `SetFingerTipDataZero` | Zero the fingertip sensor data |

### Recipe Management

| Service | Description |
|---------|-------------|
| `UpdateRecipeCurrentPoseData` | Save the current pose as a recipe |
| `UpdateRecipeTargetPoseData` | Save a target pose as a recipe |
| `LoadRecipePoseData` | Load a saved pose recipe |
| `UpdateRecipeGainData` | Save current PID gains as a recipe |
| `LoadRecipeGainData` | Load saved PID gain settings |
| `UpdateRecipeGraspData` | Save grasp parameters as a recipe |
| `LoadRecipeGraspData` | Load saved grasp parameters |

### Blend Motion (Multi-Waypoint)

| Service | Description |
|---------|-------------|
| `UpdateBlendJoint` | Update a blend motion waypoint |
| `AddMoveBlendJoint` | Add a waypoint to the blend queue |
| `ClearMoveBlendJoint` | Clear all waypoints from the blend queue |
| `SetMoveBlendJoint` | Configure blend motion parameters |
| `StartMoveBlendJoint` | Execute the blend motion |
| `StopMoveBlendJoint` | Stop the currently running blend motion |

### Data Getters

| Service | Description |
|---------|-------------|
| `GetReceivedGripperData` | Get the latest gripper data on demand |
| `GetReceivedFingertipSensorData` | Get the latest F/T sensor data |
| `GetReceivedGPIOData` | Get the latest GPIO states |
| `GetCommunicationPeriod` | Get the current communication cycle time |
| `GetDataProcessing` | Get the data processing status |
| `GetDiagnosisSystem` | Get the latest diagnostic information |

## Step-by-Step Usage Example

Here is a basic workflow for using the SDK bridge:

**Step 1: Launch the bridge node.**

```bash
ros2 run dg_sdk_ros2_bridge dg_sdk_ros2_bridge_node
```

**Step 2: Configure and connect to the gripper.**

```bash
# Set up the gripper system parameters
ros2 service call /dg/SetGripperSystem dg_msgs/srv/SetGripperSystem "{...}"

# Connect to the gripper
ros2 service call /dg/ConnectToGripper dg_msgs/srv/ConnectToGripper "{...}"

# Start the system
ros2 service call /dg/SystemStart dg_msgs/srv/SystemStart "{}"
```

**Step 3: Monitor gripper data.**

```bash
# Watch joint positions, velocities, and currents
ros2 topic echo /dg/gripper_data

# Watch F/T sensor data
ros2 topic echo /dg/fingertip_sensor_data
```

**Step 4: Send motion commands.**

```bash
# Move all joints to target positions
ros2 service call /dg/MoveJointAll dg_msgs/srv/MoveJointAll "{...}"
```

**Step 5: Stop and disconnect.**

```bash
ros2 service call /dg/SystemStop dg_msgs/srv/SystemStop "{}"
ros2 service call /dg/DisconnectToGripper dg_msgs/srv/DisconnectToGripper "{}"
```

## Message Types

All custom messages and services used by the bridge are defined in the `dg_msgs` package. See {doc}`/reference/messages` and {doc}`/reference/services` for complete definitions.
