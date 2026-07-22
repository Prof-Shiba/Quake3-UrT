#include <Windows.h>
#include <iostream>

// mov dword ptr ds:[0x0102AE98], ebx
DWORD ReturnAddress = 0x52D2FD;

__declspec(naked) void CodeCave() {
    //
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

