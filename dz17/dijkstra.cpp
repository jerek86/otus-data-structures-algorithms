#include <array>
#include <iomanip>
#include <iostream>
#include <functional>
#include <limits>
#include <optional>
#include <string>
#include <iostream>
#include <sstream>

#include "../dz4/vector_array.hpp"

using namespace std;

struct Edge {
    int v1;
    int v2;
    int weight;
};

struct Graph {
    Edge** adjacency_list_;
    
    const size_t VERTEX_COUNT;
    const size_t EDGES_MAX;

    Graph(size_t vertex_count, size_t edges_max) 
     : VERTEX_COUNT(vertex_count), EDGES_MAX(edges_max) {
        adjacency_list_ = new Edge* [VERTEX_COUNT];
        for (size_t i = 0; i < VERTEX_COUNT; ++i) {
            adjacency_list_[i] = new Edge [EDGES_MAX];
            for (size_t j = 0; j < EDGES_MAX; ++j) {
                adjacency_list_[i][j].v1 = VERTEX_COUNT; // invalid value
            }
        }
    }

    ~Graph() {
        for (size_t i = 0; i < VERTEX_COUNT; ++i) {
            delete adjacency_list_[i];
        }
        delete adjacency_list_;
    }

    bool add_edge(int from, int to, int weight) {
        if (from >= VERTEX_COUNT || from >= VERTEX_COUNT) {
            return false;
        }

        for (size_t j = 0; j < EDGES_MAX; ++j) {
            if (adjacency_list_[from][j].v1 >= VERTEX_COUNT) { // is free
                adjacency_list_[from][j].v1 = from;
                adjacency_list_[from][j].v2 = to;
                adjacency_list_[from][j].weight = weight;
                break;
            }
        }

        for (size_t j = 0; j < EDGES_MAX; ++j) {
            if (adjacency_list_[to][j].v1 >= VERTEX_COUNT) { // is free
                adjacency_list_[to][j].v1 = to;
                adjacency_list_[to][j].v2 = from;
                adjacency_list_[to][j].weight = weight;
                break;
            }
        }

        return true;
    }

    void dump() {
        cout << "graph adjacency list:" << endl;
        for (size_t i = 0; i < VERTEX_COUNT; ++i) {
            cout << "[" << i << "]: ";
            for (size_t j = 0; j < EDGES_MAX; ++j) {
                if (adjacency_list_[i][j].v1 >= VERTEX_COUNT) {
                    break;
                }
                cout << adjacency_list_[i][j].v2 << ": " << adjacency_list_[i][j].weight << " ";
            }
            cout << endl;
        }

    }
};

void dump(VectorArray<Edge>& vec) {
    for (size_t i = 0; i < vec.size(); ++i) {
        auto edge = vec.get(i);
        cout << edge.v1 << " <-> " << edge.v2 << " : " << edge.weight << endl;
    }
}

class EdgesInversedHeap {
public:
    EdgesInversedHeap(int max_size)
     : MAX_SIZE_(max_size) {
        h = new Edge [MAX_SIZE_];
    }
    ~EdgesInversedHeap() {
        delete [] h;
    }

    int size() { return heap_size_; }
    bool empty() { return heap_size_ == 0; }

    void put(Edge edge) {
        int i = heap_size_;
        int p;

        h[i] = edge;
        p = parent(i);
        while(p >= 0 && i > 0)  {
            if (h[i].weight < h[p].weight) {
                Edge temp = h[i];
                h[i] = h[p];
                h[p] = temp;
            }
            i = p;
            p = parent(i);
        }
        heap_size_++;        
    }

    Edge pop() {
        Edge result = h[0];

        h[0] = h[heap_size_-1];
        --heap_size_;
        heapify(0);
        return result;
    }

    void heapify(int i) {
        int l = left(i);
        int r = right(i);
        Edge temp;

        if (l < heap_size_) {
            if (h[i].weight > h[l].weight) {
                swap(i, l);
                heapify(l);
            }
        }
        if (r < heap_size_) {
            if (h[i].weight > h[r].weight) {
                swap(i, r);
                heapify(r);
            }
        }
    }
private:
    int left(int i) { return 2 * i + 1; }
    int right(int i) { return 2 * i + 2; }
    int parent(int i) { return (i - 1) / 2; }

    void swap(int i, int j) {
        Edge temp = h[i];
        h[i] = h[j];
        h[j] = temp;
    }

    const int MAX_SIZE_ = 100;
    Edge* h;
    int heap_size_ = 0;
};

class DijkstraAlgo {
public:
    DijkstraAlgo() = default;

    VectorArray<Edge> get_paths_weight(const Graph& graph, int v) {
        VectorArray<bool> visited;
        for (int i = 0; i < graph.VERTEX_COUNT; ++i) {
            visited.add(false);
        }

        VectorArray<Edge> result;
        for (int i = 0; i < graph.VERTEX_COUNT; ++i) {
            result.add(Edge{v, i, std::numeric_limits<int>::max()});
        }
        result.get(v).weight = 0;

        EdgesInversedHeap heap(graph.VERTEX_COUNT);
        heap.put(Edge{v, v, 0});

        while (!heap.empty()) {
            Edge e = heap.pop();
            v = e.v2;
            if (visited.get(v)) {
                continue;
            }
            visited.get(v) = true;

            for (size_t j = 0; j < graph.EDGES_MAX; ++j) {
                Edge edge = graph.adjacency_list_[v][j];
                if (edge.v1 >= graph.VERTEX_COUNT) {
                    break;
                }
                int w = edge.v2;
                if (result.get(w).weight > result.get(v).weight + edge.weight) {
                    result.get(w).weight = result.get(v).weight + edge.weight;
                    heap.put(result.get(w));
                }
            }
        }
    
        dump(result);

        return result;
    }

private:
};

int main() {
    Graph graph(6, 4);

    graph.add_edge(0, 1, 7);
    graph.add_edge(0, 2, 9);
    graph.add_edge(0, 5, 14);
    graph.add_edge(1, 2, 10);
    graph.add_edge(1, 3, 15);
    graph.add_edge(2, 3, 11);
    graph.add_edge(2, 5, 2);
    graph.add_edge(3, 4, 6);
    graph.add_edge(4, 5, 9);

    graph.dump();

    DijkstraAlgo{}.get_paths_weight(graph, 0);

    return 0;
}
