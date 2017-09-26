cmake_minimum_required(VERSION 2.8.3)
project(vss)

## Find catkin macros and libraries
## if COMPONENTS list like find_package(catkin REQUIRED COMPONENTS xyz)
## is used, also find other catkin packages
find_package(catkin REQUIRED)


find_package(catkin REQUIRED COMPONENTS
  roscpp
  rospy
  std_msgs
  sensor_msgs
  cv_bridge
  image_transport
  geometry_msgs 
  #vrep_common
  message_generation
)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
include_directories(include ${catkin_INCLUDE_DIRS})

## System dependencies are found with CMake's conventions
# find_package(Boost REQUIRED COMPONENTS system)


## Uncomment this if the package has a setup.py. This macro ensures
## modules and global scripts declared therein get installed
## See http://ros.org/doc/api/catkin/html/user_guide/setup_dot_py.html
# catkin_python_setup()

################################################
## Declare ROS messages, services and actions ##
################################################

## To declare and build messages, services or actions from within this
## package, follow these steps:
## * Let MSG_DEP_SET be the set of packages whose message types you use in
##   your messages/services/actions (e.g. std_msgs, actionlib_msgs, ...).
## * In the file package.xml:
##   * add a build_depend and a run_depend tag for each package in MSG_DEP_SET
##   * If MSG_DEP_SET isn't empty the following dependencies might have been
##     pulled in transitively but can be declared for certainty nonetheless:
##     * add a build_depend tag for "message_generation"
##     * add a run_depend tag for "message_runtime"
## * In this file (CMakeLists.txt):
##   * add "message_generation" and every package in MSG_DEP_SET to
##     find_package(catkin REQUIRED COMPONENTS ...)
##   * add "message_runtime" and every package in MSG_DEP_SET to
##     catkin_package(CATKIN_DEPENDS ...)
##   * uncomment the add_*_files sections below as needed
##     and list every .msg/.srv/.action file to be processed
##   * uncomment the generate_messages entry below
##   * add every package in MSG_DEP_SET to generate_messages(DEPENDENCIES ...)

## Generate messages in the 'msg' folder
add_message_files(
  FILES
  ponto.msg
  detectedObjects.msg
  pos.msg
  pos_array.msg
)

## Generate services in the 'srv' folder
# add_service_files(
#   FILES
#   Service1.srv
#   Service2.srv
# )

## Generate actions in the 'action' folder
# add_action_files(
#   FILES
#   Action1.action
#   Action2.action
# )

## Generate added messages and services with any dependencies listed here
generate_messages(
  DEPENDENCIES
  std_msgs  # Or other packages containing msgs
)

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
#  LIBRARIES vss
#  CATKIN_DEPENDS other_catkin_pkg
#  DEPENDS system_lib
   CATKIN_DEPENDS message_runtime
)

###########
## Build ##
###########

## Specify additional locations of header files
## Your package locations should be listed before other locations
# include_directories(include)

## Declare a cpp library
# add_library(vss
#   src/${PROJECT_NAME}/vss.cpp
# )


#simulador

add_executable(pause src/pause.cpp) 

add_executable(teste src/Subscriber.cpp) 

add_executable(estrategia src/estrategia/funcoes.hpp src/estrategia/funcoes.cpp src/estrategia/wavefront.cpp) 
##add_dependencies(simulador vrep_common_generate_messages_cpp)

## Declare a cpp executable

#Detecção de linha/bola/inimigo
add_executable(setLine src/setLine/main.cpp)
add_executable(detectEnemy src/detectObject/multipleObjectTracking.cpp src/detectObject/robot.cpp)

#conversor de imagens pro ar_pose
add_executable(im_converter src/im_converter/main.cpp)
add_executable(robot_view src/interface/show.cpp)

#comunicação serial de arduino com ros
#add_executable(arducom src/arducom/main.cpp)
add_executable(joystick src/arducom/publisher.cpp)

#controle
add_executable(controle src/controle/main.cpp src/controle/fut.cpp)



## Add cmake target dependencies of the executable/library
## as an example, message headers may need to be generated before nodes
# add_dependencies(vss_node vss_generate_messages_cpp)

## Specify libraries to link a library or executable target against
# target_link_libraries(vss_node
#   ${catkin_LIBRARIES}
# )

#############
## Install ##
#############

# all install targets should use catkin DESTINATION variables
# See http://ros.org/doc/api/catkin/html/adv_user_guide/variables.html

## Mark executable scripts (Python etc.) for installation
## in contrast to setup.py, you can choose the destination
# install(PROGRAMS
#   scripts/my_python_script
#   DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
# )

## Mark executables and/or libraries for installation
# install(TARGETS vss vss_node
#   ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
#   LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
#   RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
# )

## Mark cpp header files for installation
# install(DIRECTORY include/${PROJECT_NAME}/
#   DESTINATION ${CATKIN_PACKAGE_INCLUDE_DESTINATION}
#   FILES_MATCHING PATTERN "*.h"
#   PATTERN ".svn" EXCLUDE
# )

## Mark other files for installation (e.g. launch and bag files, etc.)
# install(FILES
#   # myfile1
#   # myfile2
#   DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}
# )

#############
## Testing ##
#############

## Add gtest based cpp test target and link libraries
# catkin_add_gtest(${PROJECT_NAME}-test test/test_vss.cpp)
# if(TARGET ${PROJECT_NAME}-test)
#   target_link_libraries(${PROJECT_NAME}-test ${PROJECT_NAME})
# endif()

## Add folders to be run by python nosetests
# catkin_add_nosetests(test)
target_link_libraries(im_converter ${OpenCV_LIBRARIES})
target_link_libraries (im_converter ${catkin_LIBRARIES} )
target_link_libraries(robot_view ${OpenCV_LIBRARIES})
target_link_libraries (robot_view ${catkin_LIBRARIES} )
target_link_libraries(setLine ${OpenCV_LIBRARIES})
target_link_libraries (setLine ${catkin_LIBRARIES} )
target_link_libraries (detectEnemy ${catkin_LIBRARIES} )
target_link_libraries(detectEnemy ${OpenCV_LIBS} )
target_link_libraries(estrategia ${catkin_LIBRARIES})
#target_link_libraries(arducom ${catkin_LIBRARIES})
target_link_libraries(joystick ${catkin_LIBRARIES})
target_link_libraries(controle ${catkin_LIBRARIES})
target_link_libraries(teste ${catkin_LIBRARIES})
target_link_libraries(pause ${catkin_LIBRARIES})


