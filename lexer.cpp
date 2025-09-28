#include "lexer.h"


// Private Functions (Constructor + Helpers)

Lexer::Lexer(std::string source_code) {
    this->input = source_code;
    this->position = 0;
    this->curr_line = 1;
}

char Lexer::getCurrentChar() {

}

char Lexer::getNextChar() {

}

bool Lexer::isDelimiter() {

}
bool Lexer::isLetter(char c) {

}

bool Lexer::isDigit(char c) {

}

bool Lexer::isKeyword(std::string word) {

}

// Public functions

Token Lexer::getNextToken(){

}

std::vector<Token> Lexer::getAllTokens() {

}

std::string Lexer::getTokenName(TokenType type) {

}





// nextToken() {
//     while (pos < code.legnth*()) {
//         char current_char = peek();

//         if (is_whitespace(current_char)) {
//             // continue?
//             continue;
//         }

//         // the first value has to be a letter
//         if (isalpha(current_char)) {
//             return // identifier function
//         }

        
//     }
// }