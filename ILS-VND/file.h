#pragma once

class File{
  public:
    void ReadGraph(std::string file_name, std::string type, int& no_nodes,
                   std::vector<std::vector<int>>& adjacency_matrix);

    void WriteResults(std::string file_name, std::string name, double avg_time,
                      int avg_iter, double avg_best_time, int avg_best_iter,
                      double avg_solution_weight, int best_found_solution,
                      int best_known_solution);
};