# Hardware Interface

This page explains how the `delto_hardware` package works under the hood as a ros2_control hardware interface. If you just want to use the gripper, the {doc}`/getting_started/quickstart` and the gripper-specific pages are sufficient. This page is for those who want to understand the internals.

## Unified Design

`delto_hardware` provides a single `SystemInterface` plugin that handles **all** gripper models. Rather than having a separate hardware interface for each model, one codebase supports everything through model-aware configuration.

```{note}
**What is a SystemInterface?** In ros2_control terminology, a `SystemInterface` is a hardware plugin that can handle multiple joints (and optionally sensors and GPIOs) as a single unit. This is perfect for the Delto gripper, where all joints share a single TCP connection and are read/written in one communication cycle.
```

## Model Capabilities

| Model | ID | Fingers | Joints | F/T Sensor | GPIO | Motor Direction |
|-------|----|---------|--------|------------|------|-----------------|
| DG-1F-M | `0x1F02` | 1 | 3 | No | Yes | Firmware-dependent (legacy below v2.8) |
| DG-2F-M | `0x2F02` | 2 | 6 | No | Yes | Firmware-dependent (legacy below v2.8) |
| ~~DG-3F-B~~ | `0x3F01` | 3 | 12 | No | Yes | Firmware-dependent (legacy below v3.6) |
| DG-3F-M | `0x3F02` | 3 | 12 | Yes | Yes | Firmware-dependent (legacy below v2.8) |
| DG-4F | `0x4F02` | 4 | 18 | Yes | Yes | Always standard |
| DG-5F-L | `0x5F12` | 5 | 20 | Yes | Yes | Firmware-dependent (legacy below v2.8) |
| DG-5F-R | `0x5F22` | 5 | 20 | Yes | Yes | Firmware-dependent (legacy below v2.8) |
| DG-5F-S-L | `0x5F14` | 5 | 20 | Yes | No | Always standard |
| DG-5F-S-R | `0x5F24` | 5 | 20 | Yes | No | Always standard |
| DG-5F-S15-L | `0x5F34` | 5 | 15 | Yes | No | Always standard |
| DG-5F-S15-R | `0x5F44` | 5 | 15 | Yes | No | Always standard |

```{note}
The DG-5F-S variants have no GPIO hardware, so `IO:=true` is ignored on those
models (the driver logs a warning). The same applies to DG-5F-M (Short Wrist),
which shares Product ID `0x5F12` / `0x5F22` with the standard DG-5F-M and
therefore cannot be told apart from the ID alone.
```

## Lifecycle

The hardware interface follows the standard ros2_control lifecycle:

### on_init() -- Initialization

When the controller manager starts, it calls `on_init()`:

1. **Parse URDF parameters** -- Read `delto_ip`, `delto_port`, `delto_model`, `hand_type`, `fingertip_sensor`, and `IO` from the URDF/XACRO hardware description.
2. **Establish TCP connection** -- Create a `DeltoTCP::Communication` instance and connect to the gripper at the configured IP and port.
3. **Query firmware** -- Read the firmware version to determine the correct motor direction mapping.
4. **Detect sensors** -- Query the sensor type (F/T 6-axis, 3-axis, 4-axis, tactile, etc.) and configure data parsing accordingly.
5. **Create ROS 2 services** -- Set up the F/T calibration and GPIO services.

### on_activate() -- Activation

Verify the connection is active and the gripper is responding.

### read() -- Called Every Control Cycle

Each cycle (typically at 100-300 Hz):

1. Call `DeltoTCP::Communication::GetData()` to receive current joint positions, velocities, currents, temperatures, sensor data, and GPIO states from the gripper.
2. Update all state interfaces with the received data.
3. Process any pending service callbacks (F/T calibration, GPIO commands).

If the connection is lost:
- Return OK with the last known data (to prevent ros2_control from deactivating the hardware).
- The background reconnection thread attempts to restore the connection.

### write() -- Called Every Control Cycle

Each cycle:

1. Read the effort command from the controller (set via the `effort` command interface).
2. Apply current-to-duty conversion (motor-specific scaling and direction mapping).
3. Call `DeltoTCP::Communication::SendDuty()` to send the duty values to the gripper.

If the connection is lost, the write is silently skipped (same as read).

### on_deactivate() -- Shutdown

Disconnect from the gripper and clean up resources.

## Motor Direction and Firmware Compatibility

Older firmware versions on some models use different motor direction mappings. The hardware interface handles this automatically:

| Model | Firmware Threshold | Behavior |
|-------|--------------------|----------|
| DG-1F-M | v2.8 | Versions below 2.8 use legacy direction mapping |
| DG-2F-M | v2.8 | Versions below 2.8 use legacy direction mapping |
| DG-3F-B | v3.6 | Versions below 3.6 use legacy direction mapping |
| DG-3F-M | v2.8 | Versions below 2.8 use legacy direction mapping |
| DG-5F | v2.8 | Versions below 2.8 use legacy direction mapping |
| DG-4F | N/A | Always uses standard direction |
| DG-5F-S | N/A | Always uses standard direction |
| DG-5F-S15 | N/A | Always uses standard direction |

The firmware version is queried during `on_init()` and the correct mapping is applied automatically. You do not need to configure this manually.

## Connection Resilience

The hardware interface is designed to handle temporary connection losses gracefully:

1. **Detection:** TCP keepalive probes detect cable disconnection within approximately 4 seconds (1s idle + 3 probes at 1s intervals).
2. **Reconnection:** A background thread attempts to reconnect at 1-second intervals, up to 10 retries.
3. **Graceful degradation:** During disconnection, `read()` and `write()` return OK with stale data. This prevents the controller manager from deactivating the hardware, which would require manual reactivation.
4. **Monitoring:** The `system/connection_status` state interface reports `1.0` when connected and `0.0` when disconnected, allowing your application to detect and respond to connection issues.

## Data Flow Diagram

```
Controller Manager (100-300 Hz loop)
    |
    ├── read()
    │     └── DeltoTCP::Communication::GetData()
    │           └── TCP recv ← Gripper Firmware
    │                 └── Parse: joints, velocity, current, temp, F/T, GPIO
    │                       └── Update state interfaces
    │
    └── write()
          └── Read effort command interfaces
                └── CurrentControl() → ConvertDuty()
                      └── DeltoTCP::Communication::SendDuty()
                            └── TCP send → Gripper Firmware
```
