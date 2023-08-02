#include "ILS-VND.h"

#include <random>

Algorithm::Algorithm(Graph* graph) : graph_(graph), 
        current_sol_(Solution(graph)), best_sol_(Solution(graph)),
        new_sol_(Solution(graph)) {}

void Algorithm::Maximize(Solution& s) {
    std::random_device rd;
    std::mt19937 gen(rd());

    // pick random vertices and insert them into S until it is maximal
    while (!s.IsMaximal()) {
        std::uniform_int_distribution<> dist(s.SolutionSize(),
            s.SolutionSize() + s.FreeSize() - 1);
        s.Insert(s.GetVertex(dist(gen)));
    }
}

void Algorithm::Perturb(int c, Solution& s) {
    std::random_device rd;
    std::mt19937 gen(rd());

    new_sol_ = s;
    // pick c random vertices and insert them into S
    for (int i = 0; i < c; ++i) {
        std::uniform_int_distribution<> dist(new_sol_.SolutionSize(),
                                             graph_->no_nodes() - 1);
        int vertex = new_sol_.GetVertex(dist(gen));
        for (int neighbor : graph_->AdjacencyVector(vertex)) {
            if (new_sol_.IsInSolution(neighbor)) {
                new_sol_.Remove(neighbor);
            }
        }
        
        new_sol_.Insert(vertex);
    }
    
    Maximize(new_sol_);
}

bool Algorithm::FirstImprovement(int k, Solution& s) {
    new_sol_ = s;
    if (k == 1) {
        return new_sol_.N1();
    } else {
        return new_sol_.N2();
    }
}

void Algorithm::LocalSearch(Solution& s) {
    int k = 1;
    while (k <= 2) {
        if (!FirstImprovement(k, s)) {
            ++k;
        } else {
            k = 1;
            s = new_sol_;
            Maximize(s);
        }
    }
}

void Algorithm::Accept(Solution& s, Solution& new_s, Solution& best_s, int& i,
                       int& local_best_w, int c2, int c3, int c4) {
    int weight_s = s.SolutionWeight();
    if (weight_s < new_s.SolutionWeight()) {
        s = new_s;
        i = 1;

        weight_s = s.SolutionWeight();
        if (local_best_w < weight_s) {
            local_best_w = weight_s;
            i = i - s.SolutionSize() / c2;
        }

        if (best_s.SolutionWeight() < weight_s) {
            best_s = s;
            i = i - s.SolutionSize() * c3;
        }
    } else {
        if (i <= s.SolutionSize() / c2) {
            ++i;
        } else {
            local_best_w = weight_s;
            Perturb(c4, s);
            s = new_sol_;
            i = 1;
        }
    }
}

Solution& Algorithm::RunAlgorithm(int max_iter, int c1, int c2, int c3, int c4) {
    Solution& S = current_sol_;
    Solution& S_best = best_sol_;
    Solution& S_new = new_sol_;

    Maximize(S);
    LocalSearch(S);
    S_best = S;

    int local_best_w = S.SolutionWeight();
    int i = 1;
    int no_change = 0;
    int previous_weight = 0;
    while (max_iter-- && no_change != NO_CHANGE) {
        if (!(max_iter % 50000)) {
            std::cout << "Iteration #" << MAX_ITER - max_iter << std::endl;
            std::cout << S_best.SolutionWeight() << std::endl;
        }

        if (S.SolutionWeight() > previous_weight) {
            previous_weight = S.SolutionWeight();
            no_change = 0;
        } else {
            ++no_change;
        }
        if(S_best.SolutionWeight() == 2990) break;

        Perturb(c1, S);
        LocalSearch(S_new);
        Accept(S, S_new, S_best, i, local_best_w, c2, c3, c4);
    }
    
    std::cout << std::endl << "Algorithm ended after " << MAX_ITER - max_iter <<
        " iterations." << std::endl;

    return S_best;
}