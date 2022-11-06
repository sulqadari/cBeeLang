#ifndef BEELANG_CHUNK_H
    #define BEELANG_CHUNK_H

    #include "common.h"
    #include "value.h"

    
    /// @brief Each instruction has a one-byte operation code (opcode)
    /// which designates what kind of instruction we're dealing with -
    /// add, subtract, look up variable, etc.
    typedef enum
    {
        OP_CONSTANT,    // produce a particular constant
        OP_RETURN,      // return from the current function
    } OpCode;

    /// @brief Dynamic array to store a portion of instructions and associated
    /// metadata.
    /// If the count is less that the capacity, a new element is stored
    /// at the end of array.
    /// If there is no spare capacity, then:
    /// 1. allocate a new array with the size two times bigger than previous
    /// 2. Update the value of capacity variable.
    /// 3. copy elements from the old array to the new one.
    /// 4. delete the old array.
    /// 5. update the code pointer to point to the new array.
    /// 6. store the new element in the new array.
    /// 7. update count variable.
    typedef struct
    {
        int count;      // total quantity of elements are actually in use.
        int capacity;   // number of elements in the array we have allocated.
        uint8_t* code;  // dynamic array which stores a chunk of bytes.
        ValueArray constants;   // Chunk's constants pool
    }Chunk;

    /// @brief initializes a given dynamic array to keep instructions set
    /// @param chunk 
    void initChunk(Chunk* chunk);

    /// @brief Frees chunk
    /// @param chunk 
    void freeChunk(Chunk* chunk);

    /// @brief Appends instruction byte to the end of the chunk
    /// @param chunk dynamic array
    /// @param byte kind of instruction
    void writeChunk(Chunk* chunk, uint8_t byte);

    /// @brief Writes the constant into chunk's constant pool.
    /// @param chunk 
    /// @param value 
    /// @return the index where the constant was appended.
    int addConstant(Chunk* chunk, Value value);

#endif // BEELANG_CHUNK_H