#include <iostream>
#include "graph.h"

int main() {
    /*
    std::string file_name("../test/BHOSLIB/frb30-15-1.txt");
    Graph graph(file_name);
    graph.print();*/
    int no_nodes = 8;
    std::vector<std::vector<bool>> neigh_matrix;
    std::vector<bool> tmp(no_nodes, false);
    for(int i = 0; i < no_nodes; ++i) neigh_matrix.push_back(tmp);

    neigh_matrix[0][4] = neigh_matrix[1][5] = neigh_matrix[2][6] =
    neigh_matrix[3][7] = neigh_matrix[4][5] = neigh_matrix[5][6] = 
    neigh_matrix[6][7] = neigh_matrix[7][4] = true;
    for(int i = 0; i < no_nodes; ++i){
        for(int j = 0; j < no_nodes; ++j){
            if(neigh_matrix[i][j]) neigh_matrix[j][i] = true;
        }
    }
    
    Graph graph(no_nodes, neigh_matrix);
    //graph.print();
    //algorithm(&graph);
}

/*NOTE TO FUTURE SELF: DIMACS IS USED FOR THE CLIQUE PROBLEM, SO WE CONSIDER
ITS COMPLEMENTARY GRAPHS, THERE IS A COMMENT ABOUT BHOSLIB ON PAGE 6 THAT I DO
NOT QUITE UNDERSTAND*/