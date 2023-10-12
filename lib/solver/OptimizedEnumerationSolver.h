#pragma once

#include "CircuitSATSolver.h"

class OptimizedEnumerationSolver : public CircuitSATSolver {
public:
    Result solve(Circuit &circuit) const override;
};


