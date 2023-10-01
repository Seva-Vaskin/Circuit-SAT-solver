#pragma once

#include <string>

#define INPUT_NODE (1u << 0u)
#define OUTPUT_NODE (1u << 1u)

class CircuitNode {
public:
    CircuitNode(std::string name, size_t id, unsigned type);

    CircuitNode(const CircuitNode &other) = default;

    [[nodiscard]] std::string name() const;

    [[nodiscard]] size_t id() const;

    [[nodiscard]] bool isInput() const;

    bool isInput(bool newVal);

    [[nodiscard]] bool isOutput() const;

    bool isOutput(bool newVal);

    bool operator<(const CircuitNode &other) const;

private:
    std::string _name;
    size_t _id;
    bool _isInput;
    bool _isOutput;
};
