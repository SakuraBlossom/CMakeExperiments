cmake_minimum_required(VERSION 3.0)
project(node_design_demo)
add_compile_options(-std=c++17)

## Find catkin macros and libraries
## if COMPONENTS list like find_package(catkin REQUIRED COMPONENTS xyz)
## is used, also find other catkin packages
find_package(catkin REQUIRED)

## System dependencies are found with CMake's conventions
find_package(Boost REQUIRED COMPONENTS python python3)
find_package(PythonLibs 2.7 REQUIRED) # sets ${PYTHON_INCLUDE_DIRS}

find_package(catkin REQUIRED COMPONENTS
  cv_bridge
  dynamic_reconfigure
  message_generation
  roscpp
  rospy
  std_msgs
  geometry_msgs
  image_transport
  visualization_msgs
)

include_directories(
  include
  ${catkin_INCLUDE_DIRS}
)

catkin_package(
  INCLUDE_DIRS include
  LIBRARIES simple_class_node
  CATKIN_DEPENDS ${PKG_DEPS}
)

add_library(simple_class_node
  src/simple_node_class.cpp)

target_link_libraries(simple_class_node
  ${catkin_LIBRARIES})

add_executable(simple_node
  src/simple_node.cpp)

target_link_libraries(simple_node
  ${catkin_LIBRARIES})

add_executable(single_thread_node_instance
  src/single_thread_node_instance.cpp)

target_link_libraries(single_thread_node_instance
  ${catkin_LIBRARIES}
  simple_class_node)

add_executable(multi_thread_node_instance
  src/multi_thread_node_instance.cpp)
target_link_libraries(multi_thread_node_instance
  ${catkin_LIBRARIES}
  simple_class_node)

add_executable(async_multi_thread_node_instance
  src/async_multi_thread_node_instance.cpp)
target_link_libraries(async_multi_thread_node_instance
  ${catkin_LIBRARIES}
  simple_class_node)







