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
    FREE_ARRAY(Double, constantPool->constants, constantPool->capacity);
    initConstantPool(constantPool);
}

void printValue(Double value)
{
    printf("%g", value);
}


void appendConstant(ConstantPool *constantPool, Double constant)
{
    // Increase array's capacity if there is no space for the next constant
    if (constantPool->capacity < constantPool->count + 1)
    {
        int oldCapacity = constantPool->capacity;
        
        constantPool->capacity = INCREASE_CAPACITY(oldCapacity);
        constantPool->constants = INCREASE_ARRAY(Double, constantPool->constants,
                                        oldCapacity, constantPool->capacity);
        
    }

    constantPool->constants[constantPool->count] = constant;
    constantPool->count++;
}