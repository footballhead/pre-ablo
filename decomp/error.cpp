#include "error.h"

#include <windows.h>

#include "control.h"

//
// initialized data (.data:004B87C0)
//

char msgcnt = 0; // index into msgtable

//
// uninitialized data (.data:004B87C0)
//

char msgflag;
// Queue of messages waiting to be shown. I guess this is useful because:
// 1. these can be set in the title screen but are only visible once in game
// 2. the game can show multiple error messages by showing one at a time over time
// (I don't think this is every initialized...)
char msgtable[80];
char msgdelay;

//
// code (.text:004578D0)
//

// .text:004578D0
// Taken from Devilution and tweaked
void InitDiabloMsg(char e)
{
    int i;

    for (i = 0; i < msgcnt; i++)
    {
        if (msgtable[i] == e)
            return;
    }

    msgtable[msgcnt] = e;
    if (msgcnt < (BYTE)sizeof(msgtable)) // not sure if cast to BYTE here will generate correct opcodes
        msgcnt++;

    msgflag = msgtable[0];
    msgdelay = 50; // different!!!
}

// .text:0045795C
void DrawDiabloMsg()
{
    // TODO

    // TODO inlined assembly for transparent background

    switch (msgflag)
    {
    case EMSG_NO_AUTOMAP_IN_TOWN:
        strcpy(tempstr, "No automap available in town");
        break;
    case EMSG_NO_MULTIPLAYER_IN_DEMO:
        strcpy(tempstr, "No multiplayer functions in demo");
        break;
    case EMSG_DIRECT_SOUND_FAILED:
        strcpy(tempstr, "Direct Sound Creation Failed");
        break;
    case EMSG_NO_TMP_DIR:
        strcpy(tempstr, "Temp directory availability error");
        break;
    case EMSG_NO_SPACE_TO_SAVE:
        strcpy(tempstr, "Not enough space to save");
        break;
    case EMSG_NO_PAUSE_IN_TOWN:
        strcpy(tempstr, "No Pause in town");
        break;
    case EMSG_COPY_TO_HDD:
        strcpy(tempstr, "Copying to a hard disk is recommended");
        break;
    case EMSG_SHRINE_MYSTERIOUS:
        strcpy(tempstr, "Odd sensations...");
        break;
    case EMSG_SHRINE_IMPOSING:
        strcpy(tempstr, "A surge of blood interrupts your thoughts");
        break;
    case EMSG_SHRINE_HIDDEN:
        strcpy(tempstr, "Energy passes through your equipment...");
        break;
    case EMSG_SHRINE_MAGICAL:
        strcpy(tempstr, "Growling is heard throughout the dungeon");
        break;
    case EMSG_SHRINE_MYSTIC:
        strcpy(tempstr, "Your skills increase, but at a price...");
        break;
    case EMSG_SHRINE_ENCHANTED:
        strcpy(tempstr, "Did you forget something?");
        break;
    case EMSG_SHRINE_THAUMATURGIC:
        strcpy(tempstr, "You hear a series of creaks and thumps...");
        break;
    case EMSG_SHRINE_FASCINATING:
        strcpy(tempstr, "You are the powerless master of fire!");
        break;
    case EMSG_SHRINE_CRYPTIC:
        strcpy(tempstr, "Power comes from your disorientation...");
        break;
    case EMSG_SHRINE_SUPERNATURAL:
        strcpy(tempstr, "You hear a strange cry from the distance");
        break;
    case EMSG_SHRINE_EERIE:
        strcpy(tempstr, "You forget who you are!");
        break;
    case EMSG_SHRINE_SPIRITUAL:
        strcpy(tempstr, "Untold Wealth!");
        break;
    }

    // TODO the rest
}
