#include <stdio.h>
#include <stdlib.h>
#include "../include/common.h"
#include "../include/compiler.h"
#include "../include/debug.h"
#include "../include/vm.h"

VM vm;

/*
  The core function which intended to interpret bytecode and output result.
*/
static InterpretResult run(void);

static void resetStack(void)
{
    vm.stackTop = vm.stack;
}

void initVM(void)
{
    resetStack();
}

void freeVM(void)
{

}

void push(Double value)
{
    if ((vm.stackTop - vm.stack) >= (STACK_MAX * sizeof(Double)))
    {
        exit(STACK_OVERFLOW);
    }

    *vm.stackTop = value;
    vm.stackTop++;
}

Double pop(void)
{
    if (((int64_t)vm.stackTop - (int64_t)vm.stack) < 0)
    {
        exit(STACK_UNDERFLOW);
    }

    vm.stackTop--;
    return *vm.stackTop;
}

static InterpretResult run(void)
{
#define READ_BYTE() (*vm.ip++)
#define READ_CONSTANT() (vm.bytecode->constantPool.constants[READ_BYTE()])
#define BINARY_OP(op)     \
    do {                  \
        double b = pop(); \
        double a = pop(); \
        push(a op b);     \
    } while (false)

    for (;;)
    {
#ifdef DEBUG_TRACE_EXECUTION
        // stack trace start
        printf("          ");
        for (Double* slot = vm.stack; slot < vm.stackTop; slot++)
        {
            printf("[ ");
            printValue(*slot);
            printf(" ]");
        }
        printf("\n");
        // stack trace end

        disassembleInstruction(vm.bytecode, (int)(vm.ip - vm.bytecode->code));
#endif //DEBUG_TRACE_EXECUTION

        uint8_t instruction;
        switch (instruction = READ_BYTE())
        {
            case OP_CONSTANT:
            {
                Double constant = READ_CONSTANT();
                push(constant);
            }break;
            case OP_ADD:BINARY_OP(+); break;
            case OP_SUBTRACT: BINARY_OP(-); break;
            case OP_MULTIPLY: BINARY_OP(*); break;
            case OP_DIVIDE: BINARY_OP(/); break;
            case OP_NEGATE:
            {
                push(-pop());
            }break;
            case OP_RETURN:
            {
                printValue(pop());
                printf("\n");
                return INTERPRET_OK;
            }
        }
    }

#undef READ_BYTE
#undef READ_CONSTANT
#undef BINARY_OP
}

InterpretResult interpret(const char *source)
{
    Bytecode bytecode;
    initBytecode(&bytecode);

    if (!compile(source, &bytecode))
    {
        freeBytecode(&bytecode);
        return INTERPRET_COMPILE_ERROR;
    }

    vm.bytecode = &bytecode;
    vm.ip = vm.bytecode->code;

    InterpretResult result = run();

    freeBytecode(&bytecode);
    return result;
}