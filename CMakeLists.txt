cmake_minimum_required(VERSION 3.18)

project(sprtop_core)

set(CMAKE_CXX_STANDARD 23)
find_package(Threads REQUIRED)
find_package(pybind11 REQUIRED CONFIG)
pybind11_add_module(sprtop_core sprtop/sprtop_core.cpp)
target_link_libraries(sprtop_core PRIVATE pybind11::module Threads::Threads)
add_executable(sprtop_test sprtop/sprtop_test.cpp)
target_link_libraries(sprtop_test PRIVATE pybind11::module Threads::Threads)
include_directories(lib/pcm/src)
add_subdirectory(lib/pcm/)
