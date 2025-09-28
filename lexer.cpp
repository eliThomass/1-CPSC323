#include "lexer.h"
#include <unordered_set>


// Private Functions (Constructor + Helpers)

Lexer::Lexer(std::string source_code) {
    this->input = source_code;
    this->position = 0;
    this->curr_line = 1;
}

char Lexer::getCurrentChar() {
    return input[position];
}

void Lexer::advance() {
    position++;
}

bool Lexer::isDelimiter() {
    switch(getCurrentChar()) {
        // Does current char == case (delimiter)?
        case '(':
        case ')':
        case '{':
        case '}':
        case ';':
        case ',':
        case '#':
        case '+':
        case '-':
        case '*':
        case '/':
        case '=':
        case '<':
        case '>':
        case '!':
            return true;
        // If not delimiter, return false
        default:
            return false;
    }
}

bool Lexer::isLetter(char c) {
    return isalpha(c);
}

bool Lexer::isDigit(char c) {
    return isdigit(c);
}

bool Lexer::isKeyword(std::string word) {
    static const std::unordered_set<std::string> keywords = {
        "function", "integer", "boolean", "real", "if", "fi", 
        "else", "return", "get", "put", "while"
    };

    // Does the word exist in out set?
    int isword = keywords.count(word);

    // True if word is in set, false otherwise
    return isword == 1;
}

//bool Lexer::IdDFSM(const std::string& word){
//    if (word.empty()) return false;
//    
//    int state = 1;
//
//    for (char c : word) {
//        if (state == 1)
//        {
//            if (isLetter(c))
//                state = 2;
//            else if (isDigit(c) || c == '$')
//                state == 6;
//            else
//                return false;
//        }
//        else if (state == 2)
//        {
//            if (isLetter(c))
//                state = 3;
//            else if (isDigit(c))
//                state = 4;
//            else if (c == '$')
//                state = 5;
//            else
//                return false;
//        }
//        else if (state == 3)
//        {
//            if (isLetter(c))
//                state = 3;
//            else if (isDigit(c))
//                state = 4;
//            else if (c == '$')
//                state = 5;
//            else
//                return false;
//        }
//        else if (state == 4)
//        {
//            if (isLetter(c))
//                state = 3;
//            else if (isDigit(c))
//                state = 4;
//            else if (c == '$')
//                state = 5;
//            else
//                return false;
//        }
//        else if (state == 5)
//        {
//            if (isLetter(c))
//                state = 3;
//            else if (isDigit(c))
//                state = 4;
//            else if (c == '$')
//                state = 5;
//            else
//                return false;
//        }
//        else
//            return false;
//    }
//    
//    // Accepting states == 2,3,4,5
//    return(state == 2 || state == 3 || state == 4 || state == 5);
//
//    }

// Public functions

Token Lexer::getNextToken(){
    std::string token = "";
    while getCurrentChar() != ' ' {
        
    }
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
