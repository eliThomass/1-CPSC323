#include "assembler.h"
#include <iomanip>  // for std::setw
#include <fstream>

AssemblyCodeGenerator::AssemblyCodeGenerator() {
    currentInstruction = 1;           // instructions start at 1
    instructions.reserve(1000);       // assignment says: at least 1000 instructions
}

// gen_instr(op, oprnd)
void AssemblyCodeGenerator::gen_instr(std::string op, int operand) {
    Instruction inst;
    inst.address = currentInstruction; // like instr_address in the notes
    inst.op = op;
    inst.operand = operand;
    inst.hasOperand = true;

    instructions.push_back(inst);
    currentInstruction++;             // move to next instruction slot
}

// gen_instr(op, nil) – no operand (ADD, MUL, LABEL, etc.)
void AssemblyCodeGenerator::gen_instr(std::string op) {
    Instruction inst;
    inst.address = currentInstruction;
    inst.op = op;
    inst.operand = 0;              // ignored
    inst.hasOperand = false;          // corresponds to “nil” in the handout

    instructions.push_back(inst);
    currentInstruction++;
}

// current instruction address (used for LABEL/JUMP targets)
int AssemblyCodeGenerator::get_current_address() {
    return currentInstruction;
}

// Print the instruction table at the end (like his example listing)
void AssemblyCodeGenerator::print_assembly_code(std::string filename) {
    std::ofstream outfile(filename);
    // Format similar to:
    // 1     PUSHI      0
    for (const auto& inst : instructions) {
        outfile << std::left << std::setw(5) << inst.address 
                << std::left << std::setw(10) << inst.op;

        if (inst.hasOperand) {
            outfile << inst.operand;
        }

        outfile << std::endl;
    }

    outfile.close();
}


void AssemblyCodeGenerator::backpatch(int instrAddress, int targetAddress) {
    // instrAddress is 1-based; our vector is 0-based
    int index = instrAddress - 1;

    if (index >= 0 && index < instructions.size()) {
        instructions[index].operand = targetAddress;
        instructions[index].hasOperand = true;
    }
    else {
        std::cerr << "Backpatch error: invalid instruction address " << instrAddress << "\n";
    }
}

// Getter for our jump stack
int AssemblyCodeGenerator::get_jump() {
    return jumpStack.back();
}

// Pops our jump stack
void AssemblyCodeGenerator::pop_jump() {
    jumpStack.pop_back();
}

// Pushes to our jump stack
void AssemblyCodeGenerator::push_jump(int addr) {
    jumpStack.push_back(addr);
}