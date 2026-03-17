# delto_hardware ROS 2 Package 🚀

## 📌 Overview

The `delto_hardware` package provides a **unified ROS2 Hardware Interface** for all DELTO gripper models. This package consolidates the hardware interface implementation that was previously duplicated across individual driver packages (dg3f_b, dg3f_m, dg4f, dg5f).

## 🎯 Supported Gripper Models

| Model | Model ID | Description | DOF |
|-------|----------|-------------|-----|
| DG3F-B | 0x3F01 | 3-Finger Basic Gripper | 12 |
| DG3F-M | 0x3F02 | 3-Finger Medium Gripper | 12 |
| DG4F | 0x4F02 | 4-Finger Gripper | 18 |
| DG5F-L | 0x5F12 | 5-Finger Left Hand | 20 |
| DG5F-R | 0x5F22 | 5-Finger Right Hand | 20 |

## 📦 Features

- **Automatic Model Detection**: Identifies gripper model via firmware communication
- **Position/Effort Control**: Supports both position and effort command interfaces
- **Force/Torque Sensors**: Broadcasts fingertip F/T sensor data (DG5F models)
- **GPIO Support**: Motor on/off and grasp/release commands
- **Firmware Compatibility**: Handles motor direction based on firmware version

## 🔧 Hardware Interface

The `delto_hardware/SystemInterface` plugin provides:

### State Interfaces
- `position`: Joint position feedback
- `velocity`: Joint velocity feedback
- `effort`: Joint effort/torque feedback

### Command Interfaces
- `position`: Position command
- `effort`: Effort/torque command

### Sensors 
- Force/Torque sensors for each fingertip (DG3F-M, DG4F, DG5F models)

### GPIO
- Output: 3 channels 
- Input: 1 channel

## 🔌 Services

The hardware interface provides ROS2 services for runtime configuration:

### F/T Sensor Service
| Service | Type | Description |
|---------|------|-------------|
| `~/set_ft_sensor_offset` | `std_srvs/Trigger` | Zero/calibrate F/T sensors (set current reading as offset) |

**Example:**
```bash
ros2 service call /dg5f_right/delto_hardware_interface_node/set_ft_sensor_offset std_srvs/srv/Trigger {}
```

### GPIO Services
| Service | Type | Description |
|---------|------|-------------|
| `~/set_gpio_output1` | `std_srvs/SetBool` | Motor ON (true) / OFF (false) |
| `~/set_gpio_output2` | `std_srvs/SetBool` | Grasp command |
| `~/set_gpio_output3` | `std_srvs/SetBool` | Release command |

**Example:**
```bash
# Motor ON
ros2 service call /dg5f_left/set_gpio_output1 std_srvs/srv/SetBool "{data: true}"

# Grasp
ros2 service call /dg5f_left/set_gpio_output2 std_srvs/srv/SetBool "{data: true}"
```

## 📦 Installation

### Build
```bash
cd ~/your_ws
colcon build --packages-select delto_hardware
```

### Dependencies
- ros2_control
- hardware_interface
- pluginlib
- rclcpp

## 🔌 Usage in URDF/XACRO

```xml
<ros2_control name="DeltoGripperSystem" type="system">
  <hardware>
    <plugin>delto_hardware/SystemInterface</plugin>
    <param name="delto_ip">169.254.186.72</param>
    <param name="delto_port">502</param>
  </hardware>
  <!-- joints... -->
</ros2_control>
```

## 📁 Package Structure

```
delto_hardware/
├── CMakeLists.txt
├── package.xml
├── delto_hardware.xml          # Plugin registration
├── include/
│   └── delto_hardware/
│       └── system_interface.hpp
└── src/
    └── system_interface.cpp
```

## 🌐 Namespaces

All driver packages using this hardware interface support namespaces:

| Driver | Namespace |
|--------|-----------|
| dg3f_b_driver | `/dg3f_b/` |
| dg3f_m_driver | `/dg3f_m/` |
| dg4f_driver | `/dg4f/` |
| dg5f_driver (right) | `/dg5f_right/` |
| dg5f_driver (left) | `/dg5f_left/` |

## 🤝 Contributing
Contributions are encouraged:

1. Fork repository
2. Create branch (`git checkout -b feature/my-feature`)
3. Commit changes (`git commit -am 'Add my feature'`)
4. Push (`git push origin feature/my-feature`)
5. Open pull request

## 📄 License
BSD-3-Clause

## 📧 Contact
[TESOLLO SUPPORT](mailto:support@tesollo.com)
