#pragma once

#include <cstring>
#include <stdexcept>

#include "i_array.hpp"

template<class T> class VectorArray : public IArray<T> {
public:
    VectorArray(size_t vector) 
     : VECTOR_(vector), array_size_(0), size_(0) {
        array_ = new T [array_size_];
    }

    VectorArray() 
     : VectorArray(10) {}

    VectorArray(const VectorArray& other) {
        operator=(other);
    }

    ~VectorArray() {
        if (array_) {
            delete [] array_;
        }
    }

    size_t size() override {
        return size_;
    }

    void add(T item) override {
        if (size() == array_size_) {
            resize();
        }
        array_[size()] = item;
        size_++;
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

        if (size() == array_size_) {
            T* newArray = new T[array_size_ + VECTOR_];
            for (size_t i = 0; i < index; ++i) newArray[i] = array_[i];
            for (size_t i = index; i < size_; ++i) newArray[i + 1] = array_[i];
            delete [] array_;
            array_ = newArray;
            array_size_ += VECTOR_;
        } else {
            for (size_t i = size_; i > index ; --i) array_[i] = array_[i - 1];
        }

        array_[index] = item;
        size_++;
    }
    T remove(size_t index) override {
        if (index >= size()) {
            throw std::out_of_range("remove: index >= size()");
        }

        T result = get(index);

        for (size_t i = index; i < size_; ++i) array_[i] = array_[i + 1];
        size_--;

        return result;
    }

    VectorArray& operator=(const VectorArray& other) {
        if (&other == this) {
            return *this;
        }

        VECTOR_ = other.VECTOR_;
        array_size_ = other.array_size_;
        size_ = other.size_;

        array_ = new T [array_size_];
        for (size_t i = 0; i < array_size_; ++i) array_[i] = other.array_[i];
    
        return *this;
    }
private:
    void resize() {
        T* newArray = new T[array_size_ + VECTOR_];
        for (size_t i = 0; i < array_size_; ++i) newArray[i] = array_[i];
        delete [] array_;
        array_ = newArray;
        array_size_ += VECTOR_;
    }

    size_t VECTOR_;
    
    T* array_;
    size_t array_size_;
    size_t size_;
};
