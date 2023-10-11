#pragma once

#include "CircuitSATSolver.h"

class CNFReductionSolver : public CircuitSATSolver {
    Result solve(Circuit &circuit) const override;
};
