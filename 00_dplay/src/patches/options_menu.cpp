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
auto const wndproc_title_newloadquit = reinterpret_cast<void (__fastcall *)(HWND, UINT, WPARAM, LPARAM)>(0x0041A06E);
auto const menu_redraw = reinterpret_cast<void (__fastcall *)(HWND)>(0x00419982);
auto const palette_update = reinterpret_cast<void (__fastcall *)()>(0x004826F0);
auto const PlayRndSFX = reinterpret_cast<void (__fastcall *)(int)>(0x0046491F);
auto const PaletteFadeOut = reinterpret_cast<void (__fastcall *)(int)>(0x0048237B);
auto const PaletteFadeIn = reinterpret_cast<void (__fastcall *)(int)>(0x0048233A);

BYTE** const pMenuBackgroundCel = reinterpret_cast<BYTE**>(0x005DE8F0);
BYTE** const pDiabFrCel = reinterpret_cast<BYTE**>(0x005DE900);
BYTE** const pPenTitleCel = reinterpret_cast<BYTE**>(0x005DE8F8);
int* const titleLogoFrame = reinterpret_cast<int*>(0x005DE8EC);
int* const menu_selected_index = reinterpret_cast<int*>(0x005DE8E4);
int* const pentSpinFrame = reinterpret_cast<int*>(0x005DE8E8);
int* const MouseX = reinterpret_cast<int*>(0x0061B730);
int* const MouseY = reinterpret_cast<int*>(0x0061B734);
bool* const bActive = reinterpret_cast<bool*>(0x0061BF68);
UINT* const Msg_on_fadeout_done = reinterpret_cast<UINT*>(0x00061A950);
BYTE* const fade_param = reinterpret_cast<BYTE*>(0x005DE8E0);

//
// Locals
//

bool options_menu_open = false;
int options_selection = 0;
int scrollview_patch_top = 0;
constexpr int scrollview_size = 20;
constexpr int font_small_height = 22; // from gendata/titleqtxt.cel

void adjust_scrollview()
{
    // if (options_selection < scrollview_patch_top) {
    //     scrollview_patch_top = options_selection;
    // } else if (options_selection >= scrollview_patch_top + scrollview_size - 1) {
    //     scrollview_patch_top = options_selection - scrollview_size - 1;
    // }
}

void draw_options_menu()
{
    DrawCel(64, 639, *pMenuBackgroundCel, 1, 640);
    // DrawCel(64, 377, *pDiabFrCel, *titleLogoFrame, 640);

    for (auto i = 0; i < scrollview_size; i++) {
        auto patch_index = i + scrollview_patch_top;
        if (patch_index >= get_patches().size()) {
            break;
        }

        // 160 just seems to be the right X offset... there's probably a gutter or something?
        const auto drawy = 160 + (i + 1) * font_small_height;

        if (patch_index == options_selection) {
            print_title_str_small(64, drawy, ">"); // TODO this renders as "
        }
        if (get_patches()[patch_index].checked) {
            print_title_str_small(64 + font_small_height, drawy, "X");
        }
        print_title_str_small(64 + font_small_height * 2, drawy, get_patches()[patch_index].name);
    }
}

void wndproc_options(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
    // The following are incredible nessary:
    //
    // * WM_MOUSEMOVE: Assign to MouseX, MouseY otherwise the mouse will be frozen in place
    // * WM_USR: Call menu_redraw otherwise the game looks like it's frozen

    // Fade out/in is not implemented because I don't understand it 100%

    // TODO: Mouse navigation

    switch (Msg) {
    case WM_ACTIVATEAPP:
        *bActive = wParam;
        if (bActive) {
            palette_update();
        }
        break;
    case WM_DESTROY:
        ShowCursor(true);
        PostQuitMessage(0);
        break;
    case WM_MOUSEMOVE:
        *MouseX = lParam & 0xFFFF;
        *MouseY = lParam >> 16;
        break;
    case WM_KEYDOWN:
        switch (wParam) {
        case VK_ESCAPE:
            options_menu_open = false;
            break;
        case VK_UP:
            --options_selection;
            if (options_selection < 0) {
                options_selection = 0;
            }
            if (options_selection < scrollview_patch_top) {
                --scrollview_patch_top;
            }
            PlayRndSFX(0x2F);
            break;
        case VK_DOWN:
            ++options_selection;
            if (options_selection >= get_patches().size()) {
                options_selection = get_patches().size() - 1;
            }
            if (options_selection >= scrollview_patch_top + scrollview_size) {
                ++scrollview_patch_top;
            }
            adjust_scrollview();
            PlayRndSFX(0x2F);
            break;
        case VK_RETURN: {
            auto& patch = get_patches()[options_selection];
            patch.checked = !patch.checked;
            save_checked_patches();
            PlayRndSFX(0x2E);
            break;
        }
        }
        break;
    case WM_USER:
        menu_redraw(hWnd);
        break;
    default:
        DefWindowProcA(hWnd, Msg, wParam, lParam);
        break;
    }
}

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
    if (options_menu_open) {
        draw_options_menu();
        return;
    }

    DrawCel(64, 639, *pMenuBackgroundCel, 1, 640);
    DrawCel(64, 377, *pDiabFrCel, *titleLogoFrame, 640);

    constexpr char const* options[] = {
        "New Game",
        "Load Game",
        "Options", // TODO make selectable
        "Quit",
    };
    constexpr int options_text_width[] = {
        226,
        258,
        180, // TODO tune
        113,
    };
    print_title_str_small(80, 624, PRDE_VERSION);
    for (auto i = 0; i < 4; ++i) {
        auto const drawx = (640 - options_text_width[i]) / 2 + 64;
        auto const drawy = i * 45 + 450;
        print_title_str_large(drawx, drawy, options[i]);
        if (i == *menu_selected_index) {
            DrawCel(155, drawy, *pPenTitleCel, *pentSpinFrame, 48);
            DrawCel(563, drawy, *pPenTitleCel, *pentSpinFrame, 48);
        }
    }
}

void __fastcall wndproc_title_newloadquit_hook(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
    if (options_menu_open) {
        wndproc_options(hWnd, Msg, wParam, lParam);
        return;
    }

    if (Msg == WM_KEYDOWN && wParam == VK_RETURN && *menu_selected_index == 2) {
        options_menu_open = true;
        PlayRndSFX(0x2E);
        return;
    }
    wndproc_title_newloadquit(hWnd, Msg, wParam, lParam);
}

} // namespace

bool options_menu_main()
{
    bool ok = true;

    ok &= patch_jmp(0x00419111, (void*)draw_main_menu_hook);
    // Allow 4 options when pressing up
    ok &= patch<uint8_t>(0x0041A1FE + 6, 0x03);
    // Allow 4 options when pressing down
    ok &= patch<uint8_t>(0x0041A26A + 6, 0x03);
    // Option 4 is now quit
    ok &= patch<uint8_t>(0x0041A486 + 3, 0x03);
    // intercept wndproc logic
    ok &= patch_call(0x00488205, (void*)wndproc_title_newloadquit_hook);

    // TODO fix mouse navigation

    return ok;
}
