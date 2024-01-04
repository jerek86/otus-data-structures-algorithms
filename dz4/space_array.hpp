#pragma once

#include <cstring>
#include <stdexcept>

#include "i_array.hpp"
#include "single_array.hpp"
#include "vector_array.hpp"

template<class T> class SpaceArray : public IArray<T> {
public:
    SpaceArray(size_t vector, size_t filling) 
     : VECTOR_(vector), FILLING_(filling), array_(), size_(0) {
        if (FILLING_ >= VECTOR_) {
            throw std::out_of_range("FILLING_ >= VECTOR_");
        }
        array_.add(VectorArray<T>(VECTOR_));
    }

    SpaceArray() 
     : SpaceArray(20, 10) {}

    size_t size() override {
        return size_;
    }

    void add(T item) override {
        size_t last_not_filled_array_index = array_.size() - 1;
        for (; last_not_filled_array_index > 0; ++last_not_filled_array_index) {
            if (array_.get(last_not_filled_array_index).size() != 0) {
                break;
            }
        }

        if (array_.get(last_not_filled_array_index).size() >= FILLING_) {
            last_not_filled_array_index++;
            if (last_not_filled_array_index >= array_.size()) {
                array_.add(VectorArray<T>(VECTOR_));
            }
        }

        array_.get(last_not_filled_array_index).add(item);

        size_++;
    }

    T& get(size_t index) override {
        if (index >= size()) {
            throw std::out_of_range("get: index >= size()");
        }

        size_t counter = 0;
        size_t vector_index = 0;
        for (; vector_index < array_.size(); ++vector_index) {
            if (counter + array_.get(vector_index).size() > index) {
                break;
            }
            counter += array_.get(vector_index).size();
        }
        return array_.get(vector_index).get(index - counter);
    }

    void add(T item, size_t index) override {
        if (index > size()) {
            throw std::out_of_range("add: index > size()");
        }

        size_t counter = 0;
        size_t vector_index = 0;
        for (; vector_index < array_.size(); ++vector_index) {
            if (counter + array_.get(vector_index).size() > index) {
                break;
            }
            counter += array_.get(vector_index).size();
        }

        if (array_.get(vector_index).size() >= VECTOR_) {
            array_.add(VectorArray<T>(VECTOR_), vector_index);
            for (size_t j = 0; j < FILLING_; ++j) {
                array_.get(vector_index).add(array_.get(vector_index + 1).remove(0));
            }

            if (counter + array_.get(vector_index).size() <= index) {
                counter += array_.get(vector_index).size();
                vector_index++;
            }
        }

        array_.get(vector_index).add(item, index - counter);

        size_++;
    }
    T remove(size_t index) override {
        if (index >= size()) {
            throw std::out_of_range("remove: index >= size()");
        }

        size_t counter = 0;
        size_t vector_index = 0;
        for (; vector_index < array_.size(); ++vector_index) {
            if (counter + array_.get(vector_index).size() > index) {
                break;
            }
            counter += array_.get(vector_index).size();
        }

        T result = array_.get(vector_index).remove(index - counter);
        if (array_.get(vector_index).size() == 0) {
            array_.remove(vector_index);
        }

        size_--;

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
    const size_t FILLING_;
    
    SingleArray<VectorArray<T>> array_;
    size_t size_;
};
