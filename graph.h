#pragma once

#include<bits/stdc++.h>

class RemoveVertices{
private:
    std::set<int> remove;

public:
    RemoveVertices(std::set<int> remove) : remove(remove){}

    bool operator()(int vertex){
        return remove.erase(vertex);
    }
};

class Graph{
private:
    int no_nodes;
    std::vector<std::vector<bool>> neigh_matrix;
    std::vector<int> weights;

public:
    class Solution{
    public:
        Graph& graph;
        std::vector<int> S;
        std::vector<std::pair<int, std::list<int>>> S_c;
        std::vector<int> mi;
        std::forward_list<int> candidates;
        std::vector<std::forward_list<std::pair<int, std::list<int>::iterator>>>
            adj_matrix;

        Solution(Graph& graph) : graph(graph){
            mi = graph.get_weights();
            for(int i = 0; i < graph.get_no_nodes(); ++i){
                S_c.push_back(std::pair<int, std::list<int>>
                                (i, std::list<int>()));
            }

            for(int i = 0; i < graph.get_no_nodes(); ++i){
                for(int j = graph.get_no_nodes(); j >= 0; --j){
                    if(graph.neigh_matrix[i][j])
                        adj_matrix[i].push_front(
                            std::make_pair(j, S_c[j].second.begin()));
                }
            }
        }

        Solution operator=(const Solution& sol){
            return(Solution(sol));
        }

        void change(int vertex, bool increase);
        void insert(std::set<int> vertices);
        bool solution_contains(int vertex);
        std::list<int> find_solution_neighbors(int vertex);
        void insertc(std::set<int> vertices);
    };

    Graph(int no_nodes, std::vector<std::vector<bool>> neigh_matrix) :
            no_nodes(no_nodes), neigh_matrix(neigh_matrix){}

    Graph(std::string file_name){
        std::ifstream file(file_name);
        std::string tmp;
        std::string line;

        if(file.is_open()){
            std::getline(file, line);
            std::stringstream ss(line);

            ss >> tmp;
            ss >> tmp;
            ss >> tmp;

            no_nodes = std::stoi(tmp);
            std::vector<bool> tmp(no_nodes, false);
            for(int i = 0; i < no_nodes; ++i) neigh_matrix.push_back(tmp);

            for(int i = 0; i < no_nodes; ++i){
                weights.push_back(i % 200 + 1);
            }
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

            neigh_matrix[fst - 1][snd - 1] = true;
            neigh_matrix[snd - 1][fst - 1] = true;
        }
    }

    int get_no_nodes(){
        return no_nodes;
    }

    int get_weight(int index){
        return weights[index];
    }

    std::vector<int> get_weights(){
        return weights;
    }

    bool exists_edge(int v1, int v2){
        return neigh_matrix[v1][v2];
    }

    void print_graph(){
        for(int i = 0; i < no_nodes; ++i){
            for(int j = i; j < no_nodes; ++j){
                if(neigh_matrix[i][j]) std::cout << i << " " << j << std::endl;
            }
        }
    }

    void maximize(Solution& sol);
    int weight(Solution& sol);
    Solution perturb(int c, Solution sol);
    Solution first_improvement(int k, Solution& sol);
    void local_search(Solution* sol);
    void accept(Solution* sol, Solution* sol_new, Solution* sol_best, int* i,
                int* local_best_w, int c2, int c3, int c4);
    Solution algorithm(int max_iter = 100, int c1 = 1, int c2 = 3, int c3 = 4,
                        int c4 = 2);
};


// NOTE TO FUTURE SELF: IT MIGHT MAKE SENSE TO USE A LIST IN RemoveVertices,
// SO THAT ERASE HAS CONSTANT COMPLEXITY