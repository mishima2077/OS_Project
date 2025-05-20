#include "c312.h"
#include <iostream>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <sstream>

// Print memory to std::cerr
void printMemory(const std::vector<long>& memory) {
    for (size_t i = 0; i < 5; ++i) {
        std::cerr << "[" << i << "] = " << memory[i] << std::endl;
    }

    for (size_t i = 5; i < memory.size(); ++i) {
        if (memory[i] != 0)
            std::cerr << "[" << i << "] = " << memory[i] << std::endl;
    }
}

// Load program from file (each line: address instruction)
void loadProgramFromFile(const std::string& filename, std::vector<long>& memoryData, std::unordered_map<long, std::string>& instructions) {
    std::ifstream infile(filename);
    if (!infile) {
        std::cerr << "Cannot open file: " << filename << std::endl;
        exit(1);
    }
    std::string line;
    bool inData = false, inInstr = false;
    while (std::getline(infile, line)) {
        // Remove comments
        size_t comment = line.find('#');
        if (comment != std::string::npos) line = line.substr(0, comment);
        // Trim whitespace
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
    // Optionally, set stack pointer if your OS expects it
    if (memoryData.size() > 1 && memoryData[1] == 0)
        memoryData[1] = memoryData.size() - 1;
}

int main(int argc, char* argv[]) {
    if (argc < 4) {
        std::cerr << "Usage: ./simulate filename -D debug_flag" << std::endl;
        return 1;
    }
    std::string filename = argv[1];
    int debug = std::stoi(argv[3]);

    std::vector<long> memoryData(200, 0); // Adjust size as needed
    std::unordered_map<long, std::string> instructions;
    loadProgramFromFile(filename, memoryData, instructions);

    C312 cpu(memoryData.size());
    cpu.loadProgram(memoryData, instructions);

    if (debug == 0) {
        while (!cpu.isHalted()) {
            cpu.execute();
        }
        printMemory(cpu.getMemory());
    } else if (debug == 1) {
        while (!cpu.isHalted()) {
            cpu.execute();
            printMemory(cpu.getMemory());
        }
    } else if (debug == 2) {
        while (!cpu.isHalted()) {
            cpu.execute();
            printMemory(cpu.getMemory());
            std::cerr << "Press Enter to continue..." << std::endl;
            std::cin.get();
        }
    } else {
        std::cerr << "Invalid debug flag. Use 0, 1, or 2." << std::endl;
        return 1;
    }
    return 0;
}