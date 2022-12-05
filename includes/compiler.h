#ifndef BEELANG_COMPILER_H
    #define BEELANG_COMPILER_H
    #include "common.h"
    #include "scanner.h"
    #include "vm.h"
    #include "debug.h"

    /**
     * @brief Transforms the source code into bytecode and stores
     * the result in a given Chunk.
     * 
     * @param source the string of source code
     * @param chunk pointer to the chunk of bytecode where compiler stores instructions.
     * @return true if non compile time error have occured. False otherwise
     */
    bool compile(const char* source, Chunk* chunk);
    
#endif //BEELANG_COMPILER_H