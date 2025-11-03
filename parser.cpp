#include "parser.h"
#include <iostream>
#include <stdexcept>

Parser::Parser(Lexer& lexer, bool print_switch): lexer(lexer), print_switch(print_switch) 
{
    nextToken();
}


// The main parsing function, we use it to start the recursion
void Parser::parse()
{
    // If any recursion inside Rat25 throws an error, we know we have a syntax error.
    try {
        Rat25F();
        std::cout << "Parse Successful" << std::endl;

    } catch (const std::runtime_error& e) {
        std::cerr << "Parse Failed: " << e.what() << std::endl;
        std::cerr << "  ...at token: " << current_token.value
                  << " (Line " << current_token.line << ")" << std::endl;
    }
}

void Parser::nextToken()
{
    current_token = lexer.getNextToken();
}

// Check if our current token matches what we should have
void Parser::match(TokenType expected)
{
    if (current_token.type == expected) {
        nextToken();
    } else {
        std::string err_msg = "Token type did not match expected";
        throw std::runtime_error(err_msg);
    }
}

// R1
void Parser::Rat25F() {
    if (print_switch) { std::cout << "Entering <Rat25F>" << std::endl; }

    if (print_switch) { std::cout << "Exiting <Rat25F>" << std::endl; }
}

// R2
void Parser::OptFunctionDefinitions() {
    if (print_switch) { std::cout << "Entering <OptFunctionDefinitions>" << std::endl; }

    if (print_switch) { std::cout << "Exiting <OptFunctionDefinitions>" << std::endl; }
}

// R3
void Parser::FunctionDefinitions() {
    if (print_switch) { std::cout << "Entering <FunctionDefinitions>" << std::endl; }

    if (print_switch) { std::cout << "Exiting <FunctionDefinitions>" << std::endl; }
}

// R4
void Parser::Function() {
    if (print_switch) { std::cout << "Entering <Function>" << std::endl; }

    if (print_switch) { std::cout << "Exiting <Function>" << std::endl; }
}

// R5
void Parser::OptParameterList() {
    if (print_switch) { std::cout << "Entering <OptParameterList>" << std::endl; }

    if (print_switch) { std::cout << "Exiting <OptParameterList>" << std::endl; }
}

// R6
void Parser::ParameterList() {
    if (print_switch) { std::cout << "Entering <ParameterList>" << std::endl; }

    if (print_switch) { std::cout << "Exiting <ParameterList>" << std::endl; }
}

// R7
void Parser::Parameter() {
    if (print_switch) { std::cout << "Entering <Parameter>" << std::endl; }

    if (print_switch) { std::cout << "Exiting <Parameter>" << std::endl; }
}

// R8
void Parser::Qualifier() {
    if (print_switch) { std::cout << "Entering <Qualifier>" << std::endl; }

    if (print_switch) { std::cout << "Exiting <Qualifier>" << std::endl; }
}

// R9
void Parser::Body() {
    if (print_switch) { std::cout << "Entering <Body>" << std::endl; }

    if (print_switch) { std::cout << "Exiting <Body>" << std::endl; }
}

// R10
void Parser::OptDeclarationList() {
    if (print_switch) { std::cout << "Entering <OptDeclarationList>" << std::endl; }
    // TODO: Implement R10
    if (print_switch) { std::cout << "Exiting <OptDeclarationList>" << std::endl; }
}

// R11
void Parser::DeclarationList() {
    if (print_switch) { std::cout << "Entering <DeclarationList>" << std::endl; }

    if (print_switch) { std::cout << "Exiting <DeclarationList>" << std::endl; }
}

// R12
void Parser::Declaration() {
    if (print_switch) { std::cout << "Entering <Declaration>" << std::endl; }

    if (print_switch) { std::cout << "Exiting <Declaration>" << std::endl; }
}

// R13
void Parser::IDs() {
    if (print_switch) { std::cout << "Entering <IDs>" << std::endl; }

    if (print_switch) { std::cout << "Exiting <IDs>" << std::endl; }
}

// R14
void Parser::StatementList() {
    if (print_switch) { std::cout << "Entering <StatementList>" << std::endl; }

    if (print_switch) { std::cout << "Exiting <StatementList>" << std::endl; }
}

// R15
void Parser::Statement() {
    if (print_switch) { std::cout << "Entering <Statement>" << std::endl; }

    if (print_switch) { std::cout << "Exiting <Statement>" << std::endl; }
}

// R16
void Parser::Compound() {
    if (print_switch) { std::cout << "Entering <Compound>" << std::endl; }

    if (print_switch) { std::cout << "Exiting <Compound>" << std::endl; }
}

// R17
void Parser::Assign() {
    if (print_switch) { std::cout << "Entering <Assign>" << std::endl; }

    if (print_switch) { std::cout << "Exiting <Assign>" << std::endl; }
}

// R18
void Parser::If() {
    if (print_switch) { std::cout << "Entering <If>" << std::endl; }

    if (print_switch) { std::cout << "Exiting <If>" << std::endl; }
}

// R19
void Parser::Return() {
    if (print_switch) { std::cout << "Entering <Return>" << std::endl; }

    if (print_switch) { std::cout << "Exiting <Return>" << std::endl; }
}

// R20
void Parser::Print() {
    if (print_switch) { std::cout << "Entering <Print>" << std::endl; }

    if (print_switch) { std::cout << "Exiting <Print>" << std::endl; }
}

// R21
void Parser::Scan() {
    if (print_switch) { std::cout << "Entering <Scan>" << std::endl; }

    if (print_switch) { std::cout << "Exiting <Scan>" << std::endl; }
}

// R22
void Parser::While() {
    if (print_switch) { std::cout << "Entering <While>" << std::endl; }

    if (print_switch) { std::cout << "Exiting <While>" << std::endl; }
}

// R23
void Parser::Condition() {
    if (print_switch) { std::cout << "Entering <Condition>" << std::endl; }

    if (print_switch) { std::cout << "Exiting <Condition>" << std::endl; }
}

// R24
void Parser::Relop() {
    if (print_switch) { std::cout << "Entering <Relop>" << std::endl; }

    if (print_switch) { std::cout << "Exiting <Relop>" << std::endl; }
}

// R25
void Parser::Expression() {
    if (print_switch) { std::cout << "Entering <Expression>" << std::endl; }
    
    Term();
    ExpressionPrime();
    
    if (print_switch) { std::cout << "Exiting <Expression>" << std::endl; }
}

// R25'
void Parser::ExpressionPrime() {
    if (print_switch) { std::cout << "Entering <ExpressionPrime>" << std::endl; }

    if (print_switch) { std::cout << "Exiting <ExpressionPrime>" << std::endl; }
}

// R26
void Parser::Term() {
    if (print_switch) { std::cout << "Entering <Term>" << std::endl; }
    
    Factor();
    TermPrime();
    
    if (print_switch) { std::cout << "Exiting <Term>" << std::endl; }
}

// R26'
void Parser::TermPrime() {
    if (print_switch) { std::cout << "Entering <TermPrime>" << std::endl; }
    
    if (print_switch) { std::cout << "Exiting <TermPrime>" << std::endl; }
}

// R27
void Parser::Factor() {
    if (print_switch) { std::cout << "Entering <Factor>" << std::endl; }

    if (print_switch) { std::cout << "Exiting <Factor>" << std::endl; }
}

// R28
void Parser::Primary() {
    if (print_switch) { std::cout << "Entering <Primary>" << std::endl; }

    if (print_switch) { std::cout << "Exiting <Primary>" << std::endl; }
}
