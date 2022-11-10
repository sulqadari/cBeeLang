#ifndef BEELANG_CHUNK_H
    #define BEELANG_CHUNK_H

    #include "common.h"
    #include "value.h"

    /**
     * @brief Enumeration of supported opcodes.
     * In bytecode format under implementation, each instruction has one-byte opcode
     * followed by zero and more operands: [opcode, opcode, operand, opcode].
     * Each opcode determines how many operand bytes it has and what they mean.
     * Opcode's operands specify instruction format.
     */
    typedef enum
    {
        OP_CONSTANT,    // load a constant for use: [load, idx]
        OP_CONSTANT_W,  // load a constant for use: [load, idx_w]
        OP_RETURN,      // return from the current function: [return]
    } OpCode;

    /**
     * @brief Chunk - a sequence of bytecode. This structure
     * represents a dynamic array of instructions.
     * 
     */
    typedef struct
    {
        int count;      // total quantity of elements are actually in use.
        int capacity;   // number of elements in the array we have allocated.
        uint8_t* code;  // dynamic array which stores a chunk of bytes 
        int* lines;     // keeps track of line numbers of the offending source code (exactly parallels the bytecode array).
        ValueArray constants;   // Chunk's constant pool
    }Chunk;

    /**
     * @brief Initializes Chunk.
     * The dynamic array starts off completely empty.
     * @param chunk pointer to Chunk structure
     */
    void initChunk(Chunk* chunk);

    /**
     * @brief Appends instruciton to the end of the chunk.
     * 
     * @param chunk pointer to Chunk structure
     * @param byte bytecode
     * @param line the tracker of the line at which an error have occured.
     */
    void writeChunk(Chunk* chunk, uint8_t byte, int line);

    /**
     * @brief Releases all of the memory allocated for dynamic array of Chunk structure.
     * 
     * @param chunk pointer to Chunk structure
     */
    void freeChunk(Chunk* chunk);

    /**
     * @brief convenience function to add a new constant to the chunk.
     * This method calls writeValueArray() which in turn stores a given value
     * in Chunk's ValueArray.
     * @param chunk pointer to target chunk
     * @param value value to be stored in chunk's ValueArray
     * @return int index within ValuesArray where the constant was appended.
     */
    int addConstant(Chunk* chunk, Value value);

#endif // BEELANG_CHUNK_H