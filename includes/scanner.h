#ifndef BEELANG_SCANNER_H
    #define BEELANG_SCANNER_H

    #include "../includes/common.h"
    
    typedef enum {
        // Single-character tokens.
        TOKEN_LEFT_PAREN, TOKEN_RIGHT_PAREN,
        TOKEN_LEFT_BRACE, TOKEN_RIGHT_BRACE,
        TOKEN_COMMA, TOKEN_DOT, TOKEN_MINUS, TOKEN_PLUS,
        TOKEN_SEMICOLON, TOKEN_SLASH, TOKEN_STAR,
        // One or two character tokens.
        TOKEN_BANG, TOKEN_BANG_EQUAL,
        TOKEN_EQUAL, TOKEN_EQUAL_EQUAL,
        TOKEN_GREATER, TOKEN_GREATER_EQUAL,
        TOKEN_LESS, TOKEN_LESS_EQUAL,
        // Literals.
        TOKEN_IDENTIFIER, TOKEN_STRING, TOKEN_NUMBER,
        // Keywords.
        TOKEN_AND, TOKEN_CLASS, TOKEN_ELSE, TOKEN_FALSE,
        TOKEN_FOR, TOKEN_FUN, TOKEN_IF, TOKEN_NIL, TOKEN_OR,
        TOKEN_PRINT, TOKEN_RETURN, TOKEN_SUPER, TOKEN_THIS,
        TOKEN_TRUE, TOKEN_VAR, TOKEN_WHILE,

        TOKEN_ERROR, TOKEN_EOF
    } TokenType;

    typedef struct
    {
        TokenType type;     // token type enum
        const char* start;
        int length;
        int line;
    }Token;

    void initScanner(const char* source);

    /**
     * @brief Each call to this function scans a complete token.
     * When we enter into in we start at the beginning of a new token.
     * 
     * Note: its implementation doesn't report lexical errors, it just
     * returns TOKEN_ERROR and leaves it up to the to the Parser
     * to report them.
     * @return Token 
     */
    Token scanToken();
    
#endif //BEELANG_SCANNER_H