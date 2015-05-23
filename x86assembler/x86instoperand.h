#ifndef __X86_INST_OPERAND__
#define __X86_INST_OPERAND__

#include <ostream>

enum VarType
{
    BEGIN,
    BASE_OPERAND,
    MEMORY_OPERAND,
    IMMEDIATE,
    REGISTER,
    END,
};

enum DataType
{
    I32,
    U32,
    F32,
};

enum class X86Register
{
#define R(A, B) A = B,
#include "x86regs.h"
#undef R
};

#define setClassInstance(varType) \
    virtual VarType getClassInstance() { return varType; } \
    virtual bool    isInstanceOf(VarType type) { return varType == type; }

#define setClassInstance1(varType, parent) \
    virtual VarType getClassInstance() { return varType; } \
    virtual bool    isInstanceOf(VarType type) { return ((varType == type) ? true : parent::isInstanceOf(type)); }

#define setClassInstance2(varType, parent1, parent2) \
    virtual VarType getClassInstance() { return varType; } \
    virtual bool    isInstanceOf(VarType type) { return ((varType == type) ? true : (parent1::isInstanceOf(type) || parent2::isInstanceOf(type))); }

class BaseOperand
{
public:
    setClassInstance(VarType::BASE_OPERAND);

protected:
    virtual void print(std::ostream &out) = 0;

public:
    friend std::ostream& operator<<(std::ostream &str, BaseOperand& data)
    {
        data.print(str);
        return str;
    }
};

class ImmediateOperand : public BaseOperand
{
public:
    ImmediateOperand(unsigned int u32)
    {
        m_data.u32 = u32;
        m_dataType = U32;
    }

    ImmediateOperand(int i32)
    {
        m_data.i32 = i32;
        m_dataType = I32;
    }

    ImmediateOperand(float f32)
    {
        m_data.f32 = f32;
        m_dataType = F32;
    }

    setClassInstance1(VarType::IMMEDIATE, BaseOperand);

    friend std::ostream& operator<<(std::ostream& out, const ImmediateOperand& imm)
    {
        if (imm.m_dataType == I32)
            out << imm.m_data.i32;
        else if (imm.m_dataType == U32)
            out << imm.m_data.u32;
        else
            out << imm.m_data.f32;
        return out;
    }

protected:
    void print(std::ostream &out)
    {
        if (m_dataType == I32)
            out << m_data.i32;
        else if (m_dataType == U32)
            out << m_data.u32;
        else
            out << m_data.f32;
    }

private:
    union data
    {
        unsigned int u32;
        int          i32;
        float        f32;
    };

    data m_data;
    DataType m_dataType;

public:
    data getData() { return m_data; }

    template <typename M>
    M getValue()
    {
        if (m_dataType == I32)
            return m_data.i32;
        else if (m_dataType == F32)
            return m_data.f32;
        else
            return m_data.u32;
    }
};

class RegisterOperand : public BaseOperand
{
public:
    RegisterOperand(X86Register reg)
        : m_reg(reg)
    {}

    setClassInstance1(VarType::REGISTER, BaseOperand);

    friend std::ostream& operator<<(std::ostream& out, const RegisterOperand &reg)
    {
        // out << reg.m_regName.c_str();
        return out;
    }

    X86Register get() const { return m_reg; }

protected:
    void print(std::ostream& out)
    {
    }

private:
    X86Register m_reg;
};

class MemoryOperand : public RegisterOperand
{
public:
    MemoryOperand(X86Register reg, int location)
        : RegisterOperand(reg)
        , m_tempLocation(location)
    {}

    setClassInstance1(VarType::MEMORY_OPERAND, BaseOperand);

    friend std::ostream& operator<<(std::ostream& out, const MemoryOperand& var)
    {
        out << var.m_tempLocation;
        return out;
    }

    int getTempLocation() const { return m_tempLocation; }

protected:
    void print(std::ostream& out)
    {
        out << " Stack Spill Loc: " << m_tempLocation << "\n";
    }

private:
    int m_tempLocation;
};

#endif