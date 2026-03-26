# Communication Protocol

## Overview

All Delto grippers communicate over TCP/IP using a custom binary protocol implemented in `dg_tcp_comm`.

## Connection

- Default IP: `169.254.186.72`
- Default Port: `502`
- Protocol: TCP

## Data Flow

```
delto_hardware::read()
  → DeltoTCP::Communication::GetData()
    → TCP receive → parse → DeltoReceivedData struct

delto_hardware::write()
  → CurrentControl() → ConvertDuty()
    → DeltoTCP::Communication::SendDuty()
      → TCP send
```

## SensorType

The sensor type is reported by firmware via `GET_VERSION` and determines which sensor fields are populated in `DeltoReceivedData`:

| Enum | Value | Description | Data per finger |
|------|-------|-------------|-----------------|
| `NONE` | `0x00` | No sensor | -- |
| `FT_6AXIS` | `0x01` | 6-axis F/T sensor | 12 bytes (6 x int16) |
| `FT_3AXIS` | `0x02` | 3-axis F/T sensor | 12 bytes (6 x int16, 3 unused) |
| `TACTILE_M` | `0x03` | Tactile matrix (3x5) | 15 bytes (uint8 per cell) |
| `FT_4AXIS` | `0x04` | 4-axis F/T sensor | 12 bytes (6 x int16, 2 unused) |
| `TACTILE_S` | `0x05` | Tactile matrix (3x6) | 36 bytes (uint16 per cell) |

## DeltoReceivedData

| Field | Type | Description |
|-------|------|-------------|
| `joint` | `vector<double>` | Joint positions (rad) |
| `velocity` | `vector<double>` | Joint velocities (rad/s) |
| `current` | `vector<double>` | Motor currents (A) |
| `temperature` | `vector<double>` | Motor temperatures (C) |
| `fingertip_sensor` | `vector<double>` | F/T sensor data (N fingers x 6 axes) |
| `gpio` | `vector<bool>` | GPIO states (3 outputs + 1 input) |
| `tactile_m` | `vector<vector<uint8_t>>` | Tactile M data: per finger, 3x5=15 cells (uint8) |
| `tactile_s` | `vector<vector<uint16_t>>` | Tactile S data: per finger, 3x6=18 cells (uint16) |

Depending on `SensorType`, only the relevant sensor field is populated:
- `FT_6AXIS` / `FT_3AXIS` / `FT_4AXIS` -> `fingertip_sensor`
- `TACTILE_M` -> `tactile_m`
- `TACTILE_S` -> `tactile_s`
