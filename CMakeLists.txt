cmake_minimum_required(VERSION 3.0.2)
project(nanocraw_ctrl)

## Compile as C++11, supported in ROS Kinetic and newer
# add_compile_options(-std=c++11)

## Find catkin macros and libraries
## if COMPONENTS list like find_package(catkin REQUIRED COMPONENTS xyz)
## is used, also find other catkin packages
find_package(catkin REQUIRED COMPONENTS
  geometry_msgs
  roscpp
  rospy
  std_msgs
  message_generation
)

add_message_files(
  FILES
  CrawlerCmd.msg
)
generate_messages(DEPENDENCIES std_msgs)


#catkin_package(
#  INCLUDE_DIRS include
# LIBRARIES icp_control
#  CATKIN_DEPENDS roscpp rospy std_msgs message_runtime
#  DEPENDS system_lib
#)

install(DIRECTORY
  launch
  DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}
)

catkin_install_python(PROGRAMS scripts/joycon_ope.py
  DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION})

#include_directories(
# include
#  ${catkin_INCLUDE_DIRS}
#)
