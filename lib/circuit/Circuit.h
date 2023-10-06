#pragma once

#include <set>
#include <map>
#include <string>
#include <vector>
#include <filesystem>
#include <unordered_map>
#include <functional>
#include <optional>

#include "CircuitNode.h"

class Circuit {
public:
    class Function {
    public:
        Function() = default;

        explicit Function(uint32_t answerMask);

        bool operator()(uint32_t argumentsMask) const;

        [[nodiscard]] bool isNone() const;

    private:
        uint32_t _answerMask = -1;
    };

    struct Functions {
        inline static const Function AND = Function()
    };

    Circuit(const Circuit &) = default;

    Circuit() = default;

    void addInput(const std::string &inputName);

    void addOutput(const std::string &outputName);

    void addInternal(const std::string &internalName, const std::vector<std::string> &arguments,
                     std::function<bool(bool, bool)> function);

private:

    std::set<CircuitNode> _literal;
    std::vector<CircuitNode *> _inputs;
    std::vector<CircuitNode *> _outputs;
};