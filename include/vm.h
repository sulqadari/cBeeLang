#ifndef _H_BEELANG_VM
#define _H_BEELANG_VM

#include "bytecode.h"
#include "value.h"

#define STACK_MAX 256

typedef struct
{
    Bytecode *bytecode; // instruction set
    uint8_t *ip;        // instruction pointer
    Double stack[STACK_MAX];
    Double *stackTop;   // stack pointer
}VM;

typedef enum
{
    INTERPRET_OK,
    INTERPRET_COMPILE_ERROR,
    INTERPRET_RUNTIME_ERROR
}InterpretResult;


void initVM(void);
void freeVM(void);
InterpretResult interpret(Bytecode *bytecode);
void push(Double value);
Double pop(void);

#endif  //_H_BEELANG_VM