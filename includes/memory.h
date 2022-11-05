#ifndef BEELANG_MEMORY_H
    #define BEELANG_MEMORY_H

    #include "common.h"

    ///Calculates a new capacity based on a given current capacity.
    #define GROW_CAPACITY(capacity) ( (capacity) < 8 ? 8 : (capacity) * 2 )

    #define GROW_ARRAY(type, pointer, oldCount, newCount) \
                        (type*)reallocate(pointer, \
                                        sizeof(type) * (oldCount), \
                                        sizeof(type) * (newCount))

    /// Frees the memory by passing in zero for the newSize
    #define FREE_ARRAY(type, pointer, oldCount) \
                        reallocate(pointer, sizeof(type) * (oldCount), 0)

    /// @brief Will be used for all dynamic memory managmeent: allocating,
    /// freeing and changing size of an existing allocation.
    /// Routing all of those operations through a single function is important
    /// for garbage collector that needs to keep track of how much memory
    /// is in use.
    /// If oldsize == 0 and newSize > 0: allocate new block;
    /// If oldsize > 0 and newSize == 0: Free allocation;
    /// If 0 < oldsize < newSize : allocate new block;
    /// If 0 < oldsize > newSize : grow existing allocation;
    /// @param pointer dynamic array
    /// @param oldSize old size
    /// @param newSize new size
    /// @return 
    void* reallocate(void* pointer, size_t oldSize, size_t newSize);

#endif // BEELANG_MEMORY_H