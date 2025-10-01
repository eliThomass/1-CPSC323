#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include "lexer.h"
#include <sstream> // Read whole input into one string

int main() {
    // Prompt user for an input file to parse
    std::string filein;
    std::cout << "Enter input file: ";
    std::cin >> filein;

    // Check if file exists
    std::ifstream file(filein);
    if (!file) {
        std::cerr << "Error: could not open input file\n";
        return 1;
    }

    // Put all of source code into one long string
    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string sourceCode = buffer.str();
    file.close();

    // Initialize our lexer, then get all tokens from sourceCode
    Lexer lexer(sourceCode);
    std::vector<Token> tokens = lexer.getAllTokens();

    // Opens an output file for writing results
    std::ofstream outFile("tokens_output.txt");

    std::cout << std::left << std::setw(15) << "token" << std::setw(6) << "|" << "lexeme" << "\n";
    outFile << std::left << std::setw(15) << "token" << std::setw(6) << "|" << "lexeme\n";

    std::cout << std::endl;
    outFile << std::endl;

    // Print tokens
    for (auto& token : tokens) {
        std::string category = lexer.getCategoryName(token.type);

        std::cout << std::left << std::setw(15) << category << std::setw(6) << "|"<< token.value << "\n";
        outFile << std::left << std::setw(15) << category << std::setw(6) << "|" << token.value << "\n";
    }
    outFile.close();
    return 0;
}
