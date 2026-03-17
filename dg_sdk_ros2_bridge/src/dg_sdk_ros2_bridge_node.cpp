#include "dg_bridge_includes.hpp"

class DGSDKRos2Bridge : public rclcpp::Node {
 public:
  DGSDKRos2Bridge() : Node("dg_sdk_ros2_bridge") {
    instance_ = this;
    RCLCPP_INFO(this->get_logger(), "DG SDK ROS2 Bridge Node Starting...");

    connected_publisher_ =
        this->create_publisher<std_msgs::msg::Bool>("/dg/connected", 10);

    gripper_data_publisher_ =
        this->create_publisher<dg_msgs::msg::ReceivedGripperData>(
            "/dg/gripper_data", 10);

    period_publisher_ = this->create_publisher<std_msgs::msg::Int32>(
        "/dg/communication_period", 10);

    fingertip_sensor_publisher_ =
        this->create_publisher<dg_msgs::msg::ReceivedFingertipSensorData>(
            "/dg/fingertip_sensor_data", 10);

    gpio_data_publisher_ =
        this->create_publisher<dg_msgs::msg::ReceivedGPIOData>("/dg/gpio_data",
                                                               10);

    diagnosis_publisher_ =
        this->create_publisher<dg_msgs::msg::DiagnosisSystem>(
            "/dg/diagnosis_system", 10);

    setupServices();
    InitializedCallback();

    RCLCPP_INFO(this->get_logger(),
                "DG SDK ROS2 Bridge Node Started Successfully");
  }
  static void OnConnected() {
    // std::cout << "Connected gripper server" << std::endl;
    RCLCPP_INFO(rclcpp::get_logger("dg_sdk_ros2_bridge"),
                "Connected gripper server");

    if (instance_) {
      auto msg = std_msgs::msg::Bool();
      msg.data = true;
      instance_->connected_publisher_->publish(msg);
    }
  }

  static void OnDisconnected() {
    // std::cout << "Disconnected gripper server" << std::endl;
    RCLCPP_INFO(rclcpp::get_logger("dg_sdk_ros2_bridge"),
                "Disconnected gripper server");
    if (instance_) {
      auto msg = std_msgs::msg::Bool();
      msg.data = false;
      instance_->connected_publisher_->publish(msg);
    }
  }

  static void RecvGripperData(const ReceivedGripperData data) {
    // float joint[MAX_JOINT_COUNT];
    // int current[MAX_JOINT_COUNT];
    // int velocity[MAX_JOINT_COUNT];
    // float temperature[MAX_JOINT_COUNT];
    // float TCP[6 * MAX_FINGER_COUNT];
    // int moving;
    // int targetArrived;
    // int blendMoveState;
    // int currentBlendIndex;
    // int productID;
    // int firmwareVersion;

    auto msg = dg_msgs::msg::ReceivedGripperData();

    std::copy(data.joint, data.joint + MAX_JOINT_COUNT, msg.joint.begin());
    std::copy(data.current, data.current + MAX_JOINT_COUNT,
              msg.current.begin());
    std::copy(data.velocity, data.velocity + MAX_JOINT_COUNT,
              msg.velocity.begin());

    std::copy(data.temperature, data.temperature + MAX_JOINT_COUNT,
              msg.temperature.begin());
    std::copy(data.TCP, data.TCP + 6 * MAX_FINGER_COUNT, msg.tcp.begin());
    msg.moving = data.moving;
    msg.target_arrived = data.targetArrived;
    msg.blend_move_state = data.blendMoveState;
    msg.current_blend_index = data.currentBlendIndex;
    msg.product_id = data.productID;
    msg.firmware_version = data.firmwareVersion;

    // Publish the gripper data
    if (instance_) {
      instance_->gripper_data_publisher_->publish(msg);
    }
  }

  static void OnCountPeriod(const int period) {
    if (instance_) {
      auto msg = std_msgs::msg::Int32();
      msg.data = period;
      instance_->period_publisher_->publish(msg);
    }
  }

  static void OnDiagnosisData(const DiagnosisSystem diagnose) {
    // diag = diagnose;
    int process;
    int step;
    int jointId;
    int period;
    int joint;
    int temperature;

    process = diagnose.process;
    step = diagnose.step;
    jointId = diagnose.jointId;
    period = diagnose.period;
    joint = diagnose.joint;
    temperature = diagnose.temperature;

    auto msg = dg_msgs::msg::DiagnosisSystem();
    msg.process = process;
    msg.step = step;
    msg.joint_id = jointId;
    msg.period = period;
    msg.joint = joint;
    msg.temperature = temperature;

    if (instance_) {
      instance_->diagnosis_publisher_->publish(msg);
    }
  }

  static void RecvSensorData(const ReceivedFingertipSensorData recvSensorData) {
    // float forceTorque[6 * MAX_FINGER_COUNT];
    // sensorData = recvSensorData;
    auto msg = dg_msgs::msg::ReceivedFingertipSensorData();
    std::copy(recvSensorData.forceTorque,
              recvSensorData.forceTorque + 6 * MAX_FINGER_COUNT,
              msg.force_torque.begin());

    if (instance_) {
      instance_->fingertip_sensor_publisher_->publish(msg);
    }
  }
  static void RecvGPIOData(const ReceivedGPIOData recvGPIOData) {
    // gpioData = recvGPIOData; // TODO: Declare this variable if needed

    auto msg = dg_msgs::msg::ReceivedGPIOData();
    std::copy(recvGPIOData.GPIO, recvGPIOData.GPIO + 4, msg.gpio.begin());

    if (instance_) {
      instance_->gpio_data_publisher_->publish(msg);
    }
  }

  void InitializedCallback() {
    CallbackForOnConnected(*OnConnected);
    CallbackForOnDisconnected(*OnDisconnected);
    CallbackForOnReceivedGripperData(*RecvGripperData);
    CallbackForOnCommunicationPeriod(*OnCountPeriod);
    CallbackForOnDiagnosisSystem(*OnDiagnosisData);
    CallbackForOnReceivedFingertipSensorData(*RecvSensorData);
    CallbackForOnReceivedGPIOData(*RecvGPIOData);
  }

 private:
  static DGSDKRos2Bridge* instance_;

  void setupServices() {
    // System Setting Services

    set_gripper_system_service_ =
        this->create_service<dg_msgs::srv::SetGripperSystem>(
            "/dg/set_gripper_system",
            std::bind(&DGSDKRos2Bridge::setGripperSystemCallback, this,
                      std::placeholders::_1, std::placeholders::_2));

    set_gripper_option_service_ =
        this->create_service<dg_msgs::srv::SetGripperOption>(
            "/dg/set_gripper_option",
            std::bind(&DGSDKRos2Bridge::setGripperOptionCallback, this,
                      std::placeholders::_1, std::placeholders::_2));

    connect_to_gripper_service_ =
        this->create_service<dg_msgs::srv::ConnectToGripper>(
            "/dg/connect_to_gripper",
            std::bind(&DGSDKRos2Bridge::connectToGripperCallback, this,
                      std::placeholders::_1, std::placeholders::_2));

    disconnect_to_gripper_service_ =
        this->create_service<dg_msgs::srv::DisconnectToGripper>(
            "/dg/disconnect_to_gripper",
            std::bind(&DGSDKRos2Bridge::disconnectToGripperCallback, this,
                      std::placeholders::_1, std::placeholders::_2));

    system_start_service_ = this->create_service<dg_msgs::srv::SystemStart>(
        "/dg/system_start",
        std::bind(&DGSDKRos2Bridge::systemStartCallback, this,
                  std::placeholders::_1, std::placeholders::_2));

    system_stop_service_ = this->create_service<dg_msgs::srv::SystemStop>(
        "/dg/system_stop",
        std::bind(&DGSDKRos2Bridge::systemStopCallback, this,
                  std::placeholders::_1, std::placeholders::_2));

    set_ip_service_ = this->create_service<dg_msgs::srv::SetIp>(
        "/dg/set_ip", std::bind(&DGSDKRos2Bridge::setIpCallback, this,
                                std::placeholders::_1, std::placeholders::_2));

    set_low_pass_filter_alpha_service_ =
        this->create_service<dg_msgs::srv::SetLowPassFilterAlpha>(
            "/dg/set_low_pass_filter_alpha",
            std::bind(&DGSDKRos2Bridge::setLowPassFilterAlphaCallback, this,
                      std::placeholders::_1, std::placeholders::_2));

    set_gpio_output_service_ =
        this->create_service<dg_msgs::srv::SetGPIOOutput>(
            "/dg/set_gpio_output",
            std::bind(&DGSDKRos2Bridge::setGPIOOutputCallback, this,
                      std::placeholders::_1, std::placeholders::_2));

    set_gpio_output_all_service_ =
        this->create_service<dg_msgs::srv::SetGPIOOutputAll>(
            "/dg/set_gpio_output_all",
            std::bind(&DGSDKRos2Bridge::setGPIOOutputAllCallback, this,
                      std::placeholders::_1, std::placeholders::_2));

    set_torque_limit_mode_service_ =
        this->create_service<dg_msgs::srv::SetTorqueLimitMode>(
            "/dg/set_torque_limit_mode",
            std::bind(&DGSDKRos2Bridge::setTorqueLimitModeCallback, this,
                      std::placeholders::_1, std::placeholders::_2));

    set_boot_mode_service_ = this->create_service<dg_msgs::srv::SetBootMode>(
        "/dg/set_boot_mode",
        std::bind(&DGSDKRos2Bridge::setBootModeCallback, this,
                  std::placeholders::_1, std::placeholders::_2));

    eeprom_write_service_ = this->create_service<dg_msgs::srv::EEPROMWrite>(
        "/dg/eeprom_write",
        std::bind(&DGSDKRos2Bridge::eepromWriteCallback, this,
                  std::placeholders::_1, std::placeholders::_2));

    system_diagnosis_service_ =
        this->create_service<dg_msgs::srv::SystemDiagnosis>(
            "/dg/system_diagnosis",
            std::bind(&DGSDKRos2Bridge::systemDiagnosisCallback, this,
                      std::placeholders::_1, std::placeholders::_2));

    backup_recipe_data_service_ =
        this->create_service<dg_msgs::srv::BackupRecipeData>(
            "/dg/backup_recipe_data",
            std::bind(&DGSDKRos2Bridge::backupRecipeDataCallback, this,
                      std::placeholders::_1, std::placeholders::_2));

    restore_recipe_data_service_ =
        this->create_service<dg_msgs::srv::RestoreRecipeData>(
            "/dg/restore_recipe_data",
            std::bind(&DGSDKRos2Bridge::restoreRecipeDataCallback, this,
                      std::placeholders::_1, std::placeholders::_2));

    /// 요 까징지
    // Motion Setting Services
    set_grasp_data_service_ = this->create_service<dg_msgs::srv::SetGraspData>(
        "/dg/set_grasp_data",
        std::bind(&DGSDKRos2Bridge::setGraspDataCallback, this,
                  std::placeholders::_1, std::placeholders::_2));

    set_grasp_force_service_ =
        this->create_service<dg_msgs::srv::SetGraspForce>(
            "/dg/set_grasp_force",
            std::bind(&DGSDKRos2Bridge::setGraspForceCallback, this,
                      std::placeholders::_1, std::placeholders::_2));

    start_grasp_motion_service_ =
        this->create_service<dg_msgs::srv::StartGraspMotion>(
            "/dg/start_grasp_motion",
            std::bind(&DGSDKRos2Bridge::startGraspMotionCallback, this,
                      std::placeholders::_1, std::placeholders::_2));

    move_joint_service_ = this->create_service<dg_msgs::srv::MoveJoint>(
        "/dg/move_joint",
        std::bind(&DGSDKRos2Bridge::moveJointCallback, this,
                  std::placeholders::_1, std::placeholders::_2));

    move_joint_all_service_ = this->create_service<dg_msgs::srv::MoveJointAll>(
        "/dg/move_joint_all",
        std::bind(&DGSDKRos2Bridge::moveJointAllCallback, this,
                  std::placeholders::_1, std::placeholders::_2));

    // Getter Services
    get_received_gripper_data_service_ =
        this->create_service<dg_msgs::srv::GetReceivedGripperData>(
            "/dg/get_received_gripper_data",
            std::bind(&DGSDKRos2Bridge::getReceivedGripperDataCallback, this,
                      std::placeholders::_1, std::placeholders::_2));

    // Joint Gain Functions
    set_joint_gain_p_service_ =
        this->create_service<dg_msgs::srv::SetJointGainP>(
            "/dg/set_joint_gain_p",
            std::bind(&DGSDKRos2Bridge::setJointGainPCallback, this,
                      std::placeholders::_1, std::placeholders::_2));

    set_joint_gain_p_finger_service_ =
        this->create_service<dg_msgs::srv::SetJointGainPFinger>(
            "/dg/set_joint_gain_p_finger",
            std::bind(&DGSDKRos2Bridge::setJointGainPFingerCallback, this,
                      std::placeholders::_1, std::placeholders::_2));

    set_joint_gain_p_all_service_ =
        this->create_service<dg_msgs::srv::SetJointGainPAll>(
            "/dg/set_joint_gain_p_all",
            std::bind(&DGSDKRos2Bridge::setJointGainPAllCallback, this,
                      std::placeholders::_1, std::placeholders::_2));

    set_joint_gain_d_service_ =
        this->create_service<dg_msgs::srv::SetJointGainD>(
            "/dg/set_joint_gain_d",
            std::bind(&DGSDKRos2Bridge::setJointGainDCallback, this,
                      std::placeholders::_1, std::placeholders::_2));

    set_joint_gain_d_finger_service_ =
        this->create_service<dg_msgs::srv::SetJointGainDFinger>(
            "/dg/set_joint_gain_d_finger",
            std::bind(&DGSDKRos2Bridge::setJointGainDFingerCallback, this,
                      std::placeholders::_1, std::placeholders::_2));

    set_joint_gain_d_all_service_ =
        this->create_service<dg_msgs::srv::SetJointGainDAll>(
            "/dg/set_joint_gain_d_all",
            std::bind(&DGSDKRos2Bridge::setJointGainDAllCallback, this,
                      std::placeholders::_1, std::placeholders::_2));

    set_joint_gain_i_service_ =
        this->create_service<dg_msgs::srv::SetJointGainI>(
            "/dg/set_joint_gain_i",
            std::bind(&DGSDKRos2Bridge::setJointGainICallback, this,
                      std::placeholders::_1, std::placeholders::_2));

    set_joint_gain_i_finger_service_ =
        this->create_service<dg_msgs::srv::SetJointGainIFinger>(
            "/dg/set_joint_gain_i_finger",
            std::bind(&DGSDKRos2Bridge::setJointGainIFingerCallback, this,
                      std::placeholders::_1, std::placeholders::_2));

    set_joint_gain_i_all_service_ =
        this->create_service<dg_msgs::srv::SetJointGainIAll>(
            "/dg/set_joint_gain_i_all",
            std::bind(&DGSDKRos2Bridge::setJointGainIAllCallback, this,
                      std::placeholders::_1, std::placeholders::_2));

    set_control_pid_mode_service_ =
        this->create_service<dg_msgs::srv::SetControlPIDMode>(
            "/dg/set_control_pid_mode",
            std::bind(&DGSDKRos2Bridge::setControlPIDModeCallback, this,
                      std::placeholders::_1, std::placeholders::_2));

    set_joint_gain_pid_service_ =
        this->create_service<dg_msgs::srv::SetJointGainPID>(
            "/dg/set_joint_gain_pid",
            std::bind(&DGSDKRos2Bridge::setJointGainPIDCallback, this,
                      std::placeholders::_1, std::placeholders::_2));

    set_joint_gain_pid_finger_service_ =
        this->create_service<dg_msgs::srv::SetJointGainPIDFinger>(
            "/dg/set_joint_gain_pid_finger",
            std::bind(&DGSDKRos2Bridge::setJointGainPIDFingerCallback, this,
                      std::placeholders::_1, std::placeholders::_2));

    set_joint_gain_pid_all_service_ =
        this->create_service<dg_msgs::srv::SetJointGainPIDAll>(
            "/dg/set_joint_gain_pid_all",
            std::bind(&DGSDKRos2Bridge::setJointGainPIDAllCallback, this,
                      std::placeholders::_1, std::placeholders::_2));

    set_joint_gain_pid_all_equal_service_ =
        this->create_service<dg_msgs::srv::SetJointGainPIDAllEqual>(
            "/dg/set_joint_gain_pid_all_equal",
            std::bind(&DGSDKRos2Bridge::setJointGainPIDAllEqualCallback, this,
                      std::placeholders::_1, std::placeholders::_2));

    // Motion Time Functions
    set_motion_time_joint_service_ =
        this->create_service<dg_msgs::srv::SetMotionTimeJoint>(
            "/dg/set_motion_time_joint",
            std::bind(&DGSDKRos2Bridge::setMotionTimeJointCallback, this,
                      std::placeholders::_1, std::placeholders::_2));

    set_motion_time_finger_service_ =
        this->create_service<dg_msgs::srv::SetMotionTimeFinger>(
            "/dg/set_motion_time_finger",
            std::bind(&DGSDKRos2Bridge::setMotionTimeFingerCallback, this,
                      std::placeholders::_1, std::placeholders::_2));

    set_motion_time_all_service_ =
        this->create_service<dg_msgs::srv::SetMotionTimeAll>(
            "/dg/set_motion_time_all",
            std::bind(&DGSDKRos2Bridge::setMotionTimeAllCallback, this,
                      std::placeholders::_1, std::placeholders::_2));

    set_motion_time_all_equal_service_ =
        this->create_service<dg_msgs::srv::SetMotionTimeAllEqual>(
            "/dg/set_motion_time_all_equal",
            std::bind(&DGSDKRos2Bridge::setMotionTimeAllEqualCallback, this,
                      std::placeholders::_1, std::placeholders::_2));

    // Position Mode Functions
    set_position_mode_joint_service_ =
        this->create_service<dg_msgs::srv::SetPositionModeJoint>(
            "/dg/set_position_mode_joint",
            std::bind(&DGSDKRos2Bridge::setPositionModeJointCallback, this,
                      std::placeholders::_1, std::placeholders::_2));

    set_position_mode_finger_service_ =
        this->create_service<dg_msgs::srv::SetPositionModeFinger>(
            "/dg/set_position_mode_finger",
            std::bind(&DGSDKRos2Bridge::setPositionModeFingerCallback, this,
                      std::placeholders::_1, std::placeholders::_2));

    set_position_mode_all_service_ =
        this->create_service<dg_msgs::srv::SetPositionModeAll>(
            "/dg/set_position_mode_all",
            std::bind(&DGSDKRos2Bridge::setPositionModeAllCallback, this,
                      std::placeholders::_1, std::placeholders::_2));

    set_current_control_mode_service_ =
        this->create_service<dg_msgs::srv::SetCurrentControlMode>(
            "/dg/set_current_control_mode",
            std::bind(&DGSDKRos2Bridge::setCurrentControlModeCallback, this,
                      std::placeholders::_1, std::placeholders::_2));

    set_target_current_joint_service_ =
        this->create_service<dg_msgs::srv::SetTargetCurrentJoint>(
            "/dg/set_target_current_joint",
            std::bind(&DGSDKRos2Bridge::setTargetCurrentJointCallback, this,
                      std::placeholders::_1, std::placeholders::_2));

    set_target_current_finger_service_ =
        this->create_service<dg_msgs::srv::SetTargetCurrentFinger>(
            "/dg/set_target_current_finger",
            std::bind(&DGSDKRos2Bridge::setTargetCurrentFingerCallback, this,
                      std::placeholders::_1, std::placeholders::_2));

    set_target_current_all_service_ =
        this->create_service<dg_msgs::srv::SetTargetCurrentAll>(
            "/dg/set_target_current_all",
            std::bind(&DGSDKRos2Bridge::setTargetCurrentAllCallback, this,
                      std::placeholders::_1, std::placeholders::_2));

    // Recipe Functions - TODO: Implement these callbacks
    update_recipe_current_pose_data_service_ =
        this->create_service<dg_msgs::srv::UpdateRecipeCurrentPoseData>(
            "/dg/update_recipe_current_pose_data",
            std::bind(&DGSDKRos2Bridge::updateRecipeCurrentPoseDataCallback,
                      this, std::placeholders::_1, std::placeholders::_2));

    update_recipe_target_pose_data_service_ =
        this->create_service<dg_msgs::srv::UpdateRecipeTargetPoseData>(
            "/dg/update_recipe_target_pose_data",
            std::bind(&DGSDKRos2Bridge::updateRecipeTargetPoseDataCallback,
                      this, std::placeholders::_1, std::placeholders::_2));

    load_recipe_pose_data_service_ =
        this->create_service<dg_msgs::srv::LoadRecipePoseData>(
            "/dg/load_recipe_pose_data",
            std::bind(&DGSDKRos2Bridge::loadRecipePoseDataCallback, this,
                      std::placeholders::_1, std::placeholders::_2));

    update_recipe_gain_data_service_ =
        this->create_service<dg_msgs::srv::UpdateRecipeGainData>(
            "/dg/update_recipe_gain_data",
            std::bind(&DGSDKRos2Bridge::updateRecipeGainDataCallback, this,
                      std::placeholders::_1, std::placeholders::_2));

    load_recipe_gain_data_service_ =
        this->create_service<dg_msgs::srv::LoadRecipeGainData>(
            "/dg/load_recipe_gain_data",
            std::bind(&DGSDKRos2Bridge::loadRecipeGainDataCallback, this,
                      std::placeholders::_1, std::placeholders::_2));

    update_recipe_grasp_data_service_ =
        this->create_service<dg_msgs::srv::UpdateRecipeGraspData>(
            "/dg/update_recipe_grasp_data",
            std::bind(&DGSDKRos2Bridge::updateRecipeGraspDataCallback, this,
                      std::placeholders::_1, std::placeholders::_2));

    load_recipe_grasp_data_service_ =
        this->create_service<dg_msgs::srv::LoadRecipeGraspData>(
            "/dg/load_recipe_grasp_data",
            std::bind(&DGSDKRos2Bridge::loadRecipeGraspDataCallback, this,
                      std::placeholders::_1, std::placeholders::_2));

    // Motion Functions - TODO: Implement these callbacks
    manual_teach_mode_service_ =
        this->create_service<dg_msgs::srv::ManualTeachMode>(
            "/dg/manual_teach_mode",
            std::bind(&DGSDKRos2Bridge::manualTeachModeCallback, this,
                      std::placeholders::_1, std::placeholders::_2));

    move_joint_finger_service_ =
        this->create_service<dg_msgs::srv::MoveJointFinger>(
            "/dg/move_joint_finger",
            std::bind(&DGSDKRos2Bridge::moveJointFingerCallback, this,
                      std::placeholders::_1, std::placeholders::_2));

    move_servo_joint_service_ =
        this->create_service<dg_msgs::srv::MoveServoJoint>(
            "/dg/move_servo_joint",
            std::bind(&DGSDKRos2Bridge::moveServoJointCallback, this,
                      std::placeholders::_1, std::placeholders::_2));

    // Getter Functions - TODO: Implement these callbacks (keeping these
    // commented for now)
    /*
    get_communication_period_service_ =
    this->create_service<dg_msgs::srv::GetCommunicationPeriod>(
        "/dg/get_communication_period",
        std::bind(&DGSDKRos2Bridge::getCommunicationPeriodCallback, this,
                  std::placeholders::_1, std::placeholders::_2));

    get_received_fingertip_sensor_data_service_ =
    this->create_service<dg_msgs::srv::GetReceivedFingertipSensorData>(
        "/dg/get_received_fingertip_sensor_data",
        std::bind(&DGSDKRos2Bridge::getReceivedFingertipSensorDataCallback,
    this, std::placeholders::_1, std::placeholders::_2));

    get_received_gpio_data_service_ =
    this->create_service<dg_msgs::srv::GetReceivedGPIOData>(
        "/dg/get_received_gpio_data",
        std::bind(&DGSDKRos2Bridge::getReceivedGPIODataCallback, this,
                  std::placeholders::_1, std::placeholders::_2));

    get_data_processing_service_ =
    this->create_service<dg_msgs::srv::GetDataProcessing>(
        "/dg/get_data_processing",
        std::bind(&DGSDKRos2Bridge::getDataProcessingCallback, this,
                  std::placeholders::_1, std::placeholders::_2));

    get_diagnosis_system_service_ =
    this->create_service<dg_msgs::srv::GetDiagnosisSystem>(
        "/dg/get_diagnosis_system",
        std::bind(&DGSDKRos2Bridge::getDiagnosisSystemCallback, this,
                  std::placeholders::_1, std::placeholders::_2));
    */
  }

  // Callback implementations
  void setGripperSystemCallback(
      const std::shared_ptr<dg_msgs::srv::SetGripperSystem::Request> request,
      std::shared_ptr<dg_msgs::srv::SetGripperSystem::Response> response) {
    GripperSystemSetting setting;

    // strncpy(setting.comport, request->setting.comport.c_str(),
    //         MAX_COMPORT_NAME_SIZE - 1);

    // strncpy(setting.ip, request->setting.ip.c_str(),
    //         MAX_GRIPPER_IP_ADDRESS_SIZE - 1);
    RCLCPP_INFO(
        this->get_logger(), "Setting Gripper System with IP: %s, Comport: %s",
        request->setting.ip.c_str(), request->setting.comport.c_str());
    memcpy(setting.ip, request->setting.ip.c_str(),
           MAX_GRIPPER_IP_ADDRESS_SIZE);
    memcpy(setting.comport, request->setting.comport.c_str(),
           MAX_COMPORT_NAME_SIZE);

    setting.port = request->setting.port;
    setting.readTimeout = request->setting.read_timeout;
    setting.controlMode = request->setting.control_mode;
    setting.communicationMode = request->setting.communication_mode;
    setting.slaveID = request->setting.slave_id;
    setting.baudrate = request->setting.baudrate;

    DG_RESULT result = SetGripperSystem(setting);
RCLCPP_INFO(this->get_logger(), "Gripper Option Set Result: %d", result);
    InitializedCallback();
    response->result = static_cast<int>(result);
  }

  void setGripperOptionCallback(
      const std::shared_ptr<dg_msgs::srv::SetGripperOption::Request> request,
      std::shared_ptr<dg_msgs::srv::SetGripperOption::Response> response) {
    GripperSetting setting;

    std::copy(request->setting.joint_offset.begin(),
              request->setting.joint_offset.end(), setting.jointOffset);

    std::copy(request->setting.joint_inpose.begin(),
              request->setting.joint_inpose.end(), setting.jointInpose);

    std::copy(request->setting.tcp_inpose.begin(),
              request->setting.tcp_inpose.end(), setting.tcpInpose);

    std::copy(request->setting.orientation_inpose.begin(),
              request->setting.orientation_inpose.end(),
              setting.orientationInpose);

    std::copy(request->setting.received_data_type.begin(),
              request->setting.received_data_type.end(),
              setting.receivedDataType);

    setting.movingInpose = request->setting.moving_inpose;
    setting.jointCount = request->setting.joint_count;
    setting.fingerCount = request->setting.finger_count;
    setting.model = static_cast<DG_MODEL>(request->setting.model);

    DG_RESULT result = SetGripperOption(setting);
    

    response->result = static_cast<int>(result);
  }

  void connectToGripperCallback(
      const std::shared_ptr<dg_msgs::srv::ConnectToGripper::Request>,
      std::shared_ptr<dg_msgs::srv::ConnectToGripper::Response> response) {
    DG_RESULT result = ConnectToGripper();
    response->result = static_cast<int>(result);
  }

  void disconnectToGripperCallback(
      const std::shared_ptr<dg_msgs::srv::DisconnectToGripper::Request>,
      std::shared_ptr<dg_msgs::srv::DisconnectToGripper::Response> response) {
    DG_RESULT result = DisconnectToGripper();
    response->result = static_cast<int>(result);
  }

  void systemStartCallback(
      const std::shared_ptr<dg_msgs::srv::SystemStart::Request>,
      std::shared_ptr<dg_msgs::srv::SystemStart::Response> response) {
    DG_RESULT result = SystemStart();
    response->result = static_cast<int>(result);
  }

  void systemStopCallback(
      const std::shared_ptr<dg_msgs::srv::SystemStop::Request>,
      std::shared_ptr<dg_msgs::srv::SystemStop::Response> response) {
    DG_RESULT result = SystemStop();
    response->result = static_cast<int>(result);
  }

  void setIpCallback(
      const std::shared_ptr<dg_msgs::srv::SetIp::Request> request,
      std::shared_ptr<dg_msgs::srv::SetIp::Response> response) {
    int ip[4];
    for (int i = 0; i < 4 && i < static_cast<int>(request->ip.size()); i++) {
      ip[i] = request->ip[i];
    }
    DG_RESULT result = SetIp(ip, request->port);

    response->result = static_cast<int>(result);
  }

  // other system services

  void setLowPassFilterAlphaCallback(
      const std::shared_ptr<dg_msgs::srv::SetLowPassFilterAlpha::Request>
          request,
      std::shared_ptr<dg_msgs::srv::SetLowPassFilterAlpha::Response> response) {
    DG_RESULT result = SetLowPassFilterAlpha(request->is_used, request->alpha);
    response->result = static_cast<int>(result);
  }

  void setBootModeCallback(
      const std::shared_ptr<dg_msgs::srv::SetBootMode::Request> request,
      std::shared_ptr<dg_msgs::srv::SetBootMode::Response> response) {
    DG_RESULT result =
        SetBootMode(const_cast<char*>(request->password.c_str()));
    response->result = static_cast<int>(result);
  }

  void setGPIOOutputCallback(
      const std::shared_ptr<dg_msgs::srv::SetGPIOOutput::Request> request,
      std::shared_ptr<dg_msgs::srv::SetGPIOOutput::Response> response) {
    DG_RESULT result = SetGPIOOuput(request->gpio, request->output_number);
    response->result = static_cast<int>(result);
  }

  void setGPIOOutputAllCallback(
      const std::shared_ptr<dg_msgs::srv::SetGPIOOutputAll::Request> request,
      std::shared_ptr<dg_msgs::srv::SetGPIOOutputAll::Response> response) {
    int gpio[MAX_GRIPPER_GPIO_SIZE];

    for (int i = 0; i < MAX_GRIPPER_GPIO_SIZE &&
                    i < static_cast<int>(request->output.size());
         i++) {
      gpio[i] = request->output[i];
    }
    DG_RESULT result = SetGPIOOuputAll(gpio);
    response->result = static_cast<int>(result);
  }

  void setTorqueLimitModeCallback(
      const std::shared_ptr<dg_msgs::srv::SetTorqueLimitMode::Request> request,
      std::shared_ptr<dg_msgs::srv::SetTorqueLimitMode::Response> response) {
    DG_RESULT result = SetTorqueLimitMode(request->is_on);
    response->result = static_cast<int>(result);
  }

  void eepromWriteCallback(
      const std::shared_ptr<dg_msgs::srv::EEPROMWrite::Request> /* request */,
      std::shared_ptr<dg_msgs::srv::EEPROMWrite::Response> response) {
    DG_RESULT result = EEPROMWrite();
    response->result = static_cast<int>(result);
  }

  void systemDiagnosisCallback(
      const std::shared_ptr<
          dg_msgs::srv::SystemDiagnosis::Request> /* request */,
      std::shared_ptr<dg_msgs::srv::SystemDiagnosis::Response> response) {
    DG_RESULT result = SystemDiagnosis();
    response->result = static_cast<int>(result);
  }

  void backupRecipeDataCallback(
      const std::shared_ptr<dg_msgs::srv::BackupRecipeData::Request> request,
      std::shared_ptr<dg_msgs::srv::BackupRecipeData::Response> response) {
    DG_RESULT result =
        BackupRecipeData(const_cast<char*>(request->path.c_str()));
    response->result = static_cast<int>(result);
  }

  void restoreRecipeDataCallback(
      const std::shared_ptr<dg_msgs::srv::RestoreRecipeData::Request> request,
      std::shared_ptr<dg_msgs::srv::RestoreRecipeData::Response> response) {
    DG_RESULT result =
        RestoreRecipeData(const_cast<char*>(request->path.c_str()));
    response->result = static_cast<int>(result);
  }

  /// Motion setting

  void setGraspDataCallback(
      const std::shared_ptr<dg_msgs::srv::SetGraspData::Request> request,
      std::shared_ptr<dg_msgs::srv::SetGraspData::Response> response) {
    DG_RESULT result = SetGraspData(
        static_cast<DG_GRASP_MODE>(request->grasp_mode), request->grasp_force,
        request->grasp_option, request->smooth_grasping);
    response->result = static_cast<int>(result);
  }

  void setGraspForceCallback(
      const std::shared_ptr<dg_msgs::srv::SetGraspForce::Request> request,
      std::shared_ptr<dg_msgs::srv::SetGraspForce::Response> response) {
    DG_RESULT result = SetGraspForce(request->grasp_force);
    response->result = static_cast<int>(result);
  }

  void startGraspMotionCallback(
      const std::shared_ptr<dg_msgs::srv::StartGraspMotion::Request> request,
      std::shared_ptr<dg_msgs::srv::StartGraspMotion::Response> response) {
    DG_RESULT result = StartGraspMotion(request->is_grasp);
    response->result = static_cast<int>(result);
  }

  void moveJointCallback(
      const std::shared_ptr<dg_msgs::srv::MoveJoint::Request> request,
      std::shared_ptr<dg_msgs::srv::MoveJoint::Response> response) {
    DG_RESULT result = MoveJoint(request->target_joint, request->joint_number);
    response->result = static_cast<int>(result);
  }

  void moveJointAllCallback(
      const std::shared_ptr<dg_msgs::srv::MoveJointAll::Request> request,
      std::shared_ptr<dg_msgs::srv::MoveJointAll::Response> response) {
    float targetJoints[MAX_JOINT_COUNT];
    for (int i = 0; i < MAX_JOINT_COUNT; i++) {
      if (i < static_cast<int>(request->target_joint.size())) {
        targetJoints[i] = request->target_joint[i];
      } else {
        targetJoints[i] = 0.0f;
      }
    }

    DG_RESULT result = MoveJointAll(targetJoints);
    response->result = static_cast<int>(result);
  }

  void getReceivedGripperDataCallback(
      const std::shared_ptr<
          dg_msgs::srv::GetReceivedGripperData::Request> /* request */,
      std::shared_ptr<dg_msgs::srv::GetReceivedGripperData::Response>
          response) {
    ReceivedGripperData data;
    DG_RESULT result = GetReceivedGripperData(&data);

    if (result == DG_RESULT_NONE) {
      std::copy(data.joint, data.joint + MAX_JOINT_COUNT,
                response->gripper_data.joint.begin());
      std::copy(data.current, data.current + MAX_JOINT_COUNT,
                response->gripper_data.current.begin());
      std::copy(data.velocity, data.velocity + MAX_JOINT_COUNT,
                response->gripper_data.velocity.begin());
      std::copy(data.temperature, data.temperature + MAX_JOINT_COUNT,
                response->gripper_data.temperature.begin());
      std::copy(data.TCP, data.TCP + 6 * MAX_FINGER_COUNT,
                response->gripper_data.tcp.begin());
      response->gripper_data.moving = data.moving;
      response->gripper_data.target_arrived = data.targetArrived;
      response->gripper_data.blend_move_state = data.blendMoveState;
      response->gripper_data.current_blend_index = data.currentBlendIndex;
      response->gripper_data.product_id = data.productID;
      response->gripper_data.firmware_version = data.firmwareVersion;
    }

    response->result = static_cast<int>(result);
  }

  // Joint Gain P Functions
  void setJointGainPCallback(
      const std::shared_ptr<dg_msgs::srv::SetJointGainP::Request> request,
      std::shared_ptr<dg_msgs::srv::SetJointGainP::Response> response) {
    DG_RESULT result = SetJointGainP(request->gain_p, request->joint_number);
    response->result = static_cast<int>(result);
  }

  void setJointGainPFingerCallback(
      const std::shared_ptr<dg_msgs::srv::SetJointGainPFinger::Request> request,
      std::shared_ptr<dg_msgs::srv::SetJointGainPFinger::Response> response) {
    float gainP[MAX_JOINT_COUNT];
    for (int i = 0; i < MAX_JOINT_COUNT; i++) {
      if (i < static_cast<int>(request->gain_p.size())) {
        gainP[i] = request->gain_p[i];
      } else {
        gainP[i] = 0.0f;
      }
    }
    DG_RESULT result = SetJointGainPFinger(gainP, request->finger_number);
    response->result = static_cast<int>(result);
  }

  void setJointGainPAllCallback(
      const std::shared_ptr<dg_msgs::srv::SetJointGainPAll::Request> request,
      std::shared_ptr<dg_msgs::srv::SetJointGainPAll::Response> response) {
    float gainP[MAX_JOINT_COUNT];
    for (int i = 0; i < MAX_JOINT_COUNT; i++) {
      if (i < static_cast<int>(request->gain_p.size())) {
        gainP[i] = request->gain_p[i];
      } else {
        gainP[i] = 0.0f;
      }
    }
    DG_RESULT result = SetJointGainPAll(gainP);
    response->result = static_cast<int>(result);
  }

  // Joint Gain D Functions
  void setJointGainDCallback(
      const std::shared_ptr<dg_msgs::srv::SetJointGainD::Request> request,
      std::shared_ptr<dg_msgs::srv::SetJointGainD::Response> response) {
    DG_RESULT result = SetJointGainD(request->gain_d, request->joint_number);
    response->result = static_cast<int>(result);
  }

  void setJointGainDFingerCallback(
      const std::shared_ptr<dg_msgs::srv::SetJointGainDFinger::Request> request,
      std::shared_ptr<dg_msgs::srv::SetJointGainDFinger::Response> response) {
    float gainD[MAX_JOINT_COUNT];
    for (int i = 0; i < MAX_JOINT_COUNT; i++) {
      if (i < static_cast<int>(request->gain_d.size())) {
        gainD[i] = request->gain_d[i];
      } else {
        gainD[i] = 0.0f;
      }
    }
    DG_RESULT result = SetJointGainDFinger(gainD, request->finger_number);
    response->result = static_cast<int>(result);
  }

  void setJointGainDAllCallback(
      const std::shared_ptr<dg_msgs::srv::SetJointGainDAll::Request> request,
      std::shared_ptr<dg_msgs::srv::SetJointGainDAll::Response> response) {
    float gainD[MAX_JOINT_COUNT];
    for (int i = 0; i < MAX_JOINT_COUNT; i++) {
      if (i < static_cast<int>(request->gain_d.size())) {
        gainD[i] = request->gain_d[i];
      } else {
        gainD[i] = 0.0f;
      }
    }
    DG_RESULT result = SetJointGainDAll(gainD);
    response->result = static_cast<int>(result);
  }

  // Joint Gain I Functions
  void setJointGainICallback(
      const std::shared_ptr<dg_msgs::srv::SetJointGainI::Request> request,
      std::shared_ptr<dg_msgs::srv::SetJointGainI::Response> response) {
    DG_RESULT result =
        SetJointGainI(request->gain_i, request->i_limit, request->joint_number);
    response->result = static_cast<int>(result);
  }

  void setJointGainIFingerCallback(
      const std::shared_ptr<dg_msgs::srv::SetJointGainIFinger::Request> request,
      std::shared_ptr<dg_msgs::srv::SetJointGainIFinger::Response> response) {
    float gainI[MAX_JOINT_COUNT];
    float iLimit[MAX_JOINT_COUNT];
    for (int i = 0; i < MAX_JOINT_COUNT; i++) {
      if (i < static_cast<int>(request->gain_i.size())) {
        gainI[i] = request->gain_i[i];
      } else {
        gainI[i] = 0.0f;
      }
      if (i < static_cast<int>(request->i_limit.size())) {
        iLimit[i] = request->i_limit[i];
      } else {
        iLimit[i] = 0.0f;
      }
    }
    DG_RESULT result =
        SetJointGainIFinger(gainI, iLimit, request->finger_number);
    response->result = static_cast<int>(result);
  }

  void setJointGainIAllCallback(
      const std::shared_ptr<dg_msgs::srv::SetJointGainIAll::Request> request,
      std::shared_ptr<dg_msgs::srv::SetJointGainIAll::Response> response) {
    float gainI[MAX_JOINT_COUNT];
    float iLimit[MAX_JOINT_COUNT];
    for (int i = 0; i < MAX_JOINT_COUNT; i++) {
      if (i < static_cast<int>(request->gain_i.size())) {
        gainI[i] = request->gain_i[i];
      } else {
        gainI[i] = 0.0f;
      }
      if (i < static_cast<int>(request->i_limit.size())) {
        iLimit[i] = request->i_limit[i];
      } else {
        iLimit[i] = 0.0f;
      }
    }
    DG_RESULT result = SetJointGainIAll(gainI, iLimit);
    response->result = static_cast<int>(result);
  }

  void setControlPIDModeCallback(
      const std::shared_ptr<dg_msgs::srv::SetControlPIDMode::Request> request,
      std::shared_ptr<dg_msgs::srv::SetControlPIDMode::Response> response) {
    DG_RESULT result = SetControlPIDMode(request->mode);
    response->result = static_cast<int>(result);
  }

  void setJointGainPIDCallback(
      const std::shared_ptr<dg_msgs::srv::SetJointGainPID::Request> request,
      std::shared_ptr<dg_msgs::srv::SetJointGainPID::Response> response) {
    DG_RESULT result =
        SetJointGainPID(request->gain_p, request->gain_d, request->gain_i,
                        request->i_limit, request->joint_number);
    response->result = static_cast<int>(result);
  }

  void setJointGainPIDFingerCallback(
      const std::shared_ptr<dg_msgs::srv::SetJointGainPIDFinger::Request>
          request,
      std::shared_ptr<dg_msgs::srv::SetJointGainPIDFinger::Response> response) {
    float gainP[MAX_JOINT_COUNT];
    float gainD[MAX_JOINT_COUNT];
    float gainI[MAX_JOINT_COUNT];
    float iLimit[MAX_JOINT_COUNT];

    for (int i = 0; i < MAX_JOINT_COUNT; i++) {
      gainP[i] = i < static_cast<int>(request->gain_p.size())
                     ? request->gain_p[i]
                     : 0.0f;
      gainD[i] = i < static_cast<int>(request->gain_d.size())
                     ? request->gain_d[i]
                     : 0.0f;
      gainI[i] = i < static_cast<int>(request->gain_i.size())
                     ? request->gain_i[i]
                     : 0.0f;
      iLimit[i] = i < static_cast<int>(request->i_limit.size())
                      ? request->i_limit[i]
                      : 0.0f;
    }

    DG_RESULT result = SetJointGainPIDFinger(gainP, gainD, gainI, iLimit,
                                             request->finger_number);
    response->result = static_cast<int>(result);
  }

  void setJointGainPIDAllCallback(
      const std::shared_ptr<dg_msgs::srv::SetJointGainPIDAll::Request> request,
      std::shared_ptr<dg_msgs::srv::SetJointGainPIDAll::Response> response) {
    float gainP[MAX_JOINT_COUNT];
    float gainD[MAX_JOINT_COUNT];
    float gainI[MAX_JOINT_COUNT];
    float iLimit[MAX_JOINT_COUNT];

    for (int i = 0; i < MAX_JOINT_COUNT; i++) {
      gainP[i] = i < static_cast<int>(request->gain_p.size())
                     ? request->gain_p[i]
                     : 0.0f;
      gainD[i] = i < static_cast<int>(request->gain_d.size())
                     ? request->gain_d[i]
                     : 0.0f;
      gainI[i] = i < static_cast<int>(request->gain_i.size())
                     ? request->gain_i[i]
                     : 0.0f;
      iLimit[i] = i < static_cast<int>(request->i_limit.size())
                      ? request->i_limit[i]
                      : 0.0f;
    }

    DG_RESULT result = SetJointGainPIDAll(gainP, gainD, gainI, iLimit);
    response->result = static_cast<int>(result);
  }

  void setJointGainPIDAllEqualCallback(
      const std::shared_ptr<dg_msgs::srv::SetJointGainPIDAllEqual::Request>
          request,
      std::shared_ptr<dg_msgs::srv::SetJointGainPIDAllEqual::Response>
          response) {
    DG_RESULT result = SetJointGainPIDAllEqual(
        request->gain_p, request->gain_d, request->gain_i, request->i_limit);
    response->result = static_cast<int>(result);
  }

  // Motion Time Functions
  void setMotionTimeJointCallback(
      const std::shared_ptr<dg_msgs::srv::SetMotionTimeJoint::Request> request,
      std::shared_ptr<dg_msgs::srv::SetMotionTimeJoint::Response> response) {
    DG_RESULT result =
        SetMotionTimeJoint(request->motion_time, request->joint_number);
    response->result = static_cast<int>(result);
  }

  void setMotionTimeFingerCallback(
      const std::shared_ptr<dg_msgs::srv::SetMotionTimeFinger::Request> request,
      std::shared_ptr<dg_msgs::srv::SetMotionTimeFinger::Response> response) {
    int motionTime[MAX_JOINT_COUNT];
    for (int i = 0; i < MAX_JOINT_COUNT; i++) {
      if (i < static_cast<int>(request->motion_time.size())) {
        motionTime[i] = request->motion_time[i];
      } else {
        motionTime[i] = 0;
      }
    }
    DG_RESULT result = SetMotionTimeFinger(motionTime, request->finger_number);
    response->result = static_cast<int>(result);
  }

  void setMotionTimeAllCallback(
      const std::shared_ptr<dg_msgs::srv::SetMotionTimeAll::Request> request,
      std::shared_ptr<dg_msgs::srv::SetMotionTimeAll::Response> response) {
    int motionTime[MAX_JOINT_COUNT];
    for (int i = 0; i < MAX_JOINT_COUNT; i++) {
      if (i < static_cast<int>(request->motion_time.size())) {
        motionTime[i] = request->motion_time[i];
      } else {
        motionTime[i] = 0;
      }
    }
    DG_RESULT result = SetMotionTimeAll(motionTime);
    response->result = static_cast<int>(result);
  }

  void setMotionTimeAllEqualCallback(
      const std::shared_ptr<dg_msgs::srv::SetMotionTimeAllEqual::Request>
          request,
      std::shared_ptr<dg_msgs::srv::SetMotionTimeAllEqual::Response> response) {
    DG_RESULT result = SetMotionTimeAllEqual(request->motion_time);
    response->result = static_cast<int>(result);
  }

  // Position Mode Functions
  void setPositionModeJointCallback(
      const std::shared_ptr<dg_msgs::srv::SetPositionModeJoint::Request>
          request,
      std::shared_ptr<dg_msgs::srv::SetPositionModeJoint::Response> response) {
    DG_RESULT result =
        SetPositionModeJoint(request->position_mode, request->joint_number);
    response->result = static_cast<int>(result);
  }

  void setPositionModeFingerCallback(
      const std::shared_ptr<dg_msgs::srv::SetPositionModeFinger::Request>
          request,
      std::shared_ptr<dg_msgs::srv::SetPositionModeFinger::Response> response) {
    int positionMode[MAX_JOINT_COUNT];
    for (int i = 0; i < MAX_JOINT_COUNT; i++) {
      if (i < static_cast<int>(request->position_mode.size())) {
        positionMode[i] = request->position_mode[i];
      } else {
        positionMode[i] = 0;
      }
    }
    DG_RESULT result =
        SetPositionModeFinger(positionMode, request->finger_number);
    response->result = static_cast<int>(result);
  }

  void setPositionModeAllCallback(
      const std::shared_ptr<dg_msgs::srv::SetPositionModeAll::Request> request,
      std::shared_ptr<dg_msgs::srv::SetPositionModeAll::Response> response) {
    int positionMode[MAX_JOINT_COUNT];
    for (int i = 0; i < MAX_JOINT_COUNT; i++) {
      if (i < static_cast<int>(request->position_mode.size())) {
        positionMode[i] = request->position_mode[i];
      } else {
        positionMode[i] = 0;
      }
    }
    DG_RESULT result = SetPositionModeAll(positionMode);
    response->result = static_cast<int>(result);
  }

  // Current Control Functions
  void setCurrentControlModeCallback(
      const std::shared_ptr<dg_msgs::srv::SetCurrentControlMode::Request>
          request,
      std::shared_ptr<dg_msgs::srv::SetCurrentControlMode::Response> response) {
    DG_RESULT result = SetCurrentControlMode(request->is_on);
    response->result = static_cast<int>(result);
  }

  void setTargetCurrentJointCallback(
      const std::shared_ptr<dg_msgs::srv::SetTargetCurrentJoint::Request>
          request,
      std::shared_ptr<dg_msgs::srv::SetTargetCurrentJoint::Response> response) {
    DG_RESULT result =
        SetTargetCurrentJoint(request->target_current, request->joint_number);
    response->result = static_cast<int>(result);
  }

  void setTargetCurrentFingerCallback(
      const std::shared_ptr<dg_msgs::srv::SetTargetCurrentFinger::Request>
          request,
      std::shared_ptr<dg_msgs::srv::SetTargetCurrentFinger::Response>
          response) {
    int targetCurrent[MAX_JOINT_COUNT];
    for (int i = 0; i < MAX_JOINT_COUNT; i++) {
      if (i < static_cast<int>(request->target_current.size())) {
        targetCurrent[i] = request->target_current[i];
      } else {
        targetCurrent[i] = 0;
      }
    }
    DG_RESULT result =
        SetTargetCurrentFinger(targetCurrent, request->finger_number);
    response->result = static_cast<int>(result);
  }

  void setTargetCurrentAllCallback(
      const std::shared_ptr<dg_msgs::srv::SetTargetCurrentAll::Request> request,
      std::shared_ptr<dg_msgs::srv::SetTargetCurrentAll::Response> response) {
    int targetCurrent[MAX_JOINT_COUNT];
    for (int i = 0; i < MAX_JOINT_COUNT; i++) {
      if (i < static_cast<int>(request->target_current.size())) {
        targetCurrent[i] = request->target_current[i];
      } else {
        targetCurrent[i] = 0;
      }
    }
    DG_RESULT result = SetTargetCurrentAll(targetCurrent);
    response->result = static_cast<int>(result);
  }

  // Recipe Functions - TODO: Implement these callbacks
  void updateRecipeCurrentPoseDataCallback(
      const std::shared_ptr<dg_msgs::srv::UpdateRecipeCurrentPoseData::Request>
          request,
      std::shared_ptr<dg_msgs::srv::UpdateRecipeCurrentPoseData::Response>
          response) {
    DG_RESULT result = UpdateRecipeCurrentPoseData(request->pose_number);
    response->result = static_cast<int>(result);
  }

  void updateRecipeTargetPoseDataCallback(
      const std::shared_ptr<dg_msgs::srv::UpdateRecipeTargetPoseData::Request>
          request,
      std::shared_ptr<dg_msgs::srv::UpdateRecipeTargetPoseData::Response>
          response) {
    // TODO: Implement this function
    // RCLCPP_WARN(this->get_logger(), "updateRecipeTargetPoseDataCallback not
    // implemented yet"); response->result = -1; // Not implemented

    DG_RESULT result = UpdateRecipeTargetPoseData(request->pose_number);
    response->result = static_cast<int>(result);
  }

  void loadRecipePoseDataCallback(
      const std::shared_ptr<dg_msgs::srv::LoadRecipePoseData::Request> request,
      std::shared_ptr<dg_msgs::srv::LoadRecipePoseData::Response> response) {
    // TODO: Implement this function
    // RCLCPP_WARN(this->get_logger(), "loadRecipePoseDataCallback not
    // implemented yet"); response->result = -1; // Not implemented

    DG_RESULT result = LoadRecipePoseData(request->pose_number);
    response->result = static_cast<int>(result);
  }

  void updateRecipeGainDataCallback(
      const std::shared_ptr<dg_msgs::srv::UpdateRecipeGainData::Request>
          request,
      std::shared_ptr<dg_msgs::srv::UpdateRecipeGainData::Response> response) {
    DG_RESULT result = UpdateRecipeGainData(request->gain_number);
    response->result = static_cast<int>(result);
  }

  void loadRecipeGainDataCallback(
      const std::shared_ptr<dg_msgs::srv::LoadRecipeGainData::Request> request,
      std::shared_ptr<dg_msgs::srv::LoadRecipeGainData::Response> response) {
    DG_RESULT result = LoadRecipeGainData(request->gain_number);
    response->result = static_cast<int>(result);
  }

  void updateRecipeGraspDataCallback(
      const std::shared_ptr<dg_msgs::srv::UpdateRecipeGraspData::Request>
          request,
      std::shared_ptr<dg_msgs::srv::UpdateRecipeGraspData::Response> response) {
    DG_RESULT result = UpdateRecipeGraspData(request->grasp_number);
    response->result = static_cast<int>(result);
  }

  void loadRecipeGraspDataCallback(
      const std::shared_ptr<dg_msgs::srv::LoadRecipeGraspData::Request> request,
      std::shared_ptr<dg_msgs::srv::LoadRecipeGraspData::Response> response) {
    DG_RESULT result = LoadRecipeGraspData(request->grasp_number);
    response->result = static_cast<int>(result);
  }

  // Motion Functions - TODO: Implement these callbacks
  void manualTeachModeCallback(
      const std::shared_ptr<dg_msgs::srv::ManualTeachMode::Request> request,
      std::shared_ptr<dg_msgs::srv::ManualTeachMode::Response> response) {
    DG_RESULT result = ManualTeachMode(request->is_on);
    response->result = static_cast<int>(result);
  }

  void moveJointFingerCallback(
      const std::shared_ptr<dg_msgs::srv::MoveJointFinger::Request> request,
      std::shared_ptr<dg_msgs::srv::MoveJointFinger::Response> response) {
    float targetJoint[MAX_FINGER_JOINT_COUNT];
    for (int i = 0; i < MAX_FINGER_JOINT_COUNT; i++) {
      if (i < static_cast<int>(request->target_joint.size())) {
        targetJoint[i] = request->target_joint[i];
      } else {
        targetJoint[i] = 0.0f;
      }
    }
    DG_RESULT result = MoveJointFinger(targetJoint, request->finger_number);
    response->result = static_cast<int>(result);
  }

  void moveServoJointCallback(
      const std::shared_ptr<dg_msgs::srv::MoveServoJoint::Request> request,
      std::shared_ptr<dg_msgs::srv::MoveServoJoint::Response> response) {
    float targetJoint[MAX_JOINT_COUNT];

    for (int i = 0; i < MAX_JOINT_COUNT; i++) {
      if (i < static_cast<int>(request->target_joint.size())) {
        targetJoint[i] = request->target_joint[i];
      } else {
        targetJoint[i] = 0.0f;
      }
    }
    DG_RESULT result = MoveServoJoint(targetJoint);

    response->result = static_cast<int>(result);
  }

 private:
  rclcpp::Publisher<std_msgs::msg::Bool>::SharedPtr connected_publisher_;
  rclcpp::Publisher<std_msgs::msg::Int32>::SharedPtr period_publisher_;

  rclcpp::Publisher<dg_msgs::msg::ReceivedGripperData>::SharedPtr
      gripper_data_publisher_;
  rclcpp::Publisher<dg_msgs::msg::ReceivedFingertipSensorData>::SharedPtr
      fingertip_sensor_publisher_;
  rclcpp::Publisher<dg_msgs::msg::ReceivedGPIOData>::SharedPtr
      gpio_data_publisher_;
  rclcpp::Publisher<dg_msgs::msg::DiagnosisSystem>::SharedPtr
      diagnosis_publisher_;

  // Service declarations
  rclcpp::Service<dg_msgs::srv::SetGripperSystem>::SharedPtr
      set_gripper_system_service_;
  rclcpp::Service<dg_msgs::srv::SetGripperOption>::SharedPtr
      set_gripper_option_service_;
  rclcpp::Service<dg_msgs::srv::ConnectToGripper>::SharedPtr
      connect_to_gripper_service_;
  rclcpp::Service<dg_msgs::srv::DisconnectToGripper>::SharedPtr
      disconnect_to_gripper_service_;
  rclcpp::Service<dg_msgs::srv::SystemStart>::SharedPtr system_start_service_;
  rclcpp::Service<dg_msgs::srv::SystemStop>::SharedPtr system_stop_service_;
  rclcpp::Service<dg_msgs::srv::SetIp>::SharedPtr set_ip_service_;
  rclcpp::Service<dg_msgs::srv::SetGraspData>::SharedPtr
      set_grasp_data_service_;
  rclcpp::Service<dg_msgs::srv::SetGraspForce>::SharedPtr
      set_grasp_force_service_;
  rclcpp::Service<dg_msgs::srv::StartGraspMotion>::SharedPtr
      start_grasp_motion_service_;
  rclcpp::Service<dg_msgs::srv::MoveJoint>::SharedPtr move_joint_service_;
  rclcpp::Service<dg_msgs::srv::MoveJointAll>::SharedPtr
      move_joint_all_service_;
  rclcpp::Service<dg_msgs::srv::GetReceivedGripperData>::SharedPtr
      get_received_gripper_data_service_;
  rclcpp::Service<dg_msgs::srv::SetLowPassFilterAlpha>::SharedPtr
      set_low_pass_filter_alpha_service_;
  rclcpp::Service<dg_msgs::srv::SetGPIOOutput>::SharedPtr
      set_gpio_output_service_;
  rclcpp::Service<dg_msgs::srv::SetGPIOOutputAll>::SharedPtr
      set_gpio_output_all_service_;
  rclcpp::Service<dg_msgs::srv::SetTorqueLimitMode>::SharedPtr
      set_torque_limit_mode_service_;
  rclcpp::Service<dg_msgs::srv::SetBootMode>::SharedPtr set_boot_mode_service_;
  rclcpp::Service<dg_msgs::srv::EEPROMWrite>::SharedPtr eeprom_write_service_;
  rclcpp::Service<dg_msgs::srv::SystemDiagnosis>::SharedPtr
      system_diagnosis_service_;
  rclcpp::Service<dg_msgs::srv::BackupRecipeData>::SharedPtr
      backup_recipe_data_service_;
  rclcpp::Service<dg_msgs::srv::RestoreRecipeData>::SharedPtr
      restore_recipe_data_service_;

  // Joint Gain Services
  rclcpp::Service<dg_msgs::srv::SetJointGainP>::SharedPtr
      set_joint_gain_p_service_;
  rclcpp::Service<dg_msgs::srv::SetJointGainPFinger>::SharedPtr
      set_joint_gain_p_finger_service_;
  rclcpp::Service<dg_msgs::srv::SetJointGainPAll>::SharedPtr
      set_joint_gain_p_all_service_;
  rclcpp::Service<dg_msgs::srv::SetJointGainD>::SharedPtr
      set_joint_gain_d_service_;
  rclcpp::Service<dg_msgs::srv::SetJointGainDFinger>::SharedPtr
      set_joint_gain_d_finger_service_;
  rclcpp::Service<dg_msgs::srv::SetJointGainDAll>::SharedPtr
      set_joint_gain_d_all_service_;
  rclcpp::Service<dg_msgs::srv::SetJointGainI>::SharedPtr
      set_joint_gain_i_service_;
  rclcpp::Service<dg_msgs::srv::SetJointGainIFinger>::SharedPtr
      set_joint_gain_i_finger_service_;
  rclcpp::Service<dg_msgs::srv::SetJointGainIAll>::SharedPtr
      set_joint_gain_i_all_service_;
  rclcpp::Service<dg_msgs::srv::SetControlPIDMode>::SharedPtr
      set_control_pid_mode_service_;
  rclcpp::Service<dg_msgs::srv::SetJointGainPID>::SharedPtr
      set_joint_gain_pid_service_;
  rclcpp::Service<dg_msgs::srv::SetJointGainPIDFinger>::SharedPtr
      set_joint_gain_pid_finger_service_;
  rclcpp::Service<dg_msgs::srv::SetJointGainPIDAll>::SharedPtr
      set_joint_gain_pid_all_service_;
  rclcpp::Service<dg_msgs::srv::SetJointGainPIDAllEqual>::SharedPtr
      set_joint_gain_pid_all_equal_service_;

  // Motion Time Services
  rclcpp::Service<dg_msgs::srv::SetMotionTimeJoint>::SharedPtr
      set_motion_time_joint_service_;
  rclcpp::Service<dg_msgs::srv::SetMotionTimeFinger>::SharedPtr
      set_motion_time_finger_service_;
  rclcpp::Service<dg_msgs::srv::SetMotionTimeAll>::SharedPtr
      set_motion_time_all_service_;
  rclcpp::Service<dg_msgs::srv::SetMotionTimeAllEqual>::SharedPtr
      set_motion_time_all_equal_service_;

  // Position Mode Services
  rclcpp::Service<dg_msgs::srv::SetPositionModeJoint>::SharedPtr
      set_position_mode_joint_service_;
  rclcpp::Service<dg_msgs::srv::SetPositionModeFinger>::SharedPtr
      set_position_mode_finger_service_;
  rclcpp::Service<dg_msgs::srv::SetPositionModeAll>::SharedPtr
      set_position_mode_all_service_;

  // Current Control Services
  rclcpp::Service<dg_msgs::srv::SetCurrentControlMode>::SharedPtr
      set_current_control_mode_service_;
  rclcpp::Service<dg_msgs::srv::SetTargetCurrentJoint>::SharedPtr
      set_target_current_joint_service_;
  rclcpp::Service<dg_msgs::srv::SetTargetCurrentFinger>::SharedPtr
      set_target_current_finger_service_;
  rclcpp::Service<dg_msgs::srv::SetTargetCurrentAll>::SharedPtr
      set_target_current_all_service_;

  // Recipe Services
  rclcpp::Service<dg_msgs::srv::UpdateRecipeCurrentPoseData>::SharedPtr
      update_recipe_current_pose_data_service_;
  rclcpp::Service<dg_msgs::srv::UpdateRecipeTargetPoseData>::SharedPtr
      update_recipe_target_pose_data_service_;
  rclcpp::Service<dg_msgs::srv::LoadRecipePoseData>::SharedPtr
      load_recipe_pose_data_service_;
  rclcpp::Service<dg_msgs::srv::UpdateRecipeGainData>::SharedPtr
      update_recipe_gain_data_service_;
  rclcpp::Service<dg_msgs::srv::LoadRecipeGainData>::SharedPtr
      load_recipe_gain_data_service_;
  rclcpp::Service<dg_msgs::srv::UpdateRecipeGraspData>::SharedPtr
      update_recipe_grasp_data_service_;
  rclcpp::Service<dg_msgs::srv::LoadRecipeGraspData>::SharedPtr
      load_recipe_grasp_data_service_;

  // Additional Motion Services
  rclcpp::Service<dg_msgs::srv::ManualTeachMode>::SharedPtr
      manual_teach_mode_service_;
  rclcpp::Service<dg_msgs::srv::MoveJointFinger>::SharedPtr
      move_joint_finger_service_;
  rclcpp::Service<dg_msgs::srv::MoveServoJoint>::SharedPtr
      move_servo_joint_service_;

  // Additional Getter Services
  rclcpp::Service<dg_msgs::srv::GetCommunicationPeriod>::SharedPtr
      get_communication_period_service_;
  rclcpp::Service<dg_msgs::srv::GetReceivedFingertipSensorData>::SharedPtr
      get_received_fingertip_sensor_data_service_;
  rclcpp::Service<dg_msgs::srv::GetReceivedGPIOData>::SharedPtr
      get_received_gpio_data_service_;
  rclcpp::Service<dg_msgs::srv::GetDataProcessing>::SharedPtr
      get_data_processing_service_;
  rclcpp::Service<dg_msgs::srv::GetDiagnosisSystem>::SharedPtr
      get_diagnosis_system_service_;
};

// Static member definition
DGSDKRos2Bridge* DGSDKRos2Bridge::instance_ = nullptr;

int main(int argc, char* argv[]) {
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<DGSDKRos2Bridge>());
  rclcpp::shutdown();
  return 0;
}