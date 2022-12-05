#include <stdlib.h>
#include "../include/bytecode.h"
#include "../include/memory.h"

void initBytecode(Bytecode *bytecode)
{
    bytecode->count = 0;
    bytecode->capacity = 0;
    bytecode->code = NULL;
}

void freeBytecode(Bytecode *bytecode)
{
    FREE_ARRAY(uint8_t, bytecode->code, bytecode->capacity);
    initBytecode(bytecode);
}

void appendBytecode(Bytecode *bytecode, uint8_t byte)
{
    if (bytecode->capacity < bytecode->count + 1)
    {
        int prevSize = bytecode->capacity;
        
        bytecode->capacity = INCREASE_CAPACITY(prevSize);
        bytecode->code = INCREASE_ARRAY(uint8_t, bytecode->code,
                                        prevSize, bytecode->capacity);
        
    }

    bytecode->code[bytecode->count] = byte;
    bytecode->count++;
}
