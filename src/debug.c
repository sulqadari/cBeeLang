#include <stdio.h>
#include "../includes/debug.h"
#include "../includes/value.h"

void disassembleChunk(Chunk* chunk, const char* name)
{
    printf("== %s ==\n", name); // print chunk's header.

    for (int offset = 0; offset < chunk->count; )
    {
        offset = disassembleInstruction(chunk, offset);
    }
}

int disassembleInstruction(Chunk* chunk, int offset)
{
    printf("%04d ", offset);    // print the offset of successive instruction
    
    // display source line
    if (offset > 0 && chunk->lines[offset] == chunk->lines[offset - 1])
        printf("    | "); // designates that current opcode is at the same line with preceding one
    else
        printf("%4d ", chunk->lines[offset]);
    
    uint8_t instruction = chunk->code[offset];  // retrieve the opcode at the given offset.
    switch(instruction)
    {
        case OP_CONSTANT:
            return constantInstruction("OP_CONSTANT", chunk, offset);
        case OP_RETURN:
            return simpleInstruction("OP_RETURN", offset);
        default:
            printf("Unknown opcode %d\n", instruction);
            return offset + 1;
    }

}

static int constantInstruction(const char* name, Chunk* chunk, int offset)
{
    uint8_t const_idx = chunk->code[offset + 1];
    printf("%-16s %4d '", name, const_idx);
    printValue(chunk->constants.values[const_idx]);
    printf("'\n");

    return offset + 2;  //[OP_CONSTANT, operand]
}

static int simpleInstruction(const char* name, int offset)
{
    printf("%s\n", name);
    return offset + 1;  //[opcode]
}
