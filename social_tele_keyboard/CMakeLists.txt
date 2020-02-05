cmake_minimum_required(VERSION 2.8.3)
project(social_tele_keyboard)

find_package(catkin REQUIRED COMPONENTS
  rospy
  std_msgs
)

catkin_python_setup()

catkin_package(
#  INCLUDE_DIRS include
#  LIBRARIES itool_relay
#  CATKIN_DEPENDS roscpp rospy std_msgs
#  DEPENDS system_lib
)

include_directories(
  ${catkin_INCLUDE_DIRS}
)

catkin_install_python (
  PROGRAMS  nodes/social_tele_keyboard
  DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)
