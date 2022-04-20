#include <windows.h>

//
// extern
//

extern char tempstr[64]; // interfac.cpp ???

//
// initialized data (.data:004B87C0)
//

char msgcnt = 0; // index into msgtable

//
// uninitialized data (.data:004B87C0)
//

DWORD unused000; // could be part of spells.cpp
char msgflag;
DWORD unused001;
char msgtable[80]; // queue of messages waiting to be shown
char msgdelay;
DWORD unused002; // could be part objects.cpp

// .text:004578D0
// Taken from Devilution and tweaked
void InitDiabloMsg(char e)
{
    int i;

    for (i = 0; i < msgcnt; i++) {
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

    switch (msgflag) {
    case 1:
        strcpy(tempstr, "No automap available in town");
        break;
    case 2:
        strcpy(tempstr, "No multiplayer functions in demo");
        break;
    case 3:
        strcpy(tempstr, "Direct Sound Creation Failed");
        break;
    case 4:
        strcpy(tempstr, "Temp directory availability error");
        break;
    case 5:
        strcpy(tempstr, "Not enough space to save");
        break;
    case 6:
        strcpy(tempstr, "No Pause in town");
        break;
    case 7:
        strcpy(tempstr, "Copying to a hard disk is recommended");
        break;
    case 8:
        strcpy(tempstr, "Odd sensations...");
        break;
    case 9:
        strcpy(tempstr, "A surge of blood interrupts your thoughts");
        break;
    case 10:
        strcpy(tempstr, "Energy passes through your equipment...");
        break;
    case 11:
        strcpy(tempstr, "Growling is heard throughout the dungeon");
        break;
    case 12:
        strcpy(tempstr, "Your skills increase, but at a price...");
        break;
    case 13:
        strcpy(tempstr, "Did you forget something?");
        break;
    case 14:
        strcpy(tempstr, "You hear a series of creaks and thumps...");
        break;
    case 15:
        strcpy(tempstr, "You are the powerless master of fire!");
        break;
    case 16:
        strcpy(tempstr, "Power comes from your disorientation...");
        break;
    case 17:
        strcpy(tempstr, "You hear a strange cry from the distance");
        break;
    case 18:
        strcpy(tempstr, "You forget who you are!");
        break;
    case 19:
        strcpy(tempstr, "Untold Wealth!");
        break;
    }

    // TODO the rest
}
