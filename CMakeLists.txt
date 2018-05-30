cmake_minimum_required(VERSION 2.8)
set(OpenCV_DIR "/home/lingkang/Documents/opencv-3.1.0/build")
project(cv_thread_track)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

#find_package(Threads REQUIRED)

set(CMAKE_CXX_FLAGS "-Wall -Wextra -pthread")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )
find_package(Threads REQUIRED)
add_definitions(-std=c++11)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
add_library(det_and_track SHARED det_and_track.cpp)
target_link_libraries(det_and_track ${OpenCV_LIBS})

link_directories(${CMAKE_CURRENT_BINARY_DIR})
add_executable( cv_thread_track main.cpp)
target_link_libraries(cv_thread_track det_and_track ${OpenCV_LIBS})
