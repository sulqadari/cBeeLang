#ifndef BEELANG_VM_H
    #define BEELANG_VM_H
    #include "chunk.h"
    #include "value.h"

    #define STACK_MAX 256

    typedef struct
    {
        Chunk* chunk;   // chunk of bytecode to be executed.
        uint8_t* ip;    // pointer to the next instruction.
        Value stack[STACK_MAX];
        Value* stackTop;    // points to the top of the stack
    }VM;

    /**
     * @brief Response types.
     * These status codes will be used to define runtime 
     * and compile-time errors. 
     */
    typedef enum
    {
        INTERPRET_OK,
        INTERPRET_COMPILE_ERROR,
        INTERPRET_RUNTIME_ERROR
    }InterpretResult;

    /**
     * @brief Creates VM
     * 
     */
    void initVM();

    /**
     * @brief Tears VM down.
     * 
     */
    void freeVM();

    /**
     * @brief Main entry point into the VM.
     * 
     * @param source the source code of the script
     * @return InterpretResult 
     */
    InterpretResult interpret(const char* source);

    /**
     * @brief stores the given element at the top of the stack
     * 
     * @param value 
     */
    void push(Value value);
    
    /**
     * @brief Pops the value from the top of the stack
     * 
     * @return Value 
     */
    Value pop();

#endif //BEELANG_VM_H