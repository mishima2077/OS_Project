#include "bios.h"
#include <fstream>
#include <sstream>
#include <iostream>

void Bios::parseFile(const std::string& filename, std::vector<long>& memoryData, std::unordered_map<long, std::string>& instructions) {
    std::ifstream infile(filename);
    if (!infile) {
        std::cerr << "Cannot open file: " << filename << std::endl;
        exit(1);
    }
    std::string line;
    bool inData = false, inInstr = false;
    while (std::getline(infile, line)) {
        size_t comment = line.find('#');
        if (comment != std::string::npos) line = line.substr(0, comment);
        line.erase(0, line.find_first_not_of(" \t\r\n"));
        line.erase(line.find_last_not_of(" \t\r\n") + 1);

        if (line.empty()) continue;
        if (line == "Begin Data Section") { inData = true; continue; }
        if (line == "End Data Section")   { inData = false; continue; }
        if (line == "Begin Instruction Section") { inInstr = true; continue; }
        if (line == "End Instruction Section")   { inInstr = false; continue; }

        if (inData) {
            std::istringstream iss(line);
            long addr, value;
            if (iss >> addr >> value) {
                if (addr >= 0 && addr < (long)memoryData.size())
                    memoryData[addr] = value;
            }
        } else if (inInstr) {
            std::istringstream iss(line);
            long addr;
            if (iss >> addr) {
                std::string instr;
                std::getline(iss, instr);
                instr.erase(0, instr.find_first_not_of(" \t"));
                instructions[addr] = instr;
            }
        }
    }
}

C312 Bios::loadAndBoot(const std::string& filename, size_t memSize) {
    std::vector<long> memoryData(memSize, 0);
    std::unordered_map<long, std::string> instructions;
    parseFile(filename, memoryData, instructions);

    C312 cpu(memoryData.size());
    cpu.loadProgram(memoryData, instructions);
    return cpu;
}