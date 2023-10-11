#include "catch2/catch_test_macros.hpp"

#include "BruteForceSolver.h"
#include "CNFReductionSolver.h"
#include "Circuit.h"

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
