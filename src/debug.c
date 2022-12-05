#include <stdio.h>
#include "../include/debug.h"

void disassembleBytecode(Bytecode *bytecode, const char *name)
{
    printf("== %s ==\n", name);

    for(int offset = 0; offset < bytecode->count; )
    {
        offset = disassembleInstruction(bytecode, offset);
    }
}

/*
    Simple instruction's handler.
    The 'simple' instruction means a one-byte instruction.
*/
static int simpleInstruction(const char *name, int offset)
{
    printf("%s\n", name);   //print name of current instruction
    return offset + 1;
}

int disassembleInstruction(Bytecode *bytecode, int offset)
{
    printf("%04d ", offset);    // print current instruction's offset
    
    //retrieve bytecode under given offset
    uint8_t instruction = bytecode->code[offset];
    switch (instruction)
    {
        case OP_RETURN:
            return simpleInstruction("OP_RETURN", offset);
        default:
            printf("Unknown opcode %d\n", instruction);
            return offset + 1;
    }
}