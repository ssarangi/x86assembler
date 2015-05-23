#ifndef __X86_INST__
#define __X86_INST__

#include "x86instoperand.h"

#define REG_DISP 3

enum D
{
    MEM = 1 << 1,
    REG = 0
};

enum S
{
    REG_32 = 1,
    REG_8_16 = 0
};

enum MOD
{
    SIB = 0,
    ONE_BYTE = 1 << 6,
    FOUR_BYTE = (1 << 1) << 6,
    REGISTER_ADDRESSING = ((1 << 1) | 1) << 6,
};

#include <vector>

typedef std::vector<unsigned char> bytestream;

#define X(INST_ENUM, INST_TYPE, OPCODE) struct x86##INST_ENUM { int opcode = OPCODE; };
#include "x86instset.h"
#undef X

class X86Assembler
{
public:
    X86Assembler() {}
    ~X86Assembler() {}

#define X(INST_ENUM, INST_TYPE, OPCODE) \
    template <typename ... OPERANDS> \
    void emit##INST_ENUM(OPERANDS... ops) { x86##INST_ENUM inst; return emit##INST_TYPE(inst, ops...); }
#include "x86instset.h"
#undef X

    bytestream& getByteStream() { return m_bytes; }

private:
    template <typename INST_ENUM_TYPE, typename... OPERANDS>
    void emitOneByteInst(INST_ENUM_TYPE inst, OPERANDS... ops)
    {
        assert(0 && "invalid type passed");
    }

    template <typename INST_ENUM_TYPE, typename... OPERANDS>
    void emitTwoByteInst(INST_ENUM_TYPE inst, OPERANDS... ops)
    {
        assert(0 && "invalid type passed");
    }

    template <typename INST_ENUM_TYPE>
    void emitOneByteInst(INST_ENUM_TYPE inst, RegisterOperand op1, RegisterOperand op2)
    {
        char byte1 = 0, byte2 = 0;

        byte1 = inst.opcode << 2;
        byte1 |= D::REG;
        byte1 |= S::REG_32;

        byte2 |= MOD::REGISTER_ADDRESSING;
        byte2 |= (char)op1.get() << REG_DISP;
        byte2 |= (char)op2.get();

        m_bytes.push_back(byte1);
        m_bytes.push_back(byte2);
    }

private:
    bytestream m_bytes;
};

#endif