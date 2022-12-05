#include "../includes/compiler.h"

/**
 * @brief A compiler has roughlt two jobs.
 * It prses the user's source code to understand what it means. Then it
 * takes that knowledge and outputs low-level instructions that produce the same
 * ssemantics.
 */
typedef struct
{
    Token current;
    Token previous;
    bool hadError;  // whether any errors occurred during compilation
    bool panicMode; // intended to avoid error cascades.
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
}Precedence;

/** function type that takes no args and returns nothing. */
typedef void (*ParseFn)();

/**
 * @brief Represents a single row in the parser table
 * 
 */
typedef struct
{
    ParseFn prefix;
    ParseFn infix;
    Precedence precedence;
}ParseRule;

/** @brief The Parser's Global Variable */
Parser parser;

/**
 * @brief The chunk used to write in an incoming instructions.
 * It will be passed into compile() function.
 */
Chunk* compilingChunk;

// ====================================================================
//                  Error handling
// ====================================================================

/**
 * @brief Prints error report.
 * 
 * @param token 
 * @param message 
 */
static void errorAt(Token* token, const char* message)
{
    /*
        if an error once occurred we just go ahead and keep compiling as normal 
        as if the error never occurred. While it's set, just suppress any other
        errors that get detected.
        Panic mode ends when the parser reaches a synchronization point.
    */
    if (parser.panicMode)
        return;
    
    // set it when an error occures.
    parser.panicMode = true;

    // the line at which the error occurred.
    fprintf(stderr, "[line: %d] Error:", token->line);

    if (TOKEN_EOF == token->type)
    {
        fprintf(stderr, " at end");
    }else if (TOKEN_ERROR == token->type)
    {
        // do nothing
    }else
    {   // print lexeme if it's human-readable
        fprintf(stderr, " at '%.*s'", token->length, token->start);
    }

    // print error message itself.
    fprintf(stderr, ": %s\n", message);

    parser.hadError = true;
}

/**
 * @brief Informs the user about the error occurred at the
 * token we just consumed.
 * @param message 
 */
static void error(const char* message)
{
    errorAt(&parser.previous, message);
}

/**
 * @brief Informs the user about the error occurred at the
 * current token.
 * @param message 
 */
static void errorAtCurrent(const char* message)
{
    errorAt(&parser.current, message);
}

// ====================================================================
//                  Helper functions
// ====================================================================

/**
 * @brief Passes compilingChunk to compile() function.
 * 
 * @return Chunk* 
 */
static Chunk* currentChunk()
{
    return compilingChunk;
}

/**
 * @brief Steps forward through the token stream.
 * It asks the <code>Scanner</code> for the next token
 * and stores it ins <code>Parser.current</code>.
 */
static void advance()
{
    // Stash the Token current in Token presious so that we can
    // get at the lexeme after we match a token.
    parser.previous = parser.current;

    // step forward through the token stream
    for(;;)
    {
        parser.current = scanToken();
        if (TOKEN_ERROR != parser.current.type)
            break;
        
        errorAtCurrent(parser.current.start);
    }
}

/**
 * @brief Similar to advance() function with the difference in that
 * it reads the next token, validates it against expected value and calls
 * advance() function.
 * This function is the foundation of most syntax errors in the compiler.
 * 
 * @param type TokenType for comparison purposes.
 * @param message error message.
 */
static void consume(const TokenType type, const char* message)
{
    if (type == parser.current.type)
    {
        advance();
        return;
    }

    errorAtCurrent(message);
}

/**
 * @brief Appends a single byte to the Chunk.
 * This function sends in the previous token's line info so that
 * runtime errors are associated with that line.
 * 
 * @param byte opcode or an operand to an instruction
 */
static void emitByte(uint8_t byte)
{
    writeChunk(currentChunk(), byte, parser.previous.line);
}

/**
 * @brief Helper function for the cases where we need to write an opcode
 * followed by a one-byte operand.
 * 
 * @param byte1 
 * @param byte2 
 */
static void emitBytes(uint8_t byte1, uint8_t byte2)
{
    emitByte(byte1);
    emitByte(byte2);
}

/**
 * @brief The intermediate implementation of VM deals only with
 * expressions which shall be printed for debugging purposes.
 * To do so we encourage compiler to insert the OP_RETURN opcode
 * to the end of the chunk which will temporarily do that stuff for us.
 */
static void emitReturn()
{
    emitByte(OP_RETURN);
}

/** @brief Inserts OP_RETURN to the end of the chunk of instructions. */
static void endCompiler()
{
    emitReturn();
#ifdef DEBUG_PRINT_CODE
    if (!parser.hadError)
    {
        disassembleChunk(currentChunk(), "code");
    }
#endif
}

static void expression();
static ParseRule* getRule(TokenType type);
static void parsePrecedence(Precedence precedence);

// ====================================================================
//                  Handling binary expressions
// ====================================================================

/**
 * @brief This function assumes that the entire left-hand operand expression
 * has already been compiled and the subsequent infix operator consumed.
 * This function comes to play at the moment when we need to handle the rest
 * of the arithmetic operators.
 */
static void binary()
{
    TokenType operatorType = parser.previous.type;
    ParseRule* rule = getRule(operatorType);

    /*
        using higher level of precedence for the right operand is because the binary
        opeartors are left-associative. Given a series of the same operators, like:
        1 + 2 + 3 + 4;
        we want to parse it like:
        ((1 + 2) + 3) + 4;
    */
    parsePrecedence((Precedence)(rule->precedence + 1));

    switch(operatorType)
    {
        case TOKEN_PLUS: emitByte(OP_ADD); break;
        case TOKEN_MINUS: emitByte(OP_SUBTRACT); break;
        case TOKEN_STAR: emitByte(OP_MULTIPLY); break;
        case TOKEN_SLASH: emitByte(OP_DIVIDE); break;
        default: return;
    }
}

// ====================================================================
//                  Handling grouping expressions
// ====================================================================

/**
 * @brief Handles the grouping expression.
 * It assumes that initial open parenthesis has already been consumed.
 * It recursively calls back into expression() to compile the expression
 * between the parentheses, then parses the close parenthesis at the end.
 * The grouping has no runtime semancits on its own and therefore doesn't
 * emit any bytecode. The inner call to expression() takes care of generating
 * bytecode for the expression inside the parentheses.
 */
static void grouping()
{
    expression();
    consume(TOKEN_RIGHT_PAREN, "Expect ')' after expression.");
}

// ====================================================================
//                  Handling number literals expressions
// ====================================================================

/**
 * @brief Inserts an entry in the constant pool.
 * 
 * @param value 
 * @return uint8_t 
 */
static uint8_t makeConstant(Value value)
{
    int idx = addConstant(currentChunk(), value);
    if (idx > UINT8_MAX)
    {
        error("Too many constants in one chunk.");
        return 0;
    }

    return (uint8_t)idx;
}

/**
 * @brief Adds the value to the constant pool by calling makeConstant()
 * function and then emits an OP_CONSTANT instruction that pushes the value
 * onto the stack at runtime.
 * @param value 
 */
static void emitConstant(Value value)
{
    emitBytes(OP_CONSTANT, makeConstant(value));
}

/**
 * @brief Compiles number literals.
 * This function (as the others as well) assumes that the token
 * for the number literal has already consumed and is stored in
 * 'parser.previous'.
 */
static void number()
{
    double value = strtod(parser.previous.start, NULL);
    emitConstant(value);
}

// ====================================================================
//                  Unary negation expressions
// ====================================================================

static void unary()
{
    TokenType operatorType = parser.previous.type;

    //compile the operand
    parsePrecedence(PREC_UNARY);

    //Emit the operator instruction
    switch(operatorType)
    {
        case TOKEN_MINUS: emitByte(OP_NEGATE); break;
        default: return;
    }
}

// ====================================================================
//                  The core functions
// ====================================================================

/**
 * @brief The table that drives the whole parser.
 * Here the designated initializer syntax is used to assign each
 * index with ParseRule structure initialized as follow:
 * { void (*prefix)(), void (*infix)(), precedence }.
 */
ParseRule rules[] = {
    [TOKEN_LEFT_PAREN]      = {grouping, NULL,   PREC_NONE},
    [TOKEN_RIGHT_PAREN]     = {NULL,     NULL,   PREC_NONE},
    [TOKEN_LEFT_BRACE]      = {NULL,     NULL,   PREC_NONE}, 
    [TOKEN_RIGHT_BRACE]     = {NULL,     NULL,   PREC_NONE},
    [TOKEN_COMMA]           = {NULL,     NULL,   PREC_NONE},
    [TOKEN_DOT]             = {NULL,     NULL,   PREC_NONE},
    [TOKEN_MINUS]           = {unary,    binary, PREC_TERM},
    [TOKEN_PLUS]            = {NULL,     binary, PREC_TERM},
    [TOKEN_SEMICOLON]       = {NULL,     NULL,   PREC_NONE},
    [TOKEN_SLASH]           = {NULL,     binary, PREC_FACTOR},
    [TOKEN_STAR]            = {NULL,     binary, PREC_FACTOR},
    [TOKEN_BANG]            = {NULL,     NULL,   PREC_NONE},
    [TOKEN_BANG_EQUAL]      = {NULL,     NULL,   PREC_NONE},
    [TOKEN_EQUAL]           = {NULL,     NULL,   PREC_NONE},
    [TOKEN_EQUAL_EQUAL]     = {NULL,     NULL,   PREC_NONE},
    [TOKEN_GREATER]         = {NULL,     NULL,   PREC_NONE},
    [TOKEN_GREATER_EQUAL]   = {NULL,     NULL,   PREC_NONE},
    [TOKEN_LESS]            = {NULL,     NULL,   PREC_NONE},
    [TOKEN_LESS_EQUAL]      = {NULL,     NULL,   PREC_NONE},
    [TOKEN_IDENTIFIER]      = {NULL,     NULL,   PREC_NONE},
    [TOKEN_STRING]          = {NULL,     NULL,   PREC_NONE},
    [TOKEN_NUMBER]          = {number,   NULL,   PREC_NONE},
    [TOKEN_AND]             = {NULL,     NULL,   PREC_NONE},
    [TOKEN_CLASS]           = {NULL,     NULL,   PREC_NONE},
    [TOKEN_ELSE]            = {NULL,     NULL,   PREC_NONE},
    [TOKEN_FALSE]           = {NULL,     NULL,   PREC_NONE},
    [TOKEN_FOR]             = {NULL,     NULL,   PREC_NONE},
    [TOKEN_FUN]             = {NULL,     NULL,   PREC_NONE},
    [TOKEN_IF]              = {NULL,     NULL,   PREC_NONE},
    [TOKEN_NIL]             = {NULL,     NULL,   PREC_NONE},
    [TOKEN_OR]              = {NULL,     NULL,   PREC_NONE},
    [TOKEN_PRINT]           = {NULL,     NULL,   PREC_NONE},
    [TOKEN_RETURN]          = {NULL,     NULL,   PREC_NONE},
    [TOKEN_SUPER]           = {NULL,     NULL,   PREC_NONE},
    [TOKEN_THIS]            = {NULL,     NULL,   PREC_NONE},
    [TOKEN_TRUE]            = {NULL,     NULL,   PREC_NONE},
    [TOKEN_VAR]             = {NULL,     NULL,   PREC_NONE},
    [TOKEN_WHILE]           = {NULL,     NULL,   PREC_NONE},
    [TOKEN_ERROR]           = {NULL,     NULL,   PREC_NONE},
    [TOKEN_EOF]             = {NULL,     NULL,   PREC_NONE},
};

/**
 * @brief This function starts at the current token and parses any expression
 * at the given precedence level.
 * 
 * @param precedence the level of precedence.
 */
static void parsePrecedence(Precedence precedence)
{
    advance();
    // fetch a rule of the given type and grab its function pointer to prefix rule 
    ParseFn prefixRule = getRule(parser.previous.type)->prefix;

    // e.g. expression starts from illegal token (else, ), } etc)
    if (NULL == prefixRule)
    {
        error("Expect expression.");
        return;
    }
    // perform that prefix rule
    prefixRule();

    // iterate through until there is more infix expressions.
    while(precedence <= getRule(parser.current.type)->precedence)
    {
        advance();
        ParseFn infixRule = getRule(parser.previous.type)->infix;
        infixRule();
    }
}

/**
 * @brief Returns the rule from rule table at the given index.
 * 
 * @param type 
 * @return ParseRule* 
 */
static ParseRule* getRule(TokenType type)
{
    return &rules[type];
}

static void expression()
{
    parsePrecedence(PREC_ASSIGNMENT);
}

/**
 * @brief Compiler's entry point.
 * 
 * @param source 
 * @param chunk 
 * @return true 
 * @return false 
 */
bool compile(const char* source, Chunk* chunk)
{
    Token token;
    initScanner(source);
    compilingChunk = chunk; // initialize before writing any bytecode

    parser.hadError = false;
    parser.panicMode = false;

    advance();
    expression();
    consume(TOKEN_EOF, "Expect end of expression");
    
    endCompiler();

    /*
        The value is inversed due to logic in interpret() function of vm.c
        module:
        consider that there is no compile-time error and this function
        returns hadError set to false. When interpret() receives this value
        it assumes that there IS actually an error and interrupts execution. 
    */
    return !parser.hadError;
}