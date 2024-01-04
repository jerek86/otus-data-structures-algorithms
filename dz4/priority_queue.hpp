#pragma once

#include "single_array.hpp"
#include "vector_array.hpp"

template<class T> class PriorityQueue {
public:
    PriorityQueue() = default;

    void enqueue(int priority, T item) {
        size_t index = get_index(priority);
        elements_.get(index).add(item);
    }

    T dequeue() {
        if (elements_.size() == 0) {
            throw std::out_of_range("queue is empty");
        }

        T result = elements_.get(0).get(0);
        elements_.get(0).remove(0);
        if (elements_.get(0).size() == 0) {
            elements_.remove(0);
            priorities_.remove(0);
        }

        return result;
    }

    void dump() {
        for (size_t i = 0; i < priorities_.size(); ++i) {
            std::cout << priorities_.get(i) << ": ";
            for (size_t j = 0; j < elements_.get(i).size(); ++j) {
                std::cout << elements_.get(i).get(j) << " ";
            }
            std::cout << std::endl;
        }
    }

private:
    size_t get_index(int priority) {
        size_t cur_priority_index = 0;
        for (; cur_priority_index < priorities_.size(); ++cur_priority_index) {
            int cur_priority = priorities_.get(cur_priority_index);
            if (cur_priority == priority) {
                return cur_priority_index;
            }
            if (cur_priority > priority) {
                break;
            }
        }
        priorities_.add(priority, cur_priority_index);
        elements_.add(VectorArray<int>{}, cur_priority_index);

        return cur_priority_index;
    }

    SingleArray<int> priorities_;
    SingleArray<VectorArray<int>> elements_;
};
