#ifndef __ERROR_H__
#define __ERROR_H__

//
// enums
//

enum diablo_message
{
    EMSG_NONE = 0,
    EMSG_NO_AUTOMAP_IN_TOWN = 1,     // "No automap available in town"
    EMSG_NO_MULTIPLAYER_IN_DEMO = 2, // "No multiplayer functions in demo"
    EMSG_DIRECT_SOUND_FAILED = 3,    // "Direct Sound Creation Failed"
    EMSG_NO_TMP_DIR = 4,             // "Temp directory availability error"
    EMSG_NO_SPACE_TO_SAVE = 5,       // "Not enough space to save"
    EMSG_NO_PAUSE_IN_TOWN = 6,       // "No Pause in town"
    EMSG_COPY_TO_HDD = 7,            // "Copying to a hard disk is recommended"
    EMSG_SHRINE_MYSTERIOUS,          // "Odd sensations..."
    EMSG_SHRINE_IMPOSING,            // "A surge of blood interrupts your thoughts"
    EMSG_SHRINE_HIDDEN,              // "Energy passes through your equipment..."
    EMSG_SHRINE_MAGICAL,             // "Growling is heard throughout the dungeon"
    EMSG_SHRINE_MYSTIC,              // "Your skills increase, but at a price..."
    EMSG_SHRINE_ENCHANTED,           // "Did you forget something?""
    EMSG_SHRINE_THAUMATURGIC,        // "You hear a series of creaks and thumps..."
    EMSG_SHRINE_FASCINATING,         // "You are the powerless master of fire!"
    EMSG_SHRINE_CRYPTIC,             // "Power comes from your disorientation..."
    EMSG_SHRINE_SUPERNATURAL,        // "You hear a strange cry from the distance"
    EMSG_SHRINE_EERIE,               // "You forget who you are!"
    EMSG_SHRINE_SPIRITUAL,           // "Untold Wealth!"
};

//
// variables
//

extern char msgflag;

//
// functions
//

void InitDiabloMsg(char e);

#endif