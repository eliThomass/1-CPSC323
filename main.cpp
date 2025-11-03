#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include "lexer.h"
#include <sstream> // Read whole input into one string
#include "parser.h"

int main() {
    // Prompt user for an input file to parse
    std::vector<std::string> filesin = {"test_small.txt", "test_medium.txt", "test_large.txt"};

    // Loop files, check if file exists
    for(std::string filein : filesin) {
        std::ifstream file(filein);
        if (!file) {
            std::cerr << "Error: could not open input file: " << filein << "\n";
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
        std::string outputFileName = filein.substr(0, filein.length() - 4) + "_output.txt";
        std::ofstream outFile(outputFileName);

        std::cout << std::left << "----------------" << filein << "----------------" << "\n";

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

        Parser parser(lexer, true);
        parser.parse();

    }
    return 0;
}
