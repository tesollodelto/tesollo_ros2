# Delto TCP Communication Library

A unified TCP communication library for DELTO grippers (DG-3F, DG-4F, DG-5F).

## Supported Models

| Model ID | Model Name | Motors | Bytes/Motor | GPIO | F/T Sensor |
|----------|------------|--------|-------------|------|------------|
| 0x3F01 | DG-3F B  | 12 | 5 | ✅ | ❌ |
| 0x3F02 | DG-3F M | 12 | 8 | ✅ | ✅ |
| 0x4F02 | DG-4F | 18 | 8 | ✅ | ✅ |
| 0x5F02 | DG-5F | 20 | 8 | ✅ | ✅ |
| 0x5F12 | DG-5F Left | 20 | 8 | ✅ | ✅ |
| 0x5F22 | DG-5F Right | 20 | 8 | ✅ | ✅ |

## Usage

### Basic Example

```cpp
#include "delto_tcp_comm/delto_developer_TCP.hpp"

// Connect to DG-5F (with GPIO and F/T sensor enabled)
DeltoTCP::Communication comm("192.168.1.100", 10000, 0x5F02, true, true);
comm.Connect();

// Read data
auto data = comm.GetData();
for (size_t i = 0; i < data.joint.size(); i++) {
    std::cout << "Joint " << i << ": " << data.joint[i] << " rad" << std::endl;
}

// Send duty command
std::vector<int> duty(20, 0);
comm.SendDuty(duty);

// GPIO control
comm.SetGPIO(true, false, false);

// Set F/T sensor offset (zero calibration)
comm.SetFTSensorOffset();

comm.Disconnect();
```

### Adding as Dependency

`package.xml`:
```xml
<depend>delto_tcp_comm</depend>
```

`CMakeLists.txt`:
```cmake
find_package(delto_tcp_comm REQUIRED)

target_link_libraries(your_target
  delto_tcp_comm::delto_tcp_comm
)
```

## Protocol

### Motor Data (8 bytes/motor - New Models)
| Offset | Size | Description |
|--------|------|-------------|
| 0 | 1 | Motor ID |
| 1-2 | 2 | Position (0.1°) |
| 3-4 | 2 | Current (mA) |
| 5-6 | 2 | Temperature (0.1°C) |
| 7 | 1 | Velocity (1 RPM) |

### Motor Data (5 bytes/motor - Legacy DG3F-B)
| Offset | Size | Description |
|--------|------|-------------|
| 0 | 1 | Motor ID |
| 1-2 | 2 | Position (0.1°) |
| 3-4 | 2 | Current (mA) |

### F/T Sensor Data (60 bytes)
- 5 fingers × 6 axes × 2 bytes
- Force (Fx, Fy, Fz): 0.1 N units
- Torque (Tx, Ty, Tz): 1 mNm units

### GPIO Data (4 bytes)
- Output 1-3, Input 1
- Level: 0 = 0V, 1 = 24V

## Build

```bash
cd ~/your_ws
colcon build --packages-select delto_tcp_comm
```
