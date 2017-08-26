
#include "alloween.h"
//#include "collision2.h"


Face_t		*PolyList=NULL;
Face_t		*FaceArray=NULL;
noeud_t		*NodeArray=NULL;
leafs_t		*LeafArray=NULL;
portal_t	*PortalArray=NULL;
char		*PVSData=NULL;
NL_t		NLlist;
Camera_t	gCamera; // = {0,0,0,0,0,0};
vect_t		gStart;
int			gLeafLocation=-1;
int			gNb_Leafs=0;
int			gNb_Portals=0;
int			gNb_nodes=0;
int			gNb_faces=0;
int			MapDataFormat = MAP_NULL;
int			texture_id_plan[TEXTURELOADED];
hBool		gMapLightened;
int			nbTabFaces;
pTabFace	gTabFaces = NULL;


hBool LoadMap()
{
	FILE	*file;
	char	line[255];
	char	path[255];
	char	*p;
	hBool	result;
	int	i;

	MapDataFormat = MAP_NULL;
	NLlist.NbNL = 0;
	NLlist.NLData = NULL;
	sprintf(path,"%s%s%s",SYSDIR,MAPDIR,mapfile.string);

	m_ConsPrint("Loading Map : %s\n", path );

#ifdef H_MAC
	file = fopen(KMiscTools::makeFilePath(path), "r");
#else
	file = fopen(path, "r");
#endif	
	
	if(file==NULL)
		return false;
	memset(line,0,255);
	fgets(line,255,file);
	fclose(file);

	game_Load2D_game_icons();

	menu_DispLoadMapSplash();
	gl_SwapBuffer();

	if(!gLoad3Dstuff)
	{
		if(!game_Load3Dstuff())
			return false;
		if(!game_LoadModels())
			return false;
		gLoad3Dstuff = true;
	}

//	if(!game_LoadModels())
//		return false;

	if(!strncmp(line,"<POLYGONS>",10))
	{
		MapDataFormat = MAP_POLYGONS;
		result = bsp_Load_POLYFILE(path);
	}
	else if( !strncmp(line,"<BSP>",5) || !strncmp(line,"<MAP>",5))
	{
		MapDataFormat = MAP_BSP;
		result = bsp_Load_MAPFILE(path);
		p = fs_RenameFileExtension(path, "hsl");
		script_Init(p);
	}
	else
		result = false;

	if(!result)
	{
		m_ConsPrint("No Map file was found!\n");
		MapDataFormat = MAP_NULL;
		game_UnloadModels();
		return false;
	}

	// ajoute le ciel
	if(gMapIntro)
	{
		ANIMENTITY	anim;

		o_ClearAnimFields(&anim);
		anim.Rot_Vel.Y = 0.001f;
		anim.Siz.X = 35.5f;
		anim.Siz.Y = 35.5f;
		anim.Siz.Z = 35.5f;
		anim.Pos.Y = 500;
		o_AddEntityToList(SKYDOME, -1, anim, false);

		o_ClearAnimFields(&anim);
		//anim.Rot_Vel.Y = 0.001f;
		anim.Siz.X = 40;
		anim.Siz.Y = 40;
		anim.Siz.Z = 40;
		anim.Pos.Y = 500;
		o_AddEntityToList(SKYDOME2, -1, anim, false);

		/*
		ANIMENTITY	anim;

		o_ClearAnimFields(&anim);
		anim.Rot_Vel.Y = 0.001f;
		anim.Siz.X = 35.5f;
		anim.Siz.Y = 35.5f;
		anim.Siz.Z = 35.5f;
		o_AddEntityToList(SKYDOME, -1, anim, false);
		*/
	}

	if(gIsMultiplayer && gIsServer)
	{
		int	index = rand() % gNb_Respawn;
		bsp_InitStartPosition(RespawnTab[index].entity_pos, gAngleView_player);
	}


	// HACK : cale sur le FPS exacte
//	if(gMapIntro)
	{
		gMenu_InitFirstSel = false;
		WorldColorR = WorldColorG = WorldColorB = 0;
		for(i=0 ; i<30 ; i++)
		{
			gl_ClearScreen();
			disp_Scene();
		}
		gl_ClearScreen();
	}

	WorldColorR = WorldColorG = WorldColorB = 1;
	return true;
}

void UnloadMap()
{
	if(MapDataFormat == MAP_NULL)
		return;
	m_ConsPrint("UnLoading Map\n");
	o_ClearEntityList();
	lb_StopSound3D();
	gl_StopFog();

	if(MapDataFormat == MAP_POLYGONS)
	{
		PolyList = liste_Delete(PolyList);
		if(gTabFaces)
		{
			s_free(gTabFaces);
			gTabFaces = NULL;
		}
	}
	else if(MapDataFormat == MAP_BSP)
	{
		bsp_Unload_MAPFILE();
		ab_Unload();
	}

	// anti pirate
	key_CheckGame();
	if(shareware2-4 != -3)
		shareware = true;

	im_UnloadMapTextures();
//	game_UnloadModels();
}

void bsp_InitStartPosition(vect_t pos, int AngleView)
{
	ANIMENTITY	anim;
	pEntity		PlayerEnt;

	gCamera.Pos.X = pos.X;
	gCamera.Pos.Y = pos.Y;
	gCamera.Pos.Z = pos.Z;

	gCamera.Rot.X = 0;
	gCamera.Rot.Y = (float)AngleView + 179.9f; // HACK : BUG BLOCKED
	gCamera.Rot.Z = 0;

	if(!gIsMultiplayer || gIsServer)
	{
		PlayerEnt = o_FindEntity(PERSO);
		if(PlayerEnt == NULL)
		{
			o_ClearAnimFields(&anim);
			anim.Pos.X = gCamera.Pos.X;
			anim.Pos.Y = gCamera.Pos.Y;
			anim.Pos.Z = gCamera.Pos.Z;
			anim.Rot.Y = (float)AngleView;
			anim.Pos_Vel.Y = 0.1f;
			o_AddEntityToList(PERSO,-1,anim,false);
		}
		else
		{
			PlayerEnt->ent_anim.Pos = gCamera.Pos;
			PlayerEnt->ent_anim.Rot.Y = (float)AngleView;
		}
	}
}

void bsp_LoadStartPosition(FILE *file)
{
	fscanf(file,"<PLAYERPOS> %f %f %f\n",&gStart.X,&gStart.Y,&gStart.Z);
	fscanf(file,"<PLAYERDIR> %d\n",&gAngleView_player);
	bsp_InitStartPosition(gStart, gAngleView_player);
}

int bsp_LoadEntity(FILE *file)
{
	int			Nb_Entities;
	int			k;
	int			Entity_ID;
	int			Entity_RotY;
	ANIMENTITY	anim;
	pEntity		LiftTrack_Ent;
	int			SpawnPos;
	hBool		LiftBool;
	float		RespawnTime = 1000.0f;
	int			nb_L;
	int			nb_E;
	int			nb_Pumpkin;
	int			PumpkinId[2];	//mapintro hack

	SpawnPos=0;
	LiftBool=false;
	LiftTrack_Ent=NULL;

	for(k=0 ; k<MAX_RESPAWN_OBJ ; k++)
	{
		RespawnTab[k].used=false;
		RespawnTab[k].pickedup = false;
	}

	fscanf(file,"<ENTITIES> %d\n",&Nb_Entities);
	gNumberOfMonsters=0;

	nb_L = 0;
	nb_E = 0;
	nb_Pumpkin = 0;

	for(k=0 ; k<Nb_Entities ; k++)
	{
		o_ClearAnimFields(&anim);
		fscanf(file,"%d %d %f %f %f\n",&Entity_ID,&Entity_RotY,&anim.Pos.X,&anim.Pos.Y,&anim.Pos.Z);
		
		// aucune entité en mode multiplayer, par contre on spawn sur les pos des monstres
		// sauf celle de la mommy (HACK)
		if(gIsMultiplayer)
		{
			if(	Entity_ID == 1 ||
				Entity_ID == 12 ||
				Entity_ID == 25 
				)
			{
				RespawnTab[SpawnPos].entity_pos = anim.Pos;
				SpawnPos++;
			}
			continue;
		}

		// En mode HIT BEST SCORE : pas de lettres a ramasser
		if(game_HitBestScoreMode)
		{
			if(	Entity_ID >= 15 && Entity_ID <= 21)
			{
				continue;
			}
		}

		// en mode map intro on donne les lettres selon la progression
		if(gMapIntro)
		{
			switch(Entity_ID)
			{
				case 13: // PUMPKIN
				if(nb_Pumpkin < 2)
				{
					PumpkinId[nb_Pumpkin] = SpawnPos;
				}
				nb_Pumpkin++;
				break;

				case 16: // A
				if(game_IsMapOpened(1) == false)
					continue;
				break;

				case 17: // L
				nb_L++;
				if(nb_L == 1)
				{
					if(game_IsMapOpened(2) == false)
						continue;
				}
				else
				{
					if(game_IsMapOpened(3) == false)
						continue;
				}
				break;

				case 18: // O
				if(game_IsMapOpened(4) == false)
					continue;
				break;

				case 19: // W
				if(game_IsMapOpened(5) == false)
					continue;
				break;

				case 20: // E
				nb_E++;
				if(nb_E == 1)
				{
					if(game_IsMapOpened(6) == false)
						continue;
				}
				else
				{
					if(game_IsMapOpened(7) == false)
						continue;
				}
				break;

				case 21: // N
				if(game_IsMapOpened(8) == false)
					continue;
				break;
			}
		}

		switch(Entity_ID)
		{
			case 1:		//DEPLA1
			RespawnTab[SpawnPos].list_id = (anim.Pos.X + 8.0f*anim.Pos.Y + 70.0f*anim.Pos.Z);
			RespawnTab[SpawnPos].entity_id = DEPLA;
			RespawnTab[SpawnPos].used = true;
			RespawnTab[SpawnPos].entity_pos = anim.Pos;
			RespawnTab[SpawnPos].Timer = 0;
			RespawnTab[SpawnPos].pickedup = false;
			anim.id_list = RespawnTab[SpawnPos].list_id;
			SpawnPos++;
			o_AddEntityToList(DEPLA,-1,anim,false);
			break;

			case 2:		//GUN1
			RespawnTab[SpawnPos].list_id = (anim.Pos.X + 8.0f*anim.Pos.Y + 70.0f*anim.Pos.Z);
			RespawnTab[SpawnPos].entity_id = GUN1;
			RespawnTab[SpawnPos].used = true;
			RespawnTab[SpawnPos].entity_pos = anim.Pos;
			RespawnTab[SpawnPos].Timer = RespawnTime;
			RespawnTab[SpawnPos].pickedup = true;
			SpawnPos++;
			break;

			case 3:		//AMO1
			RespawnTab[SpawnPos].list_id = (anim.Pos.X + 8.0f*anim.Pos.Y + 70.0f*anim.Pos.Z);
			RespawnTab[SpawnPos].entity_id = AMO1;
			RespawnTab[SpawnPos].used = true;
			RespawnTab[SpawnPos].entity_pos = anim.Pos;
			RespawnTab[SpawnPos].Timer = RespawnTime;
			RespawnTab[SpawnPos].pickedup = true;
			SpawnPos++;
			break;

			case 4:		//GUN2
			RespawnTab[SpawnPos].list_id = (anim.Pos.X + 8.0f*anim.Pos.Y + 70.0f*anim.Pos.Z);
			RespawnTab[SpawnPos].entity_id = GUN2;
			RespawnTab[SpawnPos].used = true;
			RespawnTab[SpawnPos].entity_pos = anim.Pos;
			RespawnTab[SpawnPos].Timer = RespawnTime;
			RespawnTab[SpawnPos].pickedup = true;
			SpawnPos++;
			break;

			case 5:		//AMO2
			RespawnTab[SpawnPos].list_id = (anim.Pos.X + 8.0f*anim.Pos.Y + 70.0f*anim.Pos.Z);
			RespawnTab[SpawnPos].entity_id = AMO2;
			RespawnTab[SpawnPos].used = true;
			RespawnTab[SpawnPos].entity_pos = anim.Pos;
			RespawnTab[SpawnPos].Timer = RespawnTime;
			RespawnTab[SpawnPos].pickedup = true;
			SpawnPos++;
			break;

			case 6:		//LIFT
			break;

			case 7:		//GUN3
			RespawnTab[SpawnPos].list_id = (anim.Pos.X + 8.0f*anim.Pos.Y + 70.0f*anim.Pos.Z);
			RespawnTab[SpawnPos].entity_id = GUN3;
			RespawnTab[SpawnPos].used = true;
			RespawnTab[SpawnPos].entity_pos = anim.Pos;
			RespawnTab[SpawnPos].Timer = RespawnTime;
			RespawnTab[SpawnPos].pickedup = true;
			SpawnPos++;
			break;

			case 9:		//AMO3
			RespawnTab[SpawnPos].list_id = (anim.Pos.X + 8.0f*anim.Pos.Y + 70.0f*anim.Pos.Z);
			RespawnTab[SpawnPos].entity_id = AMO3;
			RespawnTab[SpawnPos].used = true;
			RespawnTab[SpawnPos].entity_pos = anim.Pos;
			RespawnTab[SpawnPos].Timer = RespawnTime;
			RespawnTab[SpawnPos].pickedup = true;
			SpawnPos++;
			break;

			case 10:	//FIOLE1
			RespawnTab[SpawnPos].list_id = (anim.Pos.X + 8.0f*anim.Pos.Y + 70.0f*anim.Pos.Z);
			RespawnTab[SpawnPos].entity_id = FIOLE1;
			RespawnTab[SpawnPos].used = true;
			RespawnTab[SpawnPos].entity_pos = anim.Pos;
			RespawnTab[SpawnPos].Timer = 0;
			RespawnTab[SpawnPos].pickedup = true;
			SpawnPos++;
			break;

			case 11:	//LAMPE
			o_AddEntityToList(LAMPE,-1,anim,false);
	//		ds_PlaySound3D(34,anim.Pos,anim.Pos);
			break;

			case 12:	//MORBAK
			RespawnTab[SpawnPos].list_id = (anim.Pos.X + 8.0f*anim.Pos.Y + 70.0f*anim.Pos.Z);
			RespawnTab[SpawnPos].entity_id = MORBAK;
			RespawnTab[SpawnPos].used = true;
			RespawnTab[SpawnPos].entity_pos = anim.Pos;
			RespawnTab[SpawnPos].Timer = 0;
			RespawnTab[SpawnPos].pickedup = false;
			anim.id_list = RespawnTab[SpawnPos].list_id;
			SpawnPos++;
			o_AddEntityToList(MORBAK,-1,anim,false);
			break;

			case 13:	//PUMPKIN
			RespawnTab[SpawnPos].list_id = (anim.Pos.X + 8.0f*anim.Pos.Y + 70.0f*anim.Pos.Z);
			RespawnTab[SpawnPos].entity_id = PUMPKIN;
			RespawnTab[SpawnPos].used = true;
			RespawnTab[SpawnPos].entity_pos = anim.Pos;
			RespawnTab[SpawnPos].RotY = Entity_RotY;
			RespawnTab[SpawnPos].Timer = RespawnTime;
			RespawnTab[SpawnPos].pickedup = true;
			SpawnPos++;
			break;

			case 14:	//MLAMPE
			o_AddEntityToList(MLAMPE,-1,anim,false);
	//		ds_PlaySound3D(34,anim.Pos,anim.Pos);
			break;

			case 15:	//H
			anim.Rot_Vel.Y = 0.15f;
			RespawnTab[SpawnPos].list_id = (anim.Pos.X + 8.0f*anim.Pos.Y + 70.0f*anim.Pos.Z);
			RespawnTab[SpawnPos].entity_id = H;
			RespawnTab[SpawnPos].used = true;
			RespawnTab[SpawnPos].entity_pos = anim.Pos;
			RespawnTab[SpawnPos].RotY = Entity_RotY;
			RespawnTab[SpawnPos].Timer = RespawnTime;
			RespawnTab[SpawnPos].pickedup = true;
			SpawnPos++;
			break;

			case 16:	//A
			anim.Rot_Vel.Y = 0.15f;
			RespawnTab[SpawnPos].list_id = (anim.Pos.X + 8.0f*anim.Pos.Y + 70.0f*anim.Pos.Z);
			RespawnTab[SpawnPos].entity_id = A;
			RespawnTab[SpawnPos].used = true;
			RespawnTab[SpawnPos].entity_pos = anim.Pos;
			RespawnTab[SpawnPos].RotY = Entity_RotY;
			RespawnTab[SpawnPos].Timer = RespawnTime;
			RespawnTab[SpawnPos].pickedup = true;
			SpawnPos++;
			break;

			case 17:	//L
			anim.Rot_Vel.Y = 0.15f;
			RespawnTab[SpawnPos].list_id = (anim.Pos.X + 8.0f*anim.Pos.Y + 70.0f*anim.Pos.Z);
			RespawnTab[SpawnPos].entity_id = L;
			RespawnTab[SpawnPos].used = true;
			RespawnTab[SpawnPos].entity_pos = anim.Pos;
			RespawnTab[SpawnPos].RotY = Entity_RotY;
			RespawnTab[SpawnPos].Timer = RespawnTime;
			RespawnTab[SpawnPos].pickedup = true;
			SpawnPos++;
			break;

			case 18:	//O
			anim.Rot_Vel.Y = 0.15f;
			RespawnTab[SpawnPos].list_id = (anim.Pos.X + 8.0f*anim.Pos.Y + 70.0f*anim.Pos.Z);
			RespawnTab[SpawnPos].entity_id = O;
			RespawnTab[SpawnPos].used = true;
			RespawnTab[SpawnPos].entity_pos = anim.Pos;
			RespawnTab[SpawnPos].RotY = Entity_RotY;
			RespawnTab[SpawnPos].Timer = RespawnTime;
			RespawnTab[SpawnPos].pickedup = true;
			SpawnPos++;
			break;

			case 19:	//W
			anim.Rot_Vel.Y = 0.15f;
			RespawnTab[SpawnPos].list_id = (anim.Pos.X + 8.0f*anim.Pos.Y + 70.0f*anim.Pos.Z);
			RespawnTab[SpawnPos].entity_id = W;
			RespawnTab[SpawnPos].used = true;
			RespawnTab[SpawnPos].entity_pos = anim.Pos;
			RespawnTab[SpawnPos].RotY = Entity_RotY;
			RespawnTab[SpawnPos].Timer = RespawnTime;
			RespawnTab[SpawnPos].pickedup = true;
			SpawnPos++;
			break;

			case 20:	//E
			anim.Rot_Vel.Y = 0.15f;
			RespawnTab[SpawnPos].list_id = (anim.Pos.X + 8.0f*anim.Pos.Y + 70.0f*anim.Pos.Z);
			RespawnTab[SpawnPos].entity_id = E;
			RespawnTab[SpawnPos].used = true;
			RespawnTab[SpawnPos].entity_pos = anim.Pos;
			RespawnTab[SpawnPos].RotY = Entity_RotY;
			RespawnTab[SpawnPos].Timer = RespawnTime;
			RespawnTab[SpawnPos].pickedup = true;
			SpawnPos++;
			break;

			case 21:	//N
			anim.Rot_Vel.Y = 0.15f;
			RespawnTab[SpawnPos].list_id = (anim.Pos.X + 8.0f*anim.Pos.Y + 70.0f*anim.Pos.Z);
			RespawnTab[SpawnPos].entity_id = N;
			RespawnTab[SpawnPos].used = true;
			RespawnTab[SpawnPos].entity_pos = anim.Pos;
			RespawnTab[SpawnPos].RotY = Entity_RotY;
			RespawnTab[SpawnPos].Timer = RespawnTime;
			RespawnTab[SpawnPos].pickedup = true;
			SpawnPos++;
			break;

			case 22:		// ZEBAT
			RespawnTab[SpawnPos].list_id = (anim.Pos.X + 8.0f*anim.Pos.Y + 70.0f*anim.Pos.Z);
			RespawnTab[SpawnPos].entity_id = ZEBAT;
			RespawnTab[SpawnPos].used = true;
			RespawnTab[SpawnPos].entity_pos = anim.Pos;
			RespawnTab[SpawnPos].Timer = 0;
			RespawnTab[SpawnPos].pickedup = false;
			anim.id_list = RespawnTab[SpawnPos].list_id;
			SpawnPos++;
			o_AddEntityToList(ZEBAT,-1,anim,false);
			break;

			case 23:	//FIOLE2
			if(gAllGoalCleared) break;
			RespawnTab[SpawnPos].list_id = (anim.Pos.X + 8.0f*anim.Pos.Y + 70.0f*anim.Pos.Z);
			RespawnTab[SpawnPos].entity_id = FIOLE2;
			RespawnTab[SpawnPos].used = true;
			RespawnTab[SpawnPos].entity_pos = anim.Pos;
			RespawnTab[SpawnPos].Timer = 0;
			RespawnTab[SpawnPos].pickedup = true;
			SpawnPos++;
			break;

			case 24:	//FIOLE3
			RespawnTab[SpawnPos].list_id = (anim.Pos.X + 8.0f*anim.Pos.Y + 70.0f*anim.Pos.Z);
			RespawnTab[SpawnPos].entity_id = FIOLE3;
			RespawnTab[SpawnPos].used = true;
			RespawnTab[SpawnPos].entity_pos = anim.Pos;
			RespawnTab[SpawnPos].Timer = 0;
			RespawnTab[SpawnPos].pickedup = true;
			SpawnPos++;
			break;

			case 25:		// SUBZERAT
			RespawnTab[SpawnPos].list_id = (anim.Pos.X + 8.0f*anim.Pos.Y + 70.0f*anim.Pos.Z);
			RespawnTab[SpawnPos].entity_id = SUBZERAT;
			RespawnTab[SpawnPos].used = true;
			RespawnTab[SpawnPos].entity_pos = anim.Pos;
			RespawnTab[SpawnPos].Timer = 0;
			RespawnTab[SpawnPos].pickedup = false;
			anim.id_list = RespawnTab[SpawnPos].list_id;
			SpawnPos++;
			o_AddEntityToList(SUBZERAT,-1,anim,false);
			break;

			case 26:		// ZEMUMMY
			RespawnTab[SpawnPos].list_id = (anim.Pos.X + 8.0f*anim.Pos.Y + 70.0f*anim.Pos.Z);
			RespawnTab[SpawnPos].entity_id = ZEMUMMY;
			RespawnTab[SpawnPos].used = true;
			RespawnTab[SpawnPos].entity_pos = anim.Pos;
			RespawnTab[SpawnPos].Timer = 0;
			RespawnTab[SpawnPos].pickedup = false;
			anim.id_list = RespawnTab[SpawnPos].list_id;
			SpawnPos++;
			o_AddEntityToList(ZEMUMMY,-1,anim,false);
			break;

			case 27:		// STOP
			gStopPos = anim.Pos;
			m_ConsPrint("StopPos : %f %f %f\n",gStopPos.X,gStopPos.Y,gStopPos.Z);
			break;
		}
	}

	// mapintro hack : gestion des citrouilles..
	if(gMapIntro)
	{
		if(game_IsMapOpened(2) == false)
		{
			RespawnTab[PumpkinId[0]].used = false;
			RespawnTab[PumpkinId[0]].pickedup = false;
			RespawnTab[PumpkinId[1]].used = false;		
			RespawnTab[PumpkinId[1]].pickedup = false;
		}
		else if(game_IsMapOpened(5) == false)
		{
			RespawnTab[PumpkinId[0]].used = false;
			RespawnTab[PumpkinId[0]].pickedup = false;
		}
	}

	gNb_Respawn = SpawnPos;
	return Nb_Entities;
}

void bsp_LoadLightmaps(char *name, FILE *file)
{
	int		i;
	int		j;
	char	temp[255];
	char	path[255];
	char	cpname[255];
	char	*c;
	char	*s;

	m_ConsPrint(">>> %s\n", name);

	strcpy(cpname,name);
	// saute toutes les \blabla\blabla\monfichier.ase
	for(s=0, c=cpname ; *c ; c++)
	{
		if((*c) == '/')	s = c;
		if((*c) == '\\') s = c;
	}
	
	if(s)
	{
		strcpy(temp,s+1);
		strcpy(cpname,temp);
	}
	
//	m_ConsPrint("%s\n", temp);
//	m_ConsPrint("%s\n", cpname);
	
	// enleve l'extension
	for(c=cpname ; *c ; c++)
	if((*c) == '.')
	{
		*c = 0;
		break;
	}
	
//	m_ConsPrint("%s\n", cpname);
	

	fscanf(file,"<LIGHTMAPS> %d\n",&i);
	if(i>MAX_LIGHTMAP)
	{
		m_ConsPrint("too much lightmaps!\n",path);
		return;
	}
	m_ConsPrint("<LIGHTMAP> %d\n",i);

	for(j=0 ; j<i ; j++)
	{
		fscanf(file,"%s\n",temp); // &temp
		
		if(lightmap.value)
		{
			sprintf(path,"%s%s/%s",SYSTEX,cpname,temp);
			m_ConsPrint("%s\n",path); //

			menu_DispLoadMapSplash();
		//	g_DispCenterString(14, 3, "Loading.. %s", path);
		//	gl_SwapBuffer();

			txid_LIGHTMAP_N[j] = im_Load(path,-1,(int)picmip.value, true);
		}
	}
}

void bsp_LoadTextures(FILE *file, int *tx_id_plan)
{
	int		j;
	int		k;
	char	temp[255];

	fscanf(file,"<TEXTURES> %d\n",&j);

	for(k=0 ; k<j ; k++)
	{
		fscanf(file,"%s\n",temp); // &temp

//		menu_DispLoadMapSplash();
	//	g_DispCenterString(14, 3, "Loading.. %s", temp);
//		gl_SwapBuffer();

		tx_id_plan[k] = im_Load(temp,-1,(int)picmip.value, true);
                
                //printf("--> %d\n", texture_id_plan[k]);
                
		if(tx_id_plan[k] < 0)
			tx_id_plan[k] = 0;
	}
        
    //    for(k=0 ; k<j ; k++)
    //    {
    //        printf("--> %d\n", tx_id_plan[k]);
    //    }
}

void bsp_LoadLights(FILE *file)
{
	vect_t	pos;
	int		R, G, B, type;
	float	I;
	int		k, Nb_Lights;
	pEntity	pEnt;

	fscanf(file,"<LIGHTS> %d\n",&Nb_Lights);
	for(k=0 ; k<Nb_Lights ; k++)
	{
		fscanf(file,"%f %f %f %d %d %d %f %d\n",
							&pos.X,
							&pos.Y,
							&pos.Z,
							&R,
							&G,
							&B,
							&I,
							&type);

		for(pEnt=gEntitiesList ; pEnt ; pEnt=pEnt->Next)
		{
			if(pEnt->entity_id != LAMPE && pEnt->entity_id != MLAMPE)
				continue;
			if(pEnt->ent_anim.Pos.X == pos.X)
			if(pEnt->ent_anim.Pos.Y == pos.Y)
			if(pEnt->ent_anim.Pos.Z == pos.Z)
			{
				// color HACK
				pEnt->R = R * 1.2f;
				pEnt->G = G * 1.1f;
				pEnt->B = B * 1.1f;
			}
		}
	}

	if(Nb_Lights)
		gMapLightened = true;
	else
		gMapLightened = false;
}

hBool bsp_Load_MAPFILE(char *name)
{
	FILE		*file;
	int			i, j, k;
	pFace		pCurr=NULL;
	//int			count=0;
	pFace		FaceList;
	Face_t		Face;
	int			Owner1;
	int			Owner2;
	int			nVertices;
	int			n;
	int			pvs_size;
	int			Nb_pvs;
	int			Id;
	int			id_face;
	char		*p;
	
#ifdef H_MAC
	p = KMiscTools::makeFilePath(name);
	file = fopen(p,"r");
#else
	file = fopen(name,"r");
#endif
	
	if(file==NULL)
		return false;

	fscanf(file,"<MAP>\n");
	bsp_LoadStartPosition(file);
	GlobalEntities = bsp_LoadEntity(file);
	if(GlobalEntities < 0)
		return false;
	bsp_LoadTextures(file,texture_id_plan);
	bsp_LoadLights(file);
	bsp_LoadLightmaps(name, file);


//	menu_DispLoadMapSplash();
//	g_DispCenterString(14, 3, "Loading.. %s", name);
//	gl_SwapBuffer();

	
	fscanf(file,"<NODES> %d\n",&gNb_nodes);
	m_ConsPrint("<NODES> %d\n",gNb_nodes); //
	NodeArray = (noeud_t*)s_malloc(gNb_nodes*sizeof(noeud_t));
	memset(NodeArray,0,gNb_nodes*sizeof(noeud_t));
	for(i=0 ; i<gNb_nodes ; i++)
	{
		fscanf(file,"NODE[%d] LEAF[%d] FNODE[%d] BNODE[%d] POP[%f %f %f] VN[%f %f %f]\n",
			&j,
			&NodeArray[i].Feuille,
			&NodeArray[i].Front,
			&NodeArray[i].Back,
			&NodeArray[i].Plan.PointOnPlane.X,
			&NodeArray[i].Plan.PointOnPlane.Y,
			&NodeArray[i].Plan.PointOnPlane.Z,
			&NodeArray[i].Plan.VecteurNormal.X,
			&NodeArray[i].Plan.VecteurNormal.Y,
			&NodeArray[i].Plan.VecteurNormal.Z);
	}



//	col2_init_World();

	fscanf(file,"<FACES> %d\n",&gNb_faces);
	m_ConsPrint("<FACES> %d\n",gNb_faces); //
	FaceArray = (Face_t*)s_malloc(gNb_faces*sizeof(Face_t));
	memset(FaceArray,0,gNb_faces*sizeof(Face_t));
	for(k=0 ; k<gNb_faces ; k++)
	{
		fscanf(file,"%d %d %d %d %d\n",&id_face,&Face.NumberOfVertices,&Face.TextureID,&Face.LightID,&Face.Type);					
		Face.TextureID = texture_id_plan[Face.TextureID];             
		if(Face.NumberOfVertices>8)
		{
			m_ConsPrint("Too much vertices in a face\n");
			fclose(file);
			return false;
		}
		if(Face.NumberOfVertices != 3)
		{
			m_ConsPrint("vertice : %d error\n",Face.NumberOfVertices);
		}
		else
		{
			for(i=0 ; i<Face.NumberOfVertices ; i++)
			{
				fscanf(file,"%f %f %f %f %f %f %f\n",
						&Face.Vertice[i].X,
						&Face.Vertice[i].Y,
						&Face.Vertice[i].Z,
						&Face.Vertice[i].U,
						&Face.Vertice[i].V,
						&Face.Vertice[i].U_lm,
						&Face.Vertice[i].V_lm);
			}
		}

	//	col2_AddFace_Word(	Face.Vertice[0].X, Face.Vertice[0].Y, Face.Vertice[0].Z,
	//						Face.Vertice[1].X, Face.Vertice[1].Y, Face.Vertice[1].Z,
	//						Face.Vertice[2].X, Face.Vertice[2].Y, Face.Vertice[2].Z);

		Face.Plan.VecteurNormal = GetFaceNormal(Face.Vertice[0],Face.Vertice[1],Face.Vertice[2]);		
		Face.Plan.PointOnPlane.X = Face.Vertice[0].X;
		Face.Plan.PointOnPlane.Y = Face.Vertice[0].Y;
		Face.Plan.PointOnPlane.Z = Face.Vertice[0].Z;

		Face.Next = NULL;
		FaceArray[k] = Face;
	}

//	col2_FinalizeWorld();



	fscanf(file,"<LEAFS> %d\n",&gNb_Leafs);
	m_ConsPrint("<LEAFS> %d\n",gNb_Leafs); //
	LeafArray = (leafs_t*)s_malloc(gNb_Leafs*sizeof(leafs_t));
	memset(LeafArray,0,gNb_Leafs*sizeof(leafs_t));
	for(k=0 ; k<gNb_Leafs ; k++)
	{
		LeafArray[k].Nb_portals = 0;
		fscanf(file,"%d ",&Id);
		LeafArray[k].LeafID = Id;
		fscanf(file,"%d %d\n",&LeafArray[Id].Nb_faces,&LeafArray[Id].PVSIndex);
		fscanf(file,"%f %f %f %f %f %f\n",
								&LeafArray[Id].bbox.BoxMin.X,
								&LeafArray[Id].bbox.BoxMin.Y,
								&LeafArray[Id].bbox.BoxMin.Z,
								&LeafArray[Id].bbox.BoxMax.X,
								&LeafArray[Id].bbox.BoxMax.Y,
								&LeafArray[Id].bbox.BoxMax.Z);
		
		LeafArray[k].FaceIndexList = (int*)s_malloc(LeafArray[Id].Nb_faces*sizeof(int));
		memset(LeafArray[k].FaceIndexList,0,LeafArray[Id].Nb_faces*sizeof(int));

		for(j=0 ; j<LeafArray[Id].Nb_faces ; j++)
		{
			fscanf(file,"%d ",&LeafArray[k].FaceIndexList[j]);
		}
		fscanf(file,"\n");

		FaceList = NULL;
		for(j=0 ; j<LeafArray[Id].Nb_faces ; j++)
		{
			pCurr = (Face_t*)s_malloc(sizeof(Face_t));
			memset(pCurr,0,sizeof(Face_t));

			pCurr->NumberOfVertices = FaceArray[LeafArray[k].FaceIndexList[j]].NumberOfVertices;
			pCurr->TextureID = FaceArray[LeafArray[k].FaceIndexList[j]].TextureID;
			pCurr->LightID = FaceArray[LeafArray[k].FaceIndexList[j]].LightID;
			pCurr->Type = FaceArray[LeafArray[k].FaceIndexList[j]].Type;
			pCurr->Plan = FaceArray[LeafArray[k].FaceIndexList[j]].Plan;

			if(pCurr->NumberOfVertices>8)
			{
				m_ConsPrint("Too much vertices in a face\n");
				fclose(file);
				return false;
			}
			if(pCurr->NumberOfVertices!=3)
				m_ConsPrint("vertice : %d error\n",pCurr->NumberOfVertices);

			for(i=0 ; i<pCurr->NumberOfVertices ; i++)
			{
				pCurr->Vertice[i] = FaceArray[LeafArray[k].FaceIndexList[j]].Vertice[i];
			}
			pCurr->Next = FaceList;
			FaceList = pCurr;
		}
		LeafArray[k].faceliste = FaceList;
	}

	if(!ab_Load(file))
	{
		m_ConsPrint("ab loading error\n");
		fclose(file);
		return false;
	}

	fscanf(file,"<PORTALS> %d\n",&gNb_Portals);
	m_ConsPrint("<PORTALS> %d\n",gNb_Portals); //

	PortalArray = (portal_t*)s_malloc(gNb_Portals*sizeof(portal_t));
	memset(PortalArray,0,gNb_Portals*sizeof(portal_t));

	for(i=0 ; i<gNb_Portals ; i++)
	{
		fscanf(file,"%d\n",&Owner1);
		fscanf(file,"%d\n",&Owner2);
		fscanf(file,"%d\n",&nVertices);

		PortalArray[i].Vertice = (Point_t*)s_malloc(nVertices*sizeof(Point_t));

		PortalArray[i].NumberOfVertices = nVertices;
		PortalArray[i].LeafOwnerArray[0] = Owner1;
		PortalArray[i].LeafOwnerArray[1] = Owner2;

		fscanf(file,"%f %f %f %f %f %f\n",
					&PortalArray[i].Plan.VecteurNormal.X,
					&PortalArray[i].Plan.VecteurNormal.Y,
					&PortalArray[i].Plan.VecteurNormal.Z,
					&PortalArray[i].Plan.PointOnPlane.X,
					&PortalArray[i].Plan.PointOnPlane.Y,
					&PortalArray[i].Plan.PointOnPlane.Z);

		for(j=0 ; j<PortalArray[i].NumberOfVertices ; j++)
		{
			fscanf(file,"PORTAL %d : %f %f %f\n",
				&n,
				&PortalArray[i].Vertice[j].X,
				&PortalArray[i].Vertice[j].Y,
				&PortalArray[i].Vertice[j].Z);
		}
	}

	for(i=0 ; i<gNb_Portals ; i++)
	{
		Owner1 = PortalArray[i].LeafOwnerArray[0];
		Owner2 = PortalArray[i].LeafOwnerArray[1];

		if(LeafArray[Owner1].Nb_portals < MAX_PINDEX_PR_LEAF)
		{
			LeafArray[Owner1].portalIndex[LeafArray[Owner1].Nb_portals] = i;
			LeafArray[Owner1].Nb_portals++;
		}

		if(LeafArray[Owner2].Nb_portals < MAX_PINDEX_PR_LEAF)
		{
			LeafArray[Owner2].portalIndex[LeafArray[Owner2].Nb_portals] = i;
			LeafArray[Owner2].Nb_portals++;
		}
	}

	NLlist.NbNL = 0;
	NLlist.NLData = (int*)s_malloc(gNb_Leafs*sizeof(int));
	memset(NLlist.NLData,0,gNb_Leafs*sizeof(int));

	pvs_size=gNb_Leafs*gNb_Leafs;
	PVSData = (char*)s_malloc(pvs_size*sizeof(char));
	memset(PVSData,'1',sizeof(char)*pvs_size);

	fscanf(file,"<PVS> %d\n",&Nb_pvs);
	for(i=0 ; i<pvs_size ; i++)
		fscanf(file,"%c",&PVSData[i]);
	
//	fscanf(file,"\n");
//	fscanf(file,"@");

	fclose(file);
	return true;
}

void bsp_Unload_MAPFILE()
{
	int		k;

	if(PVSData)
	{
		s_free(PVSData);
		PVSData = NULL;
	}
	if(NLlist.NLData)
	{
		s_free(NLlist.NLData);
		NLlist.NLData = NULL;
	}
	if(LeafArray)
	{
		for(k=0 ; k<gNb_Leafs ; k++)
		{
			LeafArray[k].faceliste = liste_Delete(LeafArray[k].faceliste);
			s_free(LeafArray[k].FaceIndexList);
			LeafArray[k].FaceIndexList = NULL;
		}
		gNb_Leafs = 0;
		s_free(LeafArray);
		LeafArray = NULL;
	}
	if(NodeArray)
	{
		s_free(NodeArray);
		NodeArray = NULL;
		gNb_nodes = 0;
	}
	if(FaceArray)
	{
		s_free(FaceArray);
		FaceArray = NULL;
		gNb_faces = 0;
	}
	if(PortalArray)
	{
		for(k=0 ; k<gNb_Portals ; k++)
			s_free(PortalArray[k].Vertice);
		gNb_Portals = 0;
		s_free(PortalArray);
		PortalArray = NULL;
	}
}

hBool bsp_Load_POLYFILE(char *path)
{
	FILE	*file;
	Face_t	*Poly;
	int		i;
	int 	j;
	int		tmp;
	char	temp[255];

	if(PolyList)
	{
		m_ConsPrint("Polygon List not empty\n");
		return false;
	}

#ifdef H_MAC
	path = KMiscTools::makeFilePath(path);
#endif

	file = fopen(path,"rb");
	if(!file)
		return false;
	fscanf(file,"%s\n",temp);

	bsp_LoadStartPosition(file);
	GlobalEntities = bsp_LoadEntity(file);
	bsp_LoadTextures(file, texture_id_plan);
	bsp_LoadLights(file);
	
	nbTabFaces = 0;

	while(!feof(file))
	{
		Poly = (Face_t*)s_malloc(sizeof(Face_t));

		fscanf(file,"%d %d %d %d\n",&Poly->NumberOfVertices,&Poly->TextureID,&Poly->LightID,&Poly->Type);
		if(feof(file))
		{
			free(Poly);
			break;
		}
		
		nbTabFaces++;
                
      	//printf("%d %d %d %d\n", Poly->NumberOfVertices, Poly->TextureID, Poly->LightID, Poly->Type);
		Poly->TextureID = texture_id_plan[Poly->TextureID];
                
                // dev hack
            //    printf(" %d", Poly->TextureID);

		if(Poly->NumberOfVertices > 8)
		{
			m_ConsPrint("Inavlide vertex with %d points\n",Poly->NumberOfVertices);
			free(Poly);
			fclose(file);
			return false;
		}

		for(i=0 ; i<Poly->NumberOfVertices ; i++)
		{
			fscanf(file,"%f %f %f %f %f %f %f\n",
					&Poly->Vertice[i].X,
					&Poly->Vertice[i].Y,
					&Poly->Vertice[i].Z,
					&Poly->Vertice[i].U,
					&Poly->Vertice[i].V,
					&Poly->Vertice[i].U_lm,
					&Poly->Vertice[i].V_lm);
		}

		Poly->Plan.VecteurNormal = GetFaceNormal(Poly->Vertice[0],Poly->Vertice[1],Poly->Vertice[2]);		
		Poly->Plan.PointOnPlane.X = Poly->Vertice[0].X;
		Poly->Plan.PointOnPlane.Y = Poly->Vertice[0].Y;
		Poly->Plan.PointOnPlane.Z = Poly->Vertice[0].Z;

		Poly->Next = PolyList;
		PolyList = Poly;
	}
	fclose(file);
	
	// place chaque face dans un tableau que l'on cree dynamiquement
	gTabFaces = (pTabFace)s_malloc(sizeof(Facetab_t)*nbTabFaces);
	Poly = PolyList;
	for(i=0 ; i<nbTabFaces ; i++)
	{
		gTabFaces[i].id = i;
		gTabFaces[i].sorted_id = Poly->TextureID;
		gTabFaces[i].FacePtr = Poly;
	//	m_ConsPrint("%d ", gTabFaces[i].sorted_id);
		Poly = Poly->Next;
	}
	
	// tri les faces suivant l'id texture
	for(i=0 ; i<nbTabFaces ; i++)
	{
		for(j=0 ; j<nbTabFaces ; j++)
		{
			if(gTabFaces[i].sorted_id > gTabFaces[j].sorted_id)
			{
				tmp = gTabFaces[i].sorted_id;
				gTabFaces[i].sorted_id = gTabFaces[j].sorted_id;
				gTabFaces[j].sorted_id = tmp;
				
				tmp = gTabFaces[i].id;
				gTabFaces[i].id = gTabFaces[j].id;
				gTabFaces[j].id = tmp;
			}
		}
	}
			
	return true;
}
