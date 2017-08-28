
#include "alloween.cpp"

hBool	gMenu_InitFirstSel;


void menu_InitSlot(int slot, int x1, int y1, int x2, int y2, int x3, int y3, int texid, float alpha)
{
	int		x4;
	int		y4;
	int		offset_y;

	if(slot<0)
		return;
	if(slot>=MAX_SLOT)
		return;

	gMenu_InitFirstSel = true;

	offset_y = y2 - y3;

	x4 = x1;
	y4 = y1 + offset_y;

	gAlphaFont = 0;

	MenuType[slot].alpha = alpha;
	MenuType[slot].TextureID = texid;
	MenuType[slot].Activated=false;
	MenuType[slot].corner_x = x1; 
	MenuType[slot].corner_y = y1; 
	MenuType[slot].corner_lx = x2; 
	MenuType[slot].corner_ly = y2;
	MenuType[slot].menu_w = x3;
	MenuType[slot].menu_h = y3;
	MenuType[slot].menu_x = x4;
	MenuType[slot].menu_y = y4;
}

void menu_InitPaused()
{
	//saved_ProgramState = ProgramState;
	ProgramState = PS_PAUSEDGAME;

	menu_InitSlot(0,240,331,320,80,320,80,txid_RESUME,0);
	menu_InitSlot(1,240,243,320,80,320,80,txid_CONFIG3,0);
	menu_InitSlot(2,240,156,320,80,320,80,txid_LEAVE,0);
	menu_InitSlot(3,240,68,320,80,320,80,txid_EXIT,0);
}

void menu_InitPresentation()
{
	menu_InitSlot(0,142,208,256,128,140,60,txid_STARTGAME,1);
	menu_InitSlot(1,172,142,256,128,140,60,txid_HELPORDER,1);
	menu_InitSlot(2,136,74,256,128,140,60,txid_CONFIG,1);
	menu_InitSlot(3,153,10,256,128,140,50,txid_ORDERGAME,1);
	menu_InitSlot(4,178,-44,256,128,140,50,txid_QUITGAME,1);
}

void menu_InitConfig()
{
	menu_InitSlot(0,380,246,512,64,300,40,txid_SOUNDCONFIG,0);
	menu_InitSlot(1,380,168,512,64,280,40,txid_GAMEOPTIONCONFIG,0);
	menu_InitSlot(2,380,90,512,64,230,40,txid_INPUTCONFIG,0);
	menu_InitSlot(3,380,12,512,64,160,40,txid_VIDEOCONFIG,0);
	menu_InitSlot(4,0,20,120,60,120,160,txid_BUTTONBACK,0);

	menu_InitSlot(5,142,208,256,128,140,60,txid_STARTGAME_G,1);
	menu_InitSlot(6,172,142,256,128,140,60,txid_HELPORDER_G,1);
	menu_InitSlot(7,136,74,256,128,140,60,txid_CONFIG,1);
	menu_InitSlot(8,153,10,256,128,140,50,txid_ORDERGAME_G,1);
	menu_InitSlot(9,178,-44,256,128,140,50,txid_QUITGAME_G,1);
}

void menu_InitConfig_SOUND()
{
	menu_InitSlot(0,605,218,20,20,20,20,txid_BUTTON_M,0);
	menu_InitSlot(1,645,218,20,20,20,20,txid_BUTTON_P,0);
	menu_InitSlot(2,605,198,20,20,20,20,txid_BUTTON_M,0);
	menu_InitSlot(3,645,198,20,20,20,20,txid_BUTTON_P,0);
	menu_InitSlot(4,0,20,120,60,120,60,txid_BUTTONBACK,0);

	menu_InitSlot(5,142,208,256,128,140,60,txid_STARTGAME_G,1);
	menu_InitSlot(6,172,142,256,128,140,60,txid_HELPORDER_G,1);
	menu_InitSlot(7,136,74,256,128,140,60,txid_CONFIG,1);
	menu_InitSlot(8,153,10,256,128,140,50,txid_ORDERGAME_G,1);
	menu_InitSlot(9,178,-44,256,128,140,50,txid_QUITGAME_G,1);
}

void menu_InitConfig_INPUT()
{
	menu_InitSlot(0,525,358,20,20,20,20,txid_BUTTON,0);
	menu_InitSlot(1,525,338,20,20,20,20,txid_BUTTON_M,0);
	menu_InitSlot(2,565,338,20,20,20,20,txid_BUTTON_P,0);
	menu_InitSlot(3,0,20,120,60,120,60,txid_BUTTONBACK,0);

	menu_InitSlot(4,450,298,20,20,20,20,txid_BUTTON,0);
	menu_InitSlot(5,450,278,20,20,20,20,txid_BUTTON,0);
	menu_InitSlot(6,450,258,20,20,20,20,txid_BUTTON,0);
	menu_InitSlot(7,450,238,20,20,20,20,txid_BUTTON,0);
	menu_InitSlot(8,450,218,20,20,20,20,txid_BUTTON,0);
	menu_InitSlot(9,450,198,20,20,20,20,txid_BUTTON,0);
	menu_InitSlot(10,450,178,20,20,20,20,txid_BUTTON,0);
	menu_InitSlot(11,450,158,20,20,20,20,txid_BUTTON,0);
	menu_InitSlot(12,450,138,20,20,20,20,txid_BUTTON,0);
	menu_InitSlot(13,450,118,20,20,20,20,txid_BUTTON,0);
	menu_InitSlot(14,450,98,20,20,20,20,txid_BUTTON,0);
	menu_InitSlot(15,450,78,20,20,20,20,txid_BUTTON,0);
	menu_InitSlot(16,450,58,20,20,20,20,txid_BUTTON,0);

	menu_InitSlot(17,142,208,256,128,140,60,txid_STARTGAME_G,1);
	menu_InitSlot(18,172,142,256,128,140,60,txid_HELPORDER_G,1);
	menu_InitSlot(19,136,74,256,128,140,60,txid_CONFIG,1);
	menu_InitSlot(20,153,10,256,128,140,50,txid_ORDERGAME_G,1);
	menu_InitSlot(21,178,-44,256,128,140,50,txid_QUITGAME_G,1);
}

void menu_InitConfig_VIDEO()
{
	new_fullscreen = (int)fullscreen.value;
	new_videomode = (int)videomode.value;
	new_colordepth = (int)colordepth.value;
	
#ifdef H_MAC
	menu_InitSlot(7,385,138,20,20,20,20,txid_BUTTON_M,0);
	menu_InitSlot(8,425,138,20,20,20,20,txid_BUTTON_P,0);
	menu_InitSlot(9,0,20,120,60,120,60,txid_BUTTONBACK,0);
#else
	menu_InitSlot(0,425,238,20,20,20,20,txid_BUTTON,0);
	menu_InitSlot(1,385,218,20,20,20,20,txid_BUTTON_M,0);
	menu_InitSlot(2,425,218,20,20,20,20,txid_BUTTON_P,0);
	menu_InitSlot(3,425,198,20,20,20,20,txid_BUTTON,0);
	menu_InitSlot(4,425,178,20,20,20,20,txid_BUTTON,0);
	menu_InitSlot(5,385,158,20,20,20,20,txid_BUTTON_M,0);
	menu_InitSlot(6,425,158,20,20,20,20,txid_BUTTON_P,0);
	menu_InitSlot(7,385,138,20,20,20,20,txid_BUTTON_M,0);
	menu_InitSlot(8,425,138,20,20,20,20,txid_BUTTON_P,0);
	menu_InitSlot(15,490,90,120,35,120,35,txid_BUTTON,0);
	menu_InitSlot(9,0,20,120,60,120,60,txid_BUTTONBACK,0);
#endif
	
	menu_InitSlot(10,142,208,256,128,140,60,txid_STARTGAME_G,1);
	menu_InitSlot(11,172,142,256,128,140,60,txid_HELPORDER_G,1);
	menu_InitSlot(12,136,74,256,128,140,60,txid_CONFIG,1);
	menu_InitSlot(13,153,10,256,128,140,50,txid_ORDERGAME_G,1);
	menu_InitSlot(14,178,-44,256,128,140,50,txid_QUITGAME_G,1);
}

void menu_InitConfig_GAMEOPTION()
{
	menu_InitSlot(0,425,218,20,20,20,20,txid_BUTTON,0);
	menu_InitSlot(1,425,198,20,20,20,20,txid_BUTTON,0);
	menu_InitSlot(2,425,178,20,20,20,20,txid_BUTTON,0);
	menu_InitSlot(3,0,20,120,60,120,60,txid_BUTTONBACK,0);

	menu_InitSlot(4,142,208,256,128,140,60,txid_STARTGAME_G,1);
	menu_InitSlot(5,172,142,256,128,140,60,txid_HELPORDER_G,1);
	menu_InitSlot(6,136,74,256,128,140,60,txid_CONFIG,1);
	menu_InitSlot(7,153,10,256,128,140,50,txid_ORDERGAME_G,1);
	menu_InitSlot(8,178,-44,256,128,140,50,txid_QUITGAME_G,1);
}

void menu_InitSelectMAP()
{
	menu_InitSlot(0,10,4,120,60,120,60,txid_BUTTONBACK,0);
	menu_InitSlot(1,670,4,120,60,120,60,txid_BUTTONGO,0);
}

void menu_InitYouWin()
{
	menu_InitSlot(0,0,20,120,60,120,60,txid_BUTTONBACK,0);
}

void menu_InitStartMAP()
{
	menu_InitSlot(0,10,4,120,60,120,60,txid_BUTTONBACK,0);
	menu_InitSlot(1,670,4,120,60,120,60,txid_BUTTONGO,0);
}

void menu_InitHELP()
{
	menu_InitSlot(0,0,20,120,60,120,60,txid_BUTTONBACK,0);

	menu_InitSlot(1,142,208,256,128,140,60,txid_STARTGAME_G,1);
	menu_InitSlot(2,172,142,256,128,140,60,txid_HELPORDER,1);
	menu_InitSlot(3,136,74,256,128,140,60,txid_OPTION_G,1);
	menu_InitSlot(4,153,10,256,128,140,50,txid_ORDERGAME_G,1);
	menu_InitSlot(5,178,-44,256,128,140,50,txid_QUITGAME_G,1);
}

void menu_InitORDER()
{
	menu_InitSlot(0,0,4,120,60,120,60,txid_BUTTONBACK,0);

	menu_InitSlot(1,142,208,256,128,140,60,txid_STARTGAME_G,1);
	menu_InitSlot(2,172,142,256,128,140,60,txid_HELPORDER_G,1);
	menu_InitSlot(3,136,74,256,128,140,60,txid_OPTION_G,1);
	menu_InitSlot(4,153,10,256,128,140,50,txid_ORDERGAME,1);
	menu_InitSlot(5,178,-44,256,128,140,50,txid_QUITGAME_G,1);
}

void menu_InitConfig_BEGINGAME()
{
	menu_InitSlot(0,325,250,256,64,200,40,txid_EASY,0);
	menu_InitSlot(1,325,185,256,64,200,40,txid_NORMAL,0);
	menu_InitSlot(2,325,120,256,64,200,40,txid_HARD,0);
	menu_InitSlot(3,0,20,120,60,120,60,txid_BUTTONBACK,0);
}

void menu_InitConfig_YESNO()
{
	menu_InitSlot(0,355,250,100,42,100,42,txid_YES,0);
	menu_InitSlot(1,355,150,100,42,100,42,txid_NO,0);
	menu_InitSlot(2,0,20,120,60,120,60,txid_BUTTONBACK,0);
}

void menu_InitConfig_SETSLOTGAME()
{
	menu_InitSlot(0,380,246,256,64,200,40,txid_EASY,0);
	menu_InitSlot(1,380,168,256,64,200,40,txid_EASY,0);
	menu_InitSlot(2,380,90,256,64,200,40,txid_EASY,0);
	menu_InitSlot(3,380,12,256,64,200,40,txid_EASY,0);

	menu_InitSlot(4,598,250,64,64,32,48,txid_TDM,0);
	menu_InitSlot(5,598,172,64,64,32,48,txid_TDM,0);
	menu_InitSlot(6,598,94,64,64,32,48,txid_TDM,0);
	menu_InitSlot(7,598,16,64,64,32,48,txid_TDM,0);

	menu_InitSlot(8,0,20,120,60,120,60,txid_BUTTONBACK,0);

	menu_InitSlot(9,142,208,256,128,140,60,txid_STARTGAME_G,1);
	menu_InitSlot(10,172,142,256,128,140,60,txid_HELPORDER_G,1);
	menu_InitSlot(11,136,74,256,128,140,60,txid_OPTION_G,1);
	menu_InitSlot(12,153,10,256,128,140,50,txid_ORDERGAME_G,1);
	menu_InitSlot(13,178,-44,256,128,140,50,txid_QUITGAME_G,1);
}
