#include <stdio.h>
#include "../include/common.h"
#include "../include/debug.h"
#include "../include/vm.h"

VM vm;

void initVM(void)
{

}

void freeVM(void)
{

}

static InterpretResult run(void)
{
#define READ_BYTE() (*vm.ip++)
#define READ_CONSTANT() (vm.bytecode->constantPool.constants[READ_BYTE()])

    for (;;)
    {
#ifdef DEBUG_TRACE_EXECUTION
        disassembleInstruction(vm.bytecode, (int)(vm.ip - vm.bytecode->code));
#endif //DEBUG_TRACE_EXECUTION
        uint8_t instruction;
        switch (instruction = READ_BYTE())
        {
            case OP_CONSTANT:
            {
                Double constant = READ_CONSTANT();
                printValue(constant);
                printf("\n");
            }break;
            case OP_RETURN:
            {
                return INTERPRET_OK;
            }
        }
    }

#undef READ_BYTE
#undef READ_CONSTANT
}

InterpretResult interpret(Bytecode *bytecode)
{
    vm.bytecode = bytecode;
    vm.ip = vm.bytecode->code;
    return run();
}