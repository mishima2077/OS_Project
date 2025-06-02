#include "c312.h"
#include "bios.h"
#include <iostream>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <sstream>

// Print memory to std::cerr
void printMemory(const std::vector<long>& memory) {
    std::cerr << "\n========== DEBUG: MEMORY STATE ==========\n";
    std::cerr << "Program Counter [0]      : " << memory[0] << std::endl;
    std::cerr << "Stack Pointer   [1]      : " << memory[1] << std::endl;
    std::cerr << "Syscall Result  [2]      : " << memory[2] << std::endl;
    std::cerr << "-----------------------------------------\n";
    for (size_t i = 3; i < memory.size(); ++i) {
        if (memory[i] != 0)
            std::cerr << "Mem[" << i << "] = " << memory[i] << std::endl;
    }
    std::cerr << "=========================================\n";
}

int main(int argc, char* argv[]) {
    if (argc < 4) {
        std::cerr << "Usage: ./simulate filename -D debug_flag" << std::endl;
        return 1;
    }
    std::string filename = argv[1];
    int debug = std::stoi(argv[3]);

    C312 cpu = Bios::loadAndBoot(filename, 11000);

    if (debug == 0) {
        while (!cpu.isHalted()) {
            cpu.execute();
        }
        printMemory(cpu.getMemory());
    } else if (debug == 1) {
        while (!cpu.isHalted()) {
            std::cerr << "-----------------------------------------\n";
            cpu.execute();
            printMemory(cpu.getMemory());
        }
    } else if (debug == 2) {
        while (!cpu.isHalted()) {
            std::cerr << "-----------------------------------------\n";
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