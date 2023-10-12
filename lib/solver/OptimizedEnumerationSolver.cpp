#include "OptimizedEnumerationSolver.h"

#include <cassert>
#include <array>

using namespace std;

namespace {
    using NodesVector = vector<Circuit::Node *>;
    using ImplicationsVector = vector<array<vector<pair<size_t, bool>>, 2>>;

    void topologySort(const Circuit::Node &node, const Circuit &circuit, vector<size_t> &order, vector<bool> &used) {
        used[node.id] = true;
        for (auto &nextId: {node.input1, node.input2}) {
            if (nextId == (size_t) -1 || used[nextId])
                continue;
            topologySort(circuit[nextId], circuit, order, used);
        }
        order.push_back(node.id);
    }

    void
    buildImplications(const NodesVector &nodes, ImplicationsVector &implications,
                      const vector<size_t> &posInNodes) {
        for (size_t i = 0; i < nodes.size(); i++) {
            if (nodes[i]->isInput)
                continue;
            if (nodes[i]->function.argumentsCount() == 1) {
                array<vector<bool>, 2> argsToGetValue;
                for (bool arg: {false, true}) {
                    argsToGetValue[nodes[i]->function(arg)].push_back(arg);
                }
                for (bool functionRes: {false, true}) {
                    if (argsToGetValue[functionRes].size() != 1)
                        continue;
                    implications[i][functionRes].emplace_back(posInNodes.at(nodes[i]->input1),
                                                              argsToGetValue[functionRes][0]);
                }
            } else if (nodes[i]->function.argumentsCount() == 2) {
                array<vector<pair<bool, bool>>, 2> argsToGetValue;
                for (bool arg1: {false, true}) {
                    for (bool arg2: {false, true}) {
                        argsToGetValue[nodes[i]->function(arg1, arg2)].emplace_back(arg1, arg2);
                    }
                }
                for (bool functionRes: {false, true}) {
                    if (argsToGetValue[functionRes].size() != 1)
                        continue;
                    implications[i][functionRes].emplace_back(posInNodes.at(nodes[i]->input1),
                                                              argsToGetValue[functionRes][0].first);
                    implications[i][functionRes].emplace_back(posInNodes.at(nodes[i]->input2),
                                                              argsToGetValue[functionRes][0].second);
                }
            } else {
                assert(false);
            }
        }
    }

    Circuit::Value evalNode(Circuit::Node &node, Circuit &circuit) {
        if (node.function.argumentsCount() == 1) {
            return node.function(circuit[node.input1].value);
        } else if (node.function.argumentsCount() == 2) {
            return node.function(circuit[node.input1].value,
                                 circuit[node.input2].value);
        } else {
            assert(false);
            return Circuit::Value::Unknown;
        }
    }

    void
    unsetValue(size_t i, NodesVector &nodes, const ImplicationsVector &implications, vector<int> &valueSources) {
        if (nodes[i]->value == Circuit::Value::Unknown)
            return;

        assert(valueSources[i] != -1);
        for (auto &[j, _]: implications[i][int(nodes[i]->value)]) {
            unsetValue(j, nodes, implications, valueSources);
        }
        nodes[i]->value = Circuit::Value::Unknown;
        valueSources[i] = -1;
    }

    int setValue(size_t i, Circuit::Value value, int source, NodesVector &nodes,
                 const ImplicationsVector &implications, Circuit &circuit, const vector<size_t> &posInNodes,
                 vector<int> &valueSources, const vector<vector<size_t>> &parents) {
        assert(value != Circuit::Value::Unknown);
        if (nodes[i]->value != Circuit::Value::Unknown && nodes[i]->value != value) {
            assert(valueSources[i] != -1);
            return (int) valueSources[i];
        }
        if (!nodes[i]->isInput) {
            auto evalNodeValue = evalNode(*nodes[i], circuit);
            if (evalNodeValue != Circuit::Value::Unknown && evalNodeValue != value)
                return int(i) - 1;
        }
        if (nodes[i]->value == value)
            return (int) i;

        nodes[i]->value = value;
        valueSources[i] = source;
        for (size_t parentPosInNodes: parents[i]) {
            auto parentNode = nodes[parentPosInNodes];
            Circuit::Value parentNodeValue = evalNode(*parentNode, circuit);
            if (parentNodeValue == Circuit::Value::Unknown || parentNode->value == Circuit::Value::Unknown)
                continue;
            if (parentNodeValue != parentNode->value) {
                unsetValue(i, nodes, implications, valueSources);
                return (int) parentPosInNodes;
            }
        }
        for (auto [j, jVal]: implications[i][int(value)]) {
            int conflictPos = setValue(j, jVal ? Circuit::Value::True : Circuit::Value::False, int(i), nodes,
                                       implications, circuit, posInNodes, valueSources, parents);
            if (conflictPos == (int) j)
                continue;
            unsetValue(i, nodes, implications, valueSources);
            return min(conflictPos, int(i) - 1);
        }
        return int(i);
    }

    int canNodeHasValue(size_t i, Circuit::Value value, NodesVector &nodes, ImplicationsVector &implications,
                        Circuit &circuit, const vector<size_t> &posInNodes, vector<int> &valueSources,
                        const vector<vector<size_t>> &parents) {
        if (i == nodes.size())
            return (int) nodes.size();
        int conflictPos = setValue(i, value, int(i), nodes, implications, circuit, posInNodes, valueSources, parents);
        if (conflictPos != (int) i)
            return conflictPos;

        int prefFalse = canNodeHasValue(i + 1, Circuit::Value::False, nodes, implications, circuit, posInNodes,
                                        valueSources, parents);
        if (prefFalse == (int) nodes.size())
            return prefFalse;

        int prefTrue = canNodeHasValue(i + 1, Circuit::Value::True, nodes, implications, circuit, posInNodes,
                                       valueSources, parents);
        if (prefTrue == (int) nodes.size())
            return prefTrue;

        unsetValue(i, nodes, implications, valueSources);
        return min(max(prefFalse, prefTrue), int(i) - 1);
    }
}

CircuitSATSolver::Result OptimizedEnumerationSolver::solve(Circuit &circuit) const {
    assert(circuit.outputsCount() == 1);

    vector<size_t> nodeIndexes;
    vector<bool> used(circuit.nodesCount(), false);
    topologySort(circuit.output(0), circuit, nodeIndexes, used);
    reverse(nodeIndexes.begin(), nodeIndexes.end());
    assert(nodeIndexes.front() == circuit.output(0).id);

    NodesVector nodes(nodeIndexes.size());
    vector<size_t> posInNodes(circuit.nodesCount(), -1);
    for (size_t i = 0; i < nodeIndexes.size(); i++) {
        nodes[i] = &circuit[nodeIndexes[i]];
        posInNodes[nodeIndexes[i]] = i;
    }
    vector<vector<size_t>> parents(nodes.size());
    for (size_t i = 0; i < nodes.size(); i++) {
        if (nodes[i]->isInput)
            continue;
        assert(!nodes[i]->function.isNone());
        for (size_t nodeInput: {nodes[i]->input1, nodes[i]->input2}) {
            if (nodeInput == (size_t) -1)
                continue;
            parents[posInNodes[nodeInput]].push_back(i);
        }
    }

    ImplicationsVector implications(nodes.size());
    buildImplications(nodes, implications, posInNodes);
    vector<int> valueSources(nodes.size(), -1);

    int result = canNodeHasValue(0, Circuit::Value::True, nodes, implications, circuit, posInNodes, valueSources,
                                 parents);
    assert(result == -1 || result == (int) nodes.size());
    return result == -1 ? CircuitSATSolver::Result::UnSAT : CircuitSATSolver::Result::SAT;
}
