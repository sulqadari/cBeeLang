#include <stdio.h>
#include <string.h>
#include "../include/common.h"
#include "../include/scanner.h"

typedef struct
{
    const char *start;
    const char *current;
    int line;
} Scanner;

Scanner scanner;

void initScanner(const char *source)
{
    scanner.start = source;
    scanner.current = source;
    scanner.line = 1;
}

static bool isDigit(char c)
{
    return (c >= '0') && (c <= '9');
}

static bool isAtEnd(void)
{
    return *scanner.current == '\0';
}

static char advance(void)
{
    scanner.current++;
    return scanner.current[-1];
}

static char peek(void)
{
    return *scanner.current;
}

static char peekNext(void)
{
    if (isAtEnd())
        return '\0';
    
    return scanner.current[1];
}

/**
  Compares current token (pointed by scanner.current) with
  a given one. Shifts scanner.current forward if true.
  @param char expected the character to be compared
*/
static bool match(char expected)
{
    if (isAtEnd())
        return false;
    
    if (*scanner.current != expected)
        return false;
    
    scanner.current++;
    return true;
}

static Token makeToken(TokenType type)
{
    Token token;
    token.type = type;
    token.start = scanner.start;
    token.length = (int)(scanner.current - scanner.start);
    token.line = scanner.line;
    return token;
}

static Token errorToken(const char* message)
{
    Token token;
    token.type = TOKEN_ERROR;
    token.start = message;
    token.length = (int)strlen(message);
    token.line = scanner.line;
    return token;
}

/*
  Advances the Scanner past any leading whitespaces.
  This function ensures us that after it returns, the next character
  is a meaningful one.

  In case this function encounters with '\n' character, it increments
  Scanner's 'line' counter.
*/
static void skipWhitespace(void)
{
    for (;;)
    {
        // using peek() instead of advance() is to avoid
        // unintentional non-whitespace character consuming.
        char c = peek();
        switch (c)
        {
            case ' ':
            case '\r':
            case '\t':
                advance();
            break;
            case '\n':
                scanner.line++;
                advance();
            break;
            case '/':
                if (peekNext() == '/')
                {
                    // A comment goes until the end of the line.
                    while (peek() != '\n' && !isAtEnd())
                        advance();
                } else
                {
                    return;
                }
            break;
            default: return;
        }
    }
}

static Token number(void)
{
    while (isDigit(peek()))
        advance();

    // Look for a fractional part.
    if ((peek() == '.') && isDigit(peekNext()))
    {
        // Consume the ".".
        advance();

        while (isDigit(peek()))
            advance();
    }

    return makeToken(TOKEN_NUMBER);
}

static Token string(void)
{
    while ((peek() != '"') && !isAtEnd())
    {
        if (peek() == '\n')
            scanner.line++;
        
        advance();
    }

    if (isAtEnd())
        return errorToken("Unterminated string.");

    // The closing quote.
    advance();
    return makeToken(TOKEN_STRING);
}

Token scanToken(void)
{
    skipWhitespace();
    // set starting offset to the first character
    // of the successive lexeme.
    scanner.start = scanner.current;

    if (isAtEnd())
        return makeToken(TOKEN_EOF);

    char c = advance();
    
    if (isDigit(c))
        return number();
    
    switch (c) {
        case '(': return makeToken(TOKEN_LEFT_PAREN);
        case ')': return makeToken(TOKEN_RIGHT_PAREN);
        case '{': return makeToken(TOKEN_LEFT_BRACE);
        case '}': return makeToken(TOKEN_RIGHT_BRACE);
        case ';': return makeToken(TOKEN_SEMICOLON);
        case ',': return makeToken(TOKEN_COMMA);
        case '.': return makeToken(TOKEN_DOT);
        case '-': return makeToken(TOKEN_MINUS);
        case '+': return makeToken(TOKEN_PLUS);
        case '/': return makeToken(TOKEN_SLASH);
        case '*': return makeToken(TOKEN_STAR);
        case '!': return makeToken(match('=') ? TOKEN_BANG_EQUAL : TOKEN_BANG);
        case '=': return makeToken(match('=') ? TOKEN_EQUAL_EQUAL : TOKEN_EQUAL);
        case '<': return makeToken(match('=') ? TOKEN_LESS_EQUAL : TOKEN_LESS);
        case '>': return makeToken(match('=') ? TOKEN_GREATER_EQUAL : TOKEN_GREATER);
        case '"': return string();
    }

    return errorToken("Unexpected character.");
}