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
    enum class Value {
        False,
        True,
        Unknown
    };

    class Function {
    public:
        Function() = default;

        explicit Function(uint32_t answerMask);

        bool operator()(uint32_t argumentsMask) const;

        bool operator()(const std::vector<bool> &arguments) const;

        Value operator()(Value a, Value b) const;

        Value operator()(Value a) const;

        bool operator()(bool a, bool b) const;

        bool operator()(bool a) const;

        [[nodiscard]] bool isNone() const;

    private:
        static uint32_t composeMask(const std::vector<bool> &arguments);

        static uint32_t composeMask(Value a, Value b);

        static uint32_t composeMask(Value a);

        static uint32_t composeMask(bool a, bool b);

        static uint32_t composeMask(bool a);

        uint32_t _answerMask = -1;
    };

    struct Functions {
        inline static const Function AND = Function(0b1000);
        inline static const Function OR = Function(0b1110);
        inline static const Function NOT = Function(0b01);
        inline static const Function NAND = Function(0b0111);
        inline static const Function NOR = Function(0b0001);
        inline static const Function XOR = Function(0b0110);
        inline static const Function NXOR = Function(0b1001);
        inline static const Function BUFF = Function(0b10);

        inline static const std::unordered_map<std::string, Function> allFunctionsByName = {
                {"AND",  AND},
                {"OR",   OR},
                {"NOT",  NOT},
                {"NAND", NAND},
                {"NOR",  NOR},
                {"XOR",  XOR},
                {"NXOR", NXOR},
                {"BUFF", BUFF}
        };
    };

    struct Node {
        Node(std::string name, size_t id);

        std::string name;
        size_t id;
        Function function = Function();
        Value value = Value::Unknown;
        size_t input1 = -1;
        size_t input2 = -1;
        bool isInput = false;
        bool isOutput = false;
    };

    Circuit(const Circuit &) = default;

    Circuit() = default;

    void addInput(const std::string &inputName);

    void addOutput(const std::string &outputName);

    void addInternal(const std::string &internalName, const std::vector<std::string> &arguments, Function function);

    Node &operator[](size_t i);

    const Node &operator[](size_t i) const;

    Node &operator[](const std::string &nodeName);

    const Node &operator[](const std::string &nodeName) const;

    Node &input(size_t i);

    const Node &input(size_t i) const;

    Node &output(size_t i);

    const Node &output(size_t i) const;

    Node &parent(size_t i, size_t parentIndex);

    const Node &parent(size_t i, size_t parentIndex) const;

    size_t inputsCount() const;

    size_t outputsCount() const;

    size_t nodesCount() const;

    size_t parentsCount(size_t i) const;

    void clearValues();

private:
    size_t addNode(const std::string &nodeName);

    std::vector<Node> _nodes;
    std::vector<size_t> _inputs;
    std::vector<size_t> _outputs;
    std::vector<std::vector<size_t>> _parentNodes;
    std::unordered_map<std::string, size_t> _nodesByName;
};