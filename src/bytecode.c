#include <stdlib.h>
#include "../include/bytecode.h"
#include "../include/memory.h"

void initBytecode(Bytecode *bytecode)
{
    bytecode->count = 0;
    bytecode->capacity = 0;
    bytecode->code = NULL;
    bytecode->lines = NULL;
    initConstantPool(&bytecode->constantPool);
}

void freeBytecode(Bytecode *bytecode)
{
    FREE_ARRAY(uint8_t, bytecode->code, bytecode->capacity);
    FREE_ARRAY(int, bytecode->lines, bytecode->capacity);
    freeConstantPool(&bytecode->constantPool);
    initBytecode(bytecode);
}

void appendBytecode(Bytecode *bytecode, uint8_t byte, int line)
{
    // Increase array's capacity if there is no space for the next bytecode
    if (bytecode->capacity < bytecode->count + 1)
    {
        int oldCapacity = bytecode->capacity;
        
        bytecode->capacity = INCREASE_CAPACITY(oldCapacity);
        bytecode->lines = INCREASE_CAPACITY(oldCapacity);
        bytecode->code = INCREASE_ARRAY(uint8_t, bytecode->code,
                                        oldCapacity, bytecode->capacity);
        
    }

    bytecode->code[bytecode->count] = byte;
    bytecode->lines[bytecode->count] = line;
    bytecode->count++;
}

int addConstant(Bytecode *bytecode, Double value)
{
    appendConstant(&bytecode->constantPool, value);
    return bytecode->constantPool.count - 1;
}
