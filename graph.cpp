#include "graph.h"

void exit(){
    std::cout << "Aborted!" << std::endl;
    std::exit(0);
}

template<typename T>
void print(T container, std::string pre = "NULL"){
    if(pre != "NULL") std::cout << pre;
    for(auto it = container.begin(); it != container.end(); ++it)
        std::cout << *it << " ";
    std::cout << std::endl;
}

Graph::Solution::Solution(Graph* graph) : graph(graph),
        sol_neighs(std::vector<std::list<int>>(graph->no_nodes)),
        pointers(std::vector<std::vector<std::list<int>::iterator>>(
            graph->no_nodes, std::vector<std::list<int>::iterator>(
                graph->no_nodes))){
    int no_nodes = graph->get_no_nodes();
    for(int i = 0; i < no_nodes; ++i){
        S.push_back(i);
        perm.push_back(i);
    }
    S_size = 0;
    free_size = no_nodes;
    tightness.assign(no_nodes, 0);
    mi = graph->get_weights();
}

Graph::Solution::Solution(const Solution& sol) : graph(sol.graph), S(sol.S),
        perm(sol.perm), tightness(sol.tightness), S_size(sol.S_size),
        free_size(sol.free_size), mi(sol.mi), candidates(sol.candidates),
        sol_neighs(sol.sol_neighs),
        pointers(std::vector<std::vector<std::list<int>::iterator>>(
            graph->no_nodes, std::vector<std::list<int>::iterator>(
                graph->no_nodes))){
    // pointers have to point to the new vector "sol_neighs", therefore the
    // vector "pointers" has to be manually constructed
    for(int v = 0; v < graph->get_no_nodes(); ++v){
        for(auto x = sol_neighs[v].begin(); x != sol_neighs[v].end(); ++x){
            pointers[*x][v] = x; 
        }
    }
}

void swap(Graph::Solution& fst, Graph::Solution& snd){
    using std::swap;

    swap(fst.graph, snd.graph);
    swap(fst.S, snd.S);
    swap(fst.perm, snd.perm);
    swap(fst.tightness, snd.tightness);
    swap(fst.S_size, snd.S_size);
    swap(fst.free_size, snd.free_size);
    swap(fst.mi, snd.mi);
    swap(fst.candidates, snd.candidates);
    swap(fst.sol_neighs, snd.sol_neighs);
    swap(fst.pointers, snd.pointers);
}

Graph::Solution& Graph::Solution::operator=(Graph::Solution rhs){
    swap(*this, rhs);
    return *this;
}

void Graph::Solution::move_vertex(int vertex, int location){
    S[perm[vertex]] = S[location];
    perm[S[location]] = perm[vertex];
    perm[vertex] = location;
    S[location] = vertex;
}

void Graph::Solution::insert(int vertex){
    //::print(tightness, "Tightness before insertion: ");
    // if the vertex is free, reduce the size of the free bucket
    if(is_free(vertex)) --free_size;
    
    // if the solution isn't maximal, there still exists a free vertex
    // that we have to keep inside the free vertex bucket
    else if(!is_maximal()) move_vertex(S[S_size], S_size + free_size);

    // place the vertex into the solution
    move_vertex(vertex, S_size++);
    // update the tightness of its neighbors and the array mi and the
    // sol_neighs vectors
    for(auto it = graph->adj_matrix[vertex].begin();
            it != graph->adj_matrix[vertex].end(); ++it){
        mi[*it] -= graph->get_weight(vertex);
        if(!(tightness[*it]++) && is_free(*it))
            move_vertex(*it, free_size-- + S_size - 1);
        
        sol_neighs[*it].push_back(vertex);
        pointers[vertex][*it] = sol_neighs[*it].end();
        std::advance(pointers[vertex][*it], -1);
        /*std::cout << "Insertion of vertex " << vertex << " increased the "
                "tightness of vertex " << *it << std::endl;*/
    }

    //::print(tightness, "Tightness after insertion: ");

    // update the candidate list
    candidates.insert(vertex);
    /*std::cout << "Inserting vertex " << vertex << " into the candidate list"
        " due to its insertion into the solution" << std::endl;*/
}

void Graph::Solution::remove(int vertex){
    //::print(tightness, "Tightness before removal: ");
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

    // update the tightness of its neighbors, update the candidate list and
    // update array mi
    for(auto it = graph->adj_matrix[vertex].begin();
            it != graph->adj_matrix[vertex].end(); ++it){
        mi[*it] += graph->get_weight(vertex);
        if(!(--tightness[*it]) && is_non_free(*it))
            move_vertex(*it, S_size + free_size++);
        
        /*::print(sol_neighs[*it], "Neighs: ");
        std::cout << "vertex: " << vertex << ", it: " << *it
            << ", pointer: " << *pointers[vertex][*it] << std::endl;*/
        
        sol_neighs[*it].erase(pointers[vertex][*it]);
        /*std::cout << "Removal of vertex " << vertex << " decreased the "
                "tightness of vertex " << *it << std::endl;*/
    }
    //::print(tightness, "Tightness after removal: ");
    for(auto it = graph->adj_matrix[vertex].begin();
            it != graph->adj_matrix[vertex].end(); ++it){
        if(tightness[*it] == 1){
            // find the neighbor of *it that is in the solution and add it to
            // the candidate list
            for(auto neigh = graph->adj_matrix[*it].begin();
                    neigh != graph->adj_matrix[*it].end(); ++neigh){
                if(is_in_solution(*neigh)){
                    candidates.insert(*neigh);
                    /*std::cout << "Inserting vertex " << *neigh << " due to it"
                        " being a neighbor of a 1-tight nonsolution vertex"
                        << *it << std::endl;*/
                }
            }
        }
    }
}

const std::vector<int> Graph::Solution::get_solution(){
    std::vector<int> vec(&S[0], &S[size()]);
    std::sort(vec.begin(), vec.end());
    return vec;
}

const std::vector<int> Graph::Solution::get_solution_c(){
    std::vector<int> vec(&S[size()], &S[graph->get_no_nodes()]);
    std::sort(vec.begin(), vec.end());
    return vec;
}

void Graph::Solution::N1(std::vector<bool>* del){
    // insert a single vertex v from S_c into S and delete its neighbors
    // from S
    bool first = true;
    int i = S_size;
    while(first || i < graph->get_no_nodes()){
        int vertex = get_vertex(i);
        if((first && !(*del)[vertex]) || (!first && (*del)[vertex])){
            if(get_mi(vertex) > 0){
                insert(vertex);
                //std::cout << "N1: " << vertex << std::endl;
                for(auto it = graph->adj_matrix[vertex].begin();
                        it != graph->adj_matrix[vertex].end(); ++it)
                    if(is_in_solution(*it)) remove(*it);
                break;
            }
        }

        ++i;
        if(first && i == graph->get_no_nodes()){
            first = false;
            i = S_size;
        }
    }
}

void Graph::Solution::N2(int* ins){
    bool removed = false;/*
    ::print(candidates, "Candidates: ");
    ::print(tightness, "Tightness: ");
    ::print(get_solution(), "Solution: ");*/
    // delete one vertex from S and insert two vertices from S_c
    for(auto x = candidates.begin(); x != candidates.end();){
        bool found = false;
        if(*ins == *x){
            removed = true;
            ++x;
            continue;
        }
        
        if(!is_in_solution(*x)){
            candidates.erase(*x++);
            continue;
        }

        // build a list L(x), consisting of 1-tight neighbors of x
        std::list<int> L;
        for(auto neigh = graph->adj_matrix[*x].begin();
                neigh != graph->adj_matrix[*x].end(); ++neigh)
            if(tightness[*neigh] == 1) L.push_back(*neigh);

        if(L.size() > 1){
            // find a pair {v, w} in L(x), such that there is no edge between
            // v and w
            int w = -1;
            auto v = L.begin();
            for(; v != L.end(); ++v){
                auto w_candidate = L.begin();
                auto N = graph->adj_matrix[*v].begin();
            
                while(w_candidate != L.end()){
                    if(w_candidate == v){
                        ++w_candidate;
                        continue;
                    }
                    while(N != graph->adj_matrix[*v].end() && *N < *w_candidate)
                        ++N;
                    if(N != graph->adj_matrix[*v].end() && *N == *w_candidate){
                        ++N;
                        ++w_candidate;
                    }
                    else{
                        w = *w_candidate;
                        break;
                    }
                }
                
                if(w != -1){ 
                    insert(*v);
                    insert(w);
                    remove(*x);
                    candidates.erase(x);
                    found = true;
                    /*std::cout << "v: " << *v << ", w: " << w << ", x: " << *x
                        << std::endl;*/
                    break;
                }
            }

            if(!found) candidates.erase(x++);
            else break;
        }

        else candidates.erase(x++);
    }

    if(removed){
        candidates.insert(*ins);
        int tmp = -1;
        N2(&tmp);
    }
}

bool Graph::Solution::check(){
    bool ret = true;
    std::cout << "Checking solution!" << std::endl;
    std::vector<int> sol = get_solution();
    for(auto v = sol.begin(); v != sol.end(); ++v){
        for(auto w = sol.begin(); w != sol.end(); ++w){
            if(v == w) continue;
            if(graph->exists_edge(*v, *w)){
                std::cout << *v << " " << *w << std::endl;
                ret = false;    
            }
        }
    }
    if(ret) std::cout << "Valid solution!" << std::endl;
    else std::cout << "Invalid solution!" << std::endl;
    return ret;
}

Graph::Graph(int no_nodes, std::vector<std::vector<int>> adj_matrix) :
                no_nodes(no_nodes), adj_matrix(adj_matrix){
    for(int i = 0; i < no_nodes; ++i){
        weights.push_back((i + 1) % 200 + 1);
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
        
        adj_matrix = std::vector<std::vector<int>>(no_nodes);

        for(int i = 0; i < no_nodes; ++i){
            weights.push_back((i + 1) % 200 + 1);
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

void Graph::complement(){
    std::vector<std::vector<int>> new_adj_matrix(no_nodes);
    for(int v = 0; v < no_nodes; ++v){
        // go over all vertices w and see if there is and edge {v, w},
        // if there isn't, add it to the new adjacency matrix
        auto w_it = adj_matrix[v].begin();
        for(int w = 0; w < no_nodes; ++w){
            if(v == w) continue;
            if(w_it == adj_matrix[v].end() || *w_it > w)
                new_adj_matrix[v].push_back(w);
            else ++w_it;
        }
    }
    adj_matrix = new_adj_matrix;
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

Graph::Solution Graph::perturb(int c, Solution sol, std::vector<bool>& del,
                                int& ins){
    std::random_device rd;
    std::mt19937 gen(rd());
    
    // pick c random vertices and insert them into S
    for(int i = 0; i < c; ++i){
        std::uniform_int_distribution<> dist(sol.size(), no_nodes - 1);
        int vertex = sol.get_vertex(dist(gen));
        sol.insert(vertex);
        ins = vertex;

        // remove their neighbors from the solution S
        for(auto it = adj_matrix[vertex].begin();
                it != adj_matrix[vertex].end(); ++it)
            if(sol.is_in_solution(*it)){
                sol.remove(*it);
                del[*it] = true;
            }
    }
    
    // maximize the solution S
    maximize(sol);
    return sol;
}

Graph::Solution Graph::first_improvement(int k, Solution& sol,
                                            std::vector<bool>* del, int* ins){
    if(k == 1) sol.N1(del);
    else sol.N2(ins);
    return sol;
}

void Graph::local_search(Solution& sol, std::vector<bool>* del, int* ins){
    int k = 1;
    while(k <= 2){
        Solution new_sol = first_improvement(k, sol, del, ins);

        if(weight(new_sol) <= weight(sol)) ++k;
        else{
            k = 1;
            sol = new_sol;
            maximize(sol);
        }
    }
}

void Graph::accept(Solution& sol, Solution& new_sol, Solution& best_sol, int& i,
                    int& local_best_w, int c2, int c3, int c4){
    int S_weight = weight(sol);
    if(S_weight < weight(new_sol)){
        sol = new_sol;
        i = 1;

        S_weight = weight(sol);
        if(local_best_w < S_weight){
            local_best_w = S_weight;
            i = i - sol.size() / c2;
        }

        if(weight(best_sol) < S_weight){
            best_sol = sol;
            i = i - sol.size() * c3;
        }
    }

    else{
        if(i <= sol.size() / c2) ++i;
        else{
            local_best_w = S_weight;
            std::vector<bool> del(no_nodes, false);
            int ins = -1;
            sol = perturb(c4, sol, del, ins);
            i = 1;
        }
    }
}

Graph::Solution Graph::algorithm(int max_iter, int c1, int c2, int c3, int c4){
    Solution S(this);
    
    maximize(S);

    std::cout << "Initialization:" << std::endl;
    ::print(S.get_solution(), "S_0: "); ::print(S.get_solution_c(), "S_0_c: ");
    std::cout << "Begin local search" << std::endl;
    std::vector<bool> del(no_nodes, false);
    int ins = -1;
    local_search(S, &del, &ins);
    std::cout << "Local search done" << std::endl;
    Solution S_best = S;

    int local_best_w = weight(S);
    int i = 1;
    int no_change = 0;
    int previous_weight = 0;
    while(max_iter-- && no_change != NO_CHANGE){
        if(!(max_iter % 100)){
            std::cout << "Iteration #" << MAX_ITER - max_iter << std::endl;
            std::cout << weight(S_best) << std::endl;
            if(weight(S) > previous_weight){
                previous_weight = weight(S);
                no_change = 0;
            }
            else ++no_change;
        }
        std::vector<bool> div_deleted(no_nodes, false);
        int div_inserted;
        //std::cout << "PER:" << std::endl;
        Solution S_new = perturb(c1, S, div_deleted, div_inserted);
        //std::cout << "LS:" << std::endl;
        local_search(S_new, &div_deleted, &div_inserted);
        //std::cout << "ACC:" << std::endl;
        accept(S, S_new, S_best, i, local_best_w, c2, c3, c4);
        //std::cout << "END" << std::endl;
    }
    
    return S_best;
}

// NOTE TO FUTURE SELF: CHECK WHAT IS THE SLOWEST PART OF THE CODE AFTER
// IMPLEMENTING THE POINTER ARRAY

// NOTE TO FUTURE SELF:: I STILL HAVENT USED THE POINTER ARRAY IN THE DELETION
// OF A VERTEX TO SPEED UP THE LAST PART, ALSO, THE POINTER ARRAY BEING THE
// THAT IT IS MIGHT BE TOO SLOW FOR COPYING, THEREFORE I MIGHT HAVE TO CHANGE IT
// TO BEING TOGETHER WITH THE ARC (x, v) IN x's ADJACENCY LIST, AS THE OTHER
// PAPER SUGGESTS ON PAGE 4