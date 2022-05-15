#ifndef __OBJECTS_H__
#define __OBJECTS_H__

#include <windows.h>

//
// defines
//

#define MAXOBJECTS 127

//
// enums
//

enum shrine_type
{
    SHRINE_MYSTERIOUS,
    SHRINE_IMPOSING,
    SHRINE_HIDDEN,
    SHRINE_MAGICAL,
    SHRINE_MYSTIC,
    SHRINE_ENCHANTED,
    SHRINE_THAUMATURGIC,
    SHRINE_FASCINATING,
    SHRINE_CRYPTIC,
    SHRINE_SUPERNATURAL,
    SHRINE_EERIE,
    SHRINE_HOLY,
    SHRINE_SPIRITUAL,
    NUM_SHRINETYPE
};

//
// structs
//

#pragma pack(push, 8)
struct ObjectStruct
{
    int _otype;
    int _ox;
    int _oy;
    int _oLight;
    BOOL _oAnimFlag;
    BYTE *_oAnimData;
    int _oAnimDelay;
    int _oAnimCnt;
    int _oAnimLen;
    int _oAnimFrame;
    int _oAnimWidth;
    int _oAnimWidth2;
    int _oDelFlag;
    char _oBreak;
    char anonymous_0[3];
    int _oSolidFlag;
    int _oMissFlag;
    char _oSelFlag;
    char anonymous_1[3];
    int _oPreFlag;
    int _oTrapFlag;
    int _oDoorFlag;
    int _olid;
    int _oVar1; // if shrine then shrine type (see OperateShrine)
    int _oVar2;
    int _oVar3;
    int _oVar4;
    int _oVar5;
    int _oVar6;
    int _oVar7;
    int _oVar8;
};
#pragma pack(pop)

//
// variables
//

extern ObjectStruct object[MAXOBJECTS];
extern BOOL InitObjFlag;

//
// functions
//

void SetObjMapRange(int i, int x1, int y1, int x2, int y2, int v);
void SetBookMsg(int i, int msg);
void AddObject(int ot, int ox, int oy);
void ObjSetMicro(int dx, int dy, int pn);
void AddL1Objs(int x1, int y1, int x2, int y2);
void AddL2Objs(int x1, int y1, int x2, int y2);

#endif