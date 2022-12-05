#include <stdlib.h>
#include "../include/memory.h"

void* reallocate(void* pointer, size_t oldSize, size_t newSize)
{
    if (0 == newSize)   // deallocate memory if true
    {
        free(pointer);
        return NULL;
    }

    /*
        When oldSize == 0, behavior of realloc is equivalent to calling malloc().
        realloc() retrieves presize size of 'oldValue' implicitly by its own.
        if (oldSize > newSize): reduce the size
        if (oldSize < newSize): increase the size
    */
    void* result = realloc(pointer, newSize);
    if (NULL == result)
    {
        exit(1);
    }
    
    return result;
}