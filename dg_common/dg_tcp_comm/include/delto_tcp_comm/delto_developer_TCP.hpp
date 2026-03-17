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

#pragma once
#include <array>
#include <boost/asio.hpp>
#include <cmath>
#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

namespace DeltoTCP {

// ============================================================================
// Model IDs
// ============================================================================
enum class ModelType : uint16_t {
  DG3F_B = 0x3F01,   // DG-3F B (구형, 5바이트/모터)
  DG3F_M = 0x3F02,   // DG-3F M (신형, 8바이트/모터)
  DG4F   = 0x4F02,   // DG-4F
  DG5F   = 0x5F02,   // DG-5F
  DG5F_L = 0x5F12,   // DG-5F LEFT
  DG5F_R = 0x5F22,   // DG-5F RIGHT
};

// ============================================================================
// Data Structures
// ============================================================================
struct DeltoReceivedData {
  std::vector<double> joint;             // radian
  std::vector<double> current;           // A
  std::vector<double> temperature;       // Celsius
  std::vector<double> velocity;          // rad/s
  std::vector<bool> gpio;                // GPIO states (4 elements)
  std::vector<double> fingertip_sensor;  // F/T sensor data (N fingers × 6 axes: 3F=18, 4F=24, 5F=30)
};

// ============================================================================
// Communication Class
// ============================================================================
using boost::asio::ip::tcp;
namespace asio = boost::asio;

class Communication {
 public:
  /**
   * @brief Constructor
   * @param ip IP address of the Delto gripper
   * @param port TCP port (default: 10000)
   * @param model Model ID (see ModelType enum)
   * @param fingertip_sensor Enable fingertip F/T sensor (only DG5F)
   * @param io Enable GPIO (only DG5F)
   */
  Communication(const std::string& ip, int port, uint16_t model,
                bool fingertip_sensor = false, bool io = false);
  ~Communication();

  // Connection management
  void Connect();
  void Disconnect();
  bool IsConnected() const { return socket_.is_open(); }

  // Data exchange
  DeltoReceivedData GetData();
  void SendDuty(std::vector<int>& duty);

  // GPIO control (only for models that support it)
  void SetGPIO(bool output1, bool output2, bool output3);

  // F/T sensor control (only for models that support it)
  void SetFTSensorOffset();

  // Version info
  std::vector<uint8_t> GetFirmwareVersion();
  
  // Model info
  uint16_t GetActualModel() const { return actual_model_; }
  uint16_t GetConfiguredModel() const { return model_; }
  static std::string ModelToString(uint16_t model);

 private:
  // Connection parameters
  std::string ip_;
  int port_;
  uint16_t model_;           // Configured model (from parameter)
  uint16_t actual_model_;    // Actual model (read from device)
  bool fingertip_sensor_;
  bool io_;

  // Network
  asio::io_context io_context_;
  tcp::socket socket_;
  std::vector<uint8_t> firmware_version_;

  // Model-specific parameters
  const int motor_count_;
  const int byte_per_motor_;
  const int finger_count_;
  const int16_t expected_response_length_;

  // Protocol constants
  static constexpr uint8_t GET_DATA_CMD = 0x01;
  static constexpr uint8_t SET_DUTY_CMD = 0x05;
  static constexpr uint8_t SET_GPIO_CMD = 0x06;
  static constexpr uint8_t GET_VERSION_CMD = 0x08;
  static constexpr uint8_t SET_FT_SENSOR_OFFSET_CMD = 0x0B;

  static constexpr std::size_t HEADER_SIZE = 3;  // Length(2) + CMD(1)
  static constexpr double POSITION_SCALE = (M_PI / 1800.0);
  static constexpr double CURRENT_SCALE = 1.0;
  static constexpr double VELOCITY_SCALE = (M_PI / 1800.0);

  // GPIO data size: 4 bytes (Output 1-3, Input 1)
  static constexpr int GPIO_SIZE = 4;

  // Helper functions
  int GetMotorCount(uint16_t model);
  int GetBytePerMotor(uint16_t model);
  int GetFingerCount(uint16_t model);
  int16_t CalculateExpectedResponseLength();
  bool ReadFullPacket(std::vector<uint8_t>& buffer);
  int16_t CombineMsg(uint8_t data1, uint8_t data2);
  int8_t ConvertByte(uint8_t byte);
  bool IsNewModel() const;
  bool SupportsExtendedFeatures() const;
};

}  // namespace DeltoTCP
