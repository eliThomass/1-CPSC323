#include "SymbolTable.h"
#include <iostream>
#include <iomanip>

SymbolTable::SymbolTable() {
    next_available_address = 10000;
};

bool SymbolTable::symbolPush(std::string var_name, std::string var_type) {

    if (table.find(var_name) != table.end()) {
        return false; // symbol already exists
    }

    // Symbol creation
    Entry newEntry;
    newEntry.memory_address = next_available_address;
    newEntry.type = var_type;
    newEntry.var_name = var_name;

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

void SymbolTable::printTable() {
    std::cout << "\nSymbol Table" << std::endl;
    // Print the Header
    std::cout << std::left << std::setw(15) << "Identifier" 
              << std::left << std::setw(15) << "Address" 
              << "Type" << std::endl;
    
    std::cout << "--------------------------------------------" << std::endl;

    // Iterate through the map
    // 'pair.second' holds the 'Entry' struct with the data
    for (const auto& pair : table) {
        const Entry& entry = pair.second;

        std::cout << std::left << std::setw(15) << entry.var_name 
                  << std::left << std::setw(15) << entry.memory_address 
                  << entry.type << std::endl;
    }
    std::cout << std::endl;
}

