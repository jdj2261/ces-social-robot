cmake_minimum_required(VERSION 2.8.3)
project(social_startup)


find_package(catkin REQUIRED COMPONENTS
  roscpp
  rospy
  sensor_msgs
  std_msgs
  social_msgs
  robocare_msgs
)


catkin_package(
  INCLUDE_DIRS include
  LIBRARIES social_bringup
  CATKIN_DEPENDS roscpp rospy roslib std_msgs sensor_msgs social_msgs robocare_msgs
#    DEPENDS system_lib
)


include_directories(
        include
        ${catkin_INCLUDE_DIRS}
)

add_executable(
                        social_bringup  src/social_bringup.cpp
)

target_link_libraries(
                        social_bringup	${catkin_LIBRARIES}
)

add_dependencies(social_bringup social_msgs_generate_messages_cpp robocare_msgs_generate_messages_cpp)

install(
                        TARGETS social_bringup
                        RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
                        ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
                        LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
)

#install(DIRECTORY launch/
#  DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}/launch
#  PATTERN ".svn" EXCLUDE
#)

