#pragma once
#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include "SymbolTable.h"
#include <vector>
#include <string>
#include <iostream>   // for printing

// Example: 1  PUSHI  0
struct Instruction {
    int address;
    std::string op;
    int operand;
    bool hasOperand; // Needed for opcodes like ADD that have none.
};

class AssemblyCodeGenerator {
private:
    std::vector<Instruction> instructions;
    int currentInstruction; // starts at 1
    std::vector<int> jumpStack;
public:
    AssemblyCodeGenerator();
    void gen_instr(std::string op, int operand); // with operand
    void gen_instr(std::string op);              // for opcodes like ADD with no operand
    int get_current_address();
    void print_assembly_code(std::string filename);
    void backpatch(int instrAddress, int targetAddress);
    int get_jump();
    void pop_jump();
    void push_jump(int jumpAddr);
};

#endif
