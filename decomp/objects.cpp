#include "defines.h"
#include "enums.h"
#include "structs.h"

//
// extern
//

extern ObjectStruct object[127]; // objects.cpp?
extern PlayerStruct plr[MAX_PLRS]; // player.cpp?
extern int force_redraw; // interfac.cpp?

void PlayRndSFX(int psfx); // effects.cpp
int random_(int v); // engine.cpp
void ModifyPlrStr(int p, int l); // player.cpp
void ModifyPlrMag(int p, int l); // player.cpp
void ModifyPlrDex(int p, int l); // player.cpp
void ModifyPlrVit(int p, int l); // player.cpp
void InitDiabloMsg(char e); // error.cpp
void CalcPlrInv(int pnum); // items.cpp

// InitObjectGFX	0000000000457E30	
// FreeObjectGFX	0000000000458029	
// RndLocOk	000000000045808F	
// __dc_TrapLocOk	00000000004581D8	
// __dc_RoomLocOk	000000000045824E	
// InitRndLocObj	0000000000458312	
// InitRndLocBigObj	0000000000458438	
// ClrAllObjects	0000000000458593	
// AddTortures	0000000000458745	
// __dc_AddTrapLine	00000000004588B3	
// __dc_AddLeverObj	0000000000458C09	
// AddMazeBook	0000000000458D47	
// InitRndBarrels	0000000000458E90	
// AddL1Objs	0000000000459015	
// AddL2Objs	00000000004590FA	
// WallTrapLocOk	00000000004591DF	
// AddObjTraps	0000000000459255	
// InitObjects	000000000045951F	
// SetMapObjects	0000000000459817	
// DeleteObject	0000000000459B79	
// SetupObject	0000000000459C24	
// SetObjMapRange	0000000000459F97	
// SetBookMsg	000000000045A025	
// AddL1Door	000000000045A055	
// AddSCambBook	000000000045A15C	
// AddChest	000000000045A1AF	
// AddL2Door	000000000045A279	
// AddSarc	000000000045A2F3	
// AddFlameTrap	000000000045A35A	
// AddFlameLvr	000000000045A3D2	
// AddTrap	000000000045A419	
// AddObjLight	000000000045A4CC	
// AddBarrel	000000000045A55A	
// AddShrine	000000000045A594	
// AddBookcase	000000000045A634	
// AddPurifyingFountain	000000000045A662	
// AddDecap	000000000045A70E	
// AddObject	000000000045A75B	
// Obj_Light	000000000045A9F3	
// Obj_StopAnim	000000000045ABC7	
// Obj_Door	000000000045AC3B	
// Obj_Sarc	000000000045ADF9	
// ActivateTrapLine	000000000045AE55	
// Obj_FlameTrap	000000000045AF62	
// Obj_Trap	000000000045B2C6	
// ProcessObjects	000000000045B576	
// ObjSetMicro	000000000045B823	
// objects_set_door_piece	000000000045B8C7	
// ObjSetMini	000000000045B967	
// ObjL1Special	000000000045BA04	
// ObjL2Special	000000000045BD8A	
// DoorSet	000000000045C089	
// RedoPlayerVision	000000000045C2BD	
// OperateL1RDoor	000000000045C349	
// OperateL1LDoor	000000000045C661	
// OperateL2RDoor	000000000045C9AB	
// OperateL2LDoor	000000000045CBE0	
// MonstCheckDoors	000000000045CE15	
// ObjChangeMap	000000000045D23E	
// OperateL1Door	000000000045D388	
// OperateLever	000000000045D485	
// OperateBook	000000000045D525	
// OperateBookLever	000000000045D5C8	
// OperateSChambBk	000000000045D677	
// OperateChest	000000000045D6F9	
// OperateTrapLvr	000000000045D7E7	
// OperateSarc	000000000045D9D6	
// OperateL2Door	000000000045DAB8	
// TryDisarm	000000000045DBB5	

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
        break;
    // TODO: The rest
    }
}

// OperateSkelBook	000000000045F05C	
// OperateBookCase	000000000045F123	
// OperateFountains	000000000045F1A0	
// OperateDecap	000000000045F361	
// OperateObject	000000000045F3BB	
// BreakCrux	000000000045F57B	
// BreakBarrel	000000000045F752	
// BreakObject	000000000045FAFA	
// SyncL1Doors	000000000045FC6F	
// SyncCrux	000000000045FDED	
// SyncLever	000000000045FF22	
// SyncL2Doors	000000000045FFAC	
// SyncObjectAnim	0000000000460094	
// GetObjectStr	00000000004601E1	
