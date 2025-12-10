#include "SymbolTable.h"
#include <iostream>
#include <iomanip>
#include <fstream>

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

void SymbolTable::printTable(std::string filename) {
    std::ofstream outfile(filename, std::ios::app);
    outfile << "\nSymbol Table" << std::endl;
    outfile << std::left << std::setw(15) << "Identifier" 
            << std::left << std::setw(15) << "Address" 
            << "Type" << std::endl;
    
    outfile << "--------------------------------------------" << std::endl;

    for (const auto& pair : table) {
        const Entry& entry = pair.second;

        outfile << std::left << std::setw(15) << entry.var_name 
                << std::left << std::setw(15) << entry.memory_address 
                << entry.type << std::endl;
    }
    outfile << std::endl;
    
    outfile.close();
}

