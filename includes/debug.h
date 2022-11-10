#ifndef BEELANG_DEBUG_H
    #define BEELANG_DEBUG_H
    #include "chunk.h"

    /// @brief d
    /// @param chunk 
    /// @param name 

    /**
     * @brief Disassembles all of the instructions in the entire chunk.
     * 
     * @param chunk a pointer to Chunk structure
     * @param name a human-readable header of chunk under examination
     */
    void disassembleChunk(Chunk* chunk, const char* name);

    /**
     * @brief Core function of disassembling module.
     * disassembles each successive bytecode.
     * @param chunk  a pointer to Chunk structure
     * @param offset  opcode's offset within chunk
     * @return int offset of the next subsequent bytecode
     */
    int disassembleInstruction(Chunk* chunk, int offset);

    // =========================================================== //
    // ================ opcode-specific funcitons ================ //
    // =========================================================== //
    
    static int constantInstruction(const char* name, Chunk* chunk, int offset);
    static int simpleInstruction(const char* name, int offset);
#endif //BEELANG_DEBUG_H

