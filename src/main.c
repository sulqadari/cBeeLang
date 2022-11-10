#include "../includes/common.h"
#include "../includes/chunk.h"
#include "../includes/debug.h"
#include "../includes/vm.h"

/**
 * @brief Project entry point.
 * 
 * @param argc 
 * @param argv 
 * @return int 
 */
int main(int argc, const char* argv[])
{
    initVM();

    //chunk test
    Chunk chunk;
    initChunk(&chunk);

    int const_idx = addConstant(&chunk, 1.2);
    writeChunk(&chunk, OP_CONSTANT, 0);
    writeChunk(&chunk, const_idx, 0);
    writeChunk(&chunk, OP_RETURN, 0);

    const_idx = addConstant(&chunk, 2.1);
    writeChunk(&chunk, OP_CONSTANT, 1);
    writeChunk(&chunk, const_idx, 1);
    writeChunk(&chunk, OP_RETURN, 1);

    disassembleChunk(&chunk, "test chunk");
    freeVM();
    freeChunk(&chunk);

    return 0;
}