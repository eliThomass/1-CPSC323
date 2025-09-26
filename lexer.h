#ifndef LEXER_H
#define LEXER_H

#include <string>
#include <vector>

// tokens for rat25 lang
enum TokenType {
    // keywords for declaring types
    FUNCTION, INTEGER_TYPE, BOOLEAN_TYPE, REAL_TYPE,
    IF, FI, ELSE, RETURN, GET, PUT, WHILE,
    
    // identifiers and literal values
    IDENTIFIER, INTEGER, REAL, BOOLEAN,
    
    // operators
    PLUS, MINUS, MULTIPLY, DIVIDE, ASSIGN,
    EQUAL, NOT_EQUAL, LESS, GREATER, LESS_EQUAL, GREATER_EQUAL,
    
    // separators
    SEMICOLON, COMMA, HASH, LEFT_PAREN, RIGHT_PAREN, LEFT_BRACE,
    RIGHT_BRACE,
    
    // other
    COMMENT, UNKNOWN, END_FILE
};

// soken structure
struct Token {
    TokenType type;
    std::string value;
    int line;
};

class Lexer {
private:
    std::string input;
    int position;
    int currentLine;
    
    // helper functions
    char getCurrentChar();
    char getNextChar();
    void skipWhitespace();
    bool isLetter(char c);
    bool isDigit(char c);
    bool isKeyword(std::string word);
    TokenType getKeywordType(std::string word);
    
public:
    Lexer(std::string code);
    Token getNextToken();
    std::vector<Token> getAllTokens();
    std::string getTokenName(TokenType type);
};

#endif
