// Resets the save to a state that can be replayed. This:
//
// - Resets all quest progress.
// - Regenerates dungeons with new seeds.
//
// The core logic is in `ResetDungeon()`. Everything else exists to read/write
// the save file.
//
// This was written to be familiar to anyone who's worked on Devilution before.
// The idiom is to load into static members like `plr` and `quests`.
#include <windows.h>

#include <cstddef>
#include <ctime>
#include <filesystem>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

#include "io.h"
#include "saveload.h"
#include "structs.h"

// Need 32 bits since we use decompiled source and require the structs to pack
// correctly. CI takes care of this for us.
static_assert(sizeof(void*) == 4, "Need a 32-bit toolchain");

namespace {

constexpr std::string_view kSaveFile = "Save\\Game00.sav";

// Returns `true` if the save file is safe to be reset, `false` otherwise.
// Pauses and shows a message to the user if something is wrong.
//
// `LoadGame()` must be called first!
bool IsSaveInValidState() {
  // It's easier to ask players to save in town (rather than figure out how to
  // modify the save file in order to put them back in town).
  if (leveltype != DTYPE_TOWN) {
    MessageBox(nullptr, TEXT("Return to town first!"), TEXT("Error"),
               MB_OK | MB_ICONERROR);
    return false;
  }

  if (gbActivePlayers != 1) {
    MessageBox(nullptr, TEXT("Expected gbActivePlayers == 1!"), TEXT("Error"),
               MB_OK | MB_ICONERROR);
    return false;
  }

  return true;
}

// Modifies the in-memory save to allow for new dungeons and quests.
//
// `LoadGame()` must be called first!
void ResetDungeon() {
  for (int i = 0; i < NUMLEVELS; ++i) {
    plr[0]._pSLvlVisited[i] = FALSE;
  }

  srand(static_cast<int>(time(nullptr)));
  // Start at i=1 so we don't touch Tristram
  for (int i = 1; i < NUMLEVELS; ++i) {
    plr[0]._pSeedTbl[i] = rand();
    plr[0]._pLvlVisited[i] = FALSE;
  }

  for (int i = 0; i < numquests; ++i) {
    quests[i]._qactive = QUEST_INIT;
  }

  // TODO Remove the town portal in town since it will be broken.
}

}  // namespace

INT WINAPI WinMain(HINSTANCE /*instance*/, HINSTANCE /*prev_instance*/,
                   PSTR /*cmdline*/, INT /*cmd_show*/) {
  const int choice = MessageBox(
      nullptr,
      TEXT("This tool will reset the dungeon and quests so you can experience "
           "them again. You will lose items in the dungeon that you have not "
           "picked up. Anything in town will be kept. Your character will be "
           "left alone. Reset your progress?"),
      TEXT("Reset your progress?"), MB_YESNO | MB_ICONWARNING);
  if (choice != IDYES) {
    return 1;
  }

  // Pre-Release Demo uses a different class and window name compared to
  // Devilution. Found via decomp (look for RegisterClass and CreateWindow).
  if (FindWindow(TEXT("Diablo"), TEXT("Diablo Game")) != nullptr) {
    MessageBox(nullptr, TEXT("Quit Pre-ablo first!"), TEXT("Error"),
               MB_OK | MB_ICONERROR);
    return 1;
  }

  // `buffer` is the only copy of the save data. `tbuff` is set as mutable
  // iterator to `buffer` so saveload.h functions work.
  std::optional<std::vector<std::byte>> buffer = ReadFromFile(kSaveFile);
  if (!buffer.has_value()) {
    return 1;
  }
  if (buffer->size() < kMinimalSaveSize) {
    MessageBox(nullptr, TEXT("Unexpected save file size, cannot continue."),
               TEXT("Error"), MB_OK | MB_ICONERROR);
    return 1;
  }

  tbuff = reinterpret_cast<BYTE*>(buffer->data());
  LoadGame();
  if (!IsSaveInValidState()) {
    return 1;
  }
  ResetDungeon();

  // Backup at the last possible moment to keep the filesystem clean. If we
  // reach here then we can reasonably be sure that we'll succeed and that the
  // backup will be warranted.
  //
  // While `time()` returns seconds, it is unlikely that this will try to create
  // a folder that already exists.
  std::error_code copy_error;
  std::filesystem::copy("Save", "Save_" + std::to_string(time(nullptr)),
                        copy_error);
  if (copy_error) {
    if (MessageBox(nullptr,
                   TEXT("We couldn't make a backup of your save. You can make "
                        "one by copying the SAVE folder. Continue?"),
                   TEXT("Continue without backup?"),
                   MB_YESNO | MB_ICONWARNING) != IDYES) {
      return 1;
    }
  }

  tbuff = reinterpret_cast<BYTE*>(buffer->data());
  SaveGame();
  if (!WriteToFile(kSaveFile, buffer.value())) {
    return 1;
  }

  MessageBox(nullptr,
             TEXT("Success! Reload your game and walk to the cathedral."),
             TEXT("Success!"), MB_OK | MB_ICONINFORMATION);

  return 0;
}
