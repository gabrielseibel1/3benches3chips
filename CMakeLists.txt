cmake_minimum_required(VERSION 3.9)
project(benches C)

set(CMAKE_C_STANDARD 99)

add_executable(convolution convolution.c)