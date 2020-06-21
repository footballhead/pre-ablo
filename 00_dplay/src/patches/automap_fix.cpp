#include "patches.hpp"

#include "util.hpp"

namespace {

// This was produced with Ghidra.
// It's easier to just copy this then try to rewrite the patch in C
constexpr uint8_t InitAutomap_patch[] = {
    0xa1, 0x48, 0x36, 0x4e, 0x00, // MOV        EAX,[leveltype]
    0x83, 0xf8, 0x01,             // CMP        EAX,0x1
    0x74, 0x2f,                   // JZ         cathedral
    0x83, 0xf8, 0x02,             // CMP        EAX,0x2
    0x74, 0x4b,                   // JZ         catacombs
    0x83, 0xf8, 0x03,             // CMP        EAX,0x3
    0x74, 0x67,                   // JZ         caves
    0x83, 0xf8, 0x04,             // CMP        EAX,0x4
    0x74, 0x41,                   // JZ         catacombs
    0x83, 0xf8, 0x05,             // CMP        EAX,0x5
    0x74, 0x1b,                   // JZ         cathedral
    0x66, 0xe9, 0xdf, 0x01,       // JMP        end_of_function
};

constexpr uint8_t DrawAutomap_patch[] = {
    0xa1, 0x48, 0x36, 0x4e, 0x00,       // MOV        EAX,[leveltype]
    0x83, 0xf8, 0x01,                   // CMP        EAX,0x1
    0x0f, 0x8c, 0x47, 0x03, 0x00, 0x00, // JL         end_of_function
    0x83, 0xf8, 0x05,                   // CMP        EAX,0x5
    0x0f, 0x8f, 0x3e, 0x03, 0x00, 0x00, // JG         end_of_function
    0xeb, 0x13,                         // JMP        do_draw
};

} // namespace

void automap_fix_main()
{
    bool ok = true;
    ok &= patch_bytes(0x00451e51, InitAutomap_patch, sizeof(InitAutomap_patch));
    ok &= patch_bytes(0x004532d2, DrawAutomap_patch, sizeof(DrawAutomap_patch));
    printf("%s %s\n", __func__, ok ? "success" : "fail");
}
