#include "saveload.h"

BYTE *tbuff = nullptr;
PlayerStruct plr[MAX_PLRS]{};
QuestStruct quests[MAXQUESTS]{};
int numquests = 0;
int leveltype = 0;
int gbActivePlayers = 0;

namespace {

// Save files start with 3 magic bytes: 'MSD' for demo, '   ' otherwise
constexpr int kFileMagicSize = 3;

// Loads a 2-byte value into a 4-byte value with sign extension.
int WLoad() {
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

BOOL OLoad() {
  if (*tbuff++ == TRUE)
    return TRUE;
  else
    return FALSE;
}

void LoadPlayer(int i) {
  // Demo difference! Omit the final 9 (not 10) pointers
  memcpy(&plr[i], tbuff, sizeof(*plr) - (9 * sizeof(void *)));
  tbuff += sizeof(*plr) - (9 * sizeof(void *));
}

void LoadQuest(int i) {
  memcpy(&quests[i], tbuff, sizeof(*quests));
  tbuff += sizeof(*quests);
}

void SavePlayer(int i) {
  // Demo difference! Omit the final 9 (not 10) pointers
  memcpy(tbuff, &plr[i], sizeof(*plr) - (9 * sizeof(void *)));
  tbuff += sizeof(*plr) - (9 * sizeof(void *));
}

void SaveQuest(int i) {
  memcpy(tbuff, &quests[i], sizeof(*quests));
  tbuff += sizeof(*quests);
}

}  // namespace

void LoadGame() {
  tbuff += kFileMagicSize;
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

void SaveGame() {
  //
  // ATTENTION: If you touch this function then fix kMinimalSaveSize kthxbai
  //

  // main.cc only modifies `plr` and `quests` so that's all we repack.
  tbuff += kPlrOffset;
  SavePlayer(0);

  tbuff += kWordSize;  // skip numquests
  for (int i = 0; i < numquests; ++i) {
    SaveQuest(i);
  }
}
