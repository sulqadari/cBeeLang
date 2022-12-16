#include <stdarg.h>
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

static void runtimeError(const char *format, ...)
{
    va_list args;

    va_start(args, format);
    vfprintf(stderr, format, args);
    va_end(args);
    fputs("\n", stderr);

    size_t instruction = vm.ip - vm.bytecode->code - 1;
    int line = vm.bytecode->lines[instruction];
    fprintf(stderr, "[line %d] in script\n", line);
    resetStack();
}

void initVM(void)
{
    resetStack();
}

void freeVM(void)
{

}

void push(Value value)
{
    if ((vm.stackTop - vm.stack) >= (STACK_MAX * sizeof(Value)))
    {
        exit(STACK_OVERFLOW);
    }

    *vm.stackTop = value;
    vm.stackTop++;
}

Value pop(void)
{
    if (((int64_t)vm.stackTop - (int64_t)vm.stack) < 0)
    {
        exit(STACK_UNDERFLOW);
    }

    vm.stackTop--;
    return *vm.stackTop;
}

static Value peek(int idx)
{
    return vm.stackTop[-1 - idx];
}

static bool isFalsey(Value value)
{
    return IS_NIL(value) || (IS_BOOL(value) && !AS_BOOL(value));
}

static InterpretResult run(void)
{
#define READ_BYTE() (*vm.ip++)
#define READ_CONSTANT() (vm.bytecode->constantPool.constants[READ_BYTE()])
#define BINARY_OP(valueType, op) \
    do { \
        if (!IS_NUMBER(peek(0)) || !IS_NUMBER(peek(1))) { \
            runtimeError("Operands must be numbers."); \
            return INTERPRET_RUNTIME_ERROR; \
        } \
        double b = AS_NUMBER(pop()); \
        double a = AS_NUMBER(pop()); \
        push(valueType(a op b)); \
    } while (false)

    for (;;)
    {
#ifdef DEBUG_TRACE_VM
        // stack trace start
        printf("          ");
        for (Value* slot = vm.stack; slot < vm.stackTop; slot++)
        {
            printf("[ ");
            printValue(*slot);
            printf(" ]");
        }
        printf("\n");
        // stack trace end

        disassembleInstruction(vm.bytecode, (int)(vm.ip - vm.bytecode->code));
#endif //DEBUG_TRACE_VM

        uint8_t instruction;
        switch (instruction = READ_BYTE())
        {
            case OP_CONSTANT:
            {
                Value constant = READ_CONSTANT();
                push(constant);
            }break;
            case OP_NIL: push(NIL_VAL);                     break;
            case OP_TRUE: push(BOOL_VAL(true));             break;
            case OP_FALSE: push(BOOL_VAL(false));           break;
            case OP_EQUAL:
                Value b = pop();
                Value a = pop();
                push(BOOL_VAL(valuesEqual(a, b)));
            break;
            case OP_GREATER: BINARY_OP(BOOL_VAL, >);        break;
            case OP_LESS: BINARY_OP(BOOL_VAL, <);           break;
            case OP_ADD:BINARY_OP(NUMBER_VAL, +);           break;
            case OP_SUBTRACT: BINARY_OP(NUMBER_VAL, -);     break;
            case OP_MULTIPLY: BINARY_OP(NUMBER_VAL, *);     break;
            case OP_DIVIDE: BINARY_OP(NUMBER_VAL, /);       break;
            case OP_NOT: push(BOOL_VAL(isFalsey(pop())));   break;
            case OP_NEGATE:
            {
                if (!IS_NUMBER(peek(0)))
                {
                    runtimeError("Operand must be a number.");
                    return INTERPRET_RUNTIME_ERROR;
                }
                //push(-pop());
                push(NUMBER_VAL(-AS_NUMBER(pop())));
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