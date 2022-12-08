#include <stdio.h>
#include <stdlib.h>

#include "../include/common.h"
#include "../include/compiler.h"
#include "../include/scanner.h"

//================================================================================
//              FRONT END: PARSER AND CORRESPONDING FUNCTIONS
//================================================================================

typedef struct
{
    Token current;
    Token previous;
    bool hadError;  // Records whether any errors occured during compilation.
    bool panicMode; // Prevents from error cascades. Ends at synchronization point.
} Parser;

Parser parser;
Bytecode *compilingBytecode;

static Bytecode* currentBytecode(void)
{
    return compilingBytecode;
}

static void errorAt(Token *token, const char *message)
{
    // is set: suppress any other errors that get detected.
    if (parser.panicMode == true)
        return;
    
    parser.panicMode = true;

    fprintf(stderr, "[line %d] Error", token->line);

    if (token->type == TOKEN_EOF)
    {
        fpintf(stderr, " at end");
    }else if (token->type == TOKEN_ERROR)
    {
        // do nothing
    }else
    {
        fprintf(stderr, "  '%.*s'", token->length, token->start);
    }

    fprintf(stderr, ": %s\n ", message);
    parser.hadError= true;
}

/*
    Reports an error at the location of the token just consumed.
*/
static void error(const char *message)
{
    errorAt(&parser.previous, message);
}

/*
    Informs user about a lexical error the Scanner encountered with.
*/
static void errorAtCurrent(const char *message)
{
    errorAt(&parser.current, message);
}

/*
    Steps forward through the token stream.
    It asks the scanner for the next token and stores
    it for later use.

    Note: the Scanner doesn't report lexical errors. Instead,
    it creates special 'error token' and leaves it up to the Parser
    to report then. This takes action in this function.
*/
static void advance(void)
{
    parser.previous = parser.current;

    for (;;)
    {
        parser.current = scanToken();
        // Break loop if current token is recognizable one
        if (parser.current.type != TOKEN_ERROR)
            break;
        
        // Otherwise - Scanner encountered error-token
        errorAtCurrent(parser.current.start);
    }
}

/*
    Validates successive token againt control TokenType
    and advances parser.current.
*/
static void consume(TokenType type, const char *message)
{
    if (parser.current.type == type)
    {
        advance();
        return;
    }

    errorAtCurrent(message);
}

//================================================================================
//              BACK END: COMPILER AND CORRESPONDING FUNCTIONS
//================================================================================

/**
    Appends a single byte to the Bytecode.code.
    This function writes the given byte, which may be an opcode or
    an operand to an instruction.
    @param uint8_t
*/
static void emitByte(uint8_t byte)
{
    appendBytecode(currentBytecode(), byte, parser.previous.line);
}

/*
    Appends two bytes at once
*/
static void emitBytes(uint8_t byte1, uint8_t byte2)
{
    emitByte(byte1);
    emitByte(byte2);
}

/*
    Appends OP_RETURN to the tail of compilingBytecode entry.
*/
static void emitReturn(void)
{
    emitByte(OP_RETURN);
}

/**
    Appends constant to Constant pool.
    @returns uint8_t index in Constant pool.
*/
static uint8_t makeConstant(Double value)
{
    int index = addConstant(currentBytecode(), value);
    if (index > UINT8_MAX)
    {
        error("Too many constants in one Constant pool.");
        return 0;
    }

    return (uint8_t)index;
}

/*
    Wrapper function which at first adds the value to the Constant pool,
    then emits OP_CONSTANT and index within ConstantPool in Bytecode array. 
*/
static void emitConstant(Double value)
{
    emitBytes(OP_CONSTANT, makeConstant(value));
}
/*
    Wrapper function.
    Calls the function which appends OP_RETURN opcode at the end of compile()
    execution.
*/
static void endCompiler(void)
{
    emitReturn();
}


//================================================================================
//              PARSING PREFIX EXPRESSIONS
//================================================================================

/*
    Parentheses expression.
    This function assumes that starintg parethesis have already been consumed.
    Has no runtime semantics on its own and therefore doesn't emit any bytecode.
    The inner call to expression() takes care of generating bytecode for the expression
    inside the parentheses.
*/
static void grouping(void)
{
    expression();
    consume(TOKEN_RIGHT_PAREN, "')' token expected after expression");
}

static void number(void)
{
    double value = strtod(parser.previous.start, NULL);
    emitConstant(value);
}

static void unary(void)
{
    TokenType operatorType = parser.previous.type;
    expression();   // conpile the operand

    //Emit the operator instruction
    switch (operatorType)
    {
        case TOKEN_MINUS: emitByte(OP_NEGATE); break;
        default: return; // unreachable
    }
}

static void expression(void)
{

}

bool compile(const char *source, Bytecode *bytecode)
{
    initScanner(source);
    compilingBytecode = bytecode;
    parser.hadError = false;
    parser.panicMode = false;

    advance();          // turn Scanner on.
    expression();       // Parse expression.

    // The last token must be of type EOF
    consume(TOKEN_EOF, "End of expression expected.");
    endCompiler();

    return !parser.hadError;
}