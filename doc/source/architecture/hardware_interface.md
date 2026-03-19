# Hardware Interface

## Unified Design

`delto_hardware` provides a single `SystemInterface` plugin that handles all gripper models. The model is identified by its Model ID parameter in the URDF.

## Model Capabilities

| Model | ID | Fingers | Joints | F/T Sensor | GPIO | Note |
|-------|----|---------|--------|------------|------|------|
| ~~DG-3F-B~~ | `0x3F01` | 3 | 12 | No | Yes | *Discontinued* |
| DG-3F-M | `0x3F02` | 3 | 12 | Yes | Yes | |
| DG-4F | `0x4F02` | 4 | 18 | Yes | Yes | |
| DG-5F-L | `0x5F12` | 5 | 20 | Yes | Yes | |
| DG-5F-R | `0x5F22` | 5 | 20 | Yes | Yes | |

## Control Flow

```
on_init()     → Parse URDF params, connect via TCP, create services
on_activate() → Verify connection
read()        → Get joint/sensor/GPIO data, spin service callbacks
write()       → Current control → duty conversion → send to gripper
on_deactivate() → Disconnect
```

## Motor Direction & Firmware

Older firmware versions have different motor direction mappings. The hardware interface automatically detects the firmware version and applies the correct mapping:

| Model | Firmware Threshold |
|-------|--------------------|
| DG-3F-B | v3.6+ (revised) |
| DG-3F-M | v2.8+ (revised) |
| DG-4F | Always standard |
| DG-5F | v2.8+ (revised) |
