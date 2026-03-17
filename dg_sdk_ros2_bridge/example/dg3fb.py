#!/usr/bin/env python3

from platform import node
import rclpy
from rclpy.node import Node
import time

# DG custom service imports
from dg_msgs.srv import (
    SetGripperSystem,
    SetGripperOption,
    ConnectToGripper,
    SystemStart
)

from dg_msgs.msg import (
    GripperSystemSetting,
    GripperSetting
)


def main(args=None):
    system_setting = GripperSystemSetting()
    system_setting.comport = "/dev/ttyUSB0"
    system_setting.ip = "169.254.5.72"
    system_setting.port = 502
    system_setting.communication_mode = 0
    system_setting.control_mode = 1
    system_setting.read_timeout = 1000
    system_setting.slave_id = 1
    system_setting.baudrate = 115200

    # service client
    rclpy.init(args=args)
    node = Node("dg_sdk_ros2_bridge_node")
    client = node.create_client(SetGripperSystem, "dg/set_gripper_system")

    while not client.wait_for_service(timeout_sec=1.0):
        node.get_logger().info("Service not available, waiting again...")

    request = SetGripperSystem.Request()
    request.setting = system_setting

    future = client.call_async(request)
    rclpy.spin_until_future_complete(node, future)

    result = future.result()
    print("Message: ", result.result)

    # Connect to gripper
    connect_client = node.create_client(
        ConnectToGripper, "dg/connect_to_gripper")
    while not connect_client.wait_for_service(timeout_sec=1.0):
        node.get_logger().info("Connect service not available, waiting again...")
    connect_request = ConnectToGripper.Request()
    connect_future = connect_client.call_async(connect_request)
    rclpy.spin_until_future_complete(node, connect_future)

    connect_result = connect_future.result()
    print("Connect Message: ", connect_result.result)

    # gripper  setting
    gs = GripperSetting()

    gs.model = 0x3F01 #DG3FB 
    gs.moving_inpose = 0.5
    gs.joint_inpose = [0.0] * 20
    gs.joint_offset = [0.0] * 20
    gs.received_data_type = [1, 2, 0, 0, 0, 0]
    gs.joint_count = 12
    gs.finger_count = 3

    option_client = node.create_client(
        SetGripperOption, "dg/set_gripper_option")
    while not option_client.wait_for_service(timeout_sec=1.0):
        node.get_logger().info("Option service not available, waiting again...")
    option_request = SetGripperOption.Request()
    option_request.setting = gs
    option_future = option_client.call_async(option_request)
    rclpy.spin_until_future_complete(node, option_future)
    option_result = option_future.result()
    print("Option Message: ", option_result.result)

    time.sleep(1.0)

    # Start system
    start_client = node.create_client(SystemStart, "dg/system_start")
    while not start_client.wait_for_service(timeout_sec=1.0):
        node.get_logger().info("Start service not available, waiting again...")
    start_request = SystemStart.Request()

    start_future = start_client.call_async(start_request)
    rclpy.spin_until_future_complete(node, start_future)

    start_result = start_future.result()
    print("Start Message: ", start_result.result)

    start_request = SystemStart.Request()

    start_future = start_client.call_async(start_request)
    rclpy.spin_until_future_complete(node, start_future)

    start_result = start_future.result()
    print("Start Message: ", start_result.result)


if __name__ == "__main__":
    main()
