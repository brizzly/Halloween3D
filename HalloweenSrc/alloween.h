#ifndef ALLOWEEN__
#define ALLOWEEN__


//#define H_MAC
#define H_LINUX
//#define H_WINDOWS




typedef int		hBool;
#ifndef H_WINDOWS
#define true	1
#define false	0
#endif

typedef unsigned char	byte;
typedef unsigned short	word;
typedef unsigned long	dword;
#ifdef H_WINDOWS	// hack
typedef	unsigned int	uint;
#endif

enum
{
	TEX_TGA,
	TEX_JPG,
	TEX_UNDEF
};

typedef struct
{
	hBool	used;
	hBool	picmip_locked;
	hBool	map_tex;
	int		type;
	int		id;
	char	*name;
	
}	TEXLIST;


enum
{
	MAP_NULL,
	MAP_POLYGONS,
	MAP_BSP
};


#ifdef H_WINDOWS
static int		GlobalTextureBind=-1;
static int		GlobalEntities=0;
static int		ScreenX[] = { 512, 640, 800, 1024, 1152, 1280, 1600 };
static int		ScreenY[] = { 384, 480, 600, 768, 864, 960, 1200 };
#endif






#ifdef H_WINDOWS

#include <windows.h>
#include <Richedit.h>
#include <math.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <fstream> // .h removed in visual 8
#include <direct.h>
#include <GL\gl.h>
#include <GL\glu.h>

#endif


#ifdef H_LINUX

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdarg.h>
#include <string.h>
#include "GL/gl.h"
#include "GL/glu.h"
#include "SDL.h"

#endif


#ifdef H_MAC

#pragma GCC diagnostic ignored "-Wwrite-strings"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdarg.h>
#include <string.h>
#include "KWindow.h"
#include "KGraphic.h"
#include "KSound.h"
#include "KMusic.h"
#include "KMiscTools.h"
#include <sys/dir.h>
#include <sys/file.h>

#include <GLUT/glut.h>

#endif


#include "constantes.h"


#ifdef H_WINDOWS
#include "halloween.h"
#include "input_win.h"
#endif

#ifdef H_LINUX
#include "halloween_linux.h"
#include "input.h"
#endif

#ifdef H_MAC
#include "halloween_linux.h"
#include "input.h"
#endif


#include "interface.h"
#include "bspfile.h"
#include "objet.h"
#include "objet_proc.h"
#include "menu_init.h"
#include "menu_sel.h"
#include "menu.h"



#ifdef H_WINDOWS
#include "libbass.h"
#include "dsound.h"
#include "imageloader_win.h"
//#include "net_client.h"
//#include "net_server.h"
//#include "net_dcomp.h"
//#include "net_common.h"
#endif


#ifdef H_LINUX
//#include "libsound_linux.h"
#include "libbass.h"
#include "dsound.h"
#include "loadjpeg_linux.h"
#include "net_temp_linux.h"
#endif


#ifdef H_MAC
#include "libsound_linux.h"
#include "loadjpeg_linux.h"
#include "net_temp_linux.h"
#endif



#include "enveffect.h"
#include "sorting.h"
#include "collision.h"
//#include "collision2.h"
#include "bsptree.h"
#include "script.h"
#include "player.h"
#include "maths.h"
#include "physic.h"
#include "timer.h"
#include "tga.h"
#include "jpeg.h"
#include "image.h"
#include "opengl.h"
#include "mskmodel.h"
#include "monster.h"
#include "textdraw.h"
#include "camera.h"
#include "console.h"
#include "filesys.h"
#include "ellipsoide.h"
#include "liste.h"
#include "frustrum.h"
#include "demo.h"
#include "story.h"
#include "game.h"
#include "gameover.h"
#include "shader.h"







extern	cvar_t	visent;
extern	cvar_t	epsilon;
extern	cvar_t	draw_fps;
extern	cvar_t	player_vel;
extern	cvar_t	camera_vel;
extern	cvar_t	player_look_ud;
extern	cvar_t	player_rvel;
extern	cvar_t	draw_face;
extern	cvar_t	draw_tris;
extern	cvar_t	draw_one;
extern	cvar_t	draw_portal;
extern	cvar_t	invert_mouse;
extern	cvar_t	draw_normal;
extern	cvar_t	draw_bound;
extern	cvar_t	player_view;
extern	cvar_t	draw_cell;
extern	cvar_t	noclip_wall;
extern	cvar_t	fov;
extern	cvar_t	mapfile;
extern	cvar_t	gravity;
extern	cvar_t	drawgun;
extern	cvar_t	jumpstep;
extern	cvar_t	groundacc;
extern	cvar_t	groundslow;
extern	cvar_t	maxspeed;
extern	cvar_t	airacc;
extern	cvar_t	airslow;
extern	cvar_t	soundsys;
extern	cvar_t	respawn;
extern	cvar_t	god;
extern	cvar_t	monster;
extern	cvar_t	bombslow;
extern	cvar_t	musicvol;
extern	cvar_t	soundvol;
extern	cvar_t	autoswitch;
extern	cvar_t	fullscreen;
extern	cvar_t	colordepth;
extern	cvar_t	videomode;
extern	cvar_t	bms;
extern	cvar_t	bmd;
extern	cvar_t	statutbar;
extern	cvar_t	animspeed;
extern	cvar_t	lightmap;
extern	cvar_t	culling;
extern	cvar_t	render_ent;
extern	cvar_t	directstart;
extern	cvar_t	developer;
extern	cvar_t	offset;
extern	cvar_t	leaf;
extern	cvar_t	record;
extern	cvar_t	stoprecord;
extern	cvar_t	play;
extern	cvar_t	stopplay;
extern	cvar_t	noscript;
extern	cvar_t	gamekey_up;
extern	cvar_t	gamekey_down;
extern	cvar_t	gamekey_left;
extern	cvar_t	gamekey_right;
extern	cvar_t	gamekey_jump;
extern	cvar_t	gamekey_attack;
extern	cvar_t	gamekey_axe;
extern	cvar_t	gamekey_arbalet;
extern	cvar_t	gamekey_bombgun;
extern	cvar_t	gamekey_tromblon;
extern	cvar_t	gamekey_crouch;
extern	cvar_t	gamekey_action;
extern	cvar_t	gamekey_run;
extern	cvar_t	map_letter;
extern	cvar_t	kill_depla;
extern	cvar_t	kill_morbak;
extern	cvar_t	kill_zeubat;
extern	cvar_t	kill_subzerat;
extern	cvar_t	kill_zemummy;
extern	cvar_t	map_complete;
extern	cvar_t	music_track;
extern	cvar_t	pres_track;
extern	cvar_t	resetgame;
extern	cvar_t	picmip;
extern	cvar_t	difficulty;
extern	cvar_t	gameslot;
extern	cvar_t	gamma_;
extern	cvar_t	tflame;
extern	cvar_t	ghost;
extern	cvar_t	terminator;
extern	cvar_t	net_port;
extern	cvar_t	net_ip;
extern	cvar_t	net_mapid;
extern	cvar_t	net_clname;
extern	cvar_t	net_svname;
extern	cvar_t	net_password;
extern	cvar_t	net_gametype;
extern	cvar_t	net_dedicated;
extern	cvar_t	notextures;
extern	cvar_t	bestscore1;
extern	cvar_t	bestscore2;
extern	cvar_t	bestscore3;
extern	cvar_t	bestscore4;
extern	cvar_t	bestscore5;
extern	cvar_t	bestscore6;
extern	cvar_t	bestscore7;
extern	cvar_t	bestscore8;
extern	cvar_t	bestscore9;
extern	cvar_t	fogmode;
extern	cvar_t	fogdensity;

#ifndef H_WINDOWS
extern	KEYSINPUT	KeysInput[K_MAXKEYS+4];
#endif

extern	int	KeyBuffer[KEY_BUFFER_MAX];
extern	int		gScore;
extern	hBool	gAllGoalCleared;
extern	hBool	gPlayerWalk;
extern	float	gPlayerGunRecul;
extern	float	gPlayerBobWalkstep;
extern	float	gPlayerWalkTime;
extern	hBool	IsLaunchPossible;
extern	int		gPrintLine;
extern	int		MapDataFormat;
extern	hBool	IsScreenShot;
extern	hBool	IsConsoleActive;
extern	hBool	gKey_Console;
extern	hBool	gKey_Action;
extern	Camera_t	gCamera;
extern	vect_t	gStart;
extern	TEXLIST	*TextureList;
extern	hBool	IsConsoleActive;
extern	hBool	*KeyMask;
extern	int		gFontSize;
extern	Fnt_t	NormalFont;
extern	Fnt_t	NiceFont;
extern	int		gNb_Leafs;
extern	int		gNb_Portals;
extern	int		gNb_nodes;
extern	int		gNb_faces;
extern	float	Step;
extern	Face_t	*PortalList;
extern	char	*PVSData;
extern	NL_t	NLlist;
extern	Face_t	*PolyList;
extern	ENTITY	*gEntitiesLoadedList;
extern	pEntity	gEntitiesList;
extern	int		gLeafLocation;
extern	float	Tsin[360];
extern	float	Tcos[360];
extern	float	gDeplaHudSize;
extern	float	gMorbakHudSize;
extern	float	gMummyHudSize;
extern	float	gZebatHudSize;
extern	float	gZeratHudSize;
extern	float	gFioleCatchedSize;
extern	float	gFioleCatchedRad;
extern	hBool	gMenu_InitFirstSel;
extern	hBool   IsWaitKey;

extern	int		txid_FONT;
extern	int		txid_NICEFONT;
extern	int		txid_TARGET;
extern	int		txid_RAYURE;
extern	int		txid_LOGO;
extern	int		txid_LOGO2;
extern	int		txid_COPY;
extern	int		txid_FLECHE;
extern	int		txid_0;
extern	int		txid_1;
extern	int		txid_2;
extern	int		txid_3;
extern	int		txid_4;
extern	int		txid_5;
extern	int		txid_6;
extern	int		txid_7;
extern	int		txid_8;
extern	int		txid_9;
extern	int		txid_HEART;
extern	int		txid_STARTGAME_G;
extern	int		txid_ORDERGAME_G;
extern	int		txid_OPTION_G;
extern	int		txid_HELPORDER_G;
extern	int		txid_QUITGAME_G;
extern	int		txid_STARTGAME;
extern	int		txid_STARTGAME2;
extern	int		txid_CONFIG;
extern	int		txid_CONFIG2;
extern	int		txid_CONFIG3;
extern	int		txid_CONFIG4;
extern	int		txid_HELPORDER;
extern	int		txid_HELPORDER2;
extern	int		txid_QUITGAME;
extern	int		txid_QUITGAME2;
extern	int		txid_ORDERGAME;
extern	int		txid_ORDERGAME2;
extern	int		txid_CURSOR;
extern	int		txid_INPUTCONFIG;
extern	int		txid_VIDEOCONFIG;
extern	int		txid_SOUNDCONFIG;
extern	int		txid_INPUTCONFIG2;
extern	int		txid_VIDEOCONFIG2;
extern	int		txid_SOUNDCONFIG2;
extern	int		txid_CLOCK;
extern	int		txid_BOMBE;
extern	int		txid_BULLET;
extern	int		txid_BUTTON;
extern	int		txid_BUTTON_P;
extern	int		txid_BUTTON_M;
extern	int		txid_BUTTON_P_SEL;
extern	int		txid_BUTTON_M_SEL;
extern	int		txid_BUTTON_DEL;
extern	int		txid_BUTTON2_DEL;
extern	int		txid_GAMEOPTIONCONFIG;
extern	int		txid_GAMEOPTIONCONFIG2;
extern	int		txid_RESUME;
extern	int		txid_RESUME2;
extern	int		txid_LEAVE;
extern	int		txid_LEAVE2;
extern	int		txid_EXIT;
extern	int		txid_EXIT2;
extern	int		txid_BUTTONGO;
extern	int		txid_BUTTONGO2;
extern	int		txid_BUTTONNEXT;
extern	int		txid_BUTTONNEXT2;
extern	int		txid_BUTTONBACK;
extern	int		txid_BUTTONBACK2;
extern	int		txid_CONSOLE;
extern	int		txid_FLAME[8];
extern	int		txid_LIGHTMAP_N[64];
extern	int		txid_LOADINGMAP;
extern	int		txid_CITROUILLE;
extern	int		txid_GRIS;
extern	int		txid_ORDER;
extern	int		txid_HELP;
extern	int		txid_FIOLE1_ICONE;
extern	int		txid_FIOLE2_ICONE;
extern	int		txid_FIOLE3_ICONE;
extern	int		txid_BONUS1;
extern	int		txid_CREDIT;
extern	int		txid_BG1;
extern	int		txid_BG2;
extern	int		txid_BG2_G;
extern	int		txid_NEWTITLE[30];
extern	int		txid_TNN[2];
extern	int		txid_GAMEANIM[25];
extern	int		txid_HELPANIM[15];
extern	int		txid_OPTIONANIM[14];
extern	int		txid_ORDERANIM[19];
extern	int		txid_QUITANIM[25];
extern	int		txid_DEPLA_ICON[2];
extern	int		txid_MORBAK_ICON[2];
extern	int		txid_SUB_ICON[2];
extern	int		txid_BAT_ICON[2];
extern	int		txid_MOMY_ICON[2];
extern	int		txid_JADETEAM;
extern	int		txid_SELECTGAMESLOT;
extern	int		txid_EASY;
extern	int		txid_NORMAL;
extern	int		txid_HARD;
extern	int		txid_EMPTY;
extern	int		txid_EASY_G;
extern	int		txid_NORMAL_G;
extern	int		txid_HARD_G;
extern	int		txid_EMPTY_G;
extern	int		txid_TDM;
extern	int		txid_TDM_G;
extern	int		txid_YES;
extern	int		txid_YES_G;
extern	int		txid_NO;
extern	int		txid_NO_G;
extern	int		txid_TOILE;
extern	int		txid_DEFAULTMAP;
extern	int		txid_OMBRAGE;

extern	int		shareware;
extern	float	WorldColorR;
extern	float	WorldColorG;
extern	float	WorldColorB;
extern	int		shareware2;
extern	float	gLavaOff_sin;
extern	float	gLavaOff_cos;
extern	float	cursor_x;
extern	float	cursor_y;
extern	hBool	MouseKeyA_clicked;
extern	RESPAWN	RespawnTab[MAX_RESPAWN_OBJ];
extern	hBool	gMapBonus;
extern	hBool	gMapIntro;
extern	int		gNumberOfMonsters;
extern	int	gKilledDeplas;
extern	int	gKilledMorbaks;
extern	int	gKilledZeubat;
extern	int	gKilledSubzerat;
extern	int	gKilledZemummy;
extern	float	gFrameTime_msec;
extern	float	gTotalTime_msec;
extern	float	gFPSshowed;
extern	hBool	gDispLetters;
extern	int	gPlayerHasGun;
extern	pFace	Gun1LaunchedFaceList;
extern	pFace	Gun3FaceList;
extern	int	game_heart;
extern	int	ProgramState;
extern	int	NextProgramState;
extern	int	saved_ProgramState;
extern	hMOUSEINPUT	MouseInput;
extern	int	game_NbMaps;
extern	int	game_fleches;
extern	double	gTimeStart;
extern	float	game_clock;
extern	int	game_bombes;
extern	int	game_bullet;
extern	int	game_jade;
extern	hBool	game_isover;
extern	int	gAngleView_player;
extern	float	gCam_EarthQuake_time;
extern	float	gCam_EarthQuake_force;
extern	hBool	game_weapon1;
extern	hBool	game_weapon2;
extern	hBool	game_weapon3;
extern	char	*ConsoleBuffer;
extern	hBool	PlayerOnLift;
extern	int	gNumberOfLoadedEntities;
extern	hBool	IsValidVideo;
extern	hBool	ResumingAlloweed;
extern	hBool	gDrawCursor;
extern	int	gGameSlotToProcess;
extern	float	gDemoReminderTime;
extern	Model	gModel_depla;
extern	Model	gModel_morbak;
extern	Model	gModel_zebat;
extern	Model	gModel_subzerat;
extern	Model	gModel_zemummy;
extern	uint	gMemoryUsage;
extern	leafs_t	*LeafArray;
extern	Face_t	*FaceArray;
extern	noeud_t	*NodeArray;
extern	portal_t *PortalArray;
extern	int	gNbPts;
extern	Plan_t	gFrustrumPlanes[6];
extern	char	ErrorMessage[255];
extern	int	gNb_ActionBrush;
extern	ABRUSH	ActionBrushTab[MAX_SCRIPTSLOT];
extern	int	texture_id_plan[TEXTURELOADED];
extern	hBool	gMapLightened;
extern	int	gNb_Respawn;
extern	FILE	*gDemoFile;
extern	hBool	gScanKeyActivated;
extern	int	gIdGameKey;
extern	int	gIdScannedKey;
extern	int	gGameIdKeys[MAX_GAMEKEY+4];
extern	hBool	gPlayerCrouch;
extern	float	gPlayerCrouchStep;
extern	hBool	gPlayerRun;
extern	char	HalloLetters[9];
extern	float	HLetter_Time;
extern	float	KillDepla_Time;
extern	float	KillMorbak_Time;
extern	float	KillZebat_Time;
extern	float	KillSubzerat_Time;
extern	float	KillZemummy_Time;
extern	float	gOscilleGunsVal;
extern	hBool	gForceNoDepthTest;
extern	int	game_MapID;
extern	int	game_HitBestScoreMode;
extern	int	gNbSort;
extern	TABSORT	TabSort;
extern	int	gNbTrack;
extern	hBool	game_fiole1_catched;
extern	hBool	game_fiole2_catched;
extern	hBool	game_fiole3_catched;
extern	float	game_MatrixMode;
extern	float	game_FalconMode;
extern	float	game_InjuryMode;
extern	hBool	game_TofCheat;
extern	hBool	game_TimeLimit;
extern	float	game_DieTime;


#ifdef H_WINDOWS

extern	HMMIO	gHmmio;
extern	HWND	ghWnd;
extern	HDC		hDC;

#endif


extern	hBool	IsMouseCursor;
extern	hBool 	msclic_A;
extern	hBool 	msclic_B;
extern	float	msoffsetX;
extern	float	msoffsetY;
extern	short	kbstate[KEYMASK_SIZE];


extern	hBool	IsColorScreen;
extern	float	CS_trans;
extern	float	CS_trans2;
extern	float	CS_time;
extern	float	CS_R;
extern	float	CS_G;
extern	float	CS_B;
extern	float	CS_R_step;
extern	float	CS_G_step;
extern	float	CS_B_step;
extern	int	MapGoals[10][3][9];
extern	float	MapTime[3];
extern	float	MonsterLife[5][3];
extern	float	WeaponDammage[5][3];
extern	float	MonsterDammage[5][3];
extern	hBool	MassueCanHurt;
extern	hBool	gMultiTexturing;
extern	vect_t	gStopPos;
extern	float	game_WinTime;
extern	float	gHeartSize;
extern	SNDOBJ	gSoundObjet[MAX_SAMPLE];
extern	int	gNb_Sounds;
extern	hBool	gLoad3Dstuff;
extern	float	gCameraShowTime;
extern	long	gClockOffset;
extern	hBool	gCheatMode_pumpkin;
extern	hBool	gCheatMode_monsterbomb;
extern	hBool	gCheatMode_son;
extern	hBool	gWelcomeToHalloween;
extern	hBool	gGameEnded;
extern	int	MouseX;
extern	int	MouseY;
extern	MENU	MenuType[MAX_SLOT];
extern	int		GlobalTextureBind;
extern	int		GlobalEntities;
extern	int		ScreenX[];
extern	int		ScreenY[];
extern	int			nbTabFaces;
extern	pTabFace	gTabFaces;
extern	hBool		gIsFog;
extern	float		gIsEclair;
extern	hBool		gIsDemo;
extern	float		gAlphaFont;
extern	int		new_fullscreen, new_videomode, new_colordepth;
extern	float	gStoryDisp_time;
extern	float	gFadingTime;


extern	hBool	gIsMultiplayer;
extern	int	gMultiMapID;
extern	hBool	gIsServer;
extern	short	gConnectedPlayer;
extern	short	gMultiMapPlayed;
extern	hBool	gMultiConnected;
extern	hBool	gClientAskProtocol;
//extern	char	gNetBitKeys;
//extern	char	gNetBitKeys_back;
extern	hBool	gMultiClientReady;
extern	short	gMultiNbEntsMax;
extern	short	gMultiNbEnts;
extern	short	gMultiSV_clentid;
extern	short	gMultiSV_clentn;
extern	vect_t	gMultiSV_clentstartpos;
extern	hBool	gMultiCLleftgame;
extern	hBool	gMouseClipped;
extern	uint	gSV_FrameID;
extern	uint	gCL_FrameID;
//extern	int	gSV_score;
//extern	int	gCL_score;
extern	hBool	gMultiPodium;






#endif //ALLOWEEN__

