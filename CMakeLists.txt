cmake_minimum_required(VERSION 2.6) 
project(carvr)

find_package(OpenCV REQUIRED core imgproc highgui)
include(${OpenCV_INCLUDE_DIRS})

find_package(wxWidgets 2.9.0 REQUIRED core base adv)
include(${wxWidgets_USE_FILE})

set(CMAKE_CXX_FLAGS "-Wall -O3 -g")
set(CMAKE_CXX_LINKER_FLAGS "-headerpad_max_install_names")

set(SRCS MACOSX_BUNDLE main.cc seam.cc energy.cc app.cc frame.cc panel.cc overlay.cc bitmaps.cc image.cc worker.cc)

if(APPLE)
    add_custom_command(
        OUTPUT carvr.icns
        COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/icon.py
        DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/icon.py
     )
    set_source_files_properties(carvr.icns PROPERTIES MACOSX_PACKAGE_LOCATION Resources)
    set(SRCS ${SRCS} carvr.icns)
endif(APPLE)

add_executable(carvr ${SRCS})
target_link_libraries(carvr ${OpenCV_LIBS} ${wxWidgets_LIBRARIES})

if (APPLE)
    add_custom_command(
        TARGET carvr POST_BUILD
        COMMAND /usr/libexec/PListBuddy -c "Set :CFBundleIconFile carvr.icns" -c "Set :CFBundleName carvr" carvr.app/Contents/Info.plist
        COMMENT "Updating Info.plist"
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    )
    add_custom_command(
        TARGET carvr POST_BUILD
        COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/dylib.py
        COMMENT "Relinking dylibs"
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    )
endif(APPLE)

#add_executable (test test.cc seam.cc energy.cc app.cc frame.cc panel.cc overlay.cc bitmaps.cc image.cc)
#target_link_libraries(test ${OpenCV_LIBS} ${wxWidgets_LIBRARIES})
