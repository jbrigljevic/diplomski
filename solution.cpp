#include "graph.h"
#include "solution.h"

Solution::Solution(Graph* graph) : graph_(graph), solution_size_(0),
                                   weight_(0) {
    int no_nodes = graph->no_nodes();
    for (int i = 0; i < no_nodes; ++i) {
        solution_.push_back(i);
        permutation_.push_back(i);
    }

    free_size_ = no_nodes;
    tightness_.assign(no_nodes, 0);
    mi_ = graph->weights();
}

void Solution::MoveVertex(int vertex, int location) {
    solution_[permutation_[vertex]] = solution_[location];
    permutation_[solution_[location]] = permutation_[vertex];
    permutation_[vertex] = location;
    solution_[location] = vertex;
}

void Solution::Insert(int vertex) {
    // place the vertex into the solution
    MoveVertex(vertex, solution_size_++);
    --free_size_;
    weight_ += graph_->weight(vertex);

    // update the tightness of its neighbors and the array mi
    for (int neighbor : graph_->AdjacencyVector(vertex)) {
        mi_[neighbor] -= graph_->weight(vertex);
        ++tightness_[neighbor];
        if (IsFree(neighbor))
            MoveVertex(neighbor, solution_size_ + --free_size_);
    }
}

void Solution::Remove(int vertex) {
    // place the vertex at the end of the solution and decrease the solution size
    MoveVertex(vertex, --solution_size_);
    ++free_size_;
    weight_ -= graph_->weight(vertex);
    
    // update the tightness of its neighbors and the array mi
    for (int neighbor : graph_->AdjacencyVector(vertex)) {
        mi_[neighbor] += graph_->weight(vertex);
        if (!(--tightness_[neighbor]))
            MoveVertex(neighbor, solution_size_ + free_size_++);
    }
}

bool Solution::N1() {
    // insert a single vertex v from S_c into S and delete its neighbors
    // from S
    for (int i = graph_->no_nodes() - 1; i >= solution_size_; --i) {
        int vertex = GetVertex(i);
        if (mi_[vertex] > 0) {
            for (int neighbor : graph_->AdjacencyVector(vertex)) {
                if (IsInSolution(neighbor)) Remove(neighbor);
            }

            Insert(vertex);
            return true;
        }
    }

    return false;
}

bool Solution::N2() {
    // delete one vertex from S and insert two vertices from S_c
    for (int i = 0; i < solution_size_; ++i) {
        int x = solution_[i];
        // build a vector L(x), consisting of 1-tight neighbors of x
        std::vector<int> L;
        for (int neigh : graph_->AdjacencyVector(x)) {
            if (tightness_[neigh] == 1) L.push_back(neigh);
        }

        if (L.size() < 2) continue;
        
        // find a pair {v, w} in L(x), such that there is no edge between
        // v and w
        for (auto v : L) {
            auto N(graph_->AdjacencyVector(v));
            auto n = N.begin();
        
            for (auto w : L) {
                if (w == v) { continue; }

                while (n != N.end() && *n < w) { ++n; }

                if (n != N.end() && *n == w) {
                    ++n;
                    continue;
                } else if (graph_->weight(x) <
                           graph_->weight(w) + graph_->weight(v)) {
                    Remove(x);
                    Insert(v);
                    Insert(w);
                    return true;
                }
            }
        }
    }

    return false;
}

const std::vector<int> Solution::GetSolution() {
    std::vector<int> vec(&solution_[0], &solution_[SolutionSize()]);
    std::sort(vec.begin(), vec.end());
    return vec;
}

bool Solution::Check() {
    std::vector<int> sol = GetSolution();
    for (int v : sol) {
        for (int w : sol) {
            if (v == w) continue;
            if (graph_->ExistsEdge(v, w)) { return false; }
        }
    }
    return true;
}