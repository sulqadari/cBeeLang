#ifndef BEELANG_VALUE_H
    #define BEELANG_VALUE_H
    #include "common.h"

    /*
        Typedef abstracts actual value representation in C.
        That way, we can change that representation without
        needing to go back and fix existing code that passes
        around values.
    */
    typedef double Value;

    /// @brief Constant pool.
    // Each chunk of opcodes will carry with it a list of the values
    // that appear as literals in the program. This implementation
    // keeps all constants in there, even simple integers.
    typedef struct
    {
        int capacity;
        int count;
        Value* values;  // the double values array
    }ValueArray;

    void initValueArray(ValueArray* array);
    void writeValueArray(ValueArray* array, Value value);
    void freeValueArray(ValueArray* array);
    void printValue(Value value);
#endif //BEELANG_VALUE_H