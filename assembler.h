#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include "SymbolTable.h"

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
    public:
        AssemblyCodeGenerator();
        void gen_instr(std::string op, int operand);
        void gen_instr(std::string op); // For opcodes like ADD with no operand
        int get_current_address();
        void print_assembly_code();
};

#endif