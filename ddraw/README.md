# ddraw tailored for Pre-ablo

DIABLO.EXE uses a subset of DDRAW that we can easily reimplement with OpenGL. This is done to sidestep some of the uglier parts of the original ddraw on modern operating systems like lack proper windowed modes, some palette challenges, etc

## Code walkthrough

Anticipated usage (based on tracing DIABLO.EXE):

```c++
LPDIRECTDRAW lpDD;
DirectDrawCreate(nullptr, &lpDD, nullptr);
lpDD->SetCooperativeLevel();
lpDD->SetDisplayMode();

LPDIRECTDRAWSURFACE lpDDSurface;
lpDD->CreateSurface(&lpDDSurface);

LPDIRECTDRAWPALETTE lpDDPalette;
lpDD->CreatePalette(&lpDDPalette);
lpDDSurface->SetPalette(lpDDPalette);
lpDDPalette->SetEntries();

while (true) {
    // Do 256-color drawing into gpBuffer

    lpDDSurface->Lock();
    // Copy gpBuffer into locked buffer
    lpDDSurface->Unlock();
}

lpDDSurface->Release();
lpDDPalette->Release();
lpDD->Release();
```

The hot areas are SetDisplayMode(), CreateSurface()/DirectDrawSurface::Initialize(), Lock() and Unlock()

### DirectDraw

- SetCooperativeLevel() captures hWnd which is crucial for attaching anything to the window that DIABLO.EXE makes
- SetDisplayMode() does a lot. It:
    - captures the desired display mode resolution
    - puts into windowed fullscreen and scales the viewport to the screen (using hwnd from SetCooperativeLevel)
    - and initializes OpenGL 1.1. NDC is used to easily scale the texture on viewport
- CreateSurface() makes a DirectDrawSurface and calls Initialize() on it. This creates an OpenGL texture

### DirectDrawPalette

This is a PALETTEENTRY array. Nothing fancy. It's used by Unlock(). With OpenGL 3 this would be a candidate for uploading to the GPU as a sampler

### DirectDrawSurface

- Initialize() (called by CreateSurface) creates an OpenGL texture, then allocates CPU space for both the 256-color buffer and RGB buffer. The texture is RGBA but alpha blending is not turned on.
- Lock() only sets lPitch and lSurface because those are the only things that the callers care about
- Unlock() is called on a regular cadence (20FPS) by DIABLO.EXE when gpBuffer changes are done and there's something new to draw. This is where I translate the 256-color buffer into the RGB buffer using the palette; upload the texture information to OpenGL; and draw the texture on a screen-sized quad. DDRAW stores textures bottom-up so I flip the Y coordinate in opengl.
- In OpenGL 1.1 we use the CPU palette to translate the CPU 256-color buffer into a CPU RGB buffer which is uploaded to a texture. The hope is to use OpenGL 3 to do more on the GPU; the palette will live on th GPU as a sampler, the 256-color buffer would be uploaded to the GPU instead, then a shader would do 2x texture lookups.

### Misc notes

- I ignore the comments and implement/use Initialize. I could probably do that work elsewhere but this avoids exceptions, etc.
- I used RGBA (as opposed to RGB) as the format since PALETTEENTRY is effectively RGBA so applying the palette and uploading to OpenGL are easy
- I don't enforce the calling order contract very heavily because I assume DIABLO.EXE is predictable and the only consumer
- I use NDC so I that I have one less thing that is resolution dependent

## DIABLO.EXE Notes

- The game writes into a CPU buffer called gpBuffer. It's 256 color and slightly larger than the screen (to account for overdraw). Only a screen-sized subset -- desgined to be rendered -- is copied into the Lock()'d buffer
- Fades are accomplished by changing palette over time (expect SetEntries to be called a lot)
- Lock()/Unlock() is called on 20FPS cadence. Given it's pretty predictable, I do most of the actual logic in Unlock
- Only the PRIMARYSURFACE is created, no BACKBUFFER. There's code for double-buffering but it's incomplete
- STORM.DLL handles running cutscenes
- Things I'm surprised work:
    - Storm.DLL calls Unlock with a valid pointer (like the docs say to do) but DIABLO.EXE doesn't
    - CreateSurface() is called with PRIMARYSURFACE but dwFlags isn't set to indicate that ddsCaps is set...
