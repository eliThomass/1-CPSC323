#ifndef LEXER_H
#define LEXER_H

#include <string>
#include <vector>

// tokens for rat25 lang
enum TokenType {
    // keywords for declaring types
    KEYWORD_FUNCTION, KEYWORD_INTEGER, KEYWORD_BOOLEAN, KEYWORD_REAL,
    KEYWORD_IF, KEYWORD_FI, KEYWORD_ELSE, KEYWORD_RETURN,
    KEYWORD_GET, KEYWORD_PUT, KEYWORD_WHILE, KEYWORD_TRUE, KEYWORD_FALSE,

    // identifiers and literal values
    IDENTIFIER, INTEGER_LITERAL, REAL_LITERAL,

    // operators
    OP_PLUS, MINUS, OP_MULTIPLY, OP_DIVIDE, OP_ASSIGN,
    OP_EQUAL, OP_NOT_EQUAL, OP_LESS, OP_GREATER, OP_LESS_EQUAL, OP_GREATER_EQUAL,

    // separators
    SEP_SEMICOLON, SEP_COMMA, SEP_HASH, SEP_LEFT_PAREN, SEP_RIGHT_PAREN, SEP_LEFT_BRACE,
    SEP_RIGHT_BRACE,

    // other
    UNKNOWN
};

// token structure
struct Token {
    TokenType type;
    std::string value;
    int line;
};

class Lexer {
private:
    std::string input;
    int position;
    int curr_line;

    // helper functions
    char getCurrentChar();
    void advance();
    bool isLetter(char c);
    bool isDigit(char c);
    bool isKeyword(std::string word);
    bool IdDFSM(const std::string& word);
    bool intRealDFSM(const std::string& word);
    TokenType getKeywordType(std::string word);

public:
    Lexer(std::string code);
    Token getNextToken();
    std::vector<Token> getAllTokens();
    std::string getTokenName(TokenType type);
    std::string getCategoryName(TokenType type);
};

#endif
