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

#ifndef DELTO_HARDWARE__SYSTEM_INTERFACE_HPP_
#define DELTO_HARDWARE__SYSTEM_INTERFACE_HPP_

#include <atomic>
#include <memory>
#include <string>
#include <vector>
#include <array>

#include "hardware_interface/handle.hpp"
#include "hardware_interface/hardware_info.hpp"
#include "hardware_interface/system_interface.hpp"
#include "hardware_interface/types/hardware_interface_return_values.hpp"
#include "rclcpp/rclcpp.hpp"
#include "rclcpp_lifecycle/state.hpp"
#include "std_srvs/srv/trigger.hpp"
#include "std_srvs/srv/set_bool.hpp"

#include "delto_tcp_comm/delto_developer_TCP.hpp"
#include "delto_hardware/delto_gripper_helper.hpp"

namespace delto_hardware {

// Model IDs
constexpr uint16_t MODEL_DG3F_B = 0x3F01;
constexpr uint16_t MODEL_DG3F_M = 0x3F02;
constexpr uint16_t MODEL_DG4F   = 0x4F02;
constexpr uint16_t MODEL_DG5F_L = 0x5F12;
constexpr uint16_t MODEL_DG5F_R = 0x5F22;

// Custom hardware interface type for temperature
namespace delto_interface {
constexpr char HW_IF_TEMPERATURE[] = "temperature";
}

// Finger names for F/T sensor interfaces
// DG3F: 3 fingers, DG4F: 4 fingers, DG5F: 5 fingers
static const std::array<std::string, 5> FINGER_NAMES_5F = {
    "finger_1", "finger_2", "finger_3", "finger_4", "finger_5"
};
static const std::array<std::string, 4> FINGER_NAMES_4F = {
    "finger_1", "finger_2", "finger_3", "finger_4"
};
static const std::array<std::string, 3> FINGER_NAMES_3F = {
    "finger_1", "finger_2", "finger_3"
};

// Motor direction arrays for DG5F
static const int DG5F_LEFT_MOTOR_DIR[20] = {
    1, 1, 1,  1, -1, 1, 1,  1, -1, 1,
    1, 1, -1, 1, 1,  1, -1, 1, -1, -1
};

static const int DG5F_RIGHT_MOTOR_DIR[20] = {
    1, 1, -1, -1, -1, 1, 1,  1,
    -1, 1, 1, 1, -1, 1, 1, 1,
    -1, -1, -1, -1
};

class SystemInterface : public hardware_interface::SystemInterface {
 public:
  using return_type = hardware_interface::return_type;
  using CallbackReturn = rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn;

  CallbackReturn on_init(const hardware_interface::HardwareInfo& info) override;
  CallbackReturn on_activate(const rclcpp_lifecycle::State& previous_state) override;
  CallbackReturn on_deactivate(const rclcpp_lifecycle::State& previous_state) override;
  CallbackReturn on_shutdown(const rclcpp_lifecycle::State& previous_state) override;

  std::vector<hardware_interface::StateInterface> export_state_interfaces() override;
  std::vector<hardware_interface::CommandInterface> export_command_interfaces() override;

  return_type prepare_command_mode_switch(
      const std::vector<std::string>& start_interfaces,
      const std::vector<std::string>& stop_interfaces) override;

  return_type read(const rclcpp::Time& time, const rclcpp::Duration& period) override;
  return_type write(const rclcpp::Time& time, const rclcpp::Duration& period) override;

 private:
  // Helper methods
  void initModelSpecificSettings();
  bool checkFirmwareCompatibility();
  int getMotorDirection(size_t joint_index) const;
  std::string getFingerName(size_t finger_index) const;
  
  // Service callbacks
  void ftOffsetCallback(
      const std::shared_ptr<std_srvs::srv::Trigger::Request> request,
      std::shared_ptr<std_srvs::srv::Trigger::Response> response);
  void gpioOutput1Callback(
      const std::shared_ptr<std_srvs::srv::SetBool::Request> request,
      std::shared_ptr<std_srvs::srv::SetBool::Response> response);
  void gpioOutput2Callback(
      const std::shared_ptr<std_srvs::srv::SetBool::Request> request,
      std::shared_ptr<std_srvs::srv::SetBool::Response> response);
  void gpioOutput3Callback(
      const std::shared_ptr<std_srvs::srv::SetBool::Request> request,
      std::shared_ptr<std_srvs::srv::SetBool::Response> response);

  // Connection parameters
  std::string delto_ip_;
  int delto_port_;
  uint16_t model_;
  std::string hand_type_;  // "left" or "right" for DG5F
  
  // Model capabilities (determined by model ID)
  bool supports_ft_sensor_;      // DG3F-M, DG4F, DG5F
  bool supports_gpio_;           // All models (DG3F-B, DG3F-M, DG4F, DG5F)
  bool supports_temperature_;    // Future use
  size_t num_fingers_;           // 3, 4, or 5
  size_t num_joints_;            // 12, 18, or 20
  
  // User-enabled features (from parameters)
  bool fingertip_sensor_enabled_;
  bool io_enabled_;
  
  // Firmware version
  std::vector<uint8_t> firmware_version_;
  bool firmware_dir_revised_;  // Motor direction revised in firmware
  
  // Minimum firmware versions for motor direction revision
  // DG3F-B: v3.6+, DG3F-M: v2.8+, DG5F: v2.8+, DG4F: always revised
  int min_firmware_major_;
  int min_firmware_minor_;
  
  // Motor direction array (for models that need it)
  std::vector<int> motor_dir_;

  // Joint state data
  std::vector<double> positions_;
  std::vector<double> velocities_;
  std::vector<double> efforts_;
  std::vector<double> temperature_;
  std::vector<double> effort_commands_;
  std::vector<double> current_;
  
  // Current control
  std::vector<int> current_limit_flag_;
  std::vector<double> current_integral_;

  // Fingertip F/T sensor data
  std::vector<double> fingertip_force_x_;
  std::vector<double> fingertip_force_y_;
  std::vector<double> fingertip_force_z_;
  std::vector<double> fingertip_torque_x_;
  std::vector<double> fingertip_torque_y_;
  std::vector<double> fingertip_torque_z_;

  // GPIO data (4 values: Output 1-3, Input 1)
  std::vector<double> gpio_states_;
  bool gpio_output1_state_;
  bool gpio_output2_state_;
  bool gpio_output3_state_;

  // Connection status
  std::atomic<bool> is_connected_;
  double connection_status_;

  // Communication client
  std::unique_ptr<DeltoTCP::Communication> delto_client_;

  // ROS2 node for services
  rclcpp::Node::SharedPtr node_;
  rclcpp::Service<std_srvs::srv::Trigger>::SharedPtr ft_offset_service_;
  rclcpp::Service<std_srvs::srv::SetBool>::SharedPtr gpio_output1_service_;
  rclcpp::Service<std_srvs::srv::SetBool>::SharedPtr gpio_output2_service_;
  rclcpp::Service<std_srvs::srv::SetBool>::SharedPtr gpio_output3_service_;
};

}  // namespace delto_hardware

#endif  // DELTO_HARDWARE__SYSTEM_INTERFACE_HPP_
