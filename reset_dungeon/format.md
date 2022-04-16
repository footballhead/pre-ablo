# Game00.sav

Types are stored in big endian format.

While this file is not a fixed size, it's typically a good assumption that
gbActivePlayers == 1 and plr[0].plractive == TRUE (since single player is the
only working mode).

* 3 bytes: magic number. Either `MSD` for demo mode, or `   ` (3 ASCII spaces) for fullgame
* 1 byte: bool: setlevel
* 2 bytes: word: setlvlnum
* 2 bytes: word: currlevel
* 2 bytes: word: leveltype
* 2 bytes: word: ViewX
* 2 bytes: word: ViewY
* 1 byte: bool: invflag
* 1 byte: bool: chrflag
* 2 bytes: word: gbActivePlayers
* 2 bytes: word: nummonsters
* 2 bytes: word: numitems
* 2 bytes: word: nummissles
* 2 bytes: word: numspells
* 2 bytes: word: numobjects
* 1 byte: bool: plr[0].plractive
* 1 byte: bool: plr[1].plractive
* 1 byte: bool: plr[2].plractive
* 1 byte: bool: plr[3].plractive
* 0x4270 bytes: PlayerStruct: plr[0] if gbActivePlayers == 1 and plr[0].plractive
* 0x4270 bytes: PlayerStruct: plr[1] if gbActivePlayers == 2 and plr[1].plractive
* 0x4270 bytes: PlayerStruct: plr[2] if gbActivePlayers == 3 and plr[2].plractive
* 0x4270 bytes: PlayerStruct: plr[3] if gbActivePlayers == 4 and plr[3].plractive
* 2 bytes: word: numquests
* 16 bytes: QuestStruct: quests[0]
* 16 bytes: QuestStruct: quests[1]
* 16 bytes: QuestStruct: quests[2]
* 16 bytes: QuestStruct: quests[3]
* 16 bytes: QuestStruct: quests[4]
* 16 bytes: QuestStruct: quests[5]
* 16 bytes: QuestStruct: quests[6]
* 16 bytes: QuestStruct: quests[7]
* 16 bytes: QuestStruct: quests[8]
* 16 bytes: QuestStruct: quests[9]




... TODO ... 