# Window Mode

I've only gotten this to work properly in a rare situtaion:

 * Windows XP SP3
 * Running from IDA 5.0
 * NVidia 8840SE or something real old
 * Desktop set to 256 color through GFX driver settings

The following doesn't work:

* Running the EXE. Results in "DirectDraw init failed", the EXE will still run in thee background and hog CPU. You must use Task Manager to stop it.
    * the problem here is that IDirectDrawSurface::SetPalette returns `DDERR_INVALIDPIXELFORMAT` (0x88760091)
* Any of the compatibility settings. The app will launch then quit
* The ddrawwrapper. It quits from an exception in DirectDrawSurfaceWrapper::WrapperInitialize
    * The expectation is that DirectDraw::SetDisplayMode is called before DirectDraw::CreateSurface
	* This is not the case in Window mode so the dimensions are uninitialized garbage

---

`dx_init` (.text:00484E67) looks at a BOOL at `.data:0061BF48`. If that BOOL is `0` it will not go fullscreen.

This variable is set in WinMain (.text:00484A20):

```
.text:00484A20 55                                            push    ebp
.text:00484A21 8B EC                                         mov     ebp, esp
.text:00484A23 83 EC 50                                      sub     esp, 50h
.text:00484A26 53                                            push    ebx
.text:00484A27 56                                            push    esi
.text:00484A28 57                                            push    edi
.text:00484A29 C7 05 60 54 60 00 01 00 00 00                 mov     gbActivePlayers, 1
.text:00484A33 C7 05 48 BF 61 00 01 00 00 00                 mov     isFullscreen, 1
.text:00484A3D 83 3D 60 54 60 00 01                          cmp     gbActivePlayers, 1
```

We can set it to `0` instead:

```
.text:00484A33 C7 05 48 BF 61 00 00 00 00 00                 mov     isFullscreen, 0 ; BINOFF 0x83E33
```

This changes how `dx_init` creates the window and initializes DirectDraw.
