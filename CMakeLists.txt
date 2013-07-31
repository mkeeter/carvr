cmake_minimum_required(VERSION 2.6) 
project(carvr)

find_package(OpenCV REQUIRED)

set(CMAKE_CXX_FLAGS "-Wall -O3")
add_executable (carvr main.cc seam.cc energy.cc)
target_link_libraries(carvr ${OpenCV_LIBS})
