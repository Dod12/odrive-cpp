# Setup
cmake_minimum_required(VERSION 3.23)
set(CMAKE_CXX_STANDARD 17)
project(odrive-cpp VERSION 1.0.0 DESCRIPTION "Simple C++ ODrive driver for controlling motors" LANGUAGES CXX)
include(GNUInstallDirs)

# Library
add_library(odrive SHARED src/odrive.cpp include/find_device.h)
target_compile_definitions(odrive PUBLIC ODrive)
target_include_directories(odrive PUBLIC extern/fibre/include)

add_subdirectory(extern/odrive-hardware-control/)
set_target_properties(odrive PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(odrive PROPERTIES SOVERSION 1)

# Create public API and private headers
set_target_properties(odrive PROPERTIES PUBLIC_HEADER include/odrive.h)
target_sources(odrive PUBLIC include/odrive.h)
target_include_directories(odrive PUBLIC include)
target_include_directories(odrive INTERFACE include)
target_include_directories(odrive PRIVATE src)

# Link external library
target_link_libraries(odrive PUBLIC odrive_usb)
target_include_directories(odrive PUBLIC odrive_usb)

install(TARGETS odrive
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

# Test Executable
add_executable(test test/main.cpp)
target_link_libraries(test PUBLIC odrive)