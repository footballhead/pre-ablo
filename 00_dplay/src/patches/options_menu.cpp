#include "patches.hpp"

#include "functions.hpp"
#include "macros.hpp"
#include "util.hpp"
#include "version.h"

#define PATCH_NAME options_menu
DESCRIBE_PATCH(R"txt(DO NOT TURN OFF! If you do, delete last_patch.txt in game directory.

Insert an options menu on the main menu to customize patches.)txt");

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
auto const LoadPalette = reinterpret_cast<void (__fastcall *)(const char*, BYTE*)>(0x00481FB0);
auto const CopyPalette = reinterpret_cast<void (__fastcall *)(PALETTEENTRY*, BYTE*)>(0x004823BC);
auto const ClearScreenBuffer = reinterpret_cast<void (__fastcall *)()>(0x004813C2);
auto const SetCursor_ = reinterpret_cast<void (__fastcall *)(int)>(0x00479CAF);

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
BYTE* palette_buffer = reinterpret_cast<BYTE*>(0x0061AD60);
PALETTEENTRY* const menu_palette = reinterpret_cast<PALETTEENTRY*>(0x0061B1E8);
int* const force_redraw = reinterpret_cast<int*>(0x00419F0C);
//BYTE* const title_text_ascii2frame = reinterpret_cast<BYTE*>(0x004AA608);
//BYTE* const mfontkern = reinterpret_cast<BYTE*>(0x004A2668);
BYTE* const smalText_ascii2frame = reinterpret_cast<BYTE*>(0x004A34D0);
BYTE* const smalText_kern = reinterpret_cast<BYTE*>(0x004A3550);

//
// Locals
//

bool options_menu_open = false;
int options_selection = 0;
int scrollview_patch_top = 0;
constexpr int scrollview_size = 30;

BYTE* pPentSpn2Cel = nullptr;
constexpr auto pentSpn2Size = 12;

BYTE* pTextSlidCel = nullptr;

inline void free_cel_graphics(BYTE** cel)
{
    GlobalUnlock(GlobalHandle(*cel));
    GlobalFree(GlobalHandle(*cel));
    *cel = nullptr;
}

void load_and_use_palette(char const* filename)
{
    // Load .PAL file into existing buffer in the data partition
    LoadPalette(filename, palette_buffer);
    // Turn .PAL into RGB color
    CopyPalette(menu_palette, palette_buffer);
    // Start using the palette. Yes, the fade out/fade in routines make this happen for some reason...
    PaletteFadeOut(0x20);
    PaletteFadeIn(0x20);
}

struct font {
    // CEL data straight from MPQ
    BYTE* cel = nullptr;
    // Horizontal size of an individual frame
    int frameWidth;
    // Vertical size of an individual frame
    int frameHeight;
    // Amount to advance x position for each character
    BYTE* kerning;
    // Function to translate ascii character to CEL specific frame
    BYTE* ascii2frame;
    // Extra kerning/whitespace between chars
    int letterSpacing = 0;
    int lineSpacing = 0;

    bool is_loaded() const { return cel; }
    void load_gfx(const char* filename) { cel = LoadFileInMem(filename); }
    void free_gfx() {
        if (cel) {
            free_cel_graphics(&cel);
        }
    }

    // This is a loose decompilation of print_title_str_small
    // (x,y) are top-left coords
    void draw_string(int x, int y, const char* text) const {
        draw_string(x, y, text, strlen(text));
    }

    void draw_string(int x, int y, char const* text, size_t textlen) const {
        if (!is_loaded()) {
            printf("Refusing to draw font with unloaded graphics!\n");
            return;
        }
        y += frameHeight; // origin is bottom-left corner, but I like top-left interfaces

        for (int i = 0; i < textlen; ++i) {
            auto frame = ascii2frame[text[i]];
            if (frame > 0) {
                DrawCel(x, y, cel, frame, frameWidth);
            }
            x += kerning[frame] + letterSpacing;
        }
    }

    // (x,y) are top-left coords
    /// @param width A size constraint on the horizontal axis
    void draw_multiline_string(int x, int y, char const* text, int width) const {
        if (!is_loaded()) {
            printf("Refusing to draw font with unloaded graphics!\n");
            return;
        }

        /// @param[out] len Number of letters in the word
        /// @param[out] width The width of the next word as drawn on-screen
        /// @returns true if more words, false otherwise
        auto next_word = [this](char const* text, size_t& len, int& width) -> bool {
            if (*text == '\0') {
                return false;
            }

            width = 0;
            auto iter = text;
            for (/*empty*/; *iter != '\n' && *iter != ' ' && *iter != '\0'; ++iter) {
                width += kerning[ascii2frame[*iter]] + letterSpacing;
            }
            len = iter - text;
            return true;
        };

        int xoff = 0;
        int yoff = 0;

        size_t wordlen = 0;
        int wordwidth = 0;
        while (next_word(text, wordlen, wordwidth)) {
            // Word width overflowing bounds causes newline and carriage return. Do before drawing so the word appears
            // properly on the next line.
            if (xoff + wordwidth > width) {
                xoff = 0;
                yoff += frameHeight + lineSpacing;
            }

            // Draw just the word
            draw_string(x + xoff, y + yoff, text, wordlen);
            text += wordlen;
            xoff += wordwidth;

            // The word separator still needs to be dealt with (if there are more words)
            if (*text == '\0') {
                break;
            } else if (*text == '\n') {
                xoff = 0;
                yoff += frameHeight + lineSpacing;
            } else if (*text == ' ') {
                // Since spaces contain no visual data, we don't care if this pushes xoff > width. The next loop
                // iteration will deal with the consequences.
                xoff += kerning[ascii2frame[*text]] + letterSpacing;
            }

            // Advance past word separator
            ++text;
        }
    }
};
// font bigTextFont = {nullptr, 22, 18, mfontkern, title_text_ascii2frame, 2}; // gendata\\bigtext.cel
font smalText = {nullptr, 13, 11, smalText_kern, smalText_ascii2frame, 1, 1}; // ctrlpan\\smaltext.cel

void open_menu()
{
    options_menu_open = true;
    SetCursor_(1); // Armored glove, with in-game palette
    // Load on demand (can't do in main because MPQ not open yet)
    if (!smalText.is_loaded()) {
        smalText.load_gfx("ctrlpan\\smaltext.cel");
    }
    if (!pPentSpn2Cel) {
        pPentSpn2Cel = LoadFileInMem("data\\pentspn2.cel");
    }
    if (!pTextSlidCel) {
        pTextSlidCel = LoadFileInMem("data\\textslid.cel");
    }
    load_and_use_palette("gendata\\create.pal");
}

void close_menu()
{
    options_menu_open = false;
    SetCursor_(2);// Armored glove, with title.pal palette
    if (smalText.is_loaded()) {
        smalText.free_gfx();
    }
    if (pPentSpn2Cel) {
        free_cel_graphics(&pPentSpn2Cel);
    }
    if (pTextSlidCel) {
        free_cel_graphics(&pTextSlidCel);
    }
    load_and_use_palette("gendata\\title.pal");
}

/// @remark Depends on pTextSlidCel being loaded.
/// @remark Dimensions must be multiples of 12
/// @remark (x,y) specifies top-left
void draw_box(int x, int y, int width, int height)
{
    // Because of the gfx origin being bottom left, we need to do some pixel manip to be within desired bounds
    width -= 12;
    y += 12;
    height -= 12;

    // Draw corners
    DrawCel(x, y, pTextSlidCel, 1, 12);
    DrawCel(x, y + height, pTextSlidCel, 2, 12);
    DrawCel(x + width, y + height, pTextSlidCel, 3, 12);
    DrawCel(x + width, y, pTextSlidCel, 4, 12);

    // Draw sides. Corners already cover some of the sides
    for (auto drawx = 12; drawx < width; drawx += 12) {
        DrawCel(x + drawx, y, pTextSlidCel, 5, 12);
        DrawCel(x + drawx, y + height, pTextSlidCel, 7, 12);
    }
    for (auto drawy = 12; drawy < height; drawy += 12) {
        DrawCel(x, y + drawy, pTextSlidCel, 6, 12);
        DrawCel(x + width, y + drawy, pTextSlidCel, 8, 12);
    }
}

void draw_options_menu()
{
    ClearScreenBuffer();

    constexpr auto box_margin = 8;
    constexpr auto box_padding = 4;

    draw_box(DRAW_ORIGIN_X + box_margin, DRAW_ORIGIN_Y + box_margin, 320 - 2*box_margin, 480 - 2*box_margin);

    for (auto i = 0; i < scrollview_size; i++) {
        auto patch_index = i + scrollview_patch_top;
        if (patch_index >= get_patches().size()) {
            break;
        }

        const auto drawx = DRAW_ORIGIN_Y + box_margin + box_padding;
        const auto drawy = DRAW_ORIGIN_Y + box_margin + box_padding + i * (smalText.frameHeight + 2);

        if (patch_index == options_selection) {
            DrawCel(DRAW_ORIGIN_X + box_margin + box_padding, drawy + pentSpn2Size, pPentSpn2Cel, *pentSpinFrame, pentSpn2Size);
        }
        if (get_patches()[patch_index].checked) {
            smalText.draw_string(DRAW_ORIGIN_X + box_margin + box_padding + pentSpn2Size, drawy + smalText.frameHeight, "X");
        }

        smalText.draw_string(DRAW_ORIGIN_X + box_margin + box_padding + pentSpn2Size + smalText.frameWidth, drawy, get_patches()[patch_index].name);
    }

    draw_box(DRAW_ORIGIN_X + 320 + box_margin, DRAW_ORIGIN_Y + box_margin, 320 - 2*box_margin, 480 - 2*box_margin);
    smalText.draw_multiline_string(DRAW_ORIGIN_X + 320 + box_margin + box_padding,
        DRAW_ORIGIN_Y + box_margin + box_padding,
        get_patches()[options_selection].description,
        320 - 2*box_margin - 2*box_padding);
}

void wndproc_options(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
    // The following are incredible nessary:
    //
    // * WM_MOUSEMOVE: Assign to MouseX, MouseY otherwise the mouse will be frozen in place
    // * WM_USER: Call menu_redraw otherwise the game looks like it's frozen

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
            close_menu();
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
        open_menu();
        PlayRndSFX(0x2E);
        return;
    }
    wndproc_title_newloadquit(hWnd, Msg, wParam, lParam);
}

} // namespace

PATCH_MAIN
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
    // TODO sometimes the game can't close down properly, it gets stuck trying to free a graphic... what's up with taht?

    return ok;
}
