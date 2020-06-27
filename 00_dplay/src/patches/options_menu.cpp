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
BYTE* palette_buffer = reinterpret_cast<BYTE*>(0x0061AD60);
PALETTEENTRY* const menu_palette = reinterpret_cast<PALETTEENTRY*>(0x0061B1E8);
int* const force_redraw = reinterpret_cast<int*>(0x00419F0C);
BYTE* const titlText_ascii2frame = reinterpret_cast<BYTE*>(0x004AA608);
BYTE* const titlText_kern = reinterpret_cast<BYTE*>(0x004A2668);
BYTE* const bigTGold_ascii2frame = reinterpret_cast<BYTE*>(0x004B8C98);
BYTE* const bigTGold_kern = reinterpret_cast<BYTE*>(0x004B8D18);
BYTE* const smalText_ascii2frame = reinterpret_cast<BYTE*>(0x004A34D0);
BYTE* const smalText_kern = reinterpret_cast<BYTE*>(0x004A3550);

//
// Locals
//

bool options_menu_open = false;

enum submenu_id {
    submenu_mainview,
    submenu_restart_dialog,
};
int submenu = submenu_mainview;

enum focus_id {
    focus_listview,
    focus_back_button,
};
int focus = focus_listview;

int listview_selection = 0;

constexpr int scrollview_size = 30;

constexpr auto screen_width = 640;
constexpr auto screen_height = 480;

constexpr auto standard_box_margin = 8;
constexpr auto standard_box_padding = 4;

// constexpr auto bigTGold_height = 45;
constexpr auto bigText_height = 18;

constexpr auto listview_x = 0;
constexpr auto listview_y = 0;
constexpr auto listview_width = screen_width / 2;
constexpr auto listview_height = screen_height - bigText_height - 2 * standard_box_padding;

constexpr auto listview_content_x = listview_x + standard_box_padding + standard_box_margin;
constexpr auto listview_content_y = listview_y + standard_box_padding + standard_box_margin;
constexpr auto listview_content_width  = listview_width  - 2 * standard_box_padding - 2 * standard_box_margin;
constexpr auto listview_content_height = listview_height - 2 * standard_box_padding - 2 * standard_box_margin;

constexpr auto textbox_x = screen_width / 2;
constexpr auto textbox_y = 0;
constexpr auto textbox_width = screen_width / 2;
constexpr auto textbox_height = listview_height;

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

// void wait_for_debugger()
// {
//     // It's useful to wait until after we've patched the binary before attaching a debugger.
//     // At least, I know that IDA will freak out when the patching is performed.
//     // https://stackoverflow.com/questions/663449/what-is-the-best-way-to-attach-a-debugger-to-a-process-in-vc-at-just-the-right
//     printf("%s: Waiting for debugger...\n", __func__);
//     while (!IsDebuggerPresent()) {
//         Sleep(100);
//     }
//     printf("%s: Got debugger!\n", __func__);
//     DebugBreak();
// }

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

    int calculate_string_width(const char* text) const {
        int result = 0;
        while (*text) {
            result += kerning[ascii2frame[*text]] + letterSpacing;
            ++text;
        }
        return result;
    }

    // This is a loose decompilation of print_title_str_small
    // (x,y) are top-left coords
    void draw_string(int x, int y, const char* text) const {
        draw_string(x, y, text, strlen(text));
    }

    // (x,y) are top-left coords
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
font bigText = {nullptr, 22, bigText_height, titlText_kern, titlText_ascii2frame, 2, 2}; // gendata\\bigtext.cel
font smalText = {nullptr, 13, 11, smalText_kern, smalText_ascii2frame, 1, 1}; // ctrlpan\\smaltext.cel
font hugeText = {nullptr, 46, 45, bigTGold_kern, bigTGold_ascii2frame, 0, 0}; // data\\hugetext.cel

void open_menu()
{
    options_menu_open = true;
    submenu = submenu_mainview;

    // Load on demand (can't do in main because MPQ not open yet)
    if (!smalText.is_loaded()) {
        smalText.load_gfx("ctrlpan\\smaltext.cel");
    }
    if (!bigText.is_loaded()) {
        bigText.load_gfx("gendata\\bigtext.cel");
    }
    if (!hugeText.is_loaded()) {
        hugeText.load_gfx("data\\hugetext.cel");
    }
    if (!pPentSpn2Cel) {
        pPentSpn2Cel = LoadFileInMem("data\\pentspn2.cel");
    }
    if (!pTextSlidCel) {
        pTextSlidCel = LoadFileInMem("data\\textslid.cel");
    }

    SetCursor_(1); // Armored glove, with in-game palette
    load_and_use_palette("gendata\\create.pal");
}

void close_menu()
{
    options_menu_open = false;

    if (smalText.is_loaded()) {
        smalText.free_gfx();
    }
    if (bigText.is_loaded()) {
        bigText.free_gfx();
    }
    if (hugeText.is_loaded()) {
        hugeText.free_gfx();
    }
    if (pPentSpn2Cel) {
        free_cel_graphics(&pPentSpn2Cel);
    }
    if (pTextSlidCel) {
        free_cel_graphics(&pTextSlidCel);
    }

    SetCursor_(2);// Armored glove, with title.pal palette
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

void draw_submenu_restart_dialog()
{
    ClearScreenBuffer();

    constexpr auto text_restart = "Restart to apply patches";
    hugeText.draw_string(DRAW_ORIGIN_X + (screen_width - hugeText.calculate_string_width(text_restart)) / 2,
        DRAW_ORIGIN_Y + (screen_height - hugeText.frameHeight) / 2,
        text_restart);

    constexpr auto ok_label = "OK";
    auto const ok_label_width = bigText.calculate_string_width(ok_label);
    auto const ok_label_x = DRAW_ORIGIN_X + (screen_width - ok_label_width) / 2;
    auto const ok_label_y = DRAW_ORIGIN_Y + (screen_height - bigText.frameHeight) / 2 + hugeText.frameHeight + bigText.frameHeight;
    bigText.draw_string(ok_label_x, ok_label_y, ok_label);

    DrawCel(ok_label_x - 2 * pentSpn2Size, ok_label_y + pentSpn2Size + 2, pPentSpn2Cel, *pentSpinFrame, pentSpn2Size);
    DrawCel(ok_label_x + ok_label_width + pentSpn2Size, ok_label_y + pentSpn2Size + 2, pPentSpn2Cel, *pentSpinFrame, pentSpn2Size);
}

void wndproc_submenu_restart_dialog(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
    switch (Msg) {
    case WM_ACTIVATEAPP:
        *bActive = wParam;
        if (bActive) {
            palette_update();
        }
        break;
    case WM_DESTROY:
        // TODO: Do other things need to be here?
        ShowCursor(true);
        PostQuitMessage(0);
        break;
    case WM_MOUSEMOVE: {
        *MouseX = lParam & 0xFFFF;
        *MouseY = lParam >> 16;
        break;
    }
    case WM_LBUTTONDOWN:
        // fall-through
    case WM_KEYDOWN:
        close_menu();
        PlayRndSFX(0x2E);
        break;
    case WM_USER:
        menu_redraw(hWnd);
        break;
    default:
        DefWindowProcA(hWnd, Msg, wParam, lParam);
        break;
    }
}

void draw_submenu_mainview()
{
    ClearScreenBuffer();

    // Draw LHS panel (listview)
    draw_box(DRAW_ORIGIN_X + listview_x + standard_box_margin,
        DRAW_ORIGIN_Y + listview_x + standard_box_margin,
        listview_width - 2 * standard_box_margin,
        listview_height - 2 * standard_box_margin);

    for (auto i = 0; i < scrollview_size; i++) {
        auto const patch_index = i;
        if (patch_index >= get_patches().size()) {
            break;
        }

        auto const listview_item_height = smalText.frameHeight + smalText.lineSpacing;
        auto const drawx = DRAW_ORIGIN_X + listview_content_x;
        auto const drawy = DRAW_ORIGIN_Y + listview_content_y + i * listview_item_height;

        if (focus == focus_listview && patch_index == listview_selection) {
            DrawCel(drawx, drawy + pentSpn2Size, pPentSpn2Cel, *pentSpinFrame, pentSpn2Size);
        }
        if (get_patches()[patch_index].checked) {
            smalText.draw_string(drawx + pentSpn2Size, drawy, "X");
        }

        smalText.draw_string(drawx + pentSpn2Size + smalText.frameWidth, drawy, get_patches()[patch_index].name);
    }

    // Draw RHS panel (textbox)
    draw_box(DRAW_ORIGIN_X + textbox_x + standard_box_margin,
        DRAW_ORIGIN_Y + textbox_y + standard_box_margin,
        textbox_width - 2 * standard_box_margin,
        textbox_height - 2 * standard_box_margin);
    auto const textbox_text = focus == focus_listview ? get_patches()[listview_selection].description : "Accept chosen patches.\n\nRestart to apply patches.";
    smalText.draw_multiline_string(DRAW_ORIGIN_X + textbox_x + standard_box_margin + standard_box_padding,
        DRAW_ORIGIN_Y + textbox_y + standard_box_margin + standard_box_padding,
        textbox_text,
        textbox_width - 2 * standard_box_margin - 2 * standard_box_padding);

    // Draw text at bottom
    constexpr auto back_label = "Apply";
    auto const back_label_width = bigText.calculate_string_width(back_label);
    auto const back_label_x = DRAW_ORIGIN_X + (screen_width - back_label_width) / 2;
    auto const back_label_y = DRAW_ORIGIN_Y + screen_height - bigText.frameHeight - 8;
    bigText.draw_string(back_label_x, back_label_y, back_label);
    if (focus == focus_back_button) {
        DrawCel(back_label_x - 2 * pentSpn2Size, back_label_y + pentSpn2Size + 2, pPentSpn2Cel, *pentSpinFrame, pentSpn2Size);
        DrawCel(back_label_x + back_label_width + pentSpn2Size, back_label_y + pentSpn2Size + 2, pPentSpn2Cel, *pentSpinFrame, pentSpn2Size);
    }
}

void wndproc_submenu_mainview(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
    // The following are incredible necessary:
    //
    // * WM_MOUSEMOVE: Assign to MouseX, MouseY otherwise the mouse will be frozen in place
    // * WM_USER: Call menu_redraw otherwise the game looks like it's frozen

    switch (Msg) {
    case WM_ACTIVATEAPP:
        *bActive = wParam;
        if (bActive) {
            palette_update();
        }
        break;
    case WM_DESTROY:
        // TODO: Do other things need to be here?
        ShowCursor(true);
        PostQuitMessage(0);
        break;
    case WM_MOUSEMOVE: {
        auto const newx = lParam & 0xFFFF;
        auto const newy = lParam >> 16;
        if (newx == *MouseX && newy == *MouseY) {
            return;
        }

        *MouseX = newx;
        *MouseY = newy;
        if (*MouseX >= listview_content_x
            && *MouseY >= listview_content_y
            && *MouseX < listview_content_x + listview_content_width
            && *MouseY < listview_content_y + listview_content_height)
        {
            focus = focus_listview;

            auto const old_selection = listview_selection;
            auto const listview_item_height = smalText.frameHeight + smalText.lineSpacing;
            listview_selection = (*MouseY - listview_content_y) / listview_item_height;

            auto const num_patches = get_patches().size();
            if (listview_selection >= num_patches) {
                listview_selection = num_patches - 1;
            }

            if (old_selection != listview_selection) {
                PlayRndSFX(0x2F);
            }
        }
        break;
    }
    case WM_LBUTTONDOWN: {
        if (*MouseX >= listview_content_x
            && *MouseY >= listview_content_y
            && *MouseX < listview_content_x + listview_content_width
            && *MouseY < listview_content_y + listview_content_height)
        {
            auto& patch = get_patches()[listview_selection];
            patch.checked = !patch.checked;
            PlayRndSFX(0x2E);
        }

        if (*MouseY > listview_height) {
            submenu = submenu_restart_dialog;
            PlayRndSFX(0x2E);
        }
        break;
    }
    case WM_KEYDOWN:
        switch (wParam) {
        case VK_ESCAPE:
            submenu = submenu_restart_dialog;
            break;
        case VK_UP:
            switch (focus) {
            case focus_listview:
                --listview_selection;
                if (listview_selection < 0) {
                    listview_selection = 0;
                }
                PlayRndSFX(0x2F);
                break;
            case focus_back_button:
                focus = focus_listview;
                break;
            }
            break;
        case VK_DOWN: {
            switch (focus) {
            case focus_listview: {
                auto const num_patches = get_patches().size();
                ++listview_selection;
                if (listview_selection >= num_patches) {
                    listview_selection = num_patches - 1;
                    focus = focus_back_button;
                }
                PlayRndSFX(0x2F);
                break;
            }
            case focus_back_button:
                // Do nothing, at bottom of screen
                break;
            }
            break;
        }
        case VK_RETURN:
            switch (focus) {
            case focus_listview: {
                auto& patch = get_patches()[listview_selection];
                patch.checked = !patch.checked;
                save_checked_patches();
                PlayRndSFX(0x2E);
                break;
            }
            case focus_back_button:
                submenu = submenu_restart_dialog;
                PlayRndSFX(0x2E);
                break;
            }
            break;
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
    if (options_menu_open) {
        switch (submenu) {
        case submenu_mainview:
            draw_submenu_mainview();
            break;
        case submenu_restart_dialog:
            draw_submenu_restart_dialog();
            break;
        }
        return;
    }

    DrawCel(64, 639, *pMenuBackgroundCel, 1, 640);
    DrawCel(64, 377, *pDiabFrCel, *titleLogoFrame, 640);

    constexpr char const* options[] = {
        "New Game",
        "Load Game",
        "Options", // TODO make selectable by mouse
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
        switch (submenu) {
        case submenu_mainview:
            wndproc_submenu_mainview(hWnd, Msg, wParam, lParam);
            break;
        case submenu_restart_dialog:
            wndproc_submenu_restart_dialog(hWnd, Msg, wParam, lParam);
            break;
        }
        return;
    }

    // TODO: Mouse navigation

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
    // 

    // TODO fix mouse navigation
    // TODO sometimes the game can't close down properly, it gets stuck trying to free a graphic... what's up with taht?

    return ok;
}
