#include "ddraw_impl.h"

#include <GL/glew.h>

#include "palette.h"
#include "stub.h"
#include "surface.h"

namespace {

void ConfigureFullscreenMode(HWND hwnd, RECT rcMonitor) {
  SetWindowLong(hwnd, GWL_STYLE, WS_POPUP | WS_VISIBLE);
  const auto width = rcMonitor.right - rcMonitor.left;
  const auto height = rcMonitor.bottom - rcMonitor.top;
  MoveWindow(hwnd, rcMonitor.left, rcMonitor.top, width, height,
             /*bRepaint=*/FALSE);
}

void ScaleOpenGlViewport(const DirectDraw::DisplayModeParams &display_mode,
                         int window_width, int window_height) {
  int centered_x = 0;
  int centered_y = 0;
  auto adjusted_width = window_width;
  auto adjusted_height = window_height;

  // TODO: Find a home for pixel perfect scaling
  // auto pixel_scale =
  //     min(width / display_mode->dwWidth, height / display_mode->dwHeight);
  // adjusted_width = display_mode->dwWidth * pixel_scale;
  // adjusted_height = display_mode->dwHeight * pixel_scale;

  // Scale using aspect ratio
  // TODO: Let the user stretch if they want
  if (window_width > window_height) {
    const double display_aspect_ratio =
        static_cast<double>(display_mode.dwWidth) / display_mode.dwHeight;
    adjusted_width = static_cast<GLsizei>(window_height * display_aspect_ratio);
  } else {
    const double display_aspect_ratio =
        static_cast<double>(display_mode.dwHeight) / display_mode.dwWidth;
    adjusted_height = static_cast<GLsizei>(window_width * display_aspect_ratio);
  }

  // Center in window
  centered_x = (window_width - adjusted_width) / 2;
  centered_y = (window_height - adjusted_height) / 2;

  glViewport(centered_x, centered_y, adjusted_width, adjusted_height);
}

// After this, you can make OpenGL calls.
std::optional<DirectDraw::OpenGlState> MakeOpenGlContext(HWND hwnd) {
  const HDC hdc = ::GetDC(hwnd);
  if (hdc == nullptr) {
    TRACE("::GetDC failed!");
    return std::nullopt;
  }

  // TODO: ReleaseDC(hdc) on error

  // These are essentially the settings from the OpenGL Wiki:
  // https://www.khronos.org/opengl/wiki/Creating_an_OpenGL_Context_(WGL)
  // TODO: Support single and double buffering
  static PIXELFORMATDESCRIPTOR pfd = {
      .nSize =
          sizeof(PIXELFORMATDESCRIPTOR),  // "This value should be set to
                                          // sizeof(PIXELFORMATDESCRIPTOR)."
      .nVersion = 1,                      // "This value should be set to 1"
      .dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL |
                 PFD_DEPTH_DONTCARE,  // TODO: double buffer?
      .iPixelType = PFD_TYPE_RGBA,
      .cColorBits = 32,  // TODO ask GetDC(nullptr)
      // The wiki suggests to set all these to 0 so here we are
      .cRedBits = 0,
      .cRedShift = 0,
      .cGreenBits = 0,
      .cGreenShift = 0,
      .cBlueBits = 0,
      .cBlueShift = 0,
      .cAlphaBits = 0,
      .cAlphaShift = 0,
      .cAccumBits = 0,
      .cAccumRedBits = 0,
      .cAccumGreenBits = 0,
      .cAccumBlueBits = 0,
      .cAccumAlphaBits = 0,
      .cDepthBits = 0,               // No Depth Buffer
      .cStencilBits = 0,             // No Stencil Buffer
      .cAuxBuffers = 0,              // No Auxiliary Buffer
      .iLayerType = PFD_MAIN_PLANE,  // "Ignored... no longer used"
      .bReserved = 0,                // Something something overlay
      .dwLayerMask = 0,              // "Ignored... no longer used"
      .dwVisibleMask = 0,            // Transparent pixel
      .dwDamageMask = 0,             // "Ignored... no longer used"
  };

  const int iPixelFormat = ChoosePixelFormat(hdc, &pfd);
  if (iPixelFormat == 0) {
    TRACE("ChoosePixelFormat failed: %d\n", GetLastError());
    return std::nullopt;
  }

  if (SetPixelFormat(hdc, iPixelFormat, &pfd) == FALSE) {
    TRACE("SetPixelFormat failed: %d\n", GetLastError());
    return std::nullopt;
  }

  // TODO: Store hrc on this class and wglDeleteContext later
  const HGLRC hglrc = wglCreateContext(hdc);
  if (hglrc == nullptr) {
    TRACE("wglCreateContext failed: %d\n", GetLastError());
    return std::nullopt;
  }

  wglMakeCurrent(hdc, hglrc);

  // TODO: For OpenGL 3.3 consult the wiki:
  // https://www.khronos.org/opengl/wiki/Creating_an_OpenGL_Context_(WGL)

  return DirectDraw::OpenGlState{
      .hdc = hdc,
      .hglrc = hglrc,
  };
}

void InitOpenGl() {
  // Important! We use texture mapping!
  glEnable(GL_TEXTURE_2D);
  // No depth, no shading

  // Set up orthographic projection so that drawing at Z=0 will be pixel perfect
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  // Keep NDC

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  glClearColor(0.F, 0.F, 0.F, 1.F);
  glClear(GL_COLOR_BUFFER_BIT);

  glFlush();
}

LRESULT CALLBACK OverrideWndProc(HWND hWnd, UINT Msg, WPARAM wParam,
                                 LPARAM lParam) {
  DirectDraw *lpDD =
      reinterpret_cast<DirectDraw *>(GetWindowLongPtr(hWnd, GWL_USERDATA));

  // TODO: SetCursorPos is relative to the screen.

  LPARAM lParamOverride = lParam;

  // TODO:
  // https://learn.microsoft.com/en-us/windows/win32/inputdev/using-mouse-input

  switch (Msg) {
    case WM_MOUSEMOVE: {
      POINTS ptMouse = MAKEPOINTS(lParam);
      TRACE("WM_MOUSEMOVE: ptMouse={%d, %d}\n", ptMouse.x, ptMouse.y);

      const auto display_mode = lpDD->GetDisplayMode();
      if (!display_mode) {
        break;
      }

      const auto width = static_cast<SHORT>(display_mode->dwWidth);
      const auto height = static_cast<SHORT>(display_mode->dwHeight);

      // Feed the game a corrected mouse position. This is not the acutal mouse
      // position but the value we give to the game is treated as such because
      // it doesnt know the wiser. This is necessary since it crashes if
      // ptMouse.x < 0 :X
      //
      // (Another way to solve this is to alter the game code to correct MouseX
      // and MouseY in the same way)
      if (ptMouse.x < 0) {
        ptMouse.x = 0;
      } else if (ptMouse.x >= width - 1) {
        ptMouse.x = width - 1;
      }

      if (ptMouse.y < 0) {
        ptMouse.y = 0;
      } else if (ptMouse.y >= height - 1) {
        ptMouse.x = height - 1;
      }

      lParamOverride = MAKELPARAM(ptMouse.x, ptMouse.y);
      break;
    }
    case WM_LBUTTONDOWN: {
      TRACE("WM_LBUTTONDOWN: Capture cursor\n");

      const auto display_mode = lpDD->GetDisplayMode();
      if (!display_mode) {
        break;
      }

      // SetCapture is required for ClipCursor to take effect and stick
      SetCapture(hWnd);
      POINT ptTL = {.x = 0, .y = 0};
      POINT ptBR = {
          .x = static_cast<LONG>(display_mode->dwWidth),
          .y = static_cast<LONG>(display_mode->dwHeight),
      };
      if (ClientToScreen(hWnd, &ptTL) && ClientToScreen(hWnd, &ptBR)) {
        RECT rcScreen = {
            .left = ptTL.x,
            .top = ptTL.y,
            .right = ptBR.x,
            .bottom = ptBR.y,
        };
        ClipCursor(&rcScreen);
      }
      break;
    }
    case WM_KILLFOCUS:
      TRACE("WM_KILLFOCUS: Release cursor\n");
      ClipCursor(NULL);
      ReleaseCapture();
      break;
  }

  return CallWindowProc(lpDD->GetOldWndProc(), hWnd, Msg, wParam,
                        lParamOverride);
}

}  // namespace

DirectDraw::~DirectDraw() {
  if (gl_.hglrc != nullptr) {
    wglDeleteContext(gl_.hglrc);
  }
  if (gl_.hdc != nullptr && coop_level_) {
    ::ReleaseDC(coop_level_->hwnd, gl_.hdc);
  }
}

HRESULT DirectDraw::QueryInterface(REFIID riid, LPVOID FAR *ppvObj) noexcept {
  TRACE(
      "DirectDraw::QueryInterface(riid={%d-%d-%d-%d%d%d%d%d%d%d%d}, "
      "ppvObj=0x%p)\n",
      riid.Data1, riid.Data2, riid.Data3, riid.Data4[0], riid.Data4[1],
      riid.Data4[2], riid.Data4[3], riid.Data4[4], riid.Data4[5], riid.Data4[6],
      riid.Data4[7], ppvObj);
  if (riid == IID_IDirectDraw || riid == __uuidof(IUnknown)) {
    *ppvObj = reinterpret_cast<LPVOID *>(this);
    AddRef();
    return S_OK;
  }

  return E_NOINTERFACE;
}

ULONG DirectDraw::AddRef() noexcept {
  TRACE("DirectDraw::AddRef()\n");
  return InterlockedIncrement(&ref_count_);
}

ULONG DirectDraw::Release() noexcept {
  TRACE("DirectDraw::Release()\n");
  // TODO: assert(ref_count_ > 0);
  auto current_count = InterlockedDecrement(&ref_count_);
  if (current_count == 0) {
    delete this;
  }
  return current_count;
}

STUB(DirectDraw::Compact, void);
STUB(DirectDraw::CreateClipper, DWORD, LPDIRECTDRAWCLIPPER FAR *,
     IUnknown FAR *);

HRESULT DirectDraw::CreatePalette(DWORD dwFlags, LPPALETTEENTRY lpColorTable,
                                  LPDIRECTDRAWPALETTE FAR *lplpDDPalette,
                                  IUnknown FAR *pUnkOuter) noexcept {
  TRACE(
      "DirectDraw::CreatePalette(dwFlags=0x%X, lpColorTable=0x%p, "
      "lplpDDPalette=0x%p, "
      "pUnkOuter=0x%p)\n",
      dwFlags, lpColorTable, lplpDDPalette, pUnkOuter);

  LPDIRECTDRAWPALETTE lpDDPalette = new DirectDrawPalette();
  lpDDPalette->Initialize(this, dwFlags, lpColorTable);
  lpDDPalette->AddRef();
  *lplpDDPalette = lpDDPalette;

  return DD_OK;
}

HRESULT DirectDraw::CreateSurface(LPDDSURFACEDESC lpDDSurfaceDesc,
                                  LPDIRECTDRAWSURFACE FAR *lplpDDSurface,
                                  IUnknown FAR *pUnkOuter) noexcept {
  TRACE(
      "DirectDraw::CreateSurface(lpDDSurfaceDesc=0x%p, lplpDDSurface=0x%p, "
      "pUnkOuter=0x%p)\n",
      lpDDSurfaceDesc, lplpDDSurface, pUnkOuter);

  // TODO: inspect lpDDSurfaceDesc
  TRACE("  lpDDSurfaceDesc->dwFlags = 0x%x\n", lpDDSurfaceDesc->dwFlags);
  TRACE("  lpDDSurfaceDesc->ddsCaps.dwCaps = 0x%x\n",
        lpDDSurfaceDesc->ddsCaps.dwCaps);

  // dwFlags == 0 but DDRAW still honors ddsCaps? Goes against the docs...
  // dwCaps == DDSCAPS_PRIMARYSURFACE

  // The primary surface must not have dwWidth and dwHeight set (it uses the
  // display mode dimensions I guess... but there's no call ordering with
  // SetDisplayMode...)

  if (!display_mode_) {
    return DDERR_INCOMPATIBLEPRIMARY;
  }

  // Pass in our width and height when making the surface
  DDSURFACEDESC ddsd = *lpDDSurfaceDesc;
  ddsd.dwWidth = display_mode_->dwWidth;
  ddsd.dwHeight = display_mode_->dwHeight;
  ddsd.dwFlags |= DDSD_WIDTH | DDSD_HEIGHT;

  LPDIRECTDRAWSURFACE lpDDSurface = new DirectDrawSurface();
  lpDDSurface->Initialize(this, &ddsd);
  lpDDSurface->AddRef();
  *lplpDDSurface = lpDDSurface;

  return DD_OK;
}

STUB(DirectDraw::DuplicateSurface, LPDIRECTDRAWSURFACE lpDDSurface,
     LPDIRECTDRAWSURFACE FAR *lplpDupDDSurface);
STUB(DirectDraw::EnumDisplayModes, DWORD, LPDDSURFACEDESC, LPVOID,
     LPDDENUMMODESCALLBACK);
STUB(DirectDraw::EnumSurfaces, DWORD, LPDDSURFACEDESC, LPVOID,
     LPDDENUMSURFACESCALLBACK);
STUB(DirectDraw::FlipToGDISurface, void);
STUB(DirectDraw::GetCaps, LPDDCAPS, LPDDCAPS);
STUB(DirectDraw::GetDisplayMode, LPDDSURFACEDESC);
STUB(DirectDraw::GetFourCCCodes, DWORD FAR *, DWORD FAR *);
STUB(DirectDraw::GetGDISurface, LPDIRECTDRAWSURFACE FAR *);
STUB(DirectDraw::GetMonitorFrequency, LPDWORD);
STUB(DirectDraw::GetScanLine, LPDWORD);
STUB(DirectDraw::GetVerticalBlankStatus, LPBOOL);

HRESULT DirectDraw::Initialize(GUID FAR *lpGUID) noexcept {
  TRACE("DirectDraw::Initialize(lpGUID=0x%p)\n", lpGUID);
  // Wonder if that's a round-about way of saying "this is a private function"
  // i.e. it is used but it's only used internally... Not that I have any
  // additional work to do :shrug: In general, this pattern is common and
  // usually employed to avoid exceptions or in order to call virtual methods
  return DDERR_ALREADYINITIALIZED;
}

STUB(DirectDraw::RestoreDisplayMode, void);

HRESULT DirectDraw::SetCooperativeLevel(HWND hWnd, DWORD dwFlags) noexcept {
  TRACE("DirectDraw::SetCooperativeLevel(hWnd=0x%p, dwFlags=0x%x)\n", hWnd,
        dwFlags);

  coop_level_ = CooperativeLevelParams{
      .hwnd = hWnd,
  };

  return DD_OK;
}

HRESULT DirectDraw::SetDisplayMode(DWORD dwWidth, DWORD dwHeight,
                                   DWORD dwBpp) noexcept {
  TRACE("DirectDraw::SetDisplayMode(dwWidth=%u, dwHeight=%u, dwBpp=%u)\n",
        dwWidth, dwHeight, dwBpp);

  if (!coop_level_) {
    return DDERR_GENERIC;
  }

  display_mode_ = DisplayModeParams{
      .dwWidth = dwWidth,
      .dwHeight = dwHeight,
  };

  const HWND hwnd = coop_level_->hwnd;

  // TODO: Hook SetCursorPos so the cursor doesn't fly across the screen when a
  // panel closes/opens

  // Replace existing WndProc. Store the old one so it can be used later. Use
  // userdata to access `this` inside our WndProc
  SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));
  old_wndproc_ = reinterpret_cast<WNDPROC>(SetWindowLongPtr(
      hwnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&OverrideWndProc)));

  std::optional<OpenGlState> gl = MakeOpenGlContext(hwnd);
  if (!gl) {
    return DDERR_GENERIC;
  }
  gl_ = std::move(gl).value();

  MONITORINFO mi{};
  mi.cbSize = sizeof(MONITORINFO);
  GetMonitorInfo(MonitorFromWindow(hwnd, MONITOR_DEFAULTTONEAREST), &mi);

  const auto monitor_width = mi.rcMonitor.right - mi.rcMonitor.left;
  const auto monitor_height = mi.rcMonitor.bottom - mi.rcMonitor.top;
  // TODO: ALT+ENTER to toggle fullscreen
  ConfigureFullscreenMode(hwnd, mi.rcMonitor);
  ScaleOpenGlViewport(*display_mode_, monitor_width, monitor_height);

  InitOpenGl();

  return DD_OK;
}

STUB(DirectDraw::WaitForVerticalBlank, DWORD dwFlags, HANDLE hEvent);