#include "scrollrt.h"

#include "defines.h"
#include "diablo.h"

//
// Uninitialized variables
//

// PitchTbl[row] is the the gpBuffer offset for that row.
// Initialized in diablo_init_screen
// Seems weird that they precompute basic multiplcation...
// (Each element is i * BUFFER_WIDTH)
int PitchTbl[1024];

//
// Code
//

// DrawMissile	000000000047BE30
// CDrawMissile	000000000047C1B3
// DrawMonster	000000000047C535
// CDrawMSlabCelL	000000000047C732
// DrawObjCel	000000000047C92F
// CDrawObjCel	000000000047CBCC
// DrawEFlag1	000000000047CE69
// DrawHTLXsub	000000000047D02E
// DrawHTileLineX	000000000047DC7A
// DrawEFlag2	000000000047E2BC
// DrawHTLXsub2	000000000047E547
// DrawHTLX2	000000000047F1E7
// DrawEFlag3	000000000047F6D7
// DrawHTLXsub3	000000000047F8AF
// DrawHTLX3	000000000048054F
// SVGADrawView	0000000000480BD6
// VGADrawView	0000000000480EC1
// DrawView	000000000048120E

// .text:004813C2
// Fill the visible area with 0s
void ClearScreenBuffer()
{
    __asm {
        mov edi, gpBuffer
        add edi, 122944 //; (BUFFER_WIDTH) * BORDER_TOP + BORDER_LEFT
        mov edx, 480 //; SCREEN_HEIGHT
        xor eax, eax

    blank_row:
        mov ecx, 160 //; SCREEN_WIDTH / sizeof(DWORD)
        rep stosd
        add edi, 128    //; BUFFER_LEFT + BUFFER_RIGHT
        dec edx
        jnz blank_row
    }
}

// ScrollView	00000000004813F9
void ScrollView()
{
    // TODO
}

// .text:00481653
static void DrawMain(int bufferpos, int dwHgt, BOOL draw_cursor, BOOL draw_desc, BOOL draw_hp, BOOL draw_mana, BOOL draw_sbar, BOOL draw_btn)
{
    HRESULT hDDVal;
    DDSURFACEDESC ddsd;

    if (!gbActive)
    {
        return;
    }

    if (lpDDSBackBuf->IsLost() == DDERR_SURFACELOST)
    {
        lpDDSBackBuf->Restore();
    }

    hDDVal = lpDDSPrimary->Lock(NULL, &ddsd, DDLOCK_WAIT, NULL);

    // TODO
}

// .text:00481AFC
void scrollrt_draw_game_screen(BOOL draw_cursor)
{
    if (force_redraw == 4)
    {
        DrawMain(BORDER_TOP * BUFFER_WIDTH + BORDER_LEFT, SCREEN_HEIGHT, TRUE, FALSE, FALSE, FALSE, FALSE, FALSE);
    }
    else
    {
        DrawMain(0, 0, TRUE, FALSE, FALSE, FALSE, FALSE, FALSE);
    }
    force_redraw = 0;
}

// DrawAndBlit	0000000000481B60
