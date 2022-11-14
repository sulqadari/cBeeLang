#include <stdio.h>
#include "../includes/memory.h"
#include "../includes/value.h"

void initValueArray(ValueArray* array)
{
    array->values = NULL;
    array->capacity = 0;
    array->count = 0;

}

void freeValueArray(ValueArray* array)
{
    FREE_ARRAY(Value, array->values, array->capacity);
    ///TODO: this freeValueArray() is called from freeChunk() which after
    // that calls initChunk() where the below method is called.
    // looks like this call is redundant.
    initValueArray(array);
}

void writeValueArray(ValueArray* array, Value value)
{
    if (array->capacity < array->count + 1)
    {
        int oldCapacity = array->capacity;
        array->capacity = GROW_CAPACITY(oldCapacity);
        array->values = GROW_ARRAY(Value, array->values, oldCapacity, array->capacity);
    }

    array->values[array->count] = value;
    array->count++;
}


void printValue(Value value)
{
    printf("%g", value);
}