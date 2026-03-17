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

#include "delto_tcp_comm/delto_developer_TCP.hpp"

namespace DeltoTCP {

// ============================================================================
// Helper Functions
// ============================================================================

int Communication::GetMotorCount(uint16_t model) {
  switch (model) {
    case static_cast<uint16_t>(ModelType::DG3F_B):
    case static_cast<uint16_t>(ModelType::DG3F_M):
      return 12;
    case static_cast<uint16_t>(ModelType::DG4F):
      return 18;
    case static_cast<uint16_t>(ModelType::DG5F):
    case static_cast<uint16_t>(ModelType::DG5F_L):
    case static_cast<uint16_t>(ModelType::DG5F_R):
      return 20;
    default:
      std::cerr << "Unknown model type: 0x" << std::hex << model << std::dec
                << std::endl;
      return 12;
  }
}

int Communication::GetBytePerMotor(uint16_t model) {
  // DG3F-B (구형): 5 bytes per motor
  // ID(1) + PosL(1) + PosH(1) + CurL(1) + CurH(1)
  if (model == static_cast<uint16_t>(ModelType::DG3F_B)) {
    return 5;
  }

  // 신형 모델들: 8 bytes per motor
  // ID(1) + PosL(1) + PosH(1) + CurL(1) + CurH(1) + TempL(1) + TempH(1) + Vel(1)
  return 8;
}

int Communication::GetFingerCount(uint16_t model) {
  switch (model) {
    case static_cast<uint16_t>(ModelType::DG3F_B):
    case static_cast<uint16_t>(ModelType::DG3F_M):
      return 3;
    case static_cast<uint16_t>(ModelType::DG4F):
      return 4;
    case static_cast<uint16_t>(ModelType::DG5F):
    case static_cast<uint16_t>(ModelType::DG5F_L):
    case static_cast<uint16_t>(ModelType::DG5F_R):
      return 5;
    default:
      return 5;
  }
}

bool Communication::IsNewModel() const {
  return model_ != static_cast<uint16_t>(ModelType::DG3F_B);
}

bool Communication::SupportsExtendedFeatures() const {
  // New models (DG3F-M, DG4F, DG5F) support extended features
  return model_ == static_cast<uint16_t>(ModelType::DG3F_M) ||
         model_ == static_cast<uint16_t>(ModelType::DG4F) ||
         model_ == static_cast<uint16_t>(ModelType::DG5F) ||
         model_ == static_cast<uint16_t>(ModelType::DG5F_L) ||
         model_ == static_cast<uint16_t>(ModelType::DG5F_R);
}

int16_t Communication::CalculateExpectedResponseLength() {
  int16_t length = HEADER_SIZE + motor_count_ * byte_per_motor_;

  if (SupportsExtendedFeatures()) {
    if (fingertip_sensor_) {
      length += 2 * 6 * finger_count_;  // 2 bytes × 6 axes × N fingers
    }
    if (io_) {
      length += GPIO_SIZE;
    }
  }

  return length;
}

int16_t Communication::CombineMsg(uint8_t data1, uint8_t data2) {
  // Big Endian: data1 = high byte, data2 = low byte
  return static_cast<int16_t>(static_cast<uint16_t>(data1 << 8) | data2);
}

int8_t Communication::ConvertByte(uint8_t byte) {
  if (byte >= 0x80) {
    return static_cast<int8_t>(byte - 0x100);
  }
  return static_cast<int8_t>(byte);
}

// ============================================================================
// Constructor / Destructor
// ============================================================================

Communication::Communication(const std::string& ip, int port, uint16_t model,
                             bool fingertip_sensor, bool io)
    : ip_(ip),
      port_(port),
      model_(model),
      actual_model_(0),
      fingertip_sensor_(fingertip_sensor),
      io_(io),
      socket_(io_context_),
      motor_count_(GetMotorCount(model)),
      byte_per_motor_(GetBytePerMotor(model)),
      finger_count_(GetFingerCount(model)),
      expected_response_length_(CalculateExpectedResponseLength()) {}

std::string Communication::ModelToString(uint16_t model) {
  switch (model) {
    case static_cast<uint16_t>(ModelType::DG3F_B):
      return "DG3F-B (0x3F01)";
    case static_cast<uint16_t>(ModelType::DG3F_M):
      return "DG3F-M (0x3F02)";
    case static_cast<uint16_t>(ModelType::DG4F):
      return "DG4F (0x4F02)";
    case static_cast<uint16_t>(ModelType::DG5F):
      return "DG5F (0x5F02)";
    case static_cast<uint16_t>(ModelType::DG5F_L):
      return "DG5F-L (0x5F12)";
    case static_cast<uint16_t>(ModelType::DG5F_R):
      return "DG5F-R (0x5F22)";
    default:
      return "Unknown (0x" + 
             ([](uint16_t v) {
               char buf[8];
               snprintf(buf, sizeof(buf), "%04X", v);
               return std::string(buf);
             })(model) + ")";
  }
}

Communication::~Communication() {
  if (socket_.is_open()) {
    socket_.close();
  }
}

// ============================================================================
// Connection Management
// ============================================================================

void Communication::Connect() {
  tcp::resolver resolver(io_context_);
  boost::system::error_code ec;

  // Ensure socket is closed before attempting to connect
  if (socket_.is_open()) {
    socket_.close();
  }

  boost::asio::connect(socket_, resolver.resolve(ip_, std::to_string(port_)),
                       ec);

  if (ec) {
    std::cerr << "Could not connect: " << ec.message() << std::endl;
    throw std::runtime_error("Connection failed: " + ec.message());
  }

  // Get firmware version and model info (single request)
  // Response format: Length(2) + CMD(1) + Model(2) + Version(2) = 7 bytes
  {
    std::array<uint8_t, 3> request;
    request[0] = 0x00;              // Length_h
    request[1] = 0x03;              // Length_l
    request[2] = GET_VERSION_CMD;   // CMD

    socket_.write_some(boost::asio::buffer(request), ec);
    if (ec) {
      std::cerr << "Error sending version request: " << ec.message()
                << std::endl;
      throw std::runtime_error("Failed to get device info: " + ec.message());
    }

    std::vector<uint8_t> response(7);
    boost::asio::read(socket_, boost::asio::buffer(response),
                      boost::asio::transfer_exactly(7), ec);
    if (ec) {
      std::cerr << "Error reading version response: " << ec.message()
                << std::endl;
      throw std::runtime_error("Failed to read device info: " + ec.message());
    }

    // Parse model ID (bytes 3-4, Big Endian)
    actual_model_ = CombineMsg(response[3], response[4]);

    // Parse firmware version (bytes 5-6)
    firmware_version_ = {response[5], response[6]};

    std::cout << "========================================" << std::endl;
    std::cout << "Device Info:" << std::endl;
    std::cout << "  Firmware Version: "
              << static_cast<int>(firmware_version_[0]) << "."
              << static_cast<int>(firmware_version_[1]) << std::endl;
    std::cout << "  Configured Model: " << ModelToString(model_) << std::endl;
    std::cout << "  Actual Model:     " << ModelToString(actual_model_) << std::endl;

    // Validate model
    if (model_ != actual_model_) {
      std::cerr << "========================================" << std::endl;
      std::cerr << "WARNING: Model mismatch detected!" << std::endl;
      std::cerr << "  Expected: " << ModelToString(model_) << std::endl;
      std::cerr << "  Got:      " << ModelToString(actual_model_) << std::endl;
      std::cerr << "  Please check your URDF/xacro configuration." << std::endl;
      std::cerr << "========================================" << std::endl;
    } else {
      std::cout << "  Status:           OK (Model matched)" << std::endl;
    }
    std::cout << "========================================" << std::endl;
  }

  std::cout << "Connected to Delto Gripper (Model: " << ModelToString(actual_model_)
            << ", Motors: " << motor_count_ << ")" << std::endl;
}

void Communication::Disconnect() {
  if (socket_.is_open()) {
    socket_.close();
    std::cout << "Disconnected from Delto Gripper" << std::endl;
  }
}

// ============================================================================
// Data Exchange
// ============================================================================

bool Communication::ReadFullPacket(std::vector<uint8_t>& buffer) {
  boost::system::error_code ec;

  buffer.resize(expected_response_length_);

  std::size_t bytes_read = boost::asio::read(
      socket_, boost::asio::buffer(buffer.data(), expected_response_length_),
      boost::asio::transfer_exactly(expected_response_length_), ec);

  if (ec) {
    std::cerr << "Read error: " << ec.message() << std::endl;
    return false;
  }

  return bytes_read == static_cast<std::size_t>(expected_response_length_);
}

DeltoReceivedData Communication::GetData() {
  // Build request packet
  std::vector<uint8_t> request;

  if (model_ == static_cast<uint16_t>(ModelType::DG3F_B)) {
    // DG3F-B: Length(2) + CMD(1) + ID(2)
    request = {0x00, 0x05, GET_DATA_CMD, 0x01, 0x02};
  } else {
    // New models: Length(2) + CMD(1) + ID(4) [+ optional IDs]
    request = {0x00, 0x07, GET_DATA_CMD, 0x01, 0x02, 0x03, 0x04};

    if (SupportsExtendedFeatures()) {
      if (fingertip_sensor_) {
        request.push_back(0x05);
      }
      if (io_) {
        request.push_back(0x06);
      }
      // Update length field
      request[1] = static_cast<uint8_t>(request.size());
    }
  }

  // Send request
  boost::system::error_code ec;
  socket_.write_some(boost::asio::buffer(request), ec);

  if (ec) {
    std::cerr << "Error sending request: " << ec.message() << std::endl;

    // Try to reconnect on connection errors
    if (ec == boost::asio::error::broken_pipe ||
        ec == boost::asio::error::connection_reset ||
        ec == boost::asio::error::connection_aborted) {
      std::cerr << "Connection lost, attempting to reconnect..." << std::endl;
      try {
        socket_.close();
        Connect();
        socket_.write_some(boost::asio::buffer(request), ec);
        if (ec) {
          std::cerr << "Failed to send after reconnection: " << ec.message()
                    << std::endl;
          return DeltoReceivedData{};
        }
      } catch (const std::exception& e) {
        std::cerr << "Reconnection failed: " << e.what() << std::endl;
        return DeltoReceivedData{};
      }
    } else {
      return DeltoReceivedData{};
    }
  }

  // Read response
  std::vector<uint8_t> response;
  if (!ReadFullPacket(response)) {
    std::cerr << "Failed to read full packet" << std::endl;
    return DeltoReceivedData{};
  }

  // Validate response
  uint16_t length = CombineMsg(response[0], response[1]);
  uint8_t cmd = response[2];

  if (cmd != GET_DATA_CMD ||
      static_cast<int>(response.size()) != expected_response_length_) {
    std::cerr << "Invalid header (CMD or LENGTH mismatch)" << std::endl;
    std::cerr << "Expected: " << expected_response_length_
              << ", Got: " << response.size() << std::endl;
    std::cerr << "Received header: CMD = 0x" << std::hex << static_cast<int>(cmd)
              << ", Length = 0x" << static_cast<int>(length) << std::dec
              << std::endl;
    return DeltoReceivedData{};
  }

  // Parse motor data
  DeltoReceivedData received_data;
  received_data.joint.resize(motor_count_);
  received_data.current.resize(motor_count_);

  if (IsNewModel()) {
    received_data.velocity.resize(motor_count_);
    received_data.temperature.resize(motor_count_);
  }

  for (int i = 0; i < motor_count_; i++) {
    size_t base = HEADER_SIZE + i * byte_per_motor_;
    [[maybe_unused]] uint8_t motor_id = response[base];

    uint8_t posL = response[base + 1];
    uint8_t posH = response[base + 2];
    uint8_t curL = response[base + 3];
    uint8_t curH = response[base + 4];

    int16_t raw_position = CombineMsg(posL, posH);
    int16_t raw_current = CombineMsg(curL, curH);

    received_data.joint[i] = raw_position * POSITION_SCALE;
    received_data.current[i] = raw_current * CURRENT_SCALE;

    // New models have temperature and velocity
    if (IsNewModel()) {
      uint8_t tempL = response[base + 5];
      uint8_t tempH = response[base + 6];
      int16_t raw_temperature = CombineMsg(tempL, tempH);
      received_data.temperature[i] = raw_temperature * 0.1;

      int8_t raw_vel = ConvertByte(response[base + 7]);
      received_data.velocity[i] =
          static_cast<double>(raw_vel) * VELOCITY_SCALE * -1;
    }
  }

  // Parse fingertip sensor data (model-specific finger count)
  if (SupportsExtendedFeatures() && fingertip_sensor_) {
    size_t ft_base = HEADER_SIZE + motor_count_ * byte_per_motor_;
    received_data.fingertip_sensor.resize(finger_count_ * 6);  // N fingers × 6 axes

    for (int finger = 0; finger < finger_count_; finger++) {
      for (int axis = 0; axis < 6; axis++) {
        size_t offset = ft_base + (finger * 6 + axis) * 2;
        uint8_t dataL = response[offset];
        uint8_t dataH = response[offset + 1];
        int16_t raw_value = CombineMsg(dataL, dataH);

        if (axis < 3) {
          // Force (Fx, Fy, Fz): 0.1 N → N
          received_data.fingertip_sensor[finger * 6 + axis] = raw_value * 0.1;
        } else {
          // Torque (Tx, Ty, Tz): 1 mNm → Nm
          received_data.fingertip_sensor[finger * 6 + axis] = raw_value * 0.001;
        }
      }
    }
  }

  // Parse GPIO data (only for models with GPIO enabled)
  if (SupportsExtendedFeatures() && io_) {
    size_t gpio_base = HEADER_SIZE + motor_count_ * byte_per_motor_;
    if (fingertip_sensor_) {
      gpio_base += 2 * 6 * finger_count_;  // 2 bytes × 6 axes × N fingers
    }

    received_data.gpio.resize(4);
    for (int i = 0; i < 4; i++) {
      received_data.gpio[i] = (response[gpio_base + i] != 0);
    }
  }

  return received_data;
}

void Communication::SendDuty(std::vector<int>& duty) {
  std::vector<uint8_t> tcp_data_send;

  uint16_t total_packet_size = 3 + motor_count_ * 3;  // Header + ID(1) + Duty(2) per motor
  tcp_data_send.resize(total_packet_size);

  tcp_data_send[0] = (total_packet_size >> 8) & 0xFF;  // Length_h
  tcp_data_send[1] = (total_packet_size) & 0xFF;       // Length_l
  tcp_data_send[2] = SET_DUTY_CMD;                     // CMD

  for (int i = 0; i < motor_count_; ++i) {
    tcp_data_send[3 + i * 3] = i + 1;                  // ID
    tcp_data_send[4 + i * 3] = (duty[i] >> 8) & 0xFF;  // High byte
    tcp_data_send[5 + i * 3] = (duty[i]) & 0xFF;       // Low byte
  }

  boost::system::error_code ec;
  socket_.write_some(boost::asio::buffer(tcp_data_send), ec);

  if (ec) {
    std::cerr << "Error sending duty: " << ec.message() << std::endl;

    // Try to reconnect on connection errors
    if (ec == boost::asio::error::broken_pipe ||
        ec == boost::asio::error::connection_reset ||
        ec == boost::asio::error::connection_aborted) {
      std::cerr << "Connection lost, attempting to reconnect..." << std::endl;
      try {
        socket_.close();
        Connect();
        socket_.write_some(boost::asio::buffer(tcp_data_send), ec);
        if (ec) {
          std::cerr << "Failed to send after reconnection: " << ec.message()
                    << std::endl;
        }
      } catch (const std::exception& e) {
        std::cerr << "Reconnection failed: " << e.what() << std::endl;
      }
    }
  }
}

// ============================================================================
// GPIO Control
// ============================================================================

void Communication::SetGPIO(bool output1, bool output2, bool output3) {
  if (!SupportsExtendedFeatures()) {
    std::cerr << "GPIO not supported on this model" << std::endl;
    return;
  }

  std::array<uint8_t, 6> request;
  request[0] = 0x00;                       // Length_h
  request[1] = 0x06;                       // Length_l
  request[2] = SET_GPIO_CMD;               // CMD 0x06
  request[3] = output1 ? 0x01 : 0x00;      // Output 1
  request[4] = output2 ? 0x01 : 0x00;      // Output 2
  request[5] = output3 ? 0x01 : 0x00;      // Output 3

  boost::system::error_code ec;
  socket_.write_some(boost::asio::buffer(request), ec);

  if (ec) {
    std::cerr << "Error sending GPIO command: " << ec.message() << std::endl;
  }
}

// ============================================================================
// F/T Sensor Control
// ============================================================================

void Communication::SetFTSensorOffset() {
  if (!SupportsExtendedFeatures()) {
    std::cerr << "F/T sensor not supported on this model" << std::endl;
    return;
  }

  std::array<uint8_t, 3> request;
  request[0] = 0x00;                       // Length_h
  request[1] = 0x03;                       // Length_l
  request[2] = SET_FT_SENSOR_OFFSET_CMD;   // CMD 0x0B

  boost::system::error_code ec;
  socket_.write_some(boost::asio::buffer(request), ec);

  if (ec) {
    std::cerr << "Error sending F/T sensor offset command: " << ec.message()
              << std::endl;
  }
}

// ============================================================================
// Version Info
// ============================================================================

std::vector<uint8_t> Communication::GetFirmwareVersion() {
  return firmware_version_;
}

}  // namespace DeltoTCP
