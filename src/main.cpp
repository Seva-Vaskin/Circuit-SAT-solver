#include <filesystem>

#include "BenchParser.h"

using namespace std;

int main(int argc, char *argv[]) {
    filesystem::path filePath = "/home/vsevolod/spbu/Circuit-SAT-solver/data/benchs_for_test/5_6.bench";

    auto circuit = BenchParser::parseFile(filePath);
}