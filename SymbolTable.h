#include "parser.h"
#include "lexer.h"
#include <map>

struct Entry {
    int memory_address;
    std::string var_name;
    TokenType type;
};

class SymbolTable {
private:
    int next_available_address;
    std::map<std::string, Entry> table;
public:
    SymbolTable();
    bool symbolPush(Token symbol);
    int getAddress(std::string& lexeme);
};