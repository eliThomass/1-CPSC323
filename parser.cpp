#include "parser.h"
#include <iostream>
#include <stdexcept>
#include <iomanip>

Parser::Parser(Lexer& lexer, std::ostream& output_stream, bool print_switch) : lexer(lexer), out_stream(output_stream), print_switch(print_switch)
{
    nextToken();
}

void Parser::error(const std::string& error) {
    // Errors should still go to std::cerr (console) and the output stream
    std::cerr << "Parse Failed: " << error << std::endl;
    std::cerr << "  ...at token: " << current_token.value
        << " (Line " << current_token.line << ")" << std::endl;

    out_stream << "Parse Failed: " << error << std::endl;
    out_stream << "  ...at token: " << current_token.value
        << " (Line " << current_token.line << ")" << std::endl;

    throw std::runtime_error(error);
}


// The main parsing function, we use it to start the recursion
void Parser::parse()
{
    // If any recursion inside Rat25 throws an error, we know we have a syntax error.
    try {
        Rat25F();
        out_stream << "Parse Successful" << std::endl;

        ACG.print_assembly_code();
        st.printTable();

    }
    catch (const std::runtime_error& e) {

    }
}

void Parser::nextToken()
{
    current_token = lexer.getNextToken();
    if (print_switch) {
        out_stream << "Token: " << std::left << std::setw(15) << lexer.getCategoryName(current_token.type) << "Lexeme: " << current_token.value << std::endl;
    }
}

// Check if our current token matches what we should have
void Parser::match(TokenType expected)
{
    if (current_token.type == expected) {
        nextToken();
    }
    else {
        std::string err_msg = "Token type did not match expected " + lexer.getCategoryName(expected) +
            ", got " + lexer.getCategoryName(current_token.type);
        error(err_msg);
    }
}

// R1
void Parser::Rat25F() {
    if (print_switch) { out_stream << "   <Rat25F> -> <OptFunctionDefs> # <OptDeclarationList> <StatementList> #" << std::endl; }

    // Based on the compiler doc, we will call our other functions
    // for all the NONTERMINALS in the production.
    // For all the terminals in the production, we will use the match function.
    // We will repeat this for every production rule.


    //OptFunctionDefinitions(); // FOR ASSIGNMENT 3, there are no function definitions
    match(SEP_HASH);
    OptDeclarationList();
    StatementList();
    match(SEP_HASH);

}

// R2
void Parser::OptFunctionDefinitions() {
    if (print_switch) { out_stream << "   <OptFunctionDefinitions> -> <FunctionDefinitions> | epsilon" << std::endl; }

    switch (current_token.type)
    {
    case KEYWORD_FUNCTION:
        FunctionDefinitions();
        break;
    }

}

// R3
void Parser::FunctionDefinitions() {
    if (print_switch) { out_stream << "   <FunctionDefinitions> -> <Function> <FunctionDefinitionsPrime>" << std::endl; }

    Function();
    FunctionDefinitionsPrime();

}

// R3'
void Parser::FunctionDefinitionsPrime() {
    if (print_switch) { out_stream << "   <FunctionDefinitionsPrime> -> <Function> <FunctionDefinitionsPrime> | epsilon" << std::endl; }

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
    if (print_switch) { out_stream << "   <Function> -> function <Identifier> ( <OptParameterList> ) <OptDeclarationList> <Body>" << std::endl; }

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
    if (print_switch) { out_stream << "   <OptParameterList> -> <ParameterList> | epsilon" << std::endl; }

    if (current_token.type == IDENTIFIER) {
        ParameterList();
    }
    else {
        // epsilon (do nothing)
    }
}

// R6
void Parser::ParameterList() {
    if (print_switch) { out_stream << "   <ParameterList> -> <Parameter> <ParameterListPrime>" << std::endl; }

    Parameter();
    ParameterListPrime();
}

// R6'
void Parser::ParameterListPrime() {
    if (print_switch) { out_stream << "   <ParameterListPrime> -> , <Parameter> <ParameterListPrime> | epsilon" << std::endl; }

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
    if (print_switch) { out_stream << "   <Parameter> -> <IDs> <Qualifier>" << std::endl; }

    IDs("");
    Qualifier();
}

// R8
void Parser::Qualifier() {
    if (print_switch) { out_stream << "   <Qualifier> -> integer | boolean | real" << std::endl; }

    if (current_token.type == KEYWORD_INTEGER) {
        match(KEYWORD_INTEGER);
    }
    else if (current_token.type == KEYWORD_BOOLEAN) {
        match(KEYWORD_BOOLEAN);
    }/*
    else if (current_token.type == KEYWORD_REAL) {
        match(KEYWORD_REAL);
    }*/ // Assignment 3 has NO real qualifier
    else {
        error("Expected a type qualifier (integer or boolean)");
    }
}

// R9
void Parser::Body() {
    if (print_switch) { out_stream << "   <Body> -> { <StatementList> }" << std::endl; }

    match(SEP_LEFT_BRACE);
    StatementList();
    match(SEP_RIGHT_BRACE);
}

// R10
void Parser::OptDeclarationList() {
    if (print_switch) { out_stream << "   <OptDeclarationList> -> <DeclarationList> | epsilon" << std::endl; }

    switch (current_token.type) {
    case KEYWORD_INTEGER:
    case KEYWORD_BOOLEAN:
        // case KEYWORD_REAL: // removed for assignment 3
        DeclarationList();
        break;
    default:
        // epsilon
        break;
    }
}

// R11
void Parser::DeclarationList() {
    if (print_switch) { out_stream << "   <DeclarationList> -> <Declaration> ; <DeclarationListPrime>" << std::endl; }

    Declaration();
    match(SEP_SEMICOLON);
    DeclarationListPrime();
}

// R11'
void Parser::DeclarationListPrime() {
    if (print_switch) { out_stream << "   <DeclarationListPrime> -> <Declaration> ; <DeclarationListPrime> | epsilon" << std::endl; }

    switch (current_token.type) {
    case KEYWORD_INTEGER:
    case KEYWORD_BOOLEAN:
        // case KEYWORD_REAL: // assignment 3
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
    if (print_switch) { out_stream << "   <Declaration> -> <Qualifier> <IDs>" << std::endl; }

    std::string declared_type = "";
    if (current_token.type == KEYWORD_INTEGER) declared_type = "integer";
    else if (current_token.type == KEYWORD_BOOLEAN) declared_type = "boolean";

    Qualifier();
    IDs(declared_type);
}

// R13
void Parser::IDs(std::string declared_type) {
    if (print_switch) { out_stream << "   <IDs> -> <Identifier> <IDsPrime>" << std::endl; }

    std::string var_name = current_token.value;
    // If type is not empty, we are declaring the identifier (with its type)
    if (!declared_type.empty()) {
        if (!st.symbolPush(var_name, declared_type)) {
            error("Variable '" + var_name + "' already declared");
        };
    }
    // If type IS empty, we must be using the identifier after it's been declared
    else {
        int addr = st.getAddress(var_name);
        if (addr == -1) error("Variable '" + var_name + "' used but not declared.");
    }
    match(IDENTIFIER);
    IDsPrime(declared_type);
}

// R13'
void Parser::IDsPrime(std::string declared_type) {
    if (print_switch) { out_stream << "   <IDsPrime> -> , <IDs> | epsilon" << std::endl; }

    if (current_token.type == SEP_COMMA) {
        match(SEP_COMMA);
        IDs(declared_type);
    }
    else {
        // epsilon
    }
}

// R14
void Parser::StatementList() {
    if (print_switch) { out_stream << "   <StatementList> -> <Statement> <StatementListPrime>" << std::endl; }

    Statement();
    StatementListPrime();
}

// R14'
void Parser::StatementListPrime() {
    if (print_switch) { out_stream << "   <StatementListPrime> -> <Statement> <StatementListPrime> | epsilon" << std::endl; }

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
    if (print_switch) { out_stream << "   <Statement> -> <Compound> | <Assign> | <If> | <Return> | <Print> | <Scan> | <While>" << std::endl; }

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
    if (print_switch) { out_stream << "<Compound> -> { <StatementList> }" << std::endl; }

    match(SEP_LEFT_BRACE);
    StatementList();
    match(SEP_RIGHT_BRACE);
}

// R17
void Parser::Assign() {
    if (print_switch) {
        out_stream << "   <Assign> -> <Identifier> = <Expression> ;" << std::endl;
    }

    if (current_token.type != IDENTIFIER) {
        error("id expected at start of assignment");
    }

    std::string var_name = current_token.value;


    int addr = st.getAddress(var_name);

    if (addr == -1) {
        error("Variable '" + var_name + "' used in assignment before declaration");
    }

    match(IDENTIFIER);
    if (current_token.type != OP_ASSIGN) {
        error("= expected in assignment");
    }

    match(OP_ASSIGN);
    Expression();
    ACG.gen_instr("POPM", addr);
    match(SEP_SEMICOLON);
}

// R18
void Parser::If() {
    if (print_switch) {
        out_stream << "   <If> -> if ( <Condition> ) <Statement> <ElseOpt> fi" << std::endl;
    }

    match(KEYWORD_IF);
    match(SEP_LEFT_PAREN);

    Condition();        // leaves a JUMPZ ? and pushes its address

    match(SEP_RIGHT_PAREN);

    // Save address of JUMPZ
    int jmpAddr = jumpStack.back();
    jumpStack.pop_back();

    // THEN block
    Statement();

    // If you have ELSE, handle it
    if (current_token.type == KEYWORD_ELSE) {
        // Jump over the else body
        int jumpToEnd = ACG.get_current_address();
        ACG.gen_instr("JUMP", 0);

        // Backpatch JUMPZ to here (start of else)
        int elseStart = ACG.get_current_address();
        ACG.backpatch(jmpAddr, elseStart);

        match(KEYWORD_ELSE);
        Statement();

        // Backpatch the JUMP to END
        int endAddr = ACG.get_current_address();
        ACG.backpatch(jumpToEnd, endAddr);
    }
    else {
        // No else: patch JUMPZ to the instruction after THEN block
        int afterIf = ACG.get_current_address();
        ACG.backpatch(jmpAddr, afterIf);
    }

    match(KEYWORD_FI);
}


//R18'
void Parser::IfPrime() {
    if (print_switch) { out_stream << "   <ElseOpt> -> else <Statement> | epsilon" << std::endl; }

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
    if (print_switch) { out_stream << "   <Return> -> return <ReturnOpt> ;" << std::endl; }

    match(KEYWORD_RETURN);
    ReturnPrime();
    match(SEP_SEMICOLON);
}

//R19'
void Parser::ReturnPrime() {
    if (print_switch) { out_stream << "   <ReturnOpt> -> <Expression> | epsilon" << std::endl; }

    switch (current_token.type) {
    case IDENTIFIER:
    case INTEGER_LITERAL:
        // case REAL_LITERAL:
    case KEYWORD_TRUE:
    case KEYWORD_FALSE:
    case SEP_LEFT_PAREN:
    case MINUS:
        Expression();
        break;
    default:
        // epsilon
        break;
    }
}

// R20
void Parser::Print() {
    if (print_switch) { out_stream << "   <Print> -> put ( <Expression> ) ;" << std::endl; }

    match(KEYWORD_PUT);
    match(SEP_LEFT_PAREN);
    Expression();
    match(SEP_RIGHT_PAREN);
    ACG.gen_instr("STDOUT");
    match(SEP_SEMICOLON);
}

// R21
void Parser::Scan() {
    if (print_switch) { out_stream << "   <Scan> -> get ( <IDs> ) ;" << std::endl; }

    match(KEYWORD_GET);
    match(SEP_LEFT_PAREN);
    if (current_token.type != IDENTIFIER)
        error("Identifier expected in get");
    while (true) {
        std::string name = current_token.value;
        int addr = st.getAddress(name);
        if (addr == -1) error("Variable '" + name + "' not declared");

        match(IDENTIFIER);

        // STDIN
        ACG.gen_instr("STDIN");
        // POPM addr
        ACG.gen_instr("POPM", addr);

        if (current_token.type == SEP_COMMA) {
            match(SEP_COMMA);
        }
        else break;
    }
    match(SEP_RIGHT_PAREN);
    match(SEP_SEMICOLON);
}

// R22
void Parser::While() {
    if (print_switch) { out_stream << "   <While> -> while ( <Condition> ) <Statement>" << std::endl; }

    match(KEYWORD_WHILE);
    int loopStart = ACG.get_current_address();
    ACG.gen_instr("LABEL");
    match(SEP_LEFT_PAREN);
    Condition();
    match(SEP_RIGHT_PAREN);
    Statement();

    ACG.gen_instr("JUMP", loopStart);
    int jmpAddr = jumpStack.back();
    jumpStack.pop_back();
    int afterLoop = ACG.get_current_address();
    ACG.gen_instr("LABEL");
    ACG.backpatch(jmpAddr, afterLoop);
}

// R23
void Parser::Condition() {
    if (print_switch) { out_stream << "   <Condition> -> <Expression> <Relop> <Expression>" << std::endl; }

    Expression();
    TokenType rel = current_token.type;
    Relop();
    Expression();

    switch (rel) {
    case OP_LESS:          ACG.gen_instr("LES"); break;
    case OP_GREATER:       ACG.gen_instr("GRT"); break;
    case OP_EQUAL:         ACG.gen_instr("EQU"); break;
    case OP_NOT_EQUAL:     ACG.gen_instr("NEQ"); break;
    case OP_LESS_EQUAL:    ACG.gen_instr("LEQ"); break;
    case OP_GREATER_EQUAL: ACG.gen_instr("GEQ"); break;
    default:
        error("Invalid relational operator in condition");
    }

    int jmpAddr = ACG.get_current_address();
    ACG.gen_instr("JUMPZ", 0);

    jumpStack.push_back(jmpAddr);
}

// R24
void Parser::Relop() {
    if (print_switch) { out_stream << "   <Relop> -> == | != | > | < | <= | =>" << std::endl; }

    switch (current_token.type) {
    case OP_EQUAL:
        match(OP_EQUAL);
        break;
    case OP_NOT_EQUAL:
        match(OP_NOT_EQUAL);
        break;
    case OP_GREATER:
        match(OP_GREATER);
        break;
    case OP_LESS:
        match(OP_LESS);
        break;
    case OP_LESS_EQUAL:
        match(OP_LESS_EQUAL);
        break;
    case OP_GREATER_EQUAL:
        match(OP_GREATER_EQUAL);
        break;
    default:
        error("Expected relational operator (== != > < <= >=)");
        break;
    }
}

// R25
void Parser::Expression() {
    if (print_switch) { out_stream << "   <Expression> -> <Term> <ExpressionPrime>" << std::endl; }

    Term();
    ExpressionPrime();
}

// R25'
void Parser::ExpressionPrime() {
    if (print_switch) { out_stream << "   <ExpressionPrime> -> + <Term> <ExpressionPrime> | - <Term> <ExpressionPrime> | epsilon" << std::endl; }

    switch (current_token.type) {
    case OP_PLUS: {
        match(OP_PLUS);
        Term();
        ACG.gen_instr("ADD");
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
    if (print_switch) { out_stream << "   <Term> -> <Factor> <TermPrime>" << std::endl; }

    Factor();
    TermPrime();
}

// R26'
void Parser::TermPrime() {
    if (print_switch) { out_stream << "   <TermPrime> -> * <Factor> <TermPrime> | / <Factor> <TermPrime> | epsilon" << std::endl; }

    switch (current_token.type) {
    case OP_MULTIPLY:
        match(OP_MULTIPLY);
        Factor();
        ACG.gen_instr("MUL");
        TermPrime();
        break;
    case OP_DIVIDE:
        match(OP_DIVIDE);
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
    if (print_switch) { out_stream << "   <Factor> -> - <Primary> | <Primary>" << std::endl; }

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
    if (print_switch) { out_stream << "   <Primary> -> <Identifier> <PrimaryPrime> | <Integer> | <Real> | true | false | ( <Expression> )" << std::endl; }

    switch (current_token.type) {
    case IDENTIFIER: {
        std::string name = current_token.value;
        int addr = st.getAddress(name);
        if (addr == -1) {
            error("Variable '" + name + "' used before declaration");
        }

        ACG.gen_instr("PUSHM", addr);

        match(IDENTIFIER);
        PrimaryPrime();
        break;
    }
    case INTEGER_LITERAL: {
        int value = std::stoi(current_token.value);
        ACG.gen_instr("PUSHI", value);

        match(INTEGER_LITERAL);
        break;
    }
                        /*case REAL_LITERAL: {
                            match(REAL_LITERAL);
                            break;
                        } */ // removed for assignment 3
    case KEYWORD_TRUE:
    {
        ACG.gen_instr("PUSHI", 1);
        match(KEYWORD_TRUE);
        break;
    }
    case KEYWORD_FALSE: {
        ACG.gen_instr("PUSHI", 0);
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
    if (print_switch) { out_stream << "   <PrimaryPrime> -> ( <IDs> ) | epsilon" << std::endl; }

    if (current_token.type == SEP_LEFT_PAREN) {
        match(SEP_LEFT_PAREN);
        IDs("");
        match(SEP_RIGHT_PAREN);
    }
    else {
        //epsilon
    }
}
