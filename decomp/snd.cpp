#include <windows.h>
#include <process.h>

#include "sound.h"
#include "winapp.h"

// TODO add to header
unsigned __stdcall SNDCPP_ThreadProc(void *pArguments);
void SNDCPP_LoadFile();

//
// uninitialized variables (.data:005F6ED8)
//

HANDLE ghSndThread;
BOOL gbSndInited;
BOOL gbSndMusicPlaying;
CRITICAL_SECTION gSndCriticalSection;
TSnd hSndcppSnd;
// ...
DWORD gdwSndThreadSleepMs;

//
// code (.text:00430A90)
//

// .text:00430A90
// I think this creates the music thread?
BOOL SNDCPP_InitThread()
{
    unsigned ThreadId; // unused

    InitializeCriticalSection(&gSndCriticalSection);
    gbSndInited = TRUE;

    ghSndThread = (HANDLE)_beginthreadex(NULL, 0x400, SNDCPP_ThreadProc, NULL, CREATE_SUSPENDED, &ThreadId);
    if (ghSndThread == INVALID_HANDLE_VALUE)
    {
        goto error;
        goto cleanup;
    }

    SetThreadPriority(ghSndThread, THREAD_PRIORITY_ABOVE_NORMAL);
    gbSndMusicPlaying = FALSE;
    return TRUE;

cleanup:
    DeleteCriticalSection(&gSndCriticalSection);
    return FALSE;

error:
    goto cleanup;
}

// .text:00430B24
// I think this loads a chunk of music to play?
unsigned __stdcall SNDCPP_ThreadProc(void *pArguments)
{
    EnterCriticalSection(&gSndCriticalSection);
    while (gbSndInited)
    {
        SNDCPP_LoadFile();
        LeaveCriticalSection(&gSndCriticalSection);

        Sleep(gdwSndThreadSleepMs);

        EnterCriticalSection(&gSndCriticalSection);
    }
    LeaveCriticalSection(&gSndCriticalSection);

    return 0;
}

// .text:00430B87
// I think this loads a single chunk of music to paly
void SNDCPP_LoadFile()
{
    // TODO
}

// .text:00430D76
void SNDCPP_FreeThread()
{
    if (sglpDS == NULL)
    {
        winapp_print_fatal(TEXT("C:\\Diablo\\Direct\\Snd.cpp"), 212);
    }

    // SNDCPP_431207(); TOOD
    gbSndInited = FALSE;
    ResumeThread(ghSndThread);
    if (WaitForSingleObject(ghSndThread, INFINITE) == WAIT_FAILED)
    {
        TermMsg(TEXT("snd:1 (%d)"), GetLastError());
    }

    CloseHandle(ghSndThread);
    DeleteCriticalSection(&gSndCriticalSection);

    sglpDS->Release();
}

// snd_play_snd	0000000000430E1E
// SNDCPP_430EB2	0000000000430EB2

// sound_CreateSoundBuffer_NoStorm	0000000000430F40
// 

// SNDCPP_file_reload	0000000000431066
// sound_cleanup_0	0000000000431152
// music_start	00000000004311C8

// .text:00431207
void SNDCPP_StopThread() {
    if (gbSndMusicPlaying) {
        // SNDCPP_430EB2(); TODO
        // sound_cleanup_0(); TODO
        gbSndMusicPlaying = FALSE;
    }
}
