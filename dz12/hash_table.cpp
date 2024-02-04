#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <list>
#include <optional>
#include <vector>

using namespace std;

template<class T> class HashTable {
public:
    HashTable(int default_size = 10, float load_factor = 0.75f)
     : load_factor_(load_factor) {
        array_.resize(default_size);
    }

    void put(const string& key, const T& item) {
        array_[get_index(key)].push_back(TableItem{key, item});
        rehash();
    }

    optional<T> get(const string& key) {
        auto& list = array_[get_index(key)];
        for (auto& item : list) {
            if (item.key == key) {
                return item.item;
            }
        }

        return nullopt;
    }

    void del(const string& key) {
        auto& list = array_[get_index(key)];
        for (auto it = list.begin(); it != list.end(); ++it) {
            if (it->key == key) {
                list.erase(it);
                return;
            }
        }        
    }

    void dump() {
        int i = 0;
        for (auto& cell : array_) {
            cout << "cell " << i++ << " :" << endl;
            for (auto& item : cell) {
                cout << "  " << item.key << " : " << item.item << endl;
            }
        }
    }

private:
    size_t get_index(const string& key) {
        return hash<string>{}(key) % array_.size();
    }
    size_t filled_cell_count() {
        size_t filled_cell_count = 0;

        for (auto& cell : array_) {
            if (!cell.empty()) filled_cell_count++;
        }

        return filled_cell_count;
    }
    size_t filled_cell_max_count() {
        return load_factor_ * array_.size();
    }

    void rehash() {
        if (filled_cell_count() < filled_cell_max_count()) {
            return;
        }

        vector<list<TableItem>> temp;
        temp.resize(array_.size() * 2);
        swap(array_, temp);
        for (auto& cell : temp) {
            for (auto& item : cell) {
                array_[get_index(item.key)].push_back(TableItem{item.key, item.item});
            }
        }
    }

    const float load_factor_;

    struct TableItem {
        string key;
        T item;
    };
    vector<list<TableItem>> array_;
};

int main() {
    HashTable<int> ht;

    for (int i = 0; i < 10; ++i) {
        ht.put(to_string(i), i);
    }
    ht.dump();

    return 0;
}