#include "surface.h"

#include <gl/gl.h>

#include <cstdio>

#include "log.h"
#include "palette.h"
#include "stub.h"

// Some prints are very intensive so hide behind a macro. This is used in
// conjunction with NDEBUG
// #define LOG_VERBOSE 1

DirectDrawSurface::~DirectDrawSurface() {
  if (lpDDPalette_ != nullptr) {
    lpDDPalette_->Release();
  }
  if (texture_ != 0) {
    glDeleteTextures(1, &texture_);
  }
}

HRESULT DirectDrawSurface::QueryInterface(REFIID riid,
                                          LPVOID FAR *ppvObj) noexcept {
  TRACE(
      "DirectDrawSurface::QueryInterface(riid={%d-%d-%d-%d%d%d%d%d%d%d%d}, "
      "ppvObj=0x%p)\n",
      riid.Data1, riid.Data2, riid.Data3, riid.Data4[0], riid.Data4[1],
      riid.Data4[2], riid.Data4[3], riid.Data4[4], riid.Data4[5], riid.Data4[6],
      riid.Data4[7], ppvObj);
  // FYI IID_IDirectDraw == IID_IDirectDrawSurface == IID_IDirectDrawPalette
  if (riid == IID_IDirectDrawSurface || riid == __uuidof(IUnknown)) {
    *ppvObj = reinterpret_cast<LPVOID *>(this);
    AddRef();
    return S_OK;
  }

  return E_NOINTERFACE;
}

ULONG DirectDrawSurface::AddRef() noexcept {
  TRACE("DirectDrawSurface::AddRef()\n");
  return InterlockedIncrement(&ref_count_);
}

ULONG DirectDrawSurface::Release() noexcept {
  TRACE("DirectDrawSurface::Release()\n");
  // TODO: assert(ref_count_ > 0);
  auto current_count = InterlockedDecrement(&ref_count_);
  if (current_count == 0) {
    delete this;
  }
  return current_count;
}

STUB(DirectDrawSurface::AddAttachedSurface, LPDIRECTDRAWSURFACE);
STUB(DirectDrawSurface::AddOverlayDirtyRect, LPRECT);
STUB(DirectDrawSurface::Blt, LPRECT, LPDIRECTDRAWSURFACE, LPRECT, DWORD,
     LPDDBLTFX);
STUB(DirectDrawSurface::BltBatch, LPDDBLTBATCH, DWORD, DWORD);
STUB(DirectDrawSurface::BltFast, DWORD, DWORD, LPDIRECTDRAWSURFACE, LPRECT,
     DWORD);
STUB(DirectDrawSurface::DeleteAttachedSurface, DWORD, LPDIRECTDRAWSURFACE);
STUB(DirectDrawSurface::EnumAttachedSurfaces, LPVOID, LPDDENUMSURFACESCALLBACK);
STUB(DirectDrawSurface::EnumOverlayZOrders, DWORD, LPVOID,
     LPDDENUMSURFACESCALLBACK);
STUB(DirectDrawSurface::Flip, LPDIRECTDRAWSURFACE, DWORD);
STUB(DirectDrawSurface::GetAttachedSurface, LPDDSCAPS,
     LPDIRECTDRAWSURFACE FAR *);
STUB(DirectDrawSurface::GetBltStatus, DWORD);
STUB(DirectDrawSurface::GetCaps, LPDDSCAPS);
STUB(DirectDrawSurface::GetClipper, LPDIRECTDRAWCLIPPER FAR *);
STUB(DirectDrawSurface::GetColorKey, DWORD, LPDDCOLORKEY);
STUB(DirectDrawSurface::GetDC, HDC FAR *);
STUB(DirectDrawSurface::GetFlipStatus, DWORD);
STUB(DirectDrawSurface::GetOverlayPosition, LPLONG, LPLONG);
STUB(DirectDrawSurface::GetPalette, LPDIRECTDRAWPALETTE FAR *);
STUB(DirectDrawSurface::GetPixelFormat, LPDDPIXELFORMAT);
STUB(DirectDrawSurface::GetSurfaceDesc, LPDDSURFACEDESC);

HRESULT DirectDrawSurface::Initialize(
    LPDIRECTDRAW lpDD, LPDDSURFACEDESC lpDDSurfaceDesc) noexcept {
  TRACE("DirectDrawSurface::Initialize(lpDD=0x%p, lpDDSurfaceDesc=0x%p)\n",
        lpDD, lpDDSurfaceDesc);

  constexpr static auto kDesiredFlags = DDSD_WIDTH | DDSD_HEIGHT;
  if ((lpDDSurfaceDesc->dwFlags & kDesiredFlags) != kDesiredFlags) {
    return DDERR_GENERIC;
  }

  const auto &width = lpDDSurfaceDesc->dwWidth;
  const auto &height = lpDDSurfaceDesc->dwHeight;
  dimensions_ = Dimensions{
      .dwWidth = width,
      .dwHeight = height,
  };

  const auto area = width * height;
  byte_buffer_.resize(area);
  color_buffer_.resize(area);

  // TODO: dimensions must be power of 2??
  glGenTextures(1, &texture_);
  glBindTexture(GL_TEXTURE_2D, texture_);

  // TODO:
  // https://www.khronos.org/opengl/wiki/Common_Mistakes#Slow_pixel_transfer_performance
  // suggests using BGRA (but I don't have that in OpenGL 1.1)
  glTexImage2D(GL_TEXTURE_2D, /*level=*/0, /*internalFormat=*/GL_RGBA8, width,
               height, /*border=*/0, GL_RGBA, GL_UNSIGNED_BYTE,
               color_buffer_.data());

  // TODO: Prefer NEAREST with pixel perfect scaling?
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  // Don't repeat, so we don't have that yellow bar at the top of the screen
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

  return DDERR_ALREADYINITIALIZED;
}

HRESULT DirectDrawSurface::IsLost() noexcept {
#ifdef LOG_VERBOSE
  TRACE("DirectDrawSurface::IsLost()\n");
#endif
  // We don't implement the same surface exclusion mechanism because we are not
  // under the same resource constraints
  return DD_OK;
}

HRESULT DirectDrawSurface::Lock(LPRECT lpDestRect,
                                LPDDSURFACEDESC lpDDSurfaceDesc, DWORD dwFlags,
                                HANDLE hEvent) noexcept {
#ifdef LOG_VERBOSE
  TRACE(
      "DirectDrawSurface::Lock(lpDestRect=0x%p, lpDDSurfaceDesc=0x%p, "
      "dwFlags=0x%x, hEvent=0x%p)\n",
      lpDestRect, lpDDSurfaceDesc, dwFlags, hEvent);
#endif
  // dwFlags=0x1 (DDLOCK_WAIT)
  // TODO: Fill in lpDDSurfaceDesc appropriately

  // DrawMain (interfac_PaintQuotes) uses lPitch, lpSurface. Return is ignored
  lpDDSurfaceDesc->lPitch = dimensions_.dwWidth;
  lpDDSurfaceDesc->lpSurface = byte_buffer_.data();
#ifdef LOG_VERBOSE
  TRACE("lpDDSurfaceDesc->lpSurface = 0x%p\n", byte_buffer_.data());
#endif

  return DD_OK;
}

STUB(DirectDrawSurface::ReleaseDC, HDC);
STUB(DirectDrawSurface::Restore, THIS);
STUB(DirectDrawSurface::SetClipper, LPDIRECTDRAWCLIPPER);
STUB(DirectDrawSurface::SetColorKey, DWORD, LPDDCOLORKEY);
STUB(DirectDrawSurface::SetOverlayPosition, LONG, LONG);

HRESULT DirectDrawSurface::SetPalette(
    LPDIRECTDRAWPALETTE lpDDPalette) noexcept {
  TRACE("DirectDrawSurface::SetPalette(lpDDPalette=0x%p)\n", lpDDPalette);

  // This is a bit of a logical jump but if they're using our DLL this is
  // probably our DirectDrawPalette and not some other implementation
  DirectDrawPalette *myDDPalette =
      static_cast<DirectDrawPalette *>(lpDDPalette);
  myDDPalette->AddRef();
  lpDDPalette_ = myDDPalette;

  return DD_OK;
}

HRESULT DirectDrawSurface::Unlock(LPVOID lpSurfaceData) noexcept {
#ifdef LOG_VERBOSE
  TRACE("DirectDrawSurface::Unlock(lpSurfaceData=0x%p)\n", lpSurfaceData);
#endif

  // The docs say that lpSurfaceData should be a valid pointer but (outside of
  // cutscenes) it is nullptr. Assume it is the thing we locked: byte_buffer_
  //
  // We narrowly expect this to only be byte_buffer_ because we only ever return
  // byte_buffer_ for Lock()
  if (lpSurfaceData == nullptr) {
    lpSurfaceData = byte_buffer_.data();
  }

  // TODO: Perform this in a shader instead. Consider OpenGL 3
  const PALETTEENTRY *raw_palette = lpDDPalette_->GetRawEntries();
  for (size_t i = 0; i < byte_buffer_.size(); ++i) {
    color_buffer_[i] = raw_palette[static_cast<BYTE>(byte_buffer_[i])];
  }

  const auto &width = dimensions_.dwWidth;
  const auto &height = dimensions_.dwHeight;

  // We have one texture and it's bound in Initialize
  glTexSubImage2D(GL_TEXTURE_2D, /*level=*/0, /*xoffset=*/0, /*yoffset=*/0,
                  width, height, GL_RGBA, GL_UNSIGNED_BYTE,
                  color_buffer_.data());

  glClearColor(0.F, 0.F, 0.F, 1.F);
  glClear(GL_COLOR_BUFFER_BIT);

  // The tex coords are vertically flipped because ddraw surfaces are defined
  // bottom-up. Vertex coords are in NDC
  glBegin(GL_QUADS);

  glTexCoord2f(0.F, 1.F);
  glVertex3f(-1.F, -1.F, 0.F);

  glTexCoord2f(1.F, 1.F);
  glVertex3f(1.F, -1.F, 0.F);

  glTexCoord2f(1.F, 0.F);
  glVertex3f(1.F, 1.F, 0.F);

  glTexCoord2f(0.F, 0.F);
  glVertex3f(-1.F, 1.F, 0.F);

  glEnd();

  glFinish();

  return DD_OK;
}

STUB(DirectDrawSurface::UpdateOverlay, LPRECT, LPDIRECTDRAWSURFACE, LPRECT,
     DWORD, LPDDOVERLAYFX);
STUB(DirectDrawSurface::UpdateOverlayDisplay, DWORD);
STUB(DirectDrawSurface::UpdateOverlayZOrder, DWORD, LPDIRECTDRAWSURFACE);
