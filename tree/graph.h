#pragma once

#include <iostream>
#include <vector>
#include <algorithm>

class Graph{
public:
    Graph() = default;
    Graph(int no_nodes, std::vector<std::vector<int>> adjacency_matrix);

    int no_nodes() const { return no_nodes_; }
    int weight(int vertex) const { return weights_[vertex]; }
    std::vector<int> weights() { return weights_; }
    std::vector<int> AdjacencyVector(int vertex) const {
        return adjacency_matrix_[vertex];
    }

    bool IsLeaf(int vertex) const {
        for(auto neigh : adjacency_matrix_[vertex])
            if (neigh > vertex) return false;
        return true;
    }

    bool ExistsEdge(int v1, int v2){
        return std::find(adjacency_matrix_[v1].begin(),
                         adjacency_matrix_[v1].end(), v2) !=
               adjacency_matrix_[v1].end();
    }

    void print() {
        std::cout << "Graph: " << std::endl;
        for(int i = 0; i < no_nodes_; ++i){
            for(int neighbor : adjacency_matrix_[i]) {
                std::cout << i << " " << neighbor << std::endl;
            }
        }
    }

private:
    int no_nodes_;
    std::vector<std::vector<int>> adjacency_matrix_;
    std::vector<int> weights_;
};