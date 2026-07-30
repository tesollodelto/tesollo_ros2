# Installation

This guide walks you through installing the Tesollo ROS 2 gripper packages from scratch. By the end, you will have a fully built workspace ready to control your Delto gripper.

## Prerequisites

Before you begin, make sure you have:

- **Ubuntu** with **ROS 2** installed and working. The supported combinations are:

  | Ubuntu Version | ROS 2 Distribution |
  |---|---|
  | 22.04 (Jammy) | Humble Hawksbill |
  | 24.04 (Noble) | Jazzy Jalisco |

- A terminal where you can run `ros2 --help` without errors. If that command fails, your ROS 2 installation is not set up correctly -- revisit the [official ROS 2 installation guide](https://docs.ros.org/en/humble/Installation.html) first.

```{note}
**What is ROS 2?** ROS 2 (Robot Operating System 2) is a framework for building robot software. It provides tools for communication between different parts of a robot system. If this is your first time using ROS 2, the [ROS 2 tutorials](https://docs.ros.org/en/humble/Tutorials.html) are a great place to start.
```

## Step 1: Install Required System Packages

The gripper drivers use **ros2_control**, which is the standard ROS 2 framework for controlling hardware. You need to install it along with its controller plugins.

**For ROS 2 Humble (Ubuntu 22.04):**

```bash
sudo apt update
sudo apt install ros-humble-ros2-control ros-humble-ros2-controllers
```

**For ROS 2 Jazzy (Ubuntu 24.04):**

```bash
sudo apt update
sudo apt install ros-jazzy-ros2-control ros-jazzy-ros2-controllers
```

**For ROS 2 Lyrical (experimental):**

```bash
sudo apt update
sudo apt install ros-lyrical-ros2-control ros-lyrical-ros2-controllers
```

```{note}
Lyrical support is experimental: the packages build in CI but are not validated
on hardware. Use Humble or Jazzy for production.
```

```{note}
**What does this install?** `ros2-control` is the hardware abstraction layer that lets ROS 2 talk to motors and sensors in a standardized way. `ros2-controllers` provides ready-made controllers like the Joint Trajectory Controller (for position control) and the Effort Controller (for direct current/torque control). The Delto gripper drivers rely on both of these.
```

### Optional: Gazebo Simulation

If you want to test your gripper in simulation before connecting real hardware, install Gazebo:

**For ROS 2 Humble:**

```bash
sudo apt install ros-humble-ros-gz
```

**For ROS 2 Jazzy:**

```bash
sudo apt install ros-jazzy-ros-gz
```

## Step 2: Create a Workspace

If you already have a ROS 2 workspace (for example `~/ros2_ws`), you can skip this step.

A **workspace** is a directory where you put all your ROS 2 packages. Here is how to create one:

```bash
mkdir -p ~/ros2_ws/src
```

```{note}
**What is a workspace?** In ROS 2, a workspace is a folder structure where your source code lives (in the `src/` directory). The build tool `colcon` compiles everything inside `src/` and puts the results in `build/`, `install/`, and `log/` directories. You can name your workspace anything, but `ros2_ws` is the most common convention.
```

## Step 3: Clone the Repository

Navigate into the `src/` directory of your workspace:

```bash
cd ~/ros2_ws/src
```

### Option A: Clone All Gripper Models (Recommended for First-Time Users)

This downloads everything -- all gripper models and shared libraries -- in one step:

```bash
git clone --recursive https://github.com/tesollodelto/tesollo_ros2.git
```

The `--recursive` flag tells git to also download all the **submodules** (each gripper model and shared library is stored in its own git repository).

### Option B: Clone Only the Gripper Model You Need

If you only need one gripper model, you can save download time by initializing only the submodules you need. The shared submodules (`dg_tcp_comm` and `dg_hardware`) are always required.

```bash
cd ~/ros2_ws/src
git clone https://github.com/tesollodelto/tesollo_ros2.git
cd tesollo_ros2
```

Then run the command for your specific model:

**DG-5F (5-finger hand):**

```bash
git submodule update --init dg5f_ros2 dg_common/dg_tcp_comm dg_common/dg_hardware
```

**DG-5F-S (5-finger hand, small version):**

```bash
git submodule update --init dg5f_s_ros2 dg_common/dg_tcp_comm dg_common/dg_hardware
```

**DG-4F (4-finger gripper):**

```bash
git submodule update --init dg4f_ros2 dg_common/dg_tcp_comm dg_common/dg_hardware
```

**DG-3F-M (3-finger gripper):**

```bash
git submodule update --init dg3f_m_ros2 dg_common/dg_tcp_comm dg_common/dg_hardware
```

```{note}
After this step, you will see empty directories for other models (e.g., `dg4f_ros2/`, `dg3f_b_ros2/`). This is normal -- empty submodule directories take no disk space and are ignored by the build tool.
```

### Option C: Clone a Single Model Independently

Each gripper model can also be cloned as a standalone repository. You must also clone the shared dependencies alongside it:

```bash
cd ~/ros2_ws/src
git clone https://github.com/tesollodelto/dg5f_ros2.git
git clone https://github.com/tesollodelto/dg_hardware.git
git clone https://github.com/tesollodelto/dg_tcp_comm.git
```

Replace `dg5f_ros2` with your model: `dg5f_s_ros2`, `dg4f_ros2`, `dg3f_m_ros2`, `dg3f_b_ros2`, `dg2f_ros2` or `dg1f_ros2`.

```{warning}
Pick this layout **or** the `tesollo_ros2` umbrella layout above, never both. `dg_hardware` and `dg_tcp_comm` are already included as submodules under `dg_common/` there, and a second copy in the same workspace makes `colcon` fail with a duplicate package name.
```

## Step 4: Install Dependencies with rosdep

Before building, let ROS 2 automatically install any missing dependencies:

```bash
cd ~/ros2_ws
rosdep install --from-paths src --ignore-src -r -y
```

```{note}
**What does `rosdep` do?** `rosdep` reads the `package.xml` files in your source code and installs any system packages they depend on. The flags mean:
- `--from-paths src` -- look at packages in the `src/` directory
- `--ignore-src` -- don't try to install packages that are already in your source tree
- `-r` -- continue even if some packages can't be resolved
- `-y` -- answer "yes" to all installation prompts
```

## Step 5: Build the Workspace

Now compile everything:

```bash
cd ~/ros2_ws
colcon build --symlink-install
```

This will take a minute or two. You should see output showing each package being built.

```{note}
**What does `colcon build` do?** `colcon` is the ROS 2 build tool. It finds all the packages in your `src/` directory and compiles them in the correct order (respecting dependencies). The `--symlink-install` flag creates symbolic links instead of copying files, which means you can edit Python scripts and config files without rebuilding.
```

## Step 6: Source the Workspace

After a successful build, you need to **source** the workspace so that ROS 2 can find your newly built packages:

```bash
source ~/ros2_ws/install/setup.bash
```

```{warning}
You must run this command in **every new terminal** you open, or add it to your `~/.bashrc` file so it runs automatically:

    echo "source ~/ros2_ws/install/setup.bash" >> ~/.bashrc
```

## Step 7: Verify the Installation

Check that the gripper packages are visible to ROS 2:

```bash
ros2 pkg list | grep dg
```

You should see output similar to this (depending on which models you installed):

```
dg3f_m_description
dg3f_m_driver
dg3f_m_gz
dg4f_description
dg4f_driver
dg4f_gz
dg5f_description
dg5f_driver
dg5f_gz
dg5f_s_description
dg5f_s_driver
dg5f_s_gz
delto_hardware
delto_tcp_comm
```

If you see your gripper model's packages listed, congratulations -- your installation is complete.

## Platform Limitations

### ARM64 / NVIDIA Jetson

The `dg_sdk_ros2_bridge` package includes a proprietary SDK library (`libDGSDK.so`) that is only available for **x86_64** architectures. If you are building on an ARM64 platform (such as NVIDIA Jetson), this package will fail to build.

**Workaround:** Exclude the SDK bridge package from your build:

```bash
colcon build --symlink-install --packages-ignore dg_sdk_ros2_bridge dg_msgs
```

This does not affect the main `ros2_control`-based drivers (`delto_hardware`), which work on all platforms.

## Troubleshooting

### "Could not find a package configuration file provided by ros2_control"

This means the ros2_control packages are not installed. Install them:

```bash
# For Humble:
sudo apt install ros-humble-ros2-control ros-humble-ros2-controllers

# For Jazzy:
sudo apt install ros-jazzy-ros2-control ros-jazzy-ros2-controllers
```

### "Could not find a package configuration file provided by controller_manager"

Same as above -- install `ros2-control`:

```bash
sudo apt install ros-${ROS_DISTRO}-ros2-control ros-${ROS_DISTRO}-ros2-controllers
```

### Build fails with "package not found" for dg5f_driver (or similar)

If you used Option B (single model), make sure you also initialized the shared submodules:

```bash
cd ~/ros2_ws/src/tesollo_ros2
git submodule update --init dg_common/dg_tcp_comm dg_common/dg_hardware
```

### "colcon build" produces no output or finishes instantly

Make sure you are in the workspace root (`~/ros2_ws`), not inside the `src/` directory. `colcon` looks for packages in the `src/` subdirectory of your current directory.

### "ros2 pkg list" does not show my packages

Make sure you sourced the workspace:

```bash
source ~/ros2_ws/install/setup.bash
```

If you added this to `~/.bashrc`, open a new terminal and try again.

## Next Steps

Now that your workspace is built, head to the {doc}`quickstart` guide to connect to your gripper and send your first commands.
