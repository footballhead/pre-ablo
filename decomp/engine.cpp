#include "engine.h"

#include "storm/storm.h"
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#include "defines.h"
#include "diablo.h"
#include "scrollrt.h"

//
// uninitialized vars (.data:???)
//

char file_to_load_with_base[MAX_PATH];
DWORD dword_61B6F4;
DWORD dword_61B6F8;
char byte_61B6FC;

//
// code (.text:00482790)
//

// .text:00482790
BYTE *LoadFileInMem(const char *pszName)
{
    int fileLen;
    BYTE *buf;
    HANDLE file;
    char fullName[256];

    sprintf(fullName, "%s%s", fileLoadPrefix, pszName);
    while (!SFileOpenFile(fullName, &file))
    {
        // This space intentionally left blank
    }

    fileLen = SFileGetFileSize(file, NULL);
    buf = (BYTE *)DiabloAllocPtr(fileLen);

    while (!SFileReadFile(file, buf, fileLen, NULL, NULL))
    {
        // This space intentionally left blank
    }

    SFileCloseFile(file);
    return buf;
}

// LoadFileWithMem    000000000048283B

// .text:004828D6
// Allocate and read the wave format. Also read the data size into pSnd->pcm_size
int LoadWaveFileHeaderInMem(const char *filename, TSnd *pSnd, LPWAVEFORMATEX *pFormat)
{
    DWORD dwSize; // unused
    DWORD unused;
    HANDLE phFile;

    SFileOpenFile(filename, &phFile);
    dwSize = SFileGetFileSize(phFile, NULL);

    // Surgically load parts of the RIFF https://docs.fileformat.com/audio/wav/
    // 0x14 to 0x24 is WAVEFORMATEX... kinda. cbSize is not stored in the file,
    // it goes straight to "data" chunk header. This means that cbSize is read
    // as "da" or 0x6164
    SFileSetFilePointer(phFile, 0x14, NULL, 0);
    *pFormat = (LPWAVEFORMATEX)GlobalAlloc(GMEM_FIXED, sizeof(WAVEFORMATEX));
    SFileReadFile(phFile, *pFormat, sizeof(WAVEFORMATEX), NULL, NULL);

    // The programmers seemed to know that they were reading part of the data
    // chunk header above and just skip over the rest ("ta")
    SFileReadFile(phFile, &unused, 2, NULL, NULL);

    // Read WAV file size (data) into the first element of pSnd (pSnd->pcm_size)
    SFileReadFile(phFile, pSnd, 4, NULL, NULL);

    SFileCloseFile(phFile);
    return 0;
}

// .text:0048297B
// See LoadWaveFileHeaderInMem
int LoadWaveFileWithMem(const char *filename, TSnd *pSnd, LPWAVEFORMATEX *pFormat, void **pcm_data)
{
    DWORD dwSize; // unused
    DWORD unused;
    HANDLE phFile;

    SFileOpenFile(filename, &phFile);
    dwSize = SFileGetFileSize(phFile, NULL);

    SFileSetFilePointer(phFile, 0x14, NULL, 0);
    SFileReadFile(phFile, *pFormat, sizeof(WAVEFORMATEX), NULL, NULL);
    SFileReadFile(phFile, &unused, 2, NULL, NULL);
    SFileReadFile(phFile, pSnd, 4, NULL, NULL);
    SFileReadFile(phFile, *pcm_data, pSnd->pcm_size, NULL, NULL);

    SFileCloseFile(phFile);
    return 0;
}

// FileGetSize    0000000000482A2A
// Demo specific!
DWORD FileGetSize(const char *filename)
{
    char buffer[256];
    sprintf(buffer, "%s%s", fileLoadPrefix, filename);
    // SFileOpenFile
    // SFileGetFileSize
    // SFileCloseFile
    //  TODO
    return 0;
}

// .text:00482A9D
char *FileAddLoadPrefix(const char *filename)
{
    sprintf(file_to_load_with_base, "%s%s", fileLoadPrefix, filename);
    return file_to_load_with_base;
}

// .text:00482AD3
// Minus the asserts, this is exactly the same as Devilution
void CelBlit(BYTE *pDecodeTo, BYTE *pRLEBytes, int nDataSize, int nWidth)
{
    int w;

    // TODO annotate
    __asm {
        mov        esi, pRLEBytes
        mov        edi, pDecodeTo
        mov        eax, BUFFER_WIDTH
        add        eax, nWidth
        mov        w, eax
        mov        ebx, nDataSize
        add        ebx, esi
    label1:
        mov        edx, nWidth
    label2:
        xor        eax, eax
        lodsb
        or        al, al
        js        label6
        sub        edx, eax
        mov        ecx, eax
        shr        ecx, 1
        jnb        label3
        movsb
        jecxz    label5
    label3:
        shr        ecx, 1
        jnb        label4
        movsw
        jecxz    label5
    label4:
        rep movsd
    label5:
        or        edx, edx
        jz        label7
        jmp        label2
    label6:
        neg        al
        add        edi, eax
        sub        edx, eax
        jnz        label2
    label7:
        sub        edi, w
        cmp        ebx, esi
        jnz        label1
    }
}

// .text:00482B53
// sx and sy are locations into the pixel buffer gpBuffer. Beware that there are
// invisible portions on top/left/bottom/right! See defines.h
//
// nCel is 1-based (don't give 0!)
void CelDraw(int sx, int sy, BYTE *pCelBuff, int nCel, int nWidth)
{
    BYTE *pRLEBytes;
    BYTE *pDest;
    int nDataSize;

    // This kinda looks like asm based on the optimizations of array access.
    // This just sets up the locals used in the next function call.
    __asm {
        mov     ebx, pCelBuff ; // Treat pCelBuff like frame table
        mov     eax, nCel
        shl     eax, 2
        add     ebx, eax ; // ebx now points to current frame offset
        mov     eax, [ebx+4] ; // ebx+4 is next frame offset
        sub     eax, [ebx] ; // length is next frame offset - current frame offset
        mov     nDataSize, eax

        mov     eax, pCelBuff
        add     eax, [ebx]
        mov     pRLEBytes, eax ; // Use frame offset to find start of frame data

        mov     eax, sy
        mov     eax, PitchTbl[eax*4] ; // idk why they use PitchTbl when it's basically sy * 768
        add     eax, sx
        add     eax, gpBuffer
        mov     pDest, eax
    }

    CelBlit(pDest, pRLEBytes, nDataSize, nWidth);
}

// engine_cel_482BB2    0000000000482BB2
// DrawSlabCel    0000000000482BFB
// CelDecodeHdrOnly    0000000000482CBC
// DecodeFullCelL    0000000000482D5F
// DrawFullCelL_xbytes    0000000000482DDC
// CelDecDatLightTrans    0000000000482E56
// engine_cel_482F54    0000000000482F54
// CelDrawLight    0000000000483046
// engine_cel_48312C    000000000048312C
// CelDecodeHdrLightTrans    00000000004831F4
// CelDrawLightTbl    00000000004832E1
// CDecodeFullCel    0000000000483477
// CDrawSlabCel    000000000048350C
// CDrawSlabCelP    00000000004835CD
// Cel2DecDatLightOnly    0000000000483670
// Cel2DecDatLightEntry    0000000000483702
// Cel2DecDatLightTrans    000000000048377C
// CDrawSlabCelL    000000000048388F
// engine_cel_483975    0000000000483975
// Cel2DecodeLightTrans    0000000000483A3D
// CDrawSlabCelI    0000000000483B2A
// CelDecodeRect    0000000000483CD5
// OutlineSlabCel    0000000000483D7E
// CelDrawHdrClrHL    0000000000483E99

// .text:00483FFB
void CelApplyTrans(BYTE *p, BYTE *ttbl, int nCel)
{
    int i;

    for (i = 1; i <= nCel; i++)
    {
        // The star of the show is the XLAT instruction
        // the rest just sets up pointers and loop sizes
        __asm {
            mov     ebx, p
            mov     eax, i
            shl     eax, 2
            add     ebx, eax // ebx is the offset within the group for the frame data

            mov     esi, p // Load group
            add     esi, [ebx] ; // Find frame i data
            add     esi, 10 // Ignore skip header

            mov     edx, [ebx+4]
            sub     edx, [ebx]
            sub     edx, 10 ; // Size is (next offset) - (this offset) - (skip header size)

            mov     edi, esi
            mov     ebx, ttbl // Used in XLAT instruction below

        top:
            xor     eax, eax
            lodsb // Load+store CEL "instruction"
            stosb
            dec     edx
            jz      done

            or      al, al
            js      top // "Transparency" instruction (eax < 0) has no data following

                // "Copy" instruction (eax > 0) is followed by data
                // the byte of instruction is the size of that data
            sub     edx, eax
            mov     ecx, eax

        apply_trans:
            lodsb
                    // Here's a weird instruction: https://www.felixcloutier.com/x86/xlat:xlatb
            xlat // AL = ttbl[AL] in our case, where AL is the pixel 256-color
            stosb
            loop apply_trans

            or      edx, edx
            jnz     top

        done:
            nop // Go to next for-loop iteration
        }

        // I bet the only reason they didn't write this in C++ is that they
        // didn't like the number of instructions that the non-optimized debug
        // compiler generated...
    }
}

// ENG_set_pixel    000000000048407B
// engine_draw_pixel    00000000004840F9
// DrawLine    00000000004841F1

// GetDirection    00000000004848A1
int GetDirection(int x1, int y1, int x2, int y2)
{
    // TODO
    return 0;
}

// .text:004849E2
int random_(int v)
{
    if (v <= 0)
    {
        return 0;
    }
    return rand() % v;
}
