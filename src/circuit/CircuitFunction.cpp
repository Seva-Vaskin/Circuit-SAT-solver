#include "CircuitFunction.h"

#include <stdexcept>
#include <utility>

using namespace std;

bool CircuitFunction::operator()(bool arg1) const {
    if (_argumentsCount != 1)
        throw runtime_error(
                "Try to run function with 1 argument while it accepts " +
                to_string(_argumentsCount) + " arguments");
    return _function(arg1, false);
}

bool CircuitFunction::operator()(bool arg1, bool arg2) const {
    if (_argumentsCount != 2)
        throw runtime_error(
                "Try to run function with 2 arguments while it accepts " +
                to_string(_argumentsCount) + " arguments");
    return _function(arg1, arg2);
}

CircuitFunction::CircuitFunction(string name, size_t argumentsCount, function<bool(bool, bool)> function)
        : _name(std::move(name)), _argumentsCount(argumentsCount), _function(std::move(function)) {
}

string CircuitFunction::name() const {
    return _name;
}


const vector<CircuitFunction> &CircuitFunction::allFunctions() {
    const static vector<CircuitFunction> _allFunctions = {
            CircuitFunction("AND", 2, [](bool a, bool b) { return a & b; }),
            CircuitFunction("OR", 2, [](bool a, bool b) { return a | b; }),
            CircuitFunction("NOT", 1, [](bool a, bool) { return !a; }),
            CircuitFunction("NAND", 2, [](bool a, bool b) { return !(a & b); }),
            CircuitFunction("NOR", 2, [](bool a, bool b) { return !(a | b); }),
            CircuitFunction("XOR", 2, [](bool a, bool b) { return a ^ b; }),
            CircuitFunction("NXOR", 2, [](bool a, bool b) { return !(a ^ b); })
    };
    return _allFunctions;
}

set<string> CircuitFunction::allFunctionNames() {
    set<string> functionNames;
    for (const auto &f: allFunctions()) {
        functionNames.emplace(f.name());
    }
    return functionNames;
}

map<string, CircuitFunction> CircuitFunction::allFunctionsByName() {
    map<string, CircuitFunction> allFunctionsByName;
    for (const auto &f: allFunctions()) {
        allFunctionsByName.emplace(f.name(), f);
    }
    return allFunctionsByName;
}

