# Architecture Overview

This page explains how the Tesollo ROS 2 gripper software is organized and how the different components work together. Understanding this architecture will help you choose the right approach for your application and troubleshoot issues.

## Repository Structure

The `tesollo_ros2` repository is organized as a collection of **git submodules**, each containing a specific gripper model or shared library:

```
tesollo_ros2/
├── dg3f_b_ros2/          [submodule] DG-3F (B type) -- discontinued
├── dg3f_m_ros2/          [submodule] DG-3F (M type) -- 3-finger gripper
├── dg4f_ros2/            [submodule] DG-4F -- 4-finger gripper
├── dg5f_ros2/            [submodule] DG-5F -- 5-finger hand (left/right)
├── dg5f_s_ros2/          [submodule] DG-5F-S -- 5-finger small hand (20-DOF/15-DOF)
├── dg_common/
│   ├── dg_hardware/      [submodule] delto_hardware -- ros2_control hardware interface
│   └── dg_tcp_comm/      [submodule] delto_tcp_comm -- TCP communication library
└── dg_sdk_ros2_bridge/   [submodule] SDK bridge + dg_msgs definitions
```

Each gripper model repository (e.g., `dg5f_ros2`) contains three ROS 2 packages:
- **description** -- URDF model, meshes, RViz display launch files
- **driver** -- Controller configurations and launch files for ros2_control
- **gz** -- Gazebo simulation launch files

The shared packages in `dg_common/` are used by all gripper models.

## Two Integration Paths

There are two ways to control a Delto gripper from ROS 2. You must choose **one** per gripper, because the gripper only accepts a single TCP connection at a time.

### Option A: ros2_control (Recommended)

```{graphviz}
digraph A {
    rankdir=LR;
    node [shape=box, style=filled, fillcolor="#E8F0FE"];

    User [label="User Application", fillcolor="#FFF3E0"];
    RC [label="ros2_control\nController"];
    HW [label="delto_hardware"];
    TCP [label="delto_tcp_comm"];
    FW [label="Gripper\nFirmware", fillcolor="#E8F5E9"];

    User -> RC [label="position / effort\ncommands"];
    RC -> HW [label="effort command\ninterface"];
    HW -> TCP [label="TCP protocol"];
    TCP -> FW;
}
```

```{note}
**What is ros2_control?** It is a ROS 2 framework that separates hardware drivers from control logic. You write your application using standard ROS 2 topics and actions, and ros2_control handles the communication with the hardware. This makes your code portable -- the same code that controls a Delto gripper can control other ros2_control-compatible hardware with minimal changes.
```

**How it works:**

1. **Your application** sends commands (position targets, effort values, or trajectories) to a **controller**.
2. The **controller** (e.g., JointTrajectoryController) processes the command and computes effort (current) values.
3. The **delto_hardware** plugin receives the effort commands and sends them to the gripper over TCP.
4. The **delto_tcp_comm** library handles the low-level TCP communication protocol.

**Available controller types:**

| Controller | Input | Output | Use Case |
|-----------|-------|--------|----------|
| JointTrajectoryController | Joint trajectories (positions over time) | Effort | Position control, MoveIt, smooth multi-joint motion |
| JointGroupEffortController | Direct effort values | Effort | Raw current/torque control, custom control loops |
| PidController (per joint) | Position reference per joint | Effort | Simple per-joint position control |
| PidController (grouped) | Position references for all joints | Effort | Multi-joint position control via topic |

**Best for:** MoveIt integration, standard ROS 2 control pipelines, portability, simulation.

### Option B: SDK Bridge

```{graphviz}
digraph B {
    rankdir=LR;
    node [shape=box, style=filled, fillcolor="#E8F0FE"];

    User [label="User Application", fillcolor="#FFF3E0"];
    Bridge [label="dg_sdk_ros2_bridge"];
    SDK [label="DG SDK\n(libDGSDK.so)"];
    FW [label="Gripper\nFirmware", fillcolor="#E8F5E9"];

    User -> Bridge [label="services / topics"];
    Bridge -> SDK;
    SDK -> FW;
}
```

**How it works:**

1. **Your application** calls ROS 2 services or reads topics from the bridge node.
2. The **dg_sdk_ros2_bridge** node translates these into DG SDK function calls.
3. The **DG SDK** (`libDGSDK.so`) handles communication with the gripper firmware.

**Best for:** Direct SDK access, recipe management, advanced PID tuning, blend motion, TCP space control, in-hand manipulation.

```{warning}
Both paths communicate with the gripper over TCP. Since the gripper only accepts a single connection, you must choose one path per gripper instance. Running both simultaneously will cause connection errors.
```

## Decision Guide

| Question | ros2_control | SDK Bridge |
|----------|-------------|------------|
| Are you using MoveIt? | Yes | No |
| Do you need simulation? | Yes (Gazebo) | No |
| Do you need recipe management? | No | Yes |
| Do you want standard ROS 2 interfaces? | Yes | Partially |
| ARM64 / Jetson support? | Partially | No (x86_64 only) |

## Submodule Strategy

Each gripper model is an independent git repository, allowing:

- **Per-model deployment** -- Clone only the gripper model you need, saving download time and disk space
- **Independent versioning** -- Each model is released and tagged separately, so updates to one model do not affect others
- **Unified development** -- The parent repository (`tesollo_ros2`) aggregates everything for developers who work across models
