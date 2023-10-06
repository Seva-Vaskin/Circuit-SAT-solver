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
    if (arguments.size() == 2) {
        node.input2 = addNode(arguments[1]).id;
    }
}

Circuit::Node &Circuit::addNode(const string &nodeName) {
    auto nodeIdIterator = _nodesByName.find(nodeName);
    if (nodeIdIterator != _nodesByName.end())
        return _nodes[nodeIdIterator->second];
    Node &node = _nodes.emplace_back(nodeName, _nodes.size());
    _nodesByName.emplace(nodeName, node.id);
    return node;
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

Circuit::Node::Node(std::string name, int id) : name(std::move(name)), id(id) {}
