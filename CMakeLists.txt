cmake_minimum_required(VERSION 3.18)

project(sprtop_core)

set(CMAKE_CXX_STANDARD 26)
find_package(Threads REQUIRED)
find_package(pybind11 REQUIRED CONFIG)
pybind11_add_module(sprtop_core lib/sprtop_core.cpp)
target_link_libraries(sprtop_core PRIVATE pybind11::module Threads::Threads)