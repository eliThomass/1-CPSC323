#include "parser.h"
#include <iostream>
#include <stdexcept>

Parser::Parser(Lexer& lexer, bool print_switch) : lexer(lexer), print_switch(print_switch)
{
    nextToken();
}

void Parser::error(const std::string& error) {
    throw std::runtime_error(error);
}


// The main parsing function, we use it to start the recursion
void Parser::parse()
{
    // If any recursion inside Rat25 throws an error, we know we have a syntax error.
    try {
        Rat25F();
        std::cout << "Parse Successful" << std::endl;

    }
    catch (const std::runtime_error& e) {
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
    }
    else {
        std::string err_msg = "Token type did not match expected";
        error(err_msg);
    }
}

// R1
void Parser::Rat25F() {
    if (print_switch) { std::cout << "Entering <Rat25F>" << std::endl; }

    // Based on the compiler doc, we will call our other functions
    // for all the NONTERMINALS in the production.
    // For all the terminals in the production, we will use the match function.
    // We will repeat this for every production rule.
    OptFunctionDefinitions();
    match(SEP_HASH);
    OptDeclarationList();
    StatementList();
    match(SEP_HASH);

    if (print_switch) { std::cout << "Exiting <Rat25F>" << std::endl; }
}

// R2
void Parser::OptFunctionDefinitions() {
    if (print_switch) { std::cout << "Entering <OptFunctionDefinitions>" << std::endl; }

    switch (current_token.type)
    {
    case KEYWORD_FUNCTION:
        FunctionDefinitions();
        break;
    }

    if (print_switch) { std::cout << "Exiting <OptFunctionDefinitions>" << std::endl; }
}

// R3
void Parser::FunctionDefinitions() {
    if (print_switch) { std::cout << "Entering <FunctionDefinitions>" << std::endl; }

    Function();
    FunctionDefinitionsPrime();

    if (print_switch) { std::cout << "Exiting <FunctionDefinitions>" << std::endl; }
}

// R3'
void Parser::FunctionDefinitionsPrime() {
    if (print_switch) { std::cout << "Entering <FunctionDefinitionsPrime>" << std::endl; }

    if (current_token.type == KEYWORD_FUNCTION) {
        Function();
        FunctionDefinitionsPrime();
    }
    else {
        // epsilon (do nothing)
    }

    if (print_switch) { std::cout << "Exiting <FunctionDefinitionsPrime>" << std::endl; }
}

// R4
void Parser::Function() {
    if (print_switch) { std::cout << "Entering <Function>" << std::endl; }

    match(KEYWORD_FUNCTION);
    match(IDENTIFIER);
    match(SEP_LEFT_PAREN);
    OptParameterList();
    match(SEP_RIGHT_PAREN);
    OptDeclarationList();
    Body();

    if (print_switch) { std::cout << "Exiting <Function>" << std::endl; }
}

// R5
void Parser::OptParameterList() {
    if (print_switch) { std::cout << "Entering <OptParameterList>" << std::endl; }

    if (current_token.type == IDENTIFIER) {
        ParameterList();
    }
    else {
        // epsilon (do nothing)
    }

    if (print_switch) { std::cout << "Exiting <OptParameterList>" << std::endl; }
}

// R6
void Parser::ParameterList() {
    if (print_switch) { std::cout << "Entering <ParameterList>" << std::endl; }

    Parameter();
    ParameterListPrime();

    if (print_switch) { std::cout << "Exiting <ParameterList>" << std::endl; }
}

// R6'
void Parser::ParameterListPrime() {
    if (print_switch) { std::cout << "Entering <ParameterListPrime>" << std::endl; }

    if (current_token.type == SEP_COMMA) {
        match(SEP_COMMA);
        Parameter();
        ParameterListPrime();
    }
    else {
        // epsilon (do nothing)
    }

    if (print_switch) { std::cout << "Exiting <ParameterListPrime>" << std::endl; }
}

// R7
void Parser::Parameter() {
    if (print_switch) { std::cout << "Entering <Parameter>" << std::endl; }

    IDs();
    Qualifier();

    if (print_switch) { std::cout << "Exiting <Parameter>" << std::endl; }
}

// R8
void Parser::Qualifier() {
    if (print_switch) { std::cout << "Entering <Qualifier>" << std::endl; }

    std::cout << current_token.type;

    if (current_token.type == KEYWORD_INTEGER) {
        match(KEYWORD_INTEGER);
    }
    else if (current_token.type == KEYWORD_BOOLEAN) {
        match(KEYWORD_BOOLEAN);
    }
    else if (current_token.type == KEYWORD_REAL) {
        match(KEYWORD_REAL);
    }
    else {
        error("Expected a type qualifier (integer, boolean, or real)");
    }

    if (print_switch) { std::cout << "Exiting <Qualifier>" << std::endl; }
}

// R9
void Parser::Body() {
    if (print_switch) { std::cout << "Entering <Body>" << std::endl; }

    match(SEP_LEFT_BRACE);
    StatementList();
    match(SEP_RIGHT_BRACE);

    if (print_switch) { std::cout << "Exiting <Body>" << std::endl; }
}

// R10
void Parser::OptDeclarationList() {
    if (print_switch) { std::cout << "Entering <OptDeclarationList>" << std::endl; }

    switch (current_token.type) {
    case KEYWORD_INTEGER:
    case KEYWORD_BOOLEAN:
    case KEYWORD_REAL:
        DeclarationList();
        break;
    default:
        // epsilon
        break;
    }

    if (print_switch) { std::cout << "Exiting <OptDeclarationList>" << std::endl; }
}

// R11
void Parser::DeclarationList() {
    if (print_switch) { std::cout << "Entering <DeclarationList>" << std::endl; }

    Declaration();
    match(SEP_SEMICOLON);
    DeclarationListPrime();

    if (print_switch) { std::cout << "Exiting <DeclarationList>" << std::endl; }
}

// R11'
void Parser::DeclarationListPrime() {
    if (print_switch) { std::cout << "Entering <DeclarationListPrime>" << std::endl; }

    switch (current_token.type) {
    case KEYWORD_INTEGER:
    case KEYWORD_BOOLEAN:
    case KEYWORD_REAL:
        Declaration();
        match(SEP_SEMICOLON);
        DeclarationListPrime();
        break;
    default:
        // epsilon
        break;
    }

    if (print_switch) { std::cout << "Exiting <DeclarationListPrime>" << std::endl; }
}

// R12
void Parser::Declaration() {
    if (print_switch) { std::cout << "Entering <Declaration>" << std::endl; }

    Qualifier();
    IDs();

    if (print_switch) { std::cout << "Exiting <Declaration>" << std::endl; }
}

// R13
void Parser::IDs() {
    if (print_switch) { std::cout << "Entering <IDs>" << std::endl; }

    match(IDENTIFIER);
    IDsPrime();

    if (print_switch) { std::cout << "Exiting <IDs>" << std::endl; }
}

// R13'
void Parser::IDsPrime() {
    if (print_switch) { std::cout << "Entering <IDsPrime>" << std::endl; }

    if (current_token.type == SEP_COMMA) {
        match(SEP_COMMA);
        IDs();
    }
    else {
        // epsilon
    }

    if (print_switch) { std::cout << "Exiting <IDsPrime>" << std::endl; }
}

// R14
void Parser::StatementList() {
    if (print_switch) { std::cout << "Entering <StatementList>" << std::endl; }

    Statement();
    StatementListPrime();

    if (print_switch) { std::cout << "Exiting <StatementList>" << std::endl; }
}

// R14'
void Parser::StatementListPrime() {
    if (print_switch) { std::cout << "Entering <StatementListPrime>" << std::endl; }

    switch (current_token.type) {
    case SEP_LEFT_BRACE:
    case IDENTIFIER:
    case KEYWORD_IF:
    case KEYWORD_RETURN:
    case KEYWORD_PUT:
    case KEYWORD_GET:
    case KEYWORD_WHILE:
        Statement();
        StatementListPrime();
        break;
    default:
        // epsilon
        break;
    }

    if (print_switch) { std::cout << "Exiting <StatementListPrime>" << std::endl; }
}

// R15
void Parser::Statement() {
    if (print_switch) { std::cout << "Entering <Statement>" << std::endl; }

    switch (current_token.type) {
    case SEP_LEFT_BRACE:
        Compound();
        break;
    case IDENTIFIER:
        Assign();
        break;
    case KEYWORD_IF:
        If();
        break;
    case KEYWORD_RETURN:
        Return();
        break;
    case KEYWORD_PUT:
        Print();
        break;
    case KEYWORD_GET:
        Scan();
        break;
    case KEYWORD_WHILE:
        While();
        break;
    default: 
        error("Expected a statement ({, id, if, return, put, get, while)");
    }


    if (print_switch) { std::cout << "Exiting <Statement>" << std::endl; }
}

// R16
void Parser::Compound() {
    if (print_switch) { std::cout << "Entering <Compound>" << std::endl; }

    match(SEP_LEFT_BRACE);
    StatementList();
    match(SEP_RIGHT_BRACE);

    if (print_switch) { std::cout << "Exiting <Compound>" << std::endl; }
}

// R17
void Parser::Assign() {
    if (print_switch) { std::cout << "Entering <Assign>" << std::endl; }

    match(IDENTIFIER);
    match(OP_ASSIGN);
    Expression();
    match(SEP_SEMICOLON);

    if (print_switch) { std::cout << "Exiting <Assign>" << std::endl; }
}

// R18
void Parser::If() {
    if (print_switch) { std::cout << "Entering <If>" << std::endl; }

    match(KEYWORD_IF);
    match(SEP_LEFT_PAREN);
    Condition();
    match(SEP_RIGHT_PAREN);
    Statement();
    IfPrime();
    match(KEYWORD_FI);

    if (print_switch) { std::cout << "Exiting <If>" << std::endl; }
}

//R18'
void Parser::IfPrime() {
    if (print_switch) { std::cout << "Entering <IfPrime>" << std::endl; }

    if (current_token.type == KEYWORD_ELSE) {
        match(KEYWORD_ELSE);
        Statement();
    }

    else {
        //epsilon do nothing
    }
    


    if (print_switch) { std::cout << "Exiting <IfPrime>" << std::endl; }
}

// R19
void Parser::Return() {
    if (print_switch) { std::cout << "Entering <Return>" << std::endl; }

    match(KEYWORD_RETURN);
    ReturnPrime();
    match(SEP_SEMICOLON);

    if (print_switch) { std::cout << "Exiting <Return>" << std::endl; }
}

//R19'
void Parser::ReturnPrime() {
    if (print_switch) { std::cout << "Entering <ReturnPrime>" << std::endl; }

    if (current_token.type == IDENTIFIER) {
        Expression();
    }
    else {
        //epsilon do nothing
    }
    
        
    if (print_switch) { std::cout << "Exiting <ReturnPrime>" << std::endl; }
}

// R20
void Parser::Print() {
    if (print_switch) { std::cout << "Entering <Print>" << std::endl; }

    match(KEYWORD_PUT);
    match(SEP_LEFT_PAREN);
    Expression();
    match(SEP_RIGHT_PAREN);
    match(SEP_SEMICOLON);

    if (print_switch) { std::cout << "Exiting <Print>" << std::endl; }
}

// R21
void Parser::Scan() {
    if (print_switch) { std::cout << "Entering <Scan>" << std::endl; }

    match(KEYWORD_GET);
    match(SEP_LEFT_PAREN);
    IDs();
    match(SEP_RIGHT_PAREN);
    match(SEP_SEMICOLON);

    if (print_switch) { std::cout << "Exiting <Scan>" << std::endl; }
}

// R22
void Parser::While() {
    if (print_switch) { std::cout << "Entering <While>" << std::endl; }

    match(KEYWORD_WHILE);
    match(SEP_LEFT_PAREN);
    Condition();
    match(SEP_RIGHT_PAREN);
    Statement();

    if (print_switch) { std::cout << "Exiting <While>" << std::endl; }
}

// R23
void Parser::Condition() {
    if (print_switch) { std::cout << "Entering <Condition>" << std::endl; }

    Expression();
    Relop();
    Expression();

    if (print_switch) { std::cout << "Exiting <Condition>" << std::endl; }
}

// R24
void Parser::Relop() {
    if (print_switch) { std::cout << "Entering <Relop>" << std::endl; }

    switch (current_token.type) {
    case OP_EQUAL:
    case OP_NOT_EQUAL:
    case OP_GREATER:
    case OP_LESS:
    case OP_LESS_EQUAL:
    case OP_GREATER_EQUAL:
        nextToken();
        break;
    default:
        error("Expected relational operator (== != > < <= >=)");
        break;
    }

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

    switch (current_token.type) {
    case OP_PLUS: {
        match(OP_PLUS);
        Term();
        ExpressionPrime();
        break;
    }
    case MINUS:
        match(MINUS);
        Term();
        ExpressionPrime();
        break;
    default: {
        //Epsilon
        break;
    }
    }


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

    switch (current_token.type) {
    case OP_MULTIPLY:
    case OP_DIVIDE:
        nextToken();
        Factor();
        TermPrime();
        break;
    default: {
        //Epsilon
        break;
    }
    }

    if (print_switch) { std::cout << "Exiting <TermPrime>" << std::endl; }
}

// R27
void Parser::Factor() {
    if (print_switch) { std::cout << "Entering <Factor>" << std::endl; }

    if (current_token.type == MINUS) {

        match(MINUS);
        Primary();

    }
    else {
        Primary();
    }


    if (print_switch) { std::cout << "Exiting <Factor>" << std::endl; }
}

// R28
void Parser::Primary() {
    if (print_switch) { std::cout << "Entering <Primary>" << std::endl; }

    switch (current_token.type) {
    case IDENTIFIER: {
        match(IDENTIFIER);
        PrimaryPrime();
        break;
    }
    case INTEGER_LITERAL: {
        match(INTEGER_LITERAL);
        break;
    }
    case REAL_LITERAL: {
        match(REAL_LITERAL);
        break;
    }
    case KEYWORD_TRUE:
    {
        match(KEYWORD_TRUE);
        break;
    }
    case KEYWORD_FALSE: {
        match(KEYWORD_FALSE);
        break;
    }
    case SEP_LEFT_PAREN: {
        match(SEP_LEFT_PAREN);
        Expression();
        match(SEP_RIGHT_PAREN);
        break;
    }
    default: {
        error("Expected primary (id, number, true/false, or '(' expression ')')");
        break;
    }
    }

    if (print_switch) { std::cout << "Exiting <Primary>" << std::endl; }
}

void Parser::PrimaryPrime() {
    if (print_switch) { std::cout << "Entering <PrimaryPrime>" << std::endl; }

    if (current_token.type == SEP_LEFT_PAREN) {
        match(SEP_LEFT_PAREN);
        IDs();
        match(SEP_RIGHT_PAREN);
    }
    else {
        //epsilon
    }

    if (print_switch) { std::cout << "Exiting <PrimaryPrime>" << std::endl; }
}
