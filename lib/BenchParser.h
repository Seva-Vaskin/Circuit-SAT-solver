#pragma once

#include <filesystem>
#include <fstream>
#include "Circuit.h"

class BenchParser {
public:
    static Circuit parseFile(const std::filesystem::path &filePath);
};
