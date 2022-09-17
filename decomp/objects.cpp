#include "objects.h"

#include <stdio.h>

#include "control.h"
#include "cursor.h"
#include "defines.h"
#include "diablo.h"
#include "effects.h"
#include "engine.h"
#include "enums.h"
#include "error.h"
#include "gendung.h"
#include "items.h"
#include "lighting.h"
#include "monster.h"
#include "minitext.h"
#include "missiles.h"
#include "objdat.h"
#include "player.h"
#include "quests.h"
#include "stores.h"
#include "themes.h"

//
// initialized data (.data:004B8A50)
//

static int bxadd[] = {-1, 0, 1, -1, 1, -1, 0, 1};
static int byadd[] = {-1, -1, -1, 0, 0, 1, 1, 1};

static const char *shrinestrs[] = {
    "Mysterious",
    "Imposing",
    "Hidden",
    "Magical",
    "Mystic",
    "Enchanted",
    "Thaumaturgic",
    "Fascinating",
    "Cryptic",
    "Supernatural",
    "Eerie",
    "Holy",
    "Spiritual"};

//
// unintialized data (.data:00600078)
//

int trapdir;
int nobjects;
int numobjfiles;

char ObjFileList[40];        // indexed by numobjfiles
int objectavail[MAXOBJECTS]; // indexed by nobjects
ObjectStruct object[MAXOBJECTS];
BOOL InitObjFlag;
int leverid;
int objectactive[MAXOBJECTS];
BYTE *pObjCels[40];
int trapid;

//
// code (.text:00457E30)
//

// .text:00457E30
// Load all the object GFX that we think we need. Requires that themes are
// alredy initailized. If an object is missed for some reason then the game will
// run but only until the point where it tries to draw that object... then it
// will crash.
// @pre themes are initailized
// @post pObjCels contains graphics data
void InitObjectGFX()
{
    int j;
    int i;
    BOOL fileload[OBJ_LOAD_LIST_SIZE];
    char filestr[32];

    for (i = 0; i < OBJ_LOAD_LIST_SIZE; i++)
    {
        fileload[i] = FALSE;
    }

    // Figure out which object graphics we could possibly need
    for (i = 0; AllObjects[i].oload != -1; i++)
    {
        if (AllObjects[i].oload == 1 && currlevel >= AllObjects[i].ominlvl && currlevel <= AllObjects[i].omaxlvl)
        {
            fileload[AllObjects[i].ofindex] = TRUE;
        }
        if (AllObjects[i].otheme != THEME_NONE)
        {
            for (j = 0; j < numthemes; j++)
            {
                if (themes[j].ttype == AllObjects[i].otheme)
                {
                    fileload[AllObjects[i].ofindex] = TRUE;
                }
            }
        }
    }

    numobjfiles = 0;

    // Load them all
    for (i = 0; i < OBJ_LOAD_LIST_SIZE; i++)
    {
        if (fileload[i])
        {
            // BUG: If numobjfiles > 40 (unlikely) then this overruns the array bounds
            // Would have to load all objects
            ObjFileList[numobjfiles] = i;
            sprintf(filestr, "Objects\\%s.CEL", ObjMasterLoadList[i]);
            // BUG: If numobjfiles > 40 (unlikely) then this overruns the array bounds
            // Would have to load all objects
            pObjCels[numobjfiles] = LoadFileInMem(filestr);
            numobjfiles++;
        }
    }
}

// .text:00458029
// Free all pObjCels
void FreeObjectGFX()
{
    int i;

    for (i = 0; i < numobjfiles; i++)
    {
        MemFreeDbg(pObjCels[i])
    }
}

// .text:0045808F
// Is the given location OK for placing an object?
BOOL RndLocOk(int xp, int yp)
{
    BOOL ok;

    ok = TRUE;
    if (dMonster[xp][yp] != 0)
    {
        ok = FALSE;
    }
    if (dPlayer[xp][yp] != 0)
    {
        ok = FALSE;
    }
    if (dObject[xp][yp] != 0)
    {
        ok = FALSE;
    }
    if (dFlags[xp][yp] & BFLAG_POPULATED)
    {
        ok = FALSE;
    }
    if (nSolidTable[dPiece[xp][yp]])
    {
        ok = FALSE;
    }
    if (leveltype == DTYPE_CATHEDRAL || leveltype == DTYPE_OLD_CATHEDRAL)
    {
        if (dPiece[xp][yp] > 126 && dPiece[xp][yp] < 144)
        {
            ok = FALSE;
        }
    }

    return ok;
}

// TODO: __dc_TrapLocOk    00000000004581D8
// TODO: __dc_RoomLocOk    000000000045824E

// .text:00458312
// Create a random number of objects (between min and max) at random, valid
// dungeon locations. A 3x3 square of free space is required to place the
// object.
//
// If there's no free space then this will never return!
static void InitRndLocObj(int min, int max, int objtype)
{
    int xp;
    int yp;
    BOOL ok;
    int i;
    int numobjs;

    numobjs = random_(max - min) + min;
    for (i = 0; i < numobjs; i++)
    {
        ok = FALSE;
        while (!ok)
        {
            ok = TRUE;

            xp = random_(80) + 16; // TODO: Magic numbers
            yp = random_(80) + 16;

            ok &= RndLocOk(xp - 1, yp - 1);
            ok &= RndLocOk(xp, yp - 1);
            ok &= RndLocOk(xp + 1, yp - 1);
            ok &= RndLocOk(xp - 1, yp);
            ok &= RndLocOk(xp, yp);
            ok &= RndLocOk(xp + 1, yp);
            ok &= RndLocOk(xp - 1, yp + 1);
            ok &= RndLocOk(xp, yp + 1);
            ok &= RndLocOk(xp + 1, yp + 1);
        }

        AddObject(objtype, xp, yp);
    }
}

// .text:00458438
// Like InitRndLocObj but the required space is 3x4
static void InitRndLocBigObj(int min, int max, int objtype)
{
    int xp;
    int yp;
    BOOL ok;
    int i;
    int numobjs;

    numobjs = random_(max - min) + min;
    for (i = 0; i < numobjs; i++)
    {
        ok = FALSE;
        while (!ok)
        {
            ok = TRUE;

            xp = random_(80) + 16;
            yp = random_(80) + 16;

            ok &= RndLocOk(xp - 1, yp - 2);
            ok &= RndLocOk(xp, yp - 2);
            ok &= RndLocOk(xp + 1, yp - 2);
            ok &= RndLocOk(xp - 1, yp - 1);
            ok &= RndLocOk(xp, yp - 1);
            ok &= RndLocOk(xp + 1, yp - 1);
            ok &= RndLocOk(xp - 1, yp);
            ok &= RndLocOk(xp, yp);
            ok &= RndLocOk(xp + 1, yp);
            ok &= RndLocOk(xp - 1, yp + 1);
            ok &= RndLocOk(xp, yp + 1);
            ok &= RndLocOk(xp + 1, yp + 1);
        }

        AddObject(objtype, xp, yp);
    }
}

// .text:00458593
// Reset objects and associated state. Basically the same as Devilution
static void ClrAllObjects()
{
    int i;

    for (i = 0; i < MAXOBJECTS; i++)
    {
        object[i]._ox = 0;
        object[i]._oy = 0;
        object[i]._oAnimData = 0;
        object[i]._oAnimDelay = 0;
        object[i]._oAnimCnt = 0;
        object[i]._oAnimLen = 0;
        object[i]._oAnimFrame = 0;
        object[i]._oDelFlag = FALSE;
        object[i]._oVar1 = 0;
        object[i]._oVar2 = 0;
        object[i]._oVar3 = 0;
        object[i]._oVar4 = 0;
    }

    nobjects = 0;
    for (i = 0; i < MAXOBJECTS; i++)
    {
        objectavail[i] = i;
        objectactive[i] = 0;
    }

    trapid = 1;
    trapdir = 0;
    leverid = 1;
}

// .text:00458745
// Add dead bodies in Butcher Chamber. Same as Devilution
static void AddTortures()
{
    int ox, oy;

    for (oy = 0; oy < MAXDUNY; oy++)
    {
        for (ox = 0; ox < MAXDUNX; ox++)
        {
            if (dPiece[ox][oy] == 367)
            {
                AddObject(OBJ_TORTURE1, ox, oy + 1);
                AddObject(OBJ_TORTURE3, ox + 2, oy - 1);
                AddObject(OBJ_TORTURE2, ox, oy + 3);
                AddObject(OBJ_TORTURE4, ox + 4, oy - 1);
                AddObject(OBJ_TORTURE5, ox, oy + 5);
                AddObject(OBJ_TNUDEM1, ox + 1, oy + 3);
                AddObject(OBJ_TNUDEM2, ox + 4, oy + 5);
                AddObject(OBJ_TNUDEM3, ox + 2, oy);
                AddObject(OBJ_TNUDEM4, ox + 3, oy + 2);
                AddObject(OBJ_TNUDEW1, ox + 2, oy + 4);
                AddObject(OBJ_TNUDEW2, ox + 2, oy + 1);
                AddObject(OBJ_TNUDEW3, ox + 4, oy + 2);
            }
        }
    }
}

// TODO: __dc_AddTrapLine    00000000004588B3
// TODO: __dc_AddLeverObj    0000000000458C09

// .text:00458D47
static void AddBookLever(int lx1, int ly1, int lx2, int ly2, int x1, int y1, int x2, int y2, int msg)
{
    int xp;
    int yp;
    BOOL ok;
    int ob;

    ok = FALSE;
    while (!ok)
    {
        ok = TRUE;

        // TODO: Having trouble with the disassembly:
        //   mov     eax, [ebp+x2]
        //   sub     eax, [ebp+x1]
        //   lea     ecx, [eax+1]    ; What the hell is this?
        //   call    random_
        //   mov     ecx, [ebp+x1]
        //   add     ecx, eax
        //   mov     [ebp+xp], ecx
        xp = random_(lx2 - lx1) + lx1;
        yp = random_(ly2 - ly1) + ly1;

        // The hilarious part is that retail/Devilution doesn't use lx1..ly2.
        // Looks like the function was rewritten at some point

        ok &= RndLocOk(xp - 1, yp - 1);
        ok &= RndLocOk(xp, yp - 1);
        ok &= RndLocOk(xp + 1, yp - 1);
        ok &= RndLocOk(xp - 1, yp);
        ok &= RndLocOk(xp, yp);
        ok &= RndLocOk(xp + 1, yp);
        ok &= RndLocOk(xp - 1, yp + 1);
        ok &= RndLocOk(xp, yp + 1);
        ok &= RndLocOk(xp + 1, yp + 1);
    }

    AddObject(OBJ_BOOKLVR, xp, yp);
    ob = dObject[xp][yp] - 1;
    SetObjMapRange(ob, x1, y1, x2, y2, leverid);
    SetBookMsg(ob, msg);
    leverid++;
}

// .text:00458E90
// Create [3..8] groups of barrels at random locations around the level. Does
// not count theme rooms.
static void InitRndBarrels()
{
    int xp, p, t, yp;
    BOOL found;
    int i, dir, numobjs, o;

    // Generate [5..8] groups
    numobjs = random_(5) + 3;
    for (i = 0; numobjs > i; i++)
    {
        // Find a free spot for the first barrel
        found = FALSE;
        while (!found)
        {
            xp = random_(80) + 16;
            yp = random_(80) + 16;
            found = RndLocOk(xp, yp);
        }

        // Create a "seed" barrel, there a chance this group will grow
        // 1 in 4 chance of the "seed" barrel being explosive
        if (random_(4) != 0)
        {
            o = OBJ_BARREL;
        }
        else
        {
            o = OBJ_BARRELEX;
        }
        AddObject(o, xp, yp);

        // Try to place additional barrels close by, creating a group.
        // p is chance that group will continue to grow. Starts off at 100%,
        // next is 50%, after is 25%, ... etc... Also, stop growing the group if
        // we don't find empty space
        p = 1;
        while (random_(p >> 1) == 0 && found)
        {
            // Try 3 times to find a neighbouring empty space
            t = 0;
            found = FALSE;
            while (!found && t < 3)
            {
                dir = random_(8);
                xp += bxadd[dir];
                yp += byadd[dir];
                found = RndLocOk(xp, yp);
                t++;
            }

            // Create another barrel. 1 in 5 chance of being explosive
            if (found)
            {
                if (random_(5) != 0)
                {
                    o = OBJ_BARREL;
                }
                else
                {
                    o = OBJ_BARRELEX;
                }
                AddObject(o, xp, yp);
                p++;
            }
        }
    }
}

// .text:00459015
// Add objects that go with certain cathedral tiles, e.g. doors and lights.
// Same as devilution
void AddL1Objs(int x1, int y1, int x2, int y2)
{
    int i, j, pn;

    for (j = y1; j < y2; j++)
    {
        for (i = x1; i < x2; i++)
        {
            pn = dPiece[i][j];
            if (pn == 270)
                AddObject(OBJ_L1LIGHT, i, j);
            if (pn == 44 || pn == 51 || pn == 214)
                AddObject(OBJ_L1LDOOR, i, j);
            if (pn == 46 || pn == 56)
                AddObject(OBJ_L1RDOOR, i, j);
        }
    }
}

// .text:004590FA
// Add objects that go with certain cathedral tiles, e.g. doors.
// Same as devilution
void AddL2Objs(int x1, int y1, int x2, int y2)
{
    int i, j, pn;

    for (j = y1; j < y2; j++)
    {
        for (i = x1; i < x2; i++)
        {
            pn = dPiece[i][j];
            if (pn == 13 || pn == 541)
                AddObject(OBJ_L2LDOOR, i, j);
            if (pn == 17 || pn == 542)
                AddObject(OBJ_L2RDOOR, i, j);
        }
    }
}

// .text:004591DF
static BOOL WallTrapLocOok(int xp, int yp)
{
    BOOL ok;

    ok = TRUE;

    if (dFlags[xp][yp] & BFLAG_POPULATED)
    {
        ok = FALSE;
    }

    if (nTrapTable[dPiece[xp][yp]])
    {
        ok = FALSE;
    }

    return ok;
}

// .text:00459255
static void AddObjTraps()
{
    // TODO

    //     if (currlevel == 1)
    //         rndv = 40;
    //     if (currlevel >= 2)
    //         rndv = 30;
    //     if (currlevel >= 5)
    //         rndv = 20;
    //     if (currlevel >= 7)
    //         rndv = 10;
    //     for (j = 0; j < MAXDUNY; j++)
    //     {
    //         for (i = 0; i < MAXDUNX; i++)
    //         {
    //             if (dObject[i][j] > 0 && random_(rndv))
    //             {
    //                 oi = dObject[i][j] - 1;
    //                 if (AllObjects[object[oi]._otype].oTrapFlag) {
    //                     // TODO
    //                 }
    //             }
    //         }
    //     }
}

// .text:0045951F
// Create all objects (apart from theme rooms) as part of level gen.
// @pre Level tiles already generated
void InitObjects()
{
    int i, j, pn;

    ClrAllObjects();

    InitObjFlag = TRUE;
    if (leveltype == DTYPE_OLD_CATHEDRAL)
    {
        if (currlevel == quests[Q_BUTCHER]._qlevel)
        {
            AddTortures();
        }

        // BUG: OBJ_SKFIRE was changed to a theme room in DTYPE_CATHEDRAL so it
        // it isn't loaded here. The game will start but then crash as soon as
        // it tries to draw the object with missing graphics.
        InitRndLocObj(5, 10, OBJ_SKFIRE);
        InitRndLocBigObj(10, 15, OBJ_SARC);
        AddL1Objs(0, 0, MAXDUNX, MAXDUNY);
    }

    if (leveltype == DTYPE_CATACOMBS)
    {
        AddL2Objs(0, 0, MAXDUNX, MAXDUNY);

        // Basically AddL2Torches
        for (j = 0; j < MAXDUNY; j++)
        {
            for (i = 0; i < MAXDUNX; i++)
            {
                pn = dPiece[i][j];
                if (pn == 1 && random_(3) == 0)
                {
                    AddObject(OBJ_TORCHL2, i, j);
                }
                if (pn == 5 && random_(3) == 0)
                {
                    AddObject(OBJ_TORCHR2, i, j);
                }
                if (pn == 37 && random_(10) == 0 && dObject[i - 1][j] == 0)
                {
                    AddObject(OBJ_TORCHL, i - 1, j);
                }
                if (pn == 41 && random_(10) == 0 && dObject[i][j - 1] == 0)
                {
                    AddObject(OBJ_TORCHR, i, j - 1);
                }
            }
        }
    }

    if (leveltype == DTYPE_CATHEDRAL)
    {
        if (currlevel == quests[Q_BUTCHER]._qlevel)
        {
            AddTortures();
        }

        InitRndLocBigObj(10, 15, OBJ_SARC);
        AddL1Objs(0, 0, MAXDUNX, MAXDUNY);

        if (currlevel == quests[Q_MAZE]._qlevel)
        {
            // TODO 11 is magic number
            AddBookLever(0, 0, MAXDUNX, MAXDUNY,
                         setpc_x, setpc_y, setpc_x + setpc_w + 1, setpc_y + setpc_h + 1,
                         11);
        }

        InitRndBarrels();
    }

    if (currlevel == quests[Q_ROCK]._qlevel)
    {
        InitRndLocObj(1, 1, OBJ_STAND);
    }
    if (currlevel == quests[Q_SCHAMB]._qlevel - 1)
    {
        InitRndLocObj(1, 1, OBJ_BOOK2R);
    }

    InitRndLocObj(5, 10, OBJ_CHEST1);
    InitRndLocObj(3, 6, OBJ_CHEST2);
    InitRndLocObj(1, 5, OBJ_CHEST3);

    AddObjTraps();
    InitObjFlag = FALSE;
}

// TODO: SetMapObjects    0000000000459817

// .text:00459B79
void DeleteObject_(int oi, int i)
{
    int ox, oy;

    ox = object[oi]._ox;
    oy = object[oi]._oy;
    dObject[ox][oy] = 0;
    objectavail[-nobjects + MAXOBJECTS] = oi;
    nobjects--;
    if (nobjects > 0 && i != nobjects)
        objectactive[i] = objectactive[nobjects];
}

// .text:00459C24
void SetupObject(int i, int x, int y, int ot)
{
    int ofi;
    int j;

    object[i]._otype = ot;
    object[i]._ox = x;
    object[i]._oy = y;

    ofi = AllObjects[ot].ofindex;
    for (j = 0; ObjFileList[j] != ofi; j++)
    {
    }

    object[i]._oAnimData = pObjCels[j];
    object[i]._oAnimFlag = AllObjects[ot].oAnimFlag;
    if (AllObjects[ot].oAnimFlag)
    {
        object[i]._oAnimDelay = AllObjects[ot].oAnimDelay;
        object[i]._oAnimCnt = random_(AllObjects[ot].oAnimDelay);
        object[i]._oAnimLen = AllObjects[ot].oAnimLen;
        object[i]._oAnimFrame = random_(AllObjects[ot].oAnimLen - 1) + 1;
    }
    else
    {
        object[i]._oAnimDelay = 1000;
        object[i]._oAnimCnt = 0;
        object[i]._oAnimLen = AllObjects[ot].oAnimLen;
        object[i]._oAnimFrame = AllObjects[ot].oAnimDelay;
    }
    object[i]._oAnimWidth = AllObjects[ot].oAnimWidth;
    object[i]._oSolidFlag = AllObjects[ot].oSolidFlag;
    object[i]._oMissFlag = AllObjects[ot].oMissFlag;
    object[i]._oLight = AllObjects[ot].oLightFlag;
    object[i]._oDelFlag = FALSE;
    object[i]._oBreak = AllObjects[ot].oBreak;
    object[i]._oSelFlag = AllObjects[ot].oSelFlag;
    object[i]._oPreFlag = FALSE;
    object[i]._oTrapFlag = FALSE;
    object[i]._oDoorFlag = FALSE;
}

// .text:00459F97
// Configure an object to act like a lever (something that changes a rectangle
// of the map when interacted with)
void SetObjMapRange(int i, int x1, int y1, int x2, int y2, int v)
{
    object[i]._oVar1 = x1;
    object[i]._oVar2 = y1;
    object[i]._oVar3 = x2;
    object[i]._oVar4 = y2;
    object[i]._oVar8 = v;
}

// .text:0045A025
// Store a QText message to play upon interacting (in var7). Only ever used here
// but I think it was in the header.
void SetBookMsg(int i, int msg)
{
    object[i]._oVar7 = msg;
}

// .text:0045A055
static void AddL1Door(int i, int x, int y, int ot)
{
    object[i]._oDoorFlag = TRUE;
    if (ot == OBJ_L1LDOOR)
    {
        object[i]._oVar1 = dPiece[x][y];
        object[i]._oVar2 = dPiece[x][y - 1];
    }
    else
    {
        object[i]._oVar1 = dPiece[x][y];
        object[i]._oVar2 = dPiece[x - 1][y];
    }
    object[i]._oVar4 = 0;
}

// .text:0045A15C
static void AddSCambBook(int i)
{
    if (quests[Q_SCHAMB]._qactive == QUEST_INIT)
    {
        return;
    }

    object[i]._oSelFlag = FALSE;
    object[i]._oAnimFrame = 6;
}

// .text:0045A1AF
static void AddChest(int i, int t)
{
    if (random_(2) == 0)
    {
        object[i]._oAnimFrame += 3;
    }

    switch (t)
    {
    case OBJ_CHEST1:
        object[i]._oVar1 = 1;
        break;
    case OBJ_CHEST2:
        object[i]._oVar1 = 2;
        break;
    case OBJ_CHEST3:
        object[i]._oVar1 = 3;
        break;
    }
}

// .text:0045A279
static void AddL2Door(int i, int x, int y, int ot)
{
    object[i]._oDoorFlag = TRUE;
    if (ot == OBJ_L2LDOOR)
        ObjSetMicro(x, y, 538);
    else
        ObjSetMicro(x, y, 540);
    object[i]._oVar4 = 0;
}

// .text:0045A2F3
static void AddSarc(int i)
{
    int x = object[i]._ox;
    int y = object[i]._oy - 1;
    dObject[x][y] = -1 - i;
}

// .text:0045A35A
static void AddFlameTrap(int i)
{
    object[i]._oVar1 = trapid;
    object[i]._oVar2 = 0;
    object[i]._oVar3 = trapdir;
    object[i]._oVar4 = 0;
}

// .text:0045A3D2
static void AddFlameLvr(int i)
{
    object[i]._oVar1 = trapid;
    object[i]._oVar2 = 49;
}

// .text:0045A419
static void AddTrap(int i, int ot)
{
    int rand_max;
    int mt;

    rand_max = currlevel / 3 + 1;
    mt = random_(rand_max);
    if (mt == 0)
        object[i]._oVar3 = MIS_ARROW;
    if (mt == 1)
        object[i]._oVar3 = MIS_FIREBOLT;
    if (mt == 2)
        object[i]._oVar3 = MIS_LIGHTCTRL;

    object[i]._oVar4 = 0;
}

// .text:0045A4CC
static void AddObjLight(int i, int r)
{
    if (InitObjFlag)
    {
        DoLighting(object[i]._ox, object[i]._oy, r, -1);
        object[i]._oVar1 = -1;
    }
    else
    {
        object[i]._oVar1 = 0;
    }
}

// .text:0045A55A
static void AddBarrel(int i, int t)
{
    object[i]._oVar1 = random_(5) + 5;
}

// .text:0045A594
static void AddShrine(int i)
{
    int val;

    object[i]._oPreFlag = TRUE;
    val = random_(NUM_SHRINETYPE);
    object[i]._oVar1 = val;

    if (val == SHRINE_MAGICAL || val == SHRINE_FASCINATING || val == SHRINE_SUPERNATURAL)
    {
        object[i]._oAnimFrame = 12;
        object[i]._oAnimLen = 22;
    }
}

// .text:0045A634
static void AddBookcase(int i)
{
    object[i]._oPreFlag = TRUE;
}

// .text:0045A662
static void AddBloodFtn(int i)
{
    int ox, oy;

    ox = object[i]._ox;
    oy = object[i]._oy;

    // object is so big it occupies more than 1 square
    dObject[ox][oy - 1] = -1 - i;
    dObject[ox - 1][oy] = -1 - i;
    dObject[ox - 1][oy - 1] = -1 - i;
}

// .text:0045A70E
static void AddDecap(int i)
{
    object[i]._oAnimFrame = random_(8) + 1;
    object[i]._oPreFlag = TRUE;
}

// .text:0045A75B
void AddObject(int ot, int ox, int oy)
{
    int oi;

    if (nobjects >= MAXOBJECTS)
        return;

    oi = objectavail[0];
    objectavail[0] = objectavail[MAXOBJECTS - 1 - nobjects];
    objectactive[nobjects] = oi;
    dObject[ox][oy] = oi + 1;
    SetupObject(oi, ox, oy, ot);

    switch (ot)
    {
    case OBJ_L1LIGHT:
    case OBJ_BCROSS:
        AddObjLight(oi, 5);
        break;
    case OBJ_SKFIRE:
    case OBJ_CANDLE2:
    case OBJ_BOOKCANDLE:
        AddObjLight(oi, 5);
        break;
    case OBJ_TORCHL:
    case OBJ_TORCHR:
    case OBJ_TORCHL2:
    case OBJ_TORCHR2:
        AddObjLight(oi, 8);
        break;
    case OBJ_L1LDOOR:
    case OBJ_L1RDOOR:
        AddL1Door(oi, ox, oy, ot);
        break;
    case OBJ_L2LDOOR:
    case OBJ_L2RDOOR:
        AddL2Door(oi, ox, oy, ot);
        break;
    case OBJ_BOOK2R:
        AddSCambBook(oi);
        break;
    case OBJ_CHEST1:
    case OBJ_CHEST2:
    case OBJ_CHEST3:
        AddChest(oi, ot);
        break;
    case OBJ_SARC:
        AddSarc(oi);
        break;
    case OBJ_FLAMEHOLE:
        AddFlameTrap(oi);
        break;
    case OBJ_FLAMELVR:
        AddFlameLvr(oi);
        break;
    case OBJ_WATER:
        object[oi]._oAnimFrame = 1;
        break;
    case OBJ_TRAPL:
    case OBJ_TRAPR:
        AddTrap(oi, ot);
        break;
    case OBJ_BARREL:
    case OBJ_BARRELEX:
        AddBarrel(oi, ot);
        break;
    case OBJ_SHRINEL:
    case OBJ_SHRINER:
        AddShrine(oi);
        break;
    case OBJ_BOOKCASEL:
    case OBJ_BOOKCASER:
        AddBookcase(oi);
        break;
    case OBJ_BLOODFTN:
        AddBloodFtn(oi);
        break;
    case OBJ_DECAP:
        AddDecap(oi);
        break;
    }
    object[oi]._oAnimWidth2 = (object[oi]._oAnimWidth - 64) >> 1;
    nobjects++;
}

// .text:0045A9F3
static void Obj_Light(int i, int lr)
{
    int p;
    int tr;
    BOOL turnon;
    int ox;
    int oy;
    int dx;
    int dy;

    if (object[i]._oVar1 == -1)
    {
        return;
    }

    ox = object[i]._ox;
    oy = object[i]._oy;
    tr = lr + 10;
    turnon = FALSE;

    if (!lightflag)
    {
        for (p = 0; p < gbActivePlayers && !turnon; p++)
        {
            dx = abs(plr[p]._px - ox);
            dy = abs(plr[p]._py - oy);
            if (dx < tr && dy < tr)
            {
                turnon = TRUE;
            }
        }
    }

    if (turnon)
    {
        if (object[i]._oVar1 == 0)
        {
            object[i]._olid = AddLight(ox, oy, lr);
        }
        object[i]._oVar1 = 1;
    }
    else
    {
        if (object[i]._oVar1 == 1)
        {
            AddUnLight(object[i]._olid);
        }
        object[i]._oVar1 = 0;
    }
}

// .text:0045ABC7
// Same as Devilution
static void Obj_StopAnim(int i)
{
    if (object[i]._oAnimFrame == object[i]._oAnimLen)
    {
        object[i]._oAnimCnt = 0;
        object[i]._oAnimDelay = 1000;
    }
}

// Obj_Door    000000000045AC3B
static void Obj_Door(int i)
{
    // TODO
}

// Obj_Sarc    000000000045ADF9
static void Obj_Sarc(int i)
{
    // TODO
}

// TODO: ActivateTrapLine    000000000045AE55

// Obj_FlameTrap    000000000045AF62
static void Obj_FlameTrap(int i)
{
    // TODO
}

// Obj_Trap    000000000045B2C6
static void Obj_Trap(int i)
{
    // TODO
}

// ProcessObjects    000000000045B576
void ProcessObjects()
{
    int oi;
    int i;

    for (i = 0; i < nobjects; ++i)
    {
        oi = objectactive[i];
        switch (object[oi]._otype)
        {
        case OBJ_L1LIGHT:
            Obj_Light(oi, 10);
            break;
        case OBJ_SKFIRE:
        case OBJ_CANDLE2:
        case OBJ_BOOKCANDLE:
            Obj_Light(oi, 5);
            break;
        case OBJ_CRUX1:
        case OBJ_CRUX2:
        case OBJ_CRUX3:
        case OBJ_BARREL:
        case OBJ_BARRELEX:
        case OBJ_SHRINEL:
        case OBJ_SHRINER:
            Obj_StopAnim(oi);
            break;
        case OBJ_L1LDOOR:
        case OBJ_L1RDOOR:
        case OBJ_L2LDOOR:
        case OBJ_L2RDOOR:
            Obj_Door(oi);
            break;
        case OBJ_BCROSS:
            Obj_Light(oi, 10);
            break;
        case OBJ_TORCHL:
        case OBJ_TORCHR:
        case OBJ_TORCHL2:
        case OBJ_TORCHR2:
            Obj_Light(oi, 8);
            break;
        case OBJ_SARC:
            Obj_Sarc(oi);
            break;
        case OBJ_FLAMEHOLE:
            Obj_FlameTrap(oi);
            break;
        case OBJ_TRAPL:
        case OBJ_TRAPR:
            Obj_Trap(oi);
            break;
        }

        if (object[oi]._oAnimFlag == 0)
            continue;

        object[oi]._oAnimCnt++;

        if (object[oi]._oAnimCnt < object[oi]._oAnimDelay)
            continue;

        object[oi]._oAnimCnt = 0;
        object[oi]._oAnimFrame++;
        if (object[oi]._oAnimFrame > object[oi]._oAnimLen)
            object[oi]._oAnimFrame = 1;
    }

    i = 0;
    while (i < nobjects)
    {
        oi = objectactive[i];
        if (object[oi]._oDelFlag)
        {
            DeleteObject_(oi, i);
            i = 0;
        }
        else
        {
            i++;
        }
    }
}

// .text:0045B823
// Change a tile at location (dx, dy) into the given tile.
// Tiles are defined in the .MIN file.
void ObjSetMicro(int dx, int dy, int pn)
{
    int i;
    MICROS *defs;
    WORD *v;

    dPiece[dx][dy] = pn;
    pn--;
    defs = &dpiece_defs_map_1[IsometricCoord(dx, dy)];
    v = (WORD *)pLevelPieces + 10 * pn;

    for (i = 0; i < 10; i++)
    {
        defs->mt[i] = v[(i & 1) - (i & 0xE) + 8];
    }
}

// objects_set_door_piece    000000000045B8C7

// .text:0045B967
// Change a megatile at location (x, y) into the given tile.
// Megatiles are defined in the .TIL file.
// Same as devilution
static void ObjSetMini(int x, int y, int v)
{
    int xx;
    int v1, v2, v3, v4;
    int yy;

    __asm {
        mov        esi, pMegaTiles
        xor        eax, eax
        mov        ax, word ptr v
        dec        eax
        shl        eax, 3
        add        esi, eax
        xor        eax, eax
        lodsw
        inc        eax
        mov        v1, eax
        lodsw
        inc        eax
        mov        v2, eax
        lodsw
        inc        eax
        mov        v3, eax
        lodsw
        inc        eax
        mov        v4, eax
    }

    xx = 2 * x + 16;
    yy = 2 * y + 16;
    ObjSetMicro(xx, yy, v1);
    ObjSetMicro(xx + 1, yy, v2);
    ObjSetMicro(xx, yy + 1, v3);
    ObjSetMicro(xx + 1, yy + 1, v4);
}

// .text:0045BA04
// Same as devilution
static void ObjL1Special(int x1, int y1, int x2, int y2)
{
    int i, j;

    for (i = y1; i <= y2; ++i)
    {
        for (j = x1; j <= x2; ++j)
        {
            dSpecial[j][i] = 0;
            if (dPiece[j][i] == 12)
                dSpecial[j][i] = 1;
            if (dPiece[j][i] == 11)
                dSpecial[j][i] = 2;
            if (dPiece[j][i] == 71)
                dSpecial[j][i] = 1;
            if (dPiece[j][i] == 259)
                dSpecial[j][i] = 5;
            if (dPiece[j][i] == 249)
                dSpecial[j][i] = 2;
            if (dPiece[j][i] == 325)
                dSpecial[j][i] = 2;
            if (dPiece[j][i] == 321)
                dSpecial[j][i] = 1;
            if (dPiece[j][i] == 255)
                dSpecial[j][i] = 4;
            if (dPiece[j][i] == 211)
                dSpecial[j][i] = 1;
            if (dPiece[j][i] == 344)
                dSpecial[j][i] = 2;
            if (dPiece[j][i] == 341)
                dSpecial[j][i] = 1;
            if (dPiece[j][i] == 331)
                dSpecial[j][i] = 2;
            if (dPiece[j][i] == 418)
                dSpecial[j][i] = 1;
            if (dPiece[j][i] == 421)
                dSpecial[j][i] = 2;
        }
    }
}

// .text:0045BD8A
// Same as devilution
static void ObjL2Special(int x1, int y1, int x2, int y2)
{
    int i, j;

    for (j = y1; j <= y2; j++)
    {
        for (i = x1; i <= x2; i++)
        {
            dSpecial[i][j] = 0;
            if (dPiece[i][j] == 541)
                dSpecial[i][j] = 5;
            if (dPiece[i][j] == 178)
                dSpecial[i][j] = 5;
            if (dPiece[i][j] == 551)
                dSpecial[i][j] = 5;
            if (dPiece[i][j] == 542)
                dSpecial[i][j] = 6;
            if (dPiece[i][j] == 553)
                dSpecial[i][j] = 6;
            if (dPiece[i][j] == 13)
                dSpecial[i][j] = 5;
            if (dPiece[i][j] == 17)
                dSpecial[i][j] = 6;
        }
    }
    for (j = y1; j <= y2; j++)
    {
        for (i = x1; i <= x2; i++)
        {
            if (dPiece[i][j] == 132)
            {
                dSpecial[i][j + 1] = 2;
                dSpecial[i][j + 2] = 1;
            }
            if (dPiece[i][j] == 135 || dPiece[i][j] == 139)
            {
                dSpecial[i + 1][j] = 3;
                dSpecial[i + 2][j] = 4;
            }
        }
    }
}

// DoorSet    000000000045C089
// RedoPlayerVision    000000000045C2BD

// .text:0045C349
static void OperateL1RDoor(int pnum, int oi)
{
    // TODO
}

// .text:0045C661
static void OperateL1LDoor(int pnum, int oi)
{
    // TODO
}

// .text:0045C9AB
static void OperateL2RDoor(int pnum, int oi)
{
    // TODO
}

// .text:0045CBE0
static void OperateL2LDoor(int pnum, int oi)
{
    // TODO
}

// MonstCheckDoors    000000000045CE15

// .text:0045D23E
static void ObjChangeMap(int x1, int y1, int x2, int y2)
{
    int i, j;

    for (j = y1; j <= y2; j++)
    {
        for (i = x1; i <= x2; i++)
        {
            ObjSetMini(i, j, pdungeon[i][j]);
            dungeon[i][j] = pdungeon[i][j];
        }
    }

    if (leveltype == DTYPE_CATHEDRAL || leveltype == DTYPE_OLD_CATHEDRAL)
    {
        ObjL1Special(2 * x1 + 16, 2 * y1 + 16, 2 * x2 + 17, 2 * y2 + 17);
        AddL1Objs(2 * x1 + 16, 2 * y1 + 16, 2 * x2 + 17, 2 * y2 + 17);
    }
    if (leveltype == DTYPE_CATACOMBS)
    {
        ObjL2Special(2 * x1 + 16, 2 * y1 + 16, 2 * x2 + 17, 2 * y2 + 17);
        AddL2Objs(2 * x1 + 16, 2 * y1 + 16, 2 * x2 + 17, 2 * y2 + 17);
    }
}

// .text:0045D388
// Practically same as Devilution (missing a param)
void OperateL1Door(int pnum, int i)
{
    int dpx, dpy;

    dpx = abs(object[i]._ox - plr[pnum]._px);
    dpy = abs(object[i]._oy - plr[pnum]._py);
    if (dpx == 1 && dpy <= 1 && object[i]._otype == OBJ_L1LDOOR)
        OperateL1LDoor(pnum, i);
    if (dpx <= 1 && dpy == 1 && object[i]._otype == OBJ_L1RDOOR)
        OperateL1RDoor(pnum, i);
}

// .text:0045D485
// Levers store a rectangle in var1-var4 defining what part of the level to change.
void OperateLever(int i)
{
    PlayRndSFX(49);
    object[i]._oSelFlag = 0;
    object[i]._oAnimFrame++;
    ObjChangeMap(object[i]._oVar1, object[i]._oVar2, object[i]._oVar3, object[i]._oVar4);
}

// .text:0045D525
// Book of Novaness, in the Bone Chamber (dlvl 7).
static void OperateBook(int i, int pnum)
{
    object[i]._oSelFlag = 0;
    object[i]._oAnimFrame++;
    plr[myplr]._pMemSpells |= SPELLBIT(SPL_NOVA);
    quests[Q_SCHAMB]._qactive = QUEST_DONE;
    InitQTextMsg(10);        // "Here is the nova spell huhh..hhuhhhh. words...."
    object[i]._oVar1 = 150;  // ???
    object[i]._oVar2 = pnum; // ???
}

// .text:0045D5C8
// Tome, on dlvl 4 that opens stairs to dlvl 5
// Very similar to OperateLever... There's no sound but text is shown.
// The text shown is stored in var7.
static void OperateBookLever(int i)
{
    object[i]._oSelFlag = 0;
    object[i]._oAnimFrame++;
    ObjChangeMap(object[i]._oVar1, object[i]._oVar2, object[i]._oVar3, object[i]._oVar4);
    InitQTextMsg(object[i]._oVar7);
}

// .text:0045D677
// Mythical tome on dlvl 6. Advances the bone chamber quest (opens way on dlvl 7)
static void OperateSChambBk(int i, int pnum)
{
    object[i]._oSelFlag = 0;
    object[i]._oAnimFrame++;
    quests[Q_SCHAMB]._qactive = QUEST_ACTIVE;
    InitQTextMsg(9);         // "...and so the knowledge was too great. [etc]"
    object[i]._oVar1 = 150;  // ???
    object[i]._oVar2 = pnum; // ???
}

// OperateChest    000000000045D6F9
// Drops var1 number of items (set in AddChest to be 1, 2, or 3).
// Has 1 in 8 chance of dropping a "useful" item
static void OperateChest(int i)
{
    int j;

    PlayRndSFX(14);
    object[i]._oSelFlag = 0;
    object[i]._oAnimFrame += 2;

    for (j = 0; j <= object[i]._oVar1; j++)
    {
        if (random_(8))
        {
            CreateRndItem(object[i]._ox, object[i]._oy);
        }
        else
        {
            CreateRndUseful(object[i]._ox, object[i]._oy);
        }
    }
}

// .text:0045D7E7
static void OperateTrapLvr(int i)
{
    int oi;
    int j;

    if (object[i]._oAnimFrame == 1)
    {
        object[i]._oAnimFrame++;

        for (j = 0; j < nobjects; j++)
        {
            oi = objectactive[j];
            if (object[oi]._otype == object[i]._oVar2 && object[oi]._oVar1 == object[i]._oVar1)
            {
                object[oi]._oVar2 = 1;
                object[oi]._oAnimFlag = 0;
            }
        }
    }
    else
    {
        object[i]._oAnimFrame--;

        for (j = 0; j < nobjects; j++)
        {
            oi = objectactive[j];
            if (object[oi]._otype == object[i]._oVar2 && object[oi]._oVar1 == object[i]._oVar1)
            {
                object[oi]._oVar2 = 0;
                if (object[oi]._oVar4 != 0)
                    object[oi]._oAnimFlag = 1;
            }
        }
    }
}

// OperateSarc    000000000045D9D6
// 40% chance of item, 60% chance of skelton
static void OperateSarc(int i)
{
    int rnd;

    PlayRndSFX(45);

    object[i]._oSelFlag = 0;
    object[i]._oAnimFlag = 1;
    object[i]._oAnimDelay = 3;

    rnd = random_(5);
    if (rnd <= 1)
    {
        CreateRndItem(object[i]._ox, object[i]._oy);
    }
    if (rnd >= 2)
    {
        SpawnSkeleton(object[i]._ox, object[i]._oy);
    }
}

// .text:0045DAB8
// Practically the same as Devilution (different params)
static void OperateL2Door(int pnum, int i)
{
    int dpx, dpy;

    dpx = abs(object[i]._ox - plr[pnum]._px);
    dpy = abs(object[i]._oy - plr[pnum]._py);
    if (dpx == 1 && dpy <= 1 && object[i]._otype == OBJ_L2LDOOR)
        OperateL2LDoor(pnum, i);
    if (dpx <= 1 && dpy == 1 && object[i]._otype == OBJ_L2RDOOR)
        OperateL2RDoor(pnum, i);
}

// .text:0045DBB5
void TryDisarm(int pnum, int i)
{
    SetCursor_(CURSOR_NONE);
    if (object[i]._oTrapFlag == 0)
    {
        return;
    }

    // TODO
}

// .text:0045DD39
void OperateShrine(int pnum, int i)
{
    // These variables are reused in different contexts within the switch.
    // Since they don't have a stable meaning, I've given them generic names.
    int var0, var1, var2, var3, var4;

    PlayRndSFX(43);
    object[i]._oSelFlag = 0;
    object[i]._oAnimFlag = TRUE;
    object[i]._oAnimDelay = 1;

    switch (object[i]._oVar1)
    {
    case SHRINE_MYSTERIOUS:
        var0 = random_(4);
        if (var0 == 0)
            var1 = 5;
        else
            var1 = -1;
        if (var0 == 1)
            var2 = 5;
        else
            var2 = -1;
        if (var0 == 2)
            var3 = 5;
        else
            var3 = -1;
        if (var0 == 3)
            var4 = 5;
        else
            var4 = -1;
        ModifyPlrStr(pnum, var1);
        ModifyPlrMag(pnum, var2);
        ModifyPlrDex(pnum, var3);
        ModifyPlrVit(pnum, var4);
        InitDiabloMsg(EMSG_SHRINE_MYSTERIOUS);
        break;
    case SHRINE_IMPOSING:
        var1 = plr[pnum]._pLevel << 7; // This is effectively 2*clvl but adjusted for HP (which is fixed point with 6 binary decimals)
        plr[pnum]._pHitPoints += var1;
        plr[pnum]._pHPBase += var1;
        if (plr[pnum]._pMaxHP < plr[pnum]._pHitPoints)
        {
            plr[pnum]._pHitPoints = plr[pnum]._pMaxHP;
            plr[pnum]._pHPBase = plr[pnum]._pMaxHPBase;
        }

        var1 = plr[pnum]._pLevel << 7;
        plr[pnum]._pMana -= var1;
        plr[pnum]._pManaBase -= var1;
        if (plr[pnum]._pMana < 0)
        {
            plr[pnum]._pManaBase -= -(0 - plr[pnum]._pMana);
            plr[pnum]._pMana = 0;
        }

        force_redraw = 4;
        InitDiabloMsg(EMSG_SHRINE_IMPOSING);
        break;
    case SHRINE_HIDDEN:
        // Determine how many items are worn (excluding rings)
        var1 = 0;
        if (plr[pnum].InvBody[INVLOC_HEAD]._itype != -1)
            var1 += 1;
        if (plr[pnum].InvBody[INVLOC_CHEST]._itype != -1)
            var1 += 1;
        if (plr[pnum].InvBody[INVLOC_HAND_LEFT]._itype != -1)
            var1 += 1;
        if (plr[pnum].InvBody[INVLOC_HAND_RIGHT]._itype != -1)
            var1 += 1;

        // Only work if wearing at least 2 pieces of equipment
        if (var1 > 1)
        {
            // Randomly destroy a worn item
            var0 = random_(var1) + 1;
            if (plr[pnum].InvBody[INVLOC_HEAD]._itype != -1)
                var0 -= 1;
            if (var0 == 0)
                plr[pnum].InvBody[INVLOC_HEAD]._itype = -1;
            if (plr[pnum].InvBody[INVLOC_CHEST]._itype != -1)
                var0 -= 1;
            if (var0 == 0)
                plr[pnum].InvBody[INVLOC_CHEST]._itype = -1;
            if (plr[pnum].InvBody[INVLOC_HAND_LEFT]._itype != -1)
                var0 -= 1;
            if (var0 == 0)
                plr[pnum].InvBody[INVLOC_HAND_LEFT]._itype = -1;
            if (plr[pnum].InvBody[INVLOC_HAND_RIGHT]._itype != -1)
                var0 -= 1;
            if (var0 == 0)
                plr[pnum].InvBody[INVLOC_HAND_RIGHT]._itype = -1;

            // Non-destroyed items get +10 durability
            if (plr[pnum].InvBody[INVLOC_HEAD]._itype != -1)
            {
                plr[pnum].InvBody[INVLOC_HEAD]._iDurability += 10;
                plr[pnum].InvBody[INVLOC_HEAD]._iMaxDur += 10;
            }
            if (plr[pnum].InvBody[INVLOC_CHEST]._itype != -1)
            {
                plr[pnum].InvBody[INVLOC_CHEST]._iDurability += 10;
                plr[pnum].InvBody[INVLOC_CHEST]._iMaxDur += 10;
            }
            if (plr[pnum].InvBody[INVLOC_HAND_LEFT]._itype != -1)
            {
                plr[pnum].InvBody[INVLOC_HAND_LEFT]._iDurability += 10;
                plr[pnum].InvBody[INVLOC_HAND_LEFT]._iMaxDur += 10;
            }
            if (plr[pnum].InvBody[INVLOC_HAND_RIGHT]._itype != -1)
            {
                plr[pnum].InvBody[INVLOC_HAND_RIGHT]._iDurability += 10;
                plr[pnum].InvBody[INVLOC_HAND_RIGHT]._iMaxDur += 10;
            }

            CalcPlrInv(pnum);
        }

        InitDiabloMsg(EMSG_SHRINE_HIDDEN);
        break;
    case SHRINE_MAGICAL:
        var2 = currlevel << 8; // Effectively 4*dlvl since HP is fixed point with 6 binary decimals
        for (var0 = 0; var0 < nummonsters; ++var0)
        {
            var1 = monstactive[var0];
            monster[var1]._mhitpoints += var2;
            monster[var1]._mmaxhp += var2;
            monster[var1].mArmorClass = 0;
        }

        InitDiabloMsg(EMSG_SHRINE_MAGICAL);
        break;
    case SHRINE_MYSTIC:
        if (plr[pnum]._pGold > 1)
        {
            var1 = currlevel;
            AddPlrExperience(pnum, var1, ((var1 * var1) * 1000)); // This function takes clvl into account to scale XP gains
            TakePlrsMoney(plr[pnum]._pGold - 1);
        }

        InitDiabloMsg(EMSG_SHRINE_MYSTIC);
        break;
    case SHRINE_ENCHANTED:
        var1 = 0;
        var2 = 1;
        for (var0 = 1; var0 <= MAX_SPELLS; var0++)
        {
            if (var2 & plr[pnum]._pMemSpells)
                var1++;
            var2 <<= 1;
        }

        if (var1 > 1)
        {
            // Best way I can describe this algorithm is that it's like a spinning prize wheel of all spells.
            // The wheel starts spinning with a random "speed" (var1 = random(58) + 1)
            // As the wheel passes over a learned spell (spell & plr[pnum]._pMemSpells), it gets "slower" (cnt--)
            // Until it finally "stops" (var1 == 0), at which point `var0` and `var2` refer to a learned spell. That's the spell to forget.
            var1 = random_(MAX_SPELLS * 2) + 1; // count
            var0 = 1;                           // spell id
            var2 = 1;                           // _pMemSpells mask
            while (var1 > 0)
            {
                if (var2 & plr[pnum]._pMemSpells)
                    var1--;
                if (var1 > 0)
                {
                    var0++;
                    if (var0 > MAX_SPELLS)
                    {
                        var0 = 1;
                        var2 = 1;
                    }
                    else
                    {
                        var2 <<= 1;
                    }
                }
            }

            // If the spell to forget is on right-click then unassign it
            if (plr[pnum]._pRSplType == RSPLTYPE_SPELL && plr[pnum]._pRSpell == var0)
            {
                plr[pnum]._pRSpell = -1;
                plr[pnum]._pRSplType = RSPLTYPE_INVALID;
                force_redraw = 4;
            }

            // Forget the randomly chosen spell
            plr[pnum]._pMemSpells ^= var2;
            plr[pnum]._pSplLvl[var0] = 0;

            // Increase non-forgotten spells by 1 level
            var2 = 1;
            for (var0 = 1; var0 <= MAX_SPELLS; var0++)
            {
                if (var2 & plr[pnum]._pMemSpells)
                    plr[pnum]._pSplLvl[var0]++;
                var2 <<= 1;
            }
        }

        InitDiabloMsg(EMSG_SHRINE_ENCHANTED);
        break;
    case SHRINE_THAUMATURGIC:
        for (var0 = 0; var0 < nobjects; ++var0)
        {
            var1 = objectactive[var0];
            if ((object[var1]._otype == OBJ_CHEST1 || object[var1]._otype == OBJ_CHEST2 || object[var1]._otype == OBJ_CHEST3) && object[var1]._oSelFlag == 0)
            {
                object[var1]._oSelFlag = 1;
                object[var1]._oAnimFrame -= 2;
            }
        }

        InitDiabloMsg(EMSG_SHRINE_THAUMATURGIC);
        break;
    case SHRINE_FASCINATING:
        plr[pnum]._pMemSpells |= SPL_FIREBOLT;
        plr[pnum]._pSplLvl[SPL_FIREBOLT] = 5;
        for (var0 = 0; var0 < nummonsters; ++var0)
        {
            var1 = monstactive[i];
            monster[var1].mMagicRes |= IMMUNE_FIRE;
        }

        InitDiabloMsg(EMSG_SHRINE_FASCINATING);
        break;
    case SHRINE_CRYPTIC:
        var0 = random_(4) + 4;
        while (var0 > 0)
        {
            do
            {
                var1 = random_(4);
                var2 = random_(4);
            } while (var1 == var2);

            if (var1 == 0)
                var3 = plr[pnum]._pBaseStr;
            if (var1 == 1)
                var3 = plr[pnum]._pBaseMag;
            if (var1 == 2)
                var3 = plr[pnum]._pBaseDex;
            if (var1 == 3)
                var3 = plr[pnum]._pBaseVit;

            if (var2 == 0)
            {
                var4 = plr[pnum]._pBaseStr;
                ModifyPlrStr(pnum, var3 - var4);
            }
            if (var2 == 1)
            {
                var4 = plr[pnum]._pBaseMag;
                ModifyPlrMag(pnum, var3 - var4);
            }
            if (var2 == 2)
            {
                var4 = plr[pnum]._pBaseDex;
                ModifyPlrDex(pnum, var3 - var4);
            }
            if (var2 == 3)
            {
                var4 = plr[pnum]._pBaseVit;
                ModifyPlrVit(pnum, var3 - var4);
            }

            if (var1 == 0)
                ModifyPlrStr(pnum, var4 - var3);
            if (var1 == 1)
                ModifyPlrMag(pnum, var4 - var3);
            if (var1 == 2)
                ModifyPlrDex(pnum, var4 - var3);
            if (var1 == 3)
                ModifyPlrVit(pnum, var4 - var3);

            var0--;
        }

        var1 = 20 << 6;
        plr[pnum]._pMaxHP += var1;
        plr[pnum]._pHitPoints += var1;
        plr[pnum]._pMaxHPBase += var1;
        plr[pnum]._pHPBase += var1;

        var2 = 20 << 6;
        plr[pnum]._pMaxMana += var2;
        plr[pnum]._pMana += var2;
        plr[pnum]._pMaxManaBase += var2;
        plr[pnum]._pManaBase += var2;

        force_redraw = 4;
        InitDiabloMsg(EMSG_SHRINE_CRYPTIC);
        break;
    case SHRINE_SUPERNATURAL:
        for (var0 = 0; var0 < nummonsters; ++var0)
        {
            var1 = monstactive[var0];
            monster[var1].mHit = 100;
            monster[var1].mHit2 = 100;
            monster[var1].mMinDamage /= 2;
            monster[var1].mMaxDamage /= 2;
            monster[var1].mMinDamage2 /= 2;
            monster[var1].mMaxDamage2 /= 2;
        }

        InitDiabloMsg(EMSG_SHRINE_SUPERNATURAL);
        break;
    case SHRINE_EERIE:
        var1 = strlen(plr[pnum]._pName);
        for (var0 = 0; var0 < var1; var0++)
        {
            tempstr[var0] = plr[pnum]._pName[var1 - var0 - 1];
        }
        tempstr[var0] = '\0';
        strcpy(plr[pnum]._pName, tempstr);

        ModifyPlrMag(pnum, 2);

        InitDiabloMsg(EMSG_SHRINE_EERIE);
        break;
    case SHRINE_HOLY:
        do
        {
            var1 = random_(MAXDUNX);
            var2 = random_(MAXDUNY);
            var3 = dPiece[var1][var2];
        } while (nSolidTable[var3] || dObject[var1][var2] || dMonster[var1][var2]);

        AddMissile(plr[pnum]._px, plr[pnum]._py, var1, var2, plr[pnum]._pdir, MIS_RNDTELEPORT, 0, pnum, 0);
        break;
    case SHRINE_SPIRITUAL:
        for (var0 = 0; var0 < NUM_INV_GRID_ELEM; var0++)
        {
            if (!plr[pnum].InvGrid[var0])
            {
                var2 = currlevel + random_(10 * currlevel);
                var1 = plr[pnum]._pNumInv;
                plr[pnum].InvList[var1] = golditem;
                plr[pnum]._pNumInv++;
                plr[pnum].InvGrid[var0] = plr[pnum]._pNumInv;
                plr[pnum].InvList[var1]._ivalue = var2;
                plr[pnum]._pGold += var2;
                SetGoldCurs(var1);
            }
        }

        InitDiabloMsg(EMSG_SHRINE_SPIRITUAL);
        break;
    }
}

// .text:0045F05C
// 20% chance of book, 80% change of scroll
static void OperateSkelBook(int i)
{
    PlayRndSFX(39);
    object[i]._oSelFlag = 0;
    object[i]._oAnimFrame += 2;
    if (random_(5))
    {
        CreateTypeItem(object[i]._ox, object[i]._oy, FALSE, ITYPE_MISC, IMISC_SCROLL);
    }
    else
    {
        CreateTypeItem(object[i]._ox, object[i]._oy, FALSE, ITYPE_MISC, IMISC_BOOK);
    }
}

// .text:0045F123
static void OperateBookCase(int i)
{
    PlayRndSFX(39);
    object[i]._oSelFlag = 0;
    object[i]._oAnimFrame -= 2;
    CreateTypeItem(object[i]._ox, object[i]._oy, FALSE, ITYPE_MISC, IMISC_BOOK);
}

// OperateFountains    000000000045F1A0
static void OperateFountains(int pnum, int i)
{
    if (plr[pnum]._pMana > (8 << 6) && plr[pnum]._pHitPoints != plr[pnum]._pMaxHP)
    {
        PlayRndSFX(43);
        plr[pnum]._pMana -= (8 << 6);
        plr[pnum]._pManaBase -= (8 << 6);
        plr[pnum]._pHitPoints += (8 << 6);
        plr[pnum]._pHPBase += (8 << 6);

        if (plr[pnum]._pHitPoints > plr[pnum]._pMaxHP)
        {
            plr[pnum]._pHitPoints = plr[pnum]._pMaxHP;
            plr[pnum]._pHPBase = plr[pnum]._pMaxHPBase;
            force_redraw = 4;
        }
    }
    else
    {
        PlayRndSFX(37);
    }
}

// .text:0045F361
static void OperateDecap(int i)
{
    object[i]._oSelFlag = 0;
    CreateRndItem(object[i]._ox, object[i]._oy);
}

// .text:0045F3BB
void OperateObject(int pnum, int i)
{
    if (pcurs == CURSOR_DISARM)
    {
        TryDisarm(pnum, i);
    }

    switch (object[i]._otype)
    {
    case OBJ_L1LDOOR:
    case OBJ_L1RDOOR:
        OperateL1Door(pnum, i);
        break;
    case OBJ_L2LDOOR:
    case OBJ_L2RDOOR:
        OperateL2Door(pnum, i);
        break;
    case OBJ_LEVER:
    case OBJ_SWITCHSKL:
        OperateLever(i);
        break;
    case OBJ_BOOK2L:
        // Book of Novaness
        OperateBook(i, pnum);
        break;
    case OBJ_BOOK2R:
        // Mythical tome, opens bone chamber
        OperateSChambBk(i, pnum);
        break;
    case OBJ_CHEST1:
    case OBJ_CHEST2:
    case OBJ_CHEST3:
        OperateChest(i);
        break;
    case OBJ_SARC:
        OperateSarc(i);
        break;
    case OBJ_FLAMELVR:
        OperateTrapLvr(i);
        break;
    case OBJ_BOOKLVR:
        // Tome on dlvl 4, opens stairs to dlvl 5
        OperateBookLever(i);
        break;
    case OBJ_SHRINEL:
    case OBJ_SHRINER:
        OperateShrine(pnum, i);
        break;
    case OBJ_SKELBOOK:
    case OBJ_BOOKSTAND:
        OperateSkelBook(i);
        break;
    case OBJ_BOOKCASEL:
    case OBJ_BOOKCASER:
        OperateBookCase(i);
        break;
    case OBJ_BLOODFTN:
        OperateFountains(pnum, i);
        break;
    case OBJ_DECAP:
        OperateDecap(i);
        break;
    }
    // TODO
}

// .text:0045F57B
// Crucifix acts like a breakable lever. ALl must be broken to trigger
void BreakCrux(int i)
{
    int otype;
    int j, oi;
    BOOL triggered;

    object[i]._oAnimFlag = 1;
    object[i]._oAnimFrame = 1;
    object[i]._oAnimDelay = 1;
    object[i]._oSolidFlag = TRUE;
    object[i]._oMissFlag = TRUE;
    object[i]._oBreak = -1;
    object[i]._oSelFlag = 0;
    triggered = TRUE;

    for (j = 0; j < nobjects; j++)
    {
        oi = objectactive[j];
        otype = object[oi]._otype;
        if (otype == OBJ_CRUX1 || otype == OBJ_CRUX2 || otype == OBJ_CRUX3)
        {
            if (object[i]._oVar8 == object[oi]._oVar8 || object[oi]._oBreak != -1)
            {
                triggered = FALSE;
            }
        }
    }

    if (triggered)
    {
        ObjChangeMap(object[i]._oVar1, object[i]._oVar2, object[i]._oVar3, object[i]._oVar4);
    }
}

// BreakBarrel    000000000045F752
// BreakObject    000000000045FAFA
// SyncL1Doors    000000000045FC6F
// SyncCrux    000000000045FDED
// SyncLever    000000000045FF22
// SyncL2Doors    000000000045FFAC
// SyncObjectAnim    0000000000460094

// .text:004601E1
void GetObjectStr(int i)
{
    switch (object[i]._otype)
    {
    case OBJ_CRUX1:
    case OBJ_CRUX2:
    case OBJ_CRUX3:
        strcpy(infostr, "Crucified Skeleton");
        break;
    case OBJ_LEVER:
    case OBJ_FLAMELVR:
        strcpy(infostr, "Lever");
        break;
    case OBJ_L1LDOOR:
    case OBJ_L1RDOOR:
    case OBJ_L2LDOOR:
    case OBJ_L2RDOOR:
        if (object[i]._oVar4 == 1)
        {
            strcpy(infostr, "Open Door");
        }
        if (object[i]._oVar4 == 0)
        {
            strcpy(infostr, "Closed Door");
        }
        if (object[i]._oVar4 == 2)
        {
            strcpy(infostr, "Blocked Door");
        }
        break;
    case OBJ_BOOK2L:
        strcpy(infostr, "Book of Novaness");
        break;
    case OBJ_SWITCHSKL:
        strcpy(infostr, "Skull Lever");
        break;
    case OBJ_BOOKLVR:
        strcpy(infostr, "Tome");
        break;
    case OBJ_BOOK2R:
        strcpy(infostr, "Mythical Book");
        break;
    case OBJ_CHEST1:
        strcpy(infostr, "Small Chest");
        break;
    case OBJ_CHEST2:
        strcpy(infostr, "Chest");
        break;
    case OBJ_CHEST3:
        strcpy(infostr, "Large Chest");
        break;
    case OBJ_SARC:
        strcpy(infostr, "Sarcophagus");
        break;
    case OBJ_BOOKSHELF:
        strcpy(infostr, "Bookshelf");
        break;
    case OBJ_BOOKCASEL:
    case OBJ_BOOKCASER:
        strcpy(infostr, "Bookcase");
        break;
    case OBJ_BARREL:
    case OBJ_BARRELEX:
        strcpy(infostr, "Barrel");
        break;
    case OBJ_SHRINEL:
    case OBJ_SHRINER:
        sprintf(tempstr, "%s Shrine", shrinestrs[object[i]._oVar1]);
        strcpy(infostr, tempstr);
        break;
    case OBJ_SKELBOOK:
        strcpy(infostr, "Skeleton Tome");
        break;
    case OBJ_BOOKSTAND:
        strcpy(infostr, "Library Book");
        break;
    case OBJ_BLOODFTN:
        strcpy(infostr, "Blood Fountain");
        break;
    case OBJ_DECAP:
        strcpy(infostr, "Decapitated Body");
        break;
    }

    if (plr[myplr]._pClass == PC_ROGUE)
    {
        if (object[i]._oTrapFlag)
        {
            sprintf(tempstr, "Trapped %s", infostr);
            strcpy(infostr, tempstr);
            infoclr = COL_RED;
        }
    }
}