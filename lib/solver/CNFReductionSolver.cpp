#include "CNFReductionSolver.h"

#include "cryptominisat.h"

using namespace CMSat;
using namespace std;

CircuitSATSolver::Result CNFReductionSolver::solve(Circuit &circuit) {
    SATSolver solver;
    solver.set_num_threads(1);

    assert(circuit.outputsCount() == 1);
    auto output = circuit.output(0);
    solver.add_clause(vector<Lit>{Lit(output.id, true)});

    for (size_t i = 0; i < circuit.nodesCount(); i++) {
        auto &node = circuit[i];
        if (node.isInput)
            continue;
        assert(!node.function.isNone() && node.input1 != (size_t) -1);
        for (bool nodeVal: {false, true}) {
            if (node.input2 == (size_t) -1) {
                for (bool inp1Val: {false, true}) {
                    if (nodeVal == node.function(inp1Val))
                        continue;
                    solver.add_clause(vector<Lit>{Lit(node.id, !nodeVal),
                                                  Lit(node.input1, !inp1Val)});
                }
            } else {
                for (bool inp1Val: {false, true}) {
                    for (bool inp2Val: {false, true}) {
                        if (nodeVal == node.function(inp1Val, inp2Val))
                            continue;
                        solver.add_clause(vector<Lit>{Lit(node.id, !nodeVal),
                                                      Lit(node.input1, !inp1Val),
                                                      Lit(node.input2, !inp2Val)});
                    }
                }
            }
        }
    }

    lbool result = solver.solve();

    if (result == l_True)
        return Result::SAT;
    else if (result == l_False)
        return Result::UnSAT;
    else
        return Result::Unknown;
}
