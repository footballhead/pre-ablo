#ifndef __OBJDAT_H__
#define __OBJDAT_H__

#include <windows.h>

//
// defines
//

// nice
#define ALL_OBJECTS_SIZE 69

#define OBJ_LOAD_LIST_SIZE 43

//
// enums
//

enum object_id
{
    OBJ_L1LIGHT = 0x0,
    OBJ_L1LDOOR = 0x1, // Cathedral door
    OBJ_L1RDOOR = 0x2, // Cathedral door
    OBJ_SKFIRE = 0x3,
    OBJ_LEVER = 0x4,
    OBJ_CHEST1 = 0x5, // Small Chest (1 item)
    OBJ_CHEST2 = 0x6, // Chest (2 items)
    OBJ_CHEST3 = 0x7, // Large Chest (3 items)
    OBJ_CANDLE1 = 0x8,
    OBJ_CANDLE2 = 0x9,
    OBJ_CANDLEO = 0xA,
    OBJ_BANNERL = 0xB,
    OBJ_BANNERM = 0xC,
    OBJ_BANNERR = 0xD,
    OBJ_SKPILE = 0xE,
    OBJ_SKSTICK1 = 0xF,
    OBJ_SKSTICK2 = 0x10,
    OBJ_SKSTICK3 = 0x11,
    OBJ_SKSTICK4 = 0x12,
    OBJ_SKSTICK5 = 0x13,
    OBJ_CRUX1 = 0x14, // Crucified Skeleton, in skeleton king chamber
    OBJ_CRUX2 = 0x15, // Crucified Skeleton, in skeleton king chamber
    OBJ_CRUX3 = 0x16, // Crucified Skeleton, in skeleton king chamber
    OBJ_STAND = 0x17,
    OBJ_ANGEL = 0x18,
    OBJ_BOOK2L = 0x19, // Book on Novaness. In Bone Chamber. Teaches Nova
    OBJ_BCROSS = 0x1A,
    OBJ_NUDEW2R = 0x1B,
    OBJ_SWITCHSKL = 0x1C,
    OBJ_TNUDEM1 = 0x1D,
    OBJ_TNUDEM2 = 0x1E,
    OBJ_TNUDEM3 = 0x1F,
    OBJ_TNUDEM4 = 0x20,
    OBJ_TNUDEW1 = 0x21,
    OBJ_TNUDEW2 = 0x22,
    OBJ_TNUDEW3 = 0x23,
    OBJ_TORTURE1 = 0x24,
    OBJ_TORTURE2 = 0x25,
    OBJ_TORTURE3 = 0x26,
    OBJ_TORTURE4 = 0x27,
    OBJ_TORTURE5 = 0x28,
    OBJ_BOOK2R = 0x29, // Book on dlvl 6, opens stairs on dlvl 7 to Bone Chamber
    OBJ_L2LDOOR = 0x2A, // Catacomb door
    OBJ_L2RDOOR = 0x2B, // Catacomb door
    OBJ_TORCHL = 0x2C,
    OBJ_TORCHR = 0x2D,
    OBJ_TORCHL2 = 0x2E,
    OBJ_TORCHR2 = 0x2F,
    OBJ_SARC = 0x30,
    OBJ_FLAMEHOLE = 0x31,
    OBJ_FLAMELVR = 0x32,
    OBJ_WATER = 0x33,
    OBJ_BOOKLVR = 0x34, // Book on dlvl 4. Opens stairs to dlvl 5
    OBJ_TRAPL = 0x35,
    OBJ_TRAPR = 0x36,
    OBJ_BOOKSHELF = 0x37,
    OBJ_WEAPRACK = 0x38,
    OBJ_BARREL = 0x39,
    OBJ_BARRELEX = 0x3A,
    OBJ_SHRINEL = 0x3B,
    OBJ_SHRINER = 0x3C,
    OBJ_SKELBOOK = 0x3D, // Book on a stand in skeleton theme room, gives a scroll/book
    OBJ_BOOKCASEL = 0x3E,
    OBJ_BOOKCASER = 0x3F,
    OBJ_BOOKSTAND = 0x40, // Book on a stand in library theme room, gives a scroll/book
    OBJ_BOOKCANDLE = 0x41,
    OBJ_BLOODFTN = 0x42,
    OBJ_DECAP = 0x43,
    OBJ_NULL_68 = 0x44,
};

//
// structs
//

#pragma pack(push, 8)
struct ObjDataStruct
{
    char oload; // TODO is this an enum??? -1 means "ignore", 1 means "consider for loading"
    char ofindex;
    char ominlvl;
    char omaxlvl;
    char olvltype;
    char otheme; // one of enum theme_id
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

extern ObjDataStruct AllObjects[ALL_OBJECTS_SIZE]; // nice
extern const char* ObjMasterLoadList[OBJ_LOAD_LIST_SIZE];

#endif