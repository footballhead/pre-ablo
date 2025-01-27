#include "patches.hpp"

#include "enums.hpp"
#include "functions.hpp"
#include "macros.hpp"
#include "util.hpp"
#include "variables.hpp"

#include <cstdlib>

#define PATCH_NAME load_mpq_customizations
DESCRIBE_PATCH(R"txt(DO NOT TURN OFF! If you do, delete last_patch.txt in game directory.

Each patch can load an additional MPQ. The contents of the patch MPQ will take precedence over DIABDAT.MPQ, allowing us to override original game files with our own at runtime.)txt");

namespace {

// This is a wrapper around MessageBox. Fun fact: it's actually dead code!
// NOTE: The message, after formatting, must be less than 512 bytes. Otherwise the local buffer will overrun, causing the game to crash.
void (__stdcall * DiabloDebugMessageBox)(LPCSTR fmt, ...) = reinterpret_cast<void (__stdcall*)(LPCSTR, ...)>(0x0047771E);

// 0x004979B6 is the jmp thunk table entry for this imported function
BOOL (__stdcall * SFileOpenArchive)(const char *szMpqName, DWORD dwPriority, DWORD dwFlags, HANDLE *phMpq) = reinterpret_cast<BOOL (__stdcall*)(const char *, DWORD, DWORD, HANDLE *)>(0x004979B6);

// This replaces a call to SFileOpenArchive, so both the calling convention and signature must match.
BOOL __stdcall LoadDiabdat_patch(const char* szMpqName, DWORD dwPriority, DWORD dwFlags, HANDLE* phMpq)
{
    // Load the original MPQ (DIABDAT.mpq) using the parameters we were given
    printf("MPQ: Loading %s...\n", szMpqName);
    BOOL ok = SFileOpenArchive(szMpqName, dwPriority, dwFlags, phMpq);
    printf("MPQ: %s: %s\n", szMpqName, ok ? "success" : "FAIL!");
    // The game will hang if DIABDAT.MPQ can't be found. Print an error and exit to prevent the hang.
    if (!ok) {
        DiabloDebugMessageBox("DIABDAT.MPQ is mssing. Please reinstall and try again.");
        ExitProcess(1);
    }

    // FYI, here are the arguments at the patched call site. They're largely irrelevant.
    //  - szMpqName is `"diabdat.mpq"`
    //  - dwPriority is `0`
    //  - dwFlags is `0`
    //  - phMpq is `&mpq_handle` (address to a global)

    // Load all patch MPQs (only if the user selected them)
    for (auto const& patch : get_patches()) {
        if (patch.checked && patch.mpq_filename) {
            HANDLE patch_mpq;
            printf("MPQ: Loading %s...\n", patch.mpq_filename);
            ok &= SFileOpenArchive(patch.mpq_filename, 1000, 0, &patch_mpq);
            printf("MPQ: %s: %s\n", patch.mpq_filename, ok ? "success" : "FAIL!");
            // Leak the MPQ handle. We don't really care and the OS should clean up when the game exits.
        }
    }

    if (!ok) {
        // I could list all the MPQs in the message but then I risk going over 512 characters and crashing the game
        DiabloDebugMessageBox("Some PRE-ABLO patch MPQs could not be loaded. They could be missing because the install is corrupted. Please reinstall and try again.");
    }

    return ok;
}

} // namespace

PATCH_MAIN
{
    // Replace MPQ loading with our own
    return patch_call(0x00485448, (void*)LoadDiabdat_patch);
}

