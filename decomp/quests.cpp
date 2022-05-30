#include "quests.h"

//
// Uninitialized variables (.data:00615F40)
//

QuestStruct quests[MAXQUESTS];
int numqlines;
int numquests;
int ReturnLvlX;
int ReturnLvlY;
int qline;
int qlist[MAXQUESTS];
BOOL questlog;
int questpentframe;

//
// Code
//

// InitQuests	0000000000471730	
// CheckQuests	00000000004718AB	
// CheckQuestKill	00000000004719CD	
// Quest_BackupDungeon	0000000000471A5D	
// DrawButcher	0000000000471ACC	
// DrawSkelKing	0000000000471B18	
// DrawSChamber	0000000000471B5B	
// DrawMaze	0000000000471C89	
// DRLG_CheckQuests	0000000000471D61	
// SetReturnLvlPos	0000000000471E4D	
// GetReturnLvlPos	0000000000471ED6	
// ResyncQuests	0000000000471EFA	
// DrawQBack	0000000000471FC1	
// PrintQLString	0000000000472030	
// DrawQLine	0000000000472218	
// DrawQuestLog	0000000000472274	
// StartQuestlog	0000000000472324	
// QuestlogDown	00000000004723C1	
// QuestlogEnter	0000000000472491	
// QuestlogESC	00000000004724F3	
// QuestlogOff	0000000000472580	
