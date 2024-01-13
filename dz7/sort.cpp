#include "../sort/sort_base.hpp"

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
