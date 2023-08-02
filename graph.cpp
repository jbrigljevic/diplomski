#include "graph.h"
#include "solution.h"

#include <fstream>
#include <sstream>

void exit() {
    std::cout << "Aborted!" << std::endl;
    std::exit(0);
}

Graph::Graph(int no_nodes, std::vector<std::vector<int>> adjacency_matrix) :
             no_nodes_(no_nodes), adjacency_matrix_(adjacency_matrix) {
    for (int i = 0; i < no_nodes_; ++i) {
        weights_.push_back((i + 1) % 200 + 1);
    }
}

Graph::Graph(std::string file_name) {
    std::ifstream file(file_name);
    std::string tmp;
    std::string line;

    if (file.is_open()) {
        std::getline(file, line);
        std::stringstream ss(line);
        ss >> tmp;
        ss >> tmp;
        ss >> tmp;
        no_nodes_ = std::stoi(tmp);
        
        adjacency_matrix_ = std::vector<std::vector<int>>(no_nodes_);

        for (int i = 0; i < no_nodes_; ++i) {
            weights_.push_back((i + 1) % 200 + 1);
        }
    }

    int fst, snd;
    while (file.good()) {
        std::getline(file, line);
        std::stringstream ss(line);
        ss >> tmp;
        ss >> tmp;
        fst = std::stoi(tmp);
        ss >> tmp;
        snd = std::stoi(tmp);
        adjacency_matrix_[fst - 1].push_back(snd - 1);
        adjacency_matrix_[snd - 1].push_back(fst - 1);
    }
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