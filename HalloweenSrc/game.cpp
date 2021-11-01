
#include "alloween.h" //was .cpp ?? by farox

int		ProgramState;
int		NextProgramState;
int		saved_ProgramState;
int		game_HitBestScoreMode;
int		game_NbMaps=0;
int		game_fleches=0;
int		game_bullet=0;
int		game_bombes=0;
int		game_heart=0;
float	game_clock=0;
int		game_jade=0;
hBool	game_isover;
hBool	game_weapon1;
hBool	game_weapon2;
hBool	game_weapon3;
char	HalloLetters[9]; // = "_________";	// "HALLOWEEN" letters to find and pick up
float	HLetter_Time = 0;
float	KillDepla_Time = 0;
float	KillMorbak_Time = 0;
float	KillZebat_Time = 0;
float	KillSubzerat_Time = 0;
float	KillZemummy_Time = 0;
int		game_MapID;
Model	gModel_depla;
Model	gModel_morbak;
Model	gModel_zebat;
Model	gModel_subzerat;
Model	gModel_zemummy;
hBool	game_fiole1_catched;
hBool	game_fiole2_catched;
hBool	game_fiole3_catched;
float	game_MatrixMode;
float	game_FalconMode;
float	game_InjuryMode;
hBool	game_TofCheat = false;
hBool	game_TimeLimit;
float	game_DieTime;
float	game_WinTime;
int		MapGoals[10][3][9];		// goal - diff - map
float	MapTime[3];				// time
float	MonsterLife[5][3];		// monster - diff
float	WeaponDammage[5][3];	// weapon - diff
float	MonsterDammage[5][3];	// monster - diff
hBool	MassueCanHurt;
float	gHeartSize = 1;
float	gDeplaHudSize = 1;		//
float	gMorbakHudSize = 1;		//
float	gMummyHudSize = 1;		//
float	gZebatHudSize = 1;		//
float	gZeratHudSize = 1;		//
float	gFioleCatchedSize = 1;	//
float	gFioleCatchedRad = 0;	//
hBool	gAllGoalCleared;
int		gScore;
float	gOscilleGunsVal;
float	gCameraShowTime;
long	gClockOffset;
hBool	gCheatMode_pumpkin = false;
hBool	gCheatMode_monsterbomb = false;
hBool	gCheatMode_son = false;
hBool	gWelcomeToHalloween = true;
hBool	gGameEnded;
hBool	gIsDemo = false;

hBool	gLoad3Dstuff = false;
float	gCatchedTime;
hBool	gMapBonus;
hBool	gMapIntro;
char	*gCatchedString;
char	gCatchedString2[255];

hBool	gClientAskProtocol;
short	gConnectedPlayer = 0;
short	gMultiMapPlayed;
hBool	gMultiConnected;
hBool	gMultiClientReady;
short	gMultiNbEntsMax;
short	gMultiNbEnts;
short	gMultiSV_clentid;
short	gMultiSV_clentn;
vect_t	gMultiSV_clentstartpos;
hBool	gMultiCLleftgame;
hBool	gMultiPodium;

struct
{
	char	name[FILENAME_MAX];
	int		format;
	int		txid;
	int		txid_sel;
	int		txid_grey;

} maps [MAX_MAPFILE];



void DispLogo(int txid)
{
	disp_DrawSprite_2(txid,144,44,512,512,false);
}

void DispText(char *txt, int txid)
{
	gl_ClearScreen();
	DispLogo(txid);
	gl_SwapBuffer();
}

int game_Load2D(char *name)
{
	char	fullpath[260];

	sprintf(fullpath,"%s%s",SYSTEX,name);
	return im_Load(fullpath,-1,-1,false);
}

void game_Load2D_game_icons()
{
	txid_0=game_Load2D("0.tga");
	txid_1=game_Load2D("1.tga");
	txid_2=game_Load2D("2.tga");
	txid_3=game_Load2D("3.tga");
	txid_4=game_Load2D("4.tga");
	txid_5=game_Load2D("5.tga");
	txid_6=game_Load2D("6.tga");
	txid_7=game_Load2D("7.tga");
	txid_8=game_Load2D("8.tga");
	txid_9=game_Load2D("9.tga");
	txid_HEART=game_Load2D("heart.tga");
	txid_CLOCK=game_Load2D("clock.tga");
	txid_BOMBE=game_Load2D("bombe.tga");
	txid_FIOLE1_ICONE=game_Load2D("fiole1_icone.tga");
	txid_FIOLE2_ICONE=game_Load2D("fiole2_icone.tga");
	txid_FIOLE3_ICONE=game_Load2D("fiole3_icone.tga");
	txid_FLECHE=game_Load2D("arme_fleche.tga");
}

void game_Load2Dicons()
{
	int		i;
	char	fullpath[260];

	m_ConsPrint("game_Load2Dicons\n");

	txid_RESUME=game_Load2D("resume_icon.tga");
	txid_RESUME2=game_Load2D("resume2_icon.tga");
	txid_LEAVE=game_Load2D("leave_icon.tga");
	txid_LEAVE2=game_Load2D("leave2_icon.tga");
	txid_EXIT=game_Load2D("exit_icon.tga");
	txid_EXIT2=game_Load2D("exit2_icon.tga");

	txid_FLAME[0] = im_Load("flame/flame1.tga",-1,(int)picmip.value,false);
	txid_FLAME[1] = im_Load("flame/flame2.tga",-1,(int)picmip.value,false);
	txid_FLAME[2] = im_Load("flame/flame3.tga",-1,(int)picmip.value,false);
	txid_FLAME[3] = im_Load("flame/flame4.tga",-1,(int)picmip.value,false);
	txid_FLAME[4] = im_Load("flame/flame5.tga",-1,(int)picmip.value,false);
	txid_FLAME[5] = im_Load("flame/flame6.tga",-1,(int)picmip.value,false);
	txid_FLAME[6] = im_Load("flame/flame7.tga",-1,(int)picmip.value,false);
	txid_FLAME[7] = im_Load("flame/flame8.tga",-1,(int)picmip.value,false);

	txid_FONT = game_Load2D("font.tga");
	txid_NICEFONT = tga_Load("fonte/fonte.tga",-1,-1,false);
	txid_TARGET = game_Load2D("target.tga");
	txid_DEFAULTMAP = game_Load2D("defaultmap.tga");
	txid_OMBRAGE = tga_Load("ombrage.tga",-1,-1,false);
	txid_RAYURE=game_Load2D("rayure.tga");
	txid_COPY=game_Load2D("copyright.tga");
	txid_CURSOR=game_Load2D("cursor.tga");
	txid_GAMEOPTIONCONFIG=game_Load2D("gameoption_icon.tga");
	txid_INPUTCONFIG=game_Load2D("control_icon.tga");
	txid_VIDEOCONFIG=game_Load2D("video_icon.tga");
	txid_SOUNDCONFIG=game_Load2D("sound_icon.tga");
	txid_INPUTCONFIG2=game_Load2D("control2_icon.tga");
	txid_VIDEOCONFIG2=game_Load2D("video2_icon.tga");
	txid_SOUNDCONFIG2=game_Load2D("sound2_icon.tga");
	txid_GAMEOPTIONCONFIG2=game_Load2D("gameoption2_icon.tga");
	txid_BULLET=game_Load2D("bullet.tga");
	txid_BUTTON=game_Load2D("button.tga");
	txid_BUTTON_P=game_Load2D("button_p.tga");
	txid_BUTTON_M=game_Load2D("button_m.tga");
	txid_BUTTON_P_SEL=game_Load2D("button_p_sel.tga");
	txid_BUTTON_M_SEL=game_Load2D("button_m_sel.tga");
	txid_BUTTON_DEL=game_Load2D("button_del.tga");
	txid_BUTTON2_DEL=game_Load2D("button2_del.tga");
	txid_BUTTONGO=game_Load2D("button_play.tga");
	txid_BUTTONGO2=game_Load2D("button2_play.tga");
	txid_BUTTONNEXT=game_Load2D("button_next.tga");
	txid_BUTTONNEXT2=game_Load2D("button2_next.tga");
	txid_BUTTONBACK=game_Load2D("button_back.tga");
	txid_BUTTONBACK2=game_Load2D("button2_back.tga");
	txid_CONSOLE=game_Load2D("console.tga");
	txid_CITROUILLE=game_Load2D("citrouille.tga");
	txid_GRIS=game_Load2D("gris.tga");
	txid_ORDER=game_Load2D("order.tga");
	txid_HELPORDER = tga_Load("interface/tombz/help.tga",-1,-1,false);
	txid_HELPORDER2 = tga_Load("interface/tombz/help_sel.tga",-1,-1,false);
	txid_HELP=game_Load2D("help.tga");
	txid_BONUS1=game_Load2D("bonus1_logo.tga");
	txid_CREDIT=game_Load2D("credit.tga");
	txid_BG1 = tga_Load("interface/bg_part1.tga",-1,-1,false);
	txid_BG2 = tga_Load("interface/bg_part2.tga",-1,-1,false);
	txid_BG2_G = tga_Load("interface/bg_part2_gris.tga",-1,-1,false);
	txid_TNN[0] = tga_Load("interface/title/tnn_1.tga",-1,-1,false);
	txid_TNN[1] = tga_Load("interface/title/tnn_2.tga",-1,-1,false);
	txid_STARTGAME = tga_Load("interface/tombz/game.tga",-1,-1,false);
	txid_STARTGAME2 = tga_Load("interface/tombz/game_sel.tga",-1,-1,false);
	txid_CONFIG = tga_Load("interface/tombz/option.tga",-1,-1,false);
	txid_CONFIG2 = tga_Load("interface/tombz/option_sel.tga",-1,-1,false);
	txid_CONFIG3 = game_Load2D("config.tga");
	txid_CONFIG4 = game_Load2D("config2.tga");
	txid_ORDERGAME = tga_Load("interface/tombz/order.tga",-1,-1,false);
	txid_ORDERGAME2 = tga_Load("interface/tombz/order_sel.tga",-1,-1,false);
	txid_QUITGAME = tga_Load("interface/tombz/quit.tga",-1,-1,false);
	txid_QUITGAME2 = tga_Load("interface/tombz/quit_sel.tga",-1,-1,false);
	txid_STARTGAME_G = tga_Load("interface/tombz/game_gris.tga",-1,-1,false);
	txid_HELPORDER_G = tga_Load("interface/tombz/help_gris.tga",-1,-1,false);
	txid_ORDERGAME_G = tga_Load("interface/tombz/order_gris.tga",-1,-1,false);
	txid_QUITGAME_G = tga_Load("interface/tombz/quit_gris.tga",-1,-1,false);
	txid_OPTION_G = tga_Load("interface/tombz/option_gris.tga",-1,-1,false);
//	txid_JADETEAM = tga_Load("systex/jt.tga",-1,-1,false);
	txid_SELECTGAMESLOT = tga_Load("interface/game/selectgameslot.tga",-1,-1,false);
	txid_EASY = tga_Load("interface/game/easy.tga",-1,-1,false);
	txid_NORMAL = tga_Load("interface/game/normal.tga",-1,-1,false);
	txid_HARD = tga_Load("interface/game/hard.tga",-1,-1,false);
	txid_EMPTY = tga_Load("interface/game/empty.tga",-1,-1,false);
	txid_EASY_G = tga_Load("interface/game/easy_b.tga",-1,-1,false);
	txid_NORMAL_G = tga_Load("interface/game/normal_b.tga",-1,-1,false);
	txid_HARD_G = tga_Load("interface/game/hard_b.tga",-1,-1,false);
	txid_EMPTY_G = tga_Load("interface/game/empty_b.tga",-1,-1,false);
	txid_TDM = tga_Load("interface/game/tdm.tga",-1,-1,false);
	txid_TDM_G = tga_Load("interface/game/tdm_b.tga",-1,-1,false);
	txid_YES = tga_Load("interface/game/yes.tga",-1,-1,false);
	txid_YES_G = tga_Load("interface/game/yes_b.tga",-1,-1,false);
	txid_NO = tga_Load("interface/game/no.tga",-1,-1,false);
	txid_NO_G = tga_Load("interface/game/no_b.tga",-1,-1,false);


	for(i=0 ; i<30 ; i++)
	{
		sprintf(fullpath,"interface/title/tt_%.2d.tga",i);
		txid_NEWTITLE[i] = tga_Load(fullpath,-1,-1,false);
	}

	for(i=0 ; i<24 ; i++)
	{
		sprintf(fullpath,"interface/tombz/game_anim/ga_%.2d.tga",i+1);
		txid_GAMEANIM[i] = tga_Load(fullpath,-1,-1,false);
	}
	for(i=0 ; i<15 ; i++)
	{
		sprintf(fullpath,"interface/tombz/help_anim/ha_%.2d.tga",i+1);
		txid_HELPANIM[i] = tga_Load(fullpath,-1,-1,false);
	}
	for(i=0 ; i<14 ; i++)
	{
		sprintf(fullpath,"interface/tombz/option_anim/oa_%.2d.tga",i+1);
		txid_OPTIONANIM[i] = tga_Load(fullpath,-1,-1,false);
	}
	for(i=0 ; i<19 ; i++)
	{
		sprintf(fullpath,"interface/tombz/order_anim/da_%.2d.tga",i+1);
		txid_ORDERANIM[i] = tga_Load(fullpath,-1,-1,false);
	}
	for(i=0 ; i<25 ; i++)
	{
		sprintf(fullpath,"interface/tombz/quit_anim/qa_%.2d.tga",i+1);
		txid_QUITANIM[i] = tga_Load(fullpath,-1,-1,false);
	}

	txid_DEPLA_ICON[0] = tga_Load("goals/depla_ok.tga",-1,-1,false);
	txid_DEPLA_ICON[1] = tga_Load("goals/depla_no.tga",-1,-1,false);
	txid_MORBAK_ICON[0] = tga_Load("goals/morbak_ok.tga",-1,-1,false);
	txid_MORBAK_ICON[1] = tga_Load("goals/morbak_no.tga",-1,-1,false);
	txid_SUB_ICON[0] = tga_Load("goals/subzerat_ok.tga",-1,-1,false);
	txid_SUB_ICON[1] = tga_Load("goals/subzerat_no.tga",-1,-1,false);
	txid_BAT_ICON[0] = tga_Load("goals/zebat_ok.tga",-1,-1,false);
	txid_BAT_ICON[1] = tga_Load("goals/zebat_no.tga",-1,-1,false);
	txid_MOMY_ICON[0] = tga_Load("goals/zemomy_ok.tga",-1,-1,false);
	txid_MOMY_ICON[1] = tga_Load("goals/zemomy_no.tga",-1,-1,false);
	txid_TOILE = tga_Load("toile.tga",-1,-1,false);
}

void game_LoadEntities()
{
	o_LoadEntityToStruct("perso.ent",PERSO);
	o_LoadEntityToStruct("gun1.ent",GUN1);
	o_LoadEntityToStruct("gun1b.ent",GUN1B);
	o_LoadEntityToStruct("gun2.ent",GUN2);
	o_LoadEntityToStruct("gun3.ent",GUN3);
	o_LoadEntityToStruct("gun3b.ent",GUN3B);
	o_LoadEntityToStruct("spike.ent",SPIKE);
	o_LoadEntityToStruct("fleche.ent",FLECHE);
	o_LoadEntityToStruct("spine.ent",SPINE);
	o_LoadEntityToStruct("masse.ent",SABRE);
	o_LoadEntityToStruct("bombe.ent",BOMBE);
	o_LoadEntityToStruct("monstre.ent",DEPLA);
	o_LoadEntityToStruct("monstre.ent",MORBAK);
	o_LoadEntityToStruct("skydome.ent",SKYDOME);
	o_LoadEntityToStruct("skydome2.ent",SKYDOME2);

	if(notextures.value)
		return;

	o_LoadEntityToStruct("amo1.ent",AMO1);
	o_LoadEntityToStruct("amo2.ent",AMO2);
	o_LoadEntityToStruct("amo3.ent",AMO3);
	o_LoadEntityToStruct("explosion.ent",EXPLOSION);
	o_LoadEntityToStruct("jade.ent",JADE);
	o_LoadEntityToStruct("lift.ent",LIFT);
	o_LoadEntityToStruct("pang.ent",PANG);
	o_LoadEntityToStruct("fiole1.ent",FIOLE1);
	o_LoadEntityToStruct("fiole2.ent",FIOLE2);
	o_LoadEntityToStruct("fiole3.ent",FIOLE3);
	o_LoadEntityToStruct("smoke.ent",SMOKE);
	o_LoadEntityToStruct("lampe.ent",LAMPE);
	o_LoadEntityToStruct("lampemur.ent",MLAMPE);
	o_LoadEntityToStruct("pumpkin.ent",PUMPKIN);
	o_LoadEntityToStruct("H.ent",H);
	o_LoadEntityToStruct("A.ent",A);
	o_LoadEntityToStruct("L.ent",L);
	o_LoadEntityToStruct("O.ent",O);
	o_LoadEntityToStruct("W.ent",W);
	o_LoadEntityToStruct("E.ent",E);
	o_LoadEntityToStruct("N.ent",N);
	o_LoadEntityToStruct("toile.ent",TOILE);
	o_LoadEntityToStruct("deplaspike.ent",DEPLASPIKE);
	o_LoadEntityToStruct("depla_head.ent",DEPLAHEAD);
	o_LoadEntityToStruct("blood.ent",BLOOD);
	o_LoadEntityToStruct("morbak_body.ent",MORBAKBODY);
}

void game_PlayWeirdSound()
{
	switch(randInt(0,5))
	{
		case 0:
		ds_PlaySound(31);
		break;

		case 1:
		ds_PlaySound(32);
		break;

		case 2:
		ds_PlaySound(35);
		break;

		case 3:
		ds_PlaySound(36);
		break;

		case 4:
		ds_PlaySound(37);
		break;
	}
}

void game_Eclair()
{
	if(gMapIntro == false)
		return;
	if(gIsFog == false)
		return;
	gl_StopFog();
	gIsEclair = (rand()%160) + 100.0f; // 35.0f;
}

void game_PlayThunderSound()
{
	static float ThunderTime = 0;
	static float SonfalTime = 0;
	int		randval;
	static	hBool	eclairLancee = false;

	if(/*ProgramState == PS_GAME &&*/ gMapIntro == false)
		return;

	if(gIsEclair)
	{
		gIsEclair -= gFrameTime_msec;
		if(gIsEclair <= 0)
		{
			gIsEclair = 0;
			gl_SetFog();
		}
	}

	SonfalTime -= gFrameTime_msec;
	if(SonfalTime <= 0)
	{
		SonfalTime = 10000.0f;
		randval = randInt(0,11);
		ds_PlaySound(85+randval);
	}

	ThunderTime -= gFrameTime_msec;
	if(ThunderTime <= 0)
	{
		eclairLancee = false;
		ThunderTime = 3000.0f + (float)randInt(0, 3000);
	}
	else
	{
		if(eclairLancee == false && gIsEclair == 0 && ThunderTime < rand()%200) // 300
		{
			eclairLancee = true;
			game_Eclair();
		}
		return;
	}

//	if(ProgramState == PS_GAME && gMapIntro == false)
//		return;

	if(developer.value)
		return;

	switch(randInt(0,4))
	{
		case 0:
		ds_PlaySound(19);
		break;

		case 1:
		ds_PlaySound(22);
		break;

		case 2:
		ds_PlaySound(30);
		break;
	}
}

hBool game_Load2Dstuff()
{
	if(!g_LoadTextureStruct())
		return false;
	g_ResetTextureStruct();
	g_InitFont();
	game_PlayThunderSound();
	txid_LOGO = game_Load2D("logo.tga");
	txid_LOGO2 = game_Load2D("logo2.tga");
	DispText("",txid_LOGO);
	game_Load2Dicons();
	return true;
}

hBool game_Load3Dstuff()
{
	ANIMENTITY	anim;
	pEntity		Gun1bEnt;
	pEntity		Gun3bEnt;
	pFace		Face;
	pFace		pCurr;
	int			i;

	if(!o_LoadEntStruct())
		return false;
	game_LoadEntities();
	

	Gun1LaunchedFaceList = liste_Delete(Gun1LaunchedFaceList);
	//Gun1LaunchedFaceList = NULL;

	o_ClearAnimFields(&anim);
	o_AddEntityToList(GUN1B,-1,anim,false);
	Gun1bEnt = o_FindEntity(GUN1B);

	if(Gun1bEnt)
	{
		for(pCurr=Gun1bEnt->FaceList ; pCurr ; pCurr=pCurr->Next)
		{
			Face = (Face_t*)s_malloc(sizeof(Face_t));
			memset(Face,0,sizeof(Face_t));

			Face->NumberOfVertices = pCurr->NumberOfVertices;
			for(i=0 ; i<pCurr->NumberOfVertices ; i++)
			{
				Face->Vertice[i] = pCurr->Vertice[i];
			}
			Face->Plan = pCurr->Plan;
			Face->FACEID = pCurr->FACEID;
			Face->LightID = pCurr->FACEID;
			Face->switched = pCurr->switched;
			Face->TextureID = pCurr->TextureID;
			Face->Type = pCurr->Type;

			Face->Next = Gun1LaunchedFaceList;
			Gun1LaunchedFaceList = Face;
		}
		o_RemoveEntityFromList(Gun1bEnt);
	}
	else
	{
		Gun1LaunchedFaceList = NULL;
	}

	// trombon interieur

	Gun3FaceList = liste_Delete(Gun3FaceList);
	//Gun3FaceList = NULL;

	o_ClearAnimFields(&anim);
	o_AddEntityToList(GUN3B,-1,anim,false);
	Gun3bEnt = o_FindEntity(GUN3B);

	if(Gun3bEnt)
	{
		for(pCurr=Gun3bEnt->FaceList ; pCurr ; pCurr=pCurr->Next)
		{
			Face = (Face_t*)s_malloc(sizeof(Face_t));
			memset(Face,0,sizeof(Face_t));

			Face->NumberOfVertices = pCurr->NumberOfVertices;
			for(i=0 ; i<pCurr->NumberOfVertices ; i++)
			{
				Face->Vertice[i] = pCurr->Vertice[i];
			}
			Face->Plan = pCurr->Plan;
			Face->FACEID = pCurr->FACEID;
			Face->LightID = pCurr->FACEID;
			Face->switched = pCurr->switched;
			Face->TextureID = pCurr->TextureID;
			Face->Type = pCurr->Type;

			Face->Next = Gun3FaceList;
			Gun3FaceList = Face;
		}
		o_RemoveEntityFromList(Gun3bEnt);
	}
	else
	{
		Gun3FaceList = NULL;
	}

	return true;
}

hBool game_LoadFileMap()
{
	char	*p;
	
#ifdef H_WINDOWS
	int	j, k;
	int	idtmp;
	char	*c;
#endif
	int		i;
	char	tmp[255];
	char	full[255];

	game_NbMaps = 0;

	for(i=0 ; i<12 ; i++)
	{
		if(i>=9)
		{
			sprintf(maps[i].name, "mapbonus%d.map", i-9);
		}
		else
		{
			if(i<3)
			{
				sprintf(maps[i].name, "h1m%d.map", i);
			}
			else
			{
				sprintf(maps[i].name, "h1m%d.map", i+1);
			}
		}
	}

	for(i=0 ; i<MAX_MAPFILE ; i++)
	{
		if(i>=12)
			break;

//		strcpy(tmp,maps[i].name);
	//	p = fs_RenameFileExtension(tmp,"tga");
	//	sprintf(full,"%s%s",LVLSHT,p);
	//	sprintf(full, "%sh1mX.tga", LVLSHT);
//		maps[i].txid = im_Load(full,-1,-1,false);
//		if(maps[i].txid < 0)
//		{
			maps[i].txid = txid_DEFAULTMAP;
//		}

#ifdef H_WINDOWS
		for(c=maps[i].name ; *c ; c++)
		if(*c == ' ')
		{
			*c = '\0';
			break;
		}
#endif

		game_NbMaps++;
	}

        
#ifdef H_WINDOWS
	for(j=0 ; j<i ; j++)
	for(k=0 ; k<i ; k++)
	{
		if(j==k)
			continue;
		if(fs_OrderName(maps[k].name,maps[j].name))
		{
			idtmp = maps[k].txid;
			maps[k].txid = maps[j].txid;
			maps[j].txid = idtmp;
		}
	}
#endif

	return true;
}

hBool game_LoadModels()
{

	if(!msk_LoadData(&gModel_depla,"depla"))
	{
		game_QuitMessage("loadModelData: depla.ms3d FAILED!");
		return false;
	}
	if(!msk_LoadData(&gModel_morbak,"morbak"))
	{
		game_QuitMessage("loadModelData: morbak.ms3d FAILED!");
		return false;
	}

	if(!msk_LoadData(&gModel_zebat,"zebat"))
	{
		game_QuitMessage("loadModelData: zebat.ms3d FAILED!");
		return false;
	}
	if(!msk_LoadData(&gModel_subzerat,"subzerat"))
	{
		game_QuitMessage("loadModelData: subzerat.ms3d FAILED!");
		return false;
	}
	if(!msk_LoadData(&gModel_zemummy,"zemummy"))
	{
		game_QuitMessage("loadModelData: zemummy.ms3d FAILED!");
		return false;
	}
	msk_GetModelBound(&gModel_depla,DEPLA);
	msk_GetModelBound(&gModel_morbak,MORBAK);
	msk_GetModelBound(&gModel_zebat,ZEBAT);
	msk_GetModelBound(&gModel_subzerat,SUBZERAT);
	msk_GetModelBound(&gModel_zemummy,ZEMUMMY);
	return true;
}

void game_UnloadModels()
{
	msk_unloadModelData(&gModel_depla);
	msk_unloadModelData(&gModel_morbak);
	msk_unloadModelData(&gModel_zebat);
	msk_unloadModelData(&gModel_subzerat);
	msk_unloadModelData(&gModel_zemummy);
}

hBool game_Load()
{
	srand((unsigned int)time(NULL));

	if(!game_Load2Dstuff())
		return false;

	if(!game_LoadFileMap())
		return false;

	IniTimer();
	InitTrigoTables();
	fx_InitParticule();
	return true;
}

void game_CameraViewInit()
{
	if(!gIsMultiplayer)
	{
		player_view.value = 1;
	}
	else
	{
		if(gIsServer)
		{
			player_view.value = 1;
		}
		else
		{
			//player_view.value = 0; // spectate mode
			player_view.value = 1;
		}
	}
}

void game_Init()
{
	game_CameraViewInit();
	gForceNoDepthTest = false;
	gPlayerGunRecul = 0;
	gPlayerBobWalkstep = 0;
	gPlayerWalk = false;
	gPlayerWalkTime = 0;
	game_DieTime = 0;
	WorldColorR = 1;
	WorldColorG = 1;
	WorldColorB = 1;
	gLavaOff_sin = 0;
	gLavaOff_cos = 0;
	IsColorScreen=false;
	ResumingAlloweed=false;
	menu_InitPresentation();						//
//	game_clock = MapTime[(int)difficulty.value];	//
	game_heart = 100;

	if(gIsDemo)
	{
		game_fleches = 10;
		game_bullet = 10;
		game_bombes = 10;
	}
	else
	{
		game_fleches = 0;
		game_bullet=0;
	    game_bombes=0;
	}

	game_jade=0;
	game_isover = false;
	gScore = 0;
	gAllGoalCleared = false;
	
	game_weapon1 = false;
	game_weapon2 = false;
	game_weapon3 = false;

	if(gIsMultiplayer)
	{
		gMultiPodium = false;
		game_weapon1 = true;
		game_weapon2 = true;
		game_weapon3 = true;
	}

	o_RemoveHudFlask();
	o_RemoveHudLetters();
	game_fiole1_catched = false;
	game_fiole2_catched = false;
	game_fiole3_catched = false;
	game_TimeLimit = true;
	game_MatrixMode = 0;
	game_FalconMode = 0;
	game_InjuryMode = 0;
	gDispLetters = false;
	gKilledDeplas = 0;
	gKilledMorbaks = 0;
	gKilledZeubat = 0;
	gKilledZemummy = 0;
	gKilledSubzerat = 0;
	gPlayerHasGun=-1;
	HLetter_Time = 0;
	KillDepla_Time = 0;
	KillMorbak_Time = 0;
	KillZebat_Time = 0;
	KillSubzerat_Time = 0;
	KillZemummy_Time = 0;
	gCatchedTime = 0;
	gCatchedString = NULL;
	MassueCanHurt = true;
	strcpy(HalloLetters,"_________");
	game_InitMapGoals();
	gMapBonus = false;
	gMapIntro = false;
	game_HitBestScoreMode = false;
	gOscilleGunsVal = 0;
	gCameraShowTime = 0;
	gIsFog = false;
	gIsEclair = 0;
	gAlphaFont = 1;

#ifdef H_LINUX // beta 1 HACK
	ProgramState = PS_PRES;
	return;
#endif

//#ifdef H_MAC // beta 1 HACK
//	ProgramState = PS_PRES;
//	return;
//#endif

	if(ProgramState == PS_LOADING)
	{
		ProgramState = PS_LOGO;
		disp_SetFading2D_Out(PS_PRES);
		DispLogo(txid_LOGO-1);
		DispLogo(txid_LOGO);
	}
	else
	{
		ProgramState = PS_PRES;
	}
}

void game_InitGoalCounters()
{
	int		n_goal;
	int		diff;

	int		Max_DeplaToKill;
	int		Max_MorbakToKill;
	int		Max_ZeBatToKill;
	int		Max_SubZeratToKill;
	int		Max_ZeMummyToKill;
	//float	zoom = 3.0f; // 1.6f

	diff = (int)difficulty.value;

	Max_DeplaToKill = MapGoals[1][diff][game_MapID];
	Max_MorbakToKill = MapGoals[2][diff][game_MapID];
	Max_ZeBatToKill = MapGoals[3][diff][game_MapID];
	Max_SubZeratToKill = MapGoals[4][diff][game_MapID];
	Max_ZeMummyToKill = MapGoals[5][diff][game_MapID];

	n_goal = MapGoals[1][diff][game_MapID];
	if(n_goal)
	{
		if(game_ReadBit((int)kill_depla.value, game_MapID))
		{
			gKilledDeplas = Max_DeplaToKill;
			KillDepla_Time = -1;
		}
	}

	n_goal = MapGoals[2][diff][game_MapID];
	if(n_goal)
	{
		if(game_ReadBit((int)kill_morbak.value, game_MapID))
		{
			gKilledMorbaks = Max_MorbakToKill;
			KillMorbak_Time = -1;
		}
	}

	n_goal = MapGoals[3][diff][game_MapID];
	if(n_goal)
	{
		if(game_ReadBit((int)kill_zeubat.value, game_MapID))
		{
			gKilledZeubat = Max_ZeBatToKill;
			KillZebat_Time = -1;
		}
	}

	n_goal = MapGoals[4][diff][game_MapID];
	if(n_goal)
	{
		if(game_ReadBit((int)kill_subzerat.value, game_MapID))
		{
			gKilledSubzerat = Max_SubZeratToKill;
			KillSubzerat_Time = -1;
		}
	}
	
	n_goal = MapGoals[5][diff][game_MapID];
	if(n_goal)
	{
		if(game_ReadBit((int)kill_zemummy.value, game_MapID))
		{
			gKilledZemummy = Max_ZeMummyToKill;
			KillZemummy_Time = -1;
		}
	}
}

void game_QuitMessage(char *message)
{
	memset(ErrorMessage,0,255);
	memcpy(ErrorMessage,message,255);
	ProgramState = PS_QUIT;
}

void QuitGame()
{
	char	path[255];

	//if(gIsMultiplayer)
	//net_UnInit();

	sprintf(path,"%s%s",SYSDIR,CONFIG);
	cons_SaveConfig(path);
	UnloadMap();
	game_UnloadModels();
	im_UnloadTextures();
	g_UnloadTextureStruct();
	o_UnloadEntStruct();
}

void game_catchedinit2(char *Text)
{
	gCatchedTime = 2*CATCHED_TIME;
	strcpy(gCatchedString2, Text);
	gCatchedString = gCatchedString2; //Text;
}

void game_catchedinit(int text_id)
{
	char	*texts[17] = {	"arf",
							"You picked up the arbalett",
							"You picked up the bombgun",
							"You picked up the tromblon",
							"You picked up arbalett ammo",
							"You picked up bombgun ammo",
							"You picked up tromblon ammo",
							"You picked up some life",
							"You picked up the red flask",
							"You picked up the green flask",
							"You picked up the blue flask",
							"You klegged depla",
							"You klegged morbak",
							"You klegged zebat",
							"You klegged subzerat",
							"You klegged zemummy"
						};

	gCatchedTime = CATCHED_TIME;
	gCatchedString = texts[text_id];
}

void game_catchedprocess()
{
	if(gCatchedTime)
	{
		gCatchedTime -= gFrameTime_msec;
		if(gCatchedTime < 0)
			gCatchedTime = 0;
		if(gCatchedString)
			g_DispString(CONSOLE_FONT,gCatchedString);
	}
}

void game_EngineProcess()
{
	float	temp1=0, temp2=0, temp3=0;

	if(ProgramState == PS_PAUSEDGAME)
	{
		temp1 = WorldColorR;
		temp2 = WorldColorG;
		temp3 = WorldColorB;
		WorldColorR = 1;
		WorldColorG = 1;
		WorldColorB = 1;
	}

	gl_ClearScreen();
	cam_EarthQuake_process();
	game_CameraShowProcess();
	o_UpdateEntities();
	//o_OscilleYprocess();
	disp_Scene();
	if(gGameEnded && game_IsCameraShow())
	{
		game_DrawAuthorNames();
	}
	game_PlayThunderSound();

	if(ProgramState == PS_PAUSEDGAME)
	{
		WorldColorR = temp1;
		WorldColorG = temp2;
		WorldColorB = temp3;
	}

	if(ProgramState == PS_GAME || ProgramState == PS_DIE || ProgramState == PS_WIN)
	{
		if(gMenu_InitFirstSel)
			gMenu_InitFirstSel = false;
		UpdatePlayer();
		UpdateTimers();
		fx_UpdateParticule();
		fx_UpdateColorScreen();
		script_Run();
		o_RespawnProcess();
		game_catchedprocess();
	}
	else if(gIsMultiplayer && ProgramState == PS_PAUSEDGAME)
	{
	}
	else if(gAllGoalCleared && ProgramState == PS_ENDSCORE)
	{
		fx_UpdateParticule();
		fx_UpdateColorScreen();
		script_Run();
		o_RespawnProcess();

		WorldColorR = 1;
		WorldColorG = 1;
		WorldColorB = 1;

		g_DispCenterString(24, 6, "PAUNDEP : Congratulations!");
		g_DispCenterString(24, 7, "Your Score: %d", gScore);
		g_Disp(12, 37, 3, "Hit ESC key");

		{
			static float angle_sin = 0;
			static float angle_sin2 = 0;
			static float angle_sin3 = 0;

			angle_sin += 0.01f * gFrameTime_msec;
			if(angle_sin >= 6.28f) angle_sin -= 6.28f;
			gCamera.Pos.Y += gFrameTime_msec * 0.0015f * Sin(angle_sin);

			angle_sin2 += 0.00012f * gFrameTime_msec;
			if(angle_sin2 >= 6.28f) angle_sin2 -= 6.28f;
			gCamera.Rot.X += gFrameTime_msec * 0.002f * Sin(angle_sin2);

			angle_sin3 += 0.0002f * gFrameTime_msec;
			if(angle_sin3 >= 6.28f) angle_sin3 -= 6.28f;
			gCamera.Rot.Y += gFrameTime_msec * 0.002f * Sin(angle_sin3);
		}
	}

	if(gAllGoalCleared && ProgramState == PS_GAME)
	{
		g_DispString(16,"Score: %d", gScore);
	}
	
//	if(gDispLetters && ProgramState == PS_GAME)
//	{
//		g_DispString(16,"%s",HalloLetters);
//	}
	
	disp_Fading2D_Process();

	//m_ReadInputs(&MouseInput);
}

void g_LoadMultiMap()
{
	int		map_id;
	int		map_id_hack;
	char	tab[255];


	map_id = gMultiMapID - 1;

	map_id_hack = gMultiMapID;
	if(map_id_hack < 4 )
		map_id_hack --;

	sprintf(tab,"h1m%d.map",map_id_hack);

	if(gIsServer)
	{
		game_TimeLimit = true;
		game_clock = MULTI_TIMELIMIT;
	}
	else
	{
		game_TimeLimit = false;
		game_clock = 0;
	}

	strcpy(mapfile.string,tab);
	txid_LOADINGMAP = maps[map_id].txid;
	disp_DrawSprite_2(txid_LOADINGMAP,0,0,800,600,false);
	gl_SwapBuffer();

	ds_LoadStreamWAVE(game_GetMusicString((int)music_track.value));
	music_track.value++;
	music_track.value = (float)((int)music_track.value % 7);

	game_MapID = map_id;
	gMapBonus = false;
	gMapIntro = false;
	LoadMap();

	ProgramState = PS_GAME;

	gMultiConnected = true;
}

hBool GameProcess()
{
	pEntity	PlayerEnt = NULL;

	// trouve l'entitee du joueur si disponible
	if(gIsMultiplayer && !gIsServer)
	{
		PlayerEnt = o_FindEntity(MORBAK);
	}
	else
	{
		PlayerEnt = o_FindEntity(PERSO);
	}
	
#ifdef H_MAC
	IN_Process(MouseInput,PlayerEnt);
#endif

	switch(ProgramState)
	{
		case PS_DEBUG:
		break;
			
		case PS_ENDSCORE:
		game_EngineProcess();
		m_ConsoleProcess();
		m_ReadInputs(&MouseInput);
		m_ProcessInputs(MouseInput);
		break;

		case PS_GAME:
		//IN_WarpMouse();
		game_EngineProcess();
		Story_Process();
		//Story_Intro();
		if(gIsMultiplayer)
		{
			//net_LagometerProcess();
			if(gMultiCLleftgame)
			{
				gMultiCLleftgame = false;
				// supprime l'entitee du client qui vient de quitter le jeu
				if(gMultiSV_clentid != -1 && gMultiSV_clentn != -1)
				{
					pEntity ClientEnt;

					ClientEnt = o_FindEntity2(gMultiSV_clentid, gMultiSV_clentn);
					if(ClientEnt)
					{
						o_RemoveEntityFromList(ClientEnt);
					}
				}
			}
		}
		m_ConsoleProcess();
		m_ReadInputs(&MouseInput);
		m_ProcessInputs(MouseInput);
		break;

		case PS_LOADCLIENT:
		if(gMultiClientReady)
		{
			// tout est pret, la partie va pouvoir commencer
			m_ConsPrint("client is ready, let's start the multiplayer game\n");
			ProgramState = PS_GAME;
		}
		game_EngineProcess();
		m_ConsoleProcess();
		m_ReadInputs(&MouseInput);
		m_ProcessInputs(MouseInput);
		break;

		case PS_LOADENT:
		if((gMultiNbEntsMax != -1) && (gMultiNbEnts >= gMultiNbEntsMax))
		{
			//	if(developer.value)
			m_ConsPrint("Le client a re\E7u les entitees dans la map en cours du serveur\n");		
			m_ConsPrint("client envoi une demande de point d'apparition\n");
#ifdef H_WINDOWS // hack
			//cl_Send("erf", PacketType_AskSpawnPos, true, true);
#endif
			ProgramState = PS_LOADCLIENT;
			break;			
		}
		game_EngineProcess();
		m_ConsoleProcess();
		m_ReadInputs(&MouseInput);
		m_ProcessInputs(MouseInput);
		break;

		case PS_CONNECTING:
		if(gMultiMapPlayed >= 1 && gMultiMapPlayed <= 9)
		{
			gMultiMapID = gMultiMapPlayed;
			g_LoadMultiMap();
			
			gMultiClientReady = false;

			m_ConsPrint("client envoi une demande des entitees au serveur\n");
#ifdef H_WINDOWS // hack
			//cl_Send("ptdr", PacketType_AskEntities, true, true);
#endif

			ProgramState = PS_LOADENT;
			break;
		}

		game_EngineProcess();
		m_ConsoleProcess();
		m_ReadInputs(&MouseInput);
		m_ProcessInputs(MouseInput);
		break;

		case PS_LOADING:
		if(!game_Load())
		{
			if(ErrorMessage[0] == '\0')
				game_QuitMessage("LoadGame FAILED");
			sys_killVID();
			sys_end();
		}

		if(!gIsMultiplayer)
			game_LaunchPresMusic();
		
		game_Init();
                
		ds_AdjustSoundVolumes();
		ds_AdjustMusicVolumes();

		// si on est en mode multiplayer on saute la presentation
		if(gIsMultiplayer)
		{
			terminator.value = 1;	// HACK : ra3 style mutliplayer mode !

			if(gIsServer)
			{
				game_CameraViewInit();
				//sv_Create();
				//sv_BeginHost("TEST");
				gConnectedPlayer = 0;
				gMultiSV_clentid = -1;
				gMultiSV_clentn = -1;
				gMenu_InitFirstSel = false;
				g_LoadMultiMap();

				//net_Init();
			}
			else
			{
				menu_DisplayDefaultMap();
				g_DispCenterString(28, 3, "Connecting to.. %s", net_ip.string);
				gl_SwapBuffer();
				/*
				if(!cl_Create())
				{
					game_Init();
				}
				else
				{
					game_CameraViewInit();
					gMultiMapPlayed = -1;
					gClientAskProtocol = true;
					gMultiNbEntsMax = -1;
					gMultiNbEnts = 0;
	#ifdef H_WINDOWS // HACK
					gMultiCLleftgame = false;
	#endif				
			
					m_ConsPrint("client envoi une demande de la version du protocol au serveur\n");
	#ifdef H_WINDOWS // HACK
					cl_Send("saledep", PacketType_AskProtocol, true, true);
	#endif
					
					gMenu_InitFirstSel = false;
					ProgramState = PS_CONNECTING;

	#ifdef H_WINDOWS // HACK
					net_Init();
	#endif
				}*/
			}
		}
		break;

		case PS_LOGO:
		gl_ClearScreen();
		DispLogo(txid_LOGO);
		disp_GetFrameTime();
		disp_Fading2D_Process();
		break;

		case PS_INTROMAP:
//		glClearColor(1,0.4,0,0);
		gl_ClearScreen();
		DispLogo(txid_LOGO2);
		disp_GetFrameTime();
		disp_Fading2D_Process();
		if(disp_IsFadeDone())
		{
			DispLogo(txid_LOGO2-1);
			game_LoadMap(-1);
			Story_SetText(0);
			game_InitCameraShow_letter();
		}
		glClearColor(0,0,0,0);
		break;

		case PS_LAUNCHMAPINTRO:
		game_LaunchMapIntro();
		break;

		case PS_PRES:
		game_EngineProcess();
		DrawGameTitle();
		menu_SelectProcess_PRES();
		m_ConsoleProcess();
		m_ReadInputs(&MouseInput);
		m_ProcessInputs(MouseInput);
		break;

		case PS_GAMEOVER:
		gl_ClearScreen();
		disp_Scene();
		o_UpdateEntities();
		go_Draw();

		switch((int)difficulty.value)
		{
			case 0:
			g_DispCenterString(17,21,"Congratulations");
			g_DispCenterString(17,22,"You have reached the end of the easy game");
			g_DispCenterString(17,23,"Try a highter level mode");
			break;

			case 1:
			g_DispCenterString(17,21,"Yeah");
			g_DispCenterString(17,22,"You have reached the end of the normal game");
			g_DispCenterString(17,23,"Try the hard mode");
			break;

			case 2:
			g_DispCenterString(17,21,"Oh my god");
			g_DispCenterString(17,22,"You have reached the end of the hard game");
			g_DispCenterString(17,24,"Coming Soon Halloween2");
			break;
		}

		m_ConsoleProcess();
		m_ReadInputs(&MouseInput);
		m_ProcessInputs(MouseInput);
		break;

		case PS_YESNO:
		game_EngineProcess();
		DrawGameMenu_YESNO();
		menu_SelectProcess_YESNO();
		m_ConsoleProcess();
		m_ReadInputs(&MouseInput);
		m_ProcessInputs(MouseInput);
		break;

		case PS_BEGINGAME:
		game_EngineProcess();
		DrawGameMenu_BEGINGAME();
		menu_SelectProcess_BEGINGAME();
		m_ConsoleProcess();
		m_ReadInputs(&MouseInput);
		m_ProcessInputs(MouseInput);
		break;

		case PS_SETSLOTGAME:
		game_EngineProcess();
		DrawGameMenu_SETSLOTGAME();
		menu_SelectProcess_SETSLOTGAME();
		// hack
		if(ProgramState != PS_SETSLOTGAME)
			break;
		m_ConsoleProcess();
		m_ReadInputs(&MouseInput);
		m_ProcessInputs(MouseInput);
		break;

		case PS_DIE:
		game_EngineProcess();
		game_DieProcess();
		m_ConsoleProcess();
		m_ReadInputs(&MouseInput);
		m_ProcessInputs(MouseInput);
		break;

		case PS_WIN:
		game_EngineProcess();
		game_WinProcess();
		menu_SelectProcess_YourWin();
		menu_DrawYouWinGame();
		m_ConsoleProcess();
		m_ReadInputs(&MouseInput);
		m_ProcessInputs(MouseInput);
		break;

		case PS_PAUSEDGAME:
		game_EngineProcess();
		DrawPausedGame();
		menu_SelectProcess_PAUSED();
		m_ConsoleProcess();
		m_ReadInputs(&MouseInput);
		m_ProcessInputs(MouseInput);
		break;

		case PS_CONFIG:
		game_EngineProcess();
		DrawGameConfig();
		menu_SelectProcess_CONFIG();
		m_ConsoleProcess();
		m_ReadInputs(&MouseInput);
		m_ProcessInputs(MouseInput);
		break;

		case PS_CONFIG_GAMEOPTION:
		game_EngineProcess();
		DrawGameConfig_GAMEOPTION();
		menu_SelectProcess_CONFIG_GAMEOPTION();
		m_ConsoleProcess();
		m_ReadInputs(&MouseInput);
		m_ProcessInputs(MouseInput);
		break;

		case PS_CONFIG_INPUT:
		game_EngineProcess();
		DrawGameConfig_INPUT();
		menu_SelectProcess_CONFIG_INPUT();
		m_ConsoleProcess();
		m_ReadInputs(&MouseInput);
		m_ProcessInputs(MouseInput);
		break;

		case PS_CONFIG_VIDEO:
		game_EngineProcess();
		DrawGameConfig_VIDEO();
		menu_SelectProcess_CONFIG_VIDEO();
		m_ConsoleProcess();
		m_ReadInputs(&MouseInput);
		m_ProcessInputs(MouseInput);
		break;

		case PS_CONFIG_SOUND:
		game_EngineProcess();
		DrawGameConfig_SOUND();
		menu_SelectProcess_CONFIG_SOUND();
		m_ConsoleProcess();
		m_ReadInputs(&MouseInput);
		m_ProcessInputs(MouseInput);
		break;

		case PS_SELECTMAP:
		gl_ClearScreen();
		DrawGameConfig_SelectMAP();
		menu_SelectProcess_SelectMAP();
		m_ConsoleProcess();
		m_ReadInputs(&MouseInput);
		m_ProcessInputs(MouseInput);
		break;

		case PS_STARTMAP:
		gl_ClearScreen();
		DrawGameConfig_StartMAP();
		menu_SelectProcess_StartMAP();
		m_ConsoleProcess();
		m_ReadInputs(&MouseInput);
		m_ProcessInputs(MouseInput);
		break;

		case PS_HELP:
		gl_ClearScreen();
		DrawGameConfig_HELP();
		menu_SelectProcess_HELP();
		m_ConsoleProcess();
		m_ProcessInputs(MouseInput);
		m_ReadInputs(&MouseInput);
		break;

		case PS_ORDER:
		gl_ClearScreen();
		DrawGameConfig_ORDER();
		menu_SelectProcess_ORDER();
		m_ConsoleProcess();
		m_ReadInputs(&MouseInput);
		m_ProcessInputs(MouseInput);
		break;

		case PS_GREETINGS:
		gAlphaFont = 1;
		game_EngineProcess();
		DrawGreetings();
		disp_DrawSprite(txid_CURSOR,(int)cursor_x-16,(int)cursor_y-16,32,32,true);
		m_ConsoleProcess();
		m_ReadInputs(&MouseInput);
		m_ProcessInputs(MouseInput);
		break;

		case PS_SHOWSHINFO:
		gAlphaFont = 1;
		game_EngineProcess();
		DrawGreetings();
		disp_DrawSprite(txid_CURSOR,(int)cursor_x-16,(int)cursor_y-16,32,32,true);
		m_ConsoleProcess();
		m_ReadInputs(&MouseInput);
		m_ProcessInputs(MouseInput);
		break;

		case PS_QUIT:
		QuitGame();
		ProgramState = PS_DEBUG;
		return true;

		default:
		gl_ClearScreen();
		disp_Scene();
		m_ConsoleProcess();
		break;
	}

	if(shareware || gIsDemo)
	{
		if(gDemoReminderTime)
		{
			gDemoReminderTime -= gFrameTime_msec;
			if(gDemoReminderTime < 0)
				gDemoReminderTime = 0;
	//		g_DispCenterString(16,8,"For registered users only!");
	//		g_DispCenterString(16,9,"Please support us, register this game.");
		}
	}

	return false;
}

void game_DrawAuthorNames()
{
	int	line = 8;
	int	fnt = 17;

	g_DispCenterString(fnt,line++,"coding");
	g_DispCenterString(fnt,line++,"JULIEN MEYER");
	line++;
	g_DispCenterString(fnt,line++,"map conception");
	g_DispCenterString(fnt,line++,"GWENAEL BEGO, ALEXIS MAURICE");
	line++;
	g_DispCenterString(fnt,line++,"game design");
	g_DispCenterString(fnt,line++,"VIANNEY DEVOIZE, THOMAS BERNOS, JULIEN CHABROLLES");
	line++;
	g_DispCenterString(fnt,line++,"music");
	g_DispCenterString(fnt,line++,"OLIVIER SOUBIELE, SEBASTIEN CHOQUENE");
	line++;
	g_DispCenterString(fnt,line++,"sound");
	g_DispCenterString(fnt,line++,"THIERRY DUTRUC-ROSSET");
	line++;
	g_DispCenterString(fnt,line++,"model");
	g_DispCenterString(fnt,line++,"SYLVAIN THIEBAUT");
	line++;
	g_DispCenterString(fnt,line++,"storyboard");
	g_DispCenterString(fnt,line++,"BENJAMIN DETROYE");
}

void DrawGreetings()
{
	gMenu_InitFirstSel = false; // hack

	// beta 1 HACK
	if(shareware || gIsDemo)
	{
		disp_DrawSprite_2(txid_ORDER,144,44,512,512,true);
	}
	else
	{
		int_DrawBackground();
		int_DrawTitle();
		game_DrawAuthorNames();
	}
}

void game_InitYouWin()
{
	pEntity	PlayerEnt;	

	ProgramState = PS_WIN;
	PlayerEnt = o_FindEntity(PERSO);
	if(PlayerEnt)
	{
		PlayerEnt->ent_anim.Pos_Vel.X = 0;
		PlayerEnt->ent_anim.Pos_Vel.Y = 0;
		PlayerEnt->ent_anim.Pos_Vel.Z = 0;

		PlayerEnt->ent_anim.Pos_Acc.X = 0;
		PlayerEnt->ent_anim.Pos_Acc.Y = 0;
		PlayerEnt->ent_anim.Pos_Acc.Z = 0;
	}
	bsp_InitStartPosition(gStart, gAngleView_player);
	MouseInput.offset_X = 0;
	MouseInput.offset_Y = 0;
	WorldColorR = WorldColorG = WorldColorB = 1;
	ds_PlaySound(40);
	PlayerSwapWeapon(0);
	//fx_ColorScreen(1800,200,0.1f,0.1f,0.1f,true);
	game_CheckMapCompleted();
	menu_InitYouWin();
}

void game_InitYouLose()
{
	pEntity	PlayerEnt;	

	PlayerEnt = o_FindEntity(PERSO);
	if(PlayerEnt)
	{
		PlayerEnt->ent_anim.Pos_Vel.X = 0;
		PlayerEnt->ent_anim.Pos_Vel.Y = 0;
		PlayerEnt->ent_anim.Pos_Vel.Z = 0;

		PlayerEnt->ent_anim.Pos_Acc.X = 0;
		PlayerEnt->ent_anim.Pos_Acc.Y = 0;
		PlayerEnt->ent_anim.Pos_Acc.Z = 0;
	}
	WorldColorR = WorldColorG = WorldColorB = 1;
	ds_PlaySound(21);
	PlayerSwapWeapon(0);
	fx_ColorScreen(3000,300,0.1f,0.1f,0.1f,true);
//	fx_ColorScreen(1800,200,0.1f,0.1f,0.1f,true);
	game_CheckMapCompleted();
	ProgramState = PS_WIN;
}

void game_InitStatutBar()
{
	ANIMENTITY	anim;
	pEntity		Ent;

	Ent = o_FindHoldedEntity(PUMPKIN);
	if(Ent)
	{
		o_RemoveEntityFromList(Ent);
	}
	o_ClearAnimFields(&anim);
	anim.Pos.X = 240;
	anim.Pos.Y = 40;
	anim.Pos.Z = 0;
	anim.Siz.X = 5;
	anim.Siz.Y = 5;
	anim.Siz.Z = 5;
	anim.Rot_Vel.Y = 0.12f;
	anim.Rot.X = 4.0f;
	o_AddEntityToList(PUMPKIN, -1, anim, true);
}

void DrawGameStatutBar()
{
	int		hauteur=0;
	int		R, G, B;
	int		Ri, Gi, Bi;
	int		c_l = 32; //64;
	int		c_h = 32; //64;
	int		health;
	pEntity	ClientEnt;
	pEntity	Ent;
	pEntity	PlayerEnt;

	if(gCameraShowTime)
		return;

	if(ProgramState != PS_GAME && ProgramState != PS_DIE)
		return;

	if(!statutbar.value)
		return;

	if(!player_view.value)
		return;

	if(gMapIntro && game_IsMapOpened(1) == false) // map training pas debloqu\E9
		return;

	Ri = (int)(WorldColorR * 255.0f);
	Gi = (int)(WorldColorG * 255.0f);
	Bi = (int)(WorldColorB * 255.0f);
	
	switch(gPlayerHasGun)
	{
		case 2:
		game_DrawTarget();
		disp_DrawSprite(txid_FLECHE,5,hauteur,48,48,false);
		disp_DrawNumber_leus(game_fleches,2,32,hauteur,c_l,c_h,Ri,Gi,Bi,true);
		break;

		case 3:
		disp_DrawSprite(txid_BOMBE,5,hauteur,48,48,false);
		disp_DrawNumber_leus(game_bombes,2,32,hauteur,c_l,c_h,Ri,Gi,Bi,true);
		break;

		case 4:
		game_DrawTarget();
		disp_DrawSprite(txid_BULLET,5,hauteur,48,48,false);
		disp_DrawNumber_leus(game_bullet,2,32,hauteur,c_l,c_h,Ri,Gi,Bi,true);
		break;
	}


	// AFFICHE LE NIVEAU DE VIE //////////////////////////////////////
	if(gIsMultiplayer && !gIsServer)
	{
		ClientEnt = o_FindEntity(MORBAK);
		if(!ClientEnt)
			return;
		health = ClientEnt->health;
	}
	else
	{
		health = game_heart;
	}
	R = 255; G = 255; B = 255;
	if(health <= 50)
	{
		if(health <= 25)
			G = B = 0;
		else
		{
			R = 255;
			G = 128;
			B = 0;
		}
	}
	PlayerEnt = o_FindEntity(PERSO);
	Ent = o_FindHoldedEntity(PUMPKIN);
	if(Ent && PlayerEnt)
	{
		gl_ProjectView(true, 800, 600);
		{
			if(gHeartSize > 1)
				Ent->ent_anim.Rot.Y = 0;
			Ent->ent_anim.Siz.X = 5 * gHeartSize;
			Ent->ent_anim.Siz.Y = 5 * gHeartSize;
			Ent->ent_anim.Siz.Z = 5 * gHeartSize;
			gl_DrawEntity(Ent, PlayerEnt, WorldColorR, WorldColorG, WorldColorB);
		}
		gl_ProjectView(false, 800, 600);
	}
	R = (int)((float)R * WorldColorR);
	G = (int)((float)G * WorldColorG);
	B = (int)((float)B * WorldColorB);
	disp_DrawNumber_leus(health,3,215,hauteur-0,c_l,c_h,R,G,B,true);


	// Affiche le timer
	R = 255; G = 255; B = 255;
	if(game_clock <= 25)
	{
		if(game_clock <= 10)
			G = B = 0;
		else
		{
			R = 255;
			G = 128;
			B = 0;
		}
	}
	if(game_TimeLimit)
	{
		if(ProgramState != PS_DIE)
		{
			disp_DrawSprite(txid_CLOCK,435,hauteur,48,48,true);
			R = (int)((float)R * WorldColorR);
			G = (int)((float)G * WorldColorG);
			B = (int)((float)B * WorldColorB);
			disp_DrawNumber_leus((int)game_clock,3,440+10,hauteur,c_l,c_h,R,G,B,true);
		}
	}
}

void game_AddHLetter(char letter)
{
//	int		mask;
	int		value;

	if(!gDispLetters)
		gDispLetters = true;

	switch(letter)
	{
		case 'H':
		HalloLetters[0] = letter;
		o_AddHudLetter('H');
		break;

		case 'A':
		HalloLetters[1] = letter;
		o_AddHudLetter('A');
		break;

		case 'L':
		if(HalloLetters[2] == letter)
		{
			HalloLetters[3] = letter;
			o_AddHudLetter('L');
		}
		else
		{
			HalloLetters[2] = letter;
			o_AddHudLetter('l');
		}
		break;

		case 'O':
		o_AddHudLetter('O');
		HalloLetters[4] = letter;
		break;

		case 'W':
		o_AddHudLetter('W');
		HalloLetters[5] = letter;
		break;

		case 'E':
		if(HalloLetters[6] == letter)
		{
			o_AddHudLetter('E');
			HalloLetters[7] = letter;
		}
		else
		{
			o_AddHudLetter('e');
			HalloLetters[6] = letter;
		}
		break;

		case 'N':
		o_AddHudLetter('N');
		HalloLetters[8] = letter;
		break;
	}
	if(!strcmp(HalloLetters,"HALLOWEEN"))
	{
		value = (int)map_letter.value;
		game_SetBit(&value, game_MapID);
	//	mask = 0x001 << game_MapID;
	//	value = value | mask;
		map_letter.value = (float)value;

		game_UpdateSlot_all(); //
		//strcpy(HalloLetters,"_________");
		game_CheckMapCompleted();
		gDispLetters = false;
		fx_ColorScreen(3000,500,0.4f,0.4f,1,true);
		ds_PlaySound(16); // welcome to halloween
		HLetter_Time = 4000;
	}
}

void game_CheckMapCompleted()
{
//	int	mask;
	int	value;
	int n_goal;
	int	diff;

	diff = (int)difficulty.value;

	// debloque la premiere map
	value = (int)map_complete.value;
	game_SetBit(&value, 0);
//	mask = 0x001;
//	value = value | mask;
	map_complete.value = (float)value;


	// LETTERS
	n_goal = MapGoals[0][diff][game_MapID];
	if(n_goal)
	{
		value = (int)map_letter.value;
		if(game_ReadBit(value, game_MapID) == false)
			return;
	//	mask = 0x001 << game_MapID;
	//	if( (value & mask) == 0)
	//		return;
	}

	// DEPLAS
	n_goal = MapGoals[1][diff][game_MapID];
	if(n_goal)
	{
		value = (int)kill_depla.value;
		if(game_ReadBit(value, game_MapID) == false)
			return;
	//	mask = 0x001 << game_MapID;
	//	if( (value & mask) == 0)
	//		return;
	}

	// MORBAKS
	n_goal = MapGoals[2][diff][game_MapID];
	if(n_goal)
	{
		value = (int)kill_morbak.value;
		if(game_ReadBit(value, game_MapID) == false)
			return;
	//	mask = 0x001 << game_MapID;
	//	if( (value & mask) == 0)
	//		return;
	}

	// ZEBATS
	n_goal = MapGoals[3][diff][game_MapID];
	if(n_goal)
	{
		value = (int)kill_zeubat.value;
		if(game_ReadBit(value, game_MapID) == false)
			return;
	//	mask = 0x001 << game_MapID;
	//	if( (value & mask) == 0)
	//		return;
	}

	// SUBZERATS
	n_goal = MapGoals[4][diff][game_MapID];
	if(n_goal)
	{
		value = (int)kill_subzerat.value;
		if(game_ReadBit(value, game_MapID) == false)
			return;
	//	mask = 0x001 << game_MapID;
	//	if( (value & mask) == 0)
	//		return;
	}

	// ZEMMUMMY
	n_goal = MapGoals[5][diff][game_MapID];
	if(n_goal)
	{
		value = (int)kill_zemummy.value;
		if(game_ReadBit(value, game_MapID) == false)
			return;
	//	mask = 0x001 << game_MapID;
	//	if( (value & mask) == 0)
	//		return;
	}

	// map goals completed so unlock next map
	m_ConsPrint("Map Complete!\n");
//	game_TimeLimit = false; // plus de limite de temps pour cette map d\E9bloqu\E9e
	game_MatrixMode = 0;	// on stop les conneries de l'effet marix car le level est fini

	value = (int)map_complete.value;
	game_SetBit(&value, game_MapID);
	map_complete.value = (float)value;

	if(game_MapID < 8)
	{
		value = (int)map_complete.value;
		game_SetBit(&value, game_MapID+1);
		map_complete.value = (float)value;
	}

	game_UpdateSlot_all();
	if(ProgramState == PS_GAME)
	{
		if(gMapBonus == false)
		{
			game_InitYouWin();
		}
	}
}

char *game_GetMusicString(int id)
{
	switch(id)
	{
		case -1:
		return TRACK_PB;

		case 0:
		return TRACK_0;

		case 1:
		return TRACK_1;

		case 2:
		return TRACK_2;

		case 3:
		return TRACK_3;

		case 4:
		return TRACK_4;

		case 5:
		return TRACK_5;

		case 6:
		return TRACK_6;

		case 7:
		return TRACK_7;

		case 8:
		return TRACK_PRES1;

		case 9:
		return TRACK_PRES2;

		case 10:
		return TRACK_PRES3;

		case 11:
		return TRACK_PRES4;

		default:
		return NULL;
	}
}

hBool game_UseFiole()
{
	pEntity		PlayerEnt;

	if(game_fiole1_catched)
	{
		game_fiole1_catched = false;
		o_RemoveHudFlask();
		PlayerEnt = o_FindEntity(PERSO);
		if(!PlayerEnt)
			return false;
		ds_PlaySound(9);
		PlayerEnt->ent_anim.Pos_Vel.Y = jumpstep.value * 2.0f;
		return true;
	}
	else if(game_fiole2_catched)
	{
		game_clock = MapTime[(int)difficulty.value]-1;
		game_fiole2_catched = false;
		o_RemoveHudFlask();
		game_MatrixMode = 20000.0f;
	//	fx_ColorScreen(2000,6000,0.4f,1,0.4f,true);
		fx_ColorScreen(2000,16000,0.4f,1,0.4f,true);
		return true;
	}
	else if(game_fiole3_catched)
	{
		game_fiole3_catched = false;
		o_RemoveHudFlask();
		game_FalconMode = 10000.0f;
		return true;
	}
	return false;
}

void game_WinProcess()
{
//	game_WinTime += gFrameTime_msec;
	if(game_WinTime >= GAME_WINTIME)
	{
		game_WinTime = 0;
		WorldColorR = WorldColorG = WorldColorB = 1;
		UnloadMap();
		game_LaunchPresMusic();
		game_LaunchMapIntro();
		return;
	}
	else
	{
		if(game_isover)
		{
			g_DispCenterString(24, 2, "You Lose");
		}
		else
		{
			g_DispCenterString(24, 2, "You Win");
		}
	}
}

void game_DieProcess()
{
	game_DieTime += gFrameTime_msec;
	if(game_DieTime >= GAME_DIETIME)
	{
		game_DieTime = 0;
		ProgramState = PS_GAME;
		PlayerSwapWeapon(0); // 1
		game_heart=100;
		PlayerResetStartPos(PERSO);
		return;
	}

	if(game_DieTime <= GAME_DIETIME * 0.3f)
	{
		if(!gPlayerCrouch)
			gCamera.Pos.Y -= 0.040f * gFrameTime_msec;
		gCamera.Rot.Z += 0.090f * gFrameTime_msec;
	}
}

void game_DrawTarget()
{
	disp_DrawSprite(txid_TARGET, -8+320, -8+240, 16, 16, true);
}

void game_UpdateSlot_all()
{
	FILE	*file;
	char	fullpath[255];

	sprintf(fullpath,"%s%sgame%d.svg",SYSDIR,SVGDIR,(int)gameslot.value);
	#ifdef H_MAC
	
	// SANDBOXED --------------------------------
	char * pDoc = KMiscTools::currentDocHomePath;
	//char fullpath[255];
	sprintf(fullpath, "%s/game%d.svg", pDoc, (int)gameslot.value);
	file = fopen(fullpath, "w");
	//file=fopen(KMiscTools::makeFilePath(fullpath),"w");
	#else
	file=fopen(fullpath,"w");
	#endif
	
	if(!file)
	{
		m_ConsPrint("error : can't save into game slot. %s\n",fullpath);
		return;
	}
	game_UpdateSlot(file,
		(int)difficulty.value,
		(int)map_letter.value,
		(int)kill_depla.value,
		(int)kill_morbak.value,
		(int)kill_zeubat.value,
		(int)kill_subzerat.value,
		(int)kill_zemummy.value,
		(int)map_complete.value);
	fclose(file);
}

void game_UpdateSlot(FILE *file, int diff, int a, int b, int c, int d, int e, int f, int g)
{
	if(!file)
		return;

	difficulty.value = (float)diff;
	map_letter.value = (float)a;
	kill_depla.value = (float)b;
	kill_morbak.value = (float)c;
	kill_zeubat.value = (float)d;
	kill_subzerat.value = (float)e;
	kill_zemummy.value = (float)f;
	map_complete.value = (float)g;

	fprintf(file,"%d\n",diff); // game difficulty (0=easy,1=normal by default,2=hard)
	fprintf(file,"%d\n",a); // mapletter
	fprintf(file,"%d\n",b); // killdepla
	fprintf(file,"%d\n",c); // killmorbak
	fprintf(file,"%d\n",d); // killzeubat
	fprintf(file,"%d\n",e); // killsubzerat
	fprintf(file,"%d\n",f); // killzemummy
	fprintf(file,"%d\n",g); // mapcomplete
}

void game_LoadSlot(FILE *file)
{
	int diff, a, b, c, d, e, f, g;

	if(!file)
		return;

	fscanf(file,"%d\n",&diff); // game difficulty (0=easy,1=normal by default,2=hard)
	fscanf(file,"%d\n",&a); // mapletter
	fscanf(file,"%d\n",&b); // killdepla
	fscanf(file,"%d\n",&c); // killmorbak
	fscanf(file,"%d\n",&d); // killzeubat
	fscanf(file,"%d\n",&e); // killsubzerat
	fscanf(file,"%d\n",&f); // killzemummy
	fscanf(file,"%d\n",&g); // mapcomplete

	difficulty.value = (float)diff;
	map_letter.value = (float)a;
	kill_depla.value = (float)b;
	kill_morbak.value = (float)c;
	kill_zeubat.value = (float)d;
	kill_subzerat.value = (float)e;
	kill_zemummy.value = (float)f;
	map_complete.value = (float)g;
}

void game_DeleteAllSlots()
{
	char	fullpath[255];
	
#ifdef H_MAC
	char * pDoc = KMiscTools::currentDocHomePath;
	sprintf(fullpath, "%s/game%d.svg", pDoc, 1);
	remove(fullpath);
	sprintf(fullpath, "%s/game%d.svg", pDoc, 2);
	remove(fullpath);
	sprintf(fullpath, "%s/game%d.svg", pDoc, 3);
	remove(fullpath);
	sprintf(fullpath, "%s/game%d.svg", pDoc, 4);
	remove(fullpath);
#else

	sprintf(fullpath,"%s%sgame%d.svg",SYSDIR,SVGDIR,1);
	remove(fullpath);
	sprintf(fullpath,"%s%sgame%d.svg",SYSDIR,SVGDIR,2);
	remove(fullpath);
	sprintf(fullpath,"%s%sgame%d.svg",SYSDIR,SVGDIR,3);
	remove(fullpath);
	sprintf(fullpath,"%s%sgame%d.svg",SYSDIR,SVGDIR,4);
	remove(fullpath);
#endif
}

int game_GetSlotDifficulty(FILE *file)
{
	int		diff;

	if(!file)
		return 1;

	fscanf(file,"%d",&diff);
	return diff;
}

void game_InitMapGoals()
{
	int		map;

	// easy						normal 						hard

	MapTime[0] = 900;			MapTime[1] = 300;			MapTime[2] = 180;

	MonsterLife[0][0] = 30;		MonsterLife[0][1] = 40;		MonsterLife[0][2] = 50;		// depla
	MonsterLife[1][0] = 20;		MonsterLife[1][1] = 30;		MonsterLife[1][2] = 40;		// morbak
	MonsterLife[2][0] = 10;		MonsterLife[2][1] = 10;		MonsterLife[2][2] = 20;		// zebat
	MonsterLife[3][0] = 10;		MonsterLife[3][1] = 20;		MonsterLife[3][2] = 30;		// subzerat
	MonsterLife[4][0] = 40;		MonsterLife[4][1] = 50;		MonsterLife[4][2] = 60;		// momie

	if(gIsMultiplayer)
	{
		int		i, j;

		for(i=0 ; i<=2 ; i++)
		for(j=0 ; j<=4 ; j++)
			MonsterLife[j][i] = 100;
	}

	MonsterDammage[0][0] = 8;	MonsterDammage[0][1] = 20;	MonsterDammage[0][2] = 30;	// lava
	MonsterDammage[1][0] = 5;	MonsterDammage[1][1] = 10;	MonsterDammage[1][2] = 20;	// spike
	MonsterDammage[2][0] = 5;	MonsterDammage[2][1] = 10;	MonsterDammage[2][2] = 20;	// toile
	MonsterDammage[3][0] = 4;	MonsterDammage[3][1] = 6;	MonsterDammage[3][2] = 10;	// explosion

	WeaponDammage[0][0] = 10;	WeaponDammage[0][1] = 10;	WeaponDammage[0][2] = 10;	// massue
	WeaponDammage[1][0] = 20;	WeaponDammage[1][1] = 20;	WeaponDammage[1][2] = 20;	// arbalette
	WeaponDammage[2][0] = 40;	WeaponDammage[2][1] = 40;	WeaponDammage[2][2] = 40;	// lancebombe
	WeaponDammage[3][0] = 7;	WeaponDammage[3][1] = 7;	WeaponDammage[3][2] = 7;	// tromblon
	WeaponDammage[4][0] = 0;	WeaponDammage[4][1] = 0;	WeaponDammage[4][2] = 0;	// 


	map = 0;	// H1M0
	MapGoals[0][0][map] = 1;	MapGoals[0][1][map] = 1;	MapGoals[0][2][map] = 1;	// lettre
	MapGoals[1][0][map] = 1;	MapGoals[1][1][map] = 1;	MapGoals[1][2][map] = 2;	// depla
	MapGoals[2][0][map] = 1;	MapGoals[2][1][map] = 1;	MapGoals[2][2][map] = 2;	// morbak
	MapGoals[3][0][map] = 0;	MapGoals[3][1][map] = 0;	MapGoals[3][2][map] = 0;	// zebat
	MapGoals[4][0][map] = 0;	MapGoals[4][1][map] = 0;	MapGoals[4][2][map] = 0;	// subzerat
	MapGoals[5][0][map] = 0;	MapGoals[5][1][map] = 0;	MapGoals[5][2][map] = 0;	// momie
	MapGoals[6][0][map] = 0;	MapGoals[6][1][map] = 0;	MapGoals[6][2][map] = 0;	// 
	MapGoals[7][0][map] = 0;	MapGoals[7][1][map] = 0;	MapGoals[7][2][map] = 0;	// 
	MapGoals[8][0][map] = 0;	MapGoals[8][1][map] = 0;	MapGoals[8][2][map] = 0;	// 
	MapGoals[9][0][map] = 0;	MapGoals[9][1][map] = 0;	MapGoals[9][2][map] = 0;	// 

	map = 1;	// H1M1
	MapGoals[0][0][map] = 0;	MapGoals[0][1][map] = 1;	MapGoals[0][2][map] = 1;
	MapGoals[1][0][map] = 3;	MapGoals[1][1][map] = 5;	MapGoals[1][2][map] = 7;
	MapGoals[2][0][map] = 2;	MapGoals[2][1][map] = 4;	MapGoals[2][2][map] = 6;
	MapGoals[3][0][map] = 2;	MapGoals[3][1][map] = 4;	MapGoals[3][2][map] = 6;
	MapGoals[4][0][map] = 0;	MapGoals[4][1][map] = 0;	MapGoals[4][2][map] = 0;
	MapGoals[5][0][map] = 0;	MapGoals[5][1][map] = 0;	MapGoals[5][2][map] = 0;
	MapGoals[6][0][map] = 0;	MapGoals[6][1][map] = 0;	MapGoals[6][2][map] = 0;
	MapGoals[7][0][map] = 0;	MapGoals[7][1][map] = 0;	MapGoals[7][2][map] = 0;
	MapGoals[8][0][map] = 0;	MapGoals[8][1][map] = 0;	MapGoals[8][2][map] = 0;
	MapGoals[9][0][map] = 0;	MapGoals[9][1][map] = 0;	MapGoals[9][2][map] = 0;

	map = 2;	// H1M2
	MapGoals[0][0][map] = 1;	MapGoals[0][1][map] = 1;	MapGoals[0][2][map] = 1;
	MapGoals[1][0][map] = 3;	MapGoals[1][1][map] = 5;	MapGoals[1][2][map] = 7;
	MapGoals[2][0][map] = 0;	MapGoals[2][1][map] = 0;	MapGoals[2][2][map] = 0;
	MapGoals[3][0][map] = 3;	MapGoals[3][1][map] = 5;	MapGoals[3][2][map] = 7;
	MapGoals[4][0][map] = 3;	MapGoals[4][1][map] = 5;	MapGoals[4][2][map] = 7;
	MapGoals[5][0][map] = 0;	MapGoals[5][1][map] = 0;	MapGoals[5][2][map] = 0;
	MapGoals[6][0][map] = 0;	MapGoals[6][1][map] = 0;	MapGoals[6][2][map] = 0;
	MapGoals[7][0][map] = 0;	MapGoals[7][1][map] = 0;	MapGoals[7][2][map] = 0;
	MapGoals[8][0][map] = 0;	MapGoals[8][1][map] = 0;	MapGoals[8][2][map] = 0;
	MapGoals[9][0][map] = 0;	MapGoals[9][1][map] = 0;	MapGoals[9][2][map] = 0;

	map = 3;	// H1M4
	MapGoals[0][0][map] = 1;	MapGoals[0][1][map] = 1;	MapGoals[0][2][map] = 1;
	MapGoals[1][0][map] = 4;	MapGoals[1][1][map] = 6;	MapGoals[1][2][map] = 8;
	MapGoals[2][0][map] = 4;	MapGoals[2][1][map] = 6;	MapGoals[2][2][map] = 8;
	MapGoals[3][0][map] = 0;	MapGoals[3][1][map] = 0;	MapGoals[3][2][map] = 0;
	MapGoals[4][0][map] = 0;	MapGoals[4][1][map] = 0;	MapGoals[4][2][map] = 0;
	MapGoals[5][0][map] = 3;	MapGoals[5][1][map] = 5;	MapGoals[5][2][map] = 7;
	MapGoals[6][0][map] = 0;	MapGoals[6][1][map] = 0;	MapGoals[6][2][map] = 0;
	MapGoals[7][0][map] = 0;	MapGoals[7][1][map] = 0;	MapGoals[7][2][map] = 0;
	MapGoals[8][0][map] = 0;	MapGoals[8][1][map] = 0;	MapGoals[8][2][map] = 0;
	MapGoals[9][0][map] = 0;	MapGoals[9][1][map] = 0;	MapGoals[9][2][map] = 0;

	map = 4;	// H1M5
	MapGoals[0][0][map] = 1;	MapGoals[0][1][map] = 1;	MapGoals[0][2][map] = 1;
	MapGoals[1][0][map] = 5;	MapGoals[1][1][map] = 8;	MapGoals[1][2][map] = 10;
	MapGoals[2][0][map] = 5;	MapGoals[2][1][map] = 8;	MapGoals[2][2][map] = 10;
	MapGoals[3][0][map] = 5;	MapGoals[3][1][map] = 8;	MapGoals[3][2][map] = 10;
	MapGoals[4][0][map] = 0;	MapGoals[4][1][map] = 0;	MapGoals[4][2][map] = 2;
	MapGoals[5][0][map] = 0;	MapGoals[5][1][map] = 0;	MapGoals[5][2][map] = 0;
	MapGoals[6][0][map] = 0;	MapGoals[6][1][map] = 0;	MapGoals[6][2][map] = 0;
	MapGoals[7][0][map] = 0;	MapGoals[7][1][map] = 0;	MapGoals[7][2][map] = 0;
	MapGoals[8][0][map] = 0;	MapGoals[8][1][map] = 0;	MapGoals[8][2][map] = 0;
	MapGoals[9][0][map] = 0;	MapGoals[9][1][map] = 0;	MapGoals[9][2][map] = 0;

	map = 5;	// H1M6
	MapGoals[0][0][map] = 1;	MapGoals[0][1][map] = 1;	MapGoals[0][2][map] = 1;
	MapGoals[1][0][map] = 0;	MapGoals[1][1][map] = 0;	MapGoals[1][2][map] = 0;
	MapGoals[2][0][map] = 5;	MapGoals[2][1][map] = 8;	MapGoals[2][2][map] = 10;
	MapGoals[3][0][map] = 5;	MapGoals[3][1][map] = 8;	MapGoals[3][2][map] = 10;
	MapGoals[4][0][map] = 0;	MapGoals[4][1][map] = 0;	MapGoals[4][2][map] = 0;
	MapGoals[5][0][map] = 5;	MapGoals[5][1][map] = 8;	MapGoals[5][2][map] = 10;
	MapGoals[6][0][map] = 0;	MapGoals[6][1][map] = 0;	MapGoals[6][2][map] = 0;
	MapGoals[7][0][map] = 0;	MapGoals[7][1][map] = 0;	MapGoals[7][2][map] = 0;
	MapGoals[8][0][map] = 0;	MapGoals[8][1][map] = 0;	MapGoals[8][2][map] = 0;
	MapGoals[9][0][map] = 0;	MapGoals[9][1][map] = 0;	MapGoals[9][2][map] = 0;

	map = 6;	// H1M7
	MapGoals[0][0][map] = 1;	MapGoals[0][1][map] = 1;	MapGoals[0][2][map] = 1;
	MapGoals[1][0][map] = 7;	MapGoals[1][1][map] = 10;	MapGoals[1][2][map] = 13;
	MapGoals[2][0][map] = 6;	MapGoals[2][1][map] = 9;	MapGoals[2][2][map] = 12;
	MapGoals[3][0][map] = 5;	MapGoals[3][1][map] = 8;	MapGoals[3][2][map] = 11;
	MapGoals[4][0][map] = 0;	MapGoals[4][1][map] = 0;	MapGoals[4][2][map] = 0;
	MapGoals[5][0][map] = 0;	MapGoals[5][1][map] = 0;	MapGoals[5][2][map] = 0;
	MapGoals[6][0][map] = 0;	MapGoals[6][1][map] = 0;	MapGoals[6][2][map] = 0;
	MapGoals[7][0][map] = 0;	MapGoals[7][1][map] = 0;	MapGoals[7][2][map] = 0;
	MapGoals[8][0][map] = 0;	MapGoals[8][1][map] = 0;	MapGoals[8][2][map] = 0;
	MapGoals[9][0][map] = 0;	MapGoals[9][1][map] = 0;	MapGoals[9][2][map] = 0;

	map = 7;	// H1M8
	MapGoals[0][0][map] = 1;	MapGoals[0][1][map] = 1;	MapGoals[0][2][map] = 1;
	MapGoals[1][0][map] = 7;	MapGoals[1][1][map] = 10;	MapGoals[1][2][map] = 13;
	MapGoals[2][0][map] = 7;	MapGoals[2][1][map] = 10;	MapGoals[2][2][map] = 13;
	MapGoals[3][0][map] = 7;	MapGoals[3][1][map] = 10;	MapGoals[3][2][map] = 13;
	MapGoals[4][0][map] = 0;	MapGoals[4][1][map] = 0;	MapGoals[4][2][map] = 0;
	MapGoals[5][0][map] = 0;	MapGoals[5][1][map] = 0;	MapGoals[5][2][map] = 0;
	MapGoals[6][0][map] = 0;	MapGoals[6][1][map] = 0;	MapGoals[6][2][map] = 0;
	MapGoals[7][0][map] = 0;	MapGoals[7][1][map] = 0;	MapGoals[7][2][map] = 0;
	MapGoals[8][0][map] = 0;	MapGoals[8][1][map] = 0;	MapGoals[8][2][map] = 0;
	MapGoals[9][0][map] = 0;	MapGoals[9][1][map] = 0;	MapGoals[9][2][map] = 0;

	map = 8;	// H1M9
	MapGoals[0][0][map] = 1;	MapGoals[0][1][map] = 1;	MapGoals[0][2][map] = 1;
	MapGoals[1][0][map] = 10;	MapGoals[1][1][map] = 13;	MapGoals[1][2][map] = 15;
	MapGoals[2][0][map] = 10;	MapGoals[2][1][map] = 13;	MapGoals[2][2][map] = 15;
	MapGoals[3][0][map] = 0;	MapGoals[3][1][map] = 1;	MapGoals[3][2][map] = 2;
	MapGoals[4][0][map] = 0;	MapGoals[4][1][map] = 0;	MapGoals[4][2][map] = 0;
	MapGoals[5][0][map] = 0;	MapGoals[5][1][map] = 0;	MapGoals[5][2][map] = 0;
	MapGoals[6][0][map] = 0;	MapGoals[6][1][map] = 0;	MapGoals[6][2][map] = 0;
	MapGoals[7][0][map] = 0;	MapGoals[7][1][map] = 0;	MapGoals[7][2][map] = 0;
	MapGoals[8][0][map] = 0;	MapGoals[8][1][map] = 0;	MapGoals[8][2][map] = 0;
	MapGoals[9][0][map] = 0;	MapGoals[9][1][map] = 0;	MapGoals[9][2][map] = 0;
}

hBool game_IsMapOpened(int map_id)
{
	int		value;

//	if(map_id >= 3)
//		map_id++;

	value = (int)map_complete.value;
	if(game_ReadBit(value, map_id) == false)
		return false;
	return true;
}

void game_LoadMap(int map_id)
{
	int		j;
	char	mapname[MAX_MAPFILE];

	gl_StopFog();

	// on met que 1/*3*/ map dans la d\E9mo du jeu halloween
	if(shareware || gIsDemo)
	{
		if(map_id > 0) // 2
		{
			ProgramState = PS_GREETINGS;
			gAlphaFont = 1;
			game_PlayWeirdSound();
			gDemoReminderTime = 4000.0f;
			return;
		}
	}

	UnloadMap();
	game_Init();
	key_CheckGame();	// anti pirate
	ProgramState = PS_GAME;

	switch(map_id)
	{
		case -1:
		gl_SetFog();
		strcpy(mapfile.string, "mapbonus0.map");
		game_MapID = map_id;
		game_TimeLimit = false;
		gMapBonus = false;
		gMapIntro = true;
		break;

		case -2:
		gl_SetFog();
		strcpy(mapfile.string, "mapbonus1.map");
		game_MapID = map_id;
		game_TimeLimit = true;
		gMapBonus = true;
		gMapIntro = false;
		break;

		case -3:
		gl_SetFog();
		strcpy(mapfile.string, "mapbonus2.map");
		game_MapID = map_id;
		game_TimeLimit = true;
		gMapBonus = true;
		gMapIntro = false;
		break;

		default:
		gMapBonus = false;
		gMapIntro = false;
		for(j=0 ; j<MAX_MAPFILE ; j++)
		{
			if(maps[map_id].name[j] == ' ')
			{
				mapname[j] = '\0';
				break;
			}
			mapname[j] = maps[map_id].name[j];
		}
		strcpy(mapfile.string, mapname);
		txid_LOADINGMAP = maps[map_id].txid;
		game_MapID = map_id;
		gl_SetFog(); // hack
		break;
	}
	sys_AntiPirate();
	gMenu_InitFirstSel = false;
	LoadMap();
	if(gMapIntro == false && gMapBonus == false)
	{
		game_clock = MapTime[(int)difficulty.value];
		game_InitGoalCounters();
	}
	if(gMapBonus == true)
	{
		game_clock = 180;
	}
	if(gMapIntro == false)
	{
		menu_InitSelectMAP();
		ProgramState = PS_SELECTMAP;
	}
	cam_StopEarthQuake();
	game_InitStatutBar();
}

void game_InitCameraShow_letter()
{
	float	duree = 4000;
	int		letter;
	int		i;

	gGameEnded = false;
	letter = -1;
	for(i=0 ; i<=9/*game_NbMaps*/ ; i++)
	{
		if(game_IsMapOpened(i) == false)
		{
			letter = i-1;
			break;
		}
	}
	if(letter < 0)
	{
		gGameEnded = true;
		duree = duree * 4;
		letter = rand() % 9;
	}

	switch(letter)
	{
		case 0: // H
		game_InitCameraShow(duree, 718, -350+0, -21, 27, 92, 0);
		break;

		case 1: // A
		game_InitCameraShow(duree, -375, -329+0, 93, 16, 241, 0);
		break;

		case 2: //L1
		game_InitCameraShow(duree, 45, 245+0, 1, 31, 118, 0);
		break;

		case 3: // L2
		game_InitCameraShow(duree, 248, 293+0, -315, 28, 92, 0);
		break;

		case 4: // O
		game_InitCameraShow(duree, -50, -77+0, 295, -40, 179, 0);
		break;

		case 5: // W
		game_InitCameraShow(duree, -209, 290+0, 280, 11, 98, 0);
		break;

		case 6: // E1
		game_InitCameraShow(duree, 134, -52+0, -612, 10, 245, 0);
		break;

		case 7: // E2
		game_InitCameraShow(duree, -524, 131+0, -30, -18, 265, 0);
		break;

		case 8: // N
		game_InitCameraShow(duree, 192, 215+0, 310, 56, 50, 0);
		break;
	}
}

void game_InitCameraShow(float timing, float posX, float posY, float posZ, float rotX, float rotY, float rotZ)
{
	gCameraShowTime = timing;
	gCamera.Pos.X = posX;
	gCamera.Pos.Y = posY;
	gCamera.Pos.Z = posZ;
	gCamera.Rot.X = rotX;
	gCamera.Rot.Y = rotY;
	gCamera.Rot.Z = rotZ;
}

void game_StopCameraShow()
{
	gCameraShowTime = 0;
	bsp_InitStartPosition(gStart, gAngleView_player);
	if(gWelcomeToHalloween)
	{
		gWelcomeToHalloween = false;
		ds_PlaySound(16); // welcome to halloween
	}
}

hBool game_IsCameraShow()
{
	if(gCameraShowTime > 0)
		return true;
	return false;
}

void game_CameraShowProcess()
{
	if(gCameraShowTime <= 0)
		return;
	gCameraShowTime -= gFrameTime_msec;
	if(gCameraShowTime <= 0)
	{
		game_StopCameraShow();
		return;
	}
}

void game_LaunchMapIntro()
{
	ProgramState = PS_INTROMAP;
}

void game_LaunchMapMusic()
{
	ds_LoadStreamWAVE(game_GetMusicString((int)music_track.value));
	music_track.value++;
	music_track.value = (float)((int)music_track.value % 7);
}

void game_LaunchBonusMapMusic()
{
    ds_LoadStreamWAVE(TRACK_PB);
}

void game_LaunchPresMusic()
{
    ds_LoadStreamWAVE(TRACK_PRES1);
}

hBool game_ReadBit(int value, int idbit)
{
	int		mask;

	if(idbit < 0 || idbit > 31)
	{
		if(developer.value)
		{
			m_ConsPrint("game_ReadBit with outbound values warning\n");
		}
		return false;
	}
	mask = 0x001 << idbit;
	if(value & mask)
	{
		return true;
	}
	return false;
}

void game_SetBit(int *value, int idbit)
{
	int		mask;

	if(idbit < 0 || idbit > 31)
	{
		if(developer.value)
		{
			m_ConsPrint("game_SetBit with outbound values warning\n");
		}
		return;
	}
	mask = (0x001 << idbit);
	*value = *value | mask;
}

void game_GameEnded()
{
	ANIMENTITY anim;

	ds_PlaySound(21);
	UnloadMap();
	pres_track.value = 0;
	game_LaunchPresMusic();
	gl_ClearScreen();

    game_MatrixMode = false;
	gCamera.Pos.X = 0;
	gCamera.Pos.Y = 0;
	gCamera.Pos.Z = 0;
	gCamera.Rot.X = 0;
	gCamera.Rot.Y = 0;
	gCamera.Rot.Z = 0;
	o_ClearEntityList();
	o_ClearAnimFields(&anim);
	anim.Pos.Y = 300.0f;
	o_AddEntityToList(PERSO,-1,anim,false);
	o_ClearAnimFields(&anim);

	switch((int)difficulty.value)
	{
		case 0:
		anim.Pos.Z = -40.0f;
		o_AddEntityToList(ZEBAT,-1,anim,false);
		break;

		case 1:
		anim.Pos.Z = -80.0f;
		o_AddEntityToList(MORBAK,-1,anim,false);
		break;

		case 2:
		anim.Pos.Z = -80.0f;
		o_AddEntityToList(DEPLA,-1,anim,false);
		break;
	}
	ProgramState = PS_GAMEOVER;
}
