#ifndef _H_BEELANG_BYTECODE
#define _H_BEELANG_BYTECODE

#include "common.h"
#include "value.h"

/*
    Almost all instructions have a one-byte operation code
    universally shortened to opcode.

    That number controls what kind of instruction we're dealing with:
    addition, subtraction, lookup variable etc.
*/
typedef enum
{
    OP_CONSTANT_LONG,   // load three-byte constant from ConstantPool 
    OP_CONSTANT,        // load the constant from ConstantPool
    OP_RETURN,          // return from function/method call
}OpCode;

/*
    Bytecode - is a series of instructions.
*/
typedef struct
{
    int count;      // actual number of elements in 'code' array
    int capacity;   // the length of 'code' array
    uint8_t *code;  // array of bytecodes
    int *lines;     // trances lines of bytecodes. Used in case runtime error occured.
    ConstantPool constantPool;
}Bytecode;

/*
    Sets the fields of 'Bytecode' structure to zero and NULL.
*/
void initBytecode(Bytecode *bytecode);

/*
    Deallocates all of the memory and calls initBytecode() function to set
    Bytecode to initial state.
*/
void freeBytecode(Bytecode *bytecode);

/*
    Appends one bytecode to the end of the 'Bytecode.code' array 
    at a time.
    If 'Bytecode.code' array haven't enough room for the new bytecode,
    then this function doubles its size by means of
    'INCREASE_CAPACITY' and 'INCREASE_ARRAY' macros.
    The former doubles Bytecode.capacity field and the latter
    increases 'Bytecode.code' by new value.
*/
void appendBytecode(Bytecode *bytecode, uint8_t byte, int line);

/*
    Convenience function to add a new constant to ConstantPool inside this module
    directly.
    @returns index of constant being appended.
*/
int addConstant(Bytecode *bytecode, Double value);

/*
    Adds constant to Bytecode.ConstantPool and then writes an appropriate instruction
    to load the constant.
*/
//void writeConstant(Bytecode *byteCode, Double constant, int line);

#endif // _H_BEELANG_BYTECODE