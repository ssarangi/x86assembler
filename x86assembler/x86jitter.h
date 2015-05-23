#ifndef __X86_JITTER__
#define __X86_JITTER__

#include <windows.h>

class X86Assembler;

class X86Jitter
{
public:
    X86Jitter()
        : m_pBuf(nullptr)
    {
        init_sys();
    }

    ~X86Jitter()
    {
    }

    void jit(X86Assembler *pAssembler);

private:
    void init_sys()
    {
        SYSTEM_INFO sysinfo;
        GetSystemInfo(&sysinfo);
        m_size = sysinfo.dwPageSize;
    }

private:
    // Do not rearrange the members. This is dependent on the order
    unsigned char *m_pBuf;
    int m_size;
};

#endif