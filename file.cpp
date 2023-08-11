#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#include "file.h"

void File::ReadGraph(std::string file_name, std::string type, int& no_nodes,
                     std::vector<std::vector<int>>& adjacency_matrix,
                     std::vector<int>& weights) {
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

        for (int i = 0; i < no_nodes; ++i) {
            weights.push_back((i + 1) % 200 + 1);
        }
        
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

        for (int i = 0; i < no_nodes; ++i) {
            weights.push_back((i + 1) % 200 + 1);
        }
        
        int fst, snd;
        while (file.good()) {
            std::getline(file, line);
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

void File::WriteResults(std::string file_name, std::string name, double avg,
                        double best_known_solution, double found_solution_avg,
                        int iter) {
    std::ofstream file("../test/" + file_name, std::ios::app);
    file << name << " " << avg << " " << best_known_solution << " " <<
        found_solution_avg << " " << iter << std::endl;
    file.close();
}