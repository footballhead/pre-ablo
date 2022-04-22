#include "defines.h"
#include "enums.h"
#include "player.h"
#include "structs.h"

//
// extern
//

extern int force_redraw; // interfac.cpp?
extern int nummonsters; // monster.cpp?
extern int monstactive[MAXMONSTERS]; // monster.cpp?
extern MonsterStruct monster[MAXMONSTERS]; // monster.cpp?
extern int currlevel;
extern char dObject[MAXDUNX][MAXDUNY]; // gendung.cpp?
extern char dPiece[MAXDUNX][MAXDUNY]; // gendung.cpp?
extern char dMonster[MAXDUNX][MAXDUNY]; // gendung.cpp?
extern ObjDataStruct AllObjects[69]; // objdat.cpp
extern QuestStruct quests[MAXQUESTS];
extern char tempstr[64]; // interfac.cpp ???
extern int nSolidTable[MAXTILES + 1];
extern ItemStruct golditem;

void PlayRndSFX(int psfx); // effects.cpp
int random_(int v); // engine.cpp
void ModifyPlrStr(int p, int l); // player.cpp
void ModifyPlrMag(int p, int l); // player.cpp
void ModifyPlrDex(int p, int l); // player.cpp
void ModifyPlrVit(int p, int l); // player.cpp
void InitDiabloMsg(char e); // error.cpp
void CalcPlrInv(int pnum); // items.cpp
void AddPlrExperience(int pnum, int lvl, int exp); // player.cpp
void TakePlrsMoney(int cost); // stores.cpp
void DoLighting(int nXPos, int nYPos, int nRadius, int Lnum);
int AddMissile(int sx, int sy, int dx_, int dy, int midir, int mitype, int micaster, int id, int midam);
void SetGoldCurs(int i);

//
// initialized data (.data:004B8A50)
//

// alignment at .data:004B8A4B ??? is this actually an unused DWORD?

int bxadd[] = { -1, 0, 1, -1, 1, -1, 0, 1 };
int byadd[] = { -1, -1, -1, 0, 0, 1, 1, 1 };

const char* shrinestrs[] = {
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
    "Spiritual"
};

//
// unintialized data (.data:00600078)
//

int trapdir;
int nobjects;
int numobjfiles;
DWORD unused000;
char ObjFileList[40]; // indexed by numobjfiles
int objectavail[MAXOBJECTS]; // indexed by nobjects
DWORD unused001;
ObjectStruct object[MAXOBJECTS];
BOOL InitObjFlag;
int leverid;
DWORD unused002;
int objectactive[MAXOBJECTS];
DWORD unused003;
BYTE *pObjCels[40];
int trapid;
DWORD unused004; // could be part of loadsave.cpp

//
// code (.text:00457E30)
//

// InitObjectGFX    0000000000457E30    
// FreeObjectGFX    0000000000458029    
// RndLocOk    000000000045808F    
// __dc_TrapLocOk    00000000004581D8    
// __dc_RoomLocOk    000000000045824E    
// InitRndLocObj    0000000000458312    
// InitRndLocBigObj    0000000000458438    
// ClrAllObjects    0000000000458593    
// AddTortures    0000000000458745    
// __dc_AddTrapLine    00000000004588B3    
// __dc_AddLeverObj    0000000000458C09    
// AddMazeBook    0000000000458D47    
// InitRndBarrels    0000000000458E90    
// AddL1Objs    0000000000459015    
// AddL2Objs    00000000004590FA    
// WallTrapLocOk    00000000004591DF    
// AddObjTraps    0000000000459255    
// InitObjects    000000000045951F    
// SetMapObjects    0000000000459817    
// DeleteObject    0000000000459B79    

// .text:00459C24
void SetupObject(int i, int x, int y, int ot)
{
    int ofi;
    int j;

    object[i]._otype = ot;
    object[i]._ox = x;
    object[i]._oy = y;

    ofi = AllObjects[ot].ofindex;
    for (j = 0; ObjFileList[j] != ofi; j++) {}

    object[i]._oAnimData = pObjCels[j];
    object[i]._oAnimFlag = AllObjects[ot].oAnimFlag;
    if (AllObjects[ot].oAnimFlag) {
        object[i]._oAnimDelay = AllObjects[ot].oAnimDelay;
        object[i]._oAnimCnt = random_(AllObjects[ot].oAnimDelay);
        object[i]._oAnimLen = AllObjects[ot].oAnimLen;
        object[i]._oAnimFrame = random_(AllObjects[ot].oAnimLen - 1) + 1;
    } else {
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

// SetObjMapRange    0000000000459F97    
// SetBookMsg    000000000045A025    
// .text:0045A055
void AddL1Door(int i, int x, int y, int ot)
{
    object[i]._oDoorFlag = TRUE;
    if (ot == OBJ_L1LDOOR) {
        object[i]._oVar1 = dPiece[x][y];
        object[i]._oVar2 = dPiece[x][y - 1];
    } else {
        object[i]._oVar1 = dPiece[x][y];
        object[i]._oVar2 = dPiece[x - 1][y];
    }
    object[i]._oVar4 = 0;
}

// .text:0045A15C
void AddSCambBook(int i)
{
    if (quests[Q_SCHAMB]._qactive == QUEST_INIT) {
        return;
    }

    object[i]._oSelFlag = FALSE;
    object[i]._oAnimFrame = 6;
}

// .text:0045A1AF
void AddChest(int i, int t)
{
    if (random_(2) == 0) {
        object[i]._oAnimFrame += 3;
    }

    switch (t) {
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
void AddL2Door(int i, int x, int y, int ot)
{
    object[i]._oDoorFlag = TRUE;
    if (ot == OBJ_L2LDOOR)
        ObjSetMicro(x, y, 538);
    else
        ObjSetMicro(x, y, 540);
    object[i]._oVar4 = 0;
}

// .text:0045A2F3
void AddSarc(int i)
{
    int x = object[i]._ox;
    int y = object[i]._oy - 1;
    dObject[x][y] = -1 - i;
}

// .text:0045A35A
void AddFlameTrap(int i)
{
    object[i]._oVar1 = trapid;
    object[i]._oVar2 = 0;
    object[i]._oVar3 = trapdir;
    object[i]._oVar4 = 0;
}

// .text:0045A3D2
void AddFlameLvr(int i)
{
    object[i]._oVar1 = trapid;
    object[i]._oVar2 = 49; // MIS_FLAMEC
}

// .text:0045A419
void AddTrap(int i, int ot)
{
    int rand_max;
    int mt;

    rand_max = currlevel / 3 + 1;
    mt = random_(rand_max);
    if (mt == 0)
        object[i]._oVar3 = 0; //MIS_ARROW;
    if (mt == 1)
        object[i]._oVar3 = 1; // MIS_FIREBOLT;
    if (mt == 2)
        object[i]._oVar3 = 7; //MIS_LIGHTCTRL;

    object[i]._oVar4 = 0;
}

// .text:0045A4CC
void AddObjLight(int i, int r)
{
    if (InitObjFlag) {
        DoLighting(object[i]._ox, object[i]._oy, r, -1);
        object[i]._oVar1 = -1;
    } else {
        object[i]._oVar1 = 0;
    }
}

// .text:0045A55A
void AddBarrel(int i, int t)
{
    object[i]._oVar1 = random_(5) + 5;
}

// .text:0045A594
void AddShrine(int i)
{
    int val;

    object[i]._oPreFlag = TRUE;
    val = random_(NUM_SHRINETYPE);
    object[i]._oVar1 = val;

    if (val == SHRINE_MAGICAL || val == SHRINE_FASCINATING || val == SHRINE_SUPERNATURAL) {
        object[i]._oAnimFrame = 12;
        object[i]._oAnimLen = 22;
    }
}

// .text:0045A634
void AddBookcase(int i)
{
    object[i]._oPreFlag = TRUE;
}

// .text:0045A662    
void AddBloodFtn(int i) {
    int ox, oy;

    ox = object[i]._ox;
    oy = object[i]._oy;

    // object is so big it occupies more than 1 square
    dObject[ox][oy - 1] = -1 - i;
    dObject[ox - 1][oy] = -1 - i;
    dObject[ox - 1][oy - 1] = -1 - i;
}

// .text:0045A70E
void AddDecap(int i)
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

    switch (ot) {
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

// Obj_Light    000000000045A9F3    
// Obj_StopAnim    000000000045ABC7    
// Obj_Door    000000000045AC3B    
// Obj_Sarc    000000000045ADF9    
// ActivateTrapLine    000000000045AE55    
// Obj_FlameTrap    000000000045AF62    
// Obj_Trap    000000000045B2C6    
// ProcessObjects    000000000045B576    

// ObjSetMicro    000000000045B823    
void ObjSetMicro(int dx, int dy, int pn);

// objects_set_door_piece    000000000045B8C7    
// ObjSetMini    000000000045B967    
// ObjL1Special    000000000045BA04    
// ObjL2Special    000000000045BD8A    
// DoorSet    000000000045C089    
// RedoPlayerVision    000000000045C2BD    
// OperateL1RDoor    000000000045C349    
// OperateL1LDoor    000000000045C661    
// OperateL2RDoor    000000000045C9AB    
// OperateL2LDoor    000000000045CBE0    
// MonstCheckDoors    000000000045CE15    
// ObjChangeMap    000000000045D23E    
// OperateL1Door    000000000045D388    
// OperateLever    000000000045D485    
// OperateBook    000000000045D525    
// OperateBookLever    000000000045D5C8    
// OperateSChambBk    000000000045D677    
// OperateChest    000000000045D6F9    
// OperateTrapLvr    000000000045D7E7    
// OperateSarc    000000000045D9D6    
// OperateL2Door    000000000045DAB8    
// TryDisarm    000000000045DBB5    

// .text:0045DD39
void OperateShrine(int pnum, int i) {
    // These variables are reused in different contexts within the switch.
    // Since they don't have a stable meaning, I've given them generic names.
    int var0, var1, var2, var3, var4;

    PlayRndSFX(43);
    object[i]._oSelFlag = 0;
    object[i]._oAnimFlag = TRUE;
    object[i]._oAnimDelay = 1;

    switch (object[i]._oVar1) {
    case SHRINE_MYSTERIOUS:
        var0 = random_(4);
        if (var0 == 0) var1 = 5; else var1 = -1;
        if (var0 == 1) var2 = 5; else var2 = -1;
        if (var0 == 2) var3 = 5; else var3 = -1;
        if (var0 == 3) var4 = 5; else var4 = -1;
        ModifyPlrStr(pnum, var1);
        ModifyPlrMag(pnum, var2);
        ModifyPlrDex(pnum, var3);
        ModifyPlrVit(pnum, var4);
        InitDiabloMsg(8); // "Odd sensations..."
        break;
    case SHRINE_IMPOSING:
        var1 = plr[pnum]._pLevel << 7; // This is effectively 2*clvl but adjusted for HP (which is fixed point with 6 binary decimals)
        plr[pnum]._pHitPoints += var1;
        plr[pnum]._pHPBase += var1;
        if (plr[pnum]._pMaxHP < plr[pnum]._pHitPoints) {
            plr[pnum]._pHitPoints = plr[pnum]._pMaxHP;
            plr[pnum]._pHPBase = plr[pnum]._pMaxHPBase;
        }

        var1 = plr[pnum]._pLevel << 7;
        plr[pnum]._pMana -= var1;
        plr[pnum]._pManaBase -= var1;
        if (plr[pnum]._pMana < 0) {
            plr[pnum]._pManaBase -= -(0 - plr[pnum]._pMana);
            plr[pnum]._pMana = 0;
        }

        force_redraw = 4;
        InitDiabloMsg(9); // "A surge of blood interrupts your thoughts"
        break;
    case SHRINE_HIDDEN:
        // Determine how many items are worn (excluding rings)
        var1 = 0;
        if (plr[pnum].InvBody[INVLOC_HEAD]._itype != -1) var1 += 1;
        if (plr[pnum].InvBody[INVLOC_CHEST]._itype != -1) var1 += 1;
        if (plr[pnum].InvBody[INVLOC_HAND_LEFT]._itype != -1) var1 += 1;
        if (plr[pnum].InvBody[INVLOC_HAND_RIGHT]._itype != -1) var1 += 1;

        // Only work if wearing at least 2 pieces of equipment
        if (var1 > 1) {
            // Randomly destroy a worn item
            var0 = random_(var1) + 1;
            if (plr[pnum].InvBody[INVLOC_HEAD]._itype != -1) var0 -= 1;
            if (var0 == 0) plr[pnum].InvBody[INVLOC_HEAD]._itype = -1;
            if (plr[pnum].InvBody[INVLOC_CHEST]._itype != -1) var0 -= 1;
            if (var0 == 0) plr[pnum].InvBody[INVLOC_CHEST]._itype = -1;
            if (plr[pnum].InvBody[INVLOC_HAND_LEFT]._itype != -1) var0 -= 1;
            if (var0 == 0) plr[pnum].InvBody[INVLOC_HAND_LEFT]._itype = -1;
            if (plr[pnum].InvBody[INVLOC_HAND_RIGHT]._itype != -1) var0 -= 1;
            if (var0 == 0) plr[pnum].InvBody[INVLOC_HAND_RIGHT]._itype = -1;

            // Non-destroyed items get +10 durability
            if (plr[pnum].InvBody[INVLOC_HEAD]._itype != -1) {
                plr[pnum].InvBody[INVLOC_HEAD]._iDurability += 10;
                plr[pnum].InvBody[INVLOC_HEAD]._iMaxDur += 10;
            }
            if (plr[pnum].InvBody[INVLOC_CHEST]._itype != -1) {
                plr[pnum].InvBody[INVLOC_CHEST]._iDurability += 10;
                plr[pnum].InvBody[INVLOC_CHEST]._iMaxDur += 10;
            }
            if (plr[pnum].InvBody[INVLOC_HAND_LEFT]._itype != -1) {
                plr[pnum].InvBody[INVLOC_HAND_LEFT]._iDurability += 10;
                plr[pnum].InvBody[INVLOC_HAND_LEFT]._iMaxDur += 10;
            }
            if (plr[pnum].InvBody[INVLOC_HAND_RIGHT]._itype != -1) {
                plr[pnum].InvBody[INVLOC_HAND_RIGHT]._iDurability += 10;
                plr[pnum].InvBody[INVLOC_HAND_RIGHT]._iMaxDur += 10;
            }

            CalcPlrInv(pnum);
        }

        InitDiabloMsg(10); // "Energy passes through your equipment..."
        break;
    case SHRINE_MAGICAL:
        var2 = currlevel << 8; // Effectively 4*dlvl since HP is fixed point with 6 binary decimals
        for (var0 = 0; var0 < nummonsters; ++var0) {
            var1 = monstactive[var0];
            monster[var1]._mhitpoints += var2;
            monster[var1]._mmaxhp += var2;
            monster[var1].mArmorClass = 0;
        }

        InitDiabloMsg(11); // "Growling is heard throughout the dungeon"
        break;
    case SHRINE_MYSTIC:
        if (plr[pnum]._pGold > 1) {
            var1 = currlevel;
            AddPlrExperience(pnum, var1, ((var1*var1) * 1000)); // This function takes clvl into account to scale XP gains
            TakePlrsMoney(plr[pnum]._pGold - 1);
        }

        InitDiabloMsg(12); // "Your skills increase, but at a price..."
        break;
    case SHRINE_ENCHANTED:
        var1 = 0;
        var2 = 1;
        for (var0 = 1; var0 <= MAX_SPELLS; var0++) {
            if (var2 & plr[pnum]._pMemSpells)
                var1++;
            var2 <<= 1;
        }

        if (var1 > 1) {
            // Best way I can describe this algorithm is that it's like a spinning prize wheel of all spells.
            // The wheel starts spinning with a random "speed" (var1 = random(58) + 1)
            // As the wheel passes over a learned spell (spell & plr[pnum]._pMemSpells), it gets "slower" (cnt--)
            // Until it finally "stops" (var1 == 0), at which point `var0` and `var2` refer to a learned spell. That's the spell to forget.
            var1 = random_(MAX_SPELLS * 2) + 1; // count
            var0 = 1; // spell id
            var2 = 1; // _pMemSpells mask
            while (var1 > 0) {
                if (var2 & plr[pnum]._pMemSpells)
                    var1--;
                if (var1 > 0) {
                    var0++;
                    if (var0 > MAX_SPELLS) {
                        var0 = 1;
                        var2 = 1;
                    } else {
                        var2 <<= 1;
                    }
                }
            }

            // If the spell to forget is on right-click then unassign it
            if (plr[pnum]._pRSplType == RSPLTYPE_SPELL && plr[pnum]._pRSpell == var0) {
                plr[pnum]._pRSpell = -1;
                plr[pnum]._pRSplType = RSPLTYPE_INVALID;
                force_redraw = 4;
            }

            // Forget the randomly chosen spell
            plr[pnum]._pMemSpells ^= var2;
            plr[pnum]._pSplLvl[var0] = 0;

            // Increase non-forgotten spells by 1 level
            var2 = 1;
            for (var0 = 1; var0 <= MAX_SPELLS; var0++) {
                if (var2 & plr[pnum]._pMemSpells)
                    plr[pnum]._pSplLvl[var0]++;
                var2 <<= 1;
            }
        }

        InitDiabloMsg(13); // "Did you forget something?""
        break;
    case SHRINE_THAUMATURGIC:
        for (var0 = 0; var0 < nobjects; ++var0) {
            var1 = objectactive[var0];
            if ((object[var1]._otype == OBJ_CHEST1 || object[var1]._otype == OBJ_CHEST2 || object[var1]._otype == OBJ_CHEST3) && object[var1]._oSelFlag == 0) {
                object[var1]._oSelFlag = 1;
                object[var1]._oAnimFrame -= 2;
            }
        }

        InitDiabloMsg(14); // "You hear a series of creaks and thumps..."
        break;
    case SHRINE_FASCINATING:
        plr[pnum]._pMemSpells |= SPL_FIREBOLT;
        plr[pnum]._pSplLvl[SPL_FIREBOLT] = 5;
        for (var0 = 0; var0 < nummonsters; ++var0) {
            var1 = monstactive[i];
            monster[var1].mMagicRes |= IMMUNE_FIRE;
        }

        InitDiabloMsg(15); // "You are the powerless master of fire!"
        break;
    case SHRINE_CRYPTIC:
        var0 = random_(4) + 4;
        while (var0 > 0) {
            do {
                var1 = random_(4);
                var2 = random_(4);
            } while (var1 == var2);

            if (var1 == 0) var3 = plr[pnum]._pBaseStr;
            if (var1 == 1) var3 = plr[pnum]._pBaseMag;
            if (var1 == 2) var3 = plr[pnum]._pBaseDex;
            if (var1 == 3) var3 = plr[pnum]._pBaseVit;

            if (var2 == 0) {
                var4 = plr[pnum]._pBaseStr;
                ModifyPlrStr(pnum, var3 - var4);
            }
            if (var2 == 1) {
                var4 = plr[pnum]._pBaseMag;
                ModifyPlrMag(pnum, var3 - var4);
            }
            if (var2 == 2) {
                var4 = plr[pnum]._pBaseDex;
                ModifyPlrDex(pnum, var3 - var4);
            }
            if (var2 == 3) {
                var4 = plr[pnum]._pBaseVit;
                ModifyPlrVit(pnum, var3 - var4);
            }

            if (var1 == 0) ModifyPlrStr(pnum, var4 - var3);
            if (var1 == 1) ModifyPlrMag(pnum, var4 - var3);
            if (var1 == 2) ModifyPlrDex(pnum, var4 - var3);
            if (var1 == 3) ModifyPlrVit(pnum, var4 - var3);

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
        InitDiabloMsg(16); // "Power comes from your disorientation..."
        break;
    case SHRINE_SUPERNATURAL:
        for (var0 = 0; var0 < nummonsters; ++var0) {
            var1 = monstactive[var0];
            monster[var1].mHit = 100;
            monster[var1].mHit2 = 100;
            monster[var1].mMinDamage /= 2;
            monster[var1].mMaxDamage /= 2;
            monster[var1].mMinDamage2 /= 2;
            monster[var1].mMaxDamage2 /= 2;
        }

        InitDiabloMsg(17); // "You hear a strange cry from the distance"
        break;
    case SHRINE_EERIE:
        var1 = strlen(plr[pnum]._pName);
        for (var0 = 0; var0 < var1; var0++) {
            tempstr[var0] = plr[pnum]._pName[var1 - var0 - 1];
        }
        tempstr[var0] = '\0';
        strcpy(plr[pnum]._pName, tempstr);

        ModifyPlrMag(pnum, 2);

        InitDiabloMsg(18); // "You forget who you are!"
        break;
    case SHRINE_HOLY:
        do {
            var1 = random_(MAXDUNX);
            var2 = random_(MAXDUNY);
            var3 = dPiece[var1][var2];
        } while(nSolidTable[var3] || dObject[var1][var2] || dMonster[var1][var2]);

        // 3 == MIS_RNDTELEPORT
        AddMissile(plr[pnum]._px, plr[pnum]._py, var1, var2, plr[pnum]._pdir, 3, 0, pnum, 0);
        break;
    case SHRINE_SPIRITUAL:
        for (var0 = 0; var0 < NUM_INV_GRID_ELEM; var0++) {
            if (!plr[pnum].InvGrid[var0]) {
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

        InitDiabloMsg(19); // "Untold Wealth!"
        break;
    }
}

// OperateSkelBook    000000000045F05C    
// OperateBookCase    000000000045F123    
// OperateFountains    000000000045F1A0    
// OperateDecap    000000000045F361    
// OperateObject    000000000045F3BB    
// BreakCrux    000000000045F57B    
// BreakBarrel    000000000045F752    
// BreakObject    000000000045FAFA    
// SyncL1Doors    000000000045FC6F    
// SyncCrux    000000000045FDED    
// SyncLever    000000000045FF22    
// SyncL2Doors    000000000045FFAC    
// SyncObjectAnim    0000000000460094    
// GetObjectStr    00000000004601E1    
