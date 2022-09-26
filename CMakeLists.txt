# Setup
cmake_minimum_required(VERSION 3.22)
set(CMAKE_CXX_STANDARD 17)
project(odrive-lib VERSION 1.0.0 DESCRIPTION "Simple C++ library for interacting with ODrive controller." LANGUAGES CXX)

find_path(LIBUSB_INCLUDE_DIR
        PATHS /usr/local/include /usr/include extern/libusb/include
        NAMES "libusb.h"
        PATH_SUFFIXES "libusb" "libusb-1.0")

find_library(LIBUSB_LIBRARY
        PATHS /usr/local/lib /usr/lib extern/libusb/lib
        NAMES "libusb-1.0.0.dylib" "libusb.dylib" "libusb.1.dylib"
        PATH_SUFFIXES "libusb" "libusb-1.0")

message(STATUS ${LIBUSB_INCLUDE_DIR})
message(STATUS ${LIBUSB_LIBRARY})

add_library(odrive_lib SHARED src/odrive.cpp)
set_target_properties(odrive_lib PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(odrive_lib PROPERTIES SOVERSION 1)
target_compile_options(odrive_lib PRIVATE -Wall)

set_target_properties(odrive_lib PROPERTIES PUBLIC_HEADER include/odrive.h include/odrive_enums.h include/odrive_endpoints.h)
target_sources(odrive_lib INTERFACE include/odrive.h include/odrive_enums.h include/odrive_endpoints.h)
target_include_directories(odrive_lib PUBLIC include)
target_include_directories(odrive_lib PRIVATE src)

target_link_libraries(odrive_lib PUBLIC ${LIBUSB_LIBRARY})
target_include_directories(odrive_lib PUBLIC ${LIBUSB_INCLUDE_DIR})

install(TARGETS odrive_lib)

add_executable(odrive_test test/main.cpp)
target_link_libraries(odrive_test PRIVATE odrive_lib)
target_link_libraries(odrive_test PRIVATE ${LIBUSB_LIBRARY})

