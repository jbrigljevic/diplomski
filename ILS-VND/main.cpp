#include "graph.h"
#include "solution.h"
#include "ILS-VND.h"

int main() {
    std::string file_name("../test/DIMACS/brock200-1.txt");
    Graph graph(file_name, "DIMACS");
    graph.Complement();

    Algorithm alg(&graph);

    /*int no_nodes = 8;
    std::vector<std::vector<int>> adj_matrix;
    adj_matrix.insert(adj_matrix.end(), { {4}, {5}, {6}, {7}, {0, 5, 7}, 
                    {1, 4, 6}, {2, 5, 7}, {3, 4, 6} });
    Graph graph(no_nodes, adj_matrix);*/

    auto start = std::chrono::high_resolution_clock::now();
    Solution& solution = alg.RunAlgorithm();
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>
                    (end - start) / 1e6;
    std::cout << "CPU time: " << duration.count() << "s" << std::endl <<
        std::endl;
    print(solution.GetSolution(), "S: ");
    std::cout << "Result: " << solution.SolutionWeight() << std::endl;
    solution.Check();
}