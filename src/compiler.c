#include "../includes/common.h"
#include "../includes/compiler.h"
#include "../includes/scanner.h"

void compile(const char* source)
{
    initScanner(source);
    
    int line = -1;
    for (;;)
    {
        Token token = scanToken();
        if (token.line != line)
        {
            printf("%4d ", token.line);
            line = token.line;
        }else
        {
            printf("    | ");
        }
        // using * lets pass the precision as an argument
        printf("%2d '%.*s'\n", token.type, token.length, token.start);

        if (TOKEN_EOF == token.type) break;
    }
}