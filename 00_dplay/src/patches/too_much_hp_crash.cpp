#include "patches.hpp"

#include "util.hpp"
#include "variables.hpp"

namespace {

void validate_eax()
{
    // Depends on eax being the raw output of (hpmax / hp) * 80
    // Wow this GCC asm syntax sucks... give me MSVC __asm
    asm (".intel_syntax noprefix\n\t"
        "cmp     eax, 80\n\t"
        "jg      over_max_fix\n\t"
        "test    eax, eax\n\t"
        "jl      under_min_fix\n\t"
        "jmp     ret\n\t"
    "over_max_fix:\n\t"
        "mov     eax, 80\n\t"
        "jmp     ret\n\t"
    "under_min_fix:\n\t"
        "xor     eax, eax\n\t"
        "jmp     ret\n\t"
    "ret:\n\t"
        "nop\n\t"
    ".att_syntax noprefix\n\t");
}

} // namespace

void too_much_hp_crash_main()
{
    bool ok = true;

    // This is the data segment, no need to adjust permissions
    *(uint32_t*)0x004BC168 = TRUE;

    ok &= nop(0x00410348, 0x00410354); // displaces 3 instructions but we only need the one (the mov_eax_dv patch we apply later)
    ok &= patch_call(0x00410348, (void*)validate_eax);
    constexpr uint8_t move_eax_dv[] = {0x89, 0x45, 0xFC};
    ok &= patch_bytes(0x00410348 + 5, move_eax_dv, sizeof(move_eax_dv));

    printf("%s %s\n", __func__, ok ? "success" : "fail");
}
