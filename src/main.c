#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/common.h"
#include "../include/vm.h"

/* Executes a single command line passed via console */
static void repl(void);

/*
  Executes a script file, whose name is passed as an argument while (e.g. binch.exe script.txt)
  @param path to script.
*/
static void runFile(const char *path);

/*
  Reads input script file into char* buffer.
  @param path to script.
  @returns buffer with input script.
*/
static char* readFile(const char *path);

int main(int argc, const char* argv[])
{
    initVM();

    if (argc == 1)
    {
        repl();
    }else if (argc == 2)
    {
        runFile(argv[1]);
    }else
    {
        fprintf(stderr, "Usage: binch.exe C:\\path\\to\\script.txt\n");
        exit(64);
    }
    
    freeVM();

    return 0;
}

static void repl(void)
{
    char line[1024];
    
    for (;;)
    {
        printf(">> ");

        if (!fgets(line, sizeof(line), stdin))
        {
            printf("\n");
            break;
        }

        interpret(line);
    }
}

static void runFile(const char *path)
{
    char *source = readFile(path);
    InterpretResult result = interpret(source);
    free(source);

    if (result == INTERPRET_COMPILE_ERROR) exit(65);
    if (result == INTERPRET_RUNTIME_ERROR) exit(70);
}

static char* readFile(const char *path)
{
    FILE *file = fopen(path, "rb");
    if (NULL == file)
    {
        fprintf(stderr, "Couldn't open file \"%s\".\n", path);
        exit(74);
    }

    // shift file pointer (don't be confused with pointer to FILE) to the end of file
    fseek(file, 0L, SEEK_END);
    // fetch the index of file pointer. Its value determines the length of buffer to be created.
    size_t fileSize = ftell(file);
    // roll file pointer back so that we will read it from the start.
    rewind(file);
    // + 1 char for string terminating symbol ('\0')
    char *buffer = (char*) malloc(fileSize + 1);
    if (NULL == buffer)
    {
        fprintf(stderr, "Not enough memory to read \"%s\".\n", path);
        exit(74);
    }
    // read input data into the buffer. Store in 'bytesRead' the actual number of characters.
    size_t bytesRead = fread(buffer, sizeof(char), fileSize, file);
    if (bytesRead < fileSize)
    {
        fprintf(stderr, "Couldn't read the whole data from the file \"%s\".\n", path);
        exit(74);
    }

    buffer[bytesRead] = '\0';

    fclose(file);
    return buffer;
}