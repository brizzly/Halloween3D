
#ifndef CONSOLE_INCLUDED
#define CONSOLE_INCLUDED

#include "alloween.h"



#ifdef H_WINDOWS

#include <tchar.h>

#endif


#ifdef H_LINUX

#include <stdarg.h>

#endif




#define	CONSOLE_NB_LINE			16
#define	CONSOLE_WIDTH			80
#define	CONSOLE_MAXDISPCHAR		5120
#define	CONSOLE_NB_CHAR			512000
#define	CONSOLE_FONT			14

#define	VERSION				"version"
#define	MAP					"map"
#define	DRAWFPS				"drawfps"
#define	PVEL				"pvel"
#define	CVEL				"cvel"
#define	PRVEL				"prvel"
#define	PLOOKUD				"plook"
#define	DRAWFACE			"drawface"
#define	DRAWTRIS			"drawtris"
#define	DRAWONE				"drawone"
#define	DRAWPORTAL			"drawportal"
#define	INVERTMOUSE			"invertmouse"
#define	DRAWNORMAL			"drawnormal"
#define	PLAYERVIEW			"pview"
#define	DRAWBOUND			"drawbound"
#define	DRAWCELL			"drawcell"
#define	NOCLIP				"noclip"
#define	FOV					"fov"
#define	MAPFILE				"filemap"
#define	GRAVITY				"gravity"
#define	QUIT				"quit"
#define	WFACTOR				"wfactor"
#define	DRAWGUN				"drawgun"
#define	CREATURE_JUMPSTEP	"jumpstep"
#define	GROUND_ACCFACTOR	"groundacc"
#define	GROUND_SLOWFACTOR	"groundslow"
#define	MAX_TRAVEL_VELOCITY	"maxspeed"
#define	AIR_ACCFACTOR		"airacc"
#define	AIR_SLOWFACTOR		"airslow"
#define	SOUNDSYSTEM			"soundsys"
#define	RESPAWNVAR			"respawn"
#define	GOD					"god"
#define	MONSTER				"monster"
#define	BOMB_SLOWFACTOR		"bombslow"
#define	MUSICVOL			"musicvol"
#define	SOUNDVOL			"soundvol"
#define	AUTOSWITCH			"autoswitch"
#define	FULLSCREEN			"fullscreen"
#define	COLORDEPTH			"colordepth"
#define	VIDEOMODE			"videomode"
#define	BLENDMODESRC		"bms"
#define	BLENDMODEDST		"bmd"
#define	STATUTBAR			"statutbar"
#define	ANIMSPEED			"animspeed"
#define	LIGHTMAP			"lightmap"
#define	CULLING				"cullface"
#define	RENDERENT			"renderent"
#define	DIRECTSTART			"directstart"
#define	EPSILON				"eps"
#define	VISENT				"visent"
#define	DEVELOPER			"developer"
#define	OFFSET				"offset"
#define	LEAF				"leaf"
#define	RECORD				"record"
#define	STOPRECORD			"stoprecord"
#define	PLAY				"play"
#define	STOPPLAY			"stopplay"
#define	NOSCRIPT			"noscript"
#define	GAMEKEY_UP			"gk_up"
#define	GAMEKEY_DOWN		"gk_down"
#define	GAMEKEY_LEFT		"gk_left"
#define	GAMEKEY_RIGHT		"gk_right"
#define	GAMEKEY_JUMP		"gk_jump"
#define	GAMEKEY_ATTACK		"gk_attack"
#define	GAMEKEY_AXE			"gk_axe"
#define	GAMEKEY_ARBALET		"gk_arbalet"
#define	GAMEKEY_BOMBGUN		"gk_bombgun"
#define	GAMEKEY_TROMBLON	"gk_tromblon"
#define	GAMEKEY_CROUCH		"gk_crouch"
#define	GAMEKEY_ACTION		"gk_action"
#define	GAMEKEY_RUN			"gk_run"
#define	RESETGAME			"resetgame"
#define	SAVECONFIG			"saveconfig"
#define	LOADCONFIG			"loadconfig"
#define	PICMIP				"picmip"
#define	GAMMA				"gamma"
#define	TFLAME				"tflame"
#define	GHOST				"ghost"
#define	TERMINATOR			"terminator"
#define	C_NETPORT			"port"
#define	C_NETIP				"ip"
#define	C_NETMAPID			"net_map"
#define	C_NETCLIENTNAME		"cl_name"
#define	C_NETSERVERNAME		"sv_name"
#define	C_NETPASSWORD		"password"
#define	C_NETGAMETYPE		"gametype"
#define	C_NETDEDICATED		"sv_dedicated"
#define	C_NOTEXTURES		"notextures"
#define	C_BESTSCORE1		"bestscore1"
#define	C_BESTSCORE2		"bestscore2"
#define	C_BESTSCORE3		"bestscore3"
#define	C_BESTSCORE4		"bestscore4"
#define	C_BESTSCORE5		"bestscore5"
#define	C_BESTSCORE6		"bestscore6"
#define	C_BESTSCORE7		"bestscore7"
#define	C_BESTSCORE8		"bestscore8"
#define	C_BESTSCORE9		"bestscore9"
#define	C_FOGMODE			"fogmode"
#define	C_FOGDENSITY		"fogdensity"


typedef void (*xcommand_t)();

typedef struct cmd_function_s
{
	struct cmd_function_s	*next;
	char					*name;
	xcommand_t				function;

} cmd_function_t;

//static	cmd_function_t	*cmd_functions;

typedef struct cvar_s
{
	char	name[255];
	char	string[255];
//	char	*name;
//	char	*string;
	float	value;
	struct	cvar_s *next;

} cvar_t;


hBool	cons_SaveConfig(char *path);
hBool	cons_LoadConfig(char *path);
hBool	m_InitConsole();
void	cons_UninitConsole();

hBool	m_InitConsoleBuffer();
void	m_ConsoleProcess();
void	m_ConsoleCmdAddInput();
void	m_ConsPrint(char *Text, ...);
hBool	m_ConsoleFindText(char *cmdtext);
void	m_ConsoleFindCommand(char *cmdtext);
void	m_DispConsoleText();
void	m_EnterConsoleString();
void	m_RegisterConsoleCmd();
void	m_FreeConsoleCmd();
void	m_cons_Version();
void	m_cons_DrawFace();
void	m_cons_DrawTris();
void	m_cons_DrawGun();
void	m_cons_DrawOne();
void	m_cons_DrawPortal();
void	m_cons_InvertMouse();
void	m_cons_DrawNormals();
void	m_cons_DrawBound();
void	m_cons_PlayerView();
void	m_cons_DrawCell();
void	m_cons_map();
void	m_cons_Noclip();
void	m_cons_JumpStep();
void	m_cons_groundacc();
void	m_cons_groundslow();
void	m_cons_ChangeFOV();
void	m_cons_ChangeGRAVITY();
void	m_cons_ChangeMaxSpeed();
void	m_cons_ChangeAirAcc();
void	m_cons_ChangeAirSlow();
void	m_cons_ChangeSound();
void	m_cons_ChangeGod();
void	m_cons_ChangeMonster();
void	m_cons_ChangeRespawn();
void	m_cons_ChangeBombslow();
void	m_cons_ChangeMusicVolume();
void	m_cons_ChangeSoundVolume();
void	m_cons_ChangeAutoswitch();
void	m_cons_ChangeFullscreen();
void	m_cons_ChangeColordepth();
void	m_cons_ChangeVideoMode();
void	m_cons_ChangeBlendSrc();
void	m_cons_ChangeBlendDst();
void	m_cons_ChangeStatutbar();
void	m_cons_ChangeAnimspeed();
void	m_cons_ChangeLightmap();
void	m_cons_ChangeDeveloper();
void	m_cons_ChangeCulling();
void	m_cons_ChangeRenderEnt();
void	m_cons_ChangeDirectStart();
void	m_cons_ChangeEpsilon();
void	m_cons_ChangeOffset();
void	m_cons_ChangeLeaf();
void	m_Cons_QuitGame();
void	m_cons_Record();
void	m_cons_StopRecord();
void	m_cons_Play();
void	m_cons_StopPlay();
void	m_cons_Noscript();
void	m_cons_ResetGame();
void	m_cons_SaveConfigToFile();
void	m_cons_LoadConfigFromFile();
void	m_cons_ChangePicmip();
void	m_cons_ChangeGamma();
void	m_cons_ChangeGhost();
void	m_cons_ChangeTerminator();




void	Cmd_AddCommand(char *cmd_name, xcommand_t function);
void	m_RegisterConsoleVar();
void	Cvar_RegisterVariable(cvar_t *variable);
cvar_t	*Cvar_FindVar (char *var_name);
float	Cvar_VariableValue (char *var_name);


#endif // CONSOLE_INCLUDED
