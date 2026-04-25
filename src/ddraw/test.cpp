#define NOMINMAX

#include <algorithm>
#include <array>
#include <cassert>
#include <cmath>
#include <cstdint>
#include <limits>

#include "DDRAW.H"

namespace {

constexpr int kWindowWidth = 640;
constexpr int kWindowHeight = 480;

LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT message, WPARAM w_param,
                                 LPARAM l_param) {
  switch (message) {
    case WM_CLOSE:    // ALT+F4
      PostQuitMessage(0);
      // Since DirectDraw has a reference to hwnd, don't call DestroyWindow
      return 0;
    default:
      break;
  }

  return DefWindowProc(hwnd, message, w_param, l_param);
}

ATOM MakeClassAtom(HINSTANCE win32_instance) {
  WNDCLASS window_class = {
      .style = 0,
      .lpfnWndProc = WindowProcedure,
      .cbClsExtra = 0,
      .cbWndExtra = 0,
      .hInstance = win32_instance,
      .hIcon = nullptr,
      .hCursor = nullptr,
      .hbrBackground = nullptr,
      .lpszMenuName = nullptr,
      .lpszClassName = TEXT("ddraw test window class"),
  };
  ATOM window_class_atom = RegisterClass(&window_class);
  assert(window_class_atom != 0);

  return window_class_atom;
}

HWND MakeWindow(HINSTANCE win32_instance, ATOM atom) {
  HWND window = CreateWindow(MAKEINTATOM(atom), TEXT("ddraw test"),
                             WS_OVERLAPPEDWINDOW, /*x=*/CW_USEDEFAULT,
                             /*y=*/CW_USEDEFAULT, /*w=*/CW_USEDEFAULT,
                             /*h=*/CW_USEDEFAULT, /*hWndParent=*/nullptr,
                             /*hMenu=*/nullptr, win32_instance,
                             /*lpParam=*/nullptr);
  assert(window != nullptr);

  ShowWindow(window, SW_SHOW);
  assert(UpdateWindow(window) != 0);

  return window;
}

inline constexpr BYTE Quantize(float f) {
  assert(f >= 0.F && f <= 1.F);
  return static_cast<BYTE>(f * std::numeric_limits<BYTE>::max());
}

inline float ColorFunction(float radians) {
  // Cut off top third and bottom third to approximate hue -> RGB.
  // Then normalize 0..1
  return std::clamp(cosf(radians) / 2.F, -1.F / 3.F, 1.F / 3.F) * (3.F / 2.F) /
             2.F +
         0.5F;
}

constexpr float kPi = 3.1415F;

inline constexpr float ToRadians(float degrees) {
  return degrees / 180.F * kPi;
};

}  // namespace

int main(int argc, char** argv) {
  // Before you can DDRAW, you need an HWND
  HINSTANCE win32_instance = GetModuleHandle(nullptr);
  assert(win32_instance != nullptr);

  ATOM window_class_atom = MakeClassAtom(win32_instance);
  HWND window = MakeWindow(win32_instance, window_class_atom);

  // Initialize DDRAW
  LPDIRECTDRAW direct_draw = nullptr;
  assert(DirectDrawCreate(/*lpGUID=*/nullptr, &direct_draw,
                          /*pUnkOuter=*/nullptr) == DD_OK);

  assert(direct_draw->SetCooperativeLevel(
             window, DDSCL_EXCLUSIVE | DDSCL_FULLSCREEN) == DD_OK);

  assert(direct_draw->SetDisplayMode(kWindowWidth, kWindowHeight,
                                     /*dwBpp=*/8) == DD_OK);

  // Approximate rainbow. Lazily use cos as periodic fn that approximates abs.
  // This isn't perfect and kind of cheats yellow but is good enough for the
  // example.
  std::array<PALETTEENTRY, 256> colors = {};
  for (int i = 0; i < static_cast<int>(colors.size()); ++i) {
    // Do one repetition over domain of colors
    float rads = i / (float)colors.size() * kPi * 2.F;
    colors[i] = {
        .peRed = Quantize(ColorFunction(rads)),
        .peGreen = Quantize(ColorFunction(rads + (2.F * kPi / 3.F))),
        .peBlue = Quantize(ColorFunction(rads - (2.F * kPi / 3.F))),
        .peFlags = 0,
    };
  }
  LPDIRECTDRAWPALETTE palette = nullptr;
  assert(direct_draw->CreatePalette(DDPCAPS_8BIT, colors.data(), &palette,
                                    /*pUnkOuter=*/nullptr) == DD_OK);

  // Make a surface with our palette to draw on
  DDSURFACEDESC surface_description = {
      .dwSize = sizeof(DDSURFACEDESC),
      .dwFlags = DDSD_WIDTH | DDSD_HEIGHT,
      .dwHeight = kWindowHeight,
      .dwWidth = kWindowWidth,
  };
  LPDIRECTDRAWSURFACE surface = nullptr;
  assert(direct_draw->CreateSurface(&surface_description, &surface,
                                    /*pUnkOuter=*/nullptr) == DD_OK);
  assert(surface->SetPalette(palette) == DD_OK);

  bool running = true;
  int ticks = 0;
  while (running) {
    // Normal windows event loop handling.
    MSG message = {};
    while (PeekMessage(&message, /*hWnd=*/nullptr, /*wMsgFilterMin=*/0,
                       /*wMsgFilterMax=*/0, PM_REMOVE) != 0) {
      if (message.message == WM_QUIT) {
        running = false;
        break;
      }
      TranslateMessage(&message);
      DispatchMessage(&message);
    }

    // Draw a diagonal rainbow gradient onto the surface.
    DDSURFACEDESC locked_surface_description = {};
    assert(surface->Lock(/*lpRect=*/nullptr, &locked_surface_description,
                         /*dwFlags=*/0, /*event=*/nullptr) == DD_OK);

    // NOTE: We don't set any other fields on DDSURFACEDESC outside lPitch and
    // lpSurface. Diablo gets width and height from other vars.
    char* data = reinterpret_cast<char*>(locked_surface_description.lpSurface);

    for (DWORD y = 0; y < kWindowHeight; ++y) {
      for (DWORD x = 0; x < kWindowWidth; ++x) {
        data[y * locked_surface_description.lPitch + x] =
            Quantize(sinf(ToRadians(ticks+x+y)) / 2.F + 0.5F);
      }
    }
    assert(surface->Unlock(locked_surface_description.lpSurface) == DD_OK);

    ++ticks;

    // Limit frame rate to 20FPS (don't hog CPU)
    Sleep(/*dwMilliseconds=*/50);
  }

  surface->Release();
  palette->Release();
  direct_draw->Release();
  UnregisterClass(MAKEINTATOM(window_class_atom), win32_instance);
  DestroyWindow(window);
  return 0;
}