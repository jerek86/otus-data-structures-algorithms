#pragma once

#include <cstring>
#include <stdexcept>

#include "i_array.hpp"

template<class T> class FactorArray : public IArray<T> {
public:
    FactorArray(int factor, int init_length) 
     : FACTOR_(static_cast<double>(factor) / 100), array_size_(init_length), size_(0) {
        array_ = new T [array_size_];
    }

    FactorArray() 
     : FactorArray(50, 10) {}

    ~FactorArray() {
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
            const size_t increment = static_cast<size_t>(array_size_ * FACTOR_);

            T* newArray = new T[array_size_  + increment];
            for (size_t i = 0; i < index; ++i) newArray[i] = array_[i];
            for (size_t i = index; i < size_; ++i) newArray[i + 1] = array_[i];
            delete [] array_;
            array_ = newArray;
            array_size_ += increment;
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

        for (size_t i = index; i <= size_; ++i) array_[i] = array_[i + 1];
        size_--;

        return result;
    }

private:
    void resize() {
        const size_t increment = static_cast<size_t>(array_size_ * FACTOR_);
        T* newArray = new T[array_size_ + increment];
        for (size_t i = 0; i < array_size_; ++i) newArray[i] = array_[i];
        delete [] array_;
        array_ = newArray;
        array_size_ += increment;
    }

    const double FACTOR_;
    
    T* array_;
    size_t array_size_;
    size_t size_;
};
