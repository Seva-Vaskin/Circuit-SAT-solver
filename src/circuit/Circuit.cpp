#include "Circuit.h"

#include <fstream>
#include <algorithm>

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
    // TODO
}

void Circuit::addOutput(const std::string &outputName) {
    // TODO
}

void Circuit::addInternal(const std::string &internalName, const std::vector<std::string> &arguments,
                          std::function<bool(bool, bool)> function) {
    // TODO
}
