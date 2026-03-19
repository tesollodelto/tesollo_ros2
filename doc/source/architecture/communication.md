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

## DeltoReceivedData

| Field | Type | Description |
|-------|------|-------------|
| `joint` | `vector<double>` | Joint positions |
| `velocity` | `vector<double>` | Joint velocities |
| `current` | `vector<double>` | Motor currents (mA) |
| `temperature` | `vector<double>` | Motor temperatures |
| `fingertip_sensor` | `vector<double>` | F/T sensor data (6 values per finger) |
| `gpio` | `vector<bool>` | GPIO states (3 outputs + 1 input) |
