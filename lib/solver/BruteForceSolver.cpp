#include "BruteForceSolver.h"

#include <cassert>

using namespace std;

namespace {
    void updateNodeValue(Circuit &circuit, Circuit::Node &node, uint32_t mask) {
        Circuit::Value newValue;
        if (node.isInput)
            newValue = Circuit::Value(bool(mask & (1 << node.id)));
        else if (node.input2 == (size_t) -1) {
            newValue = node.function(circuit[node.input1].value);
        } else {
            assert(node.input1 != (size_t) -1 && node.input1 != (size_t) -1);
            newValue = node.function(circuit[node.input1].value, circuit[node.input2].value);
        }
        if (node.value == newValue)
            return;
        node.value = newValue;
        for (size_t i = 0; i < circuit.parentsCount(node.id); i++) {
            auto &parent = circuit.parent(node.id, i);
            updateNodeValue(circuit, parent, mask);
        }
    }

    uint32_t ctz(uint32_t n) {
        return n == 0 ? sizeof(uint32_t) : __builtin_ctz(n);
    }
}

CircuitSATSolver::Result BruteForceSolver::solve(Circuit &circuit) const {
    size_t inputsCount = circuit.inputsCount();
    if (inputsCount > 30) {
        // Do not try to brute force big circuits
        return CircuitSATSolver::Result::Unknown;
    }
    assert(circuit.outputsCount() == 1);
    auto &output = circuit.output(0);

    for (uint32_t mask = 0; mask < (uint32_t(1) << inputsCount); mask++) {
        uint32_t updatedValuesMask = mask ^ (mask - 1);
        for (uint32_t currentInput = ctz(updatedValuesMask);
             currentInput < inputsCount; currentInput = ctz(updatedValuesMask)) {
            updatedValuesMask ^= (uint32_t(1) << currentInput);
            updateNodeValue(circuit, circuit[currentInput], mask);
        }
        if (output.value == Circuit::Value::True)
            return CircuitSATSolver::Result::SAT;
    }
    return CircuitSATSolver::Result::UnSAT;
}
