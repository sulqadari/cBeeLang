#ifndef BEELANG_VALUE_H
    #define BEELANG_VALUE_H
    #include "common.h"

    /**
     * @brief This type definition abstracts actual value representation in C.
     * That way, we can change that representation without
     * needing to go back and fix existing code that passes
     * around values.
     */
    typedef double Value;

    /**
     * @brief Some sort of storage, similar to 'constant pool' in JVM.
     * Each chunk of opcodes carries with it a list of the values
     * that appear as literals in the program.
     * This implementation keeps all constants in there, even simple integers.
     * Which means there is no place for 'immediate instructions' to appear in code.
     */
    typedef struct
    {
        int capacity;   // available space in the array.
        int count;      // total quantity of elements are actually in use.
        Value* values;  // the double values array
    }ValueArray;

    /**
     * @brief Initializes ValueArray.
     * @param array pointer to ValueArray structure
     */
    void initValueArray(ValueArray* array);
    
    /**
     * @brief Appends a Value to the end of ValueArray
     * 
     * @param array pointer to ValueArray structure
     * @param value Value to be stored in ValueArray
     */
    void writeValueArray(ValueArray* array, Value value);
    
    /**
     * @brief Releases all of the memory allocated for
     * dynamic array of ValueArray structure.
     * 
     * @param array pointer to ValueArray structure
     */
    void freeValueArray(ValueArray* array);

    void printValue(Value value);
#endif //BEELANG_VALUE_H