cmake_minimum_required(VERSION 2.8.3)
project(boost_python_catkin_example)
add_compile_options(-std=c++17)

## Find catkin macros and libraries
## if COMPONENTS list like find_package(catkin REQUIRED COMPONENTS xyz)
## is used, also find other catkin packages
find_package(catkin REQUIRED)

## System dependencies are found with CMake's conventions
find_package(Boost REQUIRED COMPONENTS python)
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

## Uncomment this if the package has a setup.py. This macro ensures
## modules and global scripts declared therein get installed
## See http://ros.org/doc/api/catkin/html/user_guide/setup_dot_py.html
catkin_python_setup() # this sets up the path /devel/lib/python2.7/dist-packages/boostpy_test

###################################
## catkin specific configuration ##
###################################
## The catkin_package macro generates cmake config files for your package
## Declare things to be passed to dependent projects
## INCLUDE_DIRS: uncomment this if you package contains header files
## LIBRARIES: libraries you create in this project that dependent projects also need
## CATKIN_DEPENDS: catkin_packages dependent projects also need
## DEPENDS: system dependencies of this project that dependent projects also need
catkin_package(
#  INCLUDE_DIRS include
#  LIBRARIES boostpy_test
#  CATKIN_DEPENDS roscpp rospy
#  DEPENDS system_lib
)


include($ENV{ROS_ROOT}/core/rosbuild/rosbuild.cmake)


catkin_install_python(PROGRAMS
	scripts/use_cpplib.py
	scripts/test.py
        DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION})

###########
## Build ##
###########


########## For OpenCV ##########
find_package(OpenCV REQUIRED)
include_directories(/usr/local/include)
include_directories(/usr/local/include/opencv)
include_directories(/usr/local/include/opencv2)

link_directories(/usr/local/lib)

find_package(OpenCV)
include_directories(${OpenCV_INCLUDE_DIRS})

## Specify additional locations of header files
## Your package locations should be listed before other locations
# include_directories(include)
include_directories(
  ${catkin_INCLUDE_DIRS}
  ${Boost_INCLUDE_DIRS}
  ${PYTHON_INCLUDE_DIRS}
)


########## For ROS ##########
catkin_package(
  CATKIN_DEPENDS roscpp rospy std_msgs opencv2 cv_bridge dynamic_reconfigure message_generation message_runtime
  #DEPENDS system_lib
)


########## For Catkin ##########
include_directories(include ${catkin_INCLUDE_DIRS})

#set the default path for built executables to the "bin" directory
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
#set the default path for built libraries to the "lib" directory
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)




PYTHON_ADD_MODULE(use_cpplib src/mycpplib.cpp)
add_library(mycpplib SHARED
  src/mycpplib.cpp
)
# change output directory, so python can find the module, and set prefix to ""
# to omit the default "lib".
set_target_properties(mycpplib PROPERTIES
  PREFIX ""
  LIBRARY_OUTPUT_DIRECTORY ${CATKIN_DEVEL_PREFIX}/${CATKIN_PACKAGE_PYTHON_DESTINATION}
)

target_link_libraries(mycpplib
  ${catkin_LIBRARIES}
  ${Boost_LIBRARIES}
  ${PYTHON_LIBRARIES}

)

install(TARGETS mycpplib
  ARCHIVE DESTINATION ${CATKIN_PACKAGE_PYTHON_DESTINATION}
  LIBRARY DESTINATION ${CATKIN_PACKAGE_PYTHON_DESTINATION}
)



add_library(examples SHARED
  src/pyboost_cv2_converter.cpp
  src/pyboost_cv3_converter.cpp
  src/pyboost_cv4_converter.cpp
  src/examples.cpp
)
# change output directory, so python can find the module, and set prefix to ""
# to omit the default "lib".
set_target_properties(examples PROPERTIES
  PREFIX ""
  LIBRARY_OUTPUT_DIRECTORY ${CATKIN_DEVEL_PREFIX}/${CATKIN_PACKAGE_PYTHON_DESTINATION}
)

target_link_libraries(examples
  ${catkin_LIBRARIES}
  ${Boost_LIBRARIES}
  ${OpenCV_LIBRARIES}
  ${PYTHON_LIBRARIES}
)
add_dependencies(examples
    ${PROJECT_NAME}_gencfg
)

install(TARGETS examples
  ARCHIVE DESTINATION ${CATKIN_PACKAGE_PYTHON_DESTINATION}
  LIBRARY DESTINATION ${CATKIN_PACKAGE_PYTHON_DESTINATION}
)
