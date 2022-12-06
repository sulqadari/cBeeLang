#ifndef _H_BEELANG_VM
#define _H_BEELANG_VM

#include "bytecode.h"

typedef struct
{
    Bytecode *bytecode; // instruction set
    uint8_t *ip;        // instruction pointer
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

#endif  //_H_BEELANG_VM