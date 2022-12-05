#include "../includes/scanner.h"

typedef struct
{
    const char* start;      // marks the beginning of the current lexeme being scanned.
    const char* current;    // current character being looked at.
    int line;               // tracks what line the current lexeme is on.
}Scanner;

/**
 * @brief The Scanner's Global Variable.
 * This object parses sequence of user's input represented as source code
 * and passes it to the compiler module
 */
Scanner scanner;

static bool isAtEnd()
{
    return '\0' == *scanner.current;
}

static char advance()
{
    scanner.current++;
    return scanner.current[-1];
}

/**
 * @brief Returns the current character without advancing the scanner.current
 * 
 * @return char 
 */
static char peek()
{
    return *scanner.current;
}

static char peekNext()
{
    if (isAtEnd())
        return '\0';
    
    return scanner.current[1];
}

/**
 * @brief compares the given character against the one pointed by scanner.current.
 * If they match, then scanner's <code>currect</code> will be shifted forward.
 * 
 * @param expected 
 * @return returns false if values don't match or scanner reached the end of file.
 * True otherwise.
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
    token.length = (int)(scanner.current - scanner.start);  // define the length of lexeme
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

static void skipWhitespace()
{
    for(;;)
    {
        char c = peek();
        switch(c)
        {
            case ' ' :
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
                {   // a commnet goes until the end of the line
                    while(peek() != '\n' && !isAtEnd())
                        advance();
                }else
                {
                    return;
                }
            break;
            default: return;
        }
    }
}

static TokenType checkKeyword(int start, int length, const char* rest, TokenType type)
{   
    // the lexeme must be exactly as long as the keyword and..
    if (((scanner.current - scanner.start) == (start + length)) &&
    //..the remaining characters must match exactly
         (memcmp(scanner.start + start, rest, length) == 0))
    {
        return type;
    }

    return TOKEN_IDENTIFIER;
}

/**
 * @brief Implementation of 'trie'.
 * 
 * @return TokenType 
 */
static TokenType identifierType()
{
    switch(scanner.start[0])
    {
        case 'a': return checkKeyword(1, 2, "nd", TOKEN_AND);
        case 'c': return checkKeyword(1, 4, "lass", TOKEN_CLASS);
        case 'e': return checkKeyword(1, 3, "lse", TOKEN_ELSE);
        case 'f':
            // ensure that user don't intend to use 'f' letter solely as identifier
            if (scanner.current - scanner.start > 1)
            {
                switch(scanner.start[1])
                {
                    case 'a': return checkKeyword(2, 3, "lse", TOKEN_FALSE);
                    case 'o': return checkKeyword(2, 1, "r", TOKEN_FOR);
                    case 'u': return checkKeyword(2, 1, "n", TOKEN_FUN);
                }
            }
        break;
        case 'i': return checkKeyword(1, 1, "f", TOKEN_IF);
        case 'n': return checkKeyword(1, 2, "il", TOKEN_NIL);
        case 'o': return checkKeyword(1, 1, "r", TOKEN_OR);
        case 'p': return checkKeyword(1, 4, "rint", TOKEN_PRINT);
        case 'r': return checkKeyword(1, 5, "eturn", TOKEN_RETURN);
        case 's': return checkKeyword(1, 4, "uper", TOKEN_SUPER);
        case 't':
            // the same as for 'f', we need to check that there even IS a second letter,
            // because 't' by itself is a valid identifier too.
            if (scanner.current - scanner.start > 1)
            {
                switch(scanner.current[1])
                {
                    case 'h': return checkKeyword(2, 2, "is", TOKEN_THIS);
                    case 'r': return checkKeyword(2, 2, "ue", TOKEN_TRUE);
                }
            }
        break;
        case 'v': return checkKeyword(1, 2, "ar", TOKEN_VAR);
        case 'w': return checkKeyword(1, 4, "hile", TOKEN_WHILE);
    }
    return TOKEN_IDENTIFIER;
}

static Token string()
{
    while(peek() != '"' && !isAtEnd())
    {
        if (peek() == '\n')
            scanner.line++;
        
        advance();
    }

    if (isAtEnd())
        return errorToken("Unterminated string.");

    // consume the closing double quote.
    advance();

    return makeToken(TOKEN_STRING);
}

void initScanner(const char* source)
{
    scanner.start = source;
    scanner.current = source;
    scanner.line = 1;
}

static bool isAlpha(char c)
{
    return (c >= 'a' && c <= 'z') ||
           (c >= 'A' && c <= 'Z') ||
           (c == '_');
}

static bool isDigit(char c)
{
    return c >= '0' && c <= '9';
}

static Token number()
{
    while(isDigit(peek()))
        advance();
    
    // look for a fractional part.
    if ((peek() == '.') && isDigit(peekNext()))
    {
        advance();

        while(isDigit(peek()))
            advance();
    }

    return makeToken(TOKEN_NUMBER);
}

static Token identifier()
{
    while (isAlpha(peek()) || isDigit(peek()))
        advance();
    
    return makeToken(identifierType());
}

Token scanToken()
{
    skipWhitespace();

    // set scanner.start to point to the current character so we remember where the
    // lexeme we're about to scan starts
    scanner.start = scanner.current;

    if (isAtEnd())
        return makeToken(TOKEN_EOF);
    
    char c = advance();

    if (isAlpha(c)) return identifier();
    if (isDigit(c)) return number();
    
    switch(c)
    {
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

    return errorToken("Unexpected character");
}