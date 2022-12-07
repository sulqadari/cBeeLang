#ifndef _H_BEELANG_SCANNER
#define _H_BEELANG_SCANNER

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

/*
  Note: the 'start' pointer points to the substring of the source code string.
  Thus, current implementation requires us to ensure that source string outlives all
  of the tokens. That's why runFile() function doesn't free the string until
  interpret() finishes executing the code and returns.
*/
typedef struct
{
    TokenType type;
    const char *start;  // pointer to the lexeme.
    int length;         // lexeme's length
    int line;           // line number
} Token;

void initScanner(const char *source);

/*
  Scans a complete token.
*/
Token scanToken(void);

#endif // _H_BEELANG_SCANNER