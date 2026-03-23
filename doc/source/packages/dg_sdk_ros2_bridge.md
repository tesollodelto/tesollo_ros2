# dg_sdk_ros2_bridge

[![CI](https://github.com/tesollodelto/dg_sdk_ros2_bridge/actions/workflows/ci.yml/badge.svg)](https://github.com/tesollodelto/dg_sdk_ros2_bridge/actions/workflows/ci.yml)

ROS 2 bridge for the Delto Gripper SDK (DG SDK).

## Overview

Provides a ROS 2 node that wraps the proprietary DG SDK library, exposing all gripper functions as ROS 2 topics and services. This is an alternative to using `delto_hardware` with `ros2_control`.

## When to Use

| Use Case | Recommended |
|----------|-------------|
| `ros2_control` integration | `delto_hardware` |
| Direct SDK access via ROS 2 | `dg_sdk_ros2_bridge` |
| MoveIt / trajectory control | `delto_hardware` |
| Recipe management, advanced settings | `dg_sdk_ros2_bridge` |

## Published Topics

| Topic | Type | Description |
|-------|------|-------------|
| `/dg/gripper_data` | `dg_msgs/ReceivedGripperData` | Joint positions, velocities, currents |
| `/dg/fingertip_sensor_data` | `dg_msgs/ReceivedFingertipSensorData` | F/T sensor readings |
| `/dg/gpio_data` | `dg_msgs/ReceivedGPIOData` | GPIO input states |
| `/dg/diagnosis_system` | `dg_msgs/DiagnosisSystem` | System diagnostics |

## Services

The bridge exposes 86+ services via `dg_msgs` for full SDK functionality including:

- System settings (connect, start/stop, IP config)
- Motion control (joint/TCP move, servo, grasp)
- Gain tuning (PID per joint/finger)
- Recipe management
- Blend motion
- GPIO control

See {doc}`/reference/services` for the full list.
