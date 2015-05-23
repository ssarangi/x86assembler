#include "x86jitter.h"
#include "x86assembler.h"

int __declspec(naked) invoke(X86Jitter* jitter)
{
    __asm {
        pop edx
        mov ecx, [esp]
        mov eax, [esp + 4]
        jmp[ecx]
    }
}

void X86Jitter::jit(X86Assembler *pAssembler)
{
    // First create some executable memory.
    m_pBuf = (unsigned char *)VirtualAlloc(NULL, m_size, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);

    // Set the assembly generated so far
    unsigned int m_idx = 0;
    for (unsigned char b : pAssembler->getByteStream())
        m_pBuf[m_idx++] = b;

    // Jump to the original location
    // Emulating a jump to the original location
    m_pBuf[m_idx++] = 0xff;
    m_pBuf[m_idx++] = 0xe0 | 2;
    // jmp(dx);
    
    FlushInstructionCache(GetCurrentProcess(), m_pBuf, m_size);

    invoke(this);

    VirtualFree(m_pBuf, m_size, MEM_RELEASE);
}