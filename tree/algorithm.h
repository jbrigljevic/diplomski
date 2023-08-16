#pragma once

#include "graph.h"

#include <set>

class Algorithm {
  public:
    Algorithm(const Graph& graph);

    void Calculate(int vertex);
    void Examine(int vertex);
    std::set<int> RunAlgorithm();

  private:
    const Graph& graph_;

    std::set<int> solution_;

    std::vector<int> m_;
    std::vector<int> m_comp_;
};