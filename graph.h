#pragma once

//#include<bits/stdc++.h>
#include <iostream>

class Graph{
private:
    int no_nodes;
    bool** neigh_matrix;
    int* weights;

public:
    void initialize(int n){
        no_nodes = n;
        neigh_matrix = new bool*[no_nodes];
        weights = new int[no_nodes];

        for(int i = 0; i < no_nodes; ++i){
            neigh_matrix[i] = new bool[no_nodes]();
            weights[i] = i % 200 + 1;
        }
    }

    int get_no_nodes(){
        return no_nodes;
    }

    void place_edge(int fst, int snd){
        neigh_matrix[fst][snd] = true;
        neigh_matrix[snd][fst] = true;
    }

    void print(){
        for(int i = 0; i < no_nodes; ++i){
            for(int j = i; j < no_nodes; ++j){
                if(neigh_matrix[i][j]) std::cout << i << " " << j << std::endl;
            }
        }
    }

    ~Graph(){
        for(int i = 0; i < no_nodes; ++i) delete[] neigh_matrix[i];
        delete[] neigh_matrix;
    }
};