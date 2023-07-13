cmake_minimum_required(VERSION 3.2)

project(webcam.c)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GST REQUIRED gstreamer-1.0)

add_executable(webcam webcam.c)
target_include_directories(webcam PUBLIC ${GST_INCLUDE_DIRS})
target_link_libraries(webcam ${GST_LIBRARIES})
set_target_properties(webcam PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})
