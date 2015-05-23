#include <windows.h>
#include "x86instoperand.h"
#include "x86assembler.h"
#include "x86jitter.h"
#include <stdio.h>

int main(int argc, char* argv[])
{
    // Code c1(5);

    X86Assembler assembler;
    RegisterOperand r1(X86Register::eax);
    RegisterOperand r2(X86Register::ecx);
    assembler.emitadd(r1, r2);

    X86Jitter jitter;
    jitter.jit(&assembler);

    getchar();

    return 0;
}