#pragma once

#include "solution.h"
#include "graph.h"
#include <chrono>

#define MAX_ITER 2 * (int)1e6

class Algorithm{
  public:
    Algorithm(Graph* graph);
    
    void Maximize(Solution&);
    void Perturb(int c, Solution& s);
    void LocalSearch(Solution& s);
    void Accept(Solution& s, Solution& new_s, Solution& best_s, int& i,
                int& local_best_w, int c2, int c3, int c4);
    int RunAlgorithm(int best_known_solution, int c1 = 1, int c2 = 3,
                     int c3 = 4, int c4 = 2);

    double GetExecutionTime() { return execution_time_; }
    int GetSolutionWeight() { return best_sol_.SolutionWeight(); }

  private:
    Graph* graph_;

    Solution current_sol_;
    Solution best_sol_;
    Solution new_sol_;

    double execution_time_;
};