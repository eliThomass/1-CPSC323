#include "lexer.h"
#include <unordered_set>
#include <cctype>

// Private Functions (Constructor + Helpers)

Lexer::Lexer(std::string source_code) {
    this->input = source_code;
    this->position = 0;
    this->curr_line = 1;
}

char Lexer::getCurrentChar() {
    if (position >= (int)input.size())
        return '\0';
    return input[position];
}

void Lexer::advance() {
    if (position < (int)input.size()) {
        if (input[position] == '\n') {
            curr_line++;
        }
        position++;
    }
}
//not in use feel free to delete after testing
//bool Lexer::isDelimiter() {
//    switch (getCurrentChar()) {
//        // Does current char == case (delimiter)?
//    case '(':
//    case ')':
//    case '{':
//    case '}':
//    case ';':
//    case ',':
//    case '#':
//    case '+':
//    case '-':
//    case '*':
//    case '/':
//    case '=':
//    case '<':
//    case '>':
//    case '!':
//        return true;
//        // If not delimiter, return false
//    default:
//        return false;
//    }
//}

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

    // Does the word exist in out set?
    int isword = keywords.count(word);

    // True if word is in set, false otherwise
    return isword == 1;
}

bool Lexer::IdDFSM(const std::string& word) {
    if (word.empty()) return false;  // reject empty

    // 0=Letter, 1=Digit, 2='$', 3=Other
    auto char_to_col = [this](char ch) -> int {
        if (isLetter(ch)) return 0;
        if (isDigit(ch))  return 1;
        if (ch == '$')    return 2;
        return 3;
        };

    // States: 1=start, 2=letter, 3=letter, 4=digit, 5='$', 6=dead
    // Accepting: 2,3,4,5
    static const int T[7][4] = {
        /*0:unused*/ {0, 0, 0, 0},
        /*1:start */ {2, 6, 6, 6},   // must start with letter
        /*2:letter*/ {3, 4, 5, 6},
        /*3:letter*/ {3, 4, 5, 6},
        /*4:digit */ {3, 4, 5, 6},
        /*5:$     */ {3, 4, 5, 6},
        /*6:dead  */ {6, 6, 6, 6}
    };

    int state = 1;
    for (char ch : word) {
        int col = char_to_col(ch);
        state = T[state][col];
        if (state == 6) return false; // early reject
    }
    return (state == 2 || state == 3 || state == 4 || state == 5);
}
// Public functions

Token Lexer::getNextToken() {
    // --- small local helper: look ahead without consuming ---
    auto peek = [&](int off = 1) -> char {
        int i = position + off;                                   // compute the index to peek
        if (i < 0 || i >= (int)input.size()) return '\0';         // out of range => pretend EOF
        return input[i];                                          // return the lookahead character
        };

    // 1) Skip whitespace and "quoted comments"
    while (true) {
        char c = getCurrentChar();                                // current character (or '\0' at EOF)
        if (c == '\0')                                            // end of input => return EOF sentinel
            return Token{ UNKNOWN, "", curr_line };

        // plain whitespace
        if (c == ' ' || c == '\t' || c == '\r') {                 // skip spaces/tabs/CR
            advance();                                            // move forward one character
            continue;                                             // keep skipping
        }
        if (c == '\n') {                                          // newline encountered
            advance();                                            // consume newline
            continue;                                             // keep skipping
        }

        // skips comments
        if (c == '"') {                                           // start of a quoted comment
            advance();                                            // consume the opening quote
            while (true) {                                        // scan until we find a closing quote
                char d = getCurrentChar();                        // char inside comment
                if (d == '\0')                                    // unterminated comment -> treat like EOF
                    return Token{ UNKNOWN, "", curr_line };
                if (d == '\n') curr_line++;                       // keep line count correct while skipping
                if (d == '"') {                                   // found the closing quote
                    advance();                                    // consume it
                    break;                                        // done skipping comment
                }
                advance();                                        // consume comment content
            }
            continue;                                             // after comment, resume skipping
        }

        break;                                                    // non-space / non-comment => token starts here
    }

    char c = getCurrentChar();
    if (c == '\0') return Token{ UNKNOWN, "", curr_line };        // guard for EOF

    // IDENTIFIER / KEYWORD 
    if (isLetter(c)) {                                            // likely identifier/keyword
        int start = position;                                     // remember start index
        advance();                                                // consume first (letter) char

        // consume tail: (letter | digit | '$')*
        while (true) {
            char d = getCurrentChar();
            if (isLetter(d) || isDigit(d) || d == '$')            // valid continuation?
                advance();                                        // yes -> keep consuming
            else
                break;                                            // stop when it’s no longer valid
        }

        std::string lex = input.substr(start, position - start);  // slice the candidate lexeme

        // keywords are case-insensitive; normalize a copy for matching
        std::string lower = lex;
        for (char& ch : lower)                                    //ASCII tolower
            if (ch >= 'A' && ch <= 'Z') ch = char(ch - 'A' + 'a');

        if (isKeyword(lower)) {
            return Token{ getKeywordType(lower), lex, curr_line };
        }

        // dfsm check for id
        if (IdDFSM(lex)) {
            return Token{ IDENTIFIER, lex, curr_line };
        }
        else {
            return Token{ UNKNOWN, lex, curr_line };              // DFA rejected
        }
    }

    // 3) NUMBERS (Integer or Real)
    //    Slice per the spec, then (optionally) delegate to DFAs you add later.
    //    - Integer: digits+
    //    - Real:    digits+ '.' digits+   OR   '.' digits+    (NOT '123.')
    if (isDigit(c)) {                                             // starts with a digit -> number
        int start = position;                                     // mark start of number
        while (isDigit(getCurrentChar()))                         // consume integer part
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
    
    // two-char ops
    if (c == '=' && peek() == '=') { advance(); advance(); return Token{ OP_EQUAL,         "==", curr_line }; }
    if (c == '!' && peek() == '=') { advance(); advance(); return Token{ OP_NOT_EQUAL,     "!=", curr_line }; }
    if (c == '<' && peek() == '=') { advance(); advance(); return Token{ OP_LESS_EQUAL,    "<=", curr_line }; }
    //not in the relop (rule 24) if (c == '>' && peek() == '=') { advance(); advance(); return Token{ OP_GREATER_EQUAL, ">=", curr_line }; }
    if (c == '=' && peek() == '>') {
        advance(); advance();
        return Token{ OP_GREATER_EQUAL, "=>", curr_line };
    }
    // one-char ops
    if (c == '=') { advance(); return Token{ OP_ASSIGN,  "=", curr_line }; }
    if (c == '+') { advance(); return Token{ OP_PLUS,    "+", curr_line }; }
    if (c == '-') { advance(); return Token{ MINUS,      "-", curr_line }; }
    if (c == '*') { advance(); return Token{ OP_MULTIPLY,"*", curr_line }; }
    if (c == '/') { advance(); return Token{ OP_DIVIDE,  "/", curr_line }; }
    if (c == '<') { advance(); return Token{ OP_LESS,    "<", curr_line }; }
    if (c == '>') { advance(); return Token{ OP_GREATER, ">", curr_line }; }

    // 6) Fallback: unknown character — consume one so we don't loop forever
    std::string unk(1, c);                                       // make a one-char string
    advance();                                                   // consume it
    return Token{ UNKNOWN, unk, curr_line };                     // report unknown
}


std::vector<Token> Lexer::getAllTokens() {
    std::vector<Token> tokens;                       // this will hold the full token stream

    for (;;) {                                       // keep asking for tokens until EOF
        Token t = getNextToken();                    // scan the next token from input

        // We use (UNKNOWN, "") as our EOF sentinel.
        // If you ever change the EOF signal, update this condition.
        if (t.type == UNKNOWN && t.value.empty()) {
            break;                                   // reached end-of-input; stop scanning
        }

        tokens.push_back(std::move(t));              // keep the token (UNKNOWN with text also kept)
    }

    return tokens;                                   // return the complete list to caller
}

std::string Lexer::getTokenName(TokenType type) {
    switch (type) {
        // Keywords
    case KEYWORD_FUNCTION:    return "KEYWORD_FUNCTION";
    case KEYWORD_INTEGER:     return "KEYWORD_INTEGER";
    case KEYWORD_BOOLEAN:     return "KEYWORD_BOOLEAN";
    case KEYWORD_REAL:        return "KEYWORD_REAL";
    case KEYWORD_IF:          return "KEYWORD_IF";
    case KEYWORD_FI:          return "KEYWORD_FI";
    case KEYWORD_ELSE:        return "KEYWORD_ELSE";
    case KEYWORD_RETURN:      return "KEYWORD_RETURN";
    case KEYWORD_GET:         return "KEYWORD_GET";
    case KEYWORD_PUT:         return "KEYWORD_PUT";
    case KEYWORD_WHILE:       return "KEYWORD_WHILE";
    case KEYWORD_TRUE:        return "KEYWORD_TRUE";
    case KEYWORD_FALSE:       return "KEYWORD_FALSE";

        // Identifiers & literals
    case IDENTIFIER:          return "IDENTIFIER";
    case INTEGER_LITERAL:     return "INTEGER_LITERAL";
    case REAL_LITERAL:        return "REAL_LITERAL";

        // Operators
    case OP_PLUS:             return "OP_PLUS";
    case MINUS:               return "MINUS";
    case OP_MULTIPLY:         return "OP_MULTIPLY";
    case OP_DIVIDE:           return "OP_DIVIDE";
    case OP_ASSIGN:           return "OP_ASSIGN";
    case OP_EQUAL:            return "OP_EQUAL";
    case OP_NOT_EQUAL:        return "OP_NOT_EQUAL";
    case OP_LESS:             return "OP_LESS";
    case OP_GREATER:          return "OP_GREATER";
    case OP_LESS_EQUAL:       return "OP_LESS_EQUAL";
    case OP_GREATER_EQUAL:    return "OP_GREATER_EQUAL";

        // Separators
    case SEP_SEMICOLON:       return "SEP_SEMICOLON";
    case SEP_COMMA:           return "SEP_COMMA";
    case SEP_HASH:            return "SEP_HASH";
    case SEP_LEFT_PAREN:      return "SEP_LEFT_PAREN";
    case SEP_RIGHT_PAREN:     return "SEP_RIGHT_PAREN";
    case SEP_LEFT_BRACE:      return "SEP_LEFT_BRACE";
    case SEP_RIGHT_BRACE:     return "SEP_RIGHT_BRACE";

        // Catch-all
    case UNKNOWN:             return "UNKNOWN";
    }

    return "UNKNOWN"; // default fallback
}

std::string Lexer::getCategoryName(TokenType type) {
    switch (type) {
    case KEYWORD_FUNCTION: case KEYWORD_INTEGER: case KEYWORD_BOOLEAN:
    case KEYWORD_REAL: case KEYWORD_IF: case KEYWORD_FI:
    case KEYWORD_ELSE: case KEYWORD_RETURN: case KEYWORD_GET:
    case KEYWORD_PUT: case KEYWORD_WHILE:
        return "keyword";

    case IDENTIFIER:        return "identifier";
    case INTEGER_LITERAL:   return "integer";
    case REAL_LITERAL:      return "real";

    case OP_PLUS: case MINUS: case OP_MULTIPLY: case OP_DIVIDE:
    case OP_ASSIGN: case OP_EQUAL: case OP_NOT_EQUAL:
    case OP_LESS: case OP_GREATER: case OP_LESS_EQUAL: case OP_GREATER_EQUAL:
        return "operator";

    case SEP_SEMICOLON: case SEP_COMMA: case SEP_HASH:
    case SEP_LEFT_PAREN: case SEP_RIGHT_PAREN:
    case SEP_LEFT_BRACE: case SEP_RIGHT_BRACE:
        return "separator";

    default:
        return "unknown";
    }
}


TokenType Lexer::getKeywordType(std::string word) {
    if (word == "function") return KEYWORD_FUNCTION;
    if (word == "integer")  return KEYWORD_INTEGER;
    if (word == "boolean")  return KEYWORD_BOOLEAN;
    if (word == "real")     return KEYWORD_REAL;
    if (word == "if")       return KEYWORD_IF;
    if (word == "fi")       return KEYWORD_FI;
    if (word == "else")     return KEYWORD_ELSE;
    if (word == "return")   return KEYWORD_RETURN;
    if (word == "get")      return KEYWORD_GET;
    if (word == "put")      return KEYWORD_PUT;
    if (word == "while")    return KEYWORD_WHILE;
    if (word == "true")     return KEYWORD_TRUE;
    if (word == "false")    return KEYWORD_FALSE;

    return UNKNOWN; // shouldn’t happen if isKeyword was true
}

//not in use - can delete
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
