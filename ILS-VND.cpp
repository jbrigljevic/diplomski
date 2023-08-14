#include "ILS-VND.h"

Algorithm::Algorithm(Graph* graph) : graph_(graph), 
        current_sol_(Solution(graph)), best_sol_(Solution(graph)),
        new_sol_(Solution(graph)), gen(rd()) {}

void Algorithm::Maximize(Solution& s) {
    // pick random vertices and insert them into S until it is maximal
    while (!s.IsMaximal()) {
        std::uniform_int_distribution<> dist(0, s.FreeSize() - 1);
        s.Insert(s.GetVertex(s.SolutionSize() + dist(gen)));
    }
}

void Algorithm::Perturb(int c, Solution& s) {
    new_sol_ = s;
    // pick c random vertices and insert them into S
    for (int i = 0; i < c; ++i) {
        std::uniform_int_distribution<> dist(new_sol_.SolutionSize(),
                                             graph_->no_nodes() - 1);
        int vertex = new_sol_.GetVertex(dist(gen));
        if (!new_sol_.IsFree(vertex)) {
            for (int neighbor : graph_->AdjacencyVector(vertex)) {
                if (new_sol_.IsInSolution(neighbor)) {
                    new_sol_.Remove(neighbor);
                }
            }
        }
        
        new_sol_.Insert(vertex);
    }
    
    Maximize(new_sol_);
}

void Algorithm::LocalSearch(Solution& s) {
    int k = 0;
    bool first_improvement;
    while (k < 2) {
        if (!k) {
            first_improvement = s.N1();
        } else {
            first_improvement = s.N2();
        }

        if (!first_improvement) {
            ++k;
        } else {
            k = 0;
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

int Algorithm::RunAlgorithm(int best_known_solution, int c1, int c2, int c3,
                               int c4) {
    auto start = std::chrono::high_resolution_clock::now();
    Solution& S = current_sol_;
    Solution& S_best = best_sol_;
    Solution& S_new = new_sol_;

    Maximize(S);
    LocalSearch(S);
    S_best = S;

    int local_best_w = S.SolutionWeight();
    int i = 1;
    int iter = 0;
    while (iter++ < MAX_ITER) {
        /*if (!(iter % 50000)) {
            std::cout << "Iteration #" << iter << std::endl;
            std::cout << S_best.SolutionWeight() << std::endl;
        }*/

        Perturb(c1, S);
        LocalSearch(S_new);
        Accept(S, S_new, S_best, i, local_best_w, c2, c3, c4);
        if (S_best.SolutionWeight() == best_known_solution) break;
    }

    auto end = std::chrono::high_resolution_clock::now();
    double time = std::chrono::duration_cast<std::chrono::microseconds>
                  (end - start).count() / 1e6;

    /*std::cout << std::endl << "Algorithm ended after " << iter <<
        " iterations." << std::endl;
    std::cout << "CPU time: " << time << "s" << std::endl << std::endl;
    print(S_best.GetSolution(), "Solution: ");
    std::cout << "Result: " << S_best.SolutionWeight() << std::endl;*/
    if (!S_best.Check()) std::cout << "Invalid solution!" << std::endl;

    execution_time_ = time;

    return iter;
}