cmake_minimum_required(VERSION 2.8.7)

project(roboball2d_interface)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

find_package(catkin REQUIRED COMPONENTS
  pybind11_catkin
  robot_interfaces
  shared_memory
  real_time_tools)

# defined in catkin_package mpi_cmake_modules,
# used to detected for example if using real time computer,
# also host various cmake files
DEFINE_OS()

catkin_package(
 LIBRARIES  roboball2d_interface
 INCLUDE_DIRS include
 CATKIN_DEPENDS shared_memory real_time_tools robot_interfaces)

include_directories(include
  ${catkin_INCLUDE_DIRS} )


################
# Main Library #
################

add_library( roboball2d_interface
  src/torques_action.cpp
  src/ball_gun_action.cpp
  src/mirror_action.cpp )
target_link_libraries(roboball2d_interface ${catkin_LIBRARIES})

#########
# Demos #
#########

add_executable( roboball2d_interface_demo
  demo/roboball2d_interface_demo.cpp )
target_link_libraries(roboball2d_interface_demo
  roboball2d_interface
  ${catkin_LIBRARIES})

###################
# Python bindings #
###################

pybind11_add_module(roboball2d_interface_py
  srcpy/wrappers.cpp)
target_link_libraries(roboball2d_interface_py
  PUBLIC roboball2d_interface ${catkin_LIBRARIES})
set_target_properties(roboball2d_interface_py PROPERTIES
  LIBRARY_OUTPUT_DIRECTORY ${CATKIN_DEVEL_PREFIX}/${CATKIN_GLOBAL_PYTHON_DESTINATION}
  OUTPUT_NAME roboball2d_interface)
install(TARGETS roboball2d_interface_py
  DESTINATION ${CATKIN_GLOBAL_PYTHON_DESTINATION})

###################
# Python package  #
###################

catkin_python_setup()





