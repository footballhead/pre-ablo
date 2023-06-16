#pragma once

#include <array>

#include "DDRAW.H"

constexpr auto k8BitColorCount = 256;

class DirectDrawPalette : public IDirectDrawPalette {
 public:
  //
  // IUnknown methods
  //

  HRESULT __stdcall QueryInterface(REFIID riid,
                                   LPVOID FAR* ppvObj) noexcept override;

  ULONG __stdcall AddRef() noexcept override;

  ULONG __stdcall Release() noexcept override;

  //
  // IDirectDrawPalette methods
  //

  HRESULT __stdcall GetCaps(LPDWORD) noexcept override;

  HRESULT __stdcall GetEntries(DWORD, DWORD, DWORD,
                               LPPALETTEENTRY) noexcept override;

  // This member is provided for compliance with the Common Object Model (COM)
  // protocol.  Since the DirectDrawPalette object is initialized when it is
  // created, calling this member will always result in the ALREADYINITIALIZED
  // return value.
  //
  // Parameters:
  //   lpDD: Points to the DIRECTDRAWSTUCTURE representing the DirectDraw
  //     object.
  //   dwFlags: Not used.
  //   lpDDColorTable: Not used.
  //
  // Return Values: DDERR_ALREADYINITIALIZED
  HRESULT __stdcall Initialize(LPDIRECTDRAW lpDD, DWORD dwFlags,
                               LPPALETTEENTRY lpDDColorTable) noexcept override;

  // Change entries in a DirectDrawPalette.  The changes will be performed
  // immediately.  The palette must be attached to a surface using the
  // SetPalette member before SetEntries can be used
  //
  // Parameters:
  //   dwFlags: Not currently used.  Must be zero.
  //   dwStartingEntry: The first entry to be set
  //   dwCount: The number of palette entries to be changed.
  //   lpPixelEntryReplacements: The palette entries are one byte each if the
  //     DDPCAPS_8BITENTRIES field is set and four bytes otherwise.  Each field
  //     is a color description.
  HRESULT __stdcall SetEntries(DWORD dwFlags, DWORD dwStartingEntry,
                               DWORD dwCount,
                               LPPALETTEENTRY lpEntries) noexcept override;

  //
  // Internal API
  //

  const PALETTEENTRY* GetRawEntries() const { return colors_.data(); }

 private:
  ULONG ref_count_ = 0;
  std::array<PALETTEENTRY, k8BitColorCount> colors_{};
};