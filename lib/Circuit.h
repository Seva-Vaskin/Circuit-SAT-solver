#pragma once

#include <set>
#include <map>
#include <string>
#include <vector>
#include <filesystem>
#include <unordered_map>
#include <functional>
#include <optional>

class Circuit {
public:
    class Function {
    public:
        Function() = default;

        explicit Function(uint32_t answerMask);

        bool operator()(uint32_t argumentsMask) const;

        bool operator()(const std::vector<bool>& arguments) const;

        [[nodiscard]] bool isNone() const;

        static uint32_t composeMask(const std::vector<bool>& arguments);

    private:
        uint32_t _answerMask = -1;
    };

    struct Functions {
        inline static const Function AND = Function(0b1000);
        inline static const Function OR = Function(0b1110);
        inline static const Function NOT = Function(0b10);
        inline static const Function NAND = Function(0b0111);
        inline static const Function NOR = Function(0b0001);
        inline static const Function XOR = Function(0b0110);
        inline static const Function NXOR = Function(0b1001);

        inline static const std::unordered_map<std::string, Function> allFunctionsByName = {
                {"AND",  AND},
                {"OR",   OR},
                {"NOT",  NOT},
                {"NAND", NAND},
                {"NOR",  NOR},
                {"XOR",  XOR},
                {"NXOR", NXOR}
        };
    };

    struct Node {
        enum class Value {
            False,
            True,
            Unknown
        };

        Node(std::string name, int id);

        std::string name;
        int id;
        Function function = Function();
        int input1 = -1;
        int input2 = -1;
        Value value = Value::Unknown;
        bool isInput = false;
        bool isOutput = false;
    };

    Circuit(const Circuit &) = default;

    Circuit() = default;

    void addInput(const std::string &inputName);

    void addOutput(const std::string &outputName);

    void addInternal(const std::string &internalName, const std::vector<std::string> &arguments, Function function);

private:
    Node &addNode(const std::string &nodeName);

    std::vector<Node> _nodes;
    std::vector<size_t> _inputs;
    std::vector<size_t> _outputs;
    std::unordered_map<std::string, size_t> _nodesByName;
};