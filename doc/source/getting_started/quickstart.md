# Quick Start

## 1. Launch a Gripper

Each gripper model provides launch files for `ros2_control`:

```bash
# DG-5F
ros2 launch dg5f_driver dg5f_left_driver.launch.py
ros2 launch dg5f_driver dg5f_right_driver.launch.py

# DG-5F-S (20-DOF)
ros2 launch dg5f_s_driver dg5f_s_right_driver.launch.py

# DG-5F-S (15-DOF)
ros2 launch dg5f_s_driver dg5f_s_15dof_right_driver.launch.py

# DG-4F
ros2 launch dg4f_driver dg4f_driver.launch.py

# DG-3F-M
ros2 launch dg3f_m_driver dg3f_m_driver.launch.py
```

## 2. Verify Connection

Check that the hardware interface is active:

```bash
ros2 control list_hardware_interfaces
```

You should see `position`, `velocity`, `effort` state interfaces and `effort` command interfaces listed for each joint.

## 3. Send Commands

The gripper uses effort (current) command interface:

```bash
# Example: send zero current to all joints via effort controller
ros2 topic pub /effort_controller/commands std_msgs/msg/Float64MultiArray "data: [0.0, 0.0, ...]"
```

## 4. RViz Visualization

View the gripper model in RViz with interactive joint sliders:

```bash
# DG-5F
ros2 launch dg5f_description dg5f_right_display.launch.py

# DG-5F-S
ros2 launch dg5f_s_description dg5f_s_right_display.launch.py

# DG-4F
ros2 launch dg4f_description dg4f_display.launch.py

# DG-3F-M
ros2 launch dg3f_m_description dg3f_m_display.launch.py
```

## 5. Gazebo Simulation

Launch the gripper in Gazebo for simulation:

```bash
# DG-5F
ros2 launch dg5f_gz dg5f_right_gz.launch.py

# DG-5F-S
ros2 launch dg5f_s_gz dg5f_s_right_gz.launch.py

# DG-4F
ros2 launch dg4f_gz dg4f_gz.launch.py

# DG-3F-M
ros2 launch dg3f_m_gz dg3f_m_gz.launch.py
```

## 6. F/T Sensor Calibration

Zero the fingertip force/torque sensors (DG-3F-M, DG-4F, DG-5F, DG-5F-S):

```bash
ros2 service call /delto_hardware_interface_node/set_ft_sensor_offset std_srvs/srv/Trigger
```

## 7. GPIO Control

Control motor power and grasp/release via GPIO services:

```bash
# Motor ON
ros2 service call /set_gpio_output1 std_srvs/srv/SetBool "{data: true}"

# Grasp
ros2 service call /set_gpio_output2 std_srvs/srv/SetBool "{data: true}"

# Release
ros2 service call /set_gpio_output3 std_srvs/srv/SetBool "{data: true}"
```
