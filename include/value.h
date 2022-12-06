#ifndef _H_BEELANG_VALUE
#define _H_BEELANG_VALUE

#include "common.h"

typedef double Double;

/*
    The constant pool - is an array of constant data associated
    with each compiled file. Each chunk of bytecode will carry with
    it a list of values that appear as literals in the program.

    The 'pool' array keeps all constants, even simple integers, which mean
    there is no 'immediate instructions' among opcodes.
*/
typedef struct
{
    int capacity;
    int count;
    Double *constants;
}ConstantPool;

/*
    Sets the fields of 'ConstantPool' structure to zero and NULL.
*/
void initConstantPool(ConstantPool *constantPool);

/*
    Deallocates all of the memory and calls initConstantPool() function to set
    ConstantPool to initial state.
*/
void freeConstantPool(ConstantPool *constantPool);

/*
    Prints a given value. Used in debug.c
*/
void printValue(Double value);

/*
    Appends one constant to the end of the 'ConstantPool.constants' array 
    at a time.
    If 'ConstantPool.constants' array haven't enough room for the new value,
    then this function doubles its size by means of
    'INCREASE_CAPACITY' and 'INCREASE_ARRAY' macros.
    The former doubles ConstantPool.capacity field and the latter
    increases 'ConstantPool.constants' by new value.
*/
void appendConstant(ConstantPool *constantPool, Double constant);

#endif // _H_BEELANG_VALUE