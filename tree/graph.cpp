#include "graph.h"

Graph::Graph(int no_nodes, std::vector<std::vector<int>> adjacency_matrix) :
             no_nodes_(no_nodes), adjacency_matrix_(adjacency_matrix) {
    for (int i = 0; i < no_nodes_; ++i) {
        weights_.push_back((i + 1) % 200 + 1);
    }
}