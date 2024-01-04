#pragma once

#include <vector>

#include "i_array.hpp"

template<class T> class StdVectorWrapperArray : public IArray<T> {
public:
    StdVectorWrapperArray() 
     : vector_() {}

    size_t size() override {
        return vector_.size();
    }

    void add(T item) override {
        vector_.emplace_back(item);
    }

    T& get(size_t index) override {
        if (index >= size()) {
            throw std::out_of_range("get: index >= size()");
        }
        return vector_[index];
    }

    void add(T item, size_t index) override {
        if (index > size()) {
            throw std::out_of_range("add: index > size()");
        }
        vector_.insert(vector_.begin() + index, item);
    }
    T remove(size_t index) override {
        if (index >= size()) {
            throw std::out_of_range("remove: index >= size()");
        }

        T result = get(index);

        vector_.erase(vector_.begin() + index);

        return result;
    }

private:
    std::vector<T> vector_;
};
