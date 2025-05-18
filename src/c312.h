#pragma once
#include <vector>
#include <string>

enum class Mode { KERNEL, USER };

enum class Opcode {
    SET, CPY, CPYI, ADD, ADDI, SUBI, JIF, PUSH, POP, CALL, RET, HLT, USER,
    SYSCALL_PRN, SYSCALL_HLT, SYSCALL_YIELD, INVALID
};

class C312 {
public:

private:

};