#include "CircuitNode.h"

#include <utility>

size_t CircuitNode::id() const {
    return _id;
}

std::string CircuitNode::name() const {
    return _name;
}

bool CircuitNode::operator<(const CircuitNode &other) const {
    return _id < other._id;
}

CircuitNode::CircuitNode(std::string name, size_t id, unsigned type) :
        _name(std::move(name)), _id(id), _isInput(type & INPUT_NODE), _isOutput(type & OUTPUT_NODE) {
}

bool CircuitNode::isInput() const {
    return _isInput;
}

bool CircuitNode::isOutput() const {
    return _isOutput;
}

bool CircuitNode::isInput(bool newVal) {
    return _isInput = newVal;
}

bool CircuitNode::isOutput(bool newVal) {
    return _isOutput = newVal;
}
