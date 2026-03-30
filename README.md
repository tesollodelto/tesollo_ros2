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
    <img src="https://img.shields.io/badge/ROS_2-Jazzy-blue?logo=ros" alt="ROS 2 Jazzy" />
    <img src="https://img.shields.io/badge/License-BSD--3--Clause-green" alt="License" />
  </p>
  <p>
    <a href="https://tesollodelto.github.io/tesollo_ros2/"><strong>Documentation</strong></a>
  </p>
</div>

## CI Status

**Gripper Packages**

| Package | Description | Build |
|---|---|---|
| [dg5f_ros2](https://github.com/tesollodelto/dg5f_ros2) | DG-5F 5-finger hand | [![CI](https://github.com/tesollodelto/dg5f_ros2/actions/workflows/ci.yml/badge.svg)](https://github.com/tesollodelto/dg5f_ros2/actions/workflows/ci.yml) |
| [dg5f_s_ros2](https://github.com/tesollodelto/dg5f_s_ros2) | DG-5F-S small hand | [![CI](https://github.com/tesollodelto/dg5f_s_ros2/actions/workflows/ci.yml/badge.svg)](https://github.com/tesollodelto/dg5f_s_ros2/actions/workflows/ci.yml) |
| [dg4f_ros2](https://github.com/tesollodelto/dg4f_ros2) | DG-4F 4-finger gripper | [![CI](https://github.com/tesollodelto/dg4f_ros2/actions/workflows/ci.yml/badge.svg)](https://github.com/tesollodelto/dg4f_ros2/actions/workflows/ci.yml) |
| [dg3f_m_ros2](https://github.com/tesollodelto/dg3f_m_ros2) | DG-3F-M 3-finger gripper | [![CI](https://github.com/tesollodelto/dg3f_m_ros2/actions/workflows/ci.yml/badge.svg)](https://github.com/tesollodelto/dg3f_m_ros2/actions/workflows/ci.yml) |
| [dg3f_b_ros2](https://github.com/tesollodelto/dg3f_b_ros2) | DG-3F-B 3-finger (discontinued) | [![CI](https://github.com/tesollodelto/dg3f_b_ros2/actions/workflows/ci.yml/badge.svg)](https://github.com/tesollodelto/dg3f_b_ros2/actions/workflows/ci.yml) |

**Common Packages**

| Package | Description | Build |
|---|---|---|
| [delto_hardware](https://github.com/tesollodelto/dg_hardware) | ros2_control hardware interface | [![CI](https://github.com/tesollodelto/dg_hardware/actions/workflows/ci.yml/badge.svg)](https://github.com/tesollodelto/dg_hardware/actions/workflows/ci.yml) |
| [delto_tcp_comm](https://github.com/tesollodelto/dg_tcp_comm) | TCP communication library | [![CI](https://github.com/tesollodelto/dg_tcp_comm/actions/workflows/ci.yml/badge.svg)](https://github.com/tesollodelto/dg_tcp_comm/actions/workflows/ci.yml) |
| [dg_sdk_ros2_bridge](https://github.com/tesollodelto/dg_sdk_ros2_bridge) | DG SDK bridge + dg_msgs | [![CI](https://github.com/tesollodelto/dg_sdk_ros2_bridge/actions/workflows/ci.yml/badge.svg)](https://github.com/tesollodelto/dg_sdk_ros2_bridge/actions/workflows/ci.yml) |

## Architecture

This repository uses a **per-model submodule** structure with shared common packages:

- Each gripper model (`dg3f_b`, `dg3f_m`, `dg4f`, `dg5f`, `dg5f_s`) has its own independent repository, included here as a git submodule.
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
├── dg5f_ros2/            # [submodule] DG-5F 5-finger hand
│   ├── dg5f_description/
│   ├── dg5f_driver/
│   └── dg5f_gz/
│
├── dg5f_s_ros2/          # [submodule] DG-5F-S small hand (20-DOF / 15-DOF)
│   ├── dg5f_s_description/
│   ├── dg5f_s_driver/
│   └── dg5f_s_gz/
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

```bash
git clone https://github.com/tesollodelto/tesollo_ros2.git
cd tesollo_ros2

# Pick one:
git submodule update --init dg5f_ros2   dg_common/dg_tcp_comm dg_common/dg_hardware  # DG-5F
git submodule update --init dg5f_s_ros2 dg_common/dg_tcp_comm dg_common/dg_hardware  # DG-5F-S
git submodule update --init dg4f_ros2   dg_common/dg_tcp_comm dg_common/dg_hardware  # DG-4F
git submodule update --init dg3f_m_ros2 dg_common/dg_tcp_comm dg_common/dg_hardware  # DG-3F-M
git submodule update --init dg3f_b_ros2 dg_common/dg_tcp_comm dg_common/dg_hardware  # DG-3F-B

# Optional: include dg_sdk_ros2_bridge if you need the Delto SDK bridge
git submodule update --init dg_sdk_ros2_bridge
```

## Build

```bash
cd ~/ros2_ws
colcon build --symlink-install
source install/setup.bash
```

## License

BSD-3-Clause - Copyright (c) 2025-2026 Tesollo Inc.
