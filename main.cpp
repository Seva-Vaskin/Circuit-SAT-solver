#include <filesystem>
#include <iostream>
#include <map>
#include <memory>
#include <future>

#include "BenchParser.h"
#include "BruteForceSolver.h"
#include "CNFReductionSolver.h"
#include "OptimizedEnumerationSolver.h"

using namespace std;

static const map<string, shared_ptr<CircuitSATSolver>> stringToSolver = {
        {"BruteForceSolver",           make_shared<BruteForceSolver>()},
        {"CNFReductionSolver",         make_shared<CNFReductionSolver>()},
        {"OptimizedEnumerationSolver", make_shared<OptimizedEnumerationSolver>()}
};

int main(int argc, char *argv[]) {
    if (argc != 4) {
        cerr << "Usage: CircuitSATSolver solverName testFile timeLimit" << endl;
        cerr << endl;
        cerr << "AvailableSolvers: " << endl;
        for (auto &[solverName, _]: stringToSolver) {
            cerr << "\t * " << solverName << endl;
        }
        return 1;
    }

    string solverName = argv[1];
    shared_ptr<CircuitSATSolver> solver;
    try {
        solver = stringToSolver.at(solverName);
    }
    catch (out_of_range &e) {
        cerr << e.what() << endl;
        return 1;
    }

    filesystem::path testFile = argv[2];

    Circuit circuit;
    try {
        circuit = BenchParser::parseFile(testFile);
    }
    catch (runtime_error &e) {
        cerr << e.what() << endl;
        return 1;
    }

    int timeLimit;
    try {
        timeLimit = stoi(argv[3]);
    }
    catch (invalid_argument &e) {
        cerr << e.what() << endl;
        return 1;
    }

    auto startTime = chrono::high_resolution_clock::now();
    auto task = async(launch::async, [&solver, &circuit]() {
        return solver->solve(circuit);
    });
    auto status = task.wait_for(chrono::seconds(timeLimit));

    CircuitSATSolver::Result result = status == future_status::ready ? task.get() : CircuitSATSolver::Result::Unknown;
    auto finishTime = chrono::high_resolution_clock::now();
    chrono::duration<double> duration = finishTime - startTime;
    string resultStr = result == CircuitSATSolver::Result::SAT ? "SAT" :
                       result == CircuitSATSolver::Result::UnSAT ? "UnSAT" : "Unknown";
    cout.precision(2);
    cout.setf(ios_base::fixed);
    cout << setw(7) << resultStr << " " << testFile.filename().string() << " [" << duration.count() << "] " << endl;
    exit(0);
}