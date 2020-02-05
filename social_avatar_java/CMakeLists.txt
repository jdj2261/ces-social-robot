cmake_minimum_required(VERSION 2.8.3)
project(test_game_server)

find_package(catkin REQUIRED COMPONENTS
)

catkin_package()

include_directories(
  ${catkin_INCLUDE_DIRS}
)

install(DIRECTORY system  DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION} )
install(DIRECTORY libs  DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION} )
install(DIRECTORY scripts  DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION} )