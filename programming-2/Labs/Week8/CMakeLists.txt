cmake_minimum_required(VERSION 3.31)
project(Week9 C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(Week9
        FileReading.c
        Header.h
        "main.c")
