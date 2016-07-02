
#ifndef MENU___
#define MENU___

#include "alloween.h"

#define	MAX_SLOT	24

typedef struct menutag
{
	int		TextureID;
	int		corner_x;
	int		corner_y;
	int		corner_lx;
	int		corner_ly;
	int		menu_x;
	int		menu_y;
	int		menu_w;
	int		menu_h;
	float	alpha;
	hBool	Activated;

} MENU;


void	menu_DrawYouWinGame();
void	menu_ClickedA_YouWin();
void	menu_DisplayDefaultMap();
void	menu_DispLoadMapSplash();
void	DrawPausedGame();
void	menu_ClickedA_PAUSED();
void	menu_ClickedB_PAUSED();
void	DrawGameTitle();
void	menu_ClickedA_PRES();
void	DrawGameConfig();
void	menu_ClickedA_CONFIG();
void	menu_ClickedB_CONFIG();
void	DrawGameConfig_INPUT();
void	DrawGameConfig_GAMEOPTION();
void	DrawGameConfig_VIDEO();
void	DrawGameConfig_SOUND();
void	menu_ClickedA_CONFIG_VIDEO();
void	menu_ClickedA_CONFIG_SOUND();
void	menu_ClickedA_CONFIG_INPUT();
void	menu_ClickedA_CONFIG_GAMEOPTION();
void	DrawGameConfig_SelectMAP();
void	menu_ClickedA_SelectMAP();
void	DrawGameConfig_StartMAP();
void	menu_ClickedA_StartMAP();
void	DrawGameConfig_HELP();
void	menu_ClickedA_HELP();
void	DrawGameConfig_ORDER();
void	menu_ClickedA_ORDER();
void	DrawGameMenu_BEGINGAME();
void	menu_ClickedA_BEGINGAME();
void	menu_ClickedB_BEGINGAME();
void	DrawGameMenu_YESNO();
void	menu_ClickedA_YESNO();
void	DrawGameMenu_SETSLOTGAME();
void	menu_ClickedA_SETSLOTGAME();

void	DrawGameConfig_StartMAP_goals();


#endif // MENU___
