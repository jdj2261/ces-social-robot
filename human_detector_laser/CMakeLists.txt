cmake_minimum_required(VERSION 2.8.3)
project(human_detector_laser)

find_package(catkin REQUIRED COMPONENTS
  roscpp
  rospy
  sensor_msgs
  std_msgs
)

catkin_package(
  INCLUDE_DIRS include
  LIBRARIES human_detector_laser
  CATKIN_DEPENDS roscpp rospy sensor_msgs std_msgs
  DEPENDS system_lib
)

find_package(OpenCV REQUIRED)
IF (OPENCV_FOUND)
	  MESSAGE("-- Found OpenCV version ${OPENCV_VERSION}: ${OPENCV_INCLUDE_DIRS}")
ENDIF (OPENCV_FOUND)

include_directories(
			include
			$(find silbot3_common_tools)	
			${catkin_INCLUDE_DIRS}
			${OpenCV_INCLUDE_DIRS}			
		)

link_directories(
		  	${OpenCV_LIBRARY_DIRS}
		  	/opt/ros/jade/lib
			${OPENNI2_REDIST} 
			${NITE2_REDIST64}
		)

add_executable(
			human_detector_laser	src/silbot_parking.cpp			
									src/tool_class.cpp	
		)

target_link_libraries(
			human_detector_laser	${catkin_LIBRARIES} 
									${OpenCV_LIBS}
		)

install(
			TARGETS human_detector_laser
			RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
			ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
			LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
		)

