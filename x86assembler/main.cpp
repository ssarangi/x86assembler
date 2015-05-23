#include <windows.h>
#include <stdio.h>

class Code {
    unsigned char* m_buf;
    int m_size;
    int m_idx;

    enum reg
    {
        ax = 0, cx, dx, bx, sp, bp, si, di
    };

    void init_sys();
    void generate(int param);
    bool put_byte(unsigned char b) {
        if (m_idx >= m_size) {
            return false;
        }
        m_buf[m_idx++] = b;
        return true;
    }
    bool put_int(int i)
    {
        if (m_idx >= m_size - 4)
        {
            return false;
        }

        *(int*)(m_buf + m_idx) = i;
        m_idx += 4;
        return true;
    }

    void pop(reg r) {
        put_byte(0x58 | r);
    }

    void push(reg r) {
        put_byte(0x50 | r);
    }

    void add(reg r, int imm8)

    {
        put_byte(0x83);
        put_byte(0xc0 | r);
        put_byte(imm8 & 0xff);
    }

    void jmp(reg r)
    {
        put_byte(0xff);
        put_byte(0xe0 | r);
    }

public:
    Code(int param);
    ~Code();
};

void Code::init_sys()
{
    SYSTEM_INFO si;
    GetSystemInfo(&si);
    m_size = si.dwPageSize;
}

Code::Code(int param) : m_buf(NULL), m_size(-1), m_idx(0)
{
    init_sys();
    m_buf = (unsigned char*)VirtualAlloc(NULL, m_size, MEM_COMMIT | MEM_RESERVE,
        PAGE_EXECUTE_READWRITE);
    generate(param);
}

Code::~Code()
{
    VirtualFree(m_buf, m_size, MEM_RELEASE);
    m_buf = NULL;
    m_size = 0;
}

void Code::generate(int param)
{
    add(ax, param);
    jmp(dx);
    FlushInstructionCache(GetCurrentProcess(), m_buf, m_size);
}

int __declspec(naked) invoke(Code* c, int p)
{
    __asm {
        pop edx
            mov ecx, [esp]
            mov eax, [esp + 4]
            jmp[ecx]
    }
}

int main(int argc, char* argv[])
{
    Code c1(5), c2(8);

    printf("got %d and %d\n", invoke(&c1, 4), invoke(&c2, 4));

    getchar();
    return 0;
}