#include <array>
#include <iostream>
#include <functional>
#include <optional>
#include <string>

using namespace std;

class Trie {
public:
    Trie() {
        root_ = new Node;
    }

    ~Trie() {
        clear_node(root_);
    }
    
    void insert(string word) {
        Node* node = root_;
        for (const auto& symbol : word) {
            if (node->children[index(symbol)] == nullptr) {
                node->children[index(symbol)] = new Node;
            }
            node = node->children[index(symbol)];
        }
        node->is_final = true;
    }

    bool search(string word) {
        Node* node = root_;
        for (const auto& symbol : word) {
            if (node->children[index(symbol)] == nullptr) {
                return false;
            }
            node = node->children[index(symbol)];
        }
        return node->is_final;
    }
    
    bool startsWith(string prefix) {
        Node* node = root_;
        for (const auto& symbol : prefix) {
            if (node->children[index(symbol)] == nullptr) {
                return false;
            }
            node = node->children[index(symbol)];
        }
        return true;
    }

private:
    static constexpr int index(char symbol) {
        return symbol - 'A';
    }
    static constexpr int ARRAY_SIZE = 'z' - 'A' + 1;

    struct Node {
        std::array<Node*,ARRAY_SIZE> children;
        bool is_final = false;

        Node() {
            for (auto& child : children) {
                child = nullptr;
            }
        }
    };

    static void clear_node(Node* node) {
        if (!node) {
            return;
        }

        for (auto& child : node->children) {
            clear_node(child);
        }

        delete node;
        node = nullptr;
    }

    Node* root_;
};


template<class T> class TrieContainer {
public:
    TrieContainer() {
        root_ = new Node;
    }

    ~TrieContainer() {
        clear_node(root_);
    }

    void put(const string& key, const T& item) {
        Node* node = root_;
        for (const auto& symbol : key) {
            if (node->children[index(symbol)] == nullptr) {
                node->children[index(symbol)] = new Node;
            }
            node = node->children[index(symbol)];
        }
        node->is_final = true;
        node->data = item;
    }

    optional<T> get(const string& key) {
        Node* node = root_;
        for (const auto& symbol : key) {
            if (node->children[index(symbol)] == nullptr) {
                return false;
            }
            node = node->children[index(symbol)];
        }
        return node->is_final ? node->data : nullopt;
    }

    void del(const string& key) {
        del(root_, key);
    }

    void dump() {
        dump(root_, string{}, 0);
    }

private:
    static constexpr int index(char symbol) {
        return symbol - 'A';
    }
    static constexpr int ARRAY_SIZE = 'z' - 'A' + 1;

    struct Node {
        std::array<Node*,ARRAY_SIZE> children;
        bool is_final = false;
        T data{};

        Node() {
            for (auto& child : children) {
                child = nullptr;
            }
        }

        bool has_children() {
            return any_of(children.begin(), children.end(), [] (const Node* node) {return node != nullptr;});
        }
    };

    bool del(Node* node, const string& key) {
        if (!node) {
            return false;
        }
        
        if (key.empty()) {
            if (node->has_children()) {
                node->is_final = false;
                return false;
            } else {
                return true;
            }
        } else {
            auto& child = node->children[index(key.front())];
            if (del(child, key.substr(1))) {
                delete child;
                child = nullptr;
                if (!node->has_children()) {
                    return true;
                }                
            }
        }

        return false;
    }

    static void clear_node(Node* node) {
        if (!node) {
            return;
        }

        for (auto& child : node->children) {
            clear_node(child);
        }

        delete node;
        node = nullptr;
    }

    static constexpr int LEVEL_INCREMENT = 2;
    static void dump(Node* node, const string& key, int level) {
        if (!node) {
            return;
        }

        cout << string(level, ' ') << key;
        if (node->is_final) {
            cout << ", value: " << node->data;
        }
        cout << endl;

        char cur_symbol = 'A';
        for (auto& child : node->children) {
            dump(child, key + cur_symbol, level + LEVEL_INCREMENT);
            ++cur_symbol;
        }
    }

    Node* root_;
};


int main() {
    TrieContainer<int> tc;

    tc.put("mike", 0);
    tc.put("mouce", 1);
    tc.dump();

    tc.del("mi");
    tc.dump();

    tc.del("mike");
    tc.dump();

    return 0;

    Trie* obj = new Trie();
    obj->insert("apple");
    bool param_2 = obj->search("apple");
    cout << "param_2 = " << param_2 << endl;
    bool param_3 = obj->startsWith("app");
    cout << "param_3 = " << param_2 << endl;

    return 0;
}
