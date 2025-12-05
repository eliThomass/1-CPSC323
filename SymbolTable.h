#include "parser.h"
#include "lexer.h"
#include <unordered_map>

struct Entry {
    int memory_address;
    std::string var_name;
    TokenType type;
};

class SymbolTable {
private:
    int next_available_address;
    std::unordered_map<std::string, Entry> table;
public:
    SymbolTable(int initial_address);
    int symbolPush(std::string& var_name, TokenType type);
    Entry* lookup(std::string& var_name);
}