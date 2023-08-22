#include "graph.h"
#include "algorithm.h"

#include <iostream>
#include <fstream>

int main() {
    std::vector<std::vector<int>> adj(16);
    adj[0] = {1, 2, 3}; adj[1] = {0, 4, 5}; adj[2] = {0}; adj[3] = {0, 6, 7};
    adj[4] = {1, 8, 9}; adj[5] = {1, 10, 11, 12}; adj[6] = {3}; adj[7] = {3, 13};
    adj[8] = {4}; adj[9] = {4}; adj[10] = {5, 14, 15}; adj[11] = {5};
    adj[12] = {5}; adj[13] = {7}; adj[14] = {10}; adj[15] = {10};

    Graph graph(16, adj);
    Algorithm alg(graph);
    alg.RunAlgorithm();
    std::set<int> solution = alg.GetSolution();
    int solution_weight = alg.GetSolutionWeight();

    std::ofstream file("../solution.txt", std::ios::trunc);

    file << adj.size() << std::endl;
    for (int i = 0; i < adj.size(); ++i) {
        file << i << " ";
        for (auto v: adj[i]) {
            file << v << " ";
        }
        file << std::endl;
    }
    file << "-" << std::endl;

    std::cout << "Solution: ";
    for (auto v : solution) {
        std::cout << v << " ";
        file << v << " ";
    }
    std::cout << std::endl << "Weight: " << solution_weight << std::endl;
    file.close();

    system("py ../visualization.py");

    return 0;
}