cmake_minimum_required(VERSION 3.10)
project(state_machine_module C)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_C_COMPILER "clang")

add_subdirectory(src)
add_subdirectory(test)


