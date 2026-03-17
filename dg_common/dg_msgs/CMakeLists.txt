cmake_minimum_required(VERSION 3.8)
project(dg_msgs)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rosidl_default_generators REQUIRED)

# declare the message and service files
rosidl_generate_interfaces(${PROJECT_NAME}
  # Messages
  "msg/ReceivedGripperData.msg"
  "msg/RecipeBlendData.msg"
  "msg/GripperSystemSetting.msg"
  "msg/GripperSetting.msg"
  "msg/RecipePoseData.msg"
  "msg/RecipeGainData.msg"
  "msg/RecipeGraspData.msg"
  "msg/ReceivedFingertipSensorData.msg"
  "msg/ReceivedGPIOData.msg"
  "msg/DiagnosisSystem.msg"
  
  # System Setting Services
  "srv/SetGripperSystem.srv"
  "srv/SetGripperOption.srv"
  "srv/ConnectToGripper.srv"
  "srv/DisconnectToGripper.srv"
  "srv/SystemStart.srv"
  "srv/SystemStop.srv"
  "srv/SetIp.srv"
  
  # Other System Services
  "srv/SetLowPassFilterAlpha.srv"
  "srv/SetBootMode.srv"
  "srv/SetGPIOOutput.srv"
  "srv/SetGPIOOutputAll.srv"
  "srv/SetTorqueLimitMode.srv"
  "srv/SetBootRecipe.srv"
  "srv/EEPROMWrite.srv"
  "srv/SystemDiagnosis.srv"
  "srv/BackupRecipeData.srv"
  "srv/RestoreRecipeData.srv"
  
  # Motion Setting Services
  "srv/SetGraspData.srv"
  "srv/SetGraspForce.srv"
  "srv/SetJointGainP.srv"
  "srv/SetJointGainPFinger.srv"
  "srv/SetJointGainPAll.srv"
  "srv/SetJointGainD.srv"
  "srv/SetJointGainDFinger.srv"
  "srv/SetJointGainDAll.srv"
  "srv/SetJointGainI.srv"
  "srv/SetJointGainIFinger.srv"
  "srv/SetJointGainIAll.srv"
  "srv/SetControlPIDMode.srv"
  "srv/SetJointGainPID.srv"
  "srv/SetJointGainPIDFinger.srv"
  "srv/SetJointGainPIDAll.srv"
  "srv/SetJointGainPIDAllEqual.srv"
  "srv/SetMotionTimeJoint.srv"
  "srv/SetMotionTimeFinger.srv"
  "srv/SetMotionTimeAll.srv"
  "srv/SetMotionTimeAllEqual.srv"
  "srv/SetPositionModeJoint.srv"
  "srv/SetPositionModeFinger.srv"
  "srv/SetPositionModeAll.srv"
  "srv/SetCurrentControlMode.srv"
  "srv/SetTargetCurrentJoint.srv"
  "srv/SetTargetCurrentFinger.srv"
  "srv/SetTargetCurrentAll.srv"
  
  # Recipe Services
  "srv/UpdateRecipeCurrentPoseData.srv"
  "srv/UpdateRecipeTargetPoseData.srv"
  "srv/LoadRecipePoseData.srv"
  "srv/UpdateRecipeGainData.srv"
  "srv/LoadRecipeGainData.srv"
  "srv/UpdateRecipeGraspData.srv"
  "srv/LoadRecipeGraspData.srv"
  
  # Blend Motion Services
  "srv/UpdateBlendJoint.srv"
  "srv/ClearMoveBlendJoint.srv"
  "srv/AddMoveBlendJoint.srv"
  "srv/SetMoveBlendJoint.srv"
  "srv/StartMoveBlendJoint.srv"
  "srv/StopMoveBlendJoint.srv"
  
  # Motion Services
  "srv/ManualTeachMode.srv"
  "srv/StartGraspMotion.srv"
  "srv/MoveJoint.srv"
  "srv/MoveJointFinger.srv"
  "srv/MoveJointAll.srv"
  "srv/MoveServoJoint.srv"
  
  # Experimental Services
  "srv/SetTCPGainPFinger.srv"
  "srv/SetTCPGainPAll.srv"
  "srv/SetTCPGainDFinger.srv"
  "srv/SetTCPGainDAll.srv"
  "srv/SetTCPGainIFinger.srv"
  "srv/SetTCPGainIAll.srv"
  "srv/SetTCPGainPIDFinger.srv"
  "srv/SetTCPGainPIDAll.srv"
  "srv/SetTCPMotionTimeFinger.srv"
  "srv/SetTCPMotionTimeAll.srv"
  "srv/SetTCPMotionTimeAllEqual.srv"
  "srv/MoveTCPFinger.srv"
  "srv/MoveTCPAll.srv"
  "srv/GetCurrentTcpPose.srv"
  "srv/SetManipulationGainPIDAll.srv"
  "srv/InHandManipulation.srv"
  "srv/SetFingerTipDataZero.srv"
  
  # Getter Services
  "srv/GetReceivedGripperData.srv"
  "srv/GetCommunicationPeriod.srv"
  "srv/GetReceivedFingertipSensorData.srv"
  "srv/GetReceivedGPIOData.srv"
  "srv/GetDataProcessing.srv"
  "srv/GetDiagnosisSystem.srv"
)

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  ament_lint_auto_find_test_dependencies()
endif()

ament_package()