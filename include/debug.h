#ifndef _H_BEELANG_DEBUG
#define _H_BEELANG_DEBUG

#include "bytecode.h"

/*
    The high-level function to disassemble bytecode.
    It implemented in terms of the other instruction-specific functions which
    disassemble a single instructions.
    @param Bytecode* a chunk of bytecode to be disassembled.
    @param char* a name of the bytecode chunk (for debugging purposes only).
*/
void disassembleBytecode(Bytecode *bytecode, const char *name);

/*
    The core debugging function called from within disassembleBytecode() function.
    This function in turn calls others static function which parse different
    bytecodes.
    @param Bytecode* instruction set
    @param offset offset of the bytecode to be disassembled.
    @returns int an offset to then successive instruction.
*/
int disassembleInstruction(Bytecode *bytecode, int offset);

#endif // _H_BEELANG_DEBUG