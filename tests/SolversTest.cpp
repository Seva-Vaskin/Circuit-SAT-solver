#include "catch2/catch_test_macros.hpp"

#include <random>

#include "BruteForceSolver.h"
#include "CNFReductionSolver.h"
#include "Circuit.h"

using namespace std;

namespace {
    Circuit generateCircuit(mt19937 &rnd, size_t maxInputs, size_t maxInternals) {
        vector<Circuit::Function> functions;
        for (auto &[_, function]: Circuit::Functions::allFunctionsByName)
            functions.push_back(function);
        Circuit circuit;
        size_t inputsCount = rnd() % maxInputs + 1;
        for (size_t j = 0; j < inputsCount; j++) {
            circuit.addInput(to_string(j));
        }
        size_t internalCounts = rnd() % maxInternals + 1;
        for (size_t j = inputsCount; j < internalCounts + inputsCount; j++) {
            auto function = functions[rnd() % functions.size()];
            vector<string> arguments;
            for (size_t k = 0; k < function.argumentsCount(); k++) {
                auto arg = rnd() % j;
                arguments.emplace_back(to_string(arg));
            }
            circuit.addInternal(to_string(j), arguments, function);
        }
        size_t output = rnd() % internalCounts + inputsCount;
        circuit.addOutput(to_string(output));
        return circuit;
    }
}

class SolversTester {
public:
    static void testCircuit_AND(const CircuitSATSolver &solver) {
        Circuit circuit;
        circuit.addInput("0");
        circuit.addInput("1");
        circuit.addOutput("2");
        circuit.addInternal("2", {"0", "1"}, Circuit::Functions::AND);
        REQUIRE(solver.solve(circuit) == CircuitSATSolver::Result::SAT);
        REQUIRE(circuit.input(0).value == Circuit::Value::True);
        REQUIRE(circuit.input(1).value == Circuit::Value::True);
        REQUIRE(circuit.output(0).value == Circuit::Value::True);
    }

    static void testCircuit_SimpleSAT(const CircuitSATSolver &solver) {
        Circuit circuit;
        circuit.addInput("i0");
        circuit.addInput("i1");
        circuit.addInput("i2");
        circuit.addInternal("3", {"i0", "i1"}, Circuit::Functions::AND);
        circuit.addInternal("4", {"i1", "i2"}, Circuit::Functions::AND);
        circuit.addInternal("5", {"i2"}, Circuit::Functions::NOT);
        circuit.addInternal("6", {"3"}, Circuit::Functions::NOT);
        circuit.addInternal("7", {"4", "5"}, Circuit::Functions::OR);
        circuit.addInternal("o8", {"6", "7"}, Circuit::Functions::AND);
        circuit.addOutput("o8");
        REQUIRE(solver.solve(circuit) == CircuitSATSolver::Result::SAT);
        for (size_t i = 0; i < circuit.nodesCount(); i++) {
            REQUIRE(circuit[i].value != Circuit::Value::Unknown);
        }
        REQUIRE(bool(circuit[3].value) == (bool(circuit[0].value) && bool(circuit[1].value)));
        REQUIRE(bool(circuit[4].value) == (bool(circuit[1].value) && bool(circuit[2].value)));
        REQUIRE(bool(circuit[5].value) == !bool(circuit[2].value));
        REQUIRE(bool(circuit[6].value) == !bool(circuit[3].value));
        REQUIRE(bool(circuit[7].value) == (bool(circuit[4].value) || bool(circuit[5].value)));
        REQUIRE(bool(circuit[8].value) == (bool(circuit[6].value) && bool(circuit[7].value)));
        REQUIRE(circuit.output(0).value == Circuit::Value::True);
    }

    static void testCircuit_SimpleUnSAT(const CircuitSATSolver &solver) {
        Circuit circuit;
        circuit.addInput("i0");
        circuit.addInput("i1");
        circuit.addInternal("2", {"i0", "i1"}, Circuit::Functions::XOR);
        circuit.addInternal("3", {"i0", "i1"}, Circuit::Functions::NXOR);
        circuit.addInternal("o3", {"2", "3"}, Circuit::Functions::AND);
        circuit.addOutput("o3");
        REQUIRE(solver.solve(circuit) == CircuitSATSolver::Result::UnSAT);
    }
};

TEST_CASE("BruitForceSolverTest-AND", "[AND]") {
    SolversTester::testCircuit_AND(BruteForceSolver());
}

TEST_CASE("BruitForceSolverTest-SimpleSAT", "[SimpleSAT]") {
    SolversTester::testCircuit_SimpleSAT(BruteForceSolver());
}

TEST_CASE("BruitForceSolverTest-SimpleUnSAT", "[SimpleUnSAT]") {
    SolversTester::testCircuit_SimpleUnSAT(BruteForceSolver());
}

TEST_CASE("CNFReductionSolverTest-AND", "[AND]") {
    SolversTester::testCircuit_AND(CNFReductionSolver());
}

TEST_CASE("CNFReductionSolverTest-SimpleSAT", "[SimpleSAT]") {
    SolversTester::testCircuit_SimpleSAT(CNFReductionSolver());
}

TEST_CASE("CNFReductionSolverTest-SimpleUnSAT", "[SimpleUnSAT]") {
    SolversTester::testCircuit_SimpleUnSAT(CNFReductionSolver());
}

TEST_CASE("StressTestAllSolvers", "[stress]") {
    const static vector<shared_ptr<CircuitSATSolver>> solvers = {make_shared<BruteForceSolver>(),
                                                                 make_shared<CNFReductionSolver>()};
    bool wasUnSAT = false, wasSAT = false;
    mt19937 rnd(0);

    for (int i = 0; i < 500; i++) {
        Circuit circuit = generateCircuit(rnd, 10, 100);
        CircuitSATSolver::Result result = CircuitSATSolver::Result::Unknown;
        for (auto &solver: solvers) {
            Circuit circuitCopy = circuit;
            auto currentResult = solver->solve(circuitCopy);
            REQUIRE(currentResult != CircuitSATSolver::Result::Unknown);
            if (result == CircuitSATSolver::Result::Unknown)
                result = currentResult;
            else
                REQUIRE(currentResult == result);
        }
        wasSAT |= result == CircuitSATSolver::Result::SAT;
        wasUnSAT |= result == CircuitSATSolver::Result::UnSAT;
    }

    REQUIRE(wasUnSAT);
    REQUIRE(wasSAT);
}
