#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#include "file.h"

void File::ReadGraph(std::string file_name, std::string type, int& no_nodes,
                     std::vector<std::vector<int>>& adjacency_matrix) {
    std::ifstream file(file_name);
    std::string tmp;
    std::string line;

    if (type == std::string("BHOSLIB")) { // BHOSLIB
        std::getline(file, line);
        std::stringstream ss(line);
        ss >> tmp;
        ss >> tmp;
        ss >> tmp;
        no_nodes = std::stoi(tmp);

        adjacency_matrix = std::vector<std::vector<int>>(no_nodes);
        
        int fst, snd;
        while (file.good()) {
            std::getline(file, line);
            std::stringstream ss(line);
            ss >> tmp;
            ss >> tmp;
            fst = std::stoi(tmp);
            ss >> tmp;
            snd = std::stoi(tmp);

            adjacency_matrix[fst - 1].push_back(snd - 1);
            adjacency_matrix[snd - 1].push_back(fst - 1);
        }
    } else { // DIMACS
        std::getline(file, line);
        std::getline(file, line);
        std::stringstream ss(line);
        ss >> tmp;
        no_nodes = std::stoi(tmp);

        adjacency_matrix = std::vector<std::vector<int>>(no_nodes);
        
        int fst, snd;
        while (file.good()) {
            std::getline(file, line);
            if(line == "") continue;
            std::stringstream ss(line);
            ss >> tmp;
            fst = std::stoi(tmp);
            ss >> tmp;
            snd = std::stoi(tmp);
            adjacency_matrix[fst - 1].push_back(snd - 1);
            adjacency_matrix[snd - 1].push_back(fst - 1);
        }
    }
    file.close();
}

void File::WriteResults(std::string file_name, std::string name, double avg_time,
                        int avg_iter, double avg_best_time, int avg_best_iter,
                        double avg_solution_weight, int best_found_solution,
                        int best_known_solution) {
    std::ofstream file("../test/" + file_name, std::ios::app);
    file << name << " " << avg_time << " " << avg_iter << " " << avg_best_time <<
        " " << avg_best_iter << " " << avg_solution_weight << " " <<
        best_found_solution << " " << best_known_solution << std::endl;
    file.close();
}