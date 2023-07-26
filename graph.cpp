#include "graph.h"

template<typename T>
void print(T container, std::string pre = "NULL"){
    if(pre != "NULL") std::cout << pre;
    for(auto it = container.begin(); it != container.end(); ++it)
        std::cout << *it << " ";
    std::cout << std::endl;
}

Graph::Solution::Solution(Graph& graph) : graph(graph){
    for(int i = 0; i < graph.get_no_nodes(); ++i){
        S.push_back(i);
        perm.push_back(i);
    }
    S_size = 0;
    free_size = graph.get_no_nodes();
    tightness.assign(graph.get_no_nodes(), 0);
    mi = graph.get_weights();
}

void Graph::Solution::move_vertex(int vertex, int location){
    S[perm[vertex]] = S[location];
    perm[S[location]] = perm[vertex];
    perm[vertex] = location;
    S[location] = vertex;
}

void Graph::Solution::insert(int vertex){
    // if the vertex is free, reduce the size of the free bucket
    if(is_free(vertex)) --free_size;

    // if the solution isn't maximal, there still exists a free vertex
    // that we have to keep inside the free vertex bucket
    else if(!is_maximal()) move_vertex(S[S_size], S_size + free_size);

    // place the vertex into the solution
    move_vertex(vertex, S_size++);

    // update the tightness of its neighbors
    for(auto it = graph.adj_matrix[vertex].begin();
            it != graph.adj_matrix[vertex].end(); ++it)
        if(!tightness[*it]++ && is_free(*it))
            move_vertex(*it, free_size-- + S_size - 1);

    // update the candidate list
    candidates.push_back(vertex);
}

void Graph::Solution::remove(int vertex){
    if(is_zero_tight(vertex)){
        // remove the vertex from the solution to the free bucket
        move_vertex(vertex, --S_size);
        ++free_size;
    }

    else{
        // if the solution isn't maximal, we have to protect the last free
        // vertex, by placing the vertex we are removing at the end of the
        // solution bucket
        if(!is_maximal()) move_vertex(vertex, S_size - 1);
    
        // remove the vertex from the solution to the nonfree bucket
        move_vertex(vertex, --S_size + free_size);
    }

    // update the tightness of its neighbors and update the candidate list
    for(auto it = graph.adj_matrix[vertex].begin();
            it != graph.adj_matrix[vertex].end(); ++it){
        if(!--tightness[*it] && is_non_free(*it))
            move_vertex(*it, S_size + free_size++);

        if(tightness[*it] == 1){
            // find the neighbor of *it that is in the solution and add it to
            // the candidate list
            for(auto neigh = graph.adj_matrix[*it].begin();
                    neigh != graph.adj_matrix[*it].end(); ++neigh){
                if(is_in_solution(*neigh)) candidates.push_back(*neigh);
            }
        }
    }
}

const std::vector<int> Graph::Solution::get_solution(){
    std::vector<int> vec(&S[0], &S[size()]);
    return vec;
}

const std::vector<int> Graph::Solution::get_solution_c(){
    std::vector<int> vec(&S[size()], &S[graph.get_no_nodes()]);
    return vec;
}

Graph::Graph(int no_nodes, std::vector<std::vector<int>> adj_matrix) :
                no_nodes(no_nodes), adj_matrix(adj_matrix){
    for(int i = 0; i < no_nodes; ++i){
        weights.push_back(i % 200 + 1);
    }
}

Graph::Graph(std::string file_name){
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
            std::vector<int> vec;
            adj_matrix.assign(no_nodes, vec);

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

            adj_matrix[fst - 1].push_back(snd - 1);
            adj_matrix[snd - 1].push_back(fst - 1);
        }
    }

int Graph::weight(Solution& sol){
    int weight = 0;
    for(int i = 0; i < sol.size(); ++i) weight += get_weight(sol.get_vertex(i));
    return weight;
}

void Graph::maximize(Solution& sol){
    std::random_device rd;
    std::mt19937 gen(rd());

    // pick random vertices and insert them into S until it is maximal
    while(!sol.is_maximal()){
        std::uniform_int_distribution<> dist(sol.size(),
            sol.size() + sol.sizec() - 1);
        sol.insert(sol.get_vertex(dist(gen)));
    }
}

Graph::Solution Graph::perturb(int c, Solution sol){
    std::random_device rd;
    std::mt19937 gen(rd());
    std::cout << "Begin perturbation" << std::endl;
    // pick c random vertices and insert them into S
    for(int i = 0; i < c; ++i){
        std::uniform_int_distribution<> dist(sol.size(), no_nodes - 1);
        int vertex = sol.get_vertex(dist(gen));
        std::cout << vertex << std::endl;
        sol.insert(vertex);
        // remove their neighbors from the solution S
        for(auto it = adj_matrix[vertex].begin();
                it != adj_matrix[vertex].end(); ++it){
            if(sol.is_in_solution(*it)) sol.remove(*it);
        }
    }
    std::cout << "End perturbation" << std::endl;
    // maximize the solution S
    std::cout << "Begin maximization" << std::endl;
    maximize(sol);
    std::cout << "End maximization" << std::endl;
    return sol;
}

Graph::Solution Graph::first_improvement(int k, Solution& sol){
    // Neighborhood N_1
    if(k == 1){
        // insert a single vertex v from S_c into S and delete its neighbors
        // from S
        
    }

    // Neighborhood N_2
    else{
        // delete one vertex from S and insert two vertices from S_c
    }

    return sol;
}

void Graph::local_search(Solution* sol){
    int k = 1;
    while(k <= 2){
        Solution new_sol = first_improvement(k, *sol);

        if(weight(new_sol) <= weight(*sol)) ++k;
        else{
            k = 1;
            *sol = new_sol;
            maximize(*sol);
        }
    }
}

void Graph::accept(Solution* sol, Solution* new_sol, Solution* best_sol, int* i,
                    int* local_best_w, int c2, int c3, int c4){
    int S_weight = weight(*sol);
    int S_best_weight = weight(*best_sol);
    int S_new_weight = weight(*new_sol);
    if(S_weight < S_new_weight){
        sol = new_sol;
        *i = 1;

        if(*local_best_w < S_weight){
            *local_best_w = S_weight;
            *i = *i - sol->size() / c2;
        }

        if(S_best_weight < S_weight){
            best_sol = sol;
            *i = *i - sol->size() * c3;
        }
    }

    else{
        if(*i <= sol->size() / c2) ++*i;
        else{
            *local_best_w = S_weight;
            *sol = perturb(c4, *sol);
            *i = 1;
        }
    }
}

Graph::Solution Graph::algorithm(int max_iter, int c1, int c2, int c3, int c4){
    Solution S(*this);

    maximize(S);
    std::cout << "Initialization:" << std::endl;
    ::print(S.get_solution(), "S_0: "); ::print(S.get_solution_c(), "S_0_c: ");
    std::cout << "Begin local search" << std::endl;
    local_search(&S);
    std::cout << "Local search done" << std::endl;
    Solution S_best = S;

    int local_best_w = weight(S);
    int i = 1;
    while(max_iter--){
        std::cout << "Iteration #" << 100 - max_iter << std::endl;
        Solution S_new = perturb(c1, S);
        std::cout << "Perturbation:" << std::endl;
        ::print(S_new.get_solution(), "S: ");
        ::print(S_new.get_solution_c(), "S_c: ");
        break;
        local_search(&S_new);
        accept(&S, &S_new, &S_best, &i, &local_best_w, c2, c3, c4);
    }

    return S_best;
}

// NOTE TO FUTURE SELF: THERE IS A NOTE ABOUT DIVERSIFICATION AT THE END
// OF PAGE 4 THAT SHOULD BE CONSIDERED

// NOTE TO FUTURE SELF: CHECK PAGE 4 OF THE OTHER PAPER FOR A QUICKER
// IMPLEMENTATION OF DELETION

// NOTE TO FUTURE SELF: PERTURBATION SEEMS TO WORK, AND I SHOULD CONTINUE FROM
// THERE