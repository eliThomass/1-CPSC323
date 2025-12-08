#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"
#include "SymbolTable.h"
#include "assembler.h"
#include <fstream>

class Parser
{
private:
    Lexer& lexer; // lexer reference
    Token current_token;
    bool print_switch; // printing on/off
    std::ostream& out_stream; // needed to print to output file
    SymbolTable st; // Class instance so we can add to symbol table
    AssemblyCodeGenerator ACG; // Another class instance so we can generate assembly code.
    
    // Grammar rule functions; reference project document
    void Rat25F();

    // R2 - R4
    void OptFunctionDefinitions();
    void FunctionDefinitions();
    void FunctionDefinitionsPrime();
    void Function();

    // R5 - R8
    void OptParameterList();
    void ParameterList();
    void ParameterListPrime();
    void Parameter();
    void Qualifier();

    // R9 - R13
    void Body();
    void OptDeclarationList();
    void DeclarationList();
    void DeclarationListPrime();
    void Declaration();
    void IDs(std::string declared_type);
    void IDsPrime(std::string declared_type);

    // R14 - R17
    void StatementList();
    void StatementListPrime();
    void Statement();
    void Compound();
    void Assign();

    // R18
    void If();
    void IfPrime();

    // R19
    void Return();
    void ReturnPrime();

    // R20 - R22
    void Print();
    void Scan();
    void While();

    // R23 - R24
    void Condition();
    void Relop();

    // R25 - R26
    void Expression();
    void ExpressionPrime();
    void Term();
    void TermPrime();

    // R27 - R28
    void Factor();
    void Primary();
    void PrimaryPrime();

    // Helper functions
    void match(TokenType expected);
    void nextToken(); // nwxt token in lexer stream
    void error(const std::string& message);

public:
    // Constructor and Destructor
    Parser(Lexer& lexer, std::ostream& output_stream, bool print_switch = false);
    
    // Main parsing function
    void parse();
};

#endif