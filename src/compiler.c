#include <stdio.h>
#include <stdlib.h>

#include "../include/common.h"
#include "../include/compiler.h"
#include "../include/scanner.h"

#ifdef DEBUG_PRINT_BYTECODE
#include "../include/debug.h"
#endif

/**
 * Keeps track of current and previous tokens
 * being scanned
*/
typedef struct
{
    Token current;
    Token previous;
    bool hadError;  // Records whether any errors occured during compilation.
    bool panicMode; // Prevents from error cascades. Ends at synchronization point.
} Parser;

typedef enum
{
    PREC_NONE,
    PREC_ASSIGNMENT,  // =
    PREC_OR,          // or
    PREC_AND,         // and
    PREC_EQUALITY,    // == !=
    PREC_COMPARISON,  // < > <= >=
    PREC_TERM,        // + -
    PREC_FACTOR,      // * /
    PREC_UNARY,       // ! -
    PREC_CALL,        // . ()
    PREC_PRIMARY
} Precedence;

typedef void (*ParseFn)();

typedef struct
{
    ParseFn prefix;
    ParseFn infix;
    Precedence precedence;
} ParseRule;

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
        fprintf(stderr, " at end");
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
    Validates successive token against control TokenType
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

/**
    Appends a single byte to the Bytecode.code.
    This function writes the given byte, which may be an opcode or
    an operand, to an instruction array.
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
static uint8_t makeConstant(Value value)
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
   Load the given value to the Constant pool and adds OP_CONSTANT
   instruction to Bytecode array.
*/
static void emitConstant(Value value)
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
#ifdef DEBUG_PRINT_BYTECODE
    if (!parser.hadError)
    {
        disassembleBytecode(currentBytecode(), "--== code ==--");
        //disassembleBytecode
    }
#endif
}

static void expression(void);
static ParseRule* getRule(TokenType type);
static void parsePrecedence(Precedence precedence);

static void binary(void)
{
    TokenType operatorType = parser.previous.type;

    ParseRule *rule = getRule(operatorType);

    // Eahc binary operator's right-hand operand precedence is one
    // level higher that its own. Using that, we call below mentioned
    // function with one level higher that current operator's level.
    // We one higher level of precedence for the right operand because the binary
    // operators are left-associative. Given a series of the same operator,
    // like:  1 + 2 + 3 + 4, we eant to parse it like: ((1 + 2) + 3) + 4.
    // Thus, when parsing the right-hand operand to the first +, we want to
    // consume the 2, but not the rest, so we use one level above +'s precedence.
    // But to parse the right-associative expressions (assignment), we would
    // call this function with the same precedence. 
    parsePrecedence((Precedence)(rule->precedence + 1));

    switch(operatorType)
    {
        // a != b equals !(a == b)
        case TOKEN_BANG_EQUAL:    emitBytes(OP_EQUAL, OP_NOT);  break;
        case TOKEN_EQUAL_EQUAL:   emitByte(OP_EQUAL);           break;
        case TOKEN_GREATER:       emitByte(OP_GREATER);         break;
        // a >= b equals !(a < b)
        case TOKEN_GREATER_EQUAL: emitBytes(OP_LESS, OP_NOT);   break;
        case TOKEN_LESS:          emitByte(OP_LESS);            break;
        // a <= b equals !(a > b)
        case TOKEN_LESS_EQUAL:    emitBytes(OP_GREATER, OP_NOT);break;
        case TOKEN_PLUS:          emitByte(OP_ADD);             break;
        case TOKEN_MINUS:         emitByte(OP_SUBTRACT);        break;
        case TOKEN_STAR:          emitByte(OP_MULTIPLY);        break;
        case TOKEN_SLASH:         emitByte(OP_DIVIDE);          break;
        default: return;    //unreachable
    }
}

static void literal(void)
{
    switch (parser.previous.type)
    {
        case TOKEN_FALSE: emitByte(OP_FALSE); break;
        case TOKEN_NIL: emitByte(OP_NIL); break;
        case TOKEN_TRUE: emitByte(OP_TRUE); break;
        default: return; // unreachable
    }
}

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

/**
 * Compiles number literals.
 * This function assumes that token for the number literal
 * has already been consumed and is stored in Parser.previous Token.
*/
static void number(void)
{
    double value = strtod(parser.previous.start, NULL);
    emitConstant(NUMBER_VAL(value));
}

/*
    Parses unary prefix expressions.
    This function assumes that target token is already resides
    in Parser.previous field.
    First this function compiles the operand and only after emits
    appropriate bytecode.
    
    It might seem a little weird to write to the Bytecode.code the
    negate instruction first and relative bytecode (e.g. OP_NEGATE) only after him.
    But observing the terms of order of execution reveals that:
    1. We evaluate the operand first which leaves its value on the stack.
    2. Then we pop that value, negate it, and push the result.

    After all, this is the compiler's job - parsing the program in the order it
    appears in the source code and rearranging it into the order that
    execution happens.
*/
static void unary(void)
{
    TokenType operatorType = parser.previous.type;

    // compile the operand
    parsePrecedence(PREC_UNARY);

    //Emit the operator instruction
    switch (operatorType)
    {
        case TOKEN_BANG: emitByte(OP_NOT);      break;
        case TOKEN_MINUS: emitByte(OP_NEGATE);  break;
        default: return; // unreachable
    }
}

/**
 * Array of function pointers.
 * 
 * The indexes in the array correspond to the TokenType enum
 * values, and the function at each index is the code to compile
 * an expression of that token type.
*/
ParseRule rules[] = {
    [TOKEN_LEFT_PAREN]    = {grouping, NULL,   PREC_NONE},
    [TOKEN_RIGHT_PAREN]   = {NULL,     NULL,   PREC_NONE},
    [TOKEN_LEFT_BRACE]    = {NULL,     NULL,   PREC_NONE}, 
    [TOKEN_RIGHT_BRACE]   = {NULL,     NULL,   PREC_NONE},
    [TOKEN_COMMA]         = {NULL,     NULL,   PREC_NONE},
    [TOKEN_DOT]           = {NULL,     NULL,   PREC_NONE},
    [TOKEN_MINUS]         = {unary,    binary, PREC_TERM},
    [TOKEN_PLUS]          = {NULL,     binary, PREC_TERM},
    [TOKEN_SEMICOLON]     = {NULL,     NULL,   PREC_NONE},
    [TOKEN_SLASH]         = {NULL,     binary, PREC_FACTOR},
    [TOKEN_STAR]          = {NULL,     binary, PREC_FACTOR},
    [TOKEN_BANG]          = {unary,    NULL,   PREC_NONE},
    [TOKEN_BANG_EQUAL]    = {NULL,     binary, PREC_EQUALITY},
    [TOKEN_EQUAL]         = {NULL,     NULL,   PREC_NONE},
    [TOKEN_EQUAL_EQUAL]   = {NULL,     binary, PREC_EQUALITY},
    [TOKEN_GREATER]       = {NULL,     binary, PREC_COMPARISON},
    [TOKEN_GREATER_EQUAL] = {NULL,     binary, PREC_COMPARISON},
    [TOKEN_LESS]          = {NULL,     binary, PREC_COMPARISON},
    [TOKEN_LESS_EQUAL]    = {NULL,     binary, PREC_COMPARISON},
    [TOKEN_IDENTIFIER]    = {NULL,     NULL,   PREC_NONE},
    [TOKEN_STRING]        = {NULL,     NULL,   PREC_NONE},
    [TOKEN_NUMBER]        = {number,   NULL,   PREC_NONE},
    [TOKEN_AND]           = {NULL,     NULL,   PREC_NONE},
    [TOKEN_CLASS]         = {NULL,     NULL,   PREC_NONE},
    [TOKEN_ELSE]          = {NULL,     NULL,   PREC_NONE},
    [TOKEN_FALSE]         = {literal,  NULL,   PREC_NONE},
    [TOKEN_FOR]           = {NULL,     NULL,   PREC_NONE},
    [TOKEN_FUN]           = {NULL,     NULL,   PREC_NONE},
    [TOKEN_IF]            = {NULL,     NULL,   PREC_NONE},
    [TOKEN_NIL]           = {literal,  NULL,   PREC_NONE},
    [TOKEN_OR]            = {NULL,     NULL,   PREC_NONE},
    [TOKEN_PRINT]         = {NULL,     NULL,   PREC_NONE},
    [TOKEN_RETURN]        = {NULL,     NULL,   PREC_NONE},
    [TOKEN_SUPER]         = {NULL,     NULL,   PREC_NONE},
    [TOKEN_THIS]          = {NULL,     NULL,   PREC_NONE},
    [TOKEN_TRUE]          = {literal,  NULL,   PREC_NONE},
    [TOKEN_VAR]           = {NULL,     NULL,   PREC_NONE},
    [TOKEN_WHILE]         = {NULL,     NULL,   PREC_NONE},
    [TOKEN_ERROR]         = {NULL,     NULL,   PREC_NONE},
    [TOKEN_EOF]           = {NULL,     NULL,   PREC_NONE},
};

/*
    Orchestrates all of the parsing functions.
    This function starts at the current token and parses any expression
    at the given precedence level or higher. It uses table of parsing
    function pointers.
*/
static void parsePrecedence(Precedence precedence)
{
    // read next token.
    advance();
    // look up corresponding ParseRule.
    ParseFn prefixRule = getRule(parser.previous.type)->prefix;
    if (prefixRule == NULL)
    {
        error("An expression expected.");
        return;
    }
    // does its stuff and compiles the rest of the prefix expression.
    prefixRule();

    // compiling the infix expressions.
    // If the next token is too low precedence, or isn’t an infix
    // operator at all, we’re done.
    while (precedence <= getRule(parser.current.type)->precedence)
    {
        advance(); // if so, consume current token.
        ParseFn infixRule = getRule(parser.previous.type)->infix;
        infixRule();
    }
}

static ParseRule* getRule(TokenType type)
{
    return &rules[type];
}

static void expression(void)
{
    parsePrecedence(PREC_ASSIGNMENT);
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
