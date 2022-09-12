cmake_minimum_required(VERSION 3.5)
project(odrive_hardware_interface)

# Default to C99
if(NOT CMAKE_C_STANDARD)
  set(CMAKE_C_STANDARD 99)
endif()

# Default to C++14
if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 14)
endif()

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic -Wno-implicit-fallthrough)
endif()

# find dependencies
find_path(LIBUSB_INCLUDE_DIR
        PATHS /usr/local
        NAMES libusb.h
        PATH_SUFFIXES "include" "libusb" "libusb-1.0")
find_library(LIBUSB_LIBRARY
        PATHS /usr/local
        NAMES libusb-1.0.0.dylib
        PATH_SUFFIXES "lib" "lib32" "lib64")

add_library(
  odrive_usb SHARED
  src/odrive_usb.cpp
)

target_sources(odrive_usb PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/odrive_usb.hpp ${CMAKE_CURRENT_SOURCE_DIR}/include/odrive_usb.hpp)
target_include_directories(odrive_usb INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(
  odrive_usb PUBLIC
  ${LIBUSB_LIBRARY}
)

target_include_directories(
  odrive_usb PUBLIC
  ${LIBUSB_INCLUDE_DIR}
)

target_include_directories(odrive_usb PUBLIC include)