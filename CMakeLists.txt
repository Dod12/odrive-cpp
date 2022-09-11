cmake_minimum_required(VERSION 3.23)
project(odrive_cpp)

set(CMAKE_CXX_STANDARD 17)

add_library(odrive_cpp src/library.cpp)
