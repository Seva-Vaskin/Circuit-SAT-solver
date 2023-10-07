#include "Circuit.h"

#include <fstream>
#include <algorithm>
#include <cassert>

using namespace std;

namespace {
    void deleteSpaces(string &s) {
        while (true) {
            auto it = find_if(s.begin(), s.end(), [](char c) {
                return isspace(c);
            });
            if (it == s.end())
                break;
            s.erase(it);
        }
    }
}

void Circuit::addInput(const std::string &inputName) {
    auto &node = addNode(inputName);
    if (!node.isInput) {
        node.isInput = true;
        _inputs.emplace_back(node.id);
    }
}

void Circuit::addOutput(const std::string &outputName) {
    auto &node = addNode(outputName);
    if (!node.isOutput) {
        node.isOutput = true;
        _outputs.emplace_back(node.id);
    }
}

void
Circuit::addInternal(const std::string &internalName, const std::vector<std::string> &arguments, Function function) {
    auto &node = addNode(internalName);
    assert(!arguments.empty() && arguments.size() <= 2);
    assert(!node.isInput);
    assert(node.function.isNone() && node.input1 == -1 && node.input2 == -1);
    node.function = function;
    node.input1 = addNode(arguments[0]).id;
    _parentNodes[node.input1].emplace_back(node.id);
    if (arguments.size() == 2) {
        node.input2 = addNode(arguments[1]).id;
        _parentNodes[node.input2].emplace_back(node.id);
    }
}

Circuit::Node &Circuit::addNode(const string &nodeName) {
    auto nodeIdIterator = _nodesByName.find(nodeName);
    if (nodeIdIterator != _nodesByName.end())
        return _nodes[nodeIdIterator->second];
    Node &node = _nodes.emplace_back(nodeName, _nodes.size());
    _parentNodes.emplace_back();
    _nodesByName.emplace(nodeName, node.id);
    return node;
}

Circuit::Node &Circuit::operator[](size_t i) {
    return _nodes[i];
}

const Circuit::Node &Circuit::operator[](size_t i) const {
    return _nodes[i];
}

Circuit::Node &Circuit::input(size_t i) {
    return _nodes[_inputs[i]];
}

const Circuit::Node &Circuit::input(size_t i) const {
    return _nodes[_inputs[i]];
}

Circuit::Node &Circuit::output(size_t i) {
    return _nodes[_outputs[i]];
}

const Circuit::Node &Circuit::output(size_t i) const {
    return _nodes[_outputs[i]];
}

Circuit::Node &Circuit::operator[](const string &nodeName) {
    return _nodes[_nodesByName.at(nodeName)];
}

const Circuit::Node &Circuit::operator[](const string &nodeName) const {
    return _nodes[_nodesByName.at(nodeName)];
}

size_t Circuit::inputsCount() const {
    return _inputs.size();
}

size_t Circuit::outputsCount() const {
    return _outputs.size();
}

size_t Circuit::nodesCount() const {
    return _nodes.size();
}

size_t Circuit::parentsCount(size_t i) const {
    return _parentNodes[i].size();
}

Circuit::Node &Circuit::parent(size_t i, size_t parentIndex) {
    return _nodes[_parentNodes[i][parentIndex]];
}

const Circuit::Node &Circuit::parent(size_t i, size_t parentIndex) const {
    return _nodes[_parentNodes[i][parentIndex]];
}

void Circuit::clearValues() {
    for (auto &node: _nodes) {
        node.value = Value::Unknown;
    }
}

Circuit::Function::Function(uint32_t answerMask) : _answerMask(answerMask) {}

bool Circuit::Function::operator()(uint32_t argumentsMask) const {
    return _answerMask & (1 << argumentsMask);
}

bool Circuit::Function::isNone() const {
    return _answerMask == (uint32_t) -1;
}

uint32_t Circuit::Function::composeMask(const vector<bool> &arguments) {
    assert(!arguments.empty() && arguments.size() <= 2);
    uint32_t mask = 0;
    for (size_t i = 0; i < arguments.size(); i++) {
        if (arguments[i])
            mask |= (1 << i);
    }
    return mask;
}

bool Circuit::Function::operator()(const vector<bool> &arguments) const {
    return (*this)(composeMask(arguments));
}

uint32_t Circuit::Function::composeMask(Circuit::Value a, Circuit::Value b) {
    assert(a != Circuit::Value::Unknown && b != Circuit::Value::Unknown);
    return composeMask((bool) a, (bool) b);
}

Circuit::Value Circuit::Function::operator()(Circuit::Value a, Circuit::Value b) const {
    return a == Circuit::Value::Unknown || b == Circuit::Value::Unknown ? Circuit::Value::Unknown
                                                                        : Circuit::Value((*this)(composeMask(a, b)));
}

uint32_t Circuit::Function::composeMask(Circuit::Value a) {
    assert(a != Circuit::Value::Unknown);
    return composeMask((bool) a);
}

uint32_t Circuit::Function::composeMask(bool a, bool b) {
    return a | (b << 1);
}

uint32_t Circuit::Function::composeMask(bool a) {
    return a;
}

Circuit::Value Circuit::Function::operator()(Circuit::Value a) const {
    return a == Circuit::Value::Unknown ? Circuit::Value::Unknown : Circuit::Value((*this)(composeMask(a)));
}

bool Circuit::Function::operator()(bool a, bool b) const {
    return (*this)(composeMask(a, b));
}

bool Circuit::Function::operator()(bool a) const {
    return (*this)(composeMask(a));
}

Circuit::Node::Node(std::string name, int id) : name(std::move(name)), id(id) {}
