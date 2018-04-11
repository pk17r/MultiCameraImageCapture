cmake_minimum_required (VERSION 2.8)

project(io)

find_package(OpenCV REQUIRED)

find_package(PkgConfig REQUIRED)

pkg_check_modules(udev REQUIRED libudev)

include_directories(${OpenCV_INCLUDE_DIRS})
add_library(xunit
  xunit/xunit_lib.c
  xunit/xunit_lib_cug.c
)
target_link_libraries(xunit ${udev_LIBRARIES})

add_library(uvc uvc_cam.cpp)
target_link_libraries(uvc xunit ${udev_LIBRARIES} udev)

add_executable(cv_io io.cpp)
target_link_libraries(cv_io ${OpenCV_LIBS} udev uvc)
