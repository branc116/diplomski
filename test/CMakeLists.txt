cmake_minimum_required(VERSION 3.16)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")
project(server C CXX ASM)

add_executable(server bluetooth_server.cpp)
target_link_options(server PUBLIC bluetooth)

