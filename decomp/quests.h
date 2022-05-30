#ifndef __QUESTS_H__
#define __QUESTS_H__

#include <windows.h>

//
// defines
//

#define MAXQUESTS 10

//
// enums
//

enum quest_id
{
    Q_SKELKING = 0,
    Q_BUTCHER = 1,
    Q_ROCK = 2,
    Q_SCHAMB = 3,
    Q_MAZE = 4,
    Q_INVALID = -1
};

enum quest_state
{
    QUEST_NOTAVAIL = 0,
    QUEST_INIT = 1,
    QUEST_ACTIVE = 2,
    QUEST_DONE = 3
};

//
// structs
//

#pragma pack(push, 8)
struct QuestStruct
{
  char _qlevel;
  char _qtype;
  char _qactive;
  char anonymous_2;
  int _qtx;
  int _qty;
  char _qslvl;
  char _qidx;
  char field_E;
  char field_F;
};
#pragma pack(pop)

//
// variables
//

extern QuestStruct quests[MAXQUESTS];
extern int numquests;
extern BOOL questlog;

#endif