#include <filesystem>
namespace fs = std::filesystem;

#include "test.h"
#include "file.h"

#define NO_TESTS 100

int main(){
    TestParameters parameters;
    std::vector<std::string> tests{"BHOSLIB", "DIMACS"};
    for (auto test : tests) {
        //if(test == "BHOSLIB") continue;
        bool start = false;
        for (const auto & entry :
                fs::recursive_directory_iterator("../test/" + test)) {
            std::string file_path(entry.path().string());
            size_t last_index = file_path.find_last_of("."); 
            std::string name(file_path.substr(0, last_index));

            if (test == "BHOSLIB")
                name.erase(0, std::string("../test/BHOSLIB\\").length());
            else name.erase(0, std::string("../test/DIMACS\\").length());
            
            /*if (!start && test == "BHOSLIB" && name != "frb53-24-5") {
                continue;
            } else if (!start && test == "DIMACS" && name != "MANN-a81") {
                continue;
            } else {
                start = true;
            }*/

            std::cout << "Testing file: '" << name << "'" << std::endl;

            int best_known_solution = parameters.GetValue(name, test);
            Graph graph(file_path, test);
            graph.Complement();

            double avg_time = 0.;
            double avg_best_time = 0.;
            double avg_solution_weight = 0.;
            int avg_iter = 0;
            int avg_best_iter = 0;
            int best_found_solution = 0;

            for (int i = 0; i < NO_TESTS; ++i) {
                if (!((i + 1) % 10)) std::cout << "Test #" << i + 1 << std::endl;
                Algorithm alg(&graph);
                avg_iter += alg.RunAlgorithm(best_known_solution); 
                avg_best_iter += alg.GetBestIter();
                avg_solution_weight += alg.GetSolutionWeight();
                avg_time += alg.GetExecutionTime();
                avg_best_time += alg.GetBestSolutionTime();
                if (alg.GetSolutionWeight() > best_found_solution)
                    best_found_solution = alg.GetSolutionWeight();
            }

            avg_time /= NO_TESTS;
            avg_best_time /= NO_TESTS;
            avg_solution_weight /= NO_TESTS;
            avg_iter /= NO_TESTS;
            avg_best_iter /= NO_TESTS;

            std::cout << "Average time: " << avg_time << "s" << std::endl;
            std::cout << "Best solution average time: " << avg_best_time << "s"
                << std::endl;
            break;
            File file;
            file.WriteResults(test + "_results.txt", name, avg_time, avg_iter,
                              avg_best_time, avg_best_iter, avg_solution_weight,
                              best_found_solution, best_known_solution);
        }
    }
    return 0;
}