#include "../includes/common.h"
#include "../includes/chunk.h"
#include "../includes/debug.h"

int main(int argc, const char* argv[])
{
    //chunk test
    Chunk chunk;
    initChunk(&chunk);
    writeChunk(&chunk, OP_RETURN);

    disassembleChunk(&chunk, "test chunk");
    freeChunk(&chunk);

    return 0;
}