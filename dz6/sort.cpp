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

class BubbleSort : public SortBase {
public:

    std::string algo_name() override { return "bubble_sort"; }

    void sort() override {
        for (int i = array_.size() - 1; i > 0; --i) {
            for (int j = 0; j < i; ++j) {
                if (more(array_[j], array_[j + 1])) {
                    swap(j, j + 1);
                }
            }
        }
    }
};

class BubbleSortOptimized : public SortBase {
public:

    std::string algo_name() override { return "bubble_sort_optimized"; }

    void sort() override {
        int begin = -1;
        int end = array_.size() - 2;
        bool swapped = true;

        while (swapped) {
            swapped = false;
            begin++;
            
            for (int i = begin; i <= end; ++i) {
                if (more(array_[i], array_[i + 1])) {
                    swap(i, i + 1);
                    swapped = true;
                }
            }

            if (!swapped) {
                break;
            }

            swapped = false;
            end--;

            for (int i = end; i >= begin; --i) {
                if (more(array_[i], array_[i + 1])) {
                    swap(i, i + 1);
                    swapped = true;
                }
            }
        }
    }
};

class InsertionSort : public SortBase {
public:

    std::string algo_name() override { return "insertion_sort"; }

    void sort() override {
        for (int i = 1; i < array_.size(); ++i) {
            for (int j = i - 1; j >= 0; --j) {
                if (!more(array_[j], array_[j + 1])) {
                    break;
                }
                swap(j, j + 1);
            }
        }
    }
};

class InsertionShiftSort : public SortBase {
public:

    std::string algo_name() override { return "insertion_shift_sort"; }

    void sort() override {
        for (int i = 1; i < array_.size(); ++i) {
            int j = i - 1;
            int temp;
            assign(temp, array_[i]);
            for (; j >= 0; --j) {
                if (!more(array_[j], temp)) {
                    break;
                }
                assign(array_[j + 1], array_[j]);
            }
            assign(array_[j + 1], temp);
        }
    }
};

class InsertionBinarySort : public SortBase {
public:

    std::string algo_name() override { return "insertion_binary_sort"; }

    void sort() override {
        for (int i = 1; i < array_.size(); ++i) {
            int temp;
            assign(temp, array_[i]);
            int p = binary_search(temp, 0, i - 1);
            int j = i - 1;
            for (; j >= p; --j) {
                assign(array_[j + 1], array_[j]);
            }
            assign(array_[j + 1], temp);
        }
    }
};

class ShellSort : public SortBase {
public:

    std::string algo_name() override { return "shell_sort"; }

    void sort() override {
        for (int gap = array_.size() / 2; gap > 0; gap /= 2) {
            for (int i = gap; i < array_.size(); ++i) {
                for (int j = i; j >= gap; j -= gap) {
                    if (!more(array_[j - gap], array_[j])) {
                        break;
                    }
                    swap(j - gap, j);
                }
            }
        }
    }
};

class Shell1Sort : public SortBase {
public:

    std::string algo_name() override { return "shell1_sort"; }

    void sort() override {
        // for max gap = array_.size() / 2; gap = 2^k - 1
        int max_k = std::log2(array_.size() + 1);

        for (int k = max_k; k > 0; --k) {
            int gap = std::pow(2, k) - 1;
            for (int i = gap; i < array_.size(); ++i) {
                int j = i;
                int temp;
                assign(temp, array_[j]);
                for (; j >= gap; j -= gap) {
                    if (!more(array_[j - gap], temp)) {
                        break;
                    }
                    assign(array_[j], array_[j - gap]);
                }
                assign(array_[j], temp);
            }
        }
    }
};

class Shell2Sort : public SortBase {
public:

    std::string algo_name() override { return "shell2_sort"; }

    void sort() override {
        // gap = Successive numbers of the form 2^q*3^p
        
        std::vector<int> smooth_numbers = get_3_smooth_numbers(array_.size());
        
        for (int k = smooth_numbers.size() - 1; k >= 0; --k) {
            int gap = smooth_numbers[k];            
            for (int i = gap; i < array_.size(); ++i) {
                int j = i;
                int temp;
                assign(temp, array_[j]);
                for (; j >= gap; j -= gap) {
                    if (!more(array_[j - gap], temp)) {
                        break;
                    }
                    assign(array_[j], array_[j - gap]);
                }
                assign(array_[j], temp);
            }
        }
    }

private:
    std::vector<int> get_3_smooth_numbers(int n) {
        std::vector<int> smooth_numbers;
        
        int q_mul = 1;
        while (q_mul < n) {
            int p_mul = 1;

            while (1) {
                int total_mul = q_mul * p_mul;
                if (total_mul > n) {
                    break;
                }
                smooth_numbers.emplace_back(total_mul);
                p_mul *= 3;
            }
            q_mul *= 2;
        }

        for (int gap = smooth_numbers.size() / 2; gap > 0; gap /= 2) {
            for (int i = gap; i < smooth_numbers.size(); ++i) {
                int j = i;
                int temp = smooth_numbers[j];
                for (; j >= gap; j -= gap) {
                    if (smooth_numbers[j - gap] <= temp) {
                        break;
                    }
                    smooth_numbers[j] = smooth_numbers[j - gap];
                }
                smooth_numbers[j] = temp;
            }
        }

        return smooth_numbers;
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
    test_once(BubbleSort{}, count);  
    test_once(BubbleSortOptimized{}, count);  

    test_once(InsertionSort{}, count);  
    test_once(InsertionShiftSort{}, count);  
    test_once(InsertionBinarySort{}, count);  

    test_once(ShellSort{}, count);  
    test_once(Shell1Sort{}, count);  
    test_once(Shell2Sort{}, count);  
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
