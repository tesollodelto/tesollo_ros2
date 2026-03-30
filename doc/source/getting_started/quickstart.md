# Quick Start

```{note}
Make sure you have completed the [Installation](installation.md) steps and sourced your workspace (`source install/setup.bash`).
```

## 1. Launch a Gripper

Each gripper model provides launch files for `ros2_control`. Pick the one matching your model:

**DG-5F**

```bash
ros2 launch dg5f_driver dg5f_right_driver.launch.py
```

```bash
ros2 launch dg5f_driver dg5f_left_driver.launch.py
```

**DG-5F-S (20-DOF)**

```bash
ros2 launch dg5f_s_driver dg5f_s_right_driver.launch.py
```

**DG-5F-S (15-DOF)**

```bash
ros2 launch dg5f_s_driver dg5f_s_15dof_right_driver.launch.py
```

**DG-4F**

```bash
ros2 launch dg4f_driver dg4f_driver.launch.py
```

**DG-3F-M**

```bash
ros2 launch dg3f_m_driver dg3f_m_driver.launch.py
```

### Launch Arguments

All driver launch files accept the following arguments:

| Argument | Default | Description |
|----------|---------|-------------|
| `delto_ip` | `169.254.186.72` | Gripper IP address |
| `delto_port` | `502` | Gripper TCP port |
| `fingertip_sensor` | `false` | Enable fingertip sensor data |
| `ft_broadcaster` | `false` | Enable F/T sensor broadcaster (force/torque only) |
| `io` | `false` | Enable GPIO |

```bash
ros2 launch dg5f_driver dg5f_right_driver.launch.py delto_ip:=192.168.1.100 fingertip_sensor:=true
```

## 2. Verify Connection

Check that the hardware interface is active:

```bash
ros2 control list_hardware_interfaces
```

You should see `position`, `velocity`, `effort`, and `temperature` state interfaces and `effort` command interfaces listed for each joint.

## 3. Send Commands

The gripper uses effort (current) command interface:

```bash
ros2 topic pub /effort_controller/commands std_msgs/msg/Float64MultiArray "data: [0.0, 0.0, ...]"
```

## 4. RViz Visualization

View the gripper model in RViz with interactive joint sliders:

**DG-5F**
```bash
ros2 launch dg5f_description dg5f_right_display.launch.py
```

**DG-5F-S**
```bash
ros2 launch dg5f_s_description dg5f_s_right_display.launch.py
```

**DG-4F**
```bash
ros2 launch dg4f_description dg4f_display.launch.py
```

**DG-3F-M**
```bash
ros2 launch dg3f_m_description dg3f_m_display.launch.py
```

## 5. Gazebo Simulation

Launch the gripper in Gazebo for simulation:

**DG-5F**
```bash
ros2 launch dg5f_gz dg5f_right_gz.launch.py
```

**DG-5F-S**
```bash
ros2 launch dg5f_s_gz dg5f_s_right_gz.launch.py
```

**DG-4F**
```bash
ros2 launch dg4f_gz dg4f_gz.launch.py
```

**DG-3F-M**
```bash
ros2 launch dg3f_m_gz dg3f_m_gz.launch.py
```

## 6. F/T Sensor Calibration

Zero the fingertip force/torque sensors (DG-3F-M, DG-4F, DG-5F, DG-5F-S):

```bash
ros2 service call /<namespace>/delto_hardware_interface_node/set_ft_sensor_offset std_srvs/srv/Trigger
```

Replace `<namespace>` with your gripper's namespace (e.g., `dg5f_right`, `dg4f`, `dg3f_m`).

## 7. GPIO Control

Motor ON:

```bash
ros2 service call /<namespace>/set_gpio_output1 std_srvs/srv/SetBool "{data: true}"
```

Grasp:

```bash
ros2 service call /<namespace>/set_gpio_output2 std_srvs/srv/SetBool "{data: true}"
```

Release:

```bash
ros2 service call /<namespace>/set_gpio_output3 std_srvs/srv/SetBool "{data: true}"
```
