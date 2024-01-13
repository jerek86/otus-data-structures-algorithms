#include "../sort/sort_base.hpp"

class QuickSort : public SortBase {
public:

    std::string algo_name() override { return "quick_sort"; }

    void sort() override {
        quick_sort(0, array_.size() - 1);
    }

private: 
    void quick_sort(int l, int r)
    {
        if (l >= r) {
            return;
        }
    
        int m = split(l, r);
        quick_sort(l, m - 1);
        quick_sort(m + 1, r);
    }

    int split(int l, int r) {
        int pivot = array_[r];
        int m = l - 1;

        for (int i = l; i <= r; ++i) {
            if (moreq(pivot, array_[i])) {
                swap(++m, i);
            }
        }

        return m;
    }

};

class QuickSortClassic : public SortBase {
public:

    std::string algo_name() override { return "quick_sort_classic"; }

    void sort() override {
        quick_sort(0, array_.size() - 1);
    }

private: 
    void quick_sort(int l, int r)
    {
        if (l >= r) {
            return;
        }
    
        int m = split(l, r);
        quick_sort(l, m);
        quick_sort(m + 1, r);
    }

    int split(int l, int r) {
        int pivot = array_[l];
 
        int i = l + 1;
        int j = r;

        while (1) {
            while (i <= r && more(pivot, array_[i])) {
                i++;
            }

            while (j >= i && moreq(array_[j], pivot)) {
                j--;
            }

            if (i >= j) {
                if (j != l) {
                    swap(l, j--);
                }
                return j;
            }
            swap(i++, j--);
        }
    }

};

class MergeSort : public SortBase {
public:

    std::string algo_name() override { return "merge_sort"; }

    void sort() override {
        temp_.resize(array_.size());
        merge_sort(0, array_.size() - 1);
    }

private:
    void merge_sort(int l, int r) {
        if (l >= r) {
            return;
        }

        int m = (l + r) / 2;
        merge_sort(l, m);
        merge_sort(m + 1, r);
        merge(l, m, r);
    }

    void merge(int l, int m, int r) {
        int a = l;
        int b = m + 1;
        int t = l;

        while (a <= m && b <= r) {
            if (more(array_[a], array_[b])) {
                assign(temp_[t++], array_[b++]);
            } else {
                assign(temp_[t++], array_[a++]);
            }
        }

        while (a <= m) {
            assign(temp_[t++], array_[a++]);
        }

        while (b <= r) {
            assign(temp_[t++], array_[b++]);
        }

        for (int i = l; i<= r; ++i) {
            assign(array_[i], temp_[i]);
        }
    }

    std::vector<int> temp_;
};

void test(int count) {
    test_once(QuickSort{}, count);  
    test_once(MergeSort{}, count);
    test_once(QuickSortClassic{}, count);    
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
