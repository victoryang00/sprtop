cmake_minimum_required(VERSION 3.18)

project(strtop_pybind11)

set(CMAKE_CXX_STANDARD 14)

find_package(pybind11 REQUIRED CONFIG)
pybind11_add_module(strtop_pybind11 strtop.cpp)