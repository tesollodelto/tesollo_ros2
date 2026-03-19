<div align="center">
  <img src="doc/source/_static/title.svg" alt="tesollo_ros2" width="600" />
  <p>
    Delto Gripper ROS 2 Packages for Tesollo's Robotic Grippers
  </p>
  <p>
    <a href="https://github.com/tesollodelto/tesollo_ros2">
      <img src="https://img.shields.io/badge/GitHub-tesollo__ros2-181717?logo=github" alt="GitHub" />
    </a>
    <img src="https://img.shields.io/badge/ROS_2-Humble-blue?logo=ros" alt="ROS 2 Humble" />
    <img src="https://img.shields.io/badge/Ubuntu-22.04-E95420?logo=ubuntu&logoColor=white" alt="Ubuntu 22.04" />
    <img src="https://img.shields.io/badge/License-BSD--3--Clause-green" alt="License" />
  </p>
  <p>
    <a href="https://tesollodelto.github.io/tesollo_ros2/"><strong>📖 Documentation</strong></a>
  </p>
</div>

## Architecture

This repository uses a **per-model submodule** structure with shared common packages:

- Each gripper model (`dg3f_b`, `dg3f_m`, `dg4f`, `dg5f`, `dg5fs`) has its own independent repository, included here as a git submodule.
- `dg_common` contains shared packages: `dg_tcp_comm` and `dg_hardware` (managed as submodules).
- `dg_sdk_ros2_bridge` bridges the Delto SDK with ROS 2, and includes `dg_msgs` (message/service definitions).

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
├── dg_common/            # Shared packages
│   ├── dg_hardware/          # [submodule] ros2_control hardware interface
│   └── dg_tcp_comm/          # [submodule] TCP communication library
│
└── dg_sdk_ros2_bridge/   # [submodule] Delto SDK ↔ ROS 2 bridge
    ├── dg_msgs/              # Custom messages / services
    └── dg_sdk_ros2_bridge/   # Bridge node
```

## Clone

### All models

```bash
git clone --recursive https://github.com/tesollodelto/tesollo_ros2.git
```

### Single model only

Clone the repo and initialize only the submodule you need. You must also initialize the shared submodules under `dg_common` (`dg_tcp_comm`, `dg_hardware`).

> **Note:** Other model folders (e.g. `dg3f_b_ros2/`, `dg4f_ros2/`) will appear as empty directories. This is normal git submodule behavior — they take no disk space and are ignored by `colcon build`.

**DG-3F (B type)**
```bash
git clone https://github.com/tesollodelto/tesollo_ros2.git
cd tesollo_ros2
git submodule update --init dg3f_b_ros2 dg_common/dg_tcp_comm dg_common/dg_hardware
```

**DG-3F (M type)**
```bash
git clone https://github.com/tesollodelto/tesollo_ros2.git
cd tesollo_ros2
git submodule update --init dg3f_m_ros2 dg_common/dg_tcp_comm dg_common/dg_hardware
```

**DG-4F**
```bash
git clone https://github.com/tesollodelto/tesollo_ros2.git
cd tesollo_ros2
git submodule update --init dg4f_ros2 dg_common/dg_tcp_comm dg_common/dg_hardware
```

**DG-5F**
```bash
git clone https://github.com/tesollodelto/tesollo_ros2.git
cd tesollo_ros2
git submodule update --init dg5f_ros2 dg_common/dg_tcp_comm dg_common/dg_hardware
```

**DG-5FS (S model)**
```bash
git clone https://github.com/tesollodelto/tesollo_ros2.git
cd tesollo_ros2
git submodule update --init dg5f_s_ros2 dg_common/dg_tcp_comm dg_common/dg_hardware
```

## License

BSD-3-Clause - Copyright (c) 2025 Tesollo Inc.
