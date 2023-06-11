#pragma once

#include <optional>

#include "DDRAW.H"
#include "log.h"

class DirectDraw : public IDirectDraw {
 public:
  struct OpenGlState {
    HDC hdc = nullptr;
    HGLRC hglrc = nullptr;
  };

  struct DisplayModeParams {
    DWORD dwWidth = 0;
    DWORD dwHeight = 0;
  };

  ~DirectDraw();

  //
  // IUnknown methods
  //

  HRESULT __stdcall QueryInterface(REFIID riid,
                                   LPVOID FAR *ppvObj) noexcept override;

  ULONG __stdcall AddRef() noexcept override;

  ULONG __stdcall Release() noexcept override;

  //
  // IDirectDraw methods
  //

  HRESULT __stdcall Compact() noexcept override;

  HRESULT __stdcall CreateClipper(DWORD dwFlags,
                                  LPDIRECTDRAWCLIPPER FAR *lplpDDClipper,
                                  IUnknown FAR *pUnkOuter) noexcept override;

  // Create a DIRECTDRAWPALETTE object for  this DirectDraw object.
  //
  // Parameters:
  //   dwFlags:
  //     DDPCAPS_8BIT: Index is 8 bits.  There are 256 color entries in the
  //       palette table
  //   lpColorTable: Points to an array of  16 or 256 PALETTEENTRY structures
  //     that should be used to initialize this DIRECTDRAWPALETTE object
  //   lplpDDPaelette: Points to a pointer which will be filled in with the
  //     address of the new DIRECTDRAWPALETTE object if the CreatePalette member
  //     is successful.
  //   pUnkOuter: This parameter is provided for future compability with COM
  //     aggregation features.  Presently, however, CreatePalette will return an
  //     error if it is anything but NULL.
  //
  // Return Values: DD_OK or DDERR_*
  HRESULT __stdcall CreatePalette(DWORD dwFlags, LPPALETTEENTRY lpColorTable,
                                  LPDIRECTDRAWPALETTE FAR *lplpDDPalette,
                                  IUnknown FAR *pUnkOuter) noexcept override;

  // Create a DirectDrawSurface object for this DirectDraw object.  The
  // DirectDrawSurface object  represents a Surface (pixel memory), which
  // usually resides in video card memory but may exist in system memory if
  // video memory is exhausted or if explicitly requested.  The member will fail
  // if the hardware cannot provide support for the capabilities requested or
  // has previously allocated those resources to another DirectDrawSurface
  // object.
  //
  // CreateSurface usually creates one DirectDrawSurface object.  If the
  // DDSCAPS_FLIP flag, in the dwCaps field of the DDCAPS structure, which is
  // included in the DDSURFACEDESC structure, is set, however, CreateSurface
  // will create several DirectDrawSurface objects which are referred to
  // collectively as a Complex Structure.  The additional surfaces created are
  // also referred to as "implicit" surfaces.
  //
  // Note DirectDraw does not permit the creation of surfaces that are wider
  // than the primary surface.
  HRESULT __stdcall CreateSurface(LPDDSURFACEDESC lpDDSurfaceDesc,
                                  LPDIRECTDRAWSURFACE FAR *lplpDDSurface,
                                  IUnknown FAR *pUnkOuter) noexcept override;

  HRESULT __stdcall DuplicateSurface(
      LPDIRECTDRAWSURFACE lpDDSurface,
      LPDIRECTDRAWSURFACE FAR *lplpDupDDSurface) noexcept override;

  HRESULT __stdcall EnumDisplayModes(
      DWORD dwFlags, LPDDSURFACEDESC lpDDSurfaceDesc, LPVOID lpContext,
      LPDDENUMMODESCALLBACK lpEnumCallback) noexcept override;

  HRESULT __stdcall EnumSurfaces(
      DWORD dwFlags, LPDDSURFACEDESC lpDDSD, LPVOID lpContext,
      LPDDENUMSURFACESCALLBACK lpEnumCallback) noexcept override;

  HRESULT __stdcall FlipToGDISurface() noexcept override;

  HRESULT __stdcall GetCaps(LPDDCAPS lpDDDriverCaps,
                            LPDDCAPS lpDDHELCaps) noexcept override;

  HRESULT __stdcall GetDisplayMode(
      LPDDSURFACEDESC lpDDSurfaceDesc) noexcept override;

  HRESULT __stdcall GetFourCCCodes(DWORD FAR *lpNumCodes,
                                   DWORD FAR *lpCodes) noexcept override;

  HRESULT __stdcall GetGDISurface(
      LPDIRECTDRAWSURFACE FAR *lplpGDIDDSSurface) noexcept override;

  HRESULT __stdcall GetMonitorFrequency(
      LPDWORD lpdwFrequency) noexcept override;

  HRESULT __stdcall GetScanLine(LPDWORD lpdwScanLine) noexcept override;

  HRESULT __stdcall GetVerticalBlankStatus(LPBOOL lpbIsInVB) noexcept override;

  // Initialize the DirectDraw object.  This member is provided for compliance
  // with the Common Object Model (COM) protocol.  Since the DirectDraw object
  // is initialized when it is created, calling this member will always result
  // in the ALREADYINITIALIZED return value
  HRESULT __stdcall Initialize(GUID FAR *lpGUID) noexcept override;

  HRESULT __stdcall RestoreDisplayMode() noexcept override;

  // This member determines the top-level behavior of the application.
  // DDSCL_EXCLUSIVE level is needed to call functions that can have drastic
  // performance consequences for other applications.  In order to call Compact,
  // change the display mode, or modify the behavior  (e.g. flipping) of the
  // primary surface, an application must have obtained exclusive level.  If an
  // application calls SetCooperativeLevel with DDSCL_EXCLUSIVE and
  // DDSCL_FULLSCREEN, DirectDraw will attempt to resize its window to full
  // screen.  An application must either set the DDSCL_EXCLUSIVE or DDSCL_NORMAL
  // flags, and DDSCL_EXCLUSIVE requires DDSCL_FULLSCREEN.
  //
  // ModeX modes are only available if an application sets DDSCL_ALLOWMODEX |
  // DDSCL_FULLSCREEN | DDSCL_EXCLUSIVE.  DDSCL_ALLOWMODEX cannot be used with
  // DDSCL_NORMAL. If DDSCL_ALLOWMODEX is not specifed, EnumDisplayModes will
  // not enumerate the ModeX modes, and SetDisplayMode will fail when a ModeX
  // mode is requested.  The set of supported display modes may change after
  // using SetCooperativeLevel.
  //
  // Because the ModeX modes are not supported by Windows, when in a ModeX mode
  // you cannot Lock the primary surface, Blt to the primary surface, use GetDC
  // on the primary surface, or use GDI with a screen DC.  ModeX modes are
  // indicated by the DDCAPS_MODEX flag in the DDSCAPS field of the
  // DDSURFACEDESC structure returned by Surface::GetCaps and EnumDisplayModes.
  //
  // Parameters:
  //   hWnd: Window handle used for the application.
  //   dwFlags:
  //     DDSCL_EXCLUSIVE: Application requests exclusive level.
  //     DDSCL_FULLSCREEN: Exclusive mode owner will be responsible for the
  //       entire primary surface. GDI can be ignored.
  HRESULT __stdcall SetCooperativeLevel(HWND hWnd,
                                        DWORD dwFlags) noexcept override;

  // Set the mode of the display device hardware.  SetCooperativeLevel must be
  // used to set exclusive level access before the mode can be changed.  If
  // other applications have created a DirectDrawSurface object on the Primary
  // Surface and the mode is changed, those applications' Primary Surface
  // objects will return DDERR_SURFACELOST until they are restored.
  HRESULT __stdcall SetDisplayMode(DWORD dwWidth, DWORD dwHeight,
                                   DWORD dwBpp) noexcept override;

  HRESULT __stdcall WaitForVerticalBlank(DWORD dwFlags,
                                         HANDLE hEvent) noexcept override;

  //
  // Internal API
  //

  WNDPROC GetOldWndProc() const { return old_wndproc_; }
  std::optional<DisplayModeParams> GetDisplayMode() const {
    return display_mode_;
  }

 private:
  struct CooperativeLevelParams {
    HWND hwnd = nullptr;
  };

  ULONG ref_count_ = 0;
  // Initialized by SetCooperativeLevel. Required for most functions to work
  std::optional<CooperativeLevelParams> coop_level_;
  // Initialized by SetDisplayMode.
  std::optional<DisplayModeParams> display_mode_;
  // Initialized by SetDisplayMode.
  OpenGlState gl_{};
  WNDPROC old_wndproc_ = nullptr;
};
