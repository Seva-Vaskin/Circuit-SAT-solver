#pragma once

#include "CircuitSATSolver.h"

class BruteForceSolver : public CircuitSATSolver {
public:
    Result solve(Circuit &circuit) const override;
};
