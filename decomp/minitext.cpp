#include "minitext.h"

#include <stdlib.h>

#include "diablo.h"
#include "engine.h"
#include "quests.h"
#include "scrollrt.h"

//
// initialized data, starting at .data:004A2479
//

// So here's the deal. The compiler, upon seeing alltext, will generate:
//
// * rawtext in data segment as an array of const char* (as expected)
// * alltext in data segment as an array of NULL pointers (huh?)
// * the rest of the variables in the data segment
// * the actual null-terminated c strings of rawtext in the data segment
// * two functions in code segment, one called by __cinit (global initialization) which references the other. This sets up alltext with the expected elements of rawtext

const char *rawtext[] = {
    "   I would like to ask your help. Years ago a corrupt king ",
    "destroyed the land with necromancy.  He used his dark powers to kill all before him, ",
    "then he would raise the dead to join his army of evil.  One of the king's own knights ",
    "finally struck him down; now he rests in the catacombs of the cathedral.",
    "With evil upon our land once more, I fear that the Skeleton King is again at work; ",
    "building an undead army.  If you slay the evil king, he can not raise his army of skeleton ",
    "warriors to destroy our land.",
    "You will find the king's tomb 3 levels below.|",
    "   The Skeleton Kings undead minions still plague this land! ",
    "You must kill the Dark King or the walking dead will destroy every thing! ",
    "His tomb is 3 levels below.|",
    NULL,
    "   You have saved this land from a terrible evil.  With the King ",
    "again resting in peace, the undead hordes will not return. Thank you.|",
    "   Please help....! I barely escaped from ....... the Butcher...! ",
    "He killed .... my wife ... my children! I beg of you ...... in the bane if ",
    "God ......... avenge them.........|",
    "<..........> The man's body appears to be twisted in agony!|",
    "<..........> The man appears to be in peace.|",
    NULL,
    "   I have a story that I think you might be interested in. ",
    "A few weeks ago a star fell from the sky.  A caravan carried a piece of the sky rock to ",
    "the royal capital for the King's wizards to study.  On the road north of here, they were ",
    "ambushed by evil forces eminating from the cathedral.  I looked among the refuse and ",
    "carnage for the sky rock, but it was no where to be found.  If you can find it, I will ",
    "try to make something out of it. If it is magical, it might be of some use to you!|",
    "   I'm still waiting for you to bring me that piece of star ",
    "that fell. If it is magical, I could make something that you could use ",
    "to fight the evil in our homeland.|",
    NULL,
    "   Hey! It looks like that could be it! Give me some time.....Ah! ",
    "Here you go....I made a ring out of a small piece.  I hope its of some use to you. ",
    "Good Luck!|",
    NULL,
    "...and so the knowledge was too great. With reluctance, the power of ",
    "the awesome and destructive spell was sealed forever beyond the Chamber of Bone. A ",
    "magical stairway marks the way to the Chamber. Many have tried to recover the power ",
    "sealed within, but all have fallen prey to the Guardians. You have been warned.|",
    "   Here is the nova spell huhh..hhuhhhh. words....|",
    NULL,
    "Beyond the Hall of Heros awaits the catacombs. Be warned: He who ",
    "would steal from the dead. Traps and more await below to deal with would be thieves.|",
    "Thank goodness you've returned! Much has changed since you lived here, ",
    "my friend. All was peaceful until the dark riders came and destroyed ",
    "our village. Many were cut down where they stood. Those who ",
    "took up arms were slain or dragged away to become slaves - or worse. ",
    "The church at the edge of town has been desecrated and is being used ",
    "for dark rituals. The screams that echo in the night are inhuman, but ",
    "some of our townsfolk may yet survive. Follow the path that lies ",
    "between my tavern and the blacksmith shop to find the church and ",
    "save who you can. ",
    "Perhaps I can tell you more if we speak again. Good luck.|",
    "There is an old woman who lives across the river to the east. ",
    "It is said that she knows much about magic. She may be able ",
    "to help you in your quest.|",
    NULL,
    "Continue on your quest, my friend. Only the destruction ",
    "of the evil that binds us can bring us peace.|"};

const char *alltext[] = {
    rawtext[0],  // skelking intro
    rawtext[8],  // skelking nag
    rawtext[12], // skelking done
    rawtext[14], // butcher intro
    rawtext[17], // butcher nag
    rawtext[18], // butcher done
    rawtext[20], // magic rock intro
    rawtext[26], // magic rock nag
    rawtext[30], // magic rock done
    rawtext[34], // bonechmb start
    rawtext[38], // bonechmb done
    rawtext[40], // maze open
    rawtext[42], // ogden greet
    rawtext[52], // ogden gossip
    rawtext[56], // ogden nag
    NULL};

int txtspd[] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0};

const BYTE mfontframe[] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 37, 49,
    38, 0, 39, 40, 47, 42, 43, 41, 45, 52, 44, 53, 55, 36,
    27, 28, 29, 30, 31, 32, 33, 34, 35, 51, 50, 48, 46,
    49, 54, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13,
    14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26,
    42, 0, 43, 0, 0, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11,
    12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24,
    25, 26, 48, 0, 49, 0, 0};

const BYTE mfontkern[] = {
    5, 15, 10, 13, 14, 10, 9, 13, 11, 5, 5, 11, 10, 16,
    13, 16, 10, 15, 12, 10, 14, 17, 17, 22, 17, 16, 11,
    5, 11, 11, 11, 10, 11, 11, 11, 11, 15, 5, 10, 18, 15,
    8, 6, 6, 7, 10, 9, 6, 10, 10, 5, 5, 5, 5, 11, 12};

//
// uninitialized data, starting at .data:005DDD50
//

BYTE *pMedTextCels;
BOOL qtextflag;
int scrolltexty;
int qtexty;
BYTE *pTextBoxCels;
const char *qtextptr;
int qtextSpd;

//
// code, starting at .text:0040E010
//

// .text:0040E010 - Generated by compiler for alltext global initialization
// .text:0040E025 - Generated by compiler for alltext global initialization

// .text:0040E0CB
// I think this is basically unchanged from Devilution
void InitQuestText()
{
    pMedTextCels = LoadFileInMem("Data\\MedTextS.CEL");
    pTextBoxCels = LoadFileInMem("Data\\TextBox.CEL");
    qtextflag = FALSE;
}

// .text:0040E103
void InitQTextMsg(int m)
{
    questlog = FALSE;
    qtextflag = TRUE;
    qtextptr = alltext[m];
    qtexty = 500;
    scrolltexty = 0;
    qtextSpd = txtspd[m];
}

// .text:0040E15F
// I think this is basically unchanged from Devilution
void DrawQTextBack()
{
    // Devilution has constants for all these magic numbers
    CelDraw(88, 487, pTextBoxCels, 1, 591);
    // According to Devilution, this is hand-written asm
    // Draw a 50% "transparent" rectangle through dithering
    __asm {
        mov edi, gpBuffer
        add edi, 371803
        xor eax, eax
        mov edx, 148
    yloop:
            // First row "on-pixel" dither
        mov ecx, 292
    x0loop:
        stosb // store AL (which is 0) at address ES:EDI, then increment EDI
        inc edi // skip a pixel
        loop x0loop // decrement ECX then jump, unless ECX is now 0
        stosb
                            // Next row "off-pixel" dither
        sub edi, 1353
        mov ecx, 292
    x1loop:
        inc edi
        stosb
        loop x1loop
        sub edi, 1532
        dec edx
        jnz yloop           // Keep doing this until all rows done
                // Final row
        mov ecx, 292
    x2loop:
        stosb
        inc edi
        loop x2loop
        stosb
    }
    ;
}

// .text:0040E1CE
// This is unchanged from Devilution
void PrintQTextChr(int sx, int sy, BYTE *pCelBuff, int nCel)
{
    BYTE *dst, *pStart, *pEnd, *end;

    dst = &gpBuffer[sx + PitchTbl[sy]];
    pStart = &gpBuffer[PitchTbl[209]];
    pEnd = &gpBuffer[PitchTbl[469]];

    __asm {
        mov ebx, pCelBuff
        mov eax, nCel
        shl eax, 2
        add ebx, eax
        mov eax, [ebx+4]
        sub eax, [ebx]
        mov end, eax
        mov esi, pCelBuff
        add esi, [ebx]
        mov edi, dst
        mov ebx, end
        add ebx, esi
    label1:
        mov edx, 22
    label2:
        xor eax, eax
        lodsb
        or al, al
        js label7
        sub edx, eax
        cmp edi, pStart
        jb label5
        cmp edi, pEnd
        ja label5
        mov ecx, eax
        shr ecx, 1
        jnb label3
        movsb
        jecxz label6
    label3:
        shr ecx, 1
        jnb label4
        movsw
        jecxz label6
    label4:
        rep movsd
        jmp label6
    label5:
        add esi, eax
        add edi, eax
    label6:
        or edx, edx
        jz label8
        jmp label2
    label7:
        neg al
        add edi, eax
        sub edx, eax
        jnz label2
    label8:
        sub edi, 790
        cmp ebx, esi
        jnz label1
    }
}

// DrawQText    000000000040E2AA

// .text:0040E569
void FreeQuestText()
{
    MemFreeDbg(pMedTextCels);
    MemFreeDbg(pTextBoxCels);
}
