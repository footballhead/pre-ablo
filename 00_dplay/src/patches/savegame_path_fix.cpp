#include "patches.hpp"
#include "util.hpp"

#include <cerrno>
#include <cstdlib>

namespace {

//
// Imports
//

auto did_find_game00 = reinterpret_cast<uint8_t*>(0x005DEA18);
// We're pretty lucky that game00_abspath is 256 long, otherwise we'd need to do more work...
auto game00_abspath = reinterpret_cast<char*>(0x005DE918);
constexpr auto game00_abspath_size = 256;

constexpr auto Title_FindSaveGame_addr = 0x004190AEu;

//
// Locals
//

char larger_savedir_abspath[MAX_PATH]{'\0'};
char GM_LoadGame_larger_save_abspath[MAX_PATH]{'\0'};

//
// Hooks
//

void __fastcall Title_FindSaveGame_Fix()
{
    printf("%s\n", __func__);

    // The only reason we hook this function is that patching just the stack local buffer size is harder than reimplmenting the entire function
    char buffer[MAX_PATH]{'\0'};
    *did_find_game00 = 0;
    snprintf(game00_abspath, game00_abspath_size, "%s%s", larger_savedir_abspath, "\\Game00.sav"); // Use safe counterpart
    // TODO: Terminate if snprintf would overrun game00_abspath
    auto const err = _searchenv_s(game00_abspath, nullptr, buffer, sizeof(buffer)); // Use safe counterpart
    // TODO: Terminate if _searchenv_s would overrun buffer
    if (err != ENOENT && err != EINVAL) {
        // I have no idea why this isn't just a bool but this is literally what the dissasembly shows me so shrug.jpg
        *did_find_game00 |= 0x80;
    }
    printf("%s: %s\n", __func__, *did_find_game00 ? "found save game" : "no save game");
}

void __stdcall WinMain_GetCurrentDirectoryA_Hook(DWORD /*ignored*/, LPSTR /*ignored*/)
{
    printf("%s\n", __func__);
#ifndef NDEBUG
    // It's useful to wait until after we've patched the binary before attaching a debugger.
    // At least, I know that IDA will freak out when the patching is performed.
    // https://stackoverflow.com/questions/663449/what-is-the-best-way-to-attach-a-debugger-to-a-process-in-vc-at-just-the-right
    printf("%s: Waiting for debugger...\n", __func__);
    while (!IsDebuggerPresent()) {
        Sleep(100);
    }
    printf("%s: Got debugger!\n", __func__);
    DebugBreak();
#endif
    // Ignore arguments in favor of our local buffer
    GetCurrentDirectoryA(sizeof(larger_savedir_abspath), larger_savedir_abspath);
}

} // namespace

//
// Init patch
//

void savegame_patch_fix_main()
{
    // Since the existing save game directory buffer is too small (64 chars), we patch it to use our bigger buffer.
    // The only restriction on buffer size right now is in WinMain where the buffer size is `push`'d to GetCurrentDirectory.
    // This is a signed byte immediate so values > 0x80 are sign extended. :(
    // So we're limited to 0x7F (127) in size. At least 127 > 64...

    bool ok = true;

    // Fix Title_FindSaveGame by redirecting to our own fixed function
    ok &= patch_jmp(Title_FindSaveGame_addr, (void*)Title_FindSaveGame_Fix);

    // Fix WinMain to use larger_savedir_abspath
    // Call our GetCurrentDirectory
    ok &= patch_call(0x00484BF4, (void*)WinMain_GetCurrentDirectoryA_Hook);
    ok &= patch(0x00484BF4 + 5, nop_opcode);
    // Fix call sprintf arguments
    ok &= patch_push(0x00484BFA, larger_savedir_abspath);
    ok &= patch_push(0x00484C04, larger_savedir_abspath);
    // Fix call chdir arguments
    ok &= patch_push(0x00484C11, larger_savedir_abspath);
    // TODO: Fix GetDriveType(NULL) == DRIVE_CDROM branch?

    // Fix GM_LoadGame
    ok &= patch_push(0x00460D63, larger_savedir_abspath);
    // Replace small local buffer with our own larger one.
    // Turn `lea eax, [ebp+var]` and `push eax` into only `push GM_LoadGame_larger_save_abspath`
    // Patch is smaller than existing machine code so pad the rest with nops
    ok &= patch_push(0x00460D6D, GM_LoadGame_larger_save_abspath);
    ok &= patch(0x00460D6D + 5, nop_opcode);
    ok &= patch(0x00460D73, nop_opcode);
    // Same thing again for other usage
    ok &= patch_push(0x00460D7E, GM_LoadGame_larger_save_abspath);
    ok &= patch(0x00460D7E + 5, nop_opcode);
    ok &= patch(0x00460D84, nop_opcode);

    // Fix save_game
    ok &= patch_push(0x00462A4C, larger_savedir_abspath);

    // Fix SaveLevel (normal level and setlevel)
    ok &= patch_push(0x00462A4C, larger_savedir_abspath);
    ok &= patch_push(0x00463622, larger_savedir_abspath);

    // Fix LoadLevel (normal level and setlevel)
    ok &= patch_push(0x0046373E, larger_savedir_abspath);
    ok &= patch_push(0x0046375F, larger_savedir_abspath);

    printf("%s %s\n", __func__, ok ? "success" : "fail");
}

//
// THE REST IS FOR FUTURE REFERENCE
//

#if 0
auto Title_LoadGame = reinterpret_cast<void (__fastcall *)(void)>(Title_FindSaveGame_addr);

auto WinMain_addr = 0x00484A20u;

// sizeof(tempstr) == 64
auto tempstr = reinterpret_cast<char*>(0x005DDE28);
// sizeof(game00_abspath) == 256
auto game00_abspath = reinterpret_cast<char*>(0x005DE918);
// sizeof(save_dir_abspath) == 64
auto save_dir_abspath = reinterpret_cast<char*>(0x0061B770);
auto did_find_game00 = reinterpret_cast<char*>(0x005DEA18); // this is actually a char

// approx manual decompile of 0x004190AE
void __fastcall load_title_game00()
{
    char buffer[48];
    did_find_game00 = false;
    _sprintf(game00_abspath, "%s%s", save_dir_abspath, "\\Game00.sav");
    _searchenv(game00_abspath, nullptr, buffer);
    if (buffer) {
        did_find_game00 |= 0x80;
    }
}
#endif
