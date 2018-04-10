cmake_minimum_required (VERSION 2.8)

project(io)

find_package(OpenCV REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS})

add_library(io_econA xunit_lib.c)
add_library(io_econB xunit_lib_cug.c)
target_link_libraries(io_econA udev)
target_link_libraries(io_econB udev)

add_executable(cv_io io.cpp)

target_link_libraries(cv_io ${OpenCV_LIBS} io_econA udev io_econB udev)
