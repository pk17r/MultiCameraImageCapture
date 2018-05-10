cmake_minimum_required (VERSION 2.8)
set (CMAKE_CXX_STANDARD 11)

project(io)

find_package(OpenCV REQUIRED)

find_package(PkgConfig REQUIRED)

find_package(Boost REQUIRED COMPONENTS thread system)

pkg_check_modules(udev REQUIRED libudev)

include_directories(${OpenCV_INCLUDE_DIRS})

add_library(xunit
  xunit/xunit_lib.c
  xunit/xunit_lib_cug.c
)
target_link_libraries(xunit ${udev_LIBRARIES})

add_library(uvc uvc_cam.cpp)
target_link_libraries(uvc xunit ${udev_LIBRARIES} ${Boost_LIBRARIES} udev)

include_directories(mavlink)
add_library(mav serial_port.cpp autopilot_interface.cpp)

add_library(cam camera.cpp)
INCLUDE_DIRECTORIES(mavlink)
target_link_libraries(cam ${udev_LIBRARIES} ${OpenCV_LIBS} ${Boost_LIBRARIES} udev uvc mav)

add_executable(cv_io io.cpp)
target_link_libraries(cv_io ${OpenCV_LIBS} udev uvc cam)
