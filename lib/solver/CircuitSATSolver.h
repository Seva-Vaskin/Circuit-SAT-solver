#pragma once

#include "Circuit.h"

class CircuitSATSolver {
public:
    enum class Result {
        SAT,
        UnSAT,
        Unknown
    };

    virtual Result solve(Circuit &circuit) = 0;

    virtual ~CircuitSATSolver() = default;
};
