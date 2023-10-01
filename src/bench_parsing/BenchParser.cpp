#include "BenchParser.h"

#include <string>
#include <algorithm>

#include "circuit/CircuitFunction.h"

using namespace std;

namespace {

    const string inputFunctionName = "INPUT";
    const string outputFunctionName = "OUTPUT";

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

    bool checkVariableName(const string &name) {
        return ranges::all_of(name, [](char c) {
            return ('A' <= c && c <= 'Z')
                   || ('a' <= c && c <= 'z')
                   || c == '_'
                   || ('0' <= c && c <= '9');
        });
    }

    bool checkFunctionName(const string &name, const set<string> &validNames) {
        return validNames.contains(name);
    }

    struct BenchFunction {
        string name;
        vector<string> args;
    };

    struct BenchEquation {
        string variableName;
        BenchFunction function;
    };

    unique_ptr<BenchFunction> tryParseBenchFunction(const string &s, const set<string> &validFunctionNames) {
        size_t openBracketPos = s.find('(');
        size_t closeBracketPos = s.find(')');
        if (openBracketPos == string::npos || closeBracketPos != s.size() - 1)
            return nullptr;
        string functionName = s.substr(0, openBracketPos);
        if (!checkFunctionName(functionName, validFunctionNames))
            return nullptr;
        vector<string> functionArgs = {""};
        for (size_t i = openBracketPos + 1; i < s.size(); i++) {
            string &lastArgName = functionArgs.back();
            if (s[i] == ',') {
                if (lastArgName.empty())
                    return nullptr;
                functionArgs.emplace_back();
            } else if (i != closeBracketPos) {
                lastArgName += s[i];
            }
        }
        return make_unique<BenchFunction>(functionName, functionArgs);
    }

    unique_ptr<BenchEquation> tryParseBenchEquation(const string &s) {
        size_t equalSignPos = s.find('=');
        if (equalSignPos == string::npos)
            return nullptr;
        string variableName = s.substr(0, equalSignPos);
        if (!checkVariableName(variableName))
            return nullptr;
        static const auto allFunctionNames = CircuitFunction::allFunctionNames();
        auto function = tryParseBenchFunction(s.substr(equalSignPos + 1), allFunctionNames);
        if (function == nullptr)
            return nullptr;
        return make_unique<BenchEquation>(variableName, *function);
    }

    bool tryParseFunctionAndAddToCircuit(const string &line, Circuit &circuit) {
        auto benchFunction = tryParseBenchFunction(line, {inputFunctionName, outputFunctionName});
        if (benchFunction == nullptr)
            return false;

        if (benchFunction->args.size() != 1) {
            throw runtime_error("Cannot parse line (wrong number of arguments for INPUT/OUTPUT): " + line);
        }
        string &nodeName = benchFunction->args[0];
        if (!checkVariableName(nodeName)) {
            throw runtime_error("Cannot parse line (invalid name): " + line);
        }
        if (benchFunction->name == inputFunctionName)
            circuit.addInput(nodeName);
        else if (benchFunction->name == outputFunctionName)
            circuit.addOutput(nodeName);
        else
            throw logic_error("This code line shouldn't be reached");
        return true;
    }

    bool tryParseEquationAndAddToCircuit(const string &line, Circuit &circuit) {
        auto benchEquation = tryParseBenchEquation(line);
        if (benchEquation == nullptr)
            return false;
        static const auto functionsByName = CircuitFunction::allFunctionsByName();
        auto function = functionsByName.find(benchEquation->function.name);
        if (function == functionsByName.end())
            throw runtime_error("Cannot parse line (invalid circuit function): " + line);
        circuit.addInternal(benchEquation->variableName, benchEquation->function.args, function->second);
        return true;
    }
}

Circuit BenchParser::parseFile(const filesystem::path &filePath) {
    ifstream in(filePath);

    Circuit circuit;
    string line;
    while (getline(in, line)) {
        deleteSpaces(line);
        if (line.empty())
            continue;
        bool parsed = false;
        parsed = parsed | tryParseFunctionAndAddToCircuit(line, circuit);
        parsed = parsed | tryParseEquationAndAddToCircuit(line, circuit);
        if (!parsed) {
            throw runtime_error("Cannot parse line: " + line);
        }
    }
    return circuit;
}
