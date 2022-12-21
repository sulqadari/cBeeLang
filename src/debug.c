#include <stdio.h>
#include "../include/debug.h"
#include "../include/value.h"

void disassembleBytecode(Bytecode *bytecode, const char *name)
{
    printf("== %s ==\n", name); // print bytecode chunk's name

    for(int offset = 0; offset < bytecode->count; )
    {
        offset = disassembleInstruction(bytecode, offset);
    }
}

// static int lconstantInstruction(const char *name, Bytecode *bytecode, int offset)
// {
//     // fetch OP_CONSTANT_LONG's operand which resides at three subsequent bytes.
//     uint32_t constant = (uint16_t)((((uint16_t)bytecode->code[offset + 1]) << 16) |
//                                    (((uint16_t)bytecode->code[offset + 2]) <<  8) |
//                                      (uint16_t)bytecode->code[offset + 3]);

//     // print "OP_CONSTANT" and operand's value
//     printf("%-16s %4d '", name, constant);
//     // print the constant at index 'constant' in ConstantPool
//     printValue(bytecode->constantPool.constants[constant]);
//     return offset + 4;
// }

static int constantInstruction(const char *name, Bytecode *bytecode, int offset)
{
    // fetch OP_CONSTANT's operand which resides right after it.
    uint8_t constant = bytecode->code[offset + 1];
    // print "OP_CONSTANT" and operand's value
    printf("%-16s %4d '", name, constant);
    // print the constant at index 'constant' in ConstantPool
    printValue(bytecode->constantPool.constants[constant]);
    return offset + 2;
}

/*
    Handler function of simple instructions.
    The 'simple' instruction means a one-byte instruction.
*/
static int simpleInstruction(const char *name, int offset)
{
    printf("%s\n", name);   // print name of current instruction
    return offset + 1;
}

int disassembleInstruction(Bytecode *bytecode, int offset)
{
    printf("%04d ", offset);    // print current offset within bytecode array
    
    // if current value at lines[offset] equals previous one..
    if (0 < offset && bytecode->lines[offset] == bytecode->lines[offset - 1])
    {
        printf("    | ");   //..print this pipe as designation that current bytecode
                            // resides at the same line as the preceding one.
    }else
    {
        printf("%4d ", bytecode->lines[offset]);    //print new line's number
    }

    // retrieve bytecode under given offset
    uint8_t instruction = bytecode->code[offset];
    switch (instruction)
    {
        // case OP_CONSTANT_LONG:
        //     return lconstantInstruction("OP_CONSTANT_LONG", bytecode, offset);
        case OP_CONSTANT:
            return constantInstruction("OP_CONSTANT", bytecode, offset);
        case OP_NIL:
            return simpleInstruction("OP_NIL", offset);
        case OP_TRUE:
            return simpleInstruction("OP_TRUE", offset);
        case OP_FALSE:
            return simpleInstruction("OP_FALSE", offset);
        case OP_EQUAL:
            return simpleInstruction("OP_EQUAL", offset);
        case OP_GREATER:
            return simpleInstruction("OP_GREATER", offset);
        case OP_LESS:
            return simpleInstruction("OP_LESS", offset);
        case OP_ADD:
            return simpleInstruction("OP_ADD", offset);
        case OP_SUBTRACT:
            return simpleInstruction("OP_SUBTRACT", offset);
        case OP_MULTIPLY:
            return simpleInstruction("OP_MULTIPLY", offset);
        case OP_DIVIDE:
            return simpleInstruction("OP_DIVIDE", offset);
        case OP_NOT:
            return simpleInstruction("OP_NOT", offset);
        case OP_NEGATE:
            return simpleInstruction("OP_NEGATE", offset);
        case OP_RETURN:
            return simpleInstruction("OP_RETURN", offset);
        default:
            printf("Unknown opcode %d\n", instruction);
            return offset + 1;
    }
}