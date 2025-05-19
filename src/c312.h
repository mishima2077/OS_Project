#pragma once
#include <vector>
#include <string>
#include <unordered_map>

enum class Mode { KERNEL, USER };

enum class Opcode {
    SET, CPY, CPYI, ADD, ADDI, SUBI, JIF, PUSH, POP, CALL, RET, HLT, USER,
    SYSCALL_PRN, SYSCALL_HLT, SYSCALL_YIELD, INVALID
};

class C312 {
public:
    // Constructors
    C312(size_t memorySize);
    C312();

    // Check if the CPU is halted
    bool isHalted() const;

    // Execute the current instruction in PC
    void execute();

    // Load program 
    void loadProgram(const std::vector<long>& memoryData, 
                     const std::unordered_map<long, std::string>& instructionMap);

    // Getters
    const std::vector<long>& getMemory() const;
    const std::unordered_map<long, std::string>& getInstructions() const;
    long getPC() const;
    Mode getMode() const;
    long getInstructionCount()const;

private:
    // Memory
    std::vector<long> memory;
    size_t memorySize;

    // Instruction map: maps instructions indices to their corresponding strings
    std::unordered_map<long, std::string> instructions;

    // Cpu state
    Mode mode;
    bool halted;

    // Methods for each instruction
    void Set(long value, long address);
    void Cpy(long src, long dest);
    void Cpyi(long src, long dest);
    void Add(long address, long value);
    void Addi(long dest, long address);
    void Subi(long dest, long address);
    void Jif(long address, long target);
    void Push(long address);
    void Pop(long address);
    void Call(long address);
    void Ret();
    void Hlt();
    void User();
    void SyscallPrn(long address);
    void SyscallHlt();
    void SyscallYield();

    // Helper methods for parsing and decoding instructions
    std::vector<std::string> splitInsctruction(const std::string& instruction) const;
    Opcode decodeInstruction(const std::string& instruction) const;
    long parseOperand(const std::string& operand) const;

    // Mode switching
    void switchToKernelMode();
    void switchToUserMode();
};