#pragma once

#include <filesystem>
#include <fstream>
#include "circuit/Circuit.h"

class BenchParser {
public:
    static Circuit parseFile(const std::filesystem::path &filePath);
};
