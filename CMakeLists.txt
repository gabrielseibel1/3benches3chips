cmake_minimum_required(VERSION 3.9)
project(benches C)

set(CMAKE_C_STANDARD 99)

add_executable(convolution src/convolution.c)
add_executable(fakeparser src/fakeparser.c)
add_executable(kmeans src/kmeans.c)