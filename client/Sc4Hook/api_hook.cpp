#include "windows.h"
#include <cstddef>
#include <stdint.h>


void hook(const char* module, const char* proc, unsigned& sysCallID, BYTE*& pProc, const void* pNewProc)
{
    // See <http://www.fengyuan.com/article/wmprint.html> for an explanation of
    // how this function works.

    HINSTANCE hMod = GetModuleHandleA(module);

    pProc = reinterpret_cast<BYTE*>(reinterpret_cast<ptrdiff_t>(GetProcAddress(hMod, proc)));

    if(pProc == NULL)
    {
        MessageBoxA(NULL,"Hook failed,entry point not found",0,0);
        return;
    }

#if defined(GCC) || defined(_M_IX86)
    if (pProc[0] != 0xB8){
        MessageBoxA(NULL,"Hook failed, incorect start byte",0,0);
        return;
    }

    // FIXME: Should we be reading the bytes one-by-one instead of doing an
    // unaligned read?
    sysCallID = *reinterpret_cast<unsigned*>(pProc + 1);

    DWORD flOldProtect;
    if (!VirtualProtect(pProc, 5, PAGE_EXECUTE_READWRITE, &flOldProtect))
        return;

    pProc[0] = 0xE9;
    *reinterpret_cast<unsigned*>(pProc + 1) = reinterpret_cast<intptr_t>(pNewProc) - reinterpret_cast<intptr_t>(pProc + 5);

    pProc += 5;
#else
    /* Disassembly of BeginPaint()
    00000000779FC5B0 4C 8B D1         mov         r10,rcxs
    00000000779FC5B3 B8 17 10 00 00   mov         eax,1017h
    00000000779FC5B8 0F 05            syscall
    00000000779FC5BA C3               ret
    00000000779FC5BB 90               nop
    00000000779FC5BC 90               nop
    00000000779FC5BD 90               nop
    00000000779FC5BE 90               nop
    00000000779FC5BF 90               nop
    00000000779FC5C0 90               nop
    00000000779FC5C1 90               nop
    00000000779FC5C2 90               nop
    00000000779FC5C3 90               nop
    */
    // Check for the signature as in the above disassembly
   // DWORD guard = 0xB8D18B4C;
    //if (*reinterpret_cast<DWORD*>(pProc) != guard)
   //     return;

    DWORD flOldProtect;
    VirtualProtect(pProc, 12, PAGE_EXECUTE_READWRITE, & flOldProtect);
    pProc[0] = 0x48;    // mov rax, this
    pProc[1] = 0xb8;
    *(__int64*)(pProc+2) = (__int64)pNewProc;
    pProc[10] = 0xff;   // jmp rax
    pProc[11] = 0xe0;
#endif
}
