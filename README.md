# tesollo_ros2

Delto Gripper ROS 2 packages for Tesollo's robotic grippers.

## Architecture

This repository uses a **per-model submodule** structure with shared common packages:

- Each gripper model (`dg3f_b`, `dg3f_m`, `dg4f`, `dg5f`, `dg5fs`) has its own independent repository, included here as a git submodule.
- `dg_common` provides shared packages (hardware interface, messages, TCP communication) used across all models.
- `dg_sdk_ros2_bridge` bridges the Delto SDK with ROS 2.

### Why this structure?

- **Individual product deployment** - Customers using only one model can clone this repo and initialize only the submodule they need.
- **Independent versioning** - Each model can be released and tagged independently.
- **Full integration** - This parent repo aggregates everything for development and CI.

## Repository Structure

```
tesollo_ros2/
├── dg3f_b_ros2/          # [submodule] DG-3F (B type) 3-finger gripper
│   ├── dg3f_b_description/
│   └── dg3f_b_driver/
│
├── dg3f_m_ros2/          # [submodule] DG-3F (M type) 3-finger gripper
│   ├── dg3f_m_description/
│   ├── dg3f_m_driver/
│   └── dg3f_m_gz/
│
├── dg4f_ros2/            # [submodule] DG-4F 4-finger gripper
│   ├── dg4f_description/
│   ├── dg4f_driver/
│   └── dg4f_gz/
│
├── dg5f_ros2/            # [submodule] DG-5F 5-finger gripper (includes short variant)
│   ├── dg5f_description/
│   ├── dg5f_driver/
│   └── dg5f_gz/
│
├── dg5f_s_ros2/          # [submodule] DG-5FS (S model, separate product)
│   └── dg5f_s_description/
│
├── dg_common/            # Shared packages (included directly)
│   ├── dg_hardware/          # ros2_control hardware interface
│   ├── dg_msgs/              # Custom messages / services
│   └── dg_tcp_comm/          # TCP communication library
│
└── dg_sdk_ros2_bridge/   # Delto SDK ↔ ROS 2 bridge (included directly)
```

## Clone

### All models

```bash
git clone --recursive https://github.com/tesollodelto/tesollo_ros2.git
```

### Single model only

Clone the repo and initialize only the submodule you need. `dg_common` and `dg_sdk_ros2_bridge` are always included.

> **Note:** Other model folders (e.g. `dg3f_b_ros2/`, `dg4f_ros2/`) will appear as empty directories. This is normal git submodule behavior — they take no disk space and are ignored by `colcon build`.

**DG-3F (B type)**
```bash
git clone https://github.com/tesollodelto/tesollo_ros2.git
cd tesollo_ros2
git submodule update --init dg3f_b_ros2
```

**DG-3F (M type)**
```bash
git clone https://github.com/tesollodelto/tesollo_ros2.git
cd tesollo_ros2
git submodule update --init dg3f_m_ros2
```

**DG-4F**
```bash
git clone https://github.com/tesollodelto/tesollo_ros2.git
cd tesollo_ros2
git submodule update --init dg4f_ros2
```

**DG-5F**
```bash
git clone https://github.com/tesollodelto/tesollo_ros2.git
cd tesollo_ros2
git submodule update --init dg5f_ros2
```

**DG-5FS (S model)**
```bash
git clone https://github.com/tesollodelto/tesollo_ros2.git
cd tesollo_ros2
git submodule update --init dg5f_s_ros2
```

## License

BSD-3-Clause - Copyright (c) 2025 Tesollo Inc.
