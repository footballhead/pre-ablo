#include "patches.hpp"

#include "util.hpp"
#include "version.h"

namespace {

//
// Imports
//


auto const DrawCel = reinterpret_cast<void (__fastcall *)(long, long, BYTE*, long, long)>(0x00482B53);
auto const print_title_str_small = reinterpret_cast<void (__fastcall *)(long, long, const char*)>(0x00418FB2);
auto const print_title_str_large = reinterpret_cast<void (__fastcall *)(long, long, const char*)>(0x00418F10);

BYTE** const pMenuBackgroundCel = reinterpret_cast<BYTE**>(0x005DE8F0);
BYTE** const pDiabFrCel = reinterpret_cast<BYTE**>(0x005DE900);
BYTE** const pPenTitleCel = reinterpret_cast<BYTE**>(0x005DE8F8);
int* const titleLogoFrame = reinterpret_cast<int*>(0x005DE8EC);
int* const menu_selected_index = reinterpret_cast<int*>(0x005DE8E4);
int* const pentSpinFrame = reinterpret_cast<int*>(0x005DE8E8);

//
// Hooks
//

void draw_main_menu_hook()
{
#ifndef NDEBUG
    // It's useful to wait until after we've patched the binary before attaching a debugger.
    // At least, I know that IDA will freak out when the patching is performed.
    // https://stackoverflow.com/questions/663449/what-is-the-best-way-to-attach-a-debugger-to-a-process-in-vc-at-just-the-right
    // printf("%s: Waiting for debugger...\n", __func__);
    // while (!IsDebuggerPresent()) {
    //     Sleep(100);
    // }
    // printf("%s: Got debugger!\n", __func__);
    // DebugBreak();
#endif
    constexpr char const* options[] = {
        "New Game",
        "Load Game",
        "Quit",
    };
    constexpr int options_text_width[] = {
        226,
        258,
        113,
    };
    DrawCel(64, 639, *pMenuBackgroundCel, 1, 640);
    DrawCel(64, 377, *pDiabFrCel, *titleLogoFrame, 640);
    print_title_str_small(80, 624, PRDE_VERSION);
    for (auto i = 0; i < 3; ++i) {
        auto const drawx = (640 - options_text_width[i]) / 2 + 64;
        auto const drawy = i * 45 + 450;
        print_title_str_large(drawx, drawy, options[i]);
        if (i == *menu_selected_index) {
            DrawCel(155, drawy, *pPenTitleCel, *pentSpinFrame, 48);
            DrawCel(563, drawy, *pPenTitleCel, *pentSpinFrame, 48);
        }
    }
}

} // namespace

void options_menu_main()
{
    bool ok = true;

    ok &= patch_jmp(0x00419111, (void*)draw_main_menu_hook);

    printf("%s %s\n", __func__, ok ? "success" : "fail");
}
