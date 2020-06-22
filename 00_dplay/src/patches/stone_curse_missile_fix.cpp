#include "patches.hpp"

#include "util.hpp"

namespace {

auto const DrawSlabCel = reinterpret_cast<void (__fastcall *)(long xp, long yp, BYTE *pCelBuff, long nCel, long nCelW, long ostart, long oend)>(0x00482BFB);
auto const DrawSlabCelL = reinterpret_cast<void (__fastcall *)(long xp, long yp, BYTE *pCelBuff, long nCel, long nCelW, long ostart, long oend)>(0x00483046);
auto const DrawSlabCelI = reinterpret_cast<void (__fastcall *)(long xp, long yp, BYTE *pCelBuff, long nCel, long nCelW, long ostart, long oend, char loff)>(0x004832E1);

auto const CDrawSlabCel = reinterpret_cast<void (__fastcall *)(long xp, long yp, BYTE *pCelBuff, long nCel, long nCelW, long ostart, long oend)>(0x0048350C);
auto const CDrawSlabCelL = reinterpret_cast<void (__fastcall *)(long xp, long yp, BYTE *pCelBuff, long nCel, long nCelW, long ostart, long oend)>(0x0048388F);
auto const CDrawSlabCelI = reinterpret_cast<void (__fastcall *)(long xp, long yp, BYTE *pCelBuff, long nCel, long nCelW, long ostart, long oend, char loff)>(0x00483B2A);

void __fastcall DrawSlabCel_wrapper(long xp, long yp, BYTE *pCelBuff, long nCel, long nCelW, long ostart, long oend)
{
    if (!pCelBuff) {
        return;
    }
    DrawSlabCel(xp, yp, pCelBuff, nCel, nCelW, ostart, oend);
}

void __fastcall DrawSlabCelL_wrapper(long xp, long yp, BYTE *pCelBuff, long nCel, long nCelW, long ostart, long oend)
{
    if (!pCelBuff) {
        return;
    }
    DrawSlabCelL(xp, yp, pCelBuff, nCel, nCelW, ostart, oend);
}

void __fastcall DrawSlabCelI_wrapper(long xp, long yp, BYTE *pCelBuff, long nCel, long nCelW, long ostart, long oend, char loff)
{
    if (!pCelBuff) {
        return;
    }
    DrawSlabCelI(xp, yp, pCelBuff, nCel, nCelW, ostart, oend, loff);
}

void __fastcall CDrawSlabCel_wrapper(long xp, long yp, BYTE *pCelBuff, long nCel, long nCelW, long ostart, long oend)
{
    if (!pCelBuff) {
        return;
    }
    CDrawSlabCel(xp, yp, pCelBuff, nCel, nCelW, ostart, oend);
}

void __fastcall CDrawSlabCelL_wrapper(long xp, long yp, BYTE *pCelBuff, long nCel, long nCelW, long ostart, long oend)
{
    if (!pCelBuff) {
        return;
    }
    CDrawSlabCelL(xp, yp, pCelBuff, nCel, nCelW, ostart, oend);
}

void __fastcall CDrawSlabCelI_wrapper(long xp, long yp, BYTE *pCelBuff, long nCel, long nCelW, long ostart, long oend, char loff)
{
    if (!pCelBuff) {
        return;
    }
    CDrawSlabCelI(xp, yp, pCelBuff, nCel, nCelW, ostart, oend, loff);
}

} // namespace

void stone_curse_missile_fix_main()
{
    bool ok = true;

    // Only patch the 2+ missile case because I'm lazy
    ok &= patch_call(0x0047C0F7, (void*)DrawSlabCelI_wrapper);
    ok &= patch_call(0x0047C155, (void*)DrawSlabCelL_wrapper);
    ok &= patch_call(0x0047C19D, (void*)DrawSlabCel_wrapper);

    ok &= patch_call(0x0047C479, (void*)CDrawSlabCelI_wrapper);
    ok &= patch_call(0x0047C4D7, (void*)CDrawSlabCelL_wrapper);
    ok &= patch_call(0x0047C51F, (void*)CDrawSlabCel_wrapper);

    printf("%s %s\n", __func__, ok ? "success" : "fail");
}
