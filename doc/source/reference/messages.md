# Message Definitions

All custom messages are defined in the `dg_msgs` package (part of `dg_sdk_ros2_bridge` repository).

## ReceivedGripperData

Gripper feedback data published on `/dg/gripper_data`.

```
float64[] joint
float64[] velocity
float64[] current
float64[] temperature
```

## ReceivedFingertipSensorData

F/T sensor readings published on `/dg/fingertip_sensor_data`.

```
float64[] data    # 6 values per finger (fx, fy, fz, tx, ty, tz)
```

## ReceivedGPIOData

GPIO states published on `/dg/gpio_data`.

```
bool[] data    # [output1, output2, output3, input1]
```

## DiagnosisSystem

System diagnostic information.

```{note}
See `dg_msgs/msg/DiagnosisSystem.msg` for field definitions.
```

## Recipe Messages

- `RecipePoseData` — Joint pose recipe data
- `RecipeGainData` — PID gain recipe data
- `RecipeGraspData` — Grasp recipe data
- `RecipeBlendData` — Blend motion recipe data

```{note}
See the corresponding `dg_msgs/msg/Recipe*.msg` files for field definitions.
```

## Configuration Messages

- `GripperSystemSetting` — System-level configuration
- `GripperSetting` — Gripper-specific settings

```{note}
See the corresponding `dg_msgs/msg/GripperSystemSetting.msg` and `dg_msgs/msg/GripperSetting.msg` files for field definitions.
```
