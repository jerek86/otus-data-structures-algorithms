#pragma once

#include <cstring>
#include <stdexcept>

#include "i_array.hpp"

template<class T> class SingleArray : public IArray<T> {
public:
    SingleArray() 
     : array_size_(0) {
        array_ = new T [array_size_];
    }

    ~SingleArray() {
        if (array_) {
            delete [] array_;
        }
    }

    size_t size() override {
        return array_size_;
    }

    void add(T item) override {
        resize();
        array_[size() - 1] = item;
    }

    T& get(size_t index) override {
        if (index >= size()) {
            throw std::out_of_range("get: index >= size()");
        }
        return array_[index];
    }

    void add(T item, size_t index) override {
        if (index > size()) {
            throw std::out_of_range("add: index > size()");
        }
        T* newArray = new T[size() + 1];
        for (size_t i = 0; i < index; ++i) newArray[i] = array_[i];
        for (size_t i = index; i < array_size_; ++i) newArray[i + 1] = array_[i];
        delete [] array_;
        array_ = newArray;
        array_size_++;

        array_[index] = item;
    }
    T remove(size_t index) override {
        if (index >= size()) {
            throw std::out_of_range("remove: index >= size()");
        }

        T result = get(index);

        T* newArray = new T[size() - 1];
        for (size_t i = 0; i < index; ++i) newArray[i] = array_[i];
        for (size_t i = index; i < array_size_; ++i) newArray[i] = array_[i+1];
        delete [] array_;
        array_ = newArray;
        array_size_--;

        return result;
    }

private:
    void resize() {
        T* newArray = new T[size() + 1];
        for (size_t i = 0; i < array_size_; ++i) newArray[i] = array_[i];
        delete [] array_;
        array_ = newArray;
        array_size_++;
    }

    T* array_;
    size_t array_size_;
};
