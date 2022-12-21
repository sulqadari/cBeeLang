#ifndef _H_BEELANG_MEMORY
#define _H_BEELANG_MEMORY

#include "common.h"

/*
    -= memory.h =-
    Calculates a new capacity based on a given current capacity
*/
#define INCREASE_CAPACITY(capacity) ((capacity) < 8 ? 8 : (capacity) * 2)

/*
    -= memory.h =-
    Computes the size of the array's element type and casting resulting *void
    back to a pointer of the right type and passes all these params to reallocate()
    function.
*/
#define INCREASE_ARRAY(type, pointer, oldCount, newCount) \
        (type*)reallocate(pointer, sizeof(type) * (oldCount), \
        sizeof(type) * (newCount))

/*
    -= memory.h =-
    Deallocates memory.
    @param type the type of array's elements
    @param pointer an array to be freed.
    @param oldCount the current length of array. Zero-value argument as the
    second argument to realloc() function which is passed implicitly
    leads to expected result.
*/
#define FREE_ARRAY(type, pointer, oldCount) \
        reallocate(pointer, sizeof(type) * (oldCount), 0)

/*
    -= memory.h =-
    Handles dynamic memory management:
    - allocation;
    - deallocation;
    - resizing.

    @param void* pointer to an array to be resized.
    @param size_t oldSize of pointer. If == '0' then this functions allocates new block.
                  Otherwise depends on 'newSize'.
    @param size_t newSize. If == '0' then this function frees allocation.
                  If 'oldSize' == 0 then this function allocates new block.
                  If < 'oldSize' - shrink existing allocation.
                  If > 'oldSize' - increase existing allocation.
    @returns void* casted pointer to array.
*/
void* reallocate(void* pointer, size_t oldSize, size_t newSize);

#endif // _H_BEELANG_MEMORY