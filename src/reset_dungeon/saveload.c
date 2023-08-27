#include "saveload.h"

#define MAGIC_SIZE 3
#define PLR_START_OFFSET 0x20
#define WLOAD_SIZE 2

BYTE *tbuff;
PlayerStruct plr[MAX_PLRS];
QuestStruct quests[MAXQUESTS];
int numquests;
int leveltype;
int gbActivePlayers;

// .text:00460977
static int WLoad() {
  int rv;
  if (*tbuff & 0x80) {
    rv = 0xFFFF0000;
  } else {
    rv = 0;
  }
  rv |= *tbuff++ << 8;
  rv |= *tbuff++;

  return rv;
}

static BOOL OLoad() {
  if (*tbuff++ == TRUE)
    return TRUE;
  else
    return FALSE;
}

static void LoadPlayer(int i) {
  // Demo difference! Omit the final 9 (not 10) pointers
  memcpy(&plr[i], tbuff, sizeof(*plr) - (9 * sizeof(void *)));
  tbuff += sizeof(*plr) - (9 * sizeof(void *));
}

static void LoadQuest(int i) {
  memcpy(&quests[i], tbuff, sizeof(*quests));
  tbuff += sizeof(*quests);
}

void LoadGame() {
  tbuff += MAGIC_SIZE;  // skip magic number
  BOOL setlevel = OLoad();
  int setlvlnum = WLoad();
  int currlevel = WLoad();
  leveltype = WLoad();
  int _ViewX = WLoad();
  int _ViewY = WLoad();
  BOOL invflag = OLoad();
  BOOL chrflag = OLoad();
  gbActivePlayers = WLoad();
  int _nummonsters = WLoad();
  int _numitems = WLoad();
  int _nummissiles = WLoad();
  int _numspells = WLoad();
  int _numobjects = WLoad();
  plr[0].plractive = OLoad();
  plr[1].plractive = OLoad();
  plr[2].plractive = OLoad();
  plr[3].plractive = OLoad();
  for (int i = 0; i < gbActivePlayers; ++i) {
    if (plr[i].plractive) {
      LoadPlayer(i);
    }
  }
  numquests = WLoad();
  for (int i = 0; i < MAXQUESTS; ++i) {
    LoadQuest(i);
  }

  // Don't need to load the rest
}

static void SavePlayer(int i) {
  // Demo difference! Omit the final 9 (not 10) pointers
  memcpy(tbuff, &plr[i], sizeof(*plr) - (9 * sizeof(void *)));
  tbuff += sizeof(*plr) - (9 * sizeof(void *));
}

static void SaveQuest(int i) {
  memcpy(tbuff, &quests[i], sizeof(*quests));
  tbuff += sizeof(*quests);
}

void SaveGame() {
  tbuff += PLR_START_OFFSET;  // plr[0] starts at 0x20
  SavePlayer(0);

  tbuff += WLOAD_SIZE;  // skip numquests (2 bytes)
  for (int i = 0; i < numquests; ++i) {
    SaveQuest(i);
  }
}
