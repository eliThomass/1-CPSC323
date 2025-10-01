#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include "lexer.h"
#include <sstream> // Read whole input into one string

int main() {
    std::ifstream file("test_input.txt");
    if (!file) {
        std::cerr << "Error: could not open input file\n";
        return 1;
    }

    // Put all of source code into one long string
    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string sourceCode = buffer.str();
    file.close();

    Lexer lexer(sourceCode);
    Token test = lexer.getNextToken();
    std::cout << test.value << std::endl;
    std::vector<Token> tokens = lexer.getAllTokens();

    // open an output file for writing results
    std::ofstream outFile("tokens_output.txt");
    if (!outFile) {
        std::cerr << "Error: could not create output file\n";
        return 1;
    }

    // Print header
    std::cout << std::left << std::setw(12) << "token"
        << "lexeme" << "\n";
    outFile << std::left << std::setw(12) << "token" << "lexeme\n";

    // Print tokens
    for (auto& token : tokens) {
        std::string category = lexer.getCategoryName(token.type);

        // Match professor's sample exactly:
        // everything lowercase, except "Separator" (capitalized S)
        if (category == "separator")
            category = "Separator";

        std::cout << std::left << std::setw(12)
            << category
            << token.value << "\n";
        outFile << std::left << std::setw(12) 
            << category 
            << token.value << "\n";
    }
    outFile.close();
    return 0;
}
