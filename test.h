#pragma once

#include<bits/stdc++.h>
#include "graph.h"

void Test(std::string file_name, Graph* graph){
    std::ifstream file(file_name);
    std::string tmp;
    std::string line;

    if(file.is_open()){
        std::getline(file, line);
        std::stringstream ss(line);
        
        ss >> tmp;
        ss >> tmp;
        ss >> tmp;

        graph->initialize(std::stoi(tmp));
    }

    int fst, snd;
    while(file.good()){
        std::getline(file, line);
        std::stringstream ss(line);

        ss >> tmp;
        ss >> tmp;
        fst = std::stoi(tmp);
        ss >> tmp;
        snd = std::stoi(tmp);
        graph->place_edge(fst - 1, snd - 1);
    }
}