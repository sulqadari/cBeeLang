#ifndef _H_BEELANG_DEBUG
#define _H_BEELANG_DEBUG

#include "bytecode.h"

/*
    The high-level function to disassemble bytecode.
    It implemented in terms of the other functions which just
    disassembles a single instruction.
*/
void disassembleBytecode(Bytecode *bytecode, const char *name);

/*
    The core debugging function called from within
    disassembleBytecode() function.
    @param Bytecode* instruction set
    @param offset offset of the bytecode to be disassembled.
    @returns int an offset to then successive instruction.
*/
int disassembleInstruction(Bytecode *bytecode, int offset);

#endif // _H_BEELANG_DEBUG