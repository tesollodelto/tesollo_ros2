# delto_hardware

[![CI](https://github.com/tesollodelto/dg_hardware/actions/workflows/ci.yml/badge.svg)](https://github.com/tesollodelto/dg_hardware/actions/workflows/ci.yml)

Unified `ros2_control` hardware interface for all Delto gripper models.

## Overview

A single `SystemInterface` plugin that supports DG-3F-B, DG-3F-M, DG-4F, and DG-5F through model-aware configuration. The model is selected via URDF hardware parameters.

## Hardware Parameters

| Parameter | Description | Default |
|-----------|-------------|---------|
| `delto_ip` | Gripper IP address | `169.254.186.72` |
| `delto_port` | TCP port | `502` |
| `delto_model` | Model ID (hex as decimal) | `0x5F12` (DG-5F-L) |
| `hand_type` | `left` or `right` (DG-5F only) | `left` |
| `fingertip_sensor` | Enable F/T sensor | `false` |
| `IO` | Enable GPIO | `false` |

## Interfaces

### State Interfaces

- `position` — Joint position (all joints)
- `velocity` — Joint velocity (all joints)
- `effort` — Joint current (all joints)
- `temperature` — Motor temperature (all joints)
- `force.x/y/z`, `torque.x/y/z` — Fingertip F/T sensor (per finger, if enabled)
- `gpio/output_1~3`, `gpio/input_1` — GPIO states (if enabled)
- `system/connection_status` — Connection status (1.0 = connected)

### Command Interfaces

- `effort` — Current command (all joints)

## Services

| Service | Type | Description |
|---------|------|-------------|
| `~/set_ft_sensor_offset` | `std_srvs/Trigger` | Calibrate F/T sensor zero offset |
| `~/set_gpio_output1` | `std_srvs/SetBool` | Set GPIO output 1 |
| `~/set_gpio_output2` | `std_srvs/SetBool` | Set GPIO output 2 |
| `~/set_gpio_output3` | `std_srvs/SetBool` | Set GPIO output 3 |
