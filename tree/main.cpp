#include "graph.h"
#include "algorithm.h"

int main() {
    std::vector<std::vector<int>> adj(16);
    adj[0] = {1, 2, 3}; adj[1] = {0, 4, 5}; adj[2] = {0}; adj[3] = {0, 6, 7};
    adj[4] = {1, 8, 9}; adj[5] = {1, 10, 11, 12}; adj[6] = {3}; adj[7] = {3, 13};
    adj[8] = {4}; adj[9] = {4}; adj[10] = {5, 14, 15}; adj[11] = {5};
    adj[12] = {5}; adj[13] = {7}; adj[14] = {10}; adj[15] = {10};

    Graph graph(16, adj);
    Algorithm alg(graph);
    std::set<int> solution = alg.RunAlgorithm();

    for(auto v : solution) std::cout << v << " ";
    std::cout << std::endl;

    return 0;
}