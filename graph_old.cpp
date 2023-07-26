#include "graph_old.h"

void Graph::Solution::change(int vertex, bool increase){
    // for each neighbor i of vertex, we decrease mi[i] by
    // weights[vertex]
    if(increase){ // deletion
        for(int i = 0; i < graph.no_nodes; ++i){
            if(graph.neigh_matrix[vertex][i])
                mi[i] += graph.get_weight(vertex);
        }
    }
    else{ // insertion
        for(int i = 0; i < graph.no_nodes; ++i)
            if(graph.neigh_matrix[vertex][i])
                mi[i] -= graph.get_weight(vertex);
    }
}

void Graph::Solution::insert(std::set<int> vertices){
    for(auto it = vertices.begin(); it != vertices.end(); ++it){
        S.push_back(*it);
        change(*it, false);
        candidates.push_front(*it);
    }

    RemoveVertices rm(vertices);
    S_c.erase(std::remove_if(S_c.begin(), S_c.end(), rm), S_c.end());
}

bool Graph::Solution::solution_contains(int vertex){
    for(auto it = S.begin(); it != S.end(); ++it) 
        if(*it == vertex) return true;
    return false;
}

std::list<int> Graph::Solution::find_solution_neighbors(int vertex){
    std::list<int> list;
    for(auto it = adj_matrix[vertex].begin(); it != adj_matrix[vertex].end();
            ++it){
        if(solution_contains((*it).first)) list.push_back((*it).first);
    }

    return list;
}

void Graph::Solution::insertc(std::set<int> vertices){
    for(auto it = vertices.begin(); it != vertices.end(); ++it){
        S_c.push_back(std::make_pair(*it, find_solution_neighbors(*it)));
        change(*it, true);
        for(auto neigh = adj_matrix[*it].begin();
                neigh != adj_matrix[*it].end(); ++neigh){
            if(*std::next((*neigh).second) == NULL &&
                *std::prev((*neigh).second) == NULL)
        }
    }
    
    RemoveVertices rm(vertices);
    S.erase(std::remove_if(S.begin(), S.end(), rm), S.end()); 
}

int Graph::weight(Solution& sol){
    int weight = 0;
    for(auto it = sol.S.begin(); it != sol.S.end(); ++it)
        weight += get_weight(*it);
    return weight;
}

void Graph::maximize(Solution& sol){
    // sort and shuffle S_c and then go over all of the vertices inside it,
    // emplacing valid ones into S
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::shuffle(sol.S_c.begin(), sol.S_c.end(),
                    std::default_random_engine(seed));

    std::set<int> vertices;
    for(int i = 0; i < sol.S_c.size(); ++i){
        // check if the new vertex is a neighbor of any vertices in S_0,
        // if not, add it to S_0
        int v = sol.S_c[i];
        bool good = true;
        for(auto it = sol.S.begin(); good && it != sol.S.end(); ++it)
            if(exists_edge(v, *it)) good = false;
        
        if(good) vertices.insert(v);
    }

    sol.insert(vertices);
}

Graph::Solution Graph::perturb(int c, Solution sol){
    // shuffle S_c to get c1 non-solution vertices
    std::sort(sol.S_c.begin(), sol.S_c.end());
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::shuffle(sol.S_c.begin(), sol.S_c.end(),
                    std::default_random_engine(seed));

    // insert the first c1 vertices from S_c into S and remove their neighbors
    // from S
    std::set<int> vertices;
    std::set<int> neighbors;
    for(int i = 0; i < c; ++i){
        for(auto it = sol.S.begin(); it != sol.S.end(); ++it)
            if(exists_edge(sol.S_c[i], *it)) neighbors.insert(*it);
        
        vertices.insert(sol.S_c[i]);
    }

    sol.insert(vertices);
    sol.insertc(neighbors);

    // complete the set S until it becomes maximal
    maximize(sol);
    return sol;
}

Graph::Solution Graph::first_improvement(int k, Solution& sol){
    // Neighborhood N_1
    if(k == 1){
        // insert a single vertex v from S_c into S and delete its neighbors
        // from S
        std::set<int> vertices;
        std::set<int> neighbors;

        for(auto it = sol.S_c.begin(); it != sol.S_c.end(); ++it){
            if(sol.mi[*it] > 0){
                vertices.insert(*it);
                for(auto neigh = sol.S.begin(); neigh != sol.S.end(); ++neigh)
                    if(exists_edge(*it, *neigh)) neighbors.insert(*neigh);
                
                sol.insert(vertices);
                sol.insertc(neighbors);

                break;
            }
        }
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
            *i = *i - sol->S.size() / c2;
        }

        if(S_best_weight < S_weight){
            best_sol = sol;
            *i = *i - sol->S.size() * c3;
        }
    }

    else{
        if(*i <= sol->S.size() / c2) ++*i;
        else{
            *local_best_w = S_weight;
            *sol = perturb(c4, *sol);
            *i = 1;
        }
    }
}

template<typename T>
void print(T container, std::string pre = "NULL"){
    if(pre != "NULL") std::cout << pre;
    for(auto it = container.begin(); it != container.end(); ++it)
        std::cout << *it << " ";
    std::cout << std::endl;
}

Graph::Solution Graph::algorithm(int max_iter = 100, int c1 = 1, int c2 = 3,
                                    int c3 = 4, int c4 = 2){
    Solution S(*this);
    
    maximize(S);
    std::cout << "Initialization:" << std::endl;
    print(S.S, "S_0: "); print(S.S_c, "S_0_c: ");

    local_search(&S);
    Solution S_best = S;

    int local_best_w = weight(S);
    int i = 1;
    while(max_iter--){
        Solution S_new = perturb(c1, S);
        std::cout << "Iteration #" << 100 - max_iter << std::endl;
        print(S.S, "S_0: "); print(S.S_c, "S_0_c: ");

        local_search(&S_new);
        accept(&S, &S_new, &S_best, &i, &local_best_w, c2, c3, c4);
    }

    return S_best;
}

// NOTE TO FUTURE SELF: THERE IS A NOTE ABOUT DIVERSIFICATION AT THE END
// OF PAGE 4 THAT SHOULD BE CONSIDERED