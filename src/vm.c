#include <stdio.h>
#include "../includes/common.h"
#include "../includes/vm.h"
#include "../includes/debug.h"
#include "../includes/compiler.h"


/**
 * @brief Current virtual machine.
 * Instead of passing pointer to VM around functions,
 * the global instance is declared to keep code clearer.
 */
VM vm;

void initVM()
{
    resetStack();
}

void freeVM()
{

}

InterpretResult interpret(const char* source)
{
    compile(source);
    return INTERPRET_OK;
}

void push(Value value)
{
    *vm.stackTop = value;   // dereference stackTop and assign it with value
    vm.stackTop++;          // advance stackTop thus it would point to the next index in stack
}

Value pop()
{
    vm.stackTop--;
    return *vm.stackTop;
}

static InterpretResult run()
{
    #define READ_BYTE() (*vm.ip++)  // advance instruction pointer
    //lookup the corresponding Value in the chunk's constant pool
    #define READ_CONSTANT() (vm.chunk->constants.values[READ_BYTE()])

    #define BINARY_OP(op) \
        do { \
            double b = pop(); \
            double a = pop(); \
            push(a op b); \
        }while(false)

    uint8_t instruction;
    for(;;)
    {
        #ifdef DEBUG_TRACE_EXECUTION
            printf("          ");

            for(Value* slot = vm.stack; slot < vm.stackTop; slot++)
            {
                printf("[ ");
                printValue(*slot);
                printf(" ]");
            }
            printf("\n");

            // vm.ip is incremented each time it goes through this loop.  
            // convert vm.ip back to a relative offset from the beggining of the bytecode.
            // using pointer math: current pointer - initial pointer
            disassembleInstruction(vm.chunk, (int)(vm.ip - vm.chunk->code));
        #endif

        // instruction decoding:
        // the body of each case implements opcode's behavior
        switch (instruction = READ_BYTE())
        {
            case OP_CONSTANT:
                Value constant = READ_CONSTANT();   // read opcode which follows OP_CONSTANT
                push(constant);
            break;
            case OP_ADD:        BINARY_OP(+);   break;
            case OP_SUBTRACT:   BINARY_OP(-);   break;
            case OP_MULTIPLY:   BINARY_OP(*);   break;
            case OP_DIVIDE:     BINARY_OP(/);   break;
            // pop the value from the stack and push negated value
            // back on the stack for later instructions to use.
            case OP_NEGATE:     push(-pop());   break;
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

static void resetStack()
{
    // set stackTop to point to the beggining of the array to indicate
    // that the stack si empty.
    vm.stackTop = vm.stack;
}