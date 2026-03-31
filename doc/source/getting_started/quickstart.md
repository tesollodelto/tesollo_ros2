# Quick Start

This guide walks you through connecting to a Delto gripper for the first time and sending your first commands. We will use the **DG-5F Right Hand** as the example, but the steps are similar for all models.

```{note}
Make sure you have completed the {doc}`installation` steps and sourced your workspace before continuing:

    source ~/ros2_ws/install/setup.bash
```

## Step 1: Connect the Gripper to Your Computer

The gripper communicates over a wired Ethernet connection using TCP/IP. Connect an Ethernet cable directly between your computer and the gripper.

### Configure Your Network

The gripper ships with a default IP address on a **link-local** network. You need to configure your computer's Ethernet interface to be on the same subnet.

| Setting | Default Value |
|---------|---------------|
| Gripper IP | `169.254.186.72` |
| Gripper Port | `502` |

**Step 1a: Find your Ethernet interface name.**

```bash
ip link show
```

Look for an interface that is NOT `lo` (loopback) or a wireless interface. It will typically be named something like `eth0`, `enp0s3`, `enp3s0`, or `eno1`. The interface connected to the gripper will show `state UP` if the cable is plugged in.

Example output:
```
1: lo: <LOOPBACK,UP,LOWER_UP> ...
2: enp3s0: <BROADCAST,MULTICAST,UP,LOWER_UP> ...
3: wlp2s0: <BROADCAST,MULTICAST,UP,LOWER_UP> ...
```

In this example, `enp3s0` is the Ethernet interface.

**Step 1b: Assign a static IP address on the same subnet.**

```bash
sudo ip addr add 169.254.186.1/16 dev enp3s0
```

Replace `enp3s0` with your actual interface name. The IP address `169.254.186.1` can be any address in the `169.254.x.x` range, as long as it is not the same as the gripper's IP.

```{note}
**Why 169.254.x.x?** This is a link-local address range. The gripper uses this range by default so it can communicate without needing a DHCP server or router. Your computer and the gripper just need to be on the same subnet.
```

**Step 1c: Verify the connection.**

```bash
ping 169.254.186.72
```

You should see replies like:

```
64 bytes from 169.254.186.72: icmp_seq=1 ttl=64 time=0.5 ms
```

Press `Ctrl+C` to stop the ping. If you see "Destination Host Unreachable" or no replies, check your cable connection and interface settings.

```{tip}
**Making this permanent:** The `ip addr add` command is temporary and will be lost after a reboot. To make it permanent, use your system's network manager (such as Ubuntu's Settings > Network, or `nmcli`/`nmtui` in the terminal) to set a static IP on the interface.
```

### Using a Custom IP Address

If your gripper has been configured with a different IP address, you can pass it as a launch argument (shown in the next step). You do not need to edit any config files.

## Step 2: Launch the Gripper Driver

Open a terminal, source your workspace, and launch the driver:

```bash
source ~/ros2_ws/install/setup.bash
ros2 launch dg5f_driver dg5f_right_driver.launch.py
```

```{note}
**What does this command do?** `ros2 launch` starts multiple ROS 2 nodes at once using a launch file. This particular launch file starts:

1. **ros2_control_node** -- The controller manager that loads and manages controllers
2. **robot_state_publisher** -- Publishes the robot's URDF model so other tools (like RViz) can visualize it
3. **joint_state_broadcaster** -- Reads joint positions, velocities, efforts, and temperatures from the hardware and publishes them as ROS 2 topics
4. **joint_trajectory_controller** -- A controller that accepts position commands and converts them to effort commands using a PID loop
```

If the connection is successful, you will see output like:

```
[ros2_control_node]: Loading controller 'joint_state_broadcaster'
[ros2_control_node]: Loading controller 'dg5f_right_controller'
[spawner]: Configured and activated joint_state_broadcaster
[spawner]: Configured and activated dg5f_right_controller
```

### Launch with Custom Settings

If your gripper uses a different IP address or port, or you want to enable sensors:

```bash
ros2 launch dg5f_driver dg5f_right_driver.launch.py delto_ip:=192.168.1.100 delto_port:=502 fingertip_sensor:=true ft_broadcaster:=true
```

All available launch arguments:

| Argument | Default | Description |
|----------|---------|-------------|
| `delto_ip` | `169.254.186.72` | IP address of the gripper |
| `delto_port` | `502` | TCP port of the gripper |
| `fingertip_sensor` | `false` | Enable reading fingertip F/T (force/torque) sensor data from the gripper hardware |
| `ft_broadcaster` | `false` | Enable broadcasting F/T sensor data as ROS 2 topics (requires `fingertip_sensor:=true` as well) |
| `io` | `false` | Enable GPIO (General Purpose Input/Output) interfaces |

```{warning}
To get F/T sensor data published as ROS 2 topics, you must set **both** `fingertip_sensor:=true` AND `ft_broadcaster:=true`. Setting only one is not sufficient. `fingertip_sensor` tells the hardware to read the sensor data, and `ft_broadcaster` starts the ROS 2 broadcasters that publish it.
```

## Step 3: Verify the Connection

Open a **new terminal** (keep the driver running in the first one), source the workspace, and check that everything is working.

### Check Hardware Interfaces

```bash
ros2 control list_hardware_interfaces
```

You should see a list of interfaces for each joint. For the DG-5F Right, there are 20 joints (4 per finger, 5 fingers). The output will look like:

```
command interfaces
    rj_dg_1_1/effort [available] [claimed]
    rj_dg_1_2/effort [available] [claimed]
    ...
state interfaces
    rj_dg_1_1/effort [available]
    rj_dg_1_1/position [available]
    rj_dg_1_1/temperature [available]
    rj_dg_1_1/velocity [available]
    ...
```

```{note}
**What does this output mean?**
- **command interfaces** are the ways you can send commands to the gripper. `effort` means you can send current/torque commands.
- **state interfaces** are the data you can read back from the gripper: position, velocity, effort (current), and temperature for each joint.
- `[available]` means the interface is registered and ready.
- `[claimed]` means a controller is currently using that interface.
```

### Check Active Controllers

```bash
ros2 control list_controllers
```

Expected output:

```
joint_state_broadcaster[joint_state_broadcaster/JointStateBroadcaster] active
dg5f_right_controller[joint_trajectory_controller/JointTrajectoryController] active
```

### Check Joint States

```bash
ros2 topic echo /dg5f_right/joint_states --once
```

This shows the current position, velocity, and effort (current) of all 20 joints. The `--once` flag means it will print one message and then exit.

### List All Available Topics

```bash
ros2 topic list | grep dg5f_right
```

You will see topics like:

```
/dg5f_right/controller_manager/...
/dg5f_right/dg5f_right_controller/...
/dg5f_right/joint_states
/dg5f_right/robot_description
```

## Step 4: Send Commands

The default launch file (`dg5f_right_driver.launch.py`) starts a **Joint Trajectory Controller (JTC)**. This controller accepts position targets and uses a built-in PID loop to move the joints to those positions.

### Send a Position Command via the JTC Action

The JTC uses a ROS 2 **action** to receive trajectory commands. Here is how to send a simple command that moves finger 1's first joint:

```bash
ros2 action send_goal /dg5f_right/dg5f_right_controller/follow_joint_trajectory control_msgs/action/FollowJointTrajectory "{
  trajectory: {
    joint_names: [rj_dg_1_1, rj_dg_1_2, rj_dg_1_3, rj_dg_1_4],
    points: [
      {
        positions: [0.5, 0.5, 0.5, 0.5],
        time_from_start: {sec: 2, nanosec: 0}
      }
    ]
  }
}"
```

```{note}
**What does this command do?** It sends a trajectory goal telling finger 1's four joints to move to position `0.5` radians over 2 seconds. The JTC controller will compute a smooth trajectory and send effort commands to the hardware.

**What are the joint names?** Each joint is named `rj_dg_X_Y` where:
- `r` = right hand (`l` for left)
- `X` = finger number (1-5)
- `Y` = joint number within the finger (1-4, from base to tip)
```

### Move All Joints to Zero (Home Position)

```bash
ros2 action send_goal /dg5f_right/dg5f_right_controller/follow_joint_trajectory control_msgs/action/FollowJointTrajectory "{
  trajectory: {
    joint_names: [rj_dg_1_1, rj_dg_1_2, rj_dg_1_3, rj_dg_1_4, rj_dg_2_1, rj_dg_2_2, rj_dg_2_3, rj_dg_2_4, rj_dg_3_1, rj_dg_3_2, rj_dg_3_3, rj_dg_3_4, rj_dg_4_1, rj_dg_4_2, rj_dg_4_3, rj_dg_4_4, rj_dg_5_1, rj_dg_5_2, rj_dg_5_3, rj_dg_5_4],
    points: [
      {
        positions: [0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0],
        time_from_start: {sec: 3, nanosec: 0}
      }
    ]
  }
}"
```

## Step 5: Try Other Controller Types

The default JTC controller is just one option. You can also use the **Effort Controller** or **PID Controller** by launching a different launch file. Only one controller type can be active at a time -- stop the current one first with `Ctrl+C`.

### Effort Controller (Direct Current Control)

The effort controller lets you send raw effort (current) values directly to each joint motor, with no position feedback loop.

```bash
ros2 launch dg5f_driver dg5f_right_effort_controller.launch.py
```

Send a command (20 values, one per joint):

```bash
ros2 topic pub /dg5f_right/effort_controller/commands std_msgs/msg/Float64MultiArray "{data: [0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0]}" --once
```

```{warning}
**Be careful with the effort controller.** Since there is no position feedback loop, the motors will apply whatever current you command. Start with small values (e.g., `0.1`) and increase gradually to avoid damage.
```

### PID Controller (Per-Joint Position Control)

The PID controller provides a simpler position control interface using individual `pid_controller/PidController` instances. Each joint gets its own controller that accepts a position reference and outputs effort.

**Per-joint PID (one controller per joint):**

```bash
ros2 launch dg5f_driver dg5f_right_pid_controller.launch.py
```

Send a position reference to a single joint:

```bash
ros2 topic pub /dg5f_right/rj_dg_1_1_pospid/reference std_msgs/msg/Float64 "{data: 0.5}" --once
```

**Grouped PID (single controller for all joints):**

```bash
ros2 launch dg5f_driver dg5f_right_pid_all_controller.launch.py
```

Send position references to all joints at once:

```bash
ros2 topic pub /dg5f_right/rj_dg_pospid/reference std_msgs/msg/Float64MultiArray "{data: [0.5, 0.5, 0.5, 0.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0]}" --once
```

## Step 6: RViz Visualization

View the gripper model in RViz with interactive joint sliders (no hardware connection needed):

```bash
ros2 launch dg5f_description dg5f_right_display.launch.py
```

## Step 7: Gazebo Simulation

Launch the gripper in Gazebo for testing without physical hardware:

```bash
ros2 launch dg5f_gz dg5f_right_gz.launch.py
```

## Step 8: F/T Sensor Calibration

If your gripper has fingertip force/torque sensors and you launched with `fingertip_sensor:=true`, you can zero-calibrate the sensors:

```bash
ros2 service call /dg5f_right/delto_hardware_interface_node/set_ft_sensor_offset std_srvs/srv/Trigger
```

```{note}
**When should you calibrate?** Run this service call after the gripper is powered on and settled. It sets the current sensor readings as the zero reference, so make sure nothing is touching the fingertips when you calibrate.
```

## Step 9: GPIO Control

If you launched with `io:=true`, you can control the GPIO outputs:

**Motor ON (GPIO output 1):**

```bash
ros2 service call /dg5f_right/set_gpio_output1 std_srvs/srv/SetBool "{data: true}"
```

**Grasp (GPIO output 2):**

```bash
ros2 service call /dg5f_right/set_gpio_output2 std_srvs/srv/SetBool "{data: true}"
```

**Release (GPIO output 3):**

```bash
ros2 service call /dg5f_right/set_gpio_output3 std_srvs/srv/SetBool "{data: true}"
```

## Step 10: Shutdown

To stop the gripper driver, press `Ctrl+C` in the terminal where it is running. The driver will cleanly disconnect from the gripper.

## Launch Files for Other Models

| Model | Launch Command |
|-------|----------------|
| DG-5F Left | `ros2 launch dg5f_driver dg5f_left_driver.launch.py` |
| DG-5F-S Right (20-DOF) | `ros2 launch dg5f_s_driver dg5f_s_right_driver.launch.py` |
| DG-5F-S Right (15-DOF) | `ros2 launch dg5f_s_driver dg5f_s_15dof_right_driver.launch.py` |
| DG-4F | `ros2 launch dg4f_driver dg4f_driver.launch.py` |
| DG-3F-M | `ros2 launch dg3f_m_driver dg3f_m_driver.launch.py` |

All models accept the same launch arguments (`delto_ip`, `delto_port`, `fingertip_sensor`, `ft_broadcaster`, `io`).

## Troubleshooting

### "Connection refused" or "Connection timed out"

- Verify the Ethernet cable is plugged in and the gripper is powered on.
- Check that your computer's Ethernet interface has an IP in the `169.254.x.x` range: `ip addr show`.
- Ping the gripper: `ping 169.254.186.72`.
- Make sure no other program is already connected to the gripper (the gripper only accepts one TCP connection at a time).

### "Hardware interface not available"

The gripper may not have connected successfully. Check the terminal output of the launch command for connection errors. The hardware interface implements automatic reconnection (up to 10 retries at 1-second intervals), so it may recover on its own.

### Joint states are all zeros

The gripper may still be initializing. Wait a few seconds and check again. If the problem persists, the connection may have failed silently -- restart the driver.

### "Could not find controller" error

Make sure you sourced your workspace (`source ~/ros2_ws/install/setup.bash`) in the terminal where you are running commands.

## Next Steps

- Read the detailed guide for your specific gripper model: {doc}`/grippers/dg5f`, {doc}`/grippers/dg4f`, {doc}`/grippers/dg3f_m`, {doc}`/grippers/dg5f_s`
- Learn about the {doc}`/architecture/overview` to understand how the system works
- Explore the {doc}`/packages/dg_sdk_ros2_bridge` for advanced SDK features
