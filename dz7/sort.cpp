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

        return a > b;
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

    std::vector<int> array_;

    long assignments_;
    long comparisons_;
};

class SelectionSort : public SortBase {
public:

    std::string algo_name() override { return "selection_sort"; }

    void sort() override {
        for (int i = array_.size() - 1; i > 0; --i) {
            int max = find_max(i);
            if (max != i) {
                swap(max, i);
            }
        }
    }

private:
    size_t find_max(int n) {
        size_t max_index = 0;
        for (int i = 1; i <= n; ++i) {
            if (more(array_[i], array_[max_index])) {
                max_index = i;
            }
        }

        return max_index;
    }
};

class HeapSort : public SortBase {
public:

    std::string algo_name() override { return "heap_sort"; }

    void sort() override {
        for (int h = (array_.size() / 2) - 1; h >= 0; --h) {
            heapify(h, array_.size());
        }

        for (int i = array_.size() - 1; i > 0; --i) {
            swap(0, i);
            heapify(0, i);
        }
    }

private:
    void heapify(int root, int size) {
        int p = root;
        int l = (2 * p) + 1;
        int r = l + 1;

        if (l < size && more(array_[l], array_[p])) {
            p = l;
        }
        if (r < size && more(array_[r], array_[p])) {
            p = r;
        }

        if (p == root) {
            return;
        }

        swap(root, p);
        heapify(p, size);
    }
};

void dump_result(SortBase& sort, std::string array_type, int count, int duration) {
	std::cout << std::setw(21) << sort.algo_name() << ", " << std::setw(25) << array_type << ", " << count <<
    " elements result: time = " << std::setw(6) << duration <<" ms, assignments = " << 
    std::setw(12) << sort.assignments() << " , comparisons = " << std::setw(11) << sort.comparisons() << std::endl;
}

void test_once(SortBase&& sort, int count) {
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

void test(int count) {
    test_once(SelectionSort{}, count);  
    test_once(HeapSort{}, count);  
}

int main(int argc, char** argv)
{
    if (argc < 2) {
        std::cout << "array size not specifed" << std::endl;
        return 0; 
    }

    int count = std::atoi(argv[1]);
    
    test(count);

    return 0;
}
