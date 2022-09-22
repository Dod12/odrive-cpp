# Setup
cmake_minimum_required(VERSION 3.22)
set(CMAKE_CXX_STANDARD 17)
project(odrive-lib VERSION 1.0.0 DESCRIPTION "Simple C++ library for interacting with ODrive controller." LANGUAGES CXX)

add_subdirectory(extern)

add_library(odrive_lib SHARED src/odrive.cpp)
set_target_properties(odrive_lib PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(odrive_lib PROPERTIES SOVERSION 1)

set_target_properties(odrive_lib PROPERTIES PUBLIC_HEADER include/odrive.h)
target_sources(odrive_lib INTERFACE include/odrive.h include/odrive_enums.h)
target_include_directories(odrive_lib PUBLIC include)
target_include_directories(odrive_lib PRIVATE src)

target_link_libraries(odrive_lib PUBLIC odrive_usb)
target_include_directories(odrive_lib PUBLIC odrive_usb)

add_executable(odrive_test test/main.cpp)
target_link_libraries(odrive_test PRIVATE odrive_lib)
target_link_libraries(odrive_test PRIVATE ${LIBUSB_LIBRARY})

