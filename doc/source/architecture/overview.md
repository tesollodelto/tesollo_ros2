# Architecture Overview

## Repository Structure

```
tesollo_ros2/
├── dg3f_b_ros2/          [submodule] DG-3F (B type)
├── dg3f_m_ros2/          [submodule] DG-3F (M type)
├── dg4f_ros2/            [submodule] DG-4F
├── dg5f_ros2/            [submodule] DG-5F
├── dg5f_s_ros2/          [submodule] DG-5FS
├── dg_common/
│   ├── dg_hardware/      [submodule] ros2_control hardware interface
│   └── dg_tcp_comm/      [submodule] TCP communication library
└── dg_sdk_ros2_bridge/   [submodule] SDK bridge + dg_msgs
```

## Two Integration Paths

```{graphviz}
digraph G {
    rankdir=LR;
    node [shape=box, style=filled, fillcolor="#E8F0FE"];

    User [label="User Application", fillcolor="#FFF3E0"];
    RC [label="ros2_control"];
    HW [label="delto_hardware"];
    TCP [label="dg_tcp_comm"];
    FW [label="Gripper\nFirmware", fillcolor="#E8F5E9"];

    Bridge [label="dg_sdk_ros2_bridge"];
    SDK [label="DG SDK\n(libDGSDK.so)"];

    User -> RC [label="effort commands"];
    RC -> HW;
    HW -> TCP [label="TCP protocol"];
    TCP -> FW;

    User -> Bridge [label="services / topics"];
    Bridge -> SDK;
    SDK -> FW;
}
```

### Path 1: ros2_control (Recommended)

`User` → `ros2_control` → `delto_hardware` → `dg_tcp_comm` → `Gripper`

Best for: MoveIt integration, standard ROS 2 control pipelines.

### Path 2: SDK Bridge

`User` → `dg_sdk_ros2_bridge` → `DG SDK` → `Gripper`

Best for: Direct SDK access, recipe management, advanced configuration.

## Submodule Strategy

Each gripper model is an independent git repository, allowing:

- **Per-model deployment** — Customers clone only what they need
- **Independent versioning** — Each model released and tagged separately
- **Unified development** — This parent repo aggregates everything
