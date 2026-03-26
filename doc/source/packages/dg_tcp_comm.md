# dg_tcp_comm

[![CI](https://github.com/tesollodelto/dg_tcp_comm/actions/workflows/ci.yml/badge.svg)](https://github.com/tesollodelto/dg_tcp_comm/actions/workflows/ci.yml)

Unified TCP communication library for all Delto grippers.

## Overview

Provides `DeltoTCP::Communication` class that handles the low-level TCP protocol for communicating with Delto gripper firmware. Used internally by `delto_hardware`.

## Key APIs

| Method | Description |
|--------|-------------|
| `Connect()` / `Disconnect()` | Manage TCP connection |
| `GetData()` | Read joint/velocity/current/temperature, F/T sensor, tactile, and GPIO |
| `SendDuty(std::vector<int>)` | Send motor duty commands |
| `GetFirmwareVersion()` | Query firmware version |
| `SetFTSensorOffset()` | Calibrate F/T sensor zero |
| `SetGPIO(bool, bool, bool)` | Set GPIO outputs |
| `GetSensorType()` | Get sensor type reported by firmware |
| `GetFingerSensorMask()` | Get per-finger sensor availability bitmask |

## Sensor Types

The library auto-detects the sensor type from firmware and parses the corresponding data:

| Type | Description | Data structure |
|------|-------------|----------------|
| `FT_6AXIS` | 6-axis force/torque | `fingertip_sensor` (6 doubles per finger) |
| `FT_3AXIS` | 3-axis force/torque | `fingertip_sensor` (6 doubles per finger, 3 unused) |
| `FT_4AXIS` | 4-axis force/torque | `fingertip_sensor` (6 doubles per finger, 2 unused) |
| `TACTILE_M` | 3x5 tactile matrix | `tactile_m` (15 uint8 per finger) |
| `TACTILE_S` | 3x6 tactile matrix | `tactile_s` (18 uint16 per finger) |
