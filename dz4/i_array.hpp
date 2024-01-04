#pragma once

template<class T> class IArray {
public:
    ~IArray() = default;

    virtual size_t size() = 0;
    virtual void add(T item) = 0;
    virtual T& get(size_t index) = 0;
    virtual void add(T item, size_t index) = 0;
    virtual T remove(size_t index) = 0;
};
