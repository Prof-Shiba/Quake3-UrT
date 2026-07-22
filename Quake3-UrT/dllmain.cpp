#include <Windows.h>
#include <iostream>

// this works by disabling depth testing. this also results in the game being basically unplayable
// as your viewmodel becomes visible and blocks most of the screen, but bots/other players will appear
// through walls, as well as weapons and some other entities.

// TODO: fix the shitty viewmodel issue by enabling depth testing for the localplayer.

// mov dword ptr ds:[0x0102AE98], ebx
DWORD ReturnAddress = 0x0052D303;

// opcodes
DWORD o_jmp = 0xE9;
DWORD o_nop = 0x90;

__declspec(naked) void CodeCave() {
    __asm {
        pushad
        mov dword ptr ds:[ebx + 4], 0xD
        popad
        mov dword ptr ds:[0x102AE98], ebx

        jmp ReturnAddress
    }
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH: {
        DWORD OldProtect;
        unsigned char* HookLocation = reinterpret_cast<unsigned char*>(0x0052D2FD);

        if (VirtualProtect((LPVOID)HookLocation, 5, PAGE_EXECUTE_READWRITE, &OldProtect) == 0) {
            exit(1337);
        }

        *HookLocation = o_jmp;

        // new_location - original_location + 5
        // this calculation creates an offset to the *end* of a new region of bytes
        // we must then add + 5 because the EIP will be pointing to the end of the bytes we started at so we realign
        // ex: 0x1000 -> 0x1005 when EIP is done reading the 5 bytes, then it adds the offset to EIP.
        *(DWORD*)(HookLocation + 1) = (DWORD)&CodeCave - ((DWORD)HookLocation + 5);
        *(HookLocation + 5) = o_nop;
    }
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

