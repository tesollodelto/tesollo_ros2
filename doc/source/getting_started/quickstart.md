# Quick Start

## Launch a Gripper

Each gripper model provides a launch file for `ros2_control`:

```bash
# DG-5F (left hand)
ros2 launch dg5f_driver dg5f.launch.py

# DG-4F
ros2 launch dg4f_driver dg4f.launch.py

# DG-3F (M type)
ros2 launch dg3f_m_driver dg3f_m.launch.py

# DG-3F (B type)
ros2 launch dg3f_b_driver dg3f_b.launch.py
```

## Verify Connection

Check that the hardware interface is active:

```bash
ros2 control list_hardware_interfaces
```

## Send Commands

The gripper uses effort (current) command interface:

```bash
ros2 topic pub /effort_controller/commands std_msgs/msg/Float64MultiArray "data: [0.0, 0.0, ...]"
```

## F/T Sensor Offset (DG-3F-M, DG-4F, DG-5F)

```bash
ros2 service call /delto_hardware_interface_node/set_ft_sensor_offset std_srvs/srv/Trigger
```

## GPIO Control

```bash
ros2 service call /delto_hardware_interface_node/set_gpio_output1 std_srvs/srv/SetBool "{data: true}"
```
