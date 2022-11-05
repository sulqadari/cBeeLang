#ifndef BEELANG_CHUNK_H
    #define BEELANG_CHUNK_H

    #include <common.h>

    typedef enum {
        OP_RETURN
    } OpCode;

    /// @brief stores a chunk o bytecode.
    /// This structure represents a dynamic array of instructions.
    typedef struct {
        int count;      // total elements in array
        int capacity;   // array's capacity
        uint8_t* code;
    }Chunk;

    /// @brief initializes given dynamic array to keep instructions set
    /// @param chunk 
    void initChunk(Chunk* chunk);

    /// @brief Frees chunk
    /// @param chunk 
    void freeChunk(Chunk* chunk);

    /// @brief Appends instruction to the end of the chunk
    /// @param chunk 
    /// @param byte 
    void writeChunk(Chunk* chunk, uint8_t byte);

#endif // BEELANG_CHUNK_H