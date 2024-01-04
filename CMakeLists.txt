cmake_minimum_required(VERSION 2.8)

project(otus_data_structures_and_algorithms)

include_directories(tester)

add_library(tester STATIC tester/tester.cpp)

add_executable(dz2 dz2/dz2.cpp)
target_link_libraries(dz2 tester)

add_executable(dz3_power dz3/power.cpp)
target_link_libraries(dz3_power tester)

add_executable(dz3_fibonacci dz3/fibonacci.cpp)
target_link_libraries(dz3_fibonacci tester)

add_executable(dz3_prime dz3/prime.cpp)
target_link_libraries(dz3_prime tester)

add_executable(dz4 dz4/main.cpp)
