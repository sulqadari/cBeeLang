#include <stdlib.h>
#include "../includes/memory.h"

void* reallocate(void* pointer, size_t oldSize, size_t newSize)
{
    // deallocate if newSize is zero
    if (0 == newSize)
    {
        free(pointer);
        return NULL;
    }

    void* result = realloc(pointer, newSize);
    
    if (NULL == result)
        exit(1);
    
    return result;
}