#include "multi.h"

#include "dplay_stub.h"

// initialized vars (.data:004A3AA8)

LPDIRECTPLAY lpIDP = NULL;
HANDLE dphEvent = NULL;
// ...
BOOL isSinglePlayer = FALSE;
BOOL hasJoinedGame = FALSE;
BOOL handleSystemMessage = FALSE;

// __dc_DPlayCreate	00000000004141E0	
// __dc_DPlayEnumSessions	000000000041425B	
// __dc_DPlayOpenSession	00000000004142DB	
// __dc_DPlayCreateSession	00000000004143D4	
// __dc_DPlayEnumPlayers	00000000004144D3	
// __dc_DPlayCreateMyPlayer	0000000000414514	
// __dc_DPlayDestroyPlayer	000000000041459E	
// DPlayHandleMessage	000000000041460D	
// __dc_EnumSession	0000000000414B0E	
// __dc_EnumSP	0000000000414B8A	
// __dc_EnumPlayer	0000000000414BCB	
// DPlayFreePlayer	0000000000414BED	
// DPlayInitPlayer	0000000000414CA3	
// unknown_dplay_copy_string3	0000000000414CF4	
// unknown_dplay_send_n125	0000000000414E1C	
// unknown_dplay_get_iand1	0000000000414EE5	
// unknown_dplay_get_plractive	0000000000414F6D	
// unknown_dplay_send	0000000000415053	
// unknown_dplay_send_byte	000000000041515B	
// unknown_dplay_send_word	0000000000415186	
// unknown_dplay_send_dword	00000000004151C6	
// unknown_dplay_send_bool	0000000000415236	
// unknown_dplay_send_plr	000000000041527F	
// unknown_dplay_send_mon	0000000000415888	
// unknown_dplay_send_mis	0000000000415BA5	
// unknown_dplay_send_spell	0000000000415DFC	
// unknown_dplay_send_obj	0000000000416030	
// unknown_dplay_send_item	00000000004161B5	
// unknown_dplay_recv_byte	00000000004162BD	
// unknown_dplay_recv_word	00000000004162E7	
// unknown_dplay_recv_dword	000000000041632C	
// unknown_dplay_recv_bool	00000000004163A3	
// unknown_dplay_recv_plr	00000000004163E9	
// unknown_dplay_recv_mon	00000000004169FE	
// unknown_dplay_recv_mis	0000000000416D1B	
// unknown_dplay_recv_spell	0000000000416F73	
// unknown_dplay_recv_obj	00000000004171A8	
// unknown_dplay_recv_item	000000000041732E	
// unknown_dplay_get_plrs	0000000000417437	
// unknown_dplay_wait_send	000000000041763C	
// unknown_dplay_game_send	00000000004176AB	
// unknown_dplay_case_plr	0000000000417E07	
// unknown_dplay_wait_recv	0000000000417FDA	
// unknown_dplay_game_recv	0000000000418047	
// unknown_dplay_do_recv	0000000000418766	
// unknown_dplay_init_seeds	0000000000418E25	
