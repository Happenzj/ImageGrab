cmake_minimum_required(VERSION 2.8)
project(imagegrab)

set( CMAKE_CXX_STANDARD 11)
set( CMAKE_BUILD_TYPE "Release" )
set( CMAKE_CXX_FLAGS "-std=c++11 -march=native -O3 -pthread" )

find_package(OpenCV  REQUIRED)
find_package(catkin REQUIRED COMPONENTS roscpp  std_msgs sensor_msgs topic_tools tf cv_bridge)
find_package(Boost REQUIRED COMPONENTS system thread)
catkin_package(CATKIN_DEPENDS roscpp std_msgs sensor_msgs topic_tools tf )

include_directories(${Boost_INCLUDE_DIRS} ${catkin_INCLUDE_DIRS})
include_directories(${OpenCV_INCLUDE_DIRS})

set( THIRD_PARTY_LIBS
        ${OpenCV_LIBS}
        ${catkin_LIBRARIES}
        -lboost_system)

set( EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin )
set( LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib )

add_executable(ImageGrab src/ImageGrab.cpp)
target_link_libraries(ImageGrab ${THIRD_PARTY_LIBS})