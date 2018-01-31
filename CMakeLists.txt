project(cafetine)

cmake_minimum_required(VERSION 3.1)

set(CMAKE_CXX_STANDARD 14)

add_library(example_person SHARED person.cpp)

add_library(super_secret SHARED secret.c)

add_executable(example1 main.cpp)

target_link_libraries(example1 example_person super_secret)
