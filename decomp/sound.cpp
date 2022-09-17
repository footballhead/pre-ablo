#include "sound.h"

#include <windows.h>
#include "directx/DSOUND.H"
#include <mmreg.h>
#include <msacm.h>

#include "engine.h"
#include "error.h"

//
// initialized variables (.data:004A3444)
//

LPDIRECTSOUND sglpDS = NULL;

//
// uninitialized variables (.data:005DDD70)
//

// TSnd is reused (without many fields set) to track primary buffer state.
TSnd dsnd_primary_buffer;

// .text:0040E5D0
void snd_init(HWND hWnd)
{
    HRESULT hResult;
    DWORD max_size_format;
    MMRESULT mmResult;
    DSBUFFERDESC dsbuf;

    dsnd_primary_buffer.pFormat = NULL;

    mmResult = acmMetrics(NULL, ACM_METRIC_MAX_SIZE_FORMAT, &max_size_format);
    if (mmResult != 0)
    {
        hResult = mmResult;
        goto acm_err;
        goto show_err_msg; // not sure if this is part of code or emitted by compiler...
    }

    dsnd_primary_buffer.pFormat = (LPWAVEFORMATEX)DiabloAllocPtr(max_size_format);
    if (dsnd_primary_buffer.pFormat == NULL)
    {
        goto alloc_fail;
        goto show_err_msg; // not sure if this is part of code or emitted by compiler...
    }

    dsnd_primary_buffer.pFormat->wFormatTag = WAVE_FORMAT_PCM;
    dsnd_primary_buffer.pFormat->nChannels = 2;
    dsnd_primary_buffer.pFormat->nSamplesPerSec = 22050;
    dsnd_primary_buffer.pFormat->nAvgBytesPerSec = 22050 * 4;
    dsnd_primary_buffer.pFormat->nBlockAlign = 4;
    dsnd_primary_buffer.pFormat->wBitsPerSample = 16;
    dsnd_primary_buffer.pFormat->cbSize = 0;

    hResult = DirectSoundCreate(NULL, &sglpDS, NULL);
    if (hResult != DS_OK)
    {
        goto dsound_err;
        goto show_err_msg; // not sure if this is part of code or emitted by compiler...
    }

    hResult = sglpDS->SetCooperativeLevel(hWnd, DSSCL_PRIORITY);
    if (hResult != DS_OK)
    {
        goto set_coop_fail;
        goto show_err_msg; // not sure if this is part of code or emitted by compiler...
    }

    memset(&dsbuf, 0, sizeof(DSBUFFERDESC));
    dsbuf.dwSize = sizeof(DSBUFFERDESC);
    // "The primary sound buffer represents the actual audio samples output to
    // the sound device. These samples can be a single audio stream or the
    // result of mixing several audio streams. The audio data in a primary sound
    // buffer is typically not accessed directly by applications; however, the
    // primary buffer can be used for control purposes, such as setting the
    // output volume or wave format."
    dsbuf.dwFlags = DSBCAPS_PRIMARYBUFFER;

    sglpDS->CreateSoundBuffer(&dsbuf, &dsnd_primary_buffer.DSB, NULL);
    if (hResult != DS_OK)
    {
        goto sound_buffer_err;
        goto show_err_msg; // not sure if this is part of code or emitted by compiler...
    }

    dsnd_primary_buffer.DSB->SetFormat(dsnd_primary_buffer.pFormat);
    // "Primary buffers must be played with the DSBPLAY_LOOPING flag set.
    //
    // "This method will cause primary sound buffers to start playing to the
    // sound device"
    dsnd_primary_buffer.DSB->Play(0, 0, DSBPLAY_LOOPING);

    goto done__;
    goto done_; // not sure if this is part of code or emitted by compiler...

show_err_msg:
    InitDiabloMsg(EMSG_DIRECT_SOUND_FAILED);
done_:
    goto done;
done__:
    goto done;
sound_buffer_err:
    goto show_err_msg;
set_coop_fail:
    goto show_err_msg;
dsound_err:
    goto show_err_msg;
alloc_fail:
    goto show_err_msg;
acm_err:
    goto show_err_msg;
done:
    return;
}

// .text:0040E79B
void sound_cleanup()
{
    DWORD unused;

    unused = 0;

    sound_file_cleanup(&dsnd_primary_buffer);
    if (sglpDS)
    {
        sglpDS->Release();
        sglpDS = NULL;
    }
    if (dsnd_primary_buffer.pFormat)
    {
        MemFreeDbg(dsnd_primary_buffer.pFormat);
        dsnd_primary_buffer.pFormat = NULL;
    }
}

// .text:0040E823
void sound_file_load(TSnd *pSnd, const char *path)
{
    LPVOID lpvAudioPtr2;
    HRESULT hResult;
    DWORD dwAudioBytes2;
    DWORD dwAudioBytes1;
    LPVOID lpvAudioPtr1;

    lpvAudioPtr1 = NULL;
    lpvAudioPtr2 = NULL;

    pSnd->pFormat = NULL;
    pSnd->DSB = NULL;
    pSnd->sound_path = path;
    if (LoadWaveFileHeaderInMem(FileAddLoadPrefix(path), pSnd, &pSnd->pFormat))
    {
        goto format_read_err;
        goto cleanup;
    }

    if (sound_CreateSoundBuffer(pSnd))
    {
        goto create_buffer_err;
        goto cleanup;
    }

    hResult = pSnd->DSB->Lock(/*dwWriteCursor=*/0,
                              pSnd->pcm_size,
                              &lpvAudioPtr1, &dwAudioBytes1,
                              &lpvAudioPtr2, &dwAudioBytes2,
                              /*dwFlags=*/0);
    if (hResult != DS_OK)
    {
        goto lock_err;
        goto cleanup;
    }

    if (LoadWaveFileWithMem(FileAddLoadPrefix(path), pSnd, &pSnd->pFormat, &lpvAudioPtr1))
    {
        goto load_err;
        goto cleanup;
    }

    // This goes against the docs:
    //
    // "An application must pass both pointers, lpvAudioPtr1 and lpvAudioPtr2,
    // returned by the IDirectSoundBuffer::Lock function to ensure the correct
    // pairing of IDirectSoundBuffer::Lock and IDirectSoundBuffer::Unlock. The
    // second pointer is needed even if zero bytes were written to the second
    // pointer (that is, if dwAudioBytes2 equals 0).""
    hResult = pSnd->DSB->Unlock(lpvAudioPtr1, pSnd->pcm_size, NULL, 0);
    if (hResult != DS_OK)
    {
        goto unlock_err;
        goto cleanup;
    }

    lpvAudioPtr1 = NULL;
    goto done;

cleanup:
    if (lpvAudioPtr1)
    {
        hResult = pSnd->DSB->Unlock(lpvAudioPtr1, pSnd->pcm_size, NULL, 0);
        lpvAudioPtr1 = NULL;
    }

    if (pSnd)
    {
        sound_file_cleanup(pSnd);
        if (pSnd->pFormat)
        {
            MemFreeDbg(pSnd->pFormat);
        }
        // This is interesting because a lot of TSnds are on the stack...
        MemFreeDbg(pSnd);
    }
    return;

done:
    return;
unlock_err:
    goto cleanup;
load_err:
    goto cleanup;
lock_err:
    goto cleanup;
create_buffer_err:
    goto cleanup;
format_read_err:
    goto cleanup;
}

// __dc_ReadWaveFile2	000000000040EA28

// .text:0040EBC1
HRESULT sound_file_cleanup(TSnd *sound_file)
{
    HRESULT hResult;

    hResult = 0;
    if (sound_file)
    {
        if (sound_file->DSB)
        {
            sound_file->DSB->Stop();
            hResult = sound_file->DSB->Release();
            sound_file->DSB = NULL;

            if (sound_file->pFormat)
            {
                MemFreeDbg(sound_file->pFormat);
                sound_file->pFormat = NULL;
            }
        }
    }

    return hResult;
}

// .text:0040EC75
HRESULT sound_CreateSoundBuffer(TSnd *sound_file)
{
    HRESULT hResult;
    DSBCAPS caps;
    DSBUFFERDESC DSB;

    memset(&DSB, 0, sizeof(DSBUFFERDESC));
    DSB.dwSize = sizeof(DSBUFFERDESC);
    DSB.dwFlags = 0;
    DSB.dwFlags |= DSBCAPS_STATIC;
    DSB.dwFlags |= DSBCAPS_CTRLDEFAULT;
    DSB.dwBufferBytes = sound_file->pcm_size;
    DSB.lpwfxFormat = sound_file->pFormat;
    if (sglpDS)
    {
        hResult = sglpDS->CreateSoundBuffer(&DSB, &sound_file->DSB, NULL);
        if (hResult != DS_OK)
        {
            goto sound_buffer_err;
            goto cleanup;
        }

        hResult = sound_file->DSB->SetVolume(0);
        if (hResult != DS_OK)
        {
            goto set_vol_err;
            goto cleanup;
        }

        hResult = sound_file->DSB->SetPan(0);
        if (hResult != DS_OK)
        {
            goto set_vol_err;
            goto cleanup;
        }

        caps.dwSize = sizeof(DSBCAPS);

        hResult = sound_file->DSB->GetCaps(&caps);
        if (hResult != DS_OK)
        {
            goto get_caps_err;
            goto cleanup;
        }

        if (caps.dwFlags & DSBCAPS_LOCHARDWARE)
        {
            sound_file->uses_hardware_mixing = TRUE;
        }
        else
        {
            sound_file->uses_hardware_mixing = FALSE;
        }

        goto done_create;
        goto done;
    }

cleanup:
    if (sound_file->DSB)
    {
        sound_file->DSB->Release();
        sound_file->DSB = NULL;
    }
done:
    return hResult;

done_create:
    goto done;
get_caps_err:
    goto cleanup;
set_pan_err:
    goto cleanup;
set_vol_err:
    goto cleanup;
sound_buffer_err:
    goto cleanup;
}

// __dc_sound_40EE02	000000000040EE02
// __dc_sound_cdaudio	000000000040EE7E
