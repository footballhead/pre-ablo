#include "palette.h"

#include <cstdio>

#include "log.h"
#include "stub.h"

// Some prints are very intensive so hide behind a macro. This is used in
// conjunction with NDEBUG
// #define LOG_VERBOSE 1

HRESULT DirectDrawPalette::QueryInterface(REFIID riid,
                                          LPVOID FAR *ppvObj) noexcept {
  TRACE(
      "DirectDrawPalette::QueryInterface(riid={%d-%d-%d-%d%d%d%d%d%d%d%d}, "
      "ppvObj=0x%p)\n",
      riid.Data1, riid.Data2, riid.Data3, riid.Data4[0], riid.Data4[1],
      riid.Data4[2], riid.Data4[3], riid.Data4[4], riid.Data4[5], riid.Data4[6],
      riid.Data4[7], ppvObj);
  // FYI IID_IDirectDraw == IID_IDirectDrawSurface == IID_IDirectDrawPalette
  if (riid == IID_IDirectDrawPalette || riid == __uuidof(IUnknown)) {
    *ppvObj = reinterpret_cast<LPVOID *>(this);
    AddRef();
    return S_OK;
  }

  return E_NOINTERFACE;
}

ULONG DirectDrawPalette::AddRef() noexcept {
  TRACE("DirectDrawPalette::AddRef()\n");
  return InterlockedIncrement(&ref_count_);
}

ULONG DirectDrawPalette::Release() noexcept {
  TRACE("DirectDrawPalette::Release()\n");
  // TODO: assert(ref_count_ > 0);
  auto current_count = InterlockedDecrement(&ref_count_);
  if (current_count == 0) {
    delete this;
  }
  return current_count;
}

STUB(DirectDrawPalette::GetCaps, LPDWORD);
STUB(DirectDrawPalette::GetEntries, DWORD, DWORD, DWORD, LPPALETTEENTRY);

HRESULT DirectDrawPalette::Initialize(LPDIRECTDRAW lpDD, DWORD dwFlags,
                                      LPPALETTEENTRY lpDDColorTable) noexcept {
  TRACE(
      "DirectDrawPalette::Initialize(lpDD=0x%p, dwFlags=0x%x, "
      "lpDDColorTable=0x%p)\n",
      lpDD, dwFlags, lpDDColorTable);

#ifndef NDEBUG
#ifdef LOG_VERBOSE
  TRACE("  DDColorTable=\n");
  for (int i = 0; i < 256; ++i) {
    TRACE("    [%d] = {%d, %d, %d, %d}\n", i, lpDDColorTable[i].peRed,
          lpDDColorTable[i].peGreen, lpDDColorTable[i].peBlue,
          lpDDColorTable[i].peFlags);
  }
#endif
#endif

  // TODO: assert(dwFlags & DDPCAPS_8BIT == DDPCAPS_8BIT)
  memcpy(&colors_, lpDDColorTable, sizeof(PALETTEENTRY) * k8BitColorCount);

  return DDERR_ALREADYINITIALIZED;
}

HRESULT DirectDrawPalette::SetEntries(DWORD dwFlags, DWORD dwStartingEntry,
                                      DWORD dwCount,
                                      LPPALETTEENTRY lpEntries) noexcept {
  TRACE(
      "DirectDrawPalette::SetEntries(dwFlags=0x%x, dwStartingEntry=%u, "
      "dwCount=%u, lpEntries=0x%p)\n",
      dwFlags, dwStartingEntry, dwCount, lpEntries);

  // TODO: In OpenGL 3 turn this into a texture upload

#ifndef NDEBUG
#ifdef LOG_VERBOSE
  TRACE("  ColorTable=\n");
  for (DWORD i = dwStartingEntry; i < dwStartingEntry + dwCount; ++i) {
    TRACE("    [%u] = {%d, %d, %d, %d}\n", i, lpEntries[i].peRed,
          lpEntries[i].peGreen, lpEntries[i].peBlue, lpEntries[i].peFlags);
  }
#endif
#endif

  // TODO: assert(dwFlags & DDPCAPS_8BIT == DDPCAPS_8BIT)
  memcpy(&colors_[dwStartingEntry], lpEntries, sizeof(PALETTEENTRY) * dwCount);

  return DD_OK;
}
