#include <array>
#include <iomanip>
#include <iostream>
#include <functional>
#include <optional>
#include <string>
#include <iostream>
#include <sstream>

using namespace std;

struct Graph {
    int** adjacency_list_;
    
    const size_t VERTEX_COUNT;
    const size_t EDGES_MAX;

    Graph(size_t vertex_count, size_t edges_max) 
     : VERTEX_COUNT(vertex_count), EDGES_MAX(edges_max) {
        adjacency_list_ = new int* [VERTEX_COUNT];
        for (size_t i = 0; i < VERTEX_COUNT; ++i) {
            adjacency_list_[i] = new int [EDGES_MAX];
            for (size_t j = 0; j < EDGES_MAX; ++j) {
                adjacency_list_[i][j] = VERTEX_COUNT; // invalid value
            }
        }
    }

    ~Graph() {
        for (size_t i = 0; i < VERTEX_COUNT; ++i) {
            delete adjacency_list_[i];
        }
        delete adjacency_list_;
    }

    bool add_edge(int from, int to) {
        if (from >= VERTEX_COUNT || from >= VERTEX_COUNT) {
            return false;
        }

        for (size_t j = 0; j < EDGES_MAX; ++j) {
            if (adjacency_list_[from][j] >= VERTEX_COUNT) { // is free
                adjacency_list_[from][j] = to;
                return true;
            }
        }

        // to many edges
        return false;
    }

    Graph get_inversed() const {
        Graph result{VERTEX_COUNT, EDGES_MAX};

        for (size_t i = 0; i < VERTEX_COUNT; ++i) {
            for (size_t j = 0; j < EDGES_MAX; ++j) {
                result.add_edge(adjacency_list_[i][j], i);
            }
        }

        return result;
    }

    void dump() const {
        cout << "graph adjacency list:" << endl;
        for (size_t i = 0; i < VERTEX_COUNT; ++i) {
            cout << "[" << i << "]: ";
            for (size_t j = 0; j < EDGES_MAX; ++j) {
                if (adjacency_list_[i][j] >= VERTEX_COUNT) {
                    break;
                }
                cout << adjacency_list_[i][j] << " ";
            }
            cout << endl;
        }

    }
};

template<class T> class Stack {
public:
    Stack(size_t size)
     : SIZE_(size), length_(0) {
        stack_ = new T [SIZE_];
    }
    ~Stack() {
        delete stack_;
    }

    void push(const T& item) {
        if (full()) {
            cerr << "stack is full!" << endl;
            return;
        }

        stack_[length_++] = item;
    }

    T pop() {
        if (empty()) {
            cerr << "stack is empty!" << endl;
            return T{};
        }

        return stack_[--length_];
    }

    T top() {
        if (empty()) {
            cerr << "stack is empty!" << endl;
            return T{};
        }

        return stack_[length_ - 1];
    }

    bool empty() {
        return length_ == 0;
    }

    bool full() {
        return length_ >= SIZE_;
    }

private:
    const size_t SIZE_;
    size_t length_;
    T* stack_;
};

class SequenceSearcher {
public:
    SequenceSearcher(const Graph& graph)
     : graph_(graph), search_count_(0) {
        is_visited_ = new bool [graph_.VERTEX_COUNT];
        search_result_ = new size_t [graph_.VERTEX_COUNT];
        for (size_t i = 0; i < graph_.VERTEX_COUNT; ++i) {
            is_visited_[i] = false;
        }

        for (size_t i = 0; i < graph_.VERTEX_COUNT; ++i) {
            DFS(i);
        }
    }

    ~SequenceSearcher() {
        delete [] is_visited_;
        delete [] search_result_;
    }

    size_t* get_vertex_sequence() {
        return search_result_;
    }

    void dump() {
        cout << "search_result:" << endl;
        for (size_t i = 0; i < graph_.VERTEX_COUNT; ++i) {
            cout << search_result_[i] << " ";
        }
        cout << endl;
    }

private:
    void DFS(int v) {
        if (is_visited_[v]) {
            return;
        }

        is_visited_[v] = true;

        for (size_t j = 0; j < graph_.EDGES_MAX; ++j) {
            if (graph_.adjacency_list_[v][j] >= graph_.VERTEX_COUNT) {
                break;
            }
            int vertex = graph_.adjacency_list_[v][j];
            DFS(vertex);
        }
        search_result_[search_count_++] = v;
    }

    const Graph& graph_;
    size_t* search_result_;
    size_t search_count_;
    bool* is_visited_;
};

class KosarayuSearch {
public:
    KosarayuSearch(const Graph& graph)
     : graph_(graph) {
        is_visited_ = new bool [graph_.VERTEX_COUNT];
        search_result_ = new size_t [graph_.VERTEX_COUNT];
        for (size_t i = 0; i < graph_.VERTEX_COUNT; ++i) {
            is_visited_[i] = false;
        }
    }

    ~KosarayuSearch() {
        delete [] is_visited_;
        delete [] search_result_;
    }

    void dump() {
        // graph_.dump();

        Graph inverced = graph_.get_inversed();
        // inverced.dump();

        SequenceSearcher searcher{inverced};
        // searcher.dump();

        for (int i = graph_.VERTEX_COUNT - 1; i >= 0; --i) {
            int v = searcher.get_vertex_sequence()[i];
            if (!is_visited_[v]) {
                search_count_ = 0;
                DFS(v);
                dump_result();
            }
        }
    }

private:
    void DFS(int v) {
        if (is_visited_[v]) {
            return;
        }

        search_result_[search_count_++] = v;

        is_visited_[v] = true;

        for (size_t j = 0; j < graph_.EDGES_MAX; ++j) {
            if (graph_.adjacency_list_[v][j] >= graph_.VERTEX_COUNT) {
                break;
            }
            int vertex = graph_.adjacency_list_[v][j];
            DFS(vertex);
        }
    }
    void dump_result() {
        if (search_count_ == 0) {
            return;
        }
        cout << "{";
        for (size_t i = 0; i < search_count_; ++i) {
            cout << search_result_[i] << ", ";
        }
        cout << "}" << endl;
    }

    const Graph& graph_;
    size_t* search_result_;
    size_t search_count_;
    bool* is_visited_;
};


int main() {
    Graph graph(8, 5);

    graph.add_edge(0, 1);
    graph.add_edge(1, 2);
    graph.add_edge(1, 4);
    graph.add_edge(1, 5);
    graph.add_edge(2, 3);
    graph.add_edge(2, 6);
    graph.add_edge(3, 7);
    graph.add_edge(3, 2);
    graph.add_edge(4, 0);
    graph.add_edge(4, 5);
    graph.add_edge(5, 6);
    graph.add_edge(6, 5);
    graph.add_edge(7, 3);
    graph.add_edge(7, 6);

    KosarayuSearch search{graph};
    search.dump();

    return 0;
}
