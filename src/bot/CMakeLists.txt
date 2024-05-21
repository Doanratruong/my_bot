cmake_minimum_required(VERSION 3.0.2)
project(bot)

## Find catkin macros and libraries
find_package(catkin REQUIRED COMPONENTS
  roscpp
  hardware_interface
  controller_manager
  serial
  std_msgs
  geometry_msgs
)

## Declare a catkin package
catkin_package(
  CATKIN_DEPENDS roscpp hardware_interface controller_manager serial std_msgs geometry_msgs
)

## Specify additional locations of header files
include_directories(
  ${catkin_INCLUDE_DIRS}
  include
)

## Declare a C++ executable
add_executable(mobile_robot_hardware_interface
  src/mobile_robot_hardware_interface.cpp
  src/mobile_robot_hardware_interface_node.cpp
  
)

## Specify libraries to link a library or executable target against
target_link_libraries(mobile_robot_hardware_interface
  ${catkin_LIBRARIES}
)

## Install executable targets
install(TARGETS mobile_robot_hardware_interface
  RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)

## Install Python scripts
catkin_install_python(PROGRAMS scripts/motor_control.py
  DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)

