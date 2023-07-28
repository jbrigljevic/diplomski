#pragma once

#include<bits/stdc++.h>

#define MAX_ITER 2 * pow(10, 6)
#define NO_CHANGE 10000
class Graph{
private:
    int no_nodes;
    std::vector<std::vector<int>> adj_matrix; // graph's adjacency matrix
    std::vector<int> weights; // weights of vertices

public:
    class Solution{
    private:
        Graph* graph;
        std::vector<int> S; // this is a permutation of the vertices
        std::vector<int> perm; // this remembers the location of each vertex in S
        std::vector<int> tightness; // tightness of each vertex
        int S_size;
        int free_size;
        
        std::vector<int> mi; // the vector required in N1

        std::set<int> candidates; // the vector required in N2
        std::vector<std::list<int>> sol_neighs;
        std::vector<std::vector<std::list<int>::iterator>> pointers;

    public:
        Solution(Graph* graph);
        Solution(const Solution& sol);

        friend void swap(Solution& fst, Solution& snd);
        Solution& operator=(Solution sol);

        void move_vertex(int vertex, int location);
        void insert(int vertex);
        void remove(int vertex);

        bool is_maximal(){ return !free_size; }
        bool is_in_solution(int vertex){ return perm[vertex] < S_size; }
        bool is_free(int vertex){
            return S_size <= perm[vertex] && perm[vertex] < S_size + free_size;
        }
        bool is_non_free(int vertex){ return perm[vertex] >= S_size + free_size; }
        bool is_zero_tight(int vertex){ return !tightness[vertex]; }

        int size(){ return S_size; }
        int sizec(){ return free_size; }
        int get_vertex(int index){ return S[index]; }
        int get_mi(int index){ return mi[index]; }
        const std::vector<int> get_solution();
        const std::vector<int> get_solution_c();

        void N1(std::vector<bool>* del);
        void N2(int* ins);

        bool check();
    };

    Graph(int no_nodes, std::vector<std::vector<int>> adj_matrix);
    Graph(std::string file_name);

    void complement();

    int get_no_nodes(){ return no_nodes; }
    int get_weight(int vertex){ return weights[vertex]; }
    std::vector<int> get_weights(){ return weights; }

    bool exists_edge(int v1, int v2){
        return std::find(adj_matrix[v1].begin(), adj_matrix[v1].end(), v2) != 
                        adj_matrix[v1].end();
    }

    void print(){
        for(int i = 0; i < no_nodes; ++i){
            for(auto it = adj_matrix[i].begin(); it != adj_matrix[i].end(); ++it)
            { std::cout << i << " " << *it << std::endl; }
        }
    }

    int weight(Solution& sol);
    void maximize(Solution& sol);
    Solution perturb(int c, Solution sol, std::vector<bool>& del, int& ins);
    Solution first_improvement(int k, Solution& sol,
                                std::vector<bool>* del, int* ins);
    void local_search(Solution& sol, std::vector<bool>* del, int* ins);
    void accept(Solution& sol, Solution& new_sol, Solution& best_sol, int& i,
                int& local_best_w, int c2, int c3, int c4);
    Solution algorithm(int max_iter = MAX_ITER, int c1 = 1, int c2 = 3,
                        int c3 = 4, int c4 = 2);
};

void swap(Graph::Solution& fst, Graph::Solution& snd);