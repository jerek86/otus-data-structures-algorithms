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

    void dump() {
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

class DemukronAlgo {
public:
    DemukronAlgo() = default;

    void dump_topological_sorting(const Graph& graph) {
        AdjacencyMatrix matrix(graph);
        matrix.dump();

        InputEdgesCount counts(matrix);
        counts.dump();

        int* step_vertices = new int [counts.VERTEX_COUNT];
        size_t step_vertices_count;

        int level = 0;
        while (!counts.empty()) {
            step_vertices_count = 0;
            for (size_t j = 0; j < counts.VERTEX_COUNT; ++j) {
                if (counts.counts_[j] == 0) {
                    step_vertices[step_vertices_count++] = j;
                }
            }

            if (step_vertices_count == 0) {
                cout << "error: graph is cylcic" << endl;
                break;
            }

            cout << "level " << level << ": {";
            for (size_t j = 0; j < step_vertices_count; ++j) {
                cout << step_vertices[j] << ",";
            }
            cout << "}" << endl;
            ++level;

            for (size_t j = 0; j < step_vertices_count; ++j) {
                counts.set_empty(step_vertices[j]);
                counts.subtract_line(matrix, step_vertices[j]);
            }

            // counts.dump();
        }

        delete step_vertices;
    }

private:
    struct AdjacencyMatrix {
        int** adjacency_matrix_;
        
        const size_t VERTEX_COUNT;

        AdjacencyMatrix(const Graph& graph)
         : VERTEX_COUNT(graph.VERTEX_COUNT) {
            adjacency_matrix_ = new int* [VERTEX_COUNT];
            for (size_t i = 0; i < VERTEX_COUNT; ++i) {
                adjacency_matrix_[i] = new int [VERTEX_COUNT];
                for (size_t j = 0; j < VERTEX_COUNT; ++j) {
                    adjacency_matrix_[i][j] = 0;
                }
            }

            for (size_t i = 0; i < graph.VERTEX_COUNT; ++i) {
                for (size_t j = 0; j < graph.EDGES_MAX; ++j) {
                    if (graph.adjacency_list_[i][j] >= VERTEX_COUNT) {
                        break;
                    }
                    
                    adjacency_matrix_[i][graph.adjacency_list_[i][j]]++;
                }
            }
        }

        ~AdjacencyMatrix() {
            for (size_t i = 0; i < VERTEX_COUNT; ++i) {
                delete adjacency_matrix_[i];
            }
            delete adjacency_matrix_;
        }

        void dump() {
            cout << "graph adjacency matrix :" << endl;
            cout << "      ";
            for (size_t j = 0; j < VERTEX_COUNT; ++j) {
                cout << "[" << setw(3) << j << "]";
            }
            cout << endl;
            for (size_t i = 0; i < VERTEX_COUNT; ++i) {
                cout << "[" << setw(3) << i << "]: ";
                for (size_t j = 0; j < VERTEX_COUNT; ++j) {
                    cout << setw(3) << adjacency_matrix_[i][j] << " |";
                }
                cout << endl;
            }

        }
    };

    struct InputEdgesCount {
        int* counts_;
        
        const size_t VERTEX_COUNT;
        const size_t EMPTY = (size_t)-1;

        InputEdgesCount(const AdjacencyMatrix& matrix)
         : VERTEX_COUNT(matrix.VERTEX_COUNT) {
            counts_ = new int [VERTEX_COUNT];
            for (size_t i = 0; i < VERTEX_COUNT; ++i) {
                counts_[i] = 0;
            }

            for (size_t i = 0; i < VERTEX_COUNT; ++i) {
                add_line(matrix, i);
            }
        }

        ~InputEdgesCount() {
            delete counts_;
        }

        void add_line(const AdjacencyMatrix& matrix, size_t line_num) {
            if (line_num >= matrix.VERTEX_COUNT) {
                return;
            }
            for (size_t i = 0; i < VERTEX_COUNT; ++i) {
                counts_[i] += matrix.adjacency_matrix_[line_num][i];
            }
        }
        void subtract_line(const AdjacencyMatrix& matrix, size_t line_num) {
            if (line_num >= matrix.VERTEX_COUNT) {
                return;
            }
            for (size_t i = 0; i < VERTEX_COUNT; ++i) {
                if (counts_[i] != EMPTY) {
                    counts_[i] -= matrix.adjacency_matrix_[line_num][i];
                }
            }
        }
        void set_empty(size_t ver) {
            if (ver >= VERTEX_COUNT) {
                return;
            }
            counts_[ver] = EMPTY;
        }
        
        bool empty() {
            for (size_t i = 0; i < VERTEX_COUNT; ++i) {
                if (counts_[i] != EMPTY) {
                    return false;
                }
            }
            return true;
        }

        void dump() {
            cout << "graph input edges count:" << endl;
            for (size_t j = 0; j < VERTEX_COUNT; ++j) {
                cout << "[" << setw(3) << j << "]";
            }
            cout << endl;
            for (size_t j = 0; j < VERTEX_COUNT; ++j) {
                if (counts_[j] == EMPTY) {
                    cout << "  - |";
                } else {
                    cout << setw(3) << counts_[j] << " |";
                }
            }
            cout << endl;
        }
    };

};

int main() {
    Graph graph(10, 5);

    graph.add_edge(0, 1);
    graph.add_edge(1, 4);
    graph.add_edge(2, 3);
    graph.add_edge(3, 0);
    graph.add_edge(3, 1);
    graph.add_edge(3, 4);
    graph.add_edge(3, 5);
    graph.add_edge(4, 6);
    graph.add_edge(5, 4);
    graph.add_edge(5, 7);
    graph.add_edge(6, 7);
    graph.add_edge(8, 9);
    // graph.add_edge(7, 2);

    graph.dump();

    DemukronAlgo{}.dump_topological_sorting(graph);

    return 0;
}
