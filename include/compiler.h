#ifndef _H_BEELANG_COMPILER
#define _H_BEELANG_COMPILER

#include "vm.h"

/**
  -= compiler.h =-
  Transforms a human-readable script into VM-specific bytecode.

  @param char* source code
  @param Bytecode* an entity to store produced bytecode
*/
bool compile(const char *source, Bytecode *bytecode);

#endif // _H_BEELANG_COMPILER