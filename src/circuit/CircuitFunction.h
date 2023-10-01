#pragma once

#include <string>
#include <functional>
#include <map>
#include <set>

class CircuitFunction {
public:
    CircuitFunction(std::string name, size_t argumentsCount, std::function<bool(bool, bool)> function);

    bool operator()(bool arg1) const;

    bool operator()(bool arg1, bool arg2) const;

    [[nodiscard]] std::string name() const;

    [[nodiscard]] static const std::vector<CircuitFunction> &allFunctions();

    static std::set<std::string> allFunctionNames();

    static std::map<std::string, CircuitFunction> allFunctionsByName();

private:
    std::string _name;
    size_t _argumentsCount;
    std::function<bool(bool, bool)> _function;
};


