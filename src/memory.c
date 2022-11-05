#include <stdlib.h>
#include "../includes/memory.h"

void* reallocate(void* pointer, size_t oldSize, size_t newSize) {
    // deallocate if newSize is zero
    if (0 == newSize) {
        free(pointer);
        return NULL;
    }

    // When oldSize is zero, realloca() is equivalent to calling malloc().
    // If both old and new aren't zero, those tell realloca() to resize the
    // previously allocated block.
    // If the new size is smaller than the existing block of memory, it simply
    // updates size of the block and returns the same pointer you gave it.
    // If there isn't room to grow the block in memory, realloc() allocates a new block
    // of memory of the desired size, copies over the old bytes, frees the old block,
    // and then returns a pointer to the new block.
    void* result = realloc(pointer, newSize);
    if (NULL == result) exit(1);
    
    return result;
}