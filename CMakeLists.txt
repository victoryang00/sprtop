cmake_minimum_required(VERSION 3.18)

project(strtop_core)

set(CMAKE_CXX_STANDARD 23)
find_package(Threads REQUIRED)
find_package(pybind11 REQUIRED CONFIG)
pybind11_add_module(strtop_core sprtop/strtop_core.cpp)
target_link_libraries(strtop_core PRIVATE pybind11::module Threads::Threads)
include_directories(${CMAKE_SOURCE_DIR}/lib/pcm/src)
add_subdirectory(${CMAKE_SOURCE_DIR}/lib/pcm/)
