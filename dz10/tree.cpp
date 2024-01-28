#include <algorithm>
#include <chrono>
#include <cstdlib>
#include <iostream>

using namespace std::chrono;

template<class T> class AVLThee {
public:
    AVLThee(bool need_balance = true) : root_(nullptr), need_balance_(need_balance) {}
    ~AVLThee() { clear_node(root_); }

    void insert(T x) {
        root_ = insert(x, root_);
    }

    bool search(T x) {
        return find(x) != nullptr;
    }

    void remove(T x) {
        root_ = remove(x, root_);
    }

    template<class Fn> void for_each(Fn fn) {
        DFS(fn, root_);
    }

    void dump() {
        for_each([] (const T& item) {
            std::cout << item << " ";
        });
        std::cout << std::endl;
    }
private:
    struct Node {
        std::vector<T> data;
        Node* left;
        Node* right;
        int height;

        Node(T value)
         : data(1, value), left(nullptr), right(nullptr), height(0) {}
    };

    bool need_balance(Node* node) {
        return need_balance_ && (std::abs(height(node->left) - height(node->right)) >= 2);
    }

    int height(Node* node) {
        return (node == nullptr ? -1 : node->height);
    }

    void clear_node(Node* node) {
        if(node == nullptr)
            return;
        clear_node(node->left);
        clear_node(node->right);
        delete node;
    }

    Node* find_min(Node* node)
    {
        if (node == nullptr) {
            return nullptr;
        } else if (node->left == nullptr) {
            return node;
        } else {
            return find_min(node->left);
        }
    }

    Node* single_right_rotate(Node* node) {
        Node* temp = node->left;
        node->left = temp->right;
        temp->right = node;
        node->height = std::max(height(node->left), height(node->right)) + 1;
        temp->height = std::max(height(temp->left), height(temp->right)) + 1;
        return temp;
    }

    Node* single_left_rotate(Node* node) {
        Node* temp = node->right;
        node->right = temp->left;
        temp->left = node;
        node->height = std::max(height(node->left), height(node->right)) + 1;
        temp->height = std::max(height(temp->left), height(temp->right)) + 1;
        return temp;
    }

    Node* double_left_rotate(Node* node) {
        node->right = single_right_rotate(node->right);
        return single_left_rotate(node);
    }

    Node* double_right_rotate(Node* node) {
        node->left = single_left_rotate(node->left);
        return single_right_rotate(node);
    }

    Node* find(T x) {
        Node* node = root_;
        while (node) {
            if (x < node->data.front()) {
                node = node->left;
            } else if(x > node->data.front()) {
                node = node->right;
            } else {
                return node;
            }
        }

        return nullptr;
    }

    Node* insert(T x, Node* node) {
        if(node == nullptr) {
            node = new Node(x);
        } else if(x < node->data.front()) {
            node->left = insert(x, node->left);
            if (need_balance(node)) {
                if (x < node->left->data.front()) {
                    node = single_right_rotate(node);
                } else {
                    node = double_right_rotate(node);
                }
            }
        } else if(x > node->data.front()) {
            node->right = insert(x, node->right);
            if (need_balance(node)) {
                if (x > node->right->data.front()) {
                    node = single_left_rotate(node);
                } else {
                    node = double_left_rotate(node);
                }
            }
        } else {
            node->data.emplace_back(x);
        }

        node->height = std::max(height(node->left), height(node->right)) + 1;
        return node;
    }

    Node* remove(int x, Node* node) {
        Node* temp;

        if (node == nullptr) {
            return nullptr;
        } else if (x < node->data.front()) {
            node->left = remove(x, node->left);
        } else if (x > node->data.front()) {
            node->right = remove(x, node->right);
        } else { // found
            node->data.resize(node->data.size() - 1);
            if (!node->data.empty()) {
                return node;
            }
            if (node->left && node->right) { // has 2 children
                temp = find_min(node->right);
                node->data = temp->data;
                temp->data.resize(1);
                node->right = remove(temp->data.front(), node->right);
            } else { // has one or zero child
                temp = node;
                if (node->left == nullptr) {
                    node = node->right;
                } else if (node->right == nullptr) {
                    node = node->left;
                }
                delete temp;
            }
        }

        if (node == nullptr) {
            return nullptr;
        }

        node->height = std::max(height(node->left), height(node->right)) + 1;

        if (need_balance_) {
            if (height(node->left) - height(node->right) > 1) {
                if (height(node->left) >= height(node->right)) {
                    return single_right_rotate(node);
                } else {
                    return double_right_rotate(node);
                }
            } else if (height(node->right) - height(node->left) > 1) {
                if (height(node->right->right) >= height(node->right->left)) {
                    return single_left_rotate(node);
                } else {
                    return double_left_rotate(node);
                }
            }
        }
        
        return node;
    }    

    template<class Fn> void DFS(Fn fn, Node* node) {
        if (node == nullptr) {
            return;
        }
        DFS(fn, node->left);
        for (auto& item : node->data) {
            fn(item);
        }
        DFS(fn, node->right);
    }

    Node* root_;
    const bool need_balance_;
};

int main() {
    constexpr unsigned SEED = 12345;
    std::srand(SEED);

    AVLThee<int> tree{false};
    AVLThee<int> tree_second{false};

    size_t N = 50000;
    size_t search_count = N / 10;

    std::cout << "random inserted tree:" << std::endl;

    auto start = high_resolution_clock::now();
    for (size_t i = 0; i < N; ++i) {
        int t = std::rand() % N;
        // std::cout << t << " ";
        tree.insert(t);
    }
	auto stop = high_resolution_clock::now();
	auto duration = duration_cast<milliseconds>(stop - start);

    // tree.dump();
    std::cout << "insert " << N << " elements in first tree for " << duration.count() << "ms" << std::endl;

    start = high_resolution_clock::now();
    for (size_t i = 0; i < search_count; ++i) {
        tree.search(std::rand() % N);
    }
	stop = high_resolution_clock::now();
	duration = duration_cast<milliseconds>(stop - start);
    std::cout << "search " << search_count << " elements in first tree for " << duration.count() << "ms" << std::endl;

    start = high_resolution_clock::now();
    for (size_t i = 0; i < search_count; ++i) {
        tree.remove(std::rand() % N);
    }
	stop = high_resolution_clock::now();
	duration = duration_cast<milliseconds>(stop - start);
    std::cout << "remove " << search_count << " elements in first tree for " << duration.count() << "ms" << std::endl;




    std::cout << "in order inserted tree:" << std::endl;

    start = high_resolution_clock::now();
    for (size_t i = 0; i < N; ++i) {
        tree_second.insert(i);
    }
	stop = high_resolution_clock::now();
	duration = duration_cast<milliseconds>(stop - start);

    // tree_second.dump();
    std::cout << "insert " << N << " elements in second tree for " << duration.count() << "ms" << std::endl;


    start = high_resolution_clock::now();
    for (size_t i = 0; i < search_count; ++i) {
        tree_second.search(std::rand() % N);
    }
	stop = high_resolution_clock::now();
	duration = duration_cast<milliseconds>(stop - start);
    std::cout << "search " << search_count << " elements in second tree for " << duration.count() << "ms" << std::endl;

    start = high_resolution_clock::now();
    for (size_t i = 0; i < search_count; ++i) {
        tree_second.remove(std::rand() % N);
    }
	stop = high_resolution_clock::now();
	duration = duration_cast<milliseconds>(stop - start);
    std::cout << "remove " << search_count << " elements in second tree for " << duration.count() << "ms" << std::endl;


    return 0;
}