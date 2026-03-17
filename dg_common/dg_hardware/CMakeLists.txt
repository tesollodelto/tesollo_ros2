cmake_minimum_required(VERSION 3.8)
project(delto_hardware)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

set(CMAKE_CXX_STANDARD 17)

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(hardware_interface REQUIRED)
find_package(pluginlib REQUIRED)
find_package(rclcpp REQUIRED)
find_package(rclcpp_lifecycle REQUIRED)
find_package(std_srvs REQUIRED)
find_package(delto_tcp_comm REQUIRED)
find_package(Boost REQUIRED COMPONENTS system)

# Import delto_gripper_helper library (shared across all drivers)
add_library(delto_gripper_helper SHARED IMPORTED)
set_target_properties(delto_gripper_helper PROPERTIES
  IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/lib/libdelto_gripper_helper.so
)

# Build the hardware interface library
add_library(${PROJECT_NAME} SHARED
  src/system_interface.cpp
)

target_include_directories(${PROJECT_NAME} PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)

ament_target_dependencies(${PROJECT_NAME}
  hardware_interface
  pluginlib
  rclcpp
  rclcpp_lifecycle
  std_srvs
  delto_tcp_comm
)

target_link_libraries(${PROJECT_NAME}
  delto_gripper_helper
  Boost::system
)

# Export hardware plugins
pluginlib_export_plugin_description_file(hardware_interface delto_hardware.xml)

# Install
install(
  TARGETS ${PROJECT_NAME}
  EXPORT export_${PROJECT_NAME}
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin
)

install(
  DIRECTORY include/
  DESTINATION include
)

install(
  FILES delto_hardware.xml
  DESTINATION share/${PROJECT_NAME}
)

ament_export_include_directories(include)
ament_export_libraries(${PROJECT_NAME})
ament_export_targets(export_${PROJECT_NAME})
ament_export_dependencies(
  hardware_interface
  pluginlib
  rclcpp
  rclcpp_lifecycle
  std_srvs
  delto_tcp_comm
)

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  ament_lint_auto_find_test_dependencies()
endif()

ament_package()
