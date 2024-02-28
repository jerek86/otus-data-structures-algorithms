#include <array>
#include <iomanip>
#include <iostream>
#include <functional>
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
                return true;
            }
        }

        // to many edges
        return false;
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

class UnionFind {
public:
    UnionFind(size_t size) {
        mains_ = new int [size];
        for (int i = 0; i < size; ++i) {
            mains_[i] = i;
        }
    }
    ~UnionFind() {
        delete mains_;
    }

    bool are_union(int v1, int v2) {
        return find_main(v1) == find_main(v2);
    }
    void make_union(int v1, int v2) {
        mains_[find_main(v2)] = v1;
        mains_[v2] = v1;
    }

private:
    int find_main(int v) {
        if (v == mains_[v]) {
            return v;
        }

        mains_[v] = find_main(mains_[v]);

        return mains_[v];
    }

    int* mains_;
};

class KraskalAlgo {
public:
    KraskalAlgo() = default;

    VectorArray<Edge> get_minimal_tree(const Graph& graph) {
        auto edges = get_all_edges(graph);

        auto sorted_edges = sort_edges(edges);

        UnionFind union_find{graph.VERTEX_COUNT};
        VectorArray<Edge> result;
        for (int i = 0; i < sorted_edges.size(); ++i) {
            auto& edge = sorted_edges.get(i);
            if (!union_find.are_union(edge.v1, edge.v2)) {
                result.add(edge);
                union_find.make_union(edge.v1, edge.v2);

                if (result.size() >= graph.VERTEX_COUNT - 1) {
                    break;
                }
            }
        }

        dump(result);

        return result;
    }

private:
    VectorArray<Edge> get_all_edges(const Graph& graph) {
        VectorArray<Edge> result;
        
        for (size_t i = 0; i < graph.VERTEX_COUNT; ++i) {
            for (size_t j = 0; j < graph.EDGES_MAX; ++j) {
                if (graph.adjacency_list_[i][j].v1 >= graph.VERTEX_COUNT) {
                    break;
                }
                result.add(graph.adjacency_list_[i][j]);
            }
        }

        return result;
    }

    VectorArray<Edge> sort_edges(VectorArray<Edge>& edges) {
        VectorArray<Edge> sorted_edges;

        for (int i = 0; i < edges.size(); ++i) {
            sorted_edges.add(edges.get(i));
        }
        
        for (int i = 1; i < sorted_edges.size(); ++i) {
            for (int j = i - 1; j >= 0; --j) {
                if (sorted_edges.get(j).weight <= sorted_edges.get(j + 1).weight) {
                    break;
                }
                auto temp = sorted_edges.get(j);
                sorted_edges.get(j) = sorted_edges.get(j + 1);
                sorted_edges.get(j + 1) = temp;
            }
        }

        return sorted_edges;
    }
};

int main() {
    Graph graph(7, 5);

    graph.add_edge(0, 1, 7);
    graph.add_edge(0, 3, 5);
    graph.add_edge(1, 2, 8);
    graph.add_edge(1, 3, 9);
    graph.add_edge(1, 4, 7);
    graph.add_edge(2, 4, 5);
    graph.add_edge(3, 4, 15);
    graph.add_edge(3, 5, 6);
    graph.add_edge(4, 5, 8);
    graph.add_edge(4, 6, 9);
    graph.add_edge(5, 6, 11);

    graph.dump();

    KraskalAlgo{}.get_minimal_tree(graph);

    return 0;
}
