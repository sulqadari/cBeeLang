#ifndef BEELANG_MEMORY_H
    #define BEELANG_MEMORY_H

    #include "common.h"

    /// @brief Calculates a new capacity based on a given current capacity.
    #define GROW_CAPACITY(capacity) ( ((capacity) < 8) ? 8 : ((capacity) * 2) )

    /// @brief create/grow array to the given size.
    /// This macro takes care of getting the size of the array's elements
    /// and casting the size of the array's element type and casting the
    /// resulting void* back to a pointer of the right type.
    #define GROW_ARRAY(type, pointer, oldCount, newCount) \
                        (type*)reallocate(pointer, \
                                        sizeof(type) * (oldCount), \
                                        sizeof(type) * (newCount))

    /// @brief Frees the memory by passing in zero for the newSize.
    
    #define FREE_ARRAY(type, pointer, oldCount) \
                        reallocate(pointer, sizeof(type) * (oldCount), 0)

    /// @brief Will be used for all dynamic memory managmeent: allocating,
    /// freeing and changing size of an existing allocation.
    /// Routing all of those operations through a single function is important
    /// for garbage collector that needs to keep track of how much memory
    /// is in use.
    /// * oldsize == 0 & newSize > 0: allocate new block;
    /// * oldsize > 0 & newSize == 0: free allocation;
    /// * 0 > oldsize > newSize:      shrink existing allocation;
    /// * 0 > oldsize < newSize:      grow existing allocation;
    /// Except the first case, all others rely on the C standard library's realloc() function.
    /// 
    /// @param pointer dynamic array
    /// @param oldSize old size
    /// @param newSize new size
    /// @return 
    void* reallocate(void* pointer, size_t oldSize, size_t newSize);

#endif // BEELANG_MEMORY_H