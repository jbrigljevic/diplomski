#pragma once

#include "solution.h"
#include "graph.h"

#include <chrono>
#include <random>

#define MAX_ITER 2 * (int)1e6

class Algorithm{
  public:
    Algorithm(Graph* graph);
    
    void Maximize(Solution&);
    void Perturb(int c, Solution& s);
    void LocalSearch(Solution& s);
    void Accept(Solution& s, Solution& new_s, Solution& best_s, int& i,
                int& local_best_w, int c2, int c3, int c4,
                std::chrono::time_point<std::chrono::_V2::system_clock,
                std::chrono::_V2::system_clock::duration>& end_best,
                const int& iter);
    int RunAlgorithm(int best_known_solution, int c1 = 1, int c2 = 3,
                     int c3 = 4, int c4 = 2);

    double GetExecutionTime() { return execution_time_; }
    double GetBestSolutionTime() { return best_solution_time_; }
    int GetBestIter() { return no_iter_to_best_; }
    int GetSolutionWeight() { return best_sol_.SolutionWeight(); }

  private:
    Graph* graph_;

    Solution current_sol_;
    Solution best_sol_;
    Solution new_sol_;

    std::random_device rd;
    std::mt19937 gen;

    double execution_time_;
    double best_solution_time_;
    int no_iter_to_best_;
};