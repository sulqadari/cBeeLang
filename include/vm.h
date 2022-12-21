#ifndef _H_BEELANG_VM
#define _H_BEELANG_VM

#include "bytecode.h"
#include "value.h"

#define STACK_MAX 256

typedef struct
{
    Bytecode *bytecode; // instruction set
    uint8_t *ip;        // instruction pointer
    Value stack[STACK_MAX];
    Value *stackTop;   // stack pointer
}VM;

typedef enum
{
    INTERPRET_OK,
    INTERPRET_COMPILE_ERROR,
    INTERPRET_RUNTIME_ERROR,
    STACK_OVERFLOW,
    STACK_UNDERFLOW,
}InterpretResult;


void initVM(void);
void freeVM(void);

/*
  -= vm.h =-
  VM's entry point.
  This function scans, parses and interprets source code.
*/
InterpretResult interpret(const char *source);

/*
  -= vm.h =-
  Pushes a value onto the stack.
*/
void push(Value value);

/* 
  -= vm.h =-
  Remvoes a value from the stack.
*/
Value pop(void);

#endif  //_H_BEELANG_VM