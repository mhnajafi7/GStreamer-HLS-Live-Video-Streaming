cmake_minimum_required(VERSION 3.2)

project(playvido.c)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GST REQUIRED gstreamer-1.0)

add_executable(playvideo playvideo.c)
target_include_directories(playvideo PUBLIC ${GST_INCLUDE_DIRS})
target_link_libraries(playvideo ${GST_LIBRARIES})
set_target_properties(playvideo PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})
