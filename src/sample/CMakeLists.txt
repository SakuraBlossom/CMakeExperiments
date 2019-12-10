########## Catkin Configurations ##########
cmake_minimum_required(VERSION 2.4.6)
add_compile_options(-std=c++17)

project(sample)
include($ENV{ROS_ROOT}/core/rosbuild/rosbuild.cmake)
include_directories(include)

########## For QT5 Setup ##########
# Tell CMake to run moc when necessary:
set(CMAKE_AUTOMOC ON)
# As moc files are generated in the binary dir, tell CMake
# to always look for includes there:
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Widgets finds its own dependencies (QtGui and QtCore).
find_package(Qt5Widgets REQUIRED COMPONENTS QtCore QtGui)
find_package(Qt5Network REQUIRED)

# The Qt5Widgets_INCLUDES also includes the include directories for
# dependencies QtCore and QtGui
include_directories(${Qt5Widgets_INCLUDES})

# We need add -DQT_WIDGETS_LIB when using QtWidgets in Qt 5.
add_definitions(${Qt5Widgets_DEFINITIONS})

# Executables fail to build with Qt 5 in the default configuration
# without -fPIE. We add that here.
set(CMAKE_CXX_FLAGS "${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}")

########## For OpenCV ##########
include_directories(/usr/local/include)
include_directories(/usr/local/include/opencv)
include_directories(/usr/local/include/opencv2)

link_directories(/usr/local/lib)

find_package(OpenCV)
include_directories(${OpenCV_INCLUDE_DIRS})

########## Catkin Package ##########
find_package(catkin REQUIRED COMPONENTS
  cv_bridge
  dynamic_reconfigure
  message_generation
  message_runtime
  image_transport
  roscpp
  std_msgs
  sensor_msgs
)

########## For ROS Message Files ##########
#add_message_files(
#   FILES
#)

generate_messages(
   DEPENDENCIES std_msgs sensor_msgs
)


########## For ROS ##########
catkin_package(
  CATKIN_DEPENDS roscpp rospy std_msgs sensor_msgs opencv2 cv_bridge dynamic_reconfigure message_generation message_runtime
  #DEPENDS system_lib
)

########## For Catkin ##########
include_directories(include ${catkin_INCLUDE_DIRS})

#set the default path for built executables to the "bin" directory
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
#set the default path for built libraries to the "lib" directory
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

########## For Project Files ##########


message("PLATFORM=$ENV{PLATFORM}")

if($ENV{PLATFORM} STREQUAL "AUV4")
    message("Targeting AUV 4.0")
    add_definitions(-DTARGET_AUV_4)
    
elseif($ENV{PLATFORM} STREQUAL "ASV2")
    message("Targeting ASV 2.0")
    add_definitions(-DTARGET_ASV_2)
    

elseif($ENV{PLATFORM} STREQUAL "AUV3")
    message("Targeting AUV 3.5X")
    add_definitions(-DTARGET_AUV_3)
    
else()
    message("ERROR")
    

endif()



#User Interface
add_executable(TestDemo
    src/Demo.cpp
)
target_link_libraries(TestDemo ${catkin_LIBRARIES})
target_link_libraries(TestDemo ${OpenCV_LIBRARIES})
target_link_libraries(TestDemo ${Qt5Widgets_LIBRARIES})
target_link_libraries(TestDemo ${Qt5Network_LIBRARIES})
add_dependencies(TestDemo ${PROJECT_NAME}_generate_messages_cpp)
add_dependencies(TestDemo ${PROJECT_NAME}_gencfg)

#Shawn Lum was here.

