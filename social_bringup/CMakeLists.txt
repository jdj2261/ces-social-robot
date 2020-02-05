cmake_minimum_required(VERSION 2.8.3)
project(social_bringup)

find_package(catkin REQUIRED COMPONENTS
  rospy
)

catkin_package()

include_directories(
  ${catkin_INCLUDE_DIRS}
)

install( PROGRAMS
  scripts/exhibition.py
  DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)

install (
  DIRECTORY launch/
  DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}/launch
)

install (
  DIRECTORY res/
  DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}/res
)