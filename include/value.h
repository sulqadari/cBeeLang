#ifndef _H_BEELANG_VALUE
#define _H_BEELANG_VALUE

#include "common.h"

/*
    -= value.h =-
    Enum for each kind of value the VM supports.
    In other words, this is the VM's notion of "type", not the user.
*/
typedef enum
{
    VAL_BOOL,
    VAL_NIL,
    VAL_NUMBER
} ValueType;

/*
    -= value.h =-
    Tagged union.
    Contains two parts: a type 'tag' and a payload for the actual value.
*/
typedef struct
{
    ValueType type;
    union {
        bool boolean;
        double number;
    } as;
} Value;

#define IS_BOOL(value)   ((value).type == VAL_BOOL)
#define IS_NIL(value)    ((value).type == VAL_NIL)
#define IS_NUMBER(value) ((value).type == VAL_NUMBER)

/* Unpacks ValueType.boolean to native C boolean*/
#define AS_BOOL(value)   ((value).as.boolean)
/* Unpacks ValueType.number to native C double*/
#define AS_NUMBER(value) ((value).as.number)

/* Converts from native C bool to a ValueType.boolean */
#define BOOL_VAL(value)     ((Value){VAL_BOOL, {.boolean = value}})
/* Converts from native C null to a ValueType.nul */
#define NIL_VAL             ((Value){VAL_NIL, {.number = 0}})
/* Converts from native C double to a ValueType.number */
#define NUMBER_VAL(value)   ((Value){VAL_NUMBER, {.number = value}})

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
} ConstantPool;

bool valuesEqual(Value a, Value b);
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