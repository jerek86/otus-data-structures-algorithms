#include <chrono>
#include <iostream>

#include "single_array.hpp"
#include "vector_array.hpp"
#include "factor_array.hpp"
#include "matrix_array.hpp"
#include "std_vector_wrapper.hpp"
#include "priority_queue.hpp"
#include "space_array.hpp"

using namespace std::chrono;

void print(IArray<int>& data) {
    for (size_t i = 0; i < data.size(); ++i) {
        std::cout << data.get(i) << " ";
    }
    std::cout << std::endl;
}
bool array_is_valid(IArray<int>& data, const std::string& name) {
    int reference[5] = {0, 1, 10, 2, 4};

    for (int j = 0; j < 5; j++)
        data.add(j);

    print(data); // 0 1 2 3 4

    data.add(10, 2);

    print(data); // 0 1 10 2 3 4

    data.remove(4);

    print(data); // 0 1 10 2 4

    for (size_t j = 0; j < 5; ++j) {
        if (reference[j] != data.get(j)) {
            std::cout << name << " works incorrect" << std::endl;
            return false;
        }
    }

    std::cout << name << " works correct" << std::endl;
    return true;
}

void test_array(IArray<int>& data, int total, const std::string& name) {
    auto start = high_resolution_clock::now();

    for (int j = 0; j < total; j++)
        data.add(j);

	auto stop = high_resolution_clock::now();
	auto duration = duration_cast<milliseconds>(stop - start);

	std::cout << name << " test add to end: " << duration.count() << " ms" << std::endl;


    start = high_resolution_clock::now();

    for (int j = 0; j < total; j++)
        data.add(j, 2*j);

	stop = high_resolution_clock::now();
	duration = duration_cast<milliseconds>(stop - start);

	std::cout << name << " test add to middle: " << duration.count() << " ms" << std::endl;

    start = high_resolution_clock::now();

    for (int j = 0; j < total; j++)
        data.remove(j);

	stop = high_resolution_clock::now();
	duration = duration_cast<milliseconds>(stop - start);

	std::cout << name << " test remove from middle: " << duration.count() << " ms" << std::endl;
}

void test_priority_queue() {
    PriorityQueue<int> priority_queue;

    priority_queue.dump();

    priority_queue.enqueue(10, 10);
    std::cout << "enqueue(10, 10): " << std::endl;
    priority_queue.dump();

    priority_queue.enqueue(7, 11);
    std::cout << "enqueue(7, 11): " << std::endl;
    priority_queue.dump();

    priority_queue.enqueue(10, 12);
    std::cout << "enqueue(10, 12): " << std::endl;
    priority_queue.dump();

    int i = priority_queue.dequeue();
    std::cout << "dequeue result: " << i << std::endl;
    priority_queue.dump();

    i = priority_queue.dequeue();
    std::cout << "dequeue result: " << i << std::endl;
    priority_queue.dump();
}

int main(int argc, char** argv)
{
    SingleArray<int> singleArray{};
    VectorArray<int> vectorArray{};
    FactorArray<int> factorArray{};
    MatrixArray<int> matrixArray{};
    StdVectorWrapperArray<int> stdVectorWrapperArray{};
    SpaceArray<int> space_array{};

    // array_is_valid(singleArray, "singleArray");
    // array_is_valid(vectorArray, "vectorArray");
    // array_is_valid(factorArray, "factorArray");
    // array_is_valid(matrixArray, "matrixArray");
    // array_is_valid(stdVectorWrapperArray, "stdVectorWrapperArray");
    // array_is_valid(space_array, "space_array");
    
    test_array(space_array, 100000, "space_array");

    size_t count = 100000;

    // test_array(singleArray, count, "singleArray");
    // test_array(vectorArray, count, "vectorArray");
    // test_array(factorArray, count, "factorArray");
    // test_array(matrixArray, count, "matrixArray");
    // test_array(stdVectorWrapperArray, count, "stdVectorWrapperArray");

    return 0;
}
