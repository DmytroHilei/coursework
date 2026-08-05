cmake_minimum_required(VERSION 4.1)
project(Week8 C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(Week8
        generator.c
        generator.h
        name_pools.h)
