
#ifndef GAME___
#define GAME___

#include "alloween.h"

#define	MULTI_TIMELIMIT		600

//struct
//{
//	char	name[FILENAME_MAX];
//	int		format;
//	int		txid;
//	int		txid_sel;
//	int		txid_grey;
//
//} maps [MAX_MAPFILE];


enum
{
	PS_DEBUG,
	PS_LOADING,
	PS_INTRO,
	PS_LOGO,
	PS_INTROMAP,
	PS_LAUNCHMAPINTRO,
	PS_OUTRO,
	PS_CREDIT,
	PS_PRES,
	PS_SELECTMAP,
	PS_STARTMAP,
	PS_HELP,
	PS_ORDER,
	PS_CONFIG,
	PS_CONFIG_INPUT,
	PS_CONFIG_VIDEO,
	PS_CONFIG_SOUND,
	PS_CONFIG_GAMEOPTION,
	PS_GAME,
	PS_CONNECTING,
	PS_LOADENT,
	PS_LOADCLIENT,
	PS_BEGINGAME,
	PS_YESNO,
	PS_SETSLOTGAME,
	PS_DIE,
	PS_WIN,
	PS_PAUSEDGAME,
	PS_GAMEOVER,
	PS_GREETINGS,
	PS_SHOWSHINFO,
	PS_ENDSCORE,
	PS_QUIT
};

//int		ProgramState;
//int		saved_ProgramState;

int		game_Load2D(char *name);
void	game_Load2D_game_icons();
void	game_Load2Dicons();
void	game_LoadEntities();
void	game_EngineProcess();
void	game_QuitMessage(char *message);
void	game_PlayWeirdSound();
void	game_PlayThunderSound();
void	game_InitYouWin();
void	game_InitYouLose();
hBool	game_Load2Dstuff();
hBool	game_Load3Dstuff();
hBool	game_LoadFileMap();
hBool	game_LoadModels();
void	game_UnloadModels();
hBool	game_Load();
void	game_CameraViewInit();
void	game_Init();
void	game_catchedinit2(char *Text);
void	game_catchedinit(int text_id);
void	game_catchedprocess();
hBool	GameProcess();
void	game_AddHLetter(char letter);
void	game_CheckMapCompleted();
char *	game_GetMusicString(int id);
hBool	game_UseFiole();
void	game_DieProcess();
void	game_WinProcess();
void	game_UpdateSlot_all();
void	game_UpdateSlot(FILE *file, int diff, int a, int b, int c, int d, int e, int f, int g);
void	game_LoadSlot(FILE *file);
void	game_DeleteAllSlots();
int		game_GetSlotDifficulty(FILE *file);
void	game_InitMapGoals();
void	game_DrawTarget();
void	game_LoadMap(int map_id);
void	game_InitStatutBar();
hBool	game_IsMapOpened(int map_id);
void	game_InitCameraShow_letter();
void	game_InitCameraShow(float timing, float posX, float posY, float posZ, float rotX, float rotY, float rotZ);
void	game_StopCameraShow();
hBool	game_IsCameraShow();
void	game_CameraShowProcess();
void	game_LaunchMapIntro();
void	game_LaunchMapMusic();
void	game_LaunchPresMusic();
void	game_LaunchBonusMapMusic();
hBool	game_ReadBit(int value, int idbit);
void	game_SetBit(int *value, int idbit);
void	game_DrawAuthorNames();
void	game_InitGoalCounters();
void	game_GameEnded();



void	DrawGameStatutBar();
void	DrawGreetings();
void	DispText(char *txt, int txid);
void	QuitGame();
void	g_LoadMultiMap();


#endif // GAME___
