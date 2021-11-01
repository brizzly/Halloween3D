
#include "alloween.h"  //was .cpp ?? by farox

#ifdef H_MAC
#include "KInput.h"
#endif

float	cursor_x=0;
float	cursor_y=0;
hBool	ResumingAlloweed=false;
hBool	gDrawCursor=true;
int		gGameSlotToProcess=1;
float	gDemoReminderTime = 0;
int		new_fullscreen, new_videomode, new_colordepth;


MENU	MenuType[MAX_SLOT];



void menu_DrawYouWinGame()
{
	menu_drawslot(0);
	disp_DrawSprite(txid_CURSOR,(int)cursor_x-16,(int)cursor_y-16,32,32,true);
}

void menu_ClickedA_YouWin()
{
	if(MenuType[0].Activated)
	{
		ds_PlaySound(4);
		if(game_MapID == 8)
		{
			game_GameEnded();
		}
		else
		{
			game_WinTime = GAME_WINTIME;
		}
	}
}

void DrawPausedGame()
{
	int		i;

	for(i=0 ; i<4 ; i++)
		menu_drawslot(i);

	disp_DrawSprite(txid_CURSOR,(int)cursor_x-16,(int)cursor_y-16,32,32,true);
}

void menu_ClickedA_PAUSED()
{
	int		i;

	if(MenuType[0].Activated)
	{
		ds_PlaySound(4);
		gAlphaFont = 1;
		ProgramState = saved_ProgramState; //PS_GAME;
		return;
	}
	else if(MenuType[1].Activated)
	{
		ResumingAlloweed = true;
		ds_PlaySound(4);
		menu_InitConfig();
		ProgramState = PS_CONFIG;
		return;
	}
	else if(MenuType[2].Activated)
	{
		game_UpdateSlot_all();
		ds_PlaySound(4);

		// HACK
		if(gIsMultiplayer)
		{
			//net_StopMultiplayer();
			terminator.value = 0;	// HACK : ra3 style mutliplayer mode termine!
		}

		strcpy(HalloLetters, "_________");
		if(gMapIntro == false)
		{
			game_LaunchPresMusic();
			game_LaunchMapIntro();
		}
		else
		{
			menu_InitPresentation();
			ProgramState = PS_PRES;
			UnloadMap();
		}
		return;
	}
	else if(MenuType[3].Activated)
	{
		for(i=0 ; i<4 ; i++)
			MenuType[i].Activated = false;

		game_UpdateSlot_all();
	//	ds_PlaySound(76+randInt(0,9));
		ds_PlaySound(4);

		ProgramState = PS_GREETINGS;
		gAlphaFont = 1;
		game_PlayWeirdSound();
		UnloadMap();
		game_LaunchPresMusic();
		return;
	}

	if(ProgramState == PS_GREETINGS)
	{
		ds_PlaySound(4);
		disp_SetFading2D_Out(PS_QUIT);
		//ProgramState = PS_QUIT;
		return;
	}	
}

void menu_ClickedB_PAUSED()
{
	ds_PlaySound(4);
	gAlphaFont = 1;
	ProgramState = saved_ProgramState; //PS_GAME;
}

void DrawGameTitle()
{
	int	i;

	int_DrawBackground();

	int_DrawTitle();

	for(i=0 ; i<5 ; i++) {
#ifdef H_MAC
		if(i == 3) {
			continue;
		}
#endif
		menu_drawslot(i);
	}

	disp_DrawSprite(txid_CURSOR,(int)cursor_x-16,(int)cursor_y-16,32,32,true);
}

void menu_ClickedA_PRES()
{
	int		i;
	float	cursor2_x = cursor_x * 1.25f;
	float	cursor2_y = 600 - (cursor_y * 1.25f);

	if(MenuType[0].Activated)
	{
		ds_PlaySound(4);
		menu_InitConfig_SETSLOTGAME();
		ProgramState=PS_SETSLOTGAME;
		return;
	}

	else if(MenuType[1].Activated)
	{
		ds_PlaySound(4);
		menu_InitHELP();
		ProgramState = PS_HELP;
		return;
	}
	else if(MenuType[2].Activated)
	{
		ResumingAlloweed=false;
		ds_PlaySound(4);
		menu_InitConfig();
		ProgramState=PS_CONFIG;
		return;
	}
	else if(MenuType[3].Activated)
	{
		ds_PlaySound(4);
		if(shareware || gIsDemo)
		{
//			ShowWindow(ghWnd, SW_MINIMIZE);
//                        fs_launchURL("https://www.plimus.com/jsp/buynow.jsp?contractId=1641207");

			if(ProgramState==PS_PRES)
			{
				for(i=0 ; i<4 ; i++)
					MenuType[i].Activated=false;
				//ds_PlaySound(4);
				disp_SetFading2D_Out(PS_GREETINGS);
				game_PlayWeirdSound();
				return;
			}
		}
	//	menu_InitORDER();
	//	ProgramState = PS_ORDER;
		//return; // <- hack
	}
	else if(MenuType[4].Activated)
	{
		if(ProgramState==PS_PRES)
		{
			for(i=0 ; i<4 ; i++)
				MenuType[i].Activated=false;

		//	ds_PlaySound(76+randInt(0,9));
			ds_PlaySound(4);

			disp_SetFading2D_Out(PS_GREETINGS);
			//ProgramState = PS_GREETINGS;
			game_PlayWeirdSound();
			return;
		}
	}

	if(shareware || gIsDemo)
	{
		//171 489 - 375 532
		if(	(cursor2_x >= 171 && cursor2_x < 375) &&
			(cursor2_y >= 489 && cursor2_y < 532) )
		{
			if(ProgramState == PS_GREETINGS)
			{
				disp_SetFading2D_Out(PS_QUIT);
				//ProgramState = PS_QUIT;
				return;
			}
			else if(ProgramState == PS_SHOWSHINFO)
			{
				disp_SetFading2D_Out(PS_PRES);
				//ProgramState = PS_PRES;
				return;
			}
		}
		// 417 489 - 620 532
		else if((cursor2_x >= 417 && cursor2_x < 620) &&
				(cursor2_y >= 489 && cursor2_y < 532) )
		{
			if(ProgramState == PS_GREETINGS || ProgramState == PS_SHOWSHINFO)
			{
				//ProgramState = PS_QUIT;
				disp_SetFading2D_Out(PS_QUIT);
                                
#ifdef H_WINDOWS
                                fs_launchURL("https://www.plimus.com/jsp/buynow.jsp?contractId=1641207");
#endif

#ifdef H_MAC
                                fs_launchURL("http://store.esellerate.net/jadeware/mac");
#endif

#ifdef H_LINUX
                                fs_launchURL("http://store.esellerate.net/jadeware/lnx");
#endif
			}
		}
	}
	else
	{
		if(ProgramState == PS_GREETINGS)
		{
			//ProgramState = PS_QUIT;
			disp_SetFading2D_Out(PS_QUIT);
			return;
		}
	}
}

//==================================================================================//

void DrawGameConfig()
{
	int	i;

	int_DrawBackground();
	int_DrawTitle();

	for(i=0 ; i<10 ; i++)
		menu_drawslot(i);

	disp_DrawSprite(txid_CURSOR,(int)cursor_x-16,(int)cursor_y-16,32,32,true);
}

void menu_ClickedA_CONFIG()
{
	if(MenuType[2].Activated)
	{
		ds_PlaySound(4);
		menu_InitConfig_INPUT();
		ProgramState=PS_CONFIG_INPUT;
		return;
	}
	else if(MenuType[3].Activated)
	{
		ds_PlaySound(4);
		menu_InitConfig_VIDEO();
		ProgramState=PS_CONFIG_VIDEO;
		return;
	}
	else if(MenuType[0].Activated)
	{
		ds_PlaySound(4);
		menu_InitConfig_SOUND();
		ProgramState=PS_CONFIG_SOUND;
		return;
	}
	else if(MenuType[1].Activated)
	{
		ds_PlaySound(4);
		menu_InitConfig_GAMEOPTION();
		ProgramState=PS_CONFIG_GAMEOPTION;
		return;
	}
	else if(MenuType[4].Activated)
	{
		ds_PlaySound(4);
		if(!ResumingAlloweed)
			game_Init();
		else
			menu_InitPaused();
		return;
	}
}

void menu_ClickedB_CONFIG()
{
	switch(ProgramState)
	{
		case PS_CONFIG:
		if(!ResumingAlloweed)
			game_Init();
		else
			menu_InitPaused();
		break;

		case PS_CONFIG_INPUT:
		menu_InitConfig();
		ProgramState = PS_CONFIG;
		break;

		case PS_CONFIG_GAMEOPTION:
		menu_InitConfig();
		ProgramState = PS_CONFIG;
		break;

		case PS_CONFIG_VIDEO:
		menu_InitConfig();
		ProgramState = PS_CONFIG;
		break;

		case PS_CONFIG_SOUND:
		menu_InitConfig();
		ProgramState = PS_CONFIG;
		break;
	}
}

//==================================================================================//

void DrawGameConfig_VIDEO()
{
	int	i;

	int_DrawBackground();
	int_DrawTitle();
	//disp_DrawSprite_2(txid_COPY,192,0,256,32,true);
	
#ifdef H_MAC
	menu_drawslot(7);
	menu_drawslot(8);
	menu_drawslot(9);
	
	menu_drawslot(10);
	menu_drawslot(11);
	menu_drawslot(12);
	menu_drawslot(13);
	menu_drawslot(14);
	
#else
	for(i=0 ; i<16 ; i++)
		menu_drawslot(i);
	
#endif


#ifdef H_MAC
	
#else
	g_Disp(16,24,26,"Apply");
	
	if(new_fullscreen)
		g_Disp(16,17,23,"Full screen ON");
	else
		g_Disp(16,17,23,"Full screen OFF");

	g_Disp(16,18,23,"Video mode %dx%d",(int)ScreenX[new_videomode],(int)ScreenY[new_videomode]);
	g_Disp(16,19,23,"Color depth %d",new_colordepth);
	
	if(lightmap.value)
		g_Disp(16,20,23,"Lightmap");
	else
		g_Disp(16,20,23,"Vertex");
	
	g_Disp(16,21,23,"Texture quality %d", 100-(((int)picmip.value-1)*20));

#endif
	
	g_Disp(16,22,23,"Display Brigthness");

	disp_DrawSprite(txid_CURSOR,(int)cursor_x-16,(int)cursor_y-16,32,32,true);
}

void menu_ClickedA_CONFIG_VIDEO()
{

	if(MenuType[0].Activated)
	{
		ds_PlaySound(4);
		new_fullscreen = !new_fullscreen;
	}
	else if(MenuType[1].Activated)
	{
		if(new_videomode > 0)
			new_videomode--;
		else
			new_videomode=6;
	}
	else if(MenuType[2].Activated)
	{
		if(new_videomode < 6)
			new_videomode++;
		else
			new_videomode=0;
	}
	else if(MenuType[3].Activated)
	{
		if(new_colordepth == 16)
			new_colordepth = (float)32;
		else
			new_colordepth = (float)16;
	}
	if(MenuType[4].Activated)
	{
		ds_PlaySound(4);
		lightmap.value=(float)!lightmap.value;
	}
	if(MenuType[5].Activated)
	{
		ds_PlaySound(4);
		if(picmip.value < 5)
		{
			picmip.value += 1;
			//sys_RestartVID();
		}
	}
	if(MenuType[6].Activated)
	{
		ds_PlaySound(4);
		if(picmip.value > 1)
		{
			picmip.value -= 1;
			//sys_RestartVID();
		}
	}
	else if(MenuType[7].Activated)
	{
		ds_PlaySound(4);
		gamma_.value += 0.08f;
		if(gamma_.value > 1.7f)
			gamma_.value = 1.7f;		
		sys_setGamma(gamma_.value);
	}
	else if(MenuType[8].Activated)
	{
		ds_PlaySound(4);
		gamma_.value -= 0.08f;
		if(gamma_.value < 0.3f)
			gamma_.value = 0.3f;		
		sys_setGamma(gamma_.value);
	}
	else if(MenuType[9].Activated)
	{
		ds_PlaySound(4);
		menu_InitConfig();
		ProgramState = PS_CONFIG;
	}
	else if(MenuType[15].Activated)
	{
		// apply video settings
		fullscreen.value = (float)new_fullscreen;
		videomode.value = (float)new_videomode;
		colordepth.value = (float)new_colordepth;

		ds_PlaySound(4);
		sys_killVID();
		if(!sys_setVID())
		{
			ProgramState = PS_QUIT;
			return;
		}
	#ifdef H_MAC
		KInput::hidePointer();
	#endif
		gl_SetFog();
		im_ReloadTextures();
	}
}

//==================================================================================//

void DrawGameConfig_SOUND()
{
	int	i;

	int_DrawBackground();
	int_DrawTitle();

	for(i=0 ; i<10 ; i++)
		menu_drawslot(i);

	g_Disp(16,18,20,"Music volume: %d",(int)musicvol.value);
	g_Disp(16,19,20,"Sound volume: %d",(int)soundvol.value);

	disp_DrawSprite(txid_CURSOR,(int)cursor_x-16,(int)cursor_y-16,32,32,true);
}

void menu_ClickedA_CONFIG_SOUND()
{
	if(MenuType[0].Activated)
	{
		ds_PlaySound(4);
		musicvol.value-=10.0f;
		if(musicvol.value < 0)
			musicvol.value = 0;
		ds_AdjustMusicVolumes();
	}
	else if(MenuType[1].Activated)
	{
		ds_PlaySound(4);
		musicvol.value+=10.0f;
		if(musicvol.value > 100.0f)
			musicvol.value = 100.0f;
		ds_AdjustMusicVolumes();
	}
	else if(MenuType[2].Activated)
	{
		ds_PlaySound(4);
		soundvol.value-=10.0f;
		if(soundvol.value < 0)
			soundvol.value = 0;
		ds_AdjustSoundVolumes();
	}
	else if(MenuType[3].Activated)
	{
		ds_PlaySound(4);
		soundvol.value+=10.0f;
		if(soundvol.value > 100.0f)
			soundvol.value = 100.0f;
		ds_AdjustSoundVolumes();
	}
	else if(MenuType[4].Activated)
	{
	//	ds_PlaySound(4);
		menu_InitConfig();
		ProgramState = PS_CONFIG;
	}
}

//==================================================================================//

void DrawGameConfig_INPUT()
{
	int	i;

	int_DrawBackground();
	int_DrawTitle();

	for(i=0 ; i<22 ; i++)
		menu_drawslot(i);

//	g_DispCenterString(22,6,"CUSTOMIZE CONTROL");
	if(invert_mouse.value == 0)
		g_DispCenterString(16,11,"Invert Mouse: OFF");
	else
		g_DispCenterString(16,11,"Invert Mouse: ON");
	g_DispCenterString(16,12,"Mouse Sensibility: %d",(int)(player_rvel.value*100)); // %1.2f

#ifdef H_WINDOWS
	g_Disp(16,14,25,"Walk Forward [%s]",KeyToStringName[gGameIdKeys[0]]);
	g_Disp(16,15,25,"Walk Backward [%s]",KeyToStringName[gGameIdKeys[1]]);
	g_Disp(16,16,25,"Step Left [%s]",KeyToStringName[gGameIdKeys[2]]);
	g_Disp(16,17,25,"Step Right [%s]",KeyToStringName[gGameIdKeys[3]]);
	g_Disp(16,18,25,"Jump [%s]",KeyToStringName[gGameIdKeys[4]]);
	g_Disp(16,19,25,"Crouch [%s]",KeyToStringName[gGameIdKeys[5]]);
	g_Disp(16,20,25,"Attack [%s]",KeyToStringName[gGameIdKeys[6]]);
	g_Disp(16,21,25,"Axe [%s]",KeyToStringName[gGameIdKeys[7]]);
	g_Disp(16,22,25,"Crossbow [%s]",KeyToStringName[gGameIdKeys[8]]);
	g_Disp(16,23,25,"BombGun [%s]",KeyToStringName[gGameIdKeys[9]]);
	g_Disp(16,24,25,"Tromblon [%s]",KeyToStringName[gGameIdKeys[10]]);
	g_Disp(16,25,25,"Action [%s]",KeyToStringName[gGameIdKeys[11]]);
	g_Disp(16,26,25,"Run [%s]",KeyToStringName[gGameIdKeys[12]]);
#endif 

#ifdef H_MAC
	g_Disp(16,14,25,"Walk Forward [%s]",IN_GetKeyStringName(gGameIdKeys[0]));
	g_Disp(16,15,25,"Walk Backward [%s]",IN_GetKeyStringName(gGameIdKeys[1]));
	g_Disp(16,16,25,"Step Left [%s]",IN_GetKeyStringName(gGameIdKeys[2]));
	g_Disp(16,17,25,"Step Right [%s]",IN_GetKeyStringName(gGameIdKeys[3]));
	g_Disp(16,18,25,"Jump [%s]",IN_GetKeyStringName(gGameIdKeys[4]));
	g_Disp(16,19,25,"Crouch [%s]",IN_GetKeyStringName(gGameIdKeys[5]));
	g_Disp(16,20,25,"Attack [%s]",IN_GetKeyStringName(gGameIdKeys[6]));
	g_Disp(16,21,25,"Axe [%s]",IN_GetKeyStringName(gGameIdKeys[7]));
	g_Disp(16,22,25,"Crossbow [%s]",IN_GetKeyStringName(gGameIdKeys[8]));
	g_Disp(16,23,25,"BombGun [%s]",IN_GetKeyStringName(gGameIdKeys[9]));
	g_Disp(16,24,25,"Tromblon [%s]",IN_GetKeyStringName(gGameIdKeys[10]));
	g_Disp(16,25,25,"Action [%s]",IN_GetKeyStringName(gGameIdKeys[11]));
	g_Disp(16,26,25,"Run [%s]",IN_GetKeyStringName(gGameIdKeys[12]));
#endif

#ifdef H_LINUX
// disabled by farox....todo
	//g_Disp(16,14,25,"Walk Forward [%s]", SDL_GetKeyName( KeysInput[gGameIdKeys[0]].keysym.sym ) );
	//g_Disp(16,15,25,"Walk Backward [%s]", SDL_GetKeyName( KeysInput[gGameIdKeys[1]].keysym.sym ));
	//g_Disp(16,16,25,"Step Left [%s]", SDL_GetKeyName( KeysInput[gGameIdKeys[2]].keysym.sym ));
	//g_Disp(16,17,25,"Step Right [%s]", SDL_GetKeyName( KeysInput[gGameIdKeys[3]].keysym.sym ));
	//g_Disp(16,18,25,"Jump [%s]", SDL_GetKeyName( KeysInput[gGameIdKeys[4]].keysym.sym ));
	//g_Disp(16,19,25,"Crouch [%s]", SDL_GetKeyName( KeysInput[gGameIdKeys[5]].keysym.sym ));
	//g_Disp(16,20,25,"Attack [%s]", SDL_GetKeyName( KeysInput[gGameIdKeys[6]].keysym.sym ));
	//g_Disp(16,21,25,"Axe [%s]", SDL_GetKeyName( KeysInput[gGameIdKeys[7]].keysym.sym ));
	//g_Disp(16,22,25,"Crossbow [%s]", SDL_GetKeyName( KeysInput[gGameIdKeys[8]].keysym.sym ));
	//g_Disp(16,23,25,"BombGun [%s]", SDL_GetKeyName( KeysInput[gGameIdKeys[9]].keysym.sym ));
	//g_Disp(16,24,25,"Tromblon [%s]", SDL_GetKeyName( KeysInput[gGameIdKeys[10]].keysym.sym ));
	//g_Disp(16,25,25,"Action [%s]", SDL_GetKeyName( KeysInput[gGameIdKeys[11]].keysym.sym ));
	//g_Disp(16,26,25,"Run [%s]", SDL_GetKeyName( KeysInput[gGameIdKeys[12]].keysym.sym ));
#endif	

//	g_Disp(16,27,25,"+ Insult monsters [NUMPAD_2]");
	g_Disp(16,28,25,"+ Screenshot [NUMPAD_5]");

	disp_DrawSprite(txid_CURSOR,(int)cursor_x-16,(int)cursor_y-16,32,32,true);
}

void menu_ClickedA_CONFIG_INPUT()
{
	int		i;

	if(MenuType[0].Activated)
	{
		ds_PlaySound(4);
		if(invert_mouse.value == 0)
			invert_mouse.value = 1.0f;
		else
			invert_mouse.value = 0;
	}
	else if(MenuType[1].Activated)
	{
		ds_PlaySound(4);
		player_rvel.value -= 0.01f;
		if(player_rvel.value < 0.02f)
			player_rvel.value = 0.02f;
	}
	else if(MenuType[2].Activated)
	{
		ds_PlaySound(4);
		player_rvel.value += 0.01f;
		if(player_rvel.value > 0.40f)
			player_rvel.value = 0.40f;
	}
	else if(MenuType[3].Activated)
	{
	//	ds_PlaySound(4);
		menu_InitConfig();
		ProgramState = PS_CONFIG;
	}
	else
	{
		for(i=0 ; i<MAX_GAMEKEY ; i++)
		{
			if(MenuType[4+i].Activated)
			{
				ds_PlaySound(4);
				gIdGameKey = i;
				gScanKeyActivated = true;
			}
		}
	}
}

//==================================================================================//

void DrawGameConfig_GAMEOPTION()
{
	int	i;

	int_DrawBackground();
	int_DrawTitle();

	for(i=0 ; i<9 ; i++)
		menu_drawslot(i);

	if(autoswitch.value == 0)
		g_Disp(16,18,23,"Autoswitch weapon: OFF");
	else
		g_Disp(16,18,23,"Autoswitch weapon: ON");

	if(drawgun.value == 0)
		g_Disp(16,19,23,"Draw weapon: OFF");
	else
		g_Disp(16,19,23,"Draw weapon: ON");

	if(draw_fps.value == 0)
		g_Disp(16,20,23,"Draw FPS: OFF");
	else
		g_Disp(16,20,23,"Draw FPS: ON");

	disp_DrawSprite(txid_CURSOR,(int)cursor_x-16,(int)cursor_y-16,32,32,true);
}

void menu_ClickedA_CONFIG_GAMEOPTION()
{
	if(MenuType[0].Activated)
	{
		ds_PlaySound(4);
		if(autoswitch.value == 0)
			autoswitch.value = 1.0f;
		else
			autoswitch.value = 0;
	}
	else if(MenuType[1].Activated)
	{
		ds_PlaySound(4);
		if(drawgun.value == 0)
			drawgun.value = 1.0f;
		else
			drawgun.value = 0;
	}
	else if(MenuType[2].Activated)
	{
		ds_PlaySound(4);
		if(draw_fps.value == 0)
			draw_fps.value = 1.0f;
		else
			draw_fps.value = 0;
	}
	else if(MenuType[3].Activated)
	{
	//	ds_PlaySound(4);
		menu_InitConfig();
		ProgramState = PS_CONFIG;
	}
}

//==================================================================================//

void DrawGameConfig_SelectMAP()
{
	menu_DispLoadMapSplash();

	//	g_DispString(14,"gFrameTime_msec: %f",gFrameTime_msec);
	//	g_DispString(14,"gTotalTime_msec: %f",gTotalTime_msec);
	//	g_DispString(14,"clock: %f",(float)disp_GetClock());

	menu_drawslot(0);
	menu_drawslot(1);
	disp_DrawSprite(txid_CURSOR, (int)cursor_x-16, (int)cursor_y-16, 32, 32, true);
}

void menu_ClickedA_SelectMAP()
{
	int		i;

	if(MenuType[0].Activated)
	{
		ds_PlaySound(4);
		game_LaunchMapIntro();
	}
	else if(MenuType[1].Activated)
	{
		ds_PlaySound(4);
		if(gMapBonus)
		{
			game_LaunchBonusMapMusic();
		}
		else
		{
			game_LaunchMapMusic();
		}

		ProgramState = PS_GAME;

		// HACK : cale sur le FPS exacte
		gMenu_InitFirstSel = false;
		for(i=0 ; i<30 ; i++)
		{
			gl_ClearScreen();
			disp_Scene();
		}
	}
}

//=======================================================================

void DrawGameConfig_StartMAP_goals()
{
	int		xoffset;
	int		yoffset = 280;
	int		fnt = 24;
	int		xdecal;
	int		ydecal = 26;
	int		icon_hauteur;
	int		n_goal;
	int		NombreGoals;
	char	str[255];
	hBool	AllGoalCleared = true;
	int		diff;
	int		i;

	diff = (int)difficulty.value;

	if(gIsMultiplayer)
	{
		if(gIsServer)
		{
			text_Disp(30,"TOURNAMENT",2,342,0,0,0,true);
			text_Disp(30,"TOURNAMENT",0,340,255,255,255,true);
		}
		else
		{
			g_DispCenterString(28, 3, "Connecting to.. %s", net_ip.string);
		}
		return;
	}

	// affiche les textes
	text_Disp(30,"YOU HAVE TO KILL",2,342,0,0,0,true);
	text_Disp(30,"YOU HAVE TO KILL",0,340,255,255,255,true);

	// affiche les icones des objectifs
	n_goal = MapGoals[0][diff][game_MapID];
	if(n_goal)
	{
		if(game_ReadBit((int)map_letter.value, game_MapID))
		//if((int)map_letter.value & mask)
		{
			// hack
			sprintf(str,"you found all letters");
			text_Disp(14,str,0+2,50+2,0,0,0,true);
			text_Disp(14,str,0,50+0,255,255,255,true);
		}
		else
		{
			AllGoalCleared = false;
			sprintf(str,"-find and pickup all the letters-");
			text_Disp(14,str,0+2,50+2,0,0,0,true);
			text_Disp(14,str,0,50+0,255,255,255,true);
		}
	}
	else
	{
		// hack
		sprintf(str,"-no letters to pickup here-");
		text_Disp(14,str,0+2,50+2,0,0,0,true);
		text_Disp(14,str,0,50+0,255,255,255,true);
	}

	// calcul le centrage des N icones de goals en largeur
	xoffset = 0;
	xdecal = 120;
	yoffset -= ydecal;
	icon_hauteur = 180;

	NombreGoals = 0;
	for(i=1 ; i<6 ; i++)
	{
		if(MapGoals[i][diff][game_MapID])
		{
			NombreGoals++;
		}
	}
	NombreGoals++;
	xoffset = 32 + (800 - (xdecal * NombreGoals))/2; 


	n_goal = MapGoals[1][diff][game_MapID];
	if(n_goal)
	{
		//if((int)kill_depla.value & mask)
		if(game_ReadBit((int)kill_depla.value, game_MapID))
		{
			disp_DrawSprite(txid_DEPLA_ICON[1],-15+xoffset,icon_hauteur,64,64,true);
		}
		else
		{
		AllGoalCleared = false;
		disp_DrawSprite(txid_DEPLA_ICON[0],-15+xoffset,icon_hauteur,64,64,true);

		sprintf(str,"D E P L A");
		text_Disp(14,str,-13+xoffset+2,yoffset+2,0,0,0,false);
		text_Disp(14,str,-13+xoffset+0,yoffset+0,255,255,255,false);
	
		sprintf(str,"%d",n_goal);
		text_Disp(50,str,xoffset+2,-130+yoffset+2,0,0,0,false);
		text_Disp(50,str,xoffset+0,-130+yoffset+0,255,255,255,false);
		}
		xoffset += xdecal;
	}

	n_goal = MapGoals[2][diff][game_MapID];
	if(n_goal)
	{
		//if((int)kill_morbak.value & mask)
		if(game_ReadBit((int)kill_morbak.value, game_MapID))
		{
			disp_DrawSprite(txid_MORBAK_ICON[1],-15+xoffset,icon_hauteur,64,64,true);
		}
		else
		{
		AllGoalCleared = false;
		disp_DrawSprite(txid_MORBAK_ICON[0],-15+xoffset,icon_hauteur,64,64,true);

		sprintf(str,"M O R B A K");
		text_Disp(14,str,-25+xoffset+2,yoffset+2,0,0,0,false);
		text_Disp(14,str,-25+xoffset+0,yoffset+0,255,255,255,false);

		sprintf(str,"%d",n_goal);
		text_Disp(50,str,xoffset+2,-130+yoffset+2,0,0,0,false);
		text_Disp(50,str,xoffset+0,-130+yoffset+0,255,255,255,false);
		}
		xoffset += xdecal;
	}

	n_goal = MapGoals[3][diff][game_MapID];
	if(n_goal)
	{
		//if((int)kill_zeubat.value & mask)
		if(game_ReadBit((int)kill_zeubat.value, game_MapID))
		{
			disp_DrawSprite(txid_BAT_ICON[1],-15+xoffset,icon_hauteur,64,64,true);
		}
		else
		{
		AllGoalCleared = false;
		disp_DrawSprite(txid_BAT_ICON[0],-15+xoffset,icon_hauteur,64,64,true);

		sprintf(str,"Z E B A T");
		text_Disp(14,str,-10+xoffset+2,yoffset+2,0,0,0,false);
		text_Disp(14,str,-10+xoffset+0,yoffset+0,255,255,255,false);

		sprintf(str,"%d",n_goal);
		text_Disp(50,str,xoffset+2,-130+yoffset+2,0,0,0,false);
		text_Disp(50,str,xoffset+0,-130+yoffset+0,255,255,255,false);
		}
		xoffset += xdecal;
	}

	n_goal = MapGoals[4][diff][game_MapID];
	if(n_goal)
	{
		//if((int)kill_subzerat.value & mask)
		if(game_ReadBit((int)kill_subzerat.value, game_MapID))
		{
			disp_DrawSprite(txid_SUB_ICON[1],-15+xoffset,icon_hauteur,64,64,true);
		}
		else
		{
		AllGoalCleared = false;
		disp_DrawSprite(txid_SUB_ICON[0],-15+xoffset,icon_hauteur,64,64,true);

		sprintf(str,"SUBZERAT");
		text_Disp(14,str,-25+xoffset+2,yoffset+2,0,0,0,false);
		text_Disp(14,str,-25+xoffset+0,yoffset+0,255,255,255,false);

		sprintf(str,"%d",n_goal);
		text_Disp(50,str,xoffset+2,-130+yoffset+2,0,0,0,false);
		text_Disp(50,str,xoffset+0,-130+yoffset+0,255,255,255,false);
		}
		xoffset += xdecal;
	}
	
	n_goal = MapGoals[5][diff][game_MapID];
	if(n_goal)
	{
		//if((int)kill_zemummy.value & mask)
		if(game_ReadBit((int)kill_zemummy.value, game_MapID))
		{
			disp_DrawSprite(txid_MOMY_ICON[1],-15+xoffset,icon_hauteur,64,64,true);
		}
		else
		{
		AllGoalCleared = false;
		disp_DrawSprite(txid_MOMY_ICON[0],-15+xoffset,icon_hauteur,64,64,true);

		sprintf(str,"M U M M Y");
		text_Disp(14,str,-23+xoffset+2,yoffset+2,0,0,0,false);
		text_Disp(14,str,-23+xoffset+0,yoffset+0,255,255,255,false);

		sprintf(str,"%d",n_goal);
		text_Disp(50,str,xoffset+2,-130+yoffset+2,0,0,0,false);
		text_Disp(50,str,xoffset+0,-130+yoffset+0,255,255,255,false);
		}
		xoffset += xdecal;
	}

	if(AllGoalCleared)
	{
		int best_score = 0;

		game_HitBestScoreMode = true;
		gAllGoalCleared = true;
		game_TimeLimit = true;
		game_clock = 60;

		yoffset -= ydecal;
		yoffset -= ydecal;

	//	sprintf(str,"This Level is Cleared",n_goal);

		switch(game_MapID)
		{
			case 1:	best_score = (int)bestscore1.value;		break;
			case 2:	best_score = (int)bestscore2.value;		break;
			case 3:	best_score = (int)bestscore3.value;		break;
			case 4:	best_score = (int)bestscore4.value;		break;
			case 5:	best_score = (int)bestscore5.value;		break;
			case 6:	best_score = (int)bestscore6.value;		break;
			case 7:	best_score = (int)bestscore7.value;		break;
			case 8:	best_score = (int)bestscore8.value;		break;
			case 9:	best_score = (int)bestscore9.value;		break;
		}		
		
		sprintf(str,"Kill lot of monsters and hit the best score");
		text_Disp(fnt,str,2,-100+2+480,0,0,0,true);
		text_Disp(fnt,str,0,-100+0+480,255,255,255,true);

		sprintf(str,"Best Score: %d", best_score);
		text_Disp(fnt,str,2,-100+yoffset+2,0,0,0,true);
		text_Disp(fnt,str,0,-100+yoffset+0,255,255,255,true);
	}
	else
	{
//		gAllGoalCleared = false;
	}
}

void DrawGameConfig_StartMAP()
{
	int		i;

	if(game_MapID < 0 || game_MapID >= 9)
	{
		g_DispString(16,"Error : map id greater than max alloweed : 9");
		return;
	}

	// affiche le fond et les boutons
	disp_DrawSprite_2(txid_LOADINGMAP,0,0,800,600,false);
	for(i=0 ; i<2 ; i++)
		menu_drawslot(i);

	DrawGameConfig_StartMAP_goals();
	disp_DrawSprite(txid_CURSOR,(int)cursor_x-16,(int)cursor_y-16,32,32,true);
}

void menu_DisplayDefaultMap()
{
	disp_DrawSprite_2(txid_DEFAULTMAP,0,0,800,600,false);
}

void menu_DispLoadMapSplash()
{
	if(gMapIntro)
	{
//		glClearColor(1,0.4,0,0);
		gl_ClearScreen();
		disp_DrawSprite_2(txid_LOGO2, 144, 44, 512, 512, false);
		glClearColor(0,0,0,0);
	}
	else if(gMapBonus)
	{
		disp_DrawSprite_2(txid_DEFAULTMAP,0,0,800,600,false);
	}
	else
	{
		disp_DrawSprite_2(txid_LOADINGMAP,0,0,800,600,false);
		DrawGameConfig_StartMAP_goals();
	}
}

void menu_ClickedA_StartMAP()
{
	if(MenuType[0].Activated)
	{
		ds_PlaySound(4);
		game_LaunchMapIntro();
	}
	else if(MenuType[1].Activated)
	{
		menu_DispLoadMapSplash();
		gl_SwapBuffer();

	//	gl_ClearScreen();
	//	text_Disp(14,tab,+0,220+0,255,255,255,true);
	//	gl_SwapBuffer();

	//	ds_PlaySound(4);

		game_Init();
		game_CheckMapCompleted();
		game_LaunchMapMusic();
	//	ds_LoadStreamWAVE(game_GetMusicString((int)music_track.value));
	//	music_track.value++;
	//	music_track.value = (float)((int)music_track.value % 7);
		gMapBonus = false;
		gMapIntro = false;
		gMenu_InitFirstSel = false;
		LoadMap();
		WorldColorR = WorldColorG = WorldColorB = 1;
		ProgramState = PS_GAME;
	}
}

//=======================================================================

void DrawGameConfig_HELP()
{
	int		i;

	int_DrawBackground();
	int_DrawTitle();

	disp_DrawSprite2Aplha(txid_HELP,315,-50,512,512,true,gAlphaFont);


	for(i=0 ; i<6 ; i++)
		menu_drawslot(i);
	
	disp_DrawSprite(txid_CURSOR,(int)cursor_x-16,(int)cursor_y-16,32,32,true);
}

void menu_ClickedA_HELP()
{
	if(MenuType[0].Activated)
	{
		ds_PlaySound(4);
		game_Init();
	}
}

//=======================================================================

void DrawGameConfig_ORDER()
{
	int		i;

	gMenu_InitFirstSel = false;
	int_DrawBackground();
	int_DrawTitle();

	disp_DrawSprite_2(txid_ORDER,300,5,512,512,true);

	for(i=0 ; i<6 ; i++)
		menu_drawslot(i);
	
	disp_DrawSprite(txid_CURSOR,(int)cursor_x-16,(int)cursor_y-16,32,32,true);
}

void menu_ClickedA_ORDER()
{
	if(MenuType[0].Activated)
	{
		ds_PlaySound(4);
		game_Init();
	}
}

//=======================================================================

void DrawGameMenu_BEGINGAME()
{
	int		i;
	char	tab[255];

	int_DrawBackground();
	int_DrawTitle();
	//disp_DrawSprite_2(txid_COPY,192,0,256,32,true);

	for(i=0 ; i<4 ; i++)
		menu_drawslot(i);

	sprintf(tab,"Select Game Difficulty");
	text_Disp(20,tab,+2,280+2,0,0,0,true);
	text_Disp(20,tab,+0,280+0,255,255,255,true);

//	sprintf(tab,"Easy");
//	text_Disp(30,tab,+2,200+2,192,192,192,true);
//	text_Disp(30,tab,+0,200+0,0,0,0,true);

//	sprintf(tab,"Normal");
//	text_Disp(30,tab,+2,150+2,192,192,192,true);
//	text_Disp(30,tab,+0,150+0,0,0,0,true);

//	sprintf(tab,"Hard");
//	text_Disp(30,tab,+2,100+2,192,192,192,true);
//	text_Disp(30,tab,+0,100+0,0,0,0,true);

	disp_DrawSprite(txid_CURSOR,(int)cursor_x-16,(int)cursor_y-16,32,32,true);
}

void menu_ClickedA_BEGINGAME()
{
	FILE	*file;
	char	fullpath[255];
	char	*p;

	if(MenuType[0].Activated)
	{
		ds_PlaySound(4);
		sprintf(fullpath,"%s%sgame%d.svg",SYSDIR,SVGDIR,gGameSlotToProcess);
		
		#ifdef H_MAC
		sprintf(fullpath, "%s/game%d.svg", KMiscTools::currentDocHomePath, gGameSlotToProcess);
		p = KMiscTools::makeFilePath(fullpath);
		file = fopen(p,"w");
		#else
		file = fopen(fullpath,"w");
		#endif
		
		if(file)
		{
			game_UpdateSlot(file,0,0,0,0,0,0,0,0);
			fclose(file);
		}
		game_CheckMapCompleted();
		game_LaunchMapIntro();
	}
	else if(MenuType[1].Activated)
	{
		ds_PlaySound(4);
		sprintf(fullpath,"%s%sgame%d.svg",SYSDIR,SVGDIR,gGameSlotToProcess);
		
		#ifdef H_MAC
		sprintf(fullpath, "%s/game%d.svg", KMiscTools::currentDocHomePath, gGameSlotToProcess);
		p = KMiscTools::makeFilePath(fullpath);
		file = fopen(p,"w");
		#else
		file = fopen(fullpath,"w");
		#endif
		
		if(file)
		{
			game_UpdateSlot(file,1,0,0,0,0,0,0,0);
			fclose(file);
		}
		game_CheckMapCompleted();
		game_LaunchMapIntro();
	}
	else if(MenuType[2].Activated)
	{
		ds_PlaySound(4);
		sprintf(fullpath,"%s%sgame%d.svg",SYSDIR,SVGDIR,gGameSlotToProcess);
		
		#ifdef H_MAC
		sprintf(fullpath, "%s/game%d.svg", KMiscTools::currentDocHomePath, gGameSlotToProcess);
		p = KMiscTools::makeFilePath(fullpath);
		file = fopen(p,"w");
		#else
		file = fopen(fullpath,"w");
		#endif
		
		if(file)
		{
			game_UpdateSlot(file,2,0,0,0,0,0,0,0);
			fclose(file);
		}
		game_CheckMapCompleted();
		game_LaunchMapIntro();
	}
	else if(MenuType[3].Activated)
	{
		ds_PlaySound(4);
		sprintf(fullpath,"%s%sgame%d.svg",SYSDIR,SVGDIR,gGameSlotToProcess);
		#ifdef H_MAC
		sprintf(fullpath, "%s/game%d.svg", KMiscTools::currentDocHomePath, gGameSlotToProcess);
		file = fopen(p,"w");
		#endif
		remove(fullpath);
		menu_InitPresentation();
		ProgramState = PS_PRES;
	}
}

void menu_ClickedB_BEGINGAME()
{
	char	fullpath[255];

	ds_PlaySound(4);
	sprintf(fullpath,"%s%sgame%d.svg",SYSDIR,SVGDIR,gGameSlotToProcess);
	#ifdef H_MAC
	sprintf(fullpath, "%s/game%d.svg", KMiscTools::currentDocHomePath, gGameSlotToProcess);
	#endif
	remove(fullpath);
	menu_InitPresentation();
	ProgramState = PS_PRES;
}

//=======================================================================

void DrawGameMenu_YESNO()
{
	int		i;
	char	tab[255];

	int_DrawBackground();
	int_DrawTitle();
	//disp_DrawSprite_2(txid_COPY,192,0,256,32,true);

	for(i=0 ; i<3 ; i++)
		menu_drawslot(i);

	sprintf(tab,"Are you sure you want to delete this game slot?");
	text_Disp(20,tab,+2,280+2,0,0,0,true);
	text_Disp(20,tab,+0,280+0,255,255,255,true);

//	sprintf(tab,"Yes");
//	text_Disp(30,tab,+2,200+2,192,192,192,true);
//	text_Disp(30,tab,+0,200+0,0,0,0,true);

//	sprintf(tab,"No");
//	text_Disp(30,tab,+2,130+2,192,192,192,true);
//	text_Disp(30,tab,+0,130+0,0,0,0,true);

	disp_DrawSprite(txid_CURSOR,(int)cursor_x-16,(int)cursor_y-16,32,32,true);
}

void menu_ClickedA_YESNO()
{
	char	fullpath[255];
	char	*p;

	if(MenuType[0].Activated)
	{
		ds_PlaySound(4);
		sprintf(fullpath,"%s%sgame%d.svg",SYSDIR,SVGDIR,gGameSlotToProcess);
		
		#ifdef H_MAC
		sprintf(fullpath, "%s/game%d.svg", KMiscTools::currentDocHomePath, gGameSlotToProcess);
		p = KMiscTools::makeFilePath(fullpath);
		remove(p);
		#else
		remove(fullpath);
		#endif
		
		menu_InitConfig_SETSLOTGAME();
		ProgramState=PS_SETSLOTGAME;
	}
	else if(MenuType[1].Activated)
	{
		ds_PlaySound(4);
		menu_InitConfig_SETSLOTGAME();
		ProgramState=PS_SETSLOTGAME;
	}
	else if(MenuType[2].Activated)
	{
		ds_PlaySound(4);
		menu_InitConfig_SETSLOTGAME();
		ProgramState=PS_SETSLOTGAME;
	}
}

//=======================================================================

void DrawGameMenu_SETSLOTGAME()
{
	int		i;

	int_DrawBackground();
	int_DrawTitle();

	for(i=0 ; i<9+5 ; i++)
		menu_drawslot(i);

	disp_DrawSprite(txid_CURSOR,(int)cursor_x-16,(int)cursor_y-16,32,32,true);
}

void menu_ClickedA_SETSLOTGAME()
{
	FILE	*file;
	char	fullpath[255];
	char	*p;

	if(MenuType[0].Activated)
	{
		ds_PlaySound(4);
		gameslot.value = 1;
		sprintf(fullpath,"%s%sgame1.svg",SYSDIR,SVGDIR);
		
		#ifdef H_MAC
		sprintf(fullpath, "%s/game1.svg", KMiscTools::currentDocHomePath);
		p = KMiscTools::makeFilePath(fullpath);
		file = fopen(p,"r");
		#else
		file = fopen(fullpath,"r");
		#endif
		
		if(!file)
		{
			#ifdef H_MAC
			p = KMiscTools::makeFilePath(fullpath);
			file = fopen(p,"w");
			#else
			file = fopen(fullpath,"w");
			#endif
		
			game_UpdateSlot(file,1,0,0,0,0,0,0,0);
			fclose(file);
			gGameSlotToProcess=1;
			menu_InitConfig_BEGINGAME();
			ProgramState=PS_BEGINGAME;
		}
		else
		{
			game_LoadSlot(file);
			fclose(file);
			game_CheckMapCompleted();
			gWelcomeToHalloween = true;
			//game_LaunchMapIntro();
			disp_SetFading2D_Out(PS_LAUNCHMAPINTRO);
			return;
		}
	}
	else if(MenuType[1].Activated)
	{
		ds_PlaySound(4);
		gameslot.value = 2;
		sprintf(fullpath,"%s%sgame2.svg",SYSDIR,SVGDIR);
		
		#ifdef H_MAC
		sprintf(fullpath, "%s/game2.svg", KMiscTools::currentDocHomePath);
		p = KMiscTools::makeFilePath(fullpath);
		file = fopen(p,"r");
		#else
		file = fopen(fullpath,"r");
		#endif
		
		if(!file)
		{
			#ifdef H_MAC
			p = KMiscTools::makeFilePath(fullpath);
			file = fopen(p,"w");
			#else
			file = fopen(fullpath,"w");
			#endif
		
			game_UpdateSlot(file,1,0,0,0,0,0,0,0);
			fclose(file);
			gGameSlotToProcess=2;
			menu_InitConfig_BEGINGAME();
			ProgramState=PS_BEGINGAME;
		}
		else
		{
			game_LoadSlot(file);
			fclose(file);
			game_CheckMapCompleted();
			gWelcomeToHalloween = true;
			//game_LaunchMapIntro();
			disp_SetFading2D_Out(PS_LAUNCHMAPINTRO);
			return;
		}
	}
	else if(MenuType[2].Activated)
	{
		ds_PlaySound(4);
		gameslot.value = 3;
		sprintf(fullpath,"%s%sgame3.svg",SYSDIR,SVGDIR);
		
		#ifdef H_MAC
		sprintf(fullpath, "%s/game3.svg", KMiscTools::currentDocHomePath);
		p = KMiscTools::makeFilePath(fullpath);
		file = fopen(p,"r");
		#else
		file = fopen(fullpath,"r");
		#endif
		
		if(!file)
		{
			#ifdef H_MAC
			p = KMiscTools::makeFilePath(fullpath);
			file = fopen(p,"w");
			#else
			file = fopen(fullpath,"w");
			#endif
		
			game_UpdateSlot(file,1,0,0,0,0,0,0,0);
			fclose(file);
			gGameSlotToProcess=3;
			menu_InitConfig_BEGINGAME();
			ProgramState=PS_BEGINGAME;
		}
		else
		{
			game_LoadSlot(file);
			fclose(file);
			game_CheckMapCompleted();
			gWelcomeToHalloween = true;
			//game_LaunchMapIntro();
			disp_SetFading2D_Out(PS_LAUNCHMAPINTRO);
			return;
		}
	}
	else if(MenuType[3].Activated)
	{
		ds_PlaySound(4);
		gameslot.value = 4;
		sprintf(fullpath,"%s%sgame4.svg",SYSDIR,SVGDIR);
		
		#ifdef H_MAC
		sprintf(fullpath, "%s/game4.svg", KMiscTools::currentDocHomePath);
		p = KMiscTools::makeFilePath(fullpath);
		file = fopen(p,"r");
		#else
		file = fopen(fullpath,"r");
		#endif
		
		if(!file)
		{
			#ifdef H_MAC
			p = KMiscTools::makeFilePath(fullpath);
			file = fopen(p,"w");
			#else
			file = fopen(fullpath,"w");
			#endif
		
			game_UpdateSlot(file,1,0,0,0,0,0,0,0);
			fclose(file);
			gGameSlotToProcess=4;
			menu_InitConfig_BEGINGAME();
			ProgramState=PS_BEGINGAME;
		}
		else
		{
			game_LoadSlot(file);
			fclose(file);
			game_CheckMapCompleted();
			gWelcomeToHalloween = true;
			//game_LaunchMapIntro();
			disp_SetFading2D_Out(PS_LAUNCHMAPINTRO);
			return;
		}
	}
	else if(MenuType[4].Activated)
	{
		ds_PlaySound(4);
		sprintf(fullpath,"%s%sgame1.svg",SYSDIR,SVGDIR);

		#ifdef H_MAC
		sprintf(fullpath, "%s/game1.svg", KMiscTools::currentDocHomePath);
		p = KMiscTools::makeFilePath(fullpath);
		file = fopen(p,"r");
		#else
		file = fopen(fullpath,"r");
		#endif

		if(file)
		{
			fclose(file);
			gGameSlotToProcess = 1;
			menu_InitConfig_YESNO();
			ProgramState=PS_YESNO;
		}
	}
	else if(MenuType[5].Activated)
	{
		ds_PlaySound(4);
		sprintf(fullpath,"%s%sgame2.svg",SYSDIR,SVGDIR);
		
		#ifdef H_MAC
		sprintf(fullpath, "%s/game2.svg", KMiscTools::currentDocHomePath);
		p = KMiscTools::makeFilePath(fullpath);
		file = fopen(p,"r");
		#else
		file = fopen(fullpath,"r");
		#endif
		
		if(file)
		{
			fclose(file);
			gGameSlotToProcess = 2;
			menu_InitConfig_YESNO();
			ProgramState=PS_YESNO;
		}
	}
	else if(MenuType[6].Activated)
	{
		ds_PlaySound(4);
		sprintf(fullpath,"%s%sgame3.svg",SYSDIR,SVGDIR);

		#ifdef H_MAC
		sprintf(fullpath, "%s/game3.svg", KMiscTools::currentDocHomePath);
		p = KMiscTools::makeFilePath(fullpath);
		file = fopen(p,"r");
		#else
		file = fopen(fullpath,"r");
		#endif

		if(file)
		{
			fclose(file);
			gGameSlotToProcess = 3;
			menu_InitConfig_YESNO();
			ProgramState=PS_YESNO;
		}
	}
	else if(MenuType[7].Activated)
	{
		ds_PlaySound(4);
		sprintf(fullpath,"%s%sgame4.svg",SYSDIR,SVGDIR);
		
		#ifdef H_MAC
		sprintf(fullpath, "%s/game4.svg", KMiscTools::currentDocHomePath);
		p = KMiscTools::makeFilePath(fullpath);
		file = fopen(p,"r");
		#else
		file = fopen(fullpath,"r");
		#endif
		
		if(file)
		{
			fclose(file);
			gGameSlotToProcess = 4;
			menu_InitConfig_YESNO();
			ProgramState=PS_YESNO;
		}
	}
	else if(MenuType[8].Activated)
	{
		ds_PlaySound(4);
		menu_InitPresentation();
		ProgramState=PS_PRES;
	}
}


