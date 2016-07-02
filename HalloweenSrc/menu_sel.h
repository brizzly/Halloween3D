
#ifndef MENUSELECT___
#define MENUSELECT___

#include "alloween.h"

void	menu_drawslot(int idslot);
void	menu_draw_cadre(int x, int y, int w, int h);
void	menu_ActivateSlot(int slot);
void	menu_SlotProcess(int slot, int txid_NOTSEL, int txid_SEL, char *text, int line, int fontsize);

void	menu_SelectProcess_YourWin();
void	menu_SelectProcess_PAUSED();
void	menu_SelectProcess_PRES();
void	menu_SelectProcess_CONFIG();
void	menu_SelectProcess_CONFIG_SOUND();
void	menu_SelectProcess_CONFIG_INPUT();
void	menu_SelectProcess_CONFIG_VIDEO();
void	menu_SelectProcess_CONFIG_GAMEOPTION();
void	menu_SelectProcess_SelectMAP();
void	menu_SelectProcess_StartMAP();
void	menu_SelectProcess_HELP();
void	menu_SelectProcess_ORDER();
void	menu_SelectProcess_BEGINGAME();
void	menu_SelectProcess_YESNO();
void	menu_SelectProcess_SETSLOTGAME();



#endif // MENUSELECT___
