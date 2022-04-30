#include "debug.h"

#include "defines.h"
#include "diablo.h"
#include "engine.h"

#include <stdio.h>

//#define BUFFER_LEFT_AND_RIGHT (BUFFER_LEFT + BUFFER_RIGHT)
#define BUFFER_LEFT_AND_RIGHT 128

//
// uninitialized vars (.data:005FD0A8)
//

BYTE *pSquareCels;

//
// code (.text:0044AB80)
//

// .text:0044AB80
void LoadDebugGFX()
{
    pSquareCels = LoadFileInMem("Data\\Square.CEL");
}

// .text:0044AB9F
void FreeDebugGFX()
{
    GlobalUnlock(GlobalHandle(pSquareCels));
    GlobalFree(GlobalHandle(pSquareCels));
}

// .text:000000000044ABD5
// This function is not called.
// Wrapper around TextOut for some kinds of numbers
// @param fmt Either 'c' (char), 'i' (integer), or 'l' (long)
void __dc_debug_print_number(int x, int y, char fmt, int to_print)
{
    char buffer[52];
    HDC hdc;

    hdc = GetDC(NULL);

    switch (fmt)
    {
    case 'c':
        TextOutA(hdc, x, y, buffer, sprintf(buffer, "%c", to_print));
        break;
    case 'i':
        TextOutA(hdc, x, y, buffer, sprintf(buffer, "%i", to_print));
        break;
    case 'l':
        TextOutA(hdc, x, y, buffer, sprintf(buffer, "%ld", to_print));
        break;
    }

    ReleaseDC(NULL, hdc);
}

// .text:0044ACCF
// Encode gpBuffer as a PCX (RLE compress data, slap on PCX header) and save to disk
void CaptureScreen(int pic_idx, BYTE *palette)
{
    char path[512];
    DWORD dwBytes;
    BYTE *lpBuffer;
    HFILE hFile;

    sprintf(path, "%sDScrn_%i.PCX", fileLoadPrefix, pic_idx);
    hFile = _lcreat(path, 0);
    // 0x100000 is ~1MB. I think was chosen as an arbitrarily big number to
    // ensure there's enough space. That gives ~3 bytes per pixel at 640x480
    // which, given that we're compressing the data, is way more than enough!
    lpBuffer = (BYTE *)GlobalLock(GlobalAlloc(0, 0x100000));
    __asm {
            mov     edi, lpBuffer

                     // PCX reference: https://docs.fileformat.com/image/pcx/
                     // Store header
            mov     al, 0Ah // PCX Id Number (always 0x0A)
            stosb
            mov     al, 5 // Version number (5 means PC Paintbrush 3.0)
            stosb
            mov     al, 1 // Encoding (1 means RLE)
            stosb
            mov     al, SCREEN_BPP // BitsPerPixel
            stosb
            xor     ax, ax // Left of image
            stosw
            xor     ax, ax // Top of image
            stosw
            mov     ax, SCREEN_WIDTH - 1 // Right of Image
            stosw
            mov     ax, SCREEN_HEIGHT - 1 // Bottom of Image
            stosw
            mov     ax, SCREEN_WIDTH // Horizontal resolution
            stosw
            mov     ax, SCREEN_HEIGHT // Vertical resolution
            stosw

                // Store first 16 entries of palette
                // The entire 256-color palette is stored after image data
            mov     esi, palette
            mov     ecx, 16
        store_header_palette:
            mov     al, [esi]
            stosb
            mov     al, [esi+1]
            stosb
            mov     al, [esi+2]
            stosb
            add     esi, 4
            loop    store_header_palette

            xor     al, al ; // Reserved (Always 0);
            stosb
            mov     al, 1 // Number of bit planes
            stosb
            mov     ax, SCREEN_WIDTH // Bytes per scan-line
            stosw
            xor     al, al // Rest of header (60 bytes) is 0
           // This means    PaletteType = 0
           //               HorzScreenSize = 0
           //               VertScreenSize = 0
           //               Reserved2[54] = {0}
            mov     ecx, 60
            rep stosb

            // Perform RLE compression on gpBuffer.
            // EDX is RLE run width
            // AL is the RLE run character
            // New data is read into AH. AH is then compared to AL to determine
            // whether it's part of that row or whether to start a new row
            // ECX is the column index. RLE runs do not extend across rows.
            mov     esi, gpBuffer
            add     esi, SCREENXY(0, 0)
            mov     ecx, SCREEN_HEIGHT
        start_of_row:
            push    ecx
            mov     ecx, SCREEN_WIDTH
        start_of_run:
            mov     edx, 1 // Runs start 1px long
            xor     eax, eax // Ensure EAX is clean (both high and low)
            lodsb // Load 1 byte from gpBuffer into AL
            dec ecx
            jz      end_of_run // Treat the end of a row as the end of a run
                                  // Maybe to match "Bytes per scan-line"?

        run_read:
            mov     ah, [esi]
            cmp     ah, al
            jnz     end_of_run  ; // Diferent pixel, end of run!

            inc     edx
            inc     esi
            dec     ecx
            cmp     edx, 3Fh
            xor     ecx, ecx
            jnz     run_read          // Otherwise end of run is a run of size 63 or the end of the row (whatever is first)

        end_of_run:
            cmp     edx, 1
            jz      end_of_run_of_1

        write_run:
                                  // Encode + write run length (0xC0 seems to indicate that the byte is encoding run length)
            or      dl, 0C0h
            mov     [edi], dl
            inc     edi
                                  // Write AL, the run pixel
            stosb
            jmp     check_new_row_or_run

        end_of_run_of_1:
            // Rows of size 1 may not need to write their length
            cmp     al, 0BFh
            ja      write_run
            stosb                 // Store the pixel without length (assumed run length is 1)

        check_new_row_or_run:
            cmp     ecx, 0
            jnz     start_of_run

            // End of row, move to the next one
            pop     ecx
            add     esi, BUFFER_LEFT_AND_RIGHT
            loop start_of_row

                // END OF IMAGE DATA!
                // Start storing palette
            mov     al, 12 // Mark start of 256-color palette
            stosb

            mov     esi, palette
            mov     ecx, 256
        store_palette:
            mov     al, [esi]
            stosb
            mov     al, [esi+1]
            stosb
            mov     al, [esi+2]
            stosb
            add     esi, 4
            loop    store_palette

            mov     eax, edi
            sub     eax, lpBuffer
            mov     dwBytes, eax
    }

    _lwrite(hFile, (LPCCH)lpBuffer, dwBytes);
    _lclose(hFile);

    GlobalUnlock(GlobalHandle(lpBuffer));
    GlobalFree(GlobalHandle(lpBuffer));
}
