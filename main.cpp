#include <iostream>
#include "test.h"
#include "graph.h"

int main() {
    std::string file_name("../test/BHOSLIB/frb30-15-1.txt");
    Graph graph;
    Test(file_name, &graph);

}

// WEIGHTS ARE CALCULATED AS: (i mod 200) + 1, where i is the index of a vertex

/*NOTE TO FUTURE SELF: DIMACS IS USED FOR THE CLIQUE PROBLEM, SO WE CONSIDER
ITS COMPLEMENTARY GRAPHS, THERE IS A COMMENT ABOUT BHOSLIB ON PAGE 6 THAT I DO
NOT QUITE UNDERSTAND*/