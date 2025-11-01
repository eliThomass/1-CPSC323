#include "lexer.h"
#include <fstream>

class Parser
{
private:
    Lexer& lexer; // lexer reference
    Token current_token;
    bool print_switch; // printing on/off
    
    // Grammar rule functions
    void StatementList(); //sequence of statements
    void Statement(); // decides what king of statement
    void Assign(); // like x = 5
    void Expression(); // like x+y-z
    void ExpressionPrime(); // tail ends of expressions
    void Term(); // like a * b
    void TermPrime(); // tail ends of terms
    void Factor(); // num, identifier, expressions in parenthesises
    void Condition(); // x > 5 or a == n
    void If();
    void Return();
    void Print(); // put
    void Scan(); // get
    void While();
    
    // Helper functions
    void match(TokenType expected);
    void error(const std::string& message); // 
    void nextToken(); // nwxt token in lexer stream

public:
    // Constructor and Destructor
    Parser(Lexer& lexer, bool print_switch = false);
    ~Parser();
    
    // Main parsing function
    void parse();
};
