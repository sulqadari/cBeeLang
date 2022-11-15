#include "../includes/common.h"
#include "../includes/chunk.h"
#include "../includes/debug.h"
#include "../includes/vm.h"

/**
 * @brief Stops execution. The analogous of system("pause") in C++
 * 
 */
static void idle_exec()
{
    printf("Press any key to continue...");
    getchar();
}

static void repl()
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

static char* readFile(const char* path)
{
    FILE * file = fopen(path, "rb");

    if (NULL == file)
    {
        fprintf(stderr, "Couldn't open file \"%s\".\n", path);
        exit(74);
    }

    fseek(file, 0L, SEEK_END);      // move file's pointer to the very end
    size_t fileSize = ftell(file);  // retrieve the value of the file's pointer
    rewind(file);                   // rewind back to the beginning.

    char* buffer = (char*)malloc(fileSize + 1); // allocate string gor that size + '\0'

    if (NULL == buffer)
    {
        fprintf(stderr, "Not enough memory to read \"%s\".\n", path);
        exit(74);
    }

    size_t bytesRead = fread(buffer, sizeof(char), fileSize, file); // read file

    if (bytesRead < fileSize)
    {
        fprintf(stderr, "Couldn't read file \"%s\".\n", path);
        exit(74);
    }

    buffer[bytesRead] = '\0';       // add string terminating character

    fclose(file);
    return buffer;
}

static void runFile(const char* path)
{
    char* source = readFile(path);
    InterpretResult result = interpret(source);
    free(source);

    if (INTERPRET_COMPILE_ERROR == result) exit(65);
    if (INTERPRET_RUNTIME_ERROR == result) exit(70);
}

/**
 * @brief Project entry point.
 * 
 * @param argc 
 * @param argv 
 * @return int 
 */
int main(int argc, const char* argv[])
{
    initVM();   // create isntance of VM

    if (argc == 1)
    {
        repl();
    }else if (argc == 2)
    {
        runFile(argv[1]);
    }else
    {
        fprintf(stderr, "Usage: BeeLang.exe [path]\n");
        exit(64);
    }
    freeVM();   // kill VM

    idle_exec();
    return 0;
}