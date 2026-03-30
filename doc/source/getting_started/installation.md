# Installation

## Prerequisites

| | Humble | Jazzy |
|---|---|---|
| Ubuntu | 22.04 | 24.04 |
| ROS 2 | Humble Hawksbill | Jazzy Jalisco |

Install `ros2_control` packages:

**Humble**

```bash
sudo apt install ros-humble-ros2-control ros-humble-ros2-controllers
```

**Jazzy**

```bash
sudo apt install ros-jazzy-ros2-control ros-jazzy-ros2-controllers
```

For Gazebo simulation:

**Humble**

```bash
sudo apt install ros-humble-ros-gz
```

**Jazzy**

```bash
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

**DG-5F**
```bash
git submodule update --init dg5f_ros2 dg_common/dg_tcp_comm dg_common/dg_hardware
```

**DG-5F-S**
```bash
git submodule update --init dg5f_s_ros2 dg_common/dg_tcp_comm dg_common/dg_hardware
```

**DG-4F**
```bash
git submodule update --init dg4f_ros2 dg_common/dg_tcp_comm dg_common/dg_hardware
```

**DG-3F-M**
```bash
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
```

```bash
source install/setup.bash
```

## Network Configuration

The gripper communicates over TCP/IP. Configure your network interface to match the gripper's subnet:

| Parameter | Default Value |
|-----------|---------------|
| Gripper IP | `169.254.186.72` |
| Port | `502` |

Set a static IP on the connected interface:

```bash
sudo ip addr add 169.254.186.1/16 dev <your-interface>
```

```{note}
Replace `<your-interface>` with your actual interface name (e.g., `enp0s3`). Run `ip link show` to find it.
```

Verify connectivity:

```bash
ping 169.254.186.72
```

### Custom IP / Port

To use a different gripper IP or port, edit the URDF xacro hardware parameters:

```xml
<ros2_control name="DeltoGripperSystem" type="system">
  <hardware>
    <plugin>delto_hardware/SystemInterface</plugin>
    <param name="delto_ip">169.254.186.72</param>
    <param name="delto_port">502</param>
  </hardware>
  ...
</ros2_control>
```

Or pass them as launch arguments (if the launch file supports it):

```bash
ros2 launch dg5f_driver dg5f_right_driver.launch.py delto_ip:=192.168.1.100 delto_port:=502
```
