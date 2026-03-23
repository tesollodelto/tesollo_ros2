# dg_tcp_comm

[![CI](https://github.com/tesollodelto/dg_tcp_comm/actions/workflows/ci.yml/badge.svg)](https://github.com/tesollodelto/dg_tcp_comm/actions/workflows/ci.yml)

Unified TCP communication library for all Delto grippers.

## Overview

Provides `DeltoTCP::Communication` class that handles the low-level TCP protocol for communicating with Delto gripper firmware. Used internally by `delto_hardware`.

## Key APIs

- `Connect()` / `Disconnect()` — Manage TCP connection
- `GetData()` — Read joint positions, velocities, currents, temperatures, F/T sensor data, and GPIO
- `SendDuty(std::vector<int>)` — Send motor duty commands
- `GetFirmwareVersion()` — Query firmware version
- `SetFTSensorOffset()` — Calibrate F/T sensor zero
- `SetGPIO(bool, bool, bool)` — Set GPIO outputs
