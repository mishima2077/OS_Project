#include "C312.h"
#include <sstream>
#include <stdexcept>
#include <iostream>

// Constructors
C312::C312(size_t memorySize) {
    this->memorySize = memorySize; // Initialize memory size
    memory.resize(memorySize, 0); // Initialize memory with zeros
    instructions.clear(); // Clear the instruction map
    mode = Mode::KERNEL; // Set initial mode to KERNEL
    halted = false; // Set initial halted state to false
}

C312::C312() : C312(12000) { // Default memory size
    // Left empty intentionally
}

// Public Methods

bool C312::isHalted() const {
    return halted;
}

void C312::execute() {
    // Fetch, decode, and execute the current instruction
}

void C312::loadProgram(const std::vector<long>& memoryData, 
                       const std::unordered_map<long, std::string>& instructionMap) {
}

// Getters

const std::vector<long>& C312::getMemory() const {
    return memory;
}

const std::unordered_map<long, std::string>& C312::getInstructions() const {
    return instructions;
}

long C312::getPC() const {
    return memory[0];
}

Mode C312::getMode() const {
    return mode;
}

long C312::getInstructionCount() const {
    return memory[3];
}

// Private Methods for Each Instruction

void C312::Set(long value, long address) {
    // SET B A: Set the Ath memory location with number B.
    if (address < 0 || address >= memorySize) 
        throw std::out_of_range("SET: Address out of range");
    memory[address] = value
}

void C312::Cpy(long src, long dest) {
    // CPY A1 A2: Copy the content of memory location A1 to memory A2.
    if (src < 0 || src >= memorySize || dest < 0 || dest >= memorySize) 
        throw std::out_of_range("CPY: Address out of range");
    memory[dest] = memory[src];
}

void C312::Cpyi(long src, long dest) {
    // CPYI A1 A2: Copy the memory address indexed by A1 to memory address A2.
    if (src < 0 || src >= memorySize || dest < 0 || dest >= memorySize) 
        throw std::out_of_range("CPYI: Address out of range");
    
    long indirectAdress = memory[src];
    if (indirectAdress < 0 || indirectAdress >= memorySize) 
        throw std::out_of_range("CPYI: Indirect address out of range");
    
    memory[dest] = memory[indirectAdress];
}

void C312::Add(long address, long value) {
    // ADD A B: Add number B to memory location A.
    if (address < 0 || address >= memorySize) 
        throw std::out_of_range("ADD: Address out of range");
    memory[address] += value;
}

void C312::Addi(long dest, long address) {
    // ADDI A1 A2: Add the contents of memory address A2 to address A1.
    if (address < 0 || address >= memorySize || dest < 0 || dest >= memorySize) 
        throw std::out_of_range("ADDI: Address out of range");
    memory[dest] += memory[address];
}

void C312::Subi(long dest, long address) {
    // SUBI A1 A2: Subtract the contents of memory address A2 from address A1, put the result in A2.
    if (address < 0 || address >= memorySize || dest < 0 || dest >= memorySize) 
        throw std::out_of_range("SUBI: Address out of range");
    memory[dest] -= memory[address];
}

void C312::Jif(long address, long target) {
    // JIF A C: Set the CPU program counter with C if memory location A content is <= 0.
    if (address < 0 || address >= memorySize || target < 0 || target >= memorySize) 
        throw std::out_of_range("JIF: Address out of range");

    if (memory[address] <= 0) {
        memory[0] = target - 1; // -1 because PC will be incremented after execution
}

void C312::Push(long address) {
    // PUSH A: Push memory A onto the stack. Stack grows downwards.
    if (address < 0 || address >= memorySize)
        throw std::out_of_range("PUSH: Address out of range");

    if (memory[1] <= 0)
        throw std::out_of_range("PUSH: Stack overflow");

    if (memory[1] >= memorySize)
        throw std::out_of_range("PUSH: Invalid stack pointer");


    memory[--memory[1]] = address; // Decrement stack pointer and push address
}

void C312::Pop(long address) {
    // POP A: Pop value from stack into memory A.
    if (address < 0 || address >= memorySize)
        throw std::out_of_range("POP: Address out of range");

    if (memory[1] >= memorySize)
        throw std::out_of_range("POP: Invalid stack pointer");
    memory[address] = memory[memory[1]++]; // Pop value from stack
}

void C312::Call(long address) {
    if (address < 0 || address >= memorySize)
        throw std::out_of_range("CALL: Address out of range.");
    if (memory[1] <= 0)
        throw std::out_of_range("CALL: Stack overflow.");

}

void C312::Ret() {
    // Implement the RET instruction
}

void C312::Hlt() {
    // Implement the HLT instruction
}

void C312::User() {
    // Implement the USER instruction
}

void C312::SyscallPrn(long address) {
    // Implement the SYSCALL PRN instruction
}

void C312::SyscallHlt() {
    // Implement the SYSCALL HLT instruction
}

void C312::SyscallYield() {
    // Implement the SYSCALL YIELD instruction
}

// Helper Methods for Parsing and Decoding Instructions

Opcode C312::decodeInstruction(const std::string& instruction) const {
    // Decode the opcode from the instruction string
}

std::vector<std::string> C312::splitInsctruction(const std::string& instruction) const {
    // Split the instruction string into components
}

long C312::parseOperand(const std::string& operand) const {
    // Parse an operand string into a long value
}