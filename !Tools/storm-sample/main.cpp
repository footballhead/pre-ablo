#include <windows.h>
#include "Shlwapi.h"

#include <fstream>

#include <cstdio>
#include <vector>
#include <string>

namespace {

#define STORM_PATH "STORM.DLL"
#define DIABDAT_PATH "DIABDAT.MPQ"

/// @param archivename Needs to be absolute
typedef BOOL (__stdcall *SFileOpenArchive)(LPCTSTR archivename, int priority, BOOL cdonly, HANDLE* handle);
typedef BOOL (__stdcall *SFileCloseArchive)(HANDLE* handle);
typedef BOOL (__stdcall *SFileOpenFile)(LPCSTR filename, HANDLE* handle);
typedef BOOL (__stdcall *SFileCloseFile)(HANDLE handle);
typedef BOOL (__stdcall *SFileReadFile)(HANDLE handle, LPVOID buffer, DWORD bytestoread, LPDWORD bytesread, LPVOID reserved);

SFileOpenArchive pSFileOpenArchive = nullptr;
SFileCloseArchive pSFileCloseArchive = nullptr;
SFileOpenFile pSFileOpenFile = nullptr;
SFileCloseFile pSFileCloseFile = nullptr;
SFileReadFile pSFileReadFile = nullptr;

HMODULE stormDll = nullptr;
HANDLE diabdat = nullptr;

constexpr auto bufferSize = 1024u*1024u*4u;  // 4mb
char buffer[bufferSize];

}; // namespace

void cleanup()
{
    if (diabdat && pSFileCloseArchive && !pSFileCloseArchive(&diabdat)) {
        fprintf(stderr, "Failed to close diabdat.mpq\n");
    }
    if (stormDll && !FreeLibrary(stormDll)) {
        fprintf(stderr, "Failed to free storm.dll: %d\n", GetLastError());
    }
}

int main(int argc, char** argv)
{
    //
    // Open STORM.DLL
    //

    stormDll = LoadLibrary(STORM_PATH);
    if (!stormDll) {
        fprintf(stderr, "Failed to load storm.dll: %d\n", GetLastError());
        return 1;
    }

    //
    // Get addresses of functions we need
    //

    pSFileOpenArchive = (SFileOpenArchive)GetProcAddress(stormDll, "SFileOpenArchive");
    if (!pSFileOpenArchive) {
        fprintf(stderr, "Failed to get SFileOpenArchive: %d\n", GetLastError());
        cleanup();
        return 1;
    }

    pSFileCloseArchive = (SFileCloseArchive)GetProcAddress(stormDll, "SFileCloseArchive");
    if (!pSFileCloseArchive) {
        fprintf(stderr, "Failed to get SFileCloseArchive: %d\n", GetLastError());
        cleanup();
        return 1;
    }

    pSFileOpenFile = (SFileOpenFile)GetProcAddress(stormDll, "SFileOpenFile");
    if (!pSFileOpenFile) {
        fprintf(stderr, "Failed to get SFileOpenFile: %d\n", GetLastError());
        cleanup();
        return 1;
    }

    pSFileCloseFile = (SFileCloseFile)GetProcAddress(stormDll, "SFileCloseFile");
    if (!pSFileCloseFile) {
        fprintf(stderr, "Failed to get SFileCloseFile: %d\n", GetLastError());
        cleanup();
        return 1;
    }

    pSFileReadFile = (SFileReadFile)GetProcAddress(stormDll, "SFileReadFile");
    if (!pSFileReadFile) {
        fprintf(stderr, "Failed to get SFileReadFile: %d\n", GetLastError());
        cleanup();
        return 1;
    }

    //
    // Open DIABDAT.MPQ, open MUSIC\DINTRO.WAV, read it, and save to disk
    //

    TCHAR curdir[MAX_PATH];
    if (!GetCurrentDirectory(MAX_PATH, curdir)) {
        fprintf(stderr, "Failed to get current directory: %d\n", GetLastError());
        cleanup();
        return 1;
    }

    if (!PathAppend(curdir, DIABDAT_PATH)) {
        fprintf(stderr, "Failed to append '%s' and '%s': %d\n", curdir, DIABDAT_PATH, GetLastError());
        cleanup();
        return 1;
    }

    if (!pSFileOpenArchive(curdir, 0, FALSE, &diabdat)) {
        fprintf(stderr, "Failed to open %s\n", curdir);
        cleanup();
        return 1;
    }

    //
    // Open dintro.wav and save to disk
    //

    HANDLE dintroWav = nullptr;
    memset(buffer, 0, bufferSize);
    if (!pSFileOpenFile("MUSIC\\DINTRO.WAV", &dintroWav)) {
        fprintf(stderr, "Failed to open file");
        cleanup();
        return 1;
    }

    DWORD readBytes = 0;
    pSFileReadFile(dintroWav, buffer, bufferSize, &readBytes, nullptr);
    if (readBytes > 0) {
        std::ofstream out{"dintro.wav", std::ios_base::binary};
        out.write(buffer, readBytes);
        if (out.good()) {
            fprintf(stderr, "saved to dintro.wav\n");
        } else {
            fprintf(stderr, "save failed\n");
        }
    } else {
        fprintf(stderr, "read failed: readBytes=%d\n", readBytes);
    }

    if (!pSFileCloseFile(dintroWav)) {
        fprintf(stderr, "failed to close\n");
    }

    //
    // Clean up
    //

    cleanup();
    return 0;
}
