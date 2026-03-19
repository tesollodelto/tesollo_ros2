# Installation

## Prerequisites

- Ubuntu 22.04
- ROS 2 Humble
- `ros2_control` packages

```bash
sudo apt install ros-humble-ros2-control ros-humble-ros2-controllers
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

# Example: DG-5F only
git submodule update --init dg5f_ros2 dg_sdk_ros2_bridge dg_common/dg_tcp_comm dg_common/dg_hardware
```

## Build

```bash
cd ~/ros2_ws
colcon build --symlink-install
source install/setup.bash
```
