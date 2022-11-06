#include <stdio.h>
#include "../includes/debug.h"

void disassembleChunk(Chunk* chunk, const char* name)
{
    printf("== %s ==\n", name); // print chunk's header.

    for (int offset = 0; offset < chunk->count; )
    {
        offset = disassembleInstruction(chunk, offset);
    }
}

static int simpleInstruction(const char* name, int offset)
{
    printf("%s\n", name);
    return offset + 1;
}

int disassembleInstruction(Chunk* chunk, int offset)
{
    printf("%04d ", offset);    // print the byte offset
    uint8_t instruction = chunk->code[offset];  // read single byte at the given offset.
    switch(instruction)
    {
        case OP_RETURN:
            return simpleInstruction("OP_RETURN", offset);
        default:
            printf("Unknown opcode %d\n", instruction);
            return offset + 1;
    }

}