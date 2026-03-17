#pragma once

// Standard includes
#include <rclcpp/rclcpp.hpp>
#include <memory>
#include <std_msgs/msg/bool.hpp>
#include <std_msgs/msg/int32.hpp>

// SDK includes
#include "DGSDK.h"
#include "DGDataTypes.h"

// Messages
#include "dg_msgs/msg/received_gripper_data.hpp"
#include "dg_msgs/msg/received_fingertip_sensor_data.hpp"
#include "dg_msgs/msg/received_gpio_data.hpp"
#include "dg_msgs/msg/diagnosis_system.hpp"

// System Setting Services
#include "dg_msgs/srv/set_gripper_system.hpp"
#include "dg_msgs/srv/set_gripper_option.hpp"
#include "dg_msgs/srv/connect_to_gripper.hpp"
#include "dg_msgs/srv/disconnect_to_gripper.hpp"
#include "dg_msgs/srv/system_start.hpp"
#include "dg_msgs/srv/system_stop.hpp"
#include "dg_msgs/srv/set_ip.hpp"

// Other System Services
#include "dg_msgs/srv/set_low_pass_filter_alpha.hpp"
#include "dg_msgs/srv/set_boot_mode.hpp"
#include "dg_msgs/srv/set_gpio_output.hpp"
#include "dg_msgs/srv/set_gpio_output_all.hpp"
#include "dg_msgs/srv/set_torque_limit_mode.hpp"
#include "dg_msgs/srv/set_boot_recipe.hpp"
#include "dg_msgs/srv/eeprom_write.hpp"
#include "dg_msgs/srv/system_diagnosis.hpp"
#include "dg_msgs/srv/backup_recipe_data.hpp"
#include "dg_msgs/srv/restore_recipe_data.hpp"

// Motion Setting Services
#include "dg_msgs/srv/set_grasp_data.hpp"
#include "dg_msgs/srv/set_grasp_force.hpp"
#include "dg_msgs/srv/set_joint_gain_p.hpp"
#include "dg_msgs/srv/set_joint_gain_p_finger.hpp"
#include "dg_msgs/srv/set_joint_gain_p_all.hpp"
#include "dg_msgs/srv/set_joint_gain_d.hpp"
#include "dg_msgs/srv/set_joint_gain_d_finger.hpp"
#include "dg_msgs/srv/set_joint_gain_d_all.hpp"
#include "dg_msgs/srv/set_joint_gain_i.hpp"
#include "dg_msgs/srv/set_joint_gain_i_finger.hpp"
#include "dg_msgs/srv/set_joint_gain_i_all.hpp"
#include "dg_msgs/srv/set_control_pid_mode.hpp"
#include "dg_msgs/srv/set_joint_gain_pid.hpp"
#include "dg_msgs/srv/set_joint_gain_pid_finger.hpp"
#include "dg_msgs/srv/set_joint_gain_pid_all.hpp"
#include "dg_msgs/srv/set_joint_gain_pid_all_equal.hpp"
#include "dg_msgs/srv/set_motion_time_joint.hpp"
#include "dg_msgs/srv/set_motion_time_finger.hpp"
#include "dg_msgs/srv/set_motion_time_all.hpp"
#include "dg_msgs/srv/set_motion_time_all_equal.hpp"
#include "dg_msgs/srv/set_position_mode_joint.hpp"
#include "dg_msgs/srv/set_position_mode_finger.hpp"
#include "dg_msgs/srv/set_position_mode_all.hpp"
#include "dg_msgs/srv/set_current_control_mode.hpp"
#include "dg_msgs/srv/set_target_current_joint.hpp"
#include "dg_msgs/srv/set_target_current_finger.hpp"
#include "dg_msgs/srv/set_target_current_all.hpp"

// Recipe Services
#include "dg_msgs/srv/update_recipe_current_pose_data.hpp"
#include "dg_msgs/srv/update_recipe_target_pose_data.hpp"
#include "dg_msgs/srv/load_recipe_pose_data.hpp"
#include "dg_msgs/srv/update_recipe_gain_data.hpp"
#include "dg_msgs/srv/load_recipe_gain_data.hpp"
#include "dg_msgs/srv/update_recipe_grasp_data.hpp"
#include "dg_msgs/srv/load_recipe_grasp_data.hpp"

// Blend Motion Services
#include "dg_msgs/srv/update_blend_joint.hpp"
#include "dg_msgs/srv/clear_move_blend_joint.hpp"
#include "dg_msgs/srv/add_move_blend_joint.hpp"
#include "dg_msgs/srv/set_move_blend_joint.hpp"
#include "dg_msgs/srv/start_move_blend_joint.hpp"
#include "dg_msgs/srv/stop_move_blend_joint.hpp"

// Motion Services
#include "dg_msgs/srv/manual_teach_mode.hpp"
#include "dg_msgs/srv/start_grasp_motion.hpp"
#include "dg_msgs/srv/move_joint.hpp"
#include "dg_msgs/srv/move_joint_finger.hpp"
#include "dg_msgs/srv/move_joint_all.hpp"
#include "dg_msgs/srv/move_servo_joint.hpp"

// Experimental Services (TCP Control)
#include "dg_msgs/srv/set_tcp_gain_p_finger.hpp"
#include "dg_msgs/srv/set_tcp_gain_p_all.hpp"
#include "dg_msgs/srv/set_tcp_gain_d_finger.hpp"
#include "dg_msgs/srv/set_tcp_gain_d_all.hpp"
#include "dg_msgs/srv/set_tcp_gain_i_finger.hpp"
#include "dg_msgs/srv/set_tcp_gain_i_all.hpp"
#include "dg_msgs/srv/set_tcp_gain_pid_finger.hpp"
#include "dg_msgs/srv/set_tcp_gain_pid_all.hpp"
#include "dg_msgs/srv/set_tcp_motion_time_finger.hpp"
#include "dg_msgs/srv/set_tcp_motion_time_all.hpp"
#include "dg_msgs/srv/set_tcp_motion_time_all_equal.hpp"
#include "dg_msgs/srv/move_tcp_finger.hpp"
#include "dg_msgs/srv/move_tcp_all.hpp"
#include "dg_msgs/srv/get_current_tcp_pose.hpp"
#include "dg_msgs/srv/set_manipulation_gain_pid_all.hpp"
#include "dg_msgs/srv/in_hand_manipulation.hpp"
#include "dg_msgs/srv/set_finger_tip_data_zero.hpp"

// Getter Services
#include "dg_msgs/srv/get_received_gripper_data.hpp"
#include "dg_msgs/srv/get_communication_period.hpp"
#include "dg_msgs/srv/get_received_fingertip_sensor_data.hpp"
#include "dg_msgs/srv/get_received_gpio_data.hpp"
#include "dg_msgs/srv/get_data_processing.hpp"
#include "dg_msgs/srv/get_diagnosis_system.hpp"