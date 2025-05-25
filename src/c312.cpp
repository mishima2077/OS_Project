#include "c312.h"
#include <sstream>
#include <stdexcept>
#include <iostream>
#include <cctype>

// Constructors
C312::C312(size_t memorySize) {
    this->memorySize = memorySize; // Initialize memory size
    memory.resize(memorySize, 0); // Initialize memory with zeros
    instructions.clear(); // Clear the instruction map
    mode = Mode::KERNEL; // Set initial mode to KERNEL
    halted = false; // Set initial halted state to false
}

C312::C312() : C312(1024) { // Default memory size
    // Left empty intentionally
}

// Public Methods

bool C312::isHalted() const {
    return halted;
}

// Fetch, decode, and execute the current instruction
void C312::execute() {
    long pc = memory[0];
    if (instructions.count(pc) == 0) {
        throw std::runtime_error("execute: No instruction at current PC.");
    }
    
    std::string instruction = instructions[pc];
    Opcode op = decodeInstruction(instruction);
    std::vector<std::string> tokens = splitInsctruction(instruction);
    
    // Execute based on opcode
    switch (op) {
        case Opcode::SET:
            Set(parseOperand(tokens[1]), parseOperand(tokens[2]));
            break;
        case Opcode::CPY:
            Cpy(parseOperand(tokens[1]), parseOperand(tokens[2]));
            break;
        case Opcode::CPYI:
            Cpyi(parseOperand(tokens[1]), parseOperand(tokens[2]));
            break;
        case Opcode::CPYI2:
            Cpyi2(parseOperand(tokens[1]), parseOperand(tokens[2]));
            break;
        case Opcode::ADD:
            Add(parseOperand(tokens[1]), parseOperand(tokens[2]));
            break;
        case Opcode::ADDI:
            Addi(parseOperand(tokens[1]), parseOperand(tokens[2]));
            break;
        case Opcode::SUBI:
            Subi(parseOperand(tokens[1]), parseOperand(tokens[2]));
            break;
        case Opcode::JIF:
            Jif(parseOperand(tokens[1]), parseOperand(tokens[2]));
            break;
        case Opcode::PUSH:
            Push(parseOperand(tokens[1]));
            break;
        case Opcode::POP:
            Pop(parseOperand(tokens[1]));
            break;
        case Opcode::CALL:
            Call(parseOperand(tokens[1]));
            break;
        case Opcode::RET:
            Ret();
            break;
        case Opcode::HLT:
            Hlt();
            break;
        case Opcode::USER:
            User();
            break;
        case Opcode::SYSCALL_PRN:
            SyscallPrn(parseOperand(tokens[2]));
            break;
        case Opcode::SYSCALL_HLT:
            SyscallHlt();
            break;
        case Opcode::SYSCALL_YIELD:
            SyscallYield();
            break;
        default:
            throw std::runtime_error("execute: Invalid or unimplemented opcode.");
    }

    // Increment instruction count.
    ++memory[3];

    // Increment PC if no jump occured.
    if (memory[0] == pc)
        ++memory[0];
}

void C312::loadProgram(const std::vector<long>& memoryData, 
                       const std::unordered_map<long, std::string>& instructionMap) {
    // Load memory
    if (memoryData.size() > memorySize)
        throw std::runtime_error("loadProgram: Given memory size is larger than cpu memory.");
    for (size_t i = 0; i < memoryData.size(); ++i)
        memory[i] = memoryData[i];

    // Load instructions
    instructions = instructionMap;

    // Reset CPU state
    halted = false;
    mode = Mode::KERNEL;
}

// Getters

const std::vector<long>& C312::getMemory() const {
    return memory;
}

long C312::getMemoryI(long i) const {
    if (i < 0 || i >= memorySize)
        throw std::out_of_range("getMemoryI: out of bounds.");
    return memory[i];
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
    memory[address] = value;
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

void C312::Cpyi2(long src, long dest) {
    // CPYI A1 A2: Copy the memory address indexed by A1 to memory address indexed by A2.
    if (src < 0 || src >= memorySize || dest < 0 || dest >= memorySize) 
        throw std::out_of_range("CPYI2: Address out of range");
    
    long indirectAdress = memory[src];
    if (indirectAdress < 0 || indirectAdress >= memorySize) 
        throw std::out_of_range("CPYI2: Indirect address out of range");

    long indirectDest = memory[dest];
    if (indirectDest < 0 || indirectDest >= memorySize)
        throw std::out_of_range("CPYI2: Indirect dest out of range");
    
    memory[indirectDest] = memory[indirectAdress];
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

void C312::Subi(long src, long dest) {
    // SUBI A1 A2: Subtract the contents of memory address A2 from address A1, put the result in A2.
    if (src < 0 || src >= memorySize || dest < 0 || dest >= memorySize) 
        throw std::out_of_range("SUBI: Address out of range");
    memory[dest] = memory[src] - memory[dest];
}

void C312::Jif(long address, long target) {
    // JIF A C: Set the CPU program counter with C if memory location A content is <= 0.
    if (address < 0 || address >= memorySize || target < 0 || target >= memorySize) 
        throw std::out_of_range("JIF: Address out of range");

    if (memory[address] <= 0)
        memory[0] = target;
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
    // CALL C: Call subroutine at instruction C, push return address.
    if (address < 0 || address >= memorySize)
        throw std::out_of_range("CALL: Address out of range.");
    if (memory[1] <= 0)
        throw std::out_of_range("CALL: Stack overflow.");

    Push(memory[0] + 1); // Push return address (next instruction).
    memory[0] = address; // Call subroutine at address.
}

void C312::Ret() {
    // RET: Return from subroutine by popping return address.
    Pop(0); // Pop the return address to PC.
}

void C312::Hlt() {
    // HLT: Halts the CPU.
    halted = true;
}

void C312::User() {
    // USER: Switch to user mode
    mode = Mode::USER;
}

void C312::SyscallPrn(long address) {
    mode = Mode::KERNEL;
    if (address < 0 || address > memorySize)
        throw std::out_of_range("SYSCALL PRN: Address out of bounds.");
    std::cout << memory[address] << std::endl;

    memory[2] = memory[address]; // Store the syscall result
}

void C312::SyscallHlt() {
    // It will be implemented in C312 assembly.
    mode = Mode::KERNEL;
}

void C312::SyscallYield() {
    // It will be implemented in C312 assembly.
    mode = Mode::KERNEL;
}

// Helper Methods for Parsing and Decoding Instructions

// Splits an instruction string into tokens
std::vector<std::string> C312::splitInsctruction(const std::string& instruction) const {
    std::vector<std::string> tokens;
    std::stringstream iss(instruction);
    std::string token;

    while (iss >> token) {
        if (token[0] == '#') break;
        tokens.push_back(token);
    }

    return tokens;
}

// Decodes the opcode from the instruction string
Opcode C312::decodeInstruction(const std::string& instruction) const {
    std::vector<std::string> tokens = splitInsctruction(instruction);
    if (tokens.empty())
        return Opcode::INVALID;

    std::string op = tokens[0];
    for (size_t i = 0; i < op.length(); ++i)
        op[i] = toupper(op[i]);

    Opcode code = Opcode::INVALID;
    if (op == "SET") code = Opcode::SET;
    else if (op == "CPY") code = Opcode::CPY;
    else if (op == "CPYI") code = Opcode::CPYI;
    else if (op == "CPYI2") code = Opcode::CPYI2;
    else if (op == "ADD") code = Opcode::ADD;
    else if (op == "ADDI") code = Opcode::ADDI;
    else if (op == "SUBI") code = Opcode::SUBI;
    else if (op == "JIF") code = Opcode::JIF;
    else if (op == "PUSH") code = Opcode::PUSH;
    else if (op == "POP") code = Opcode::POP;
    else if (op == "CALL") code = Opcode::CALL;
    else if (op == "RET") code = Opcode::RET;
    else if (op == "HLT") code = Opcode::HLT;
    else if (op == "USER") code = Opcode::USER;
    else if (op == "SYSCALL" && tokens.size() > 1) {
        std::string sys = tokens[1];
        for (size_t i = 0; i < sys.length(); ++i)
            sys[i] = toupper(sys[i]);
        if (sys == "PRN") code = Opcode::SYSCALL_PRN;
        else if (sys == "HLT") code = Opcode::SYSCALL_HLT;
        else if (sys == "YIELD") code = Opcode::SYSCALL_YIELD;
    }

    // Validate token count for the decoded opcode
    if (!isValidTokenCount(tokens, code))
        return Opcode::INVALID;

    return code;
}

// Helper to check if token count is valid for the given opcode
bool C312::isValidTokenCount(const std::vector<std::string>& tokens, Opcode op) const {
    if (tokens.empty()) return false;

    switch (op) {
        case Opcode::SET:
        case Opcode::CPY:
        case Opcode::CPYI:
        case Opcode::CPYI2:
        case Opcode::ADD:
        case Opcode::ADDI:
        case Opcode::SUBI:
        case Opcode::JIF:
            return tokens.size() == 3;
        case Opcode::PUSH:
        case Opcode::POP:
        case Opcode::CALL:
            return tokens.size() == 2;
        case Opcode::RET:
        case Opcode::HLT:
        case Opcode::USER:
            return tokens.size() == 1;
        case Opcode::SYSCALL_PRN:
            return tokens.size() == 3;
        case Opcode::SYSCALL_HLT:
        case Opcode::SYSCALL_YIELD:
            return tokens.size() == 2;
        default:
            return false;
    }
}

// Parse an operand string into a long value
long C312::parseOperand(const std::string& operand) const {
    return std::stol(operand);
}