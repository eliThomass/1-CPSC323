#include "lexer.h"
#include <fstream>

class Parser
{
private:
    Lexer& lexer; // lexer reference
    Token current_token;
    bool print_switch; // printing on/off
    
    // Grammar rule functions; reference project document
    void Rat25F();
    void OptFunctionDefinitions();
    void FunctionDefinitions();
    void Function();
    void OptParameterList();
    void ParameterList();
    void Parameter();
    void Qualifier();
    void Body();
    void OptDeclarationList();
    void DeclarationList();
    void Declaration();
    void IDs();
    void StatementList();
    void Statement();
    void Compound();
    void Assign();
    void If();
    void Return();
    void Print();
    void Scan();
    void While();
    void Condition();
    void Relop();
    void Expression();
    void ExpressionPrime();
    void Term();
    void TermPrime();
    void Factor();
    void Primary();

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
