#ifndef BEELANG_DEBUG_H
    #define BEELANG_DEBUG_H
    #include "chunk.h"

    /// @brief disassembles all of the instructions in the entire chunk.
    /// @param chunk 
    /// @param name 
    void disassembleChunk(Chunk* chunk, const char* name);

    /// @brief Core function of disassembling module.
    /// @param chunk 
    /// @param offset 
    /// @return 
    int disassembleInstruction(Chunk* chunk, int offset);

#endif //BEELANG_DEBUG_H

