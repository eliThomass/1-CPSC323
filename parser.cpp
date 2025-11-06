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
    std::cout << "Token: " << std::setw(8) << current_token.type << "Lexeme: " << current_token.value << std::endl;
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
    if (print_switch) { std::cout << "   <Rat25F> -> <OptFunctionDefs> # <OptDeclarationList> <StatementList> #" << std::endl; }

    // Based on the compiler doc, we will call our other functions
    // for all the NONTERMINALS in the production.
    // For all the terminals in the production, we will use the match function.
    // We will repeat this for every production rule.
    OptFunctionDefinitions();
    match(SEP_HASH);
    OptDeclarationList();
    StatementList();
    match(SEP_HASH);
    
}

// R2
void Parser::OptFunctionDefinitions() {
    if (print_switch) { std::cout << "   <OptFunctionDefinitions> -> <FunctionDefinitions> | epsilon" << std::endl; }

    switch (current_token.type)
    {
    case KEYWORD_FUNCTION:
        FunctionDefinitions();
        break;
    }

}

// R3
void Parser::FunctionDefinitions() {
    if (print_switch) { std::cout << "   <FunctionDefinitions> -> <Function> <FunctionDefinitionsPrime>" << std::endl; }

    Function();
    FunctionDefinitionsPrime();

}

// R3'
void Parser::FunctionDefinitionsPrime() {
    if (print_switch) { std::cout << "   <FunctionDefinitionsPrime> -> <Function> <FunctionDefinitionsPrime> | epsilon" << std::endl; }

    if (current_token.type == KEYWORD_FUNCTION) {
        Function();
        FunctionDefinitionsPrime();
    }
    else {
        // epsilon (do nothing)
    }
}

// R4
void Parser::Function() {
    if (print_switch) { std::cout << "   <Function> -> function <Identifier> ( <OptParameterList> ) <OptDeclarationList> <Body>" << std::endl; }

    match(KEYWORD_FUNCTION);
    match(IDENTIFIER);
    match(SEP_LEFT_PAREN);
    OptParameterList();
    match(SEP_RIGHT_PAREN);
    OptDeclarationList();
    Body();

}

// R5
void Parser::OptParameterList() {
    if (print_switch) { std::cout << "   <OptParameterList> -> <ParameterList> | epsilon" << std::endl; }

    if (current_token.type == IDENTIFIER) {
        ParameterList();
    }
    else {
        // epsilon (do nothing)
    }
}

// R6
void Parser::ParameterList() {
    if (print_switch) { std::cout << "   <ParameterList> -> <Parameter> <ParameterListPrime>" << std::endl; }

    Parameter();
    ParameterListPrime();
}

// R6'
void Parser::ParameterListPrime() {
    if (print_switch) { std::cout << "   <ParameterListPrime> -> , <Parameter> <ParameterListPrime> | epsilon" << std::endl; }

    if (current_token.type == SEP_COMMA) {
        match(SEP_COMMA);
        Parameter();
        ParameterListPrime();
    }
    else {
        // epsilon (do nothing)
    }
}

// R7
void Parser::Parameter() {
    if (print_switch) { std::cout << "   <Parameter> -> <IDs> <Qualifier>" << std::endl; }

    IDs();
    Qualifier();
}

// R8
void Parser::Qualifier() {
    if (print_switch) { std::cout << "   <Qualifier> -> integer | boolean | real" << std::endl; }

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
}

// R9
void Parser::Body() {
    if (print_switch) { std::cout << "   <Body> -> { <StatementList> }" << std::endl; }

    match(SEP_LEFT_BRACE);
    StatementList();
    match(SEP_RIGHT_BRACE);
}

// R10
void Parser::OptDeclarationList() {
    if (print_switch) { std::cout << "   <OptDeclarationList> -> <DeclarationList> | epsilon" << std::endl; }

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
}

// R11
void Parser::DeclarationList() {
    if (print_switch) { std::cout << "   <DeclarationList> -> <Declaration> ; <DeclarationListPrime>" << std::endl; }

    Declaration();
    match(SEP_SEMICOLON);
    DeclarationListPrime();
}

// R11'
void Parser::DeclarationListPrime() {
    if (print_switch) { std::cout << "   <DeclarationListPrime> -> <Declaration> ; <DeclarationListPrime> | epsilon" << std::endl; }

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
}

// R12
void Parser::Declaration() {
    if (print_switch) { std::cout << "   <Declaration> -> <Qualifier> <IDs>" << std::endl; }

    Qualifier();
    IDs();
}

// R13
void Parser::IDs() {
    if (print_switch) { std::cout << "   <IDs> -> <Identifier> <IDsPrime>" << std::endl; }

    match(IDENTIFIER);
    IDsPrime();
}

// R13'
void Parser::IDsPrime() {
    if (print_switch) { std::cout << "   <IDsPrime> -> , <IDs> | epsilon" << std::endl; }

    if (current_token.type == SEP_COMMA) {
        match(SEP_COMMA);
        IDs();
    }
    else {
        // epsilon
    }
}

// R14
void Parser::StatementList() {
    if (print_switch) { std::cout << "   <StatementList> -> <Statement> <StatementListPrime>" << std::endl; }

    Statement();
    StatementListPrime();
}

// R14'
void Parser::StatementListPrime() {
    if (print_switch) { std::cout << "   <StatementListPrime> -> <Statement> <StatementListPrime> | epsilon" << std::endl; }

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
}

// R15
void Parser::Statement() {
    if (print_switch) { std::cout << "   <Statement> -> <Compound> | <Assign> | <If> | <Return> | <Print> | <Scan> | <While>" << std::endl; }

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
}

// R16
void Parser::Compound() {
    if (print_switch) { std::cout << "<Compound> -> { <StatementList> }" << std::endl; }

    match(SEP_LEFT_BRACE);
    StatementList();
    match(SEP_RIGHT_BRACE);
}

// R17
void Parser::Assign() {
    if (print_switch) { std::cout << "   <Assign> -> <Identifier> = <Expression> ;" << std::endl; }

    match(IDENTIFIER);
    match(OP_ASSIGN);
    Expression();
    match(SEP_SEMICOLON);
}

// R18
void Parser::If() {
    if (print_switch) { std::cout << "   <If> -> if ( <Condition> ) <Statement> <ElseOpt> fi" << std::endl; }

    match(KEYWORD_IF);
    match(SEP_LEFT_PAREN);
    Condition();
    match(SEP_RIGHT_PAREN);
    Statement();
    IfPrime(); // Corresponds to <ElseOpt>
    match(KEYWORD_FI);
}

//R18'
void Parser::IfPrime() {
    if (print_switch) { std::cout << "   <ElseOpt> -> else <Statement> | epsilon" << std::endl; }

    if (current_token.type == KEYWORD_ELSE) {
        match(KEYWORD_ELSE);
        Statement();
    }

    else {
        //epsilon do nothing
    }
}

// R19
void Parser::Return() {
    if (print_switch) { std::cout << "   <Return> -> return <ReturnOpt> ;" << std::endl; }

    match(KEYWORD_RETURN);
    ReturnPrime(); // Corresponds to <ReturnOpt>
    match(SEP_SEMICOLON);
}

//R19'
void Parser::ReturnPrime() {
    if (print_switch) { std::cout << "   <ReturnOpt> -> <Expression> | epsilon" << std::endl; }

    if (current_token.type == IDENTIFIER) { // This check is likely too weak based on the grammar, but matches your original code
        Expression();
    }
    else {
        //epsilon do nothing
    }
}

// R20
void Parser::Print() {
    if (print_switch) { std::cout << "   <Print> -> put ( <Expression> ) ;" << std::endl; }

    match(KEYWORD_PUT);
    match(SEP_LEFT_PAREN);
    Expression();
    match(SEP_RIGHT_PAREN);
    match(SEP_SEMICOLON);
}

// R21
void Parser::Scan() {
    if (print_switch) { std::cout << "   <Scan> -> get ( <IDs> ) ;" << std::endl; }

    match(KEYWORD_GET);
    match(SEP_LEFT_PAREN);
    IDs();
    match(SEP_RIGHT_PAREN);
    match(SEP_SEMICOLON);
}

// R22
void Parser::While() {
    if (print_switch) { std::cout << "   <While> -> while ( <Condition> ) <Statement>" << std::endl; }

    match(KEYWORD_WHILE);
    match(SEP_LEFT_PAREN);
    Condition();
    match(SEP_RIGHT_PAREN);
    Statement();
}

// R23
void Parser::Condition() {
    if (print_switch) { std::cout << "   <Condition> -> <Expression> <Relop> <Expression>" << std::endl; }

    Expression();
    Relop();
    Expression();
}

// R24
void Parser::Relop() {
    if (print_switch) { std::cout << "   <Relop> -> == | != | > | < | <= | =>" << std::endl; }

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
}

// R25
void Parser::Expression() {
    if (print_switch) { std::cout << "   <Expression> -> <Term> <ExpressionPrime>" << std::endl; }

    Term();
    ExpressionPrime();
}

// R25'
void Parser::ExpressionPrime() {
    if (print_switch) { std::cout << "   <ExpressionPrime> -> + <Term> <ExpressionPrime> | - <Term> <ExpressionPrime> | epsilon" << std::endl; }

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
}

// R26
void Parser::Term() {
    if (print_switch) { std::cout << "   <Term> -> <Factor> <TermPrime>" << std::endl; }

    Factor();
    TermPrime();
}

// R26'
void Parser::TermPrime() {
    if (print_switch) { std::cout << "   <TermPrime> -> * <Factor> <TermPrime> | / <Factor> <TermPrime> | epsilon" << std::endl; }

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
}

// R27
void Parser::Factor() {
    if (print_switch) { std::cout << "   <Factor> -> - <Primary> | <Primary>" << std::endl; }

    if (current_token.type == MINUS) {

        match(MINUS);
        Primary();

    }
    else {
        Primary();
    }
}

// R28
void Parser::Primary() {
    if (print_switch) { std::cout << "   <Primary> -> <Identifier> <PrimaryPrime> | <Integer> | <Real> | true | false | ( <Expression> )" << std::endl; }

    switch (current_token.type) {
    case IDENTIFIER: {
        match(IDENTIFIER);
        PrimaryPrime(); // Corresponds to <CallOpt>
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
}

void Parser::PrimaryPrime() {
    if (print_switch) { std::cout << "   <PrimaryPrime> -> ( <IDs> ) | epsilon" << std::endl; }

    if (current_token.type == SEP_LEFT_PAREN) {
        match(SEP_LEFT_PAREN);
        IDs();
        match(SEP_RIGHT_PAREN);
    }
    else {
        //epsilon
    }
}