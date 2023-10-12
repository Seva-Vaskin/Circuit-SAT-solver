#include "Circuit.h"

#include <fstream>
#include <algorithm>
#include <cassert>

using namespace std;

void Circuit::addInput(const std::string &inputName) {
    auto &node = _nodes[addNode(inputName)];
    if (!node.isInput) {
        node.isInput = true;
        _inputs.emplace_back(node.id);
    }
}

void Circuit::addOutput(const std::string &outputName) {
    auto &node = _nodes[addNode(outputName)];
    if (!node.isOutput) {
        node.isOutput = true;
        _outputs.emplace_back(node.id);
    }
}

void
Circuit::addInternal(const std::string &internalName, const std::vector<std::string> &arguments, Function function) {
    size_t currNode = addNode(internalName);
    assert(!arguments.empty() && arguments.size() <= 2);
    assert(!_nodes[currNode].isInput);
    assert(_nodes[currNode].function.isNone()
           && _nodes[currNode].input1 == (size_t) -1 &&
           _nodes[currNode].input2 == (size_t) -1);
    _nodes[currNode].function = function;
    _nodes[currNode].input1 = addNode(arguments[0]);
    _parentNodes[_nodes[currNode].input1].emplace_back(_nodes[currNode].id);
    if (arguments.size() == 2) {
        _nodes[currNode].input2 = addNode(arguments[1]);
        _parentNodes[_nodes[currNode].input2].emplace_back(currNode);
    }
}

size_t Circuit::addNode(const string &nodeName) {
    auto nodeIdIterator = _nodesByName.find(nodeName);
    if (nodeIdIterator != _nodesByName.end())
        return nodeIdIterator->second;
    _nodes.emplace_back(nodeName, _nodes.size());
    Node &node = _nodes.back();
    _parentNodes.emplace_back();
    _nodesByName.emplace(nodeName, node.id);
    return node.id;
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

Circuit::Function::Function(uint32_t answerMask, uint64_t argumentsCount) : _answerMask(answerMask), _argumentsCount(argumentsCount) {}

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
    assert(_argumentsCount == 2);
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
    assert(_argumentsCount == 1);
    return a == Circuit::Value::Unknown ? Circuit::Value::Unknown : Circuit::Value((*this)(composeMask(a)));
}

bool Circuit::Function::operator()(bool a, bool b) const {
    assert(_argumentsCount == 2);
    return (*this)(composeMask(a, b));
}

bool Circuit::Function::operator()(bool a) const {
    assert(_argumentsCount == 1);
    return (*this)(composeMask(a));
}

size_t Circuit::Function::argumentsCount() const {
    return _argumentsCount;
}

Circuit::Node::Node(std::string name, size_t id) : name(std::move(name)), id(id) {}
