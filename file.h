#pragma once

class File{
  public:
    void ReadGraph(std::string file_name, std::string type, int& no_nodes,
                   std::vector<std::vector<int>>& adjacency_matrix);

    void WriteResults(std::string file_name, std::string name, double avg,
                      double best_known_solution, double found_solution_avg,
                      int iter);
};