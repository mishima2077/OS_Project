#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include "c312.h"

// Bios class is used to load the given program file into the cpu
class Bios {
public:
    // Loads the program file, configures memory/instructions, and returns a ready-to-use CPU
    static C312 loadAndBoot(const std::string& filename, size_t memSize = 11000);

private:
    static void parseFile(const std::string& filename, std::vector<long>& memoryData, std::unordered_map<long, std::string>& instructions);
};