cmake_minimum_required(VERSION 3.18)
project(httpspeedtest)

set(CMAKE_CXX_STANDARD 20)

find_package(cpr CONFIG REQUIRED)

add_executable(httpspeedtest main.cpp argshandler.hpp)

target_link_libraries(httpspeedtest PRIVATE cpr)