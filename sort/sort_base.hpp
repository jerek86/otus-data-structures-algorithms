#pragma once

#include <cstdlib>
#include <ctime>
#include <cmath>
#include <chrono>
#include <iostream>
#include <iomanip>
#include <vector>

using namespace std::chrono;

class SortBase {
public:
    SortBase() = default;
    ~SortBase() = default;

    virtual void sort() = 0;
    virtual std::string algo_name() = 0;

    void init_random_array(size_t n) {
        constexpr unsigned SEED = 12345;

        std::srand(SEED);        
        array_.resize(n);

        for (size_t i = 0; i < n; ++i) {
            array_[i] = std::rand() % n;
        }

        reset();
    }

    void init_sorted_array(size_t n) {
        array_.resize(n);

        for (size_t i = 0; i < n; ++i) {
            array_[i] = i;
        }

        reset();
    }

    void init_reversed_array(size_t n) {
        array_.resize(n);

        for (size_t i = 0; i < n; ++i) {
            array_[i] = n - i - 1;
        }

        reset();
    }

    void init_10_percent_unsorted_array(size_t n) {
        init_sorted_array(n);

        constexpr unsigned SEED = 67890;
        std::srand(SEED);

        int unsort_count = n / 20;
        for (int i = 0; i < unsort_count; ++i) {
            swap(std::rand() % n, std::rand() % n);
        }

        reset();
    }

    void init_only_numbers_array(size_t n) {
        constexpr unsigned SEED = 7654;

        std::srand(SEED);        
        array_.resize(n);

        for (size_t i = 0; i < n; ++i) {
            array_[i] = std::rand() % 10;
        }

        reset();
    }

    long assignments() { return assignments_; }
    long comparisons() { return comparisons_; }

    void dump_array() {
        size_t w = static_cast<size_t>(std::log10(array_.size()) + 0.5);
        
        for (size_t i = 0; i < array_.size(); ++i) {
            std::cout << std::setw(w) << array_[i] << " ";
            if ((i & 0x7) == 0x7) {
                std::cout << std::endl;
            }
        }
        std::cout << std::endl;
    }

protected:
    void swap(size_t a, size_t b) {
        int temp = array_[a];
        array_[a] = array_[b];
        array_[b] = temp;

        assignments_ += 3;
    }

    void assign(int& a, int& b) {
        a = b;

        assignments_++;
    }

    bool more(int a, int b) {
        comparisons_++;

        return a > b;
    }

    bool moreq(int a, int b) {
        comparisons_++;

        return a >= b;
    }

    int binary_search(int value, int low, int high) {
        if (high <= low) {
            if (moreq(value, array_[low])) {
                return low + 1;
            }
            return low;
        }

        int mid = (low + high) / 2;
        if (moreq(value, array_[mid])) {
            return binary_search(value, mid + 1, high);
        }

        return binary_search(value, low, mid);
    }

    void reset() {
        assignments_ = 0;
        comparisons_ = 0;
    }

    int find_max() {
        int max = array_[0];
        for (size_t i = 1; i < array_.size(); ++i) {
            if (array_[i] > max) {
                max = array_[i];
            }
        }

        return max;
    }

    bool is_sorted() {
        for (int i = 1; i < array_.size(); ++i) {
            if (array_[i] < array_[i-1]) {
                return false;
            }
        }

        return true;
    }

    std::vector<int> array_;

    long assignments_;
    long comparisons_;
};

static void dump_result(SortBase& sort, std::string array_type, int count, int duration) {
	std::cout << std::setw(21) << sort.algo_name() << ", " << std::setw(25) << array_type << ", " << count <<
    " elements result: time = " << std::setw(6) << duration <<" ms, assignments = " << 
    std::setw(12) << sort.assignments() << " , comparisons = " << std::setw(11) << sort.comparisons() << std::endl;
}

static void test_once(SortBase&& sort, int count) {
    sort.init_random_array(count);
    auto start = high_resolution_clock::now();
    sort.sort();
	auto stop = high_resolution_clock::now();
	auto duration = duration_cast<milliseconds>(stop - start);
    dump_result(sort, "random array", count, duration.count());

    sort.init_sorted_array(count);
    start = high_resolution_clock::now();
    sort.sort();
	stop = high_resolution_clock::now();
	duration = duration_cast<milliseconds>(stop - start);
    dump_result(sort, "sorted array", count, duration.count());

    sort.init_reversed_array(count);
    start = high_resolution_clock::now();
    sort.sort();
	stop = high_resolution_clock::now();
	duration = duration_cast<milliseconds>(stop - start);
    dump_result(sort, "reversed array", count, duration.count());

    sort.init_10_percent_unsorted_array(count);
    start = high_resolution_clock::now();
    sort.sort();
	stop = high_resolution_clock::now();
	duration = duration_cast<milliseconds>(stop - start);
    dump_result(sort, "10 percent unsorted array", count, duration.count());

    sort.init_only_numbers_array(count);
    start = high_resolution_clock::now();
    sort.sort();
	stop = high_resolution_clock::now();
	duration = duration_cast<milliseconds>(stop - start);
    dump_result(sort, "only numbers array", count, duration.count());
}
