# Installation

## Prerequisites

| | Humble | Jazzy |
|---|---|---|
| Ubuntu | 22.04 | 24.04 |
| ROS 2 | Humble Hawksbill | Jazzy Jalisco |

Install `ros2_control` packages:

```bash
# Humble
sudo apt install ros-humble-ros2-control ros-humble-ros2-controllers

# Jazzy
sudo apt install ros-jazzy-ros2-control ros-jazzy-ros2-controllers
```

For Gazebo simulation, also install:

```bash
# Humble
sudo apt install ros-humble-ros-gz

# Jazzy
sudo apt install ros-jazzy-ros-gz
```

## Clone Repository

### All models

```bash
cd ~/ros2_ws/src
git clone --recursive https://github.com/tesollodelto/tesollo_ros2.git
```

### Single model only

Clone and initialize only the submodule you need. Shared submodules (`dg_tcp_comm`, `dg_hardware`) are always required.

```bash
cd ~/ros2_ws/src
git clone https://github.com/tesollodelto/tesollo_ros2.git
cd tesollo_ros2
```

Then initialize the submodules for your model:

```bash
# DG-5F
git submodule update --init dg5f_ros2 dg_common/dg_tcp_comm dg_common/dg_hardware

# DG-5F-S
git submodule update --init dg5f_s_ros2 dg_common/dg_tcp_comm dg_common/dg_hardware

# DG-4F
git submodule update --init dg4f_ros2 dg_common/dg_tcp_comm dg_common/dg_hardware

# DG-3F-M
git submodule update --init dg3f_m_ros2 dg_common/dg_tcp_comm dg_common/dg_hardware
```

```{note}
Other model folders (e.g. `dg3f_b_ros2/`, `dg4f_ros2/`) will appear as empty directories. This is normal git submodule behavior -- they take no disk space and are ignored by `colcon build`.
```

### Standalone (without parent repo)

Each gripper model can also be cloned independently. You must clone the shared dependencies alongside it:

```bash
cd ~/ros2_ws/src
git clone https://github.com/tesollodelto/dg5f_ros2.git
git clone https://github.com/tesollodelto/dg_hardware.git
git clone https://github.com/tesollodelto/dg_tcp_comm.git
```

## Build

```bash
cd ~/ros2_ws
colcon build --symlink-install
source install/setup.bash
```

To build only a specific model:

```bash
# Example: DG-5F only
colcon build --packages-select dg5f_description dg5f_driver dg5f_gz delto_hardware delto_tcp_comm
```

## Network Configuration

The gripper communicates over TCP/IP. Configure your network interface to match the gripper's subnet:

| Parameter | Default Value |
|-----------|---------------|
| Gripper IP | `169.254.186.72` |
| Port | `502` |

Set a static IP on the connected interface:

```bash
# Example: set 169.254.186.1 on eth0
sudo ip addr add 169.254.186.1/16 dev eth0
```

Verify connectivity:

```bash
ping 169.254.186.72
```
