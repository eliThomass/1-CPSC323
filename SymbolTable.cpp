#include "SymbolTable.h"
#include <iostream>


SymbolTable::SymbolTable() {
    next_available_address = 10000;
};

bool SymbolTable::symbolPush(Token symbol) {

    if (table.find(symbol.value) != table.end()) {
        return false; // symbol already exists
    }

    // Symbol creation
    Entry newEntry;
    newEntry.memory_address = next_available_address;
    newEntry.type = symbol.type;
    newEntry.var_name = symbol.value;

    table[newEntry.var_name] = newEntry;
    next_available_address++;

    return true;
}

int SymbolTable::getAddress(std::string& lexeme){
    if (table.find(lexeme) != table.end()) { // True if lexeme is found
        return table[lexeme].memory_address;
    };
    return -1; // Error code
}

