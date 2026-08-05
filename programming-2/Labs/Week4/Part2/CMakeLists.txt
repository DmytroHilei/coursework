cmake_minimum_required(VERSION 3.31)
project(Part2 C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(Part2
        Functions_part2.c
        Header_Part2.h
        Part2.c)
