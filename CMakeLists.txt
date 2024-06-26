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

add_executable(dz5 dz5/chess.cpp)
target_link_libraries(dz5 tester)

add_executable(dz6 dz6/sort.cpp)

add_executable(dz7 dz7/sort.cpp)

add_executable(dz8 dz8/sort.cpp)

add_executable(dz9 dz9/sort.cpp)

add_executable(dz10 dz10/tree.cpp)

add_executable(dz12 dz12/hash_table.cpp)

add_executable(dz13 dz13/trie.cpp)

add_executable(dz14 dz14/kosarayu.cpp)

add_executable(dz15 dz15/demukron.cpp)

add_executable(dz16 dz16/kraskal.cpp)

add_executable(dz17 dz17/dijkstra.cpp)

add_executable(dz18 dz18/substring_search.cpp)

add_executable(rle dz20/rle.cpp)

add_executable(one_two_pears dz21/one_two_pears.cpp)
add_executable(digital_tree dz21/digital_tree.cpp)
add_executable(5x8 dz21/5x8.cpp)
add_executable(islands dz21/islands.cpp)
