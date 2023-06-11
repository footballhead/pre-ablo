#pragma once

#include "DDRAW.H"

#include "palette.h"

#include <vector>

class DirectDrawSurface : public IDirectDrawSurface {
 public:
  ~DirectDrawSurface();

  //
  // IUnknown methods
  //

  HRESULT __stdcall QueryInterface(REFIID riid,
                                   LPVOID FAR *ppvObj) noexcept override;

  ULONG __stdcall AddRef() noexcept override;

  ULONG __stdcall Release() noexcept override;

  //
  // IDirectDrawSurface methods
  //

  HRESULT __stdcall AddAttachedSurface(LPDIRECTDRAWSURFACE) noexcept override;

  HRESULT __stdcall AddOverlayDirtyRect(LPRECT) noexcept override;

  HRESULT __stdcall Blt(LPRECT, LPDIRECTDRAWSURFACE, LPRECT, DWORD,
                        LPDDBLTFX) noexcept override;

  HRESULT __stdcall BltBatch(LPDDBLTBATCH, DWORD, DWORD) noexcept override;

  HRESULT __stdcall BltFast(DWORD, DWORD, LPDIRECTDRAWSURFACE, LPRECT,
                            DWORD) noexcept override;

  HRESULT __stdcall DeleteAttachedSurface(
      DWORD, LPDIRECTDRAWSURFACE) noexcept override;

  HRESULT __stdcall EnumAttachedSurfaces(
      LPVOID, LPDDENUMSURFACESCALLBACK) noexcept override;

  HRESULT __stdcall EnumOverlayZOrders(
      DWORD, LPVOID, LPDDENUMSURFACESCALLBACK) noexcept override;

  HRESULT __stdcall Flip(LPDIRECTDRAWSURFACE, DWORD) noexcept override;

  HRESULT __stdcall GetAttachedSurface(
      LPDDSCAPS, LPDIRECTDRAWSURFACE FAR *) noexcept override;

  HRESULT __stdcall GetBltStatus(DWORD) noexcept override;

  HRESULT __stdcall GetCaps(LPDDSCAPS) noexcept override;

  HRESULT __stdcall GetClipper(LPDIRECTDRAWCLIPPER FAR *) noexcept override;

  HRESULT __stdcall GetColorKey(DWORD, LPDDCOLORKEY) noexcept override;

  HRESULT __stdcall GetDC(HDC FAR *) noexcept override;

  HRESULT __stdcall GetFlipStatus(DWORD) noexcept override;

  HRESULT __stdcall GetOverlayPosition(LPLONG, LPLONG) noexcept override;

  HRESULT __stdcall GetPalette(LPDIRECTDRAWPALETTE FAR *) noexcept override;

  HRESULT __stdcall GetPixelFormat(LPDDPIXELFORMAT) noexcept override;

  HRESULT __stdcall GetSurfaceDesc(LPDDSURFACEDESC) noexcept override;

  // Initialize a DirectDrawSurface.  This member is provided for compliance
  // with the Common Object Model (COM) protocol.  Since the DirectDrawSurface
  // object is initialized when it is created, calling this member will always
  // result in the ALREADYINITIALIZED return value.
  HRESULT __stdcall Initialize(
      LPDIRECTDRAW lpDD, LPDDSURFACEDESC lpDDSurfaceDesc) noexcept override;

  // Determine if the surface memory associated with a DirectDrawSurface has
  // been freed.  If the memory has not been freed, this member will return OK.
  // The Restore member can be used to reallocate surface memory.  When a
  // DirectDrawSurface object loses its surface memory, most members will return
  // SURFACELOST and perform no other function.
  //
  // Surfaces can lose their memory when the mode of the display card is
  // changed, or because an application received exclusive access to the display
  // card and freed all of the surface memory currently allocated on the video
  // card.
  HRESULT __stdcall IsLost() noexcept override;

  // Obtain a valid pointer to the surface memory.  DirectDraw relies on the
  // application calling Unlock.  It is illegal behavior to Blt from a region of
  // a surface that is locked.
  //
  // An application should call the Lock member with a RECT structure specifying
  // the rectangle on the surface that the application wants access to.  If the
  // application calls Lock with a NULL RECT then the application is assumed to
  // be requesting exclusive access to the entire piece of surface memory.
  //
  // The Lock member fills in a DDSURFACEDESCstructure with the information
  // needed by the application to access the surface memory.  This information
  // includes the stride (or pitch) and the pixel format of the surface if it is
  // different from the pixel format of the primary surface.  The DDSURFACEDESC
  // structure also contains a pointer to the surface memory. Since it is
  // possible to call Lock multiple times for the same Surface with different
  // destination rectangles, this pointer is used to tie the Lock and Unlock
  // calls together.
  //
  // Normally, Lock will return immediately with an error when a lock cannot be
  // obtained because a blit is in progress.  The DDLOCK_WAIT flag can be used
  // to alter this behavior.
  //
  // In order to prevent VRAM from being lost during access to a surface,
  // DirectDraw holds the Win16 lock between Lock and Unlock operations.  The
  // Win16 lock is the critical section used to serialize access to GDI and
  // USER. Although this technique allows direct access to video memory and
  // prevents other applications from changing the mode during this access, it
  // will stop Windows from running, so Lock/Unlock and GetDC/ReleaseDC periods
  // should be kept short.  Unfortunately, because Windows is stopped, GUI
  // debuggers cannot be used in between Lock/Unlock and GetDC/ReleaseDC
  // operations.
  HRESULT __stdcall Lock(LPRECT lpDestRect, LPDDSURFACEDESC lpDDSurfaceDesc,
                         DWORD dwFlags, HANDLE hEvent) noexcept override;

  HRESULT __stdcall ReleaseDC(HDC) noexcept override;

  HRESULT __stdcall Restore(THIS) noexcept override;

  HRESULT __stdcall SetClipper(LPDIRECTDRAWCLIPPER) noexcept override;

  HRESULT __stdcall SetColorKey(DWORD, LPDDCOLORKEY) noexcept override;

  HRESULT __stdcall SetOverlayPosition(LONG, LONG) noexcept override;

  // Attach the DIRECTDRAWPALETTE specified to a Surface.  The Surface will use
  // this Palette for all subsequent operations.  The palette change takes place
  // immediately, without regard to refresh timing.
  HRESULT __stdcall SetPalette(
      LPDIRECTDRAWPALETTE lpDDPalette) noexcept override;

  // Notify DirectDraw that the direct surface manipulations are complete.
  HRESULT __stdcall Unlock(LPVOID lpSurfaceData) noexcept override;

  HRESULT __stdcall UpdateOverlay(LPRECT, LPDIRECTDRAWSURFACE, LPRECT, DWORD,
                                  LPDDOVERLAYFX) noexcept override;

  HRESULT __stdcall UpdateOverlayDisplay(DWORD) noexcept override;

  HRESULT __stdcall UpdateOverlayZOrder(DWORD,
                                        LPDIRECTDRAWSURFACE) noexcept override;

 private:
  struct Dimensions {
    DWORD dwWidth = 0;
    DWORD dwHeight = 0;
  };

  ULONG ref_count_ = 0;
  DirectDrawPalette* lpDDPalette_ = nullptr;
  unsigned int texture_ = 0;
  Dimensions dimensions_{};
  std::vector<std::byte> byte_buffer_;
  std::vector<PALETTEENTRY> color_buffer_;
};