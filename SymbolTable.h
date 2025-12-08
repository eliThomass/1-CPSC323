#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H

#include "lexer.h"
#include <map>

struct Entry {
    int memory_address;
    std::string var_name;
    std::string type;
};

class SymbolTable {
private:
    int next_available_address;
    std::map<std::string, Entry> table;
public:
    SymbolTable();
    bool symbolPush(std::string var_name, std::string type);
    int getAddress(std::string& lexeme);
    void printTable();
};

#endif