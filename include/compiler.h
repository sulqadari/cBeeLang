#ifndef _H_BEELANG_COMPILER
#define _H_BEELANG_COMPILER

/*
  -= compiler.h =-
  Transforms a human-readable script into VM-specific bytecode.
*/
void compile(const char *source);

#endif // _H_BEELANG_COMPILER