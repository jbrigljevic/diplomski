#pragma once

#include "solution.h"
#include "graph.h"

#define MAX_ITER 2 * pow(10, 6)
#define NO_CHANGE 1000000

class Algorithm{
  public:
    Algorithm(Graph* graph);
    
    void Maximize(Solution&);
    void Perturb(int c, Solution& s);
    bool FirstImprovement(int k, Solution& s);
    void LocalSearch(Solution& s);
    void Accept(Solution& s, Solution& new_s, Solution& best_s, int& i,
                int& local_best_w, int c2, int c3, int c4);
    Solution& RunAlgorithm(int max_iter = MAX_ITER, int c1 = 1, int c2 = 3,
                           int c3 = 4, int c4 = 2);

  private:
    Graph* graph_;

    Solution current_sol_;
    Solution best_sol_;
    Solution new_sol_;
};