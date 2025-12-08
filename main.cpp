#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include "lexer.h"
#include <sstream> // Read whole input into one string
#include "parser.h"
#include "SymbolTable.h"

int main() {
    // Prompt user for an input file to parse 
    std::vector<std::string> filenames = {"test_small.txt", "test_medium.txt", "test_large.txt"};

    // Loop files, check if file exists
    for (std::string filename : filenames) {
        std::ifstream file(filename);
        if (!file) {
            std::cerr << "Error: could not open input file: " << filename << "\n";
            return 1;
        }

        // Put all of source code into one long string
        std::stringstream buffer;
        buffer << file.rdbuf();
        std::string sourceCode = buffer.str();
        file.close();

        /* THIS SECTION IS FOR LEXER OUTPUT, not needed for assignment 2
        
        // Initialize our lexer, then get all tokens from sourceCode
        Lexer lexer(sourceCode);
        std::vector<Token> tokens = lexer.getAllTokens();

        // Opens an output file for writing results
        std::string outputFileNameLexer = filename.substr(0, filename.length() - 4) + "_output_lexer.txt";
        std::ofstream outFile(outputFileNameLexer);

        outFile << std::left << std::setw(15) << "token" << std::setw(6) << "|" << "lexeme\n";
        outFile << std::endl;

        // Print tokens
        for (auto& token : tokens) {
            std::string category = lexer.getCategoryName(token.type);
            outFile << std::left << std::setw(15) << category << std::setw(6) << "|" << token.value << "\n";
        }

        outFile.close();
        */

        // Initialize our output file for parser results
        std::string outputFileNameParser = filename.substr(0, filename.length() - 4) + "_output_parser.txt";
        std::ofstream outFileParser(outputFileNameParser);

        // Parse the source code
        Lexer parser_lexer(sourceCode);
        Parser parser(parser_lexer, outFileParser, true);
        parser.parse();
    }
    

    return 0;
}
