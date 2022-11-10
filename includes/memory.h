#ifndef BEELANG_MEMORY_H
    #define BEELANG_MEMORY_H

    #include "common.h"

    /**
     * @brief Calculates a new capacity based on a given current capacity.
     * @param capacity actual size of a chunk
     */
    #define GROW_CAPACITY(capacity) ( ((capacity) < 8) ? 8 : ((capacity) * 2) )

    /**
     * @brief Create/grow array to the given size.
     * This macro takes care of getting the size of the array's elements
     * and casting the size of the array's element type and casting the
     * resulting void* back to a pointer of the right type.
     * @param type array's element type
     * @param pointer a pointer to dynamic array of Chunk structure
     * @param oldCount old size of capacity
     * @param newCount new size of capacity
     * @returns casted resulted void* to a pointer of the right type 
     */
    #define GROW_ARRAY(type, pointer, oldCount, newCount) \
                        (type*)reallocate(pointer, \
                                        sizeof(type) * (oldCount), \
                                        sizeof(type) * (newCount))

    /// @brief 
    
    /**
     * @brief Frees the memory of the Chunk's array by implicitly passing in
     * zero for the newSize parameter of the reallocate() function.
     * @param type array's element type
     * @param pointer a pointer to dynamic array of Chunk structure
     * @param oldCount old size of capacity
     */
    #define FREE_ARRAY(type, pointer, oldCount) \
                        reallocate(pointer, sizeof(type) * (oldCount), 0)

    
     
     

    /**
     * @brief This function is used for all dynamic memory management:
     * allocating, freeing and changing size of an existing allocation.
     * Routing all of those operations through a single function is important
     * for garbage collector that needs to keep track of how much memory
     * is in use.
     * case 1: oldsize == 0 & newSize > 0: allocate new block (behaves exactly as malloc());
     * case 2: oldsize > 0 & newSize == 0: free allocation;
     * case 3: 0 > oldsize > newSize:      shrink existing allocation;
     * case 4: 0 > oldsize < newSize:      grow existing allocation;
     * Except the case 2, all others rely on the C standard library's realloc() function.
     * @param pointer a pointer to dynamic array of Chunk structure
     * @param oldSize old size of capacity
     * @param newSize new size of capacity
     * @return void* pointer to an array with updated capacity (mostly the same one)
     */
    void* reallocate(void* pointer, size_t oldSize, size_t newSize);

#endif // BEELANG_MEMORY_H