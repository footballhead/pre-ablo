#include "ddraw_impl.h"

#include <GL/glew.h>

#include <algorithm>
#include <cassert>

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

// Performs aspect-ratio corrected scaling to fill the window with the game
// contents and ensures that the contents are centered.
//
// The window might be a different size from the requested display mode. E.g.
// your window might be 1920x1080 but Diablo renders to a 640x480 image.
//
// If the window aspect ratio is different from the game aspect ratio then
// pillar boxing or letter boxing are used to compensate.
//
// TODO: Add configuration to turn off aspect-ratio in scaling.
// TODO: Add configuration for nearest neigbor scaling instead of linear
void ScaleOpenGlViewport(const DirectDraw::DisplayModeParams& display_mode,
                         int window_width, int window_height) {
  auto adjusted_width = window_width;
  auto adjusted_height = window_height;

  // TODO: Find a home for pixel perfect scaling
  // auto pixel_scale =
  //     min(width / display_mode->dwWidth, height / display_mode->dwHeight);
  // adjusted_width = display_mode->dwWidth * pixel_scale;
  // adjusted_height = display_mode->dwHeight * pixel_scale;

  // Scale using aspect ratio
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
  int centered_x = (window_width - adjusted_width) / 2;
  int centered_y = (window_height - adjusted_height) / 2;

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

// Requires OpenGL context
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

// Wrap app window procedure in order to intercept events. This allows us to
// do stuff like trapping the cursor and fixing mouse coordinates.
//
// This is set up in SetCooperativeLevel and undone in Release
LRESULT CALLBACK OverrideWndProc(HWND hWnd, UINT Msg, WPARAM wParam,
                                 LPARAM lParam) {
  DirectDraw* lpDD =
      reinterpret_cast<DirectDraw*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
  assert(lpDD != nullptr);

  // TODO: SetCursorPos is relative to the screen.

  LPARAM lParamOverride = lParam;

  // TODO:
  // https://learn.microsoft.com/en-us/windows/win32/inputdev/using-mouse-input

  switch (Msg) {
    case WM_MOUSEMOVE: {
      POINTS ptMouse = MAKEPOINTS(lParam);

      const auto display_mode = lpDD->GetDisplayMode();
      if (!display_mode) {
        break;
      }

      // Clamp mouse position to inside the game area.
      //
      // Since the window may be bigger than the game (e.g. using a 1080p
      // monitor to run a 480p game), the cursor may be outside the area that
      // the game recognizes. This can cause out of bounds memory access.
      //
      // This works for Diablo since it renders software cursor where it thinks
      // that the mouse is located, which is then scaled to the window. But it
      // looks weird when the output is scaled.
      //
      // (Another way to solve this is to alter the game code to correct MouseX
      // and MouseY in the same way)
      POINTS top_left = {};
      POINTS bottom_right = {};
      lpDD->GetDisplayModeExtents(top_left, bottom_right);

      POINTS clamped_mouse = {
          .x = std::clamp(ptMouse.x, top_left.x, bottom_right.x),
          .y = std::clamp(ptMouse.y, top_left.y, bottom_right.y),
      };

      lParamOverride = MAKELPARAM(clamped_mouse.x, clamped_mouse.y);
      TRACE("WM_MOUSEMOVE: ptMouse={%d, %d} -> {%d, %d} \n", ptMouse.x,
            ptMouse.y, clamped_mouse.x, clamped_mouse.y);
      break;
    }
    case WM_LBUTTONDOWN: {
      TRACE("WM_LBUTTONDOWN: Capture cursor\n");

      const auto display_mode = lpDD->GetDisplayMode();
      if (!display_mode) {
        break;
      }

      // TODO: Do I need to SetCapture?

      POINT top_left = {};
      POINT bottom_right = {};
      lpDD->GetDisplayModeExtents(top_left, bottom_right);

      // Translate from client to screen so that this works in windowed mode.
      // TODO verify this
      if (ClientToScreen(hWnd, &top_left) &&
          ClientToScreen(hWnd, &bottom_right)) {
        RECT rcScreen = {
            .left = top_left.x,
            .top = top_left.y,
            .right = bottom_right.x,
            .bottom = bottom_right.y,
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
  if (coop_level_) {
    // Our window procedure wrapper relies on `this` being valid.
    SetWindowLongPtr(coop_level_->hwnd, GWLP_USERDATA, NULL);
    SetWindowLongPtr(coop_level_->hwnd, GWLP_WNDPROC,
                     reinterpret_cast<LONG_PTR>(old_wndproc_));
  }
  if (gl_.hglrc != nullptr) {
    wglDeleteContext(gl_.hglrc);
  }
  if (gl_.hdc != nullptr && coop_level_) {
    ::ReleaseDC(coop_level_->hwnd, gl_.hdc);
  }
}

HRESULT DirectDraw::QueryInterface(REFIID riid, LPVOID FAR* ppvObj) noexcept {
  TRACE(
      "DirectDraw::QueryInterface(riid={%d-%d-%d-%d%d%d%d%d%d%d%d}, "
      "ppvObj=0x%p)\n",
      riid.Data1, riid.Data2, riid.Data3, riid.Data4[0], riid.Data4[1],
      riid.Data4[2], riid.Data4[3], riid.Data4[4], riid.Data4[5], riid.Data4[6],
      riid.Data4[7], ppvObj);
  if (riid == IID_IDirectDraw || riid == __uuidof(IUnknown)) {
    *ppvObj = reinterpret_cast<LPVOID*>(this);
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
  assert(ref_count_ > 0);
  auto current_count = InterlockedDecrement(&ref_count_);
  if (current_count == 0) {
    delete this;
  }
  return current_count;
}

STUB(DirectDraw::Compact, void);
STUB(DirectDraw::CreateClipper, DWORD, LPDIRECTDRAWCLIPPER FAR*, IUnknown FAR*);

HRESULT DirectDraw::CreatePalette(DWORD dwFlags, LPPALETTEENTRY lpColorTable,
                                  LPDIRECTDRAWPALETTE FAR* lplpDDPalette,
                                  IUnknown FAR* pUnkOuter) noexcept {
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
                                  LPDIRECTDRAWSURFACE FAR* lplpDDSurface,
                                  IUnknown FAR* pUnkOuter) noexcept {
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
     LPDIRECTDRAWSURFACE FAR* lplpDupDDSurface);
STUB(DirectDraw::EnumDisplayModes, DWORD, LPDDSURFACEDESC, LPVOID,
     LPDDENUMMODESCALLBACK);
STUB(DirectDraw::EnumSurfaces, DWORD, LPDDSURFACEDESC, LPVOID,
     LPDDENUMSURFACESCALLBACK);
STUB(DirectDraw::FlipToGDISurface, void);
STUB(DirectDraw::GetCaps, LPDDCAPS, LPDDCAPS);
STUB(DirectDraw::GetDisplayMode, LPDDSURFACEDESC);
STUB(DirectDraw::GetFourCCCodes, DWORD FAR*, DWORD FAR*);
STUB(DirectDraw::GetGDISurface, LPDIRECTDRAWSURFACE FAR*);
STUB(DirectDraw::GetMonitorFrequency, LPDWORD);
STUB(DirectDraw::GetScanLine, LPDWORD);
STUB(DirectDraw::GetVerticalBlankStatus, LPBOOL);

HRESULT DirectDraw::Initialize(GUID FAR* lpGUID) noexcept {
  TRACE("DirectDraw::Initialize(lpGUID=0x%p)\n", lpGUID);
  // Wonder if that's a round-about way of saying "this is a private function"
  // i.e. it is used but it's only used internally... Not that I have any
  // additional work to do :shrug: In general, this pattern is common and
  // usually employed to avoid exceptions or in order to call virtual methods
  return DDERR_ALREADYINITIALIZED;
}

STUB(DirectDraw::RestoreDisplayMode, void);

// Wraps window procedue and creates OpenGL context.
HRESULT DirectDraw::SetCooperativeLevel(HWND hWnd, DWORD dwFlags) noexcept {
  TRACE("DirectDraw::SetCooperativeLevel(hWnd=0x%p, dwFlags=0x%x)\n", hWnd,
        dwFlags);

  // Store window handle for use in SetDisplayMode.
  coop_level_ = CooperativeLevelParams{
      .hwnd = hWnd,
  };

  // Wrap window procedure so we can react to and fix events. Store "this"
  // in user data since the window procedure is a static C function.
  SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));
  old_wndproc_ = reinterpret_cast<WNDPROC>(SetWindowLongPtr(
      hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&OverrideWndProc)));

  std::optional<OpenGlState> gl = MakeOpenGlContext(hWnd);
  if (!gl) {
    return DDERR_GENERIC;
  }
  gl_ = std::move(gl).value();

  InitOpenGl();

  // TODO: Hook SetCursorPos so the cursor doesn't fly across the screen
  // when a panel closes/opens

  return DD_OK;
}

// Enters windowed fullscreen and sets OpenGL context scaling appropriately.
HRESULT DirectDraw::SetDisplayMode(DWORD dwWidth, DWORD dwHeight,
                                   DWORD dwBpp) noexcept {
  TRACE("DirectDraw::SetDisplayMode(dwWidth=%u, dwHeight=%u, dwBpp=%u)\n",
        dwWidth, dwHeight, dwBpp);

  assert(dwBpp == 8);

  if (!coop_level_) {
    return DDERR_GENERIC;
  }

  display_mode_ = DisplayModeParams{
      .dwWidth = dwWidth,
      .dwHeight = dwHeight,
  };

  const HWND hwnd = coop_level_->hwnd;

  MONITORINFO mi{};
  mi.cbSize = sizeof(MONITORINFO);
  GetMonitorInfo(MonitorFromWindow(hwnd, MONITOR_DEFAULTTONEAREST), &mi);

  const auto monitor_width = mi.rcMonitor.right - mi.rcMonitor.left;
  const auto monitor_height = mi.rcMonitor.bottom - mi.rcMonitor.top;
  // TODO: ALT+ENTER to toggle fullscreen
  ConfigureFullscreenMode(hwnd, mi.rcMonitor);
  ScaleOpenGlViewport(*display_mode_, monitor_width, monitor_height);

  // Clear for good measure. (Not entirely sure if necessary)
  glClearColor(0.F, 0.F, 0.F, 1.F);
  glClear(GL_COLOR_BUFFER_BIT);
  glFlush();

  return DD_OK;
}

STUB(DirectDraw::WaitForVerticalBlank, DWORD dwFlags, HANDLE hEvent);