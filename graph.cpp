#include "graph.h"
#include "solution.h"
#include "file.h"

#include <fstream>
#include <sstream>

Graph::Graph(int no_nodes, std::vector<std::vector<int>> adjacency_matrix) :
             no_nodes_(no_nodes), adjacency_matrix_(adjacency_matrix) {
    for (int i = 0; i < no_nodes_; ++i) {
        weights_.push_back((i + 1) % 200 + 1);
    }
}

Graph::Graph(std::string file_name, std::string type) {
    File file;
    file.ReadGraph(file_name, type, no_nodes_, adjacency_matrix_, weights_);
}

void Graph::Complement() {
    std::vector<std::vector<int>> new_adjacency_matrix(no_nodes_);
    for (int v = 0; v < no_nodes_; ++v) {
        // go over all vertices w and see if there is and edge {v, w},
        // if there isn't, add it to the new adjacency matrix
        auto W = AdjacencyVector(v);
        auto w_it = W.begin();
        for (int w = 0; w < no_nodes_; ++w){
            if (v == w) continue;
            if (w_it == W.end() || *w_it > w) {
                new_adjacency_matrix[v].push_back(w);
            } else {
                ++w_it;
            }
        }
    }
    adjacency_matrix_ = new_adjacency_matrix;
}