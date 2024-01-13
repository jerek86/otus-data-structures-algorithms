#include "../sort/sort_base.hpp"

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
