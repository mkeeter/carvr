cmake_minimum_required(VERSION 2.6) 
project(carvr)

find_package(OpenCV REQUIRED)
include(${OpenCV_INCLUDE_DIRS})

find_package(wxWidgets REQUIRED core base)
include(${wxWidgets_USE_FILE})

set(CMAKE_CXX_FLAGS "-Wall -O3")
add_executable (carvr MACOSX_BUNDLE main.cc seam.cc energy.cc app.cc frame.cc panel.cc overlay.cc)
target_link_libraries(carvr ${OpenCV_LIBS} ${wxWidgets_LIBRARIES})
