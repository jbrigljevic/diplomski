#include <iostream>
#include "graph.h"

template<typename T>
void print(T container, std::string pre = "NULL"){
    if(pre != "NULL") std::cout << pre;
    for(auto it = container.begin(); it != container.end(); ++it)
        std::cout << *it << " ";
    std::cout << std::endl;
}

int main() {
    
    std::string file_name("../test/BHOSLIB/frb30-15-1.txt");
    Graph graph(file_name);
    graph.complement();


    /*int no_nodes = 8;
    std::vector<std::vector<int>> adj_matrix;
    adj_matrix.insert(adj_matrix.end(), { {4}, {5}, {6}, {7}, {0, 5, 7}, 
                    {1, 4, 6}, {2, 5, 7}, {3, 4, 6} });
    Graph graph(no_nodes, adj_matrix);*/


    Graph::Solution sol = graph.algorithm();
    std::vector<int> S = sol.get_solution();
    print(S, "S: ");
    std::cout << graph.weight(sol) << std::endl;
    std::cout << std::boolalpha << sol.check() << std::endl;
}

/*NOTE TO FUTURE SELF: DIMACS IS USED FOR THE CLIQUE PROBLEM, SO WE CONSIDER
ITS COMPLEMENTARY GRAPHS, THERE IS A COMMENT ABOUT BHOSLIB ON PAGE 6 THAT I DO
NOT QUITE UNDERSTAND*/