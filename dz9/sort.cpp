#include "../sort/sort_base.hpp"

class CountingSort : public SortBase {
public:

    std::string algo_name() override { return "counting_sort"; }

    void sort() override {
        int max = find_max();

        std::vector<int> counts(max + 1, 0);
        for (int a : array_) {
            counts[a]++;
        }

        for (int i = 1; i <= max; ++i) {
            counts[i] += counts[i - 1];
        }

        std::vector<int> sorted_array(array_.size());

        for (int i = array_.size() - 1; i >= 0; --i)
        {
            sorted_array[--counts[array_[i]]] = array_[i];
            assignments_++;
        }

        array_ = sorted_array;

        assignments_ += array_.size();
    }

private: 
};

class RadixSort : public SortBase {
public:

    std::string algo_name() override { return "radix_sort"; }

    void sort() override {
        int max = find_max() + 1;

        for (int byte_num = 0; (1 << byte_num*8) < max; ++byte_num) {
            count_sort(byte_num);
        }
    }

private: 
    size_t get_index(int num, int byte_num) {
        return (num >> 8 * byte_num) & 0xff;
    }
    
    void count_sort(int byte_num)
    {
        std::vector<int> counts(256, 0);

        for (int a : array_) {
            counts[get_index(a, byte_num)]++;
        }

        std::vector<int> sorted_array(array_.size());
        
        for (int i = 1; i < 256; ++i) {
            counts[i] += counts[i - 1];
        }
    
        for (int i = array_.size() - 1; i >= 0; i--) {
            sorted_array[--counts[get_index(array_[i], byte_num)]] = array_[i];
            assignments_++;
        }
    
        array_ = sorted_array;
        assignments_ += array_.size();
    }
};

class BucketSort : public SortBase {
public:

    std::string algo_name() override { return "bucket_sort"; }

    void sort() override {
        int max = find_max() + 1;

        ListNode** bucket = new ListNode*[array_.size()];
        for (size_t i = 0; i < array_.size(); ++i) {
            bucket[i] = nullptr;
        }
        
        for (int a : array_) {
            int nr = (long)a * (long)(array_.size()) / (long)max;            
            bucket[nr] = new ListNode(a, bucket[nr]);
            
            ListNode* item = bucket[nr];
            while (item->next) {
                if (more(item->next->value, item->value)) {
                    break;
                }
                int x;
                assign(x, item->value);
                assign(item->value, item->next->value);
                assign(item->next->value, x);

                item = item->next;
            }
        }

        size_t j = 0;
        for (size_t i = 0; i < array_.size(); ++i) {
            ListNode* item = bucket[i];
            while (item) {
                assign(array_[j++], item->value);
                item = item->next;
            }
        }

        // free memory
        for (size_t i = 1; i < array_.size(); ++i) {
            ListNode* l = bucket[i];
            while (l) {
                ListNode* next = l->next;
                delete l;
                l = next;
            }
        }

        delete [] bucket;
    }

private:
    struct ListNode {
        int value;
        ListNode* next;

        ListNode(int _value, ListNode* _next)
         : value(_value), next(_next) {}
    };
};

void test(int count) {
    test_once(CountingSort{}, count);  
    test_once(RadixSort{}, count);    
    test_once(BucketSort{}, count);
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
