#ifndef _H_BEELANG_VALUE
#define _H_BEELANG_VALUE

#include "common.h"

typedef double Value;

/*
    -= value.h =-
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
    Value *constants;
}ConstantPool;

/*  
    -= value.h =-
    Sets the fields of 'ConstantPool' structure to zero and NULL.
*/
void initConstantPool(ConstantPool *constantPool);

/*
    -= value.h =-
    Deallocates all of the memory and calls initConstantPool() function to set
    ConstantPool to initial state.
*/
void freeConstantPool(ConstantPool *constantPool);

/*
    -= value.h =-
    Prints a given value. Used for debugging purposes
*/
void printValue(Value value);

/*
    -= value.h =-
    Appends one constant to the end of the 'ConstantPool.constants' array 
    at a time.
    If 'ConstantPool.constants' array haven't enough room for the new value,
    then this function doubles its size by means of
    'INCREASE_CAPACITY' and 'INCREASE_ARRAY' macros.
    The former doubles ConstantPool.capacity field and the latter
    increases 'ConstantPool.constants' by new value.
*/
void appendConstant(ConstantPool *constantPool, Value constant);

#endif // _H_BEELANG_VALUE