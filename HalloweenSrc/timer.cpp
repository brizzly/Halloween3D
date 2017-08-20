
#include "alloween.cpp"

double	TimeStart=0;
double	gTimeStart=0;
double	gTime_slow=0;
//float	gFrameTime_msec = 16.667f;
//float	gTotalTime_msec;
//float	gFPSshowed;
hBool	gDispLetters = false;



double FloatTime()
{
	time_t	t;
	
	time (&t);
	
	return t-TimeStart;
}

void IniTimer()
{
	TimeStart = FloatTime();
	gTimeStart = TimeStart;
}

void GetRealTime(char *buf, double d_time)
{
	int		sec, min, hour;

	sec = (int)d_time;
	min = sec/60;
	hour = min/60;
	sec %= 60;
	min %= 60;
	hour %= 24;

	if(hour==0)
	{
		if(sec<10)
			sprintf(buf,"%2d'0%d",min,sec);
		else
			sprintf(buf,"%2d'%d",min,sec);
	}
	else
	{
		if(sec<10)
			sprintf(buf,"%2d:%2d'0%d",hour,min,sec);
		else
			sprintf(buf,"%2d:%2d'%d",hour,min,sec);
	}
}

void DrawTimer()
{
	double	d_time;
	char	RealTimer[9];

	d_time = FloatTime();
	GetRealTime(RealTimer,d_time);
	g_DispString(14,RealTimer);
}

//
// Gestion du timer du jeu ----------------------------------------- //
//
void t_TimerProcess()
{
	if(game_TimeLimit)
	{
		if(!game_MatrixMode)
		{
			if(game_clock > 0)
			{
				if(gIsMultiplayer)
				{
					if(gConnectedPlayer <= 1)
						goto no_decrease_time;
					if(!gIsServer)
						goto no_decrease_time;

					game_TimeLimit = true;
				}
				game_clock -= gFrameTime_msec * 0.001f;
				if(game_clock < 0)
					game_clock = 0;
				no_decrease_time:;
			}
			else
			{
				if(gIsMultiplayer)
				{
					//net_DispPodium();
				}
				else
				{
					if(gMapBonus == true)
					{
						game_InitYouWin();
						return;
					}
					else
					{
						if(game_isover == false)
						{
							game_isover = true;
							game_InitYouLose();
							return;
						}
					}
				}
			}
		}
	}
}

void UpdateTimers()
{
	char	tab[255];
	int		diff;
	int		monster_icon_h;
	int		monster_txt_h;
	int		icon_x, icon_y, icon_w, icon_h;

	if(gHeartSize > 1)
	{
		gHeartSize -= gFrameTime_msec * 0.001f;
		if(gHeartSize < 1)
			gHeartSize = 1;
	}

	if(gDeplaHudSize > 1)
	{
		gDeplaHudSize -= gFrameTime_msec * 0.0008f;
		if(gDeplaHudSize < 1)
			gDeplaHudSize = 1;
	}
	if(gMorbakHudSize > 1)
	{
		gMorbakHudSize -= gFrameTime_msec * 0.0008f;
		if(gMorbakHudSize < 1)
			gMorbakHudSize = 1;
	}
	if(gZebatHudSize > 1)
	{
		gZebatHudSize -= gFrameTime_msec * 0.0008f;
		if(gZebatHudSize < 1)
			gZebatHudSize = 1;
	}
	if(gZeratHudSize > 1)
	{
		gZeratHudSize -= gFrameTime_msec * 0.0008f;
		if(gZeratHudSize < 1)
			gZeratHudSize = 1;
	}
	if(gMummyHudSize > 1)
	{
		gMummyHudSize -= gFrameTime_msec * 0.0008f;
		if(gMummyHudSize < 1)
			gMummyHudSize = 1;
	}

	gFioleCatchedRad += gFrameTime_msec * 0.035f;
	if(gFioleCatchedRad > 180.0f)
		gFioleCatchedRad = 0;
	gFioleCatchedSize = 1 + (0.75f * Sin(gFioleCatchedRad));


	if(ProgramState == PS_WIN)
		return;

	if(ProgramState == PS_ENDSCORE)
		return;

	if(ProgramState != PS_GAME)
		return;

	if(KillZebat_Time > 0)
	{
		KillZebat_Time -= gFrameTime_msec;
		if(KillZebat_Time <= 0)
			KillZebat_Time = -1;
		sprintf(tab,"You killed a bunch of Zebats!");
		g_DispCenterString(24,9,tab);
	}

	if(KillMorbak_Time > 0)
	{
		KillMorbak_Time -= gFrameTime_msec;
		if(KillMorbak_Time <= 0)
			KillMorbak_Time = -1;
		sprintf(tab,"You killed a bunch of Morbaks!");
		g_DispCenterString(24,10,tab);
	}

	if(KillDepla_Time > 0)
	{
		KillDepla_Time -= gFrameTime_msec;
		if(KillDepla_Time <= 0)
			KillDepla_Time = -1;
		sprintf(tab,"You killed a bunch of Deplas!");
		g_DispCenterString(24,11,tab);
	}

	if(KillSubzerat_Time > 0)
	{
		KillSubzerat_Time -= gFrameTime_msec;
		if(KillSubzerat_Time <= 0)
			KillSubzerat_Time = -1;
		sprintf(tab,"You killed a bunch of SubZerat!");
		g_DispCenterString(24,11,tab);
	}

	if(KillZemummy_Time > 0)
	{
		KillZemummy_Time -= gFrameTime_msec;
		if(KillZemummy_Time <= 0)
			KillZemummy_Time = -1;
		sprintf(tab,"You killed a bunch of ZeMummy!");
		g_DispCenterString(24,11,tab);
	}

	if(HLetter_Time > 0)
	{
		HLetter_Time -= gFrameTime_msec;
		if(HLetter_Time < 0)
		{
			HLetter_Time = 0;
		}
		else
		{
			g_DispCenterString(24, 12, "You got %s", HalloLetters);
		}
	}

	if(game_MatrixMode > 0)
	{
		game_MatrixMode -= gFrameTime_msec;
		if(game_MatrixMode < 0)
			game_MatrixMode = 0;
	}

	if(game_FalconMode > 0)
	{
		game_FalconMode -= gFrameTime_msec;
		if(game_FalconMode < 0)
			game_FalconMode = 0;
	}

	if(game_InjuryMode > 0)
	{
		game_InjuryMode -= gFrameTime_msec;
		if(game_InjuryMode < 0)
			game_InjuryMode = 0;
	}

	// affiche l'icone de la fiole eventuellement ramassee
	if(game_fiole1_catched)
	{
		g_Disp(10,10,58,"PowerJump");
	}
	else if(game_fiole2_catched)
	{
		g_Disp(10,10,58,"MatrixMode");
	}
	else if(game_fiole3_catched)
	{
		g_Disp(10,10,58,"FalconView");
	}

	// icones des goals sur les monstres
	if(gMapBonus)
		goto no_icons;
	if(gIsMultiplayer)
		goto no_icons;
	if(gMapIntro)
		goto no_icons;
	if(game_HitBestScoreMode)
		goto no_icons;

	diff = (int)difficulty.value;
	monster_icon_h = 312;
	monster_txt_h = 17;

	if( MapGoals[1][diff][game_MapID] )
	{
		icon_x = 592;
		icon_y = monster_icon_h;
		icon_w = 32;
		icon_h = 32;
		if(gDeplaHudSize > 1)
		{
			icon_x -= (int)(16.0f*(gDeplaHudSize-1.0f));
			icon_y -= (int)(16.0f*(gDeplaHudSize-1.0f));
			icon_w = (int)(32.0f*gDeplaHudSize);
			icon_h = (int)(32.0f*gDeplaHudSize);
		}

		if(KillDepla_Time == -1)
			disp_DrawSprite(txid_DEPLA_ICON[0], icon_x, icon_y, icon_w, icon_h, true);
		else
			disp_DrawSprite(txid_DEPLA_ICON[1], icon_x, icon_y, icon_w, icon_h, true);

		monster_icon_h -= 51;

		g_Disp(10,monster_txt_h,60,"%d/%d",gKilledDeplas,MapGoals[1][diff][game_MapID]);
		monster_txt_h += 5;
	}

	if( MapGoals[2][diff][game_MapID] )
	{
		icon_x = 592;
		icon_y = monster_icon_h;
		icon_w = 32;
		icon_h = 32;
		if(gMorbakHudSize > 1)
		{
			icon_x -= (int)(16.0f*(gMorbakHudSize-1.0f));
			icon_y -= (int)(16.0f*(gMorbakHudSize-1.0f));
			icon_w = (int)(32.0f*gMorbakHudSize);
			icon_h = (int)(32.0f*gMorbakHudSize);
		}

		if(KillMorbak_Time == -1)
			disp_DrawSprite(txid_MORBAK_ICON[0], icon_x, icon_y, icon_w, icon_h, true);
		else
			disp_DrawSprite(txid_MORBAK_ICON[1], icon_x, icon_y, icon_w, icon_h, true);
		monster_icon_h -= 51;

		g_Disp(10,monster_txt_h,60,"%d/%d",gKilledMorbaks,MapGoals[2][diff][game_MapID]);
		monster_txt_h += 5;
	}

	if( MapGoals[3][diff][game_MapID] )
	{
		icon_x = 592;
		icon_y = monster_icon_h;
		icon_w = 32;
		icon_h = 32;
		if(gZebatHudSize > 1)
		{
			icon_x -= (int)(16.0f*(gZebatHudSize-1.0f));
			icon_y -= (int)(16.0f*(gZebatHudSize-1.0f));
			icon_w = (int)(32.0f*gZebatHudSize);
			icon_h = (int)(32.0f*gZebatHudSize);
		}

		if(KillZebat_Time == -1)
			disp_DrawSprite(txid_BAT_ICON[0], icon_x, icon_y, icon_w, icon_h, true);
		else
			disp_DrawSprite(txid_BAT_ICON[1], icon_x, icon_y, icon_w, icon_h, true);
		monster_icon_h -= 51;

		g_Disp(10,monster_txt_h,60,"%d/%d",gKilledZeubat,MapGoals[3][diff][game_MapID]);
		monster_txt_h += 5;
	}

	if( MapGoals[4][diff][game_MapID] )
	{
		icon_x = 592;
		icon_y = monster_icon_h;
		icon_w = 32;
		icon_h = 32;
		if(gZeratHudSize > 1)
		{
			icon_x -= (int)(16.0f*(gZeratHudSize-1.0f));
			icon_y -= (int)(16.0f*(gZeratHudSize-1.0f));
			icon_w = (int)(32.0f*gZeratHudSize);
			icon_h = (int)(32.0f*gZeratHudSize);
		}

		if(KillSubzerat_Time == -1)
			disp_DrawSprite(txid_SUB_ICON[0], icon_x, icon_y, icon_w, icon_h, true);
		else
			disp_DrawSprite(txid_SUB_ICON[1], icon_x, icon_y, icon_w, icon_h, true);
		monster_icon_h -= 51;

		g_Disp(10,monster_txt_h,60,"%d/%d",gKilledSubzerat,MapGoals[4][diff][game_MapID]);
		monster_txt_h += 5;
	}

	if( MapGoals[5][diff][game_MapID] )
	{
		icon_x = 592;
		icon_y = monster_icon_h;
		icon_w = 32;
		icon_h = 32;
		if(gMummyHudSize > 1)
		{
			icon_x -= (int)(16.0f*(gMummyHudSize-1.0f));
			icon_y -= (int)(16.0f*(gMummyHudSize-1.0f));
			icon_w = (int)(32.0f*gMummyHudSize);
			icon_h = (int)(32.0f*gMummyHudSize);
		}

		if(KillZemummy_Time == -1)
			disp_DrawSprite(txid_MOMY_ICON[0], icon_x, icon_y, icon_w, icon_h, true);
		else
			disp_DrawSprite(txid_MOMY_ICON[1], icon_x, icon_y, icon_w, icon_h, true);
		monster_icon_h -= 51;

		g_Disp(10,monster_txt_h,60,"%d/%d",gKilledZemummy,MapGoals[5][diff][game_MapID]);
		monster_txt_h += 5;
	}

no_icons:

	if(gAllGoalCleared && game_clock <= 0)
	{
		m_ConsPrint("end of score game\n");
		switch(game_MapID)
		{
			case 1:	if(gScore > bestscore1.value) bestscore1.value = (float)gScore;		break;
			case 2:	if(gScore > bestscore2.value) bestscore2.value = (float)gScore;		break;
			case 3: if(gScore > bestscore3.value) bestscore3.value = (float)gScore;		break;
			case 4:	if(gScore > bestscore4.value) bestscore4.value = (float)gScore;		break;
			case 5:	if(gScore > bestscore5.value) bestscore5.value = (float)gScore;		break;
			case 6:	if(gScore > bestscore6.value) bestscore6.value = (float)gScore;		break;
			case 7:	if(gScore > bestscore7.value) bestscore7.value = (float)gScore;		break;
			case 8:	if(gScore > bestscore8.value) bestscore8.value = (float)gScore;		break;
			case 9:	if(gScore > bestscore9.value) bestscore9.value = (float)gScore;		break;
		}		
		ProgramState = PS_ENDSCORE;
		return;
	}

	t_TimerProcess();
}

hBool SlowWorldTime(float msec)
{
	long	t;
	long	diff;
	
	t = disp_GetClock();

	diff = t-(long)gTime_slow;
	if(diff >= msec)
	{
		gTime_slow = t;
		return true;
	}
	return false;
}
