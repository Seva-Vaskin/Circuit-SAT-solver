#include "CNFReductionSolver.h"

#include "cryptominisat.h"

using namespace CMSat;
using namespace std;

CircuitSATSolver::Result CNFReductionSolver::solve(Circuit &circuit) const {
    SATSolver solver;
    solver.new_vars(circuit.nodesCount());

    assert(circuit.outputsCount() == 1);
    solver.add_clause({Lit(circuit.output(0).id, false)});

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
                    solver.add_clause(vector<Lit>{Lit(node.id, nodeVal),
                                                  Lit(node.input1, inp1Val)});
                }
            } else {
                for (bool inp1Val: {false, true}) {
                    for (bool inp2Val: {false, true}) {
                        if (nodeVal == node.function(inp1Val, inp2Val))
                            continue;
                        solver.add_clause(vector<Lit>{Lit(node.id, nodeVal),
                                                      Lit(node.input1, inp1Val),
                                                      Lit(node.input2, inp2Val)});
                    }
                }
            }
        }
    }

    lbool result = solver.solve();

    if (result == l_True) {
        const auto &model = solver.get_model();
        for (size_t i = 0; i < circuit.nodesCount(); i++) {
            circuit[i].value = model[i] == l_True ? Circuit::Value::True :
                               model[i] == l_False ? Circuit::Value::False : Circuit::Value::Unknown;
        }
        return Result::SAT;
    } else if (result == l_False)
        return Result::UnSAT;
    else
        return Result::Unknown;
}
