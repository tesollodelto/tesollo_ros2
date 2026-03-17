// Copyright 2025 TESOLLO
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
//    * Redistributions of source code must retain the above copyright
//      notice, this list of conditions and the following disclaimer.
//
//    * Redistributions in binary form must reproduce the above copyright
//      notice, this list of conditions and the following disclaimer in the
//      documentation and/or other materials provided with the distribution.
//
//    * Neither the name of the TESOLLO nor the names of its
//      contributors may be used to endorse or promote products derived from
//      this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.

#include "delto_hardware/system_interface.hpp"

#include <algorithm>
#include <chrono>
#include <cmath>
#include <limits>
#include <memory>
#include <string>
#include <vector>

#include "hardware_interface/types/hardware_interface_type_values.hpp"
#include "rclcpp/rclcpp.hpp"

namespace delto_hardware {

hardware_interface::SystemInterface::CallbackReturn SystemInterface::on_init(
    const hardware_interface::HardwareInfo& info) {
  if (hardware_interface::SystemInterface::CallbackReturn::SUCCESS !=
      hardware_interface::SystemInterface::on_init(info)) {
    return CallbackReturn::ERROR;
  }

  // Initialize arrays based on joint count
  positions_.resize(info_.joints.size(), 0.0);
  velocities_.resize(info_.joints.size(), 0.0);
  efforts_.resize(info_.joints.size(), 0.0);
  temperature_.resize(info_.joints.size(), 0.0);
  effort_commands_.resize(info_.joints.size(), 0.0);
  current_.resize(info_.joints.size(), 0.0);
  firmware_version_.resize(2, 0);
  current_limit_flag_.resize(info_.joints.size(), 0);
  current_integral_.resize(info_.joints.size(), 0.0);
  motor_dir_.resize(info_.joints.size(), 1);

  // Initialize connection status
  connection_status_ = 0.0;
  is_connected_.store(false);

  // Validate joint interfaces
  for (const hardware_interface::ComponentInfo& joint : info_.joints) {
    if (joint.command_interfaces.size() != 1) {
      RCLCPP_ERROR(rclcpp::get_logger("SystemInterface"),
                   "Joint '%s' needs a command interface.", joint.name.c_str());
      return CallbackReturn::ERROR;
    }

    if (!(joint.state_interfaces[0].name == hardware_interface::HW_IF_POSITION ||
          joint.state_interfaces[1].name == hardware_interface::HW_IF_VELOCITY)) {
      RCLCPP_ERROR(rclcpp::get_logger("SystemInterface"),
                   "Joint '%s' needs position and velocity state interfaces.",
                   joint.name.c_str());
      return CallbackReturn::ERROR;
    }
  }

  // Set default values
  delto_ip_ = "169.254.186.72";
  delto_port_ = 502;
  model_ = MODEL_DG5F_L;  // Default model
  hand_type_ = "left";
  fingertip_sensor_enabled_ = false;
  io_enabled_ = false;

  // Get parameters from hardware info
  if (info.hardware_parameters.find("delto_ip") != info.hardware_parameters.end()) {
    delto_ip_ = info.hardware_parameters.at("delto_ip");
  }

  if (info.hardware_parameters.find("delto_port") != info.hardware_parameters.end()) {
    try {
      delto_port_ = std::stoi(info.hardware_parameters.at("delto_port"));
    } catch (...) {
      RCLCPP_WARN(rclcpp::get_logger("SystemInterface"),
                  "Invalid port parameter, using default: %d", delto_port_);
    }
  }

  if (info.hardware_parameters.find("delto_model") != info.hardware_parameters.end()) {
    try {
      model_ = static_cast<uint16_t>(std::stoi(info.hardware_parameters.at("delto_model")));
    } catch (...) {
      RCLCPP_WARN(rclcpp::get_logger("SystemInterface"),
                  "Invalid model parameter, using default: 0x%X", model_);
    }
  }

  if (info.hardware_parameters.find("hand_type") != info.hardware_parameters.end()) {
    hand_type_ = info.hardware_parameters.at("hand_type");
  }

  if (info.hardware_parameters.find("fingertip_sensor") != info.hardware_parameters.end()) {
    fingertip_sensor_enabled_ = info.hardware_parameters.at("fingertip_sensor") == "true";
  }

  if (info.hardware_parameters.find("IO") != info.hardware_parameters.end()) {
    io_enabled_ = info.hardware_parameters.at("IO") == "true";
  }

  // Initialize model-specific settings
  initModelSpecificSettings();

  RCLCPP_INFO(rclcpp::get_logger("SystemInterface"), 
              "Delto model: 0x%X, Fingers: %zu, Joints: %zu", 
              model_, num_fingers_, num_joints_);
  RCLCPP_INFO(rclcpp::get_logger("SystemInterface"),
              "Supports F/T: %s, Supports GPIO: %s",
              supports_ft_sensor_ ? "yes" : "no",
              supports_gpio_ ? "yes" : "no");

  // Initialize F/T sensor arrays if supported
  if (supports_ft_sensor_) {
    fingertip_force_x_.resize(num_fingers_, 0.0);
    fingertip_force_y_.resize(num_fingers_, 0.0);
    fingertip_force_z_.resize(num_fingers_, 0.0);
    fingertip_torque_x_.resize(num_fingers_, 0.0);
    fingertip_torque_y_.resize(num_fingers_, 0.0);
    fingertip_torque_z_.resize(num_fingers_, 0.0);
  }

  // Initialize GPIO arrays if supported
  if (supports_gpio_) {
    gpio_states_.resize(4, 0.0);  // Output 1-3, Input 1
    gpio_output1_state_ = false;
    gpio_output2_state_ = false;
    gpio_output3_state_ = false;
  }

  // Create communication client
  delto_client_ = std::make_unique<DeltoTCP::Communication>(
      delto_ip_, delto_port_, model_, 
      fingertip_sensor_enabled_ && supports_ft_sensor_, 
      io_enabled_ && supports_gpio_);

  // Try to connect
  try {
    RCLCPP_INFO(rclcpp::get_logger("SystemInterface"), 
                "Attempting to connect to %s:%d", delto_ip_.c_str(), delto_port_);
    delto_client_->Connect();
    firmware_version_ = delto_client_->GetFirmwareVersion();
    
    if (firmware_version_.size() < 2) {
      RCLCPP_WARN(rclcpp::get_logger("SystemInterface"),
                  "Invalid firmware version returned, using default [0, 0]");
      firmware_version_.resize(2, 0);
    }
    
    is_connected_.store(true);
    connection_status_ = 1.0;
    
    // Check firmware compatibility for motor direction
    checkFirmwareCompatibility();
    
    RCLCPP_INFO(rclcpp::get_logger("SystemInterface"),
                "Successfully connected. Firmware: v%d.%d, Motor direction revised: %s",
                firmware_version_[0], firmware_version_[1],
                firmware_dir_revised_ ? "yes" : "no");
                
  } catch (...) {
    RCLCPP_ERROR(rclcpp::get_logger("SystemInterface"), "Connect Failed.");
    is_connected_.store(false);
    connection_status_ = 0.0;
    return CallbackReturn::FAILURE;
  }

  // Create ROS2 node for services
  node_ = rclcpp::Node::make_shared("delto_hardware_interface_node");

  // Create F/T sensor offset service if supported and enabled
  if (supports_ft_sensor_ && fingertip_sensor_enabled_) {
    ft_offset_service_ = node_->create_service<std_srvs::srv::Trigger>(
        "~/set_ft_sensor_offset",
        std::bind(&SystemInterface::ftOffsetCallback, this,
                  std::placeholders::_1, std::placeholders::_2));
    RCLCPP_INFO(rclcpp::get_logger("SystemInterface"),
                "F/T sensor offset service created: ~/set_ft_sensor_offset");
  }

  // Create GPIO services if supported and enabled
  if (supports_gpio_ && io_enabled_) {
    gpio_output1_service_ = node_->create_service<std_srvs::srv::SetBool>(
        "~/set_gpio_output1",
        std::bind(&SystemInterface::gpioOutput1Callback, this,
                  std::placeholders::_1, std::placeholders::_2));
    gpio_output2_service_ = node_->create_service<std_srvs::srv::SetBool>(
        "~/set_gpio_output2",
        std::bind(&SystemInterface::gpioOutput2Callback, this,
                  std::placeholders::_1, std::placeholders::_2));
    gpio_output3_service_ = node_->create_service<std_srvs::srv::SetBool>(
        "~/set_gpio_output3",
        std::bind(&SystemInterface::gpioOutput3Callback, this,
                  std::placeholders::_1, std::placeholders::_2));
    RCLCPP_INFO(rclcpp::get_logger("SystemInterface"),
                "GPIO services created: ~/set_gpio_output1, ~/set_gpio_output2, ~/set_gpio_output3");
  }

  return CallbackReturn::SUCCESS;
}

void SystemInterface::initModelSpecificSettings() {
  switch (model_) {
    case MODEL_DG3F_B:
      num_fingers_ = 3;
      num_joints_ = 12;
      supports_ft_sensor_ = false;
      supports_gpio_ = true;
      min_firmware_major_ = 3;
      min_firmware_minor_ = 6;
      // DG3F-B: motor direction is -1 before firmware v3.6
      for (size_t i = 0; i < motor_dir_.size(); ++i) {
        motor_dir_[i] = -1;
      }
      break;

    case MODEL_DG3F_M:
      num_fingers_ = 3;
      num_joints_ = 12;
      supports_ft_sensor_ = true;
      supports_gpio_ = true;
      min_firmware_major_ = 2;
      min_firmware_minor_ = 8;
      // DG3F-M: motor direction is -1 before firmware v2.8
      for (size_t i = 0; i < motor_dir_.size(); ++i) {
        motor_dir_[i] = -1;
      }
      break;

    case MODEL_DG4F:
      num_fingers_ = 4;
      num_joints_ = 18;
      supports_ft_sensor_ = true;
      supports_gpio_ = true;
      min_firmware_major_ = 0;  // Always revised
      min_firmware_minor_ = 0;
      // DG4F: motor direction is always 1 (no revision needed)
      for (size_t i = 0; i < motor_dir_.size(); ++i) {
        motor_dir_[i] = 1;
      }
      firmware_dir_revised_ = true;  // DG4F doesn't need revision
      break;

    case MODEL_DG5F_L:
      num_fingers_ = 5;
      num_joints_ = 20;
      supports_ft_sensor_ = true;
      supports_gpio_ = true;
      min_firmware_major_ = 2;
      min_firmware_minor_ = 8;
      hand_type_ = "left";
      // DG5F-L: use left motor direction array
      for (size_t i = 0; i < motor_dir_.size() && i < 20; ++i) {
        motor_dir_[i] = DG5F_LEFT_MOTOR_DIR[i];
      }
      break;

    case MODEL_DG5F_R:
      num_fingers_ = 5;
      num_joints_ = 20;
      supports_ft_sensor_ = true;
      supports_gpio_ = true;
      min_firmware_major_ = 2;
      min_firmware_minor_ = 8;
      hand_type_ = "right";
      // DG5F-R: use right motor direction array
      for (size_t i = 0; i < motor_dir_.size() && i < 20; ++i) {
        motor_dir_[i] = DG5F_RIGHT_MOTOR_DIR[i];
      }
      break;

    default:
      RCLCPP_WARN(rclcpp::get_logger("SystemInterface"),
                  "Unknown model 0x%X, using DG5F-L defaults", model_);
      num_fingers_ = 5;
      num_joints_ = 20;
      supports_ft_sensor_ = true;
      supports_gpio_ = true;
      min_firmware_major_ = 2;
      min_firmware_minor_ = 8;
      break;
  }
}

bool SystemInterface::checkFirmwareCompatibility() {
  // DG4F doesn't need firmware check for motor direction
  if (model_ == MODEL_DG4F) {
    firmware_dir_revised_ = true;
    return true;
  }

  // Check if firmware version meets minimum requirement for motor direction revision
  if (firmware_version_[0] > min_firmware_major_ ||
      (firmware_version_[0] == min_firmware_major_ && 
       firmware_version_[1] >= min_firmware_minor_)) {
    firmware_dir_revised_ = true;
  } else {
    firmware_dir_revised_ = false;
    RCLCPP_WARN(rclcpp::get_logger("SystemInterface"),
                "Firmware v%d.%d is older than v%d.%d. Using legacy motor direction.",
                firmware_version_[0], firmware_version_[1],
                min_firmware_major_, min_firmware_minor_);
  }
  return firmware_dir_revised_;
}

int SystemInterface::getMotorDirection(size_t joint_index) const {
  if (joint_index >= motor_dir_.size()) {
    return 1;
  }
  
  // If firmware is revised, motor direction is always 1
  if (firmware_dir_revised_) {
    return 1;
  }
  
  // Otherwise use the model-specific motor direction
  return motor_dir_[joint_index];
}

std::string SystemInterface::getFingerName(size_t finger_index) const {
  switch (num_fingers_) {
    case 5:
      if (finger_index < FINGER_NAMES_5F.size()) {
        return hand_type_+"_"+"fingertip_" + FINGER_NAMES_5F[finger_index];
      }
      break;
    case 4:
      if (finger_index < FINGER_NAMES_4F.size()) {
        return "fingertip_" + FINGER_NAMES_4F[finger_index];
      }
      break;
    case 3:
      if (finger_index < FINGER_NAMES_3F.size()) {
        return "fingertip_" + FINGER_NAMES_3F[finger_index];
      }
      break;
  }
  return "fingertip_unknown";
}

hardware_interface::SystemInterface::CallbackReturn
SystemInterface::on_deactivate(
    [[maybe_unused]] const rclcpp_lifecycle::State& previous_state) {
  RCLCPP_INFO(rclcpp::get_logger("SystemInterface"), "Deactivating driver...");
  
  if (delto_client_) {
    delto_client_->Disconnect();
  }
  is_connected_.store(false);
  connection_status_ = 0.0;

  RCLCPP_INFO(rclcpp::get_logger("SystemInterface"), "Driver deactivated");
  return CallbackReturn::SUCCESS;
}

std::vector<hardware_interface::StateInterface>
SystemInterface::export_state_interfaces() {
  std::vector<hardware_interface::StateInterface> state_interfaces;
  
  // Reserve space for all interfaces
  size_t reserve_size = info_.joints.size() * 4;  // position, velocity, effort, temperature
  if (supports_ft_sensor_ && fingertip_sensor_enabled_) {
    reserve_size += num_fingers_ * 6;  // force xyz, torque xyz per finger
  }
  if (supports_gpio_ && io_enabled_) {
    reserve_size += 4;  // 3 outputs + 1 input
  }
  reserve_size += 1;  // connection_status
  state_interfaces.reserve(reserve_size);

  // Joint state interfaces
  for (size_t i = 0; i < info_.joints.size(); i++) {
    state_interfaces.emplace_back(hardware_interface::StateInterface(
        info_.joints[i].name, hardware_interface::HW_IF_POSITION, &positions_[i]));
    state_interfaces.emplace_back(hardware_interface::StateInterface(
        info_.joints[i].name, hardware_interface::HW_IF_VELOCITY, &velocities_[i]));
    state_interfaces.emplace_back(hardware_interface::StateInterface(
        info_.joints[i].name, hardware_interface::HW_IF_EFFORT, &efforts_[i]));
    state_interfaces.emplace_back(hardware_interface::StateInterface(
        info_.joints[i].name, delto_interface::HW_IF_TEMPERATURE, &temperature_[i]));

    RCLCPP_DEBUG(rclcpp::get_logger("SystemInterface"),
                 "export_state_interfaces: %s", info_.joints[i].name.c_str());
  }

  // Fingertip F/T sensor interfaces
  if (supports_ft_sensor_ && fingertip_sensor_enabled_) {
    for (size_t finger = 0; finger < num_fingers_; finger++) {
      std::string sensor_name = getFingerName(finger);
      
      state_interfaces.emplace_back(hardware_interface::StateInterface(
          sensor_name, "force.x", &fingertip_force_x_[finger]));
      state_interfaces.emplace_back(hardware_interface::StateInterface(
          sensor_name, "force.y", &fingertip_force_y_[finger]));
      state_interfaces.emplace_back(hardware_interface::StateInterface(
          sensor_name, "force.z", &fingertip_force_z_[finger]));
      state_interfaces.emplace_back(hardware_interface::StateInterface(
          sensor_name, "torque.x", &fingertip_torque_x_[finger]));
      state_interfaces.emplace_back(hardware_interface::StateInterface(
          sensor_name, "torque.y", &fingertip_torque_y_[finger]));
      state_interfaces.emplace_back(hardware_interface::StateInterface(
          sensor_name, "torque.z", &fingertip_torque_z_[finger]));

      RCLCPP_DEBUG(rclcpp::get_logger("SystemInterface"),
                   "export_state_interfaces: %s (F/T sensor)", sensor_name.c_str());
    }
  }

  // GPIO interfaces
  if (supports_gpio_ && io_enabled_) {
    state_interfaces.emplace_back(hardware_interface::StateInterface(
        "gpio", "output_1", &gpio_states_[0]));
    state_interfaces.emplace_back(hardware_interface::StateInterface(
        "gpio", "output_2", &gpio_states_[1]));
    state_interfaces.emplace_back(hardware_interface::StateInterface(
        "gpio", "output_3", &gpio_states_[2]));
    state_interfaces.emplace_back(hardware_interface::StateInterface(
        "gpio", "input_1", &gpio_states_[3]));
    
    RCLCPP_DEBUG(rclcpp::get_logger("SystemInterface"),
                 "export_state_interfaces: gpio (4 channels)");
  }

  // Connection status
  state_interfaces.emplace_back(hardware_interface::StateInterface(
      "system", "connection_status", &connection_status_));

  return state_interfaces;
}

std::vector<hardware_interface::CommandInterface>
SystemInterface::export_command_interfaces() {
  std::vector<hardware_interface::CommandInterface> command_interfaces;
  command_interfaces.reserve(info_.joints.size());

  for (size_t i = 0; i < info_.joints.size(); i++) {
    command_interfaces.emplace_back(hardware_interface::CommandInterface(
        info_.joints[i].name, hardware_interface::HW_IF_EFFORT, &effort_commands_[i]));
  }

  return command_interfaces;
}

SystemInterface::return_type SystemInterface::prepare_command_mode_switch(
    [[maybe_unused]] const std::vector<std::string>& start_interfaces,
    [[maybe_unused]] const std::vector<std::string>& stop_interfaces) {
  return return_type::OK;
}

SystemInterface::CallbackReturn SystemInterface::on_activate(
    [[maybe_unused]] const rclcpp_lifecycle::State& previous_state) {
  RCLCPP_INFO(rclcpp::get_logger("SystemInterface"), "Activating driver...");

  if (is_connected_.load()) {
    RCLCPP_INFO(rclcpp::get_logger("SystemInterface"), "Driver activated successfully!");
    return CallbackReturn::SUCCESS;
  } else {
    RCLCPP_ERROR(rclcpp::get_logger("SystemInterface"), 
                 "Device not connected, cannot activate");
    return CallbackReturn::ERROR;
  }
}

hardware_interface::SystemInterface::CallbackReturn
SystemInterface::on_shutdown(
    [[maybe_unused]] const rclcpp_lifecycle::State& previous_state) {
  RCLCPP_INFO(rclcpp::get_logger("SystemInterface"), "Shutting down driver...");
  
  if (delto_client_) {
    delto_client_->Disconnect();
  }
  is_connected_.store(false);
  connection_status_ = 0.0;
  
  RCLCPP_INFO(rclcpp::get_logger("SystemInterface"), "Driver stopped");
  return CallbackReturn::SUCCESS;
}

SystemInterface::return_type SystemInterface::read(
    [[maybe_unused]] const rclcpp::Time& time,
    [[maybe_unused]] const rclcpp::Duration& period) {
  try {
    if (!delto_client_) {
      RCLCPP_ERROR(rclcpp::get_logger("SystemInterface"), "Client is not initialized");
      is_connected_.store(false);
      connection_status_ = 0.0;
      return return_type::ERROR;
    }

    if (!is_connected_.load()) {
      return return_type::ERROR;
    }

    DeltoTCP::DeltoReceivedData received_data;
    try {
      received_data = delto_client_->GetData();
    } catch (const std::exception& e) {
      RCLCPP_ERROR(rclcpp::get_logger("SystemInterface"), 
                   "Failed to read data: %s", e.what());
      is_connected_.store(false);
      connection_status_ = 0.0;
      return return_type::ERROR;
    }

    // Validate and copy joint data
    if (received_data.joint.size() >= positions_.size()) {
      positions_ = received_data.joint;
    }
    if (received_data.velocity.size() >= velocities_.size()) {
      velocities_ = received_data.velocity;
    }
    if (received_data.current.size() >= efforts_.size()) {
      current_ = received_data.current;
      efforts_ = current_;
    }
    if (received_data.temperature.size() >= temperature_.size()) {
      temperature_ = received_data.temperature;
    }

    // Parse fingertip sensor data
    if (supports_ft_sensor_ && fingertip_sensor_enabled_ && 
        received_data.fingertip_sensor.size() >= num_fingers_ * 6) {
      for (size_t finger = 0; finger < num_fingers_; finger++) {
        size_t base = finger * 6;
        fingertip_force_x_[finger] = received_data.fingertip_sensor[base + 0];
        fingertip_force_y_[finger] = received_data.fingertip_sensor[base + 1];
        fingertip_force_z_[finger] = received_data.fingertip_sensor[base + 2];
        fingertip_torque_x_[finger] = received_data.fingertip_sensor[base + 3];
        fingertip_torque_y_[finger] = received_data.fingertip_sensor[base + 4];
        fingertip_torque_z_[finger] = received_data.fingertip_sensor[base + 5];
      }
    }

    // Parse GPIO data
    if (supports_gpio_ && io_enabled_ && received_data.gpio.size() >= 4) {
      for (size_t i = 0; i < 4; i++) {
        gpio_states_[i] = received_data.gpio[i] ? 1.0 : 0.0;
      }
    }

    // Update connection status
    is_connected_.store(true);
    connection_status_ = 1.0;

    // Spin once to process service callbacks
    if (node_) {
      rclcpp::spin_some(node_);
    }

    return return_type::OK;
  } catch (const std::exception& e) {
    RCLCPP_ERROR(rclcpp::get_logger("SystemInterface"), 
                 "Unexpected error in read: %s", e.what());
    is_connected_.store(false);
    connection_status_ = 0.0;
    return return_type::ERROR;
  }
}

SystemInterface::return_type SystemInterface::write(
    [[maybe_unused]] const rclcpp::Time& time,
    [[maybe_unused]] const rclcpp::Duration& period) {
  if (!is_connected_.load()) {
    return return_type::ERROR;
  }

  std::vector<double> filter_effort_commands(effort_commands_.size());
  std::vector<double> duty(effort_commands_.size());
  std::vector<int> int_duty(effort_commands_.size());
  std::vector<int> current_mA(effort_commands_.size());

  for (size_t i = 0; i < effort_commands_.size(); ++i) {
    current_mA[i] = static_cast<int>(current_[i]);
  }

  try {
    filter_effort_commands = delto_gripper_helper::CurrentControl(
        effort_commands_.size(), current_mA, effort_commands_,
        current_limit_flag_, current_integral_);

    duty = delto_gripper_helper::ConvertDuty(
        effort_commands_.size(), filter_effort_commands);

    for (size_t i = 0; i < effort_commands_.size(); ++i) {
      int_duty[i] = static_cast<int>(duty[i] * 10);
      int_duty[i] = std::clamp(int_duty[i], -1000, 1000);
      
      // Apply motor direction based on firmware version
      int_duty[i] *= getMotorDirection(i);
    }

    delto_client_->SendDuty(int_duty);
    
    is_connected_.store(true);
    connection_status_ = 1.0;

  } catch (const std::exception& e) {
    RCLCPP_ERROR(rclcpp::get_logger("SystemInterface"), 
                 "Write error: %s", e.what());
    is_connected_.store(false);
    connection_status_ = 0.0;
    return return_type::ERROR;
  }

  return return_type::OK;
}

void SystemInterface::ftOffsetCallback(
    [[maybe_unused]] const std::shared_ptr<std_srvs::srv::Trigger::Request> request,
    std::shared_ptr<std_srvs::srv::Trigger::Response> response) {
  try {
    if (delto_client_ && supports_ft_sensor_ && fingertip_sensor_enabled_) {
      delto_client_->SetFTSensorOffset();
      response->success = true;
      response->message = "F/T sensor offset calibration completed successfully.";
      RCLCPP_INFO(rclcpp::get_logger("SystemInterface"),
                  "F/T sensor offset calibration completed.");
    } else {
      response->success = false;
      if (!supports_ft_sensor_) {
        response->message = "This model does not support F/T sensors.";
      } else if (!fingertip_sensor_enabled_) {
        response->message = "Fingertip sensor is not enabled.";
      } else {
        response->message = "Delto client is not initialized.";
      }
      RCLCPP_WARN(rclcpp::get_logger("SystemInterface"), "%s", response->message.c_str());
    }
  } catch (const std::exception& e) {
    response->success = false;
    response->message = std::string("F/T sensor offset calibration failed: ") + e.what();
    RCLCPP_ERROR(rclcpp::get_logger("SystemInterface"), "%s", response->message.c_str());
  }
}

void SystemInterface::gpioOutput1Callback(
    const std::shared_ptr<std_srvs::srv::SetBool::Request> request,
    std::shared_ptr<std_srvs::srv::SetBool::Response> response) {
  try {
    if (delto_client_ && supports_gpio_ && io_enabled_) {
      gpio_output1_state_ = request->data;
      delto_client_->SetGPIO(gpio_output1_state_, gpio_output2_state_, gpio_output3_state_);
      response->success = true;
      response->message = "GPIO output 1 set to " + std::string(request->data ? "ON" : "OFF");
      RCLCPP_INFO(rclcpp::get_logger("SystemInterface"), "%s", response->message.c_str());
    } else {
      response->success = false;
      response->message = io_enabled_ ? "Delto client is not initialized." : "IO is not enabled.";
      RCLCPP_WARN(rclcpp::get_logger("SystemInterface"), "%s", response->message.c_str());
    }
  } catch (const std::exception& e) {
    response->success = false;
    response->message = std::string("GPIO output 1 set failed: ") + e.what();
    RCLCPP_ERROR(rclcpp::get_logger("SystemInterface"), "%s", response->message.c_str());
  }
}

void SystemInterface::gpioOutput2Callback(
    const std::shared_ptr<std_srvs::srv::SetBool::Request> request,
    std::shared_ptr<std_srvs::srv::SetBool::Response> response) {
  try {
    if (delto_client_ && supports_gpio_ && io_enabled_) {
      gpio_output2_state_ = request->data;
      delto_client_->SetGPIO(gpio_output1_state_, gpio_output2_state_, gpio_output3_state_);
      response->success = true;
      response->message = "GPIO output 2 set to " + std::string(request->data ? "ON" : "OFF");
      RCLCPP_INFO(rclcpp::get_logger("SystemInterface"), "%s", response->message.c_str());
    } else {
      response->success = false;
      response->message = io_enabled_ ? "Delto client is not initialized." : "IO is not enabled.";
      RCLCPP_WARN(rclcpp::get_logger("SystemInterface"), "%s", response->message.c_str());
    }
  } catch (const std::exception& e) {
    response->success = false;
    response->message = std::string("GPIO output 2 set failed: ") + e.what();
    RCLCPP_ERROR(rclcpp::get_logger("SystemInterface"), "%s", response->message.c_str());
  }
}

void SystemInterface::gpioOutput3Callback(
    const std::shared_ptr<std_srvs::srv::SetBool::Request> request,
    std::shared_ptr<std_srvs::srv::SetBool::Response> response) {
  try {
    if (delto_client_ && supports_gpio_ && io_enabled_) {
      gpio_output3_state_ = request->data;
      delto_client_->SetGPIO(gpio_output1_state_, gpio_output2_state_, gpio_output3_state_);
      response->success = true;
      response->message = "GPIO output 3 set to " + std::string(request->data ? "ON" : "OFF");
      RCLCPP_INFO(rclcpp::get_logger("SystemInterface"), "%s", response->message.c_str());
    } else {
      response->success = false;
      response->message = io_enabled_ ? "Delto client is not initialized." : "IO is not enabled.";
      RCLCPP_WARN(rclcpp::get_logger("SystemInterface"), "%s", response->message.c_str());
    }
  } catch (const std::exception& e) {
    response->success = false;
    response->message = std::string("GPIO output 3 set failed: ") + e.what();
    RCLCPP_ERROR(rclcpp::get_logger("SystemInterface"), "%s", response->message.c_str());
  }
}

}  // namespace delto_hardware

#include "pluginlib/class_list_macros.hpp"
PLUGINLIB_EXPORT_CLASS(delto_hardware::SystemInterface,
                       hardware_interface::SystemInterface)
