#pragma once

#include "graph.h"

#include <set>

class Algorithm {
  public:
    Algorithm(const Graph& graph);

    void Calculate(int vertex);
    void Examine(int vertex);
    void RunAlgorithm();

    std::set<int> GetSolution() { return solution_; }
    int GetSolutionWeight() {
        int ret = 0;
        for (auto vertex : solution_) {
            ret += graph_.weight(vertex);
        }
        return ret;
    }

  private:
    const Graph& graph_;

    std::set<int> solution_;

    std::vector<int> m_;
    std::vector<int> m_comp_;
};