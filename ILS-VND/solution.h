#pragma once

#include <vector>

class Graph;

class Solution{
  private:
    Graph* graph_;

    std::vector<int> solution_;
    std::vector<int> permutation_;
    int solution_size_;
    int free_size_;
    int weight_;

    std::vector<int> tightness_;
    std::vector<int> mi_;
    
    void MoveVertex(int vertex, int location);

  public:
    Solution(Graph* graph);

    void Insert(int vertex);
    void Remove(int vertex);

    bool N1();
    bool N2();

    int SolutionSize() { return solution_size_; }
    int FreeSize() { return free_size_; }
    int SolutionWeight() { return weight_; }
    int GetVertex(int index) { return solution_[index]; }
    bool IsMaximal() { return !free_size_; }
    bool IsInSolution(int vertex) {
        return permutation_[vertex] < solution_size_;
    }

    bool IsFree(int vertex){
        return solution_size_ <= permutation_[vertex] &&
               permutation_[vertex] < solution_size_ + free_size_;
    }

    const std::vector<int> GetSolution();
    bool Check();
};