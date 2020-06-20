// Problem areas:
//
// In general, .data:0061B770 (savedir_abspath) is widely used and only 64 chars
//
// fn 004190AE:
//  * local buffer [ebp-34h] is 52 long (too small)
//  * uses savedir_abspath
//
// fn 00460D3F:
//  * references savedir_abspath
//
// fn 00461F5F:
//  * references savedir_abspath
//
// fn 00462FD5:
//  * references savedir_abspath (multiple times)
//
// fn 0046371F:
//  * references savedir_abspath (multiple times)
//
// fn 00484BBB (winmain)
//  *  references savedir_abspath (a lot)
//  * .text:00484B44 only run if GetDriveType(NULL) == DRIVE_CDROM

#include <array>
#include <cstdint>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

namespace {

//
// Imports
//

auto did_find_game00 = reinterpret_cast<uint8_t*>(0x005DEA18);
auto game00_abspath = reinterpret_cast<char*>(0x005DE918); // sizeof == 256

constexpr auto Title_FindSaveGame_addr = 0x004190AEu;

//
// Locals
//

char larger_savedir_abspath[127] = { '\0' };

//
// Helpers
//

template <typename T>
bool patch(void* addr_to_patch, T new_val)
{
    // If we don't turn the .text address to be PAGE_EXECUTE_READWRITE then the game crashes
    DWORD oldProtect = 0;
    if (!VirtualProtect(addr_to_patch, sizeof(T), PAGE_EXECUTE_READWRITE, &oldProtect)) {
        printf("%s: failed to mark address +xrw: 0x%X\n", __func__, addr_to_patch);
        return false;
    }
    // Do the actual patch
    *(T*)(addr_to_patch) = new_val;
    return true;
}

//
// Hooks
//

void __fastcall Title_FindSaveGame_Fix()
{
    printf("%s\n", __func__);

    // The only reason we hook this function is that patching just the stack local buffer size is harder than reimplmenting the entire function
    char buffer[127] = { '\0' };
    *did_find_game00 = 0;
    // We're pretty lucky that game00_abspath is 256 long, otherwise we'd need to do more work...
    sprintf(game00_abspath, "%s%s", larger_savedir_abspath, "\\Game00.sav");
    _searchenv(game00_abspath, nullptr, buffer);
    if (buffer) {
        // I have no idea why this isn't just a bool but this is literally what the dissasembly shows me so shrug.jpg
        *did_find_game00 |= 0x80;
    }
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
    ok &= patch<uint8_t>((void*)(Title_FindSaveGame_addr), 0xE9); // jmp opcode
    ok &= patch<int>((void*)(Title_FindSaveGame_addr + 1), (int)Title_FindSaveGame_Fix - (Title_FindSaveGame_addr + 5)); // jmp operand

    // Fix WinMain to use larger_savedir_abspath
    ok &= patch((void*)(0x00484BED + 1), larger_savedir_abspath);
    ok &= patch<uint8_t>((void*)(0x00484BF2 + 1), sizeof(larger_savedir_abspath)); // Tell GetCurrentDirectory that the buffer is bigger
    ok &= patch((void*)(0x00484BFA + 1), larger_savedir_abspath);
    ok &= patch((void*)(0x00484C04 + 1), larger_savedir_abspath);
    ok &= patch((void*)(0x00484C11 + 1), larger_savedir_abspath);
    // TODO: Fix GetDriveType(NULL) == DRIVE_CDROM branch?

    // Fix GM_LoadGame
    ok &= patch((void*)(0x00460D63 + 1), larger_savedir_abspath);
    // TODO: There's a stack allocated buffer 

    // Fix save_game
    ok &= patch((void*)(0x00462A4C + 1), larger_savedir_abspath);

    // Fix SaveLevel (normal level and setlevel)
    ok &= patch((void*)(0x00462A4C + 1), larger_savedir_abspath);
    ok &= patch((void*)(0x00463622 + 1), larger_savedir_abspath);

    // Fix LoadLevel (normal level and setlevel)
    ok &= patch((void*)(0x0046373E + 1), larger_savedir_abspath);
    ok &= patch((void*)(0x0046375F + 1), larger_savedir_abspath);

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
