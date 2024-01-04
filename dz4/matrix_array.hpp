#pragma once

#include <cstring>
#include <stdexcept>

#include "i_array.hpp"
#include "single_array.hpp"
#include "vector_array.hpp"

template<class T> class MatrixArray : public IArray<T> {
public:
    MatrixArray(size_t vector) 
     : VECTOR_(vector), array_(), size_(0) {}

    MatrixArray() 
     : MatrixArray(10) {}

    size_t size() override {
        return size_;
    }

    void add(T item) override {
        if (size() == array_.size() * VECTOR_) {
            array_.add(VectorArray<T>(VECTOR_));
        }
        array_.get(size_ / VECTOR_).add(item);

        size_++;
    }

    T& get(size_t index) override {
        return array_.get(index / VECTOR_).get(index % VECTOR_);
    }

    void add(T item, size_t index) override {
        if (index > size()) {
            throw std::out_of_range("add: index > size()");
        }

        if (size() == array_.size() * VECTOR_) {
            array_.add(VectorArray<T>(VECTOR_));
        }

        array_.get(size_ / VECTOR_).add(array_.get((size_ - 1) / VECTOR_).get((size_ - 1) % VECTOR_));

        for (size_t dst_index = size_ - 1; dst_index > index; --dst_index) {
            size_t src_index = dst_index - 1;
            array_.get(dst_index / VECTOR_).get(dst_index % VECTOR_) = array_.get(src_index / VECTOR_).get(src_index % VECTOR_);
        }

        array_.get(index / VECTOR_).get(index % VECTOR_) = item;
        size_++;
    }
    T remove(size_t index) override {
        if (index >= size()) {
            throw std::out_of_range("remove: index >= size()");
        }

        T result = get(index);
        for (size_t i = index + 1; i < size_; ++i) {
            size_t new_index = i - 1;
            array_.get(new_index / VECTOR_).get(new_index % VECTOR_) = array_.get(i / VECTOR_).get(i % VECTOR_);
        }

        size_--;
        array_.get(size_ / VECTOR_).remove(size_ % VECTOR_);

        return result;
    }

private:
    void dump() {
        for (size_t i = 0; i < array_.size(); ++i) {
            std::cout << i << ": ";
            for (size_t j = 0; j < array_.get(i).size(); ++j) {
                std::cout << array_.get(i).get(j) << " ";
            }
            std::cout << std::endl;
        }
        std::cout << "size_ = " << size_ << std::endl;
    }

    const size_t VECTOR_;
    
    SingleArray<VectorArray<T>> array_;
    size_t size_;
};
