#define NOMINMAX

#include <cassert>
#include <cstdint>
#include <limits>

#include "DDRAW.H"

namespace {

constexpr int kWindowWidth = 640;
constexpr int kWindowHeight = 480;

LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT message, WPARAM w_param,
                                 LPARAM l_param) {
  switch (message) {
    case WM_DESTROY:
      PostQuitMessage(0);
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

// rectangle is inclusive
void DrawRectangle(DDSURFACEDESC& surface, const RECT& rectangle,
                   BYTE color) {
  char* data = reinterpret_cast<char*>(surface.lpSurface);

  for (int x = rectangle.left; x <= rectangle.right; ++x) {
    data[rectangle.top * surface.lPitch + x] = color;
    data[rectangle.bottom * surface.lPitch + x] = color;
  }

  for (int y = rectangle.top; y <= rectangle.bottom; ++y) {;
    data[y * surface.lPitch + rectangle.left] = color;
    data[y * surface.lPitch + rectangle.right] = color;
  }
}

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

  // Initialize grayscale 256 color palette
  PALETTEENTRY colors[256] = {};
  for (int i = 0; i < sizeof(colors) / sizeof(colors[0]); ++i) {
    BYTE b = static_cast<BYTE>(i);
    colors[i] = {.peRed = b, .peGreen = b, .peBlue = b, .peFlags = 0};
  }
  LPDIRECTDRAWPALETTE palette = nullptr;
  assert(direct_draw->CreatePalette(DDPCAPS_8BIT, colors, &palette,
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
  BYTE color = 0;
  while (running) {
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

    // Draw noise into the surface buffer
    DDSURFACEDESC locked_surface_description = {};
    assert(surface->Lock(/*lpRect=*/nullptr, &locked_surface_description,
                         /*dwFlags=*/0, /*event=*/nullptr) == DD_OK);

    // You can do this with palette shifting but I want to test Lock/Unlock.
    // Since ends are inclusive, need to subtract 1
    int max_width = kWindowWidth - 1;
    int max_height = kWindowHeight - 1;
    for (int i = 0; i < max_width - i && i < max_height - i; ++i) {
      DrawRectangle(locked_surface_description,
                      RECT{i, i, max_width - i, max_height - i},
                      color + i);
    }
    assert(surface->Unlock(locked_surface_description.lpSurface) == DD_OK);

    --color;

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