# Setup
cmake_minimum_required(VERSION 3.22)
set(CMAKE_CXX_STANDARD 17)
project(odrive-usb VERSION 1.0.0 LANGUAGES CXX)

find_path(LIBUSB_INCLUDE_DIR
          PATHS /usr/local/include /usr/include ./libusb/include
          NAMES "libusb.h"
          PATH_SUFFIXES "libusb" "libusb-1.0")

find_library(LIBUSB_LIBRARY
             PATHS /usr/local/lib /usr/lib ./libusb/lib
             NAMES "libusb-1.0.0.dylib" "libusb.dylib" "libusb.1.dylib"
             PATH_SUFFIXES "libusb" "libusb-1.0")       


add_library(odrive_usb SHARED src/odrive_usb.cpp)
set_target_properties(odrive_usb PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(odrive_usb PROPERTIES SOVERSION 1)

set_target_properties(odrive_usb PROPERTIES PUBLIC_HEADER include/odrive_usb.hpp)
set_target_properties(odrive_usb PROPERTIES PUBLIC_HEADER include/odrive_endpoints.hpp)
target_sources(odrive_usb INTERFACE include/odrive_usb.hpp include/odrive_endpoints.hpp)
target_include_directories(odrive_usb PUBLIC include)
target_include_directories(odrive_usb PRIVATE src)
target_include_directories(odrive_usb PUBLIC ${LIBUSB_INCLUDE_DIR})
target_link_libraries(odrive_usb PUBLIC ${LIBUSB_LIBRARY})
