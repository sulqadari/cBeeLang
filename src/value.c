#include <stdio.h>
#include "../include/memory.h"
#include "../include/value.h"


void initConstantPool(ConstantPool *constantPool)
{
    constantPool->count = 0;
    constantPool->capacity = 0;
    constantPool->constants = NULL;
}

void freeConstantPool(ConstantPool *constantPool)
{
    FREE_ARRAY(Value, constantPool->constants, constantPool->capacity);
    initConstantPool(constantPool);
}

void printValue(Value value)
{
    switch (value.type)
    {
        case VAL_BOOL:   printf(AS_BOOL(value) ? "true" : "false"); break;
        case VAL_NIL:    printf("nil");                             break;
        case VAL_NUMBER: printf("%g", AS_NUMBER(value));            break;
    }
}

bool valuesEqual(Value a, Value b)
{
    if (a.type != b.type)   // if values have different types
        return false;       // return false.
    
    switch(a.type)
    {
        case VAL_BOOL: return AS_BOOL(a) == AS_BOOL(b);
        case VAL_NIL: return true;
        case VAL_NUMBER: return AS_NUMBER(a) == AS_NUMBER(b);
        default: return false;
    }
}

void appendConstant(ConstantPool *constantPool, Value constant)
{
    // Increase array's capacity if there is no space for the next constant
    if (constantPool->capacity < constantPool->count + 1)
    {
        int oldCapacity = constantPool->capacity;
        
        constantPool->capacity = INCREASE_CAPACITY(oldCapacity);
        constantPool->constants = INCREASE_ARRAY(Value, constantPool->constants,
                                        oldCapacity, constantPool->capacity);
        
    }

    constantPool->constants[constantPool->count] = constant;
    constantPool->count++;
}