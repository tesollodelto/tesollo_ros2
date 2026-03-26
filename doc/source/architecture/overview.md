# Architecture Overview

## Repository Structure

```
tesollo_ros2/
├── dg3f_b_ros2/          [submodule] DG-3F (B type)
├── dg3f_m_ros2/          [submodule] DG-3F (M type)
├── dg4f_ros2/            [submodule] DG-4F
├── dg5f_ros2/            [submodule] DG-5F
├── dg5f_s_ros2/          [submodule] DG-5F-S small hand (20-DOF / 15-DOF)
├── dg_common/
│   ├── dg_hardware/      [submodule] ros2_control hardware interface
│   └── dg_tcp_comm/      [submodule] TCP communication library
└── dg_sdk_ros2_bridge/   [submodule] SDK bridge + dg_msgs
```

## Two Integration Paths

The gripper accepts only one TCP connection at a time. Choose **one** of the two paths below — they cannot be used simultaneously.

### Option A: ros2_control (Recommended)

```{graphviz}
digraph A {
    rankdir=LR;
    node [shape=box, style=filled, fillcolor="#E8F0FE"];

    User [label="User Application", fillcolor="#FFF3E0"];
    RC [label="ros2_control\nController"];
    HW [label="delto_hardware"];
    TCP [label="dg_tcp_comm"];
    FW [label="Gripper\nFirmware", fillcolor="#E8F5E9"];

    User -> RC [label="position / effort\ncommands"];
    RC -> HW [label="effort command\ninterface"];
    HW -> TCP [label="TCP protocol"];
    TCP -> FW;
}
```

The controller accepts **position** or **effort** commands from the user depending on the controller type (e.g. `joint_trajectory_controller`, `effort_controller`, `pid_controller`). The hardware interface always outputs **effort (current) commands** to the gripper firmware.

Best for: MoveIt integration, standard ROS 2 control pipelines.

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

Best for: Direct SDK access, recipe management, advanced configuration.

```{note}
Both paths communicate with the gripper over TCP. Since the gripper only accepts a single connection, you must choose one path per gripper instance. Running both simultaneously will cause connection errors.
```

## Submodule Strategy

Each gripper model is an independent git repository, allowing:

- **Per-model deployment** — Customers clone only what they need
- **Independent versioning** — Each model released and tagged separately
- **Unified development** — This parent repo aggregates everything
