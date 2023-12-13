cmake_minimum_required(VERSION 2.8)

project(otus_data_structures_and_algorithms)

include_directories(tester)

add_library(tester STATIC tester/tester.cpp)

add_executable(dz2 dz2/dz2.cpp)
target_link_libraries(dz2 tester)