cmake_minimum_required(VERSION 3.26)
project(Ex5 C)

set(CMAKE_C_STANDARD 11)

add_executable(Ex5 peterson.c
        swap.c
)
