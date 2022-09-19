#ifndef __SOUND_H__
#define __SOUND_H__

#include "directx/DSOUND.H"
#include <windows.h>

//
// structs
//

struct TSnd
{
    DWORD pcm_size;
    LPWAVEFORMATEX pFormat;
    int dword_8;
    int dword_c;
    int dword_10;
    int dword_14;
    int dword_18;
    BOOL uses_hardware_mixing;
    LPDIRECTSOUNDBUFFER DSB;
    int dword_24;
    const char *sound_path;
};

//
// variables
//

extern LPDIRECTSOUND sglpDS;

//
// functions
//

void snd_init(HWND hWnd);
void sound_file_load(TSnd *pSnd, const char *path);
HRESULT sound_file_cleanup(TSnd *sound_file);
HRESULT sound_CreateSoundBuffer(TSnd *sound_file);

#endif
