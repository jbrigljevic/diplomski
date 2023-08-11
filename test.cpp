#include <filesystem>
namespace fs = std::filesystem;

#include "test.h"
#include "file.h"

#define NO_TESTS 100

int main(){
    TestParameters parameters;
    bool start = false;
    for (const auto & entry :
            fs::recursive_directory_iterator("../test/BHOSLIB")) {
        std::string file_path(entry.path().string());
        size_t last_index = file_path.find_last_of("."); 
        std::string name(file_path.substr(0, last_index));
        name.erase(0, std::string("../test/BHOSLIB\\").length());
        
        if (name == std::string("frb100-40") ||
            name == std::string("source")) continue;
        if (!start && name != "frb40-19-1") { continue; }
        else start = true;
        std::cout << "Testing file: '" << name << "'" << std::endl;

        int best_known_solution = parameters.GetValue(name, "BHOSLIB");
        Graph graph(file_path, "BHOSLIB");
        graph.Complement();

        double avg = 0.;
        double found_solution_avg = 0.;
        int iter = 0;

        for (int i = 0; i < NO_TESTS; ++i) {
            if (!((i + 1) % 10)) std::cout << "Test #" << i + 1 << std::endl;
            Algorithm alg(&graph);
            iter += alg.RunAlgorithm(best_known_solution);
            found_solution_avg += alg.GetSolutionWeight();
            avg += alg.GetExecutionTime();
        }

        avg /= NO_TESTS;
        found_solution_avg /= NO_TESTS;
        iter /= NO_TESTS;

        std::cout << "Average time: " << avg << "s" << std::endl;
        
        File file;
        file.WriteResults("BHOSLIB_results.txt", name, avg, best_known_solution,
                          found_solution_avg, iter);
    }

    return 0;
}