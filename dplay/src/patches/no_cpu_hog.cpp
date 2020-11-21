#include "patches.hpp"

#include "util.hpp"

#define PATCH_NAME no_cpu_hog
DESCRIBE_PATCH(R"txt(Prevents the game from hogging the CPU.)txt")

namespace {

//
// Hooks
//

void sleep_patch()
{
    // The sleep is the purpose of the patch, the rest is to restore control flow
    Sleep(1);

    // 484CC3h is the address of the top of the main loop
    // The effectively replaces the jump we displaced
#ifdef _MSC_VER
    __asm {
        mov eax, 484CC3h
        jmp eax
    };
#else
    asm (".intel_syntax noprefix\n\t"
        "mov     eax, 484CC3h\n\t"
        "jmp     eax\n\t"
    ".att_syntax noprefix\n\t");
#endif
}

}

PATCH_MAIN
{
    bool ok = true;
    // Displace the jump to the top of the loop
    ok &= patch_jmp(00484E53, sleep_patch);
    return ok;
}
