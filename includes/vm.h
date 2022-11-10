#ifndef BEELANG_VM_H
    #define BEELANG_VM_H
    #include "chunk.h"

    typedef struct {
        Chunk* chunk;   // chunk of bytecode to be executed.
    }VM;

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

#endif //BEELANG_VM_H