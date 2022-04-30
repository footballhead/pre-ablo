#ifndef __OBJDAT_H__
#define __OBJDAT_H__

#include <windows.h>

//
// structs
//

#pragma pack(push, 8)
struct ObjDataStruct
{
    char oload;
    char ofindex;
    char ominlvl;
    char omaxlvl;
    char olvltype;
    char otheme;
    int oAnimFlag;
    int oAnimDelay;
    int oAnimLen;
    int oAnimWidth;
    BOOL oSolidFlag;
    BOOL oMissFlag;
    BOOL oLightFlag;
    char oBreak;
    char oSelFlag;
    BOOL oTrapFlag;
};
#pragma pack(pop)

//
// variables
//

extern ObjDataStruct AllObjects[69]; // nice

#endif