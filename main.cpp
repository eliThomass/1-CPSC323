#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include "lexer.h"
#include <sstream> // Read whole input into one string

int main() {
    std::ifstream file("test_input.txt");

    // Put all of source code into one long string
    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string sourceCode = buffer.str();
    file.close();

    Lexer lexer(sourceCode);
    Token test = lexer.getNextToken();
    std::cout << test.value << std::endl;
    std::vector<Token> tokens = lexer.getAllTokens();

    std::cout << "Lexical Analysis Results\n";
    for (auto& token : tokens) {
        std::cout << "Line " << token.line << ":\t"
                  << lexer.getTokenName(token.type) << "\t\t('"
                  << token.value << "')\n";
    }

    return 0;
}