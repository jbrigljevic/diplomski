#include "algorithm.h"

Algorithm::Algorithm(const Graph& graph) : graph_(graph),
    m_(std::vector<int>(graph.no_nodes(), 0)),
    m_comp_(std::vector<int>(graph.no_nodes(), 0)) {}

void Algorithm::Calculate(int vertex) {
    m_[vertex] = graph_.weight(vertex);
    // if the vertex is a leaf, set M and M' weights
    if (graph_.IsLeaf(vertex)) {
        m_comp_[vertex] = 0;
    } else {
        for (auto neigh : graph_.AdjacencyVector(vertex)) {
            if (neigh > vertex && m_[neigh] == 0) {
                Calculate(neigh);
                m_[vertex] += m_comp_[neigh];
                m_comp_[vertex] += (m_[neigh] >= m_comp_[neigh]) ? m_[neigh] : 
                                    m_comp_[neigh];        
            }
        }
    }
}

void Algorithm::Examine(int vertex) {
    if (vertex == 0 && m_[vertex] > m_comp_[vertex]) solution_.insert(vertex);
    else {
        if (m_[vertex] > m_comp_[vertex] &&
                solution_.find(graph_.AdjacencyVector(vertex)[0]) ==
                solution_.end()) solution_.insert(vertex);
        
        for(auto neigh : graph_.AdjacencyVector(vertex)) 
            if (neigh > vertex) Examine(neigh);
    }
}

void Algorithm::RunAlgorithm() {
   Calculate(0);
   Examine(0);
}