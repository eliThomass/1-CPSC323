#include "lexer.h"
#include <unordered_set>
#include <cctype>
#include <iostream>

// Private Functions (Constructor + Helpers)

Lexer::Lexer(std::string source_code) {
    this->input = source_code;
    this->position = 0;
    this->curr_line = 1;
}

char Lexer::getCurrentChar() {
    // If our position is greater than our input,
    // our end of file character will be /0
    if (position >= (int)input.size())
        return '\0';
    return input[position];
}

void Lexer::advance() {
    // Update position and potentially current line
    if (position < (int)input.size()) {
        if (input[position] == '\n') {
            curr_line++;
        }
        position++;
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
        "else", "return", "get", "put", "while", "true", "false"
    };

    // Does the word exist in our set?
    int isword = keywords.count(word);

    // True if word is in set, false otherwise
    return isword == 1;
}

bool Lexer::IdDFSM(const std::string& word) {
    if (word.empty()) return false;

    // 0=Letter, 1=Digit, 2='$', 3=Other
    // We use a lambda function here since we
    // can't declare a function inside a function
    auto char_to_col = [this](char ch) -> int {
        if (isLetter(ch)) return 0;
        if (isDigit(ch))  return 1;
        if (ch == '$')    return 2;
        return 3;
    };

    // States: 1=starting state, 2=letter, 3=letter, 4=digit, 5='$', 6=dead
    // Accepting: 2,3,4,5
    static const int T[7][4] = {
        {0, 0, 0, 0}, // Unused state
        {2, 6, 6, 6}, // Starting state (1)
        {3, 4, 5, 6}, // First letter state (2)
        {3, 4, 5, 6}, // Repeat letter state (3)
        {3, 4, 5, 6}, // Digit state (4)
        {3, 4, 5, 6}, // $ Symbol state (5)
        {6, 6, 6, 6}  // Dead state (6)
    };

    int state = 1;
    
    for (char ch : word) {
        int col = char_to_col(ch);
        state = T[state][col];

        // If our state ever hits 6, the ID is not valid
        if (state == 6) return false;
    }
    return (state == 2 || state == 3 || state == 4 || state == 5);
}

bool Lexer::intRealDFSM(const std::string& word) {
    if (word.empty()) return false;

    // 0=Digit, 1='.', 2=Other
    auto char_to_col = [this](char ch) -> int {
        if (isDigit(ch)) return 0;
        if (ch == '.')   return 1;
        return 2;
    };

    // States: 1=start, 2=integer, 3=decimal point, 4=real, 5=dead
    // Accepting: 2 (integer), 4 (real)
    static const int T[6][3] = {
        {0, 0, 0}, // Unused state
        {2, 3, 5}, // Start state (1): digit->2, '.'->3, other->dead
        {2, 3, 5}, // Integer state (2): digit->2, '.'->3, other->dead  
        {4, 5, 5}, // Decimal point state (3): digit->4, other->dead
        {4, 5, 5}, // Real state (4): digit->4, other->dead
        {5, 5, 5}  // Dead state (5)
    };

    int state = 1;
    
    for (char ch : word) {
        int col = char_to_col(ch);
        state = T[state][col];

        // If our state ever hits 6, the ID is not valid
        if (state == 6) return false;
    }
    return (state == 2 || state == 3 || state == 4 || state == 5);
}



// Public functions

Token Lexer::getNextToken() {
    auto peek = [&](int off = 1) -> char {
        int i = position + off;
        if (i < 0 || i >= (int)input.size()) return '\0'; 
        return input[i];
        };

    // Skip whitespace and comments
    while (true) {
        char c = getCurrentChar();

        // If char is \0, we have reached end of file
        if (c == '\0') {
            return Token{ UNKNOWN, "", curr_line };
        }
        if (c == ' ' || c == '\t' || c == '\r') {
            advance();
            continue;
        }
        if (c == '\n') {
            advance();
            continue;
        }

        // Skip any comments
        if (c == '"') {
            advance();
            // Scan until we find a closing comment
            while (true) {
                char d = getCurrentChar();
                if (d == '\0')
                    return Token{ UNKNOWN, "", curr_line };
                if (d == '\n') curr_line++;
                // If we find closing comment, skip it then break
                if (d == '"') {
                    advance();
                    break;
                }
                // If we didn't find closing comment, advance to next char
                advance();
            }
            continue;
        }

        // Current character should now not be a comment or whitespace
        break;
    }

    char c = getCurrentChar();
    if (c == '\0') return Token{ UNKNOWN, "", curr_line };

    // Check if character is an identifier / keyword
    if (isLetter(c)) {
        int start = position;

        while (true) {
            char next_char = peek(1); 
            
            // Test string of what the lexeme would be if we advance
            std::string potential_lex = input.substr(start, position - start + 1);

            if (IdDFSM(potential_lex)) {
                // If our potential lexeme is still valid, advance to next character, otherwise break
                advance();
            } else {
                break;
            }
        }

        // Create our lexeme based off our previos starting position and current position now.
        std::string lex = input.substr(start, position - start);

        if (isKeyword(lex)) {
            return Token{ getKeywordType(lex), lex, curr_line };
        }

        // If not keyword, we know that it is an identifier
        return Token{ IDENTIFIER, lex, curr_line };
    }

    if (isDigit(c) || c == '.') {
        int start = position;
        while (isDigit(getCurrentChar()))
            advance();

        bool hasDot = false;                                      // track decimal point form
        if (getCurrentChar() == '.' && std::isdigit((unsigned char)peek())) {
            hasDot = true;                                        // we have '.' with at least one digit after
            advance();                                            // consume '.'
            while (isDigit(getCurrentChar()))                     // consume fractional digits
                advance();
        }

        std::string num = input.substr(start, position - start);  // slice numeric lexeme

        // add IntDFSM/RealDFSM, call them here:
        // if (!hasDot && IntDFSM(num)) return Token{ INTEGER_LITERAL, num, curr_line };
        // if ( hasDot && RealDFSM(num)) return Token{ REAL_LITERAL,   num, curr_line };
        // For now, classify from the shape
        return Token{ hasDot ? REAL_LITERAL : INTEGER_LITERAL, num, curr_line };
    }

    // Leading '.' form for reals like ".001"
    if (c == '.' && std::isdigit((unsigned char)peek())) {        // '.' followed by digit -> real like ".25"
        int start = position;                                     // start at '.'
        advance();                                                // consume '.'
        while (isDigit(getCurrentChar()))                         // consume digits after '.'
            advance();
        std::string num = input.substr(start, position - start);  // e.g., ".001"

        //add RealDFSM later, call it here first.
        return Token{ REAL_LITERAL, num, curr_line };             // classify as real for now
    }

    // SEPARATORS
    switch (c) {
    case '(': advance(); return Token{ SEP_LEFT_PAREN,  "(", curr_line };
    case ')': advance(); return Token{ SEP_RIGHT_PAREN, ")", curr_line };
    case '{': advance(); return Token{ SEP_LEFT_BRACE,  "{", curr_line };
    case '}': advance(); return Token{ SEP_RIGHT_BRACE, "}", curr_line };
    case ';': advance(); return Token{ SEP_SEMICOLON,   ";", curr_line };
    case ',': advance(); return Token{ SEP_COMMA,       ",", curr_line };
    case '#': advance(); return Token{ SEP_HASH,        "#", curr_line };
    }
   
    // OPERATORS
    
    // Check for two-character operators
    if (c == '=' && peek() == '=') { 
        advance(); 
        advance(); 
        return Token{ OP_EQUAL, "==", curr_line }; 
    }
    if (c == '!' && peek() == '=') { 
        advance(); 
        advance(); 
        return Token{ OP_NOT_EQUAL, "!=", curr_line }; 
    }
    if (c == '<' && peek() == '=') { 
        advance(); 
        advance(); 
        return Token{ OP_LESS_EQUAL, "<=", curr_line }; 
    }
    if (c == '=' && peek() == '>') {
        advance(); 
        advance();
        return Token{ OP_GREATER_EQUAL, "=>", curr_line };
    }

    // Check for one-character operators
    if (c == '=') {
        advance();
        return Token{ OP_ASSIGN, "=", curr_line };
    }
    if (c == '+') {
        advance();
        return Token{ OP_PLUS, "+", curr_line };
    }
    if (c == '-') {
        advance();
        return Token{ MINUS, "-", curr_line };
    }
    if (c == '*') {
        advance();
        return Token{ OP_MULTIPLY, "*", curr_line };
    }
    if (c == '/') {
        advance();
        return Token{ OP_DIVIDE, "/", curr_line };
    }
    if (c == '<') {
        advance();
        return Token{ OP_LESS, "<", curr_line };
    }
    if (c == '>') {
        advance();
        return Token{ OP_GREATER, ">", curr_line };
    }

    // Mark as unknown if all previous checks fails
    std::string unknown_char(1, c);
    advance();
    return Token{ UNKNOWN, unknown_char, curr_line };
}

std::vector<Token> Lexer::getAllTokens() {
    std::vector<Token> tokens;

    while (true) {
        Token t = getNextToken();

        // End of file check
        if (t.type == UNKNOWN && t.value.empty()) {
            break;
        }
        
        tokens.push_back(t);
    }

    return tokens;
}

std::string Lexer::getTokenName(TokenType type) {
    switch (type) {
        
        // All keywords
        case KEYWORD_FUNCTION:
            return "KEYWORD_FUNCTION";
        case KEYWORD_INTEGER:
            return "KEYWORD_INTEGER";
        case KEYWORD_BOOLEAN:
            return "KEYWORD_BOOLEAN";
        case KEYWORD_REAL:
            return "KEYWORD_REAL";
        case KEYWORD_IF:
            return "KEYWORD_IF";
        case KEYWORD_FI:
            return "KEYWORD_FI";
        case KEYWORD_ELSE:
            return "KEYWORD_ELSE";
        case KEYWORD_RETURN:
            return "KEYWORD_RETURN";
        case KEYWORD_GET:
            return "KEYWORD_GET";
        case KEYWORD_PUT:
            return "KEYWORD_PUT";
        case KEYWORD_WHILE:
            return "KEYWORD_WHILE";
        case KEYWORD_TRUE: 
            return "KEYWORD_TRUE";
        case KEYWORD_FALSE: 
            return "KEYWORD_FALSE";

        // All identifiers + literals
        case IDENTIFIER:
            return "IDENTIFIER";
        case INTEGER_LITERAL:
            return "INTEGER_LITERAL";
        case REAL_LITERAL: 
            return "REAL_LITERAL";

        // All operators
        case OP_PLUS:
            return "OP_PLUS";
        case MINUS:
            return "MINUS";
        case OP_MULTIPLY:
            return "OP_MULTIPLY";
        case OP_DIVIDE:
            return "OP_DIVIDE";
        case OP_ASSIGN:
            return "OP_ASSIGN";
        case OP_EQUAL:
            return "OP_EQUAL";
        case OP_NOT_EQUAL:
            return "OP_NOT_EQUAL";
        case OP_LESS:
            return "OP_LESS";
        case OP_GREATER:
            return "OP_GREATER";
        case OP_LESS_EQUAL:
            return "OP_LESS_EQUAL";
        case OP_GREATER_EQUAL:
            return "OP_GREATER_EQUAL";

        // All seperators
        case SEP_SEMICOLON:
            return "SEP_SEMICOLON";
        case SEP_COMMA:
            return "SEP_COMMA";
        case SEP_HASH:
             return "SEP_HASH";
        case SEP_LEFT_PAREN:
            return "SEP_LEFT_PAREN";
        case SEP_RIGHT_PAREN:
            return "SEP_RIGHT_PAREN";
        case SEP_LEFT_BRACE:
            return "SEP_LEFT_BRACE";
        case SEP_RIGHT_BRACE:
            return "SEP_RIGHT_BRACE";

        // Unknown
        case UNKNOWN: return "UNKNOWN";
    }

    return "UNKNOWN";
}

std::string Lexer::getCategoryName(TokenType type) {
    switch (type) {
        case KEYWORD_FUNCTION: 
        case KEYWORD_INTEGER: 
        case KEYWORD_BOOLEAN:
        case KEYWORD_REAL: 
        case KEYWORD_IF: 
        case KEYWORD_FI:
        case KEYWORD_ELSE: 
        case KEYWORD_RETURN: 
        case KEYWORD_GET:
        case KEYWORD_PUT: 
        case KEYWORD_WHILE:
        case KEYWORD_TRUE:
        case KEYWORD_FALSE:
            return "keyword";

        case IDENTIFIER: return "identifier";
        case INTEGER_LITERAL: return "integer";
        case REAL_LITERAL: return "real";

        case OP_PLUS: 
        case MINUS: 
        case OP_MULTIPLY: 
        case OP_DIVIDE:
        case OP_ASSIGN: 
        case OP_EQUAL: 
        case OP_NOT_EQUAL:
        case OP_LESS: 
        case OP_GREATER: 
        case OP_LESS_EQUAL: 
        case OP_GREATER_EQUAL:
            return "operator";

        case SEP_SEMICOLON: 
        case SEP_COMMA: 
        case SEP_HASH:
        case SEP_LEFT_PAREN: 
        case SEP_RIGHT_PAREN:
        case SEP_LEFT_BRACE: 
        case SEP_RIGHT_BRACE:
            return "separator";

        default:
            return "unknown";
    }
}

TokenType Lexer::getKeywordType(std::string word) {
    // Check our keyword
    if (word == "function") 
        return KEYWORD_FUNCTION;
    if (word == "integer") 
        return KEYWORD_INTEGER;
    if (word == "boolean") 
        return KEYWORD_BOOLEAN;
    if (word == "real")    
        return KEYWORD_REAL;
    if (word == "if")      
        return KEYWORD_IF;
    if (word == "fi")      
        return KEYWORD_FI;
    if (word == "else")    
        return KEYWORD_ELSE;
    if (word == "return")  
        return KEYWORD_RETURN;
    if (word == "get")
        return KEYWORD_GET; 
    if (word == "put")
        return KEYWORD_PUT;
    if (word == "while")
        return KEYWORD_WHILE;
    if (word == "true")
        return KEYWORD_TRUE;
    if (word == "false")
        return KEYWORD_FALSE;

    return UNKNOWN;
}