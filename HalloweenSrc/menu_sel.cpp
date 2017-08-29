
#include "alloween.cpp"

static int	anim_option = -1;
static int	anim_game = -1;
static int	anim_help = -1;
static int	anim_order = -1;


void menu_drawslot(int idslot)
{
#ifdef H_MAC
	if(MenuType[idslot].TextureID == txid_ORDERGAME_G) {
		return;
	}
#endif
	if(gMenu_InitFirstSel)
		return;
	disp_DrawSprite2Aplha(	MenuType[idslot].TextureID,
							MenuType[idslot].corner_x,
							MenuType[idslot].corner_y,
							MenuType[idslot].corner_lx,
							MenuType[idslot].corner_ly,
							true,
							MenuType[idslot].alpha);
}

void menu_draw_cadre(int x, int y, int w, int h)
{
	disp_DrawLine_2(x,y,x+w,y,2,GLRED);
	disp_DrawLine_2(x+w,y,x+w,y+h,2,GLRED);
	disp_DrawLine_2(x+w,y+h,x,y+h,2,GLRED);
	disp_DrawLine_2(x,y+h,x,y,2,GLRED);
}

void menu_ActivateSlot(int slot)
{
	int	i;

	if(slot<0)
		return;
	if(slot>=MAX_SLOT)
		return;

	if(MenuType[slot].Activated)
		return;

	// BIG HACK
	if(ProgramState == PS_PRES)
	{
		ds_PlaySound(7);
		goto no_sound;
	}

	// HACK
	if(ProgramState == PS_CONFIG)
	if(slot >= 5 && slot <= 9)
		goto no_sound;

	if(ProgramState == PS_CONFIG_SOUND)
	if(slot >= 5 && slot <= 9)
		goto no_sound;

	if(ProgramState == PS_CONFIG_INPUT)
	if(slot >= 17 && slot <= 21)
		goto no_sound;

	if(ProgramState == PS_CONFIG_VIDEO)
	if(slot >= 10 && slot <= 14)
		goto no_sound;

	if(ProgramState == PS_CONFIG_GAMEOPTION)
	if(slot >= 4 && slot <= 8)
		goto no_sound;

	if(ProgramState == PS_SETSLOTGAME)
	if(slot >= 9 && slot <= 13)
		goto no_sound;

	if(ProgramState == PS_HELP)
	if(slot >= 1 && slot <= 5)
		goto no_sound;

	if(ProgramState == PS_ORDER)
	if(slot >= 1 && slot <= 5)
		goto no_sound;

//	if(ProgramState == PS_SELECTMAP)
//	if((slot >= 0 && slot < game_NbMaps) || (game_NbMaps > game_NbMaps))
//		goto no_sound;

		ds_PlaySound(3);

no_sound:

	for(i=0 ; i<MAX_SLOT ; i++)
		MenuType[i].Activated = false;
	
	MenuType[slot].Activated = true;
}

void menu_SlotProcess(int slot, int txid_NOTSEL, int txid_SEL, char *text, int line, int fontsize)
{
	float	cursor2_x;
	float	cursor2_y;


	if(slot<0)
		return;
	if(slot>=MAX_SLOT)
		return;

	gMenu_InitFirstSel = false;

	if(MenuType[slot].alpha < 1)
	{
		MenuType[slot].alpha += gFrameTime_msec * 0.002f;
		if(MenuType[slot].alpha > 1)
		{
			MenuType[slot].alpha = 1;
		}
		gAlphaFont = MenuType[slot].alpha;
	}
	

	if(developer.value)
		menu_draw_cadre(MenuType[slot].menu_x,MenuType[slot].menu_y,MenuType[slot].menu_w,MenuType[slot].menu_h);

	cursor2_x = cursor_x * 1.25f;
	cursor2_y = cursor_y * 1.25f;

	if(	(cursor2_x >= MenuType[slot].menu_x && cursor2_x < MenuType[slot].menu_x+MenuType[slot].menu_w) &&
		(cursor2_y >= MenuType[slot].menu_y && cursor2_y < MenuType[slot].menu_y+MenuType[slot].menu_h) )
	{
		if(text)
			g_DispCenterString(fontsize,line,text);
		MenuType[slot].TextureID = txid_SEL;
		menu_ActivateSlot(slot);
	}
	else
	{
		MenuType[slot].TextureID = txid_NOTSEL;
		MenuType[slot].Activated=false;
	}
}

void menu_SelectProcess_YourWin()
{
	menu_SlotProcess(0,txid_BUTTONBACK,txid_BUTTONBACK2,NULL,0,0);
}

void menu_SelectProcess_PAUSED()
{
	menu_SlotProcess(0,txid_RESUME,txid_RESUME2,NULL,0,0);
	menu_SlotProcess(1,txid_CONFIG3,txid_CONFIG4,NULL,0,0);
	menu_SlotProcess(2,txid_LEAVE,txid_LEAVE2,NULL,0,0);
	menu_SlotProcess(3,txid_EXIT,txid_EXIT2,NULL,0,0);
}

void menu_SelectProcess_PRES()
{
//	static int	anim_game = -1;
//	static int	anim_help = -1;
//	static int	anim_option = -1;
//	static int	anim_order = -1;
//	static int	anim_quit = -1;
	int		id1;
	int		id2;
	int		id3;
	int		id4;
	int		id5;

	id1 = int_GetButonId(&anim_game,24,0);
	id2 = int_GetButonId(&anim_help,14,0);
	id3 = int_GetButonId(&anim_option,13,0);
	id4 = int_GetButonId(&anim_order,18,0);
	id5 = int_GetButonId(&anim_order,24,0);

	menu_SlotProcess(0,txid_STARTGAME,txid_GAMEANIM[id1],NULL,0,0);
	menu_SlotProcess(1,txid_HELPORDER,txid_HELPANIM[id2],NULL,0,0);
	menu_SlotProcess(2,txid_CONFIG,txid_OPTIONANIM[id3],NULL,0,0);
	menu_SlotProcess(3,txid_ORDERGAME,txid_ORDERANIM[id4],NULL,0,0);
	menu_SlotProcess(4,txid_QUITGAME,txid_QUITANIM[id5],NULL,0,0);
}

void menu_SelectProcess_CONFIG()
{
	int		id3;

	id3 = int_GetButonId(&anim_option,13,0);

	menu_SlotProcess(0,txid_SOUNDCONFIG,txid_SOUNDCONFIG2,NULL,0,0);
	menu_SlotProcess(1,txid_GAMEOPTIONCONFIG,txid_GAMEOPTIONCONFIG2,NULL,0,0);
	menu_SlotProcess(2,txid_INPUTCONFIG,txid_INPUTCONFIG2,NULL,0,0);
	menu_SlotProcess(3,txid_VIDEOCONFIG,txid_VIDEOCONFIG2,NULL,0,0);
	menu_SlotProcess(4,txid_BUTTONBACK,txid_BUTTONBACK2,NULL,0,0);

	menu_SlotProcess(5,txid_STARTGAME_G,txid_STARTGAME_G,NULL,0,0);
	menu_SlotProcess(6,txid_HELPORDER_G,txid_HELPORDER_G,NULL,0,0);
	menu_SlotProcess(7,txid_OPTIONANIM[id3],txid_OPTIONANIM[id3],NULL,0,0);
	menu_SlotProcess(8,txid_ORDERGAME_G,txid_ORDERGAME_G,NULL,0,0);
	menu_SlotProcess(9,txid_QUITGAME_G,txid_QUITGAME_G,NULL,0,0);
}

void menu_SelectProcess_CONFIG_SOUND()
{
	int		id3;

	id3 = int_GetButonId(&anim_option,13,0);

	menu_SlotProcess(0,txid_BUTTON_M,txid_BUTTON_M_SEL,"Decrease music volume",39,12);
	menu_SlotProcess(1,txid_BUTTON_P,txid_BUTTON_P_SEL,"Increase music volume",39,12);
	menu_SlotProcess(2,txid_BUTTON_M,txid_BUTTON_M_SEL,"Decrease sound volume",39,12);
	menu_SlotProcess(3,txid_BUTTON_P,txid_BUTTON_P_SEL,"Increase sound volume",39,12);
	menu_SlotProcess(4,txid_BUTTONBACK,txid_BUTTONBACK2,NULL,0,0);

	menu_SlotProcess(5,txid_STARTGAME_G,txid_STARTGAME_G,NULL,0,0);
	menu_SlotProcess(6,txid_HELPORDER_G,txid_HELPORDER_G,NULL,0,0);
	menu_SlotProcess(7,txid_OPTIONANIM[id3],txid_OPTIONANIM[id3],NULL,0,0);
	menu_SlotProcess(8,txid_ORDERGAME_G,txid_ORDERGAME_G,NULL,0,0);
	menu_SlotProcess(9,txid_QUITGAME_G,txid_QUITGAME_G,NULL,0,0);
}

void menu_SelectProcess_CONFIG_INPUT()
{
	int		id3;

	id3 = int_GetButonId(&anim_option,13,0);

	menu_SlotProcess(0,txid_BUTTON,txid_BUTTON_P_SEL,"Toggle Mouse Invert/Regular",39,12);
	menu_SlotProcess(1,txid_BUTTON_M,txid_BUTTON_M_SEL,"Decrease Mouse sensibility",39,12);
	menu_SlotProcess(2,txid_BUTTON_P,txid_BUTTON_P_SEL,"Increase Mouse sensibility",39,12);
	menu_SlotProcess(3,txid_BUTTONBACK,txid_BUTTONBACK2,NULL,0,0);
	menu_SlotProcess(4,txid_BUTTON,txid_BUTTON_P_SEL,"CLICK THEN PRESS A KEY",39,12);
	menu_SlotProcess(5,txid_BUTTON,txid_BUTTON_P_SEL,"CLICK THEN PRESS A KEY",39,12);
	menu_SlotProcess(6,txid_BUTTON,txid_BUTTON_P_SEL,"CLICK THEN PRESS A KEY",39,12);
	menu_SlotProcess(7,txid_BUTTON,txid_BUTTON_P_SEL,"CLICK THEN PRESS A KEY",39,12);
	menu_SlotProcess(8,txid_BUTTON,txid_BUTTON_P_SEL,"CLICK THEN PRESS A KEY",39,12);
	menu_SlotProcess(9,txid_BUTTON,txid_BUTTON_P_SEL,"CLICK THEN PRESS A KEY",39,12);
	menu_SlotProcess(10,txid_BUTTON,txid_BUTTON_P_SEL,"CLICK THEN PRESS A KEY",39,12);
	menu_SlotProcess(11,txid_BUTTON,txid_BUTTON_P_SEL,"CLICK THEN PRESS A KEY",39,12);
	menu_SlotProcess(12,txid_BUTTON,txid_BUTTON_P_SEL,"CLICK THEN PRESS A KEY",39,12);
	menu_SlotProcess(13,txid_BUTTON,txid_BUTTON_P_SEL,"CLICK THEN PRESS A KEY",39,12);
	menu_SlotProcess(14,txid_BUTTON,txid_BUTTON_P_SEL,"CLICK THEN PRESS A KEY",39,12);
	menu_SlotProcess(15,txid_BUTTON,txid_BUTTON_P_SEL,"CLICK THEN PRESS A KEY",39,12);
	menu_SlotProcess(16,txid_BUTTON,txid_BUTTON_P_SEL,"CLICK THEN PRESS A KEY",39,12);

	menu_SlotProcess(17,txid_STARTGAME_G,txid_STARTGAME_G,NULL,0,0);
	menu_SlotProcess(18,txid_HELPORDER_G,txid_HELPORDER_G,NULL,0,0);
	menu_SlotProcess(19,txid_OPTIONANIM[id3],txid_OPTIONANIM[id3],NULL,0,0);
	menu_SlotProcess(20,txid_ORDERGAME_G,txid_ORDERGAME_G,NULL,0,0);
	menu_SlotProcess(21,txid_QUITGAME_G,txid_QUITGAME_G,NULL,0,0);
}

void menu_SelectProcess_CONFIG_VIDEO()
{
	int		id3;

	id3 = int_GetButonId(&anim_option,13,0);
	
	int slotId = 0;
	
#ifdef H_MAC
	slotId = 7;
	menu_SlotProcess(slotId++,txid_BUTTON_M,txid_BUTTON_M_SEL,"Decrease brightness",39,12);
	menu_SlotProcess(slotId++,txid_BUTTON_P,txid_BUTTON_P_SEL,"Increase brightness",39,12);
	menu_SlotProcess(slotId++,txid_BUTTONBACK,txid_BUTTONBACK2,NULL,0,0);
	
	menu_SlotProcess(slotId++,txid_STARTGAME_G,txid_STARTGAME_G,NULL,0,0);
	menu_SlotProcess(slotId++,txid_HELPORDER_G,txid_HELPORDER_G,NULL,0,0);
	menu_SlotProcess(slotId++,txid_OPTIONANIM[id3],txid_OPTIONANIM[id3],NULL,0,0);
	menu_SlotProcess(slotId++,txid_ORDERGAME_G,txid_ORDERGAME_G,NULL,0,0);
	menu_SlotProcess(slotId++,txid_QUITGAME_G,txid_QUITGAME_G,NULL,0,0);

#else
	
	menu_SlotProcess(slotId++,txid_BUTTON,txid_BUTTON_P_SEL,"Toggle Fullscreen/windowed video mode",39,12);
	menu_SlotProcess(slotId++,txid_BUTTON_M,txid_BUTTON_M_SEL,"Change video mode",39,12);
	menu_SlotProcess(slotId++,txid_BUTTON_P,txid_BUTTON_P_SEL,"Change video mode",39,12);
	menu_SlotProcess(slotId++,txid_BUTTON,txid_BUTTON_P_SEL,"Change color depth",39,12);

	
	menu_SlotProcess(slotId++,txid_BUTTON,txid_BUTTON_P_SEL,"Change lightning mode",39,12);
	menu_SlotProcess(slotId++,txid_BUTTON_M,txid_BUTTON_M_SEL,"Decrease texture quality",39,12);
	menu_SlotProcess(slotId++,txid_BUTTON_P,txid_BUTTON_P_SEL,"Increase texture quality",39,12);
	menu_SlotProcess(slotId++,txid_BUTTON_M,txid_BUTTON_M_SEL,"Decrease brightness",39,12);
	menu_SlotProcess(slotId++,txid_BUTTON_P,txid_BUTTON_P_SEL,"Increase brightness",39,12);
	menu_SlotProcess(slotId++,txid_BUTTONBACK,txid_BUTTONBACK2,NULL,0,0);

	menu_SlotProcess(slotId++,txid_STARTGAME_G,txid_STARTGAME_G,NULL,0,0);
	menu_SlotProcess(slotId++,txid_HELPORDER_G,txid_HELPORDER_G,NULL,0,0);
	menu_SlotProcess(slotId++,txid_OPTIONANIM[id3],txid_OPTIONANIM[id3],NULL,0,0);
	menu_SlotProcess(slotId++,txid_ORDERGAME_G,txid_ORDERGAME_G,NULL,0,0);
	menu_SlotProcess(slotId++,txid_QUITGAME_G,txid_QUITGAME_G,NULL,0,0);

	
	menu_SlotProcess(slotId++,txid_BUTTON,txid_BUTTON_P_SEL,"Apply new settings (be patient)",39,12);
#endif
}

void menu_SelectProcess_CONFIG_GAMEOPTION()
{
	int		id3;

	id3 = int_GetButonId(&anim_option,13,0);

	menu_SlotProcess(0,txid_BUTTON,txid_BUTTON_P_SEL,"Toggle autoswitch weapon mode",39,12);
	menu_SlotProcess(1,txid_BUTTON,txid_BUTTON_P_SEL,"Toggle Draw/NotDraw weapon",39,12);
	menu_SlotProcess(2,txid_BUTTON,txid_BUTTON_P_SEL,"Toggle Draw/NotDraw FPS",39,12);
	menu_SlotProcess(3,txid_BUTTONBACK,txid_BUTTONBACK2,NULL,0,0);

	menu_SlotProcess(4,txid_STARTGAME_G,txid_STARTGAME_G,NULL,0,0);
	menu_SlotProcess(5,txid_HELPORDER_G,txid_HELPORDER_G,NULL,0,0);
	menu_SlotProcess(6,txid_OPTIONANIM[id3],txid_OPTIONANIM[id3],NULL,0,0);
	menu_SlotProcess(7,txid_ORDERGAME_G,txid_ORDERGAME_G,NULL,0,0);
	menu_SlotProcess(8,txid_QUITGAME_G,txid_QUITGAME_G,NULL,0,0);
}

void menu_SelectProcess_SelectMAP()
{
	menu_SlotProcess(0,txid_BUTTONBACK,txid_BUTTONBACK2,"",28,16);
	menu_SlotProcess(1,txid_BUTTONGO,txid_BUTTONGO2,"",28,16);
}

void menu_SelectProcess_StartMAP()
{
	menu_SlotProcess(0,txid_BUTTONBACK,txid_BUTTONBACK2,"",28,16);
	menu_SlotProcess(1,txid_BUTTONGO,txid_BUTTONGO2,"",28,16);
}

void menu_SelectProcess_HELP()
{
	int		id2;
	id2 = int_GetButonId(&anim_help,14,0);

	menu_SlotProcess(0,txid_BUTTONBACK,txid_BUTTONBACK2,"",28,16);

	menu_SlotProcess(1,txid_STARTGAME_G,txid_STARTGAME_G,NULL,39,12);
	menu_SlotProcess(2,txid_HELPANIM[id2],txid_HELPANIM[id2],NULL,39,12);
	menu_SlotProcess(3,txid_OPTION_G,txid_OPTION_G,NULL,39,12);
	menu_SlotProcess(4,txid_ORDERGAME_G,txid_ORDERGAME_G,NULL,39,12);
	menu_SlotProcess(5,txid_QUITGAME_G,txid_QUITGAME_G,NULL,39,12);
}

void menu_SelectProcess_ORDER()
{
	int		id4;
	id4 = int_GetButonId(&anim_order,18,0);

	menu_SlotProcess(0,txid_BUTTONBACK,txid_BUTTONBACK2,"",28,16);

	menu_SlotProcess(1,txid_STARTGAME_G,txid_STARTGAME_G,NULL,39,12);
	menu_SlotProcess(2,txid_HELPORDER_G,txid_HELPORDER_G,NULL,39,12);
	menu_SlotProcess(3,txid_OPTION_G,txid_OPTION_G,NULL,39,12);
	menu_SlotProcess(4,txid_ORDERANIM[id4],txid_ORDERANIM[id4],NULL,39,12);
	menu_SlotProcess(5,txid_QUITGAME_G,txid_QUITGAME_G,NULL,39,12);
}

void menu_SelectProcess_BEGINGAME()
{
	menu_SlotProcess(0,txid_EASY,txid_EASY_G,"Easy Mode : Rookies start here",39,12);
	menu_SlotProcess(1,txid_NORMAL,txid_NORMAL_G,"Medium Mode : Now we are talking",39,12);
	menu_SlotProcess(2,txid_HARD,txid_HARD_G,"Hard Mode : Experts only",39,12);
	menu_SlotProcess(3,txid_BUTTONBACK,txid_BUTTONBACK2,NULL,0,0);
}

void menu_SelectProcess_YESNO()
{
	menu_SlotProcess(0,txid_YES,txid_YES_G,"Yes, I'm sure!",39,12);
	menu_SlotProcess(1,txid_NO,txid_NO_G,"No, I don't know..",39,12);
	menu_SlotProcess(2,txid_BUTTONBACK,txid_BUTTONBACK2,NULL,0,0);
}

void menu_SelectProcess_SETSLOTGAME()
{
	char	fullpath[256];
	int		i;
	int		diff;
	FILE	*file;
	int		id1;
	char	*p;

	id1 = int_GetButonId(&anim_game,24,0);

	for(i=0 ; i<4 ; i++)
	{
#ifdef H_MAC
		sprintf(fullpath, "%s/game%d.svg", KMiscTools::currentDocHomePath, i+1);
#else
		sprintf(fullpath,"%s%sgame%d.svg",SYSDIR,SVGDIR,i+1);
#endif
		
#ifdef H_MAC
		p = KMiscTools::makeFilePath(fullpath);
		file = fopen(p,"r");
#else
		file = fopen(fullpath,"r");
#endif
		
		if(!file)
		{
			menu_SlotProcess(i,txid_EMPTY,txid_EMPTY_G,"Create/Load a game in this slot",39,12);
		}
		else
		{
			diff = game_GetSlotDifficulty(file);
			fclose(file);
			switch(diff)
			{
				case 0:
				menu_SlotProcess(i,txid_EASY,txid_EASY_G,"Create/Load a game in this slot",39,12);
				break;

				case 2:
				menu_SlotProcess(i,txid_HARD,txid_HARD_G,"Create/Load a game in this slot",39,12);
				break;

				default:
				menu_SlotProcess(i,txid_NORMAL,txid_NORMAL_G,"Create/Load a game in this slot",39,12);
				break;
			}
		}
	}

//	menu_SlotProcess(0,txid_EASY,txid_EASY_G,"Create/Load a game in slot1",26,16);
//	menu_SlotProcess(1,txid_EASY,txid_EASY_G,"Create/Load a game in slot2",26,16);
//	menu_SlotProcess(2,txid_EASY,txid_EASY_G,"Create/Load a game in slot3",26,16);
//	menu_SlotProcess(3,txid_EASY,txid_EASY_G,"Create/Load a game in slot4",26,16);

	menu_SlotProcess(4,txid_TDM,txid_TDM_G,"Delete slot1",39,12);
	menu_SlotProcess(5,txid_TDM,txid_TDM_G,"Delete slot2",39,12);
	menu_SlotProcess(6,txid_TDM,txid_TDM_G,"Delete slot3",39,12);
	menu_SlotProcess(7,txid_TDM,txid_TDM_G,"Delete slot4",39,12);

	menu_SlotProcess(8,txid_BUTTONBACK,txid_BUTTONBACK2,NULL,0,0);

	menu_SlotProcess(9,txid_GAMEANIM[id1],txid_GAMEANIM[id1],NULL,0,0);
	menu_SlotProcess(10,txid_HELPORDER_G,txid_HELPORDER_G,NULL,0,0);
	menu_SlotProcess(11,txid_OPTION_G,txid_OPTION_G,NULL,0,0);
	menu_SlotProcess(12,txid_ORDERGAME_G,txid_ORDERGAME_G,NULL,0,0);
	menu_SlotProcess(13,txid_QUITGAME_G,txid_QUITGAME_G,NULL,0,0);
}


