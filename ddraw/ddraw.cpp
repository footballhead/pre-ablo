#define INITGUID
#include "DDRAW.H"

#include <gl/gl.h>

#include <optional>

#include "ddraw_impl.h"
#include "log.h"
#include "palette.h"
#include "surface.h"

extern "C" {

HRESULT WINAPI DirectDrawCreate(GUID FAR *lpGUID, LPDIRECTDRAW FAR *lplpDD,
                                IUnknown FAR *pUnkOuter) {
  TRACE("DirectDrawCreate(lpGUID=0x%p, lplpDD=0x%p, pUnkOuter=0x%p)\n", lpGUID,
        lplpDD, pUnkOuter);

  LPDIRECTDRAW lpDD = new DirectDraw();
  lpDD->Initialize(lpGUID);
  lpDD->AddRef();
  *lplpDD = lpDD;

  return DD_OK;
}

BOOL APIENTRY DllMain(HMODULE /*hModule*/, DWORD ul_reason_for_call,
                      LPVOID /*lpReserved*/) {
  switch (ul_reason_for_call) {
    case DLL_PROCESS_ATTACH:
      // Theoretically, we shouldn't do initialization here. Instead, it should
      // be done at the top of DIABLO.EXE main(). This is a well known
      // limitation of DllMain spelled out in the docs:
      // https://docs.microsoft.com/en-us/windows/win32/dlls/dynamic-link-library-best-practices
      // I think we get away with it because ddraw.dll is loaded after the DLLs
      // on which we depend.

#ifndef NDEBUG
      // See TRACE in a new terminal window
      // TODO: redirect to file #ifdef NDEBUG?
      AllocConsole();
      freopen("CONOUT$", "wb", stdout);
#endif

      TRACE("SetCursorPos=0x%p\n", &SetCursorPos);
      break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
      break;
  }
  return TRUE;
}
}