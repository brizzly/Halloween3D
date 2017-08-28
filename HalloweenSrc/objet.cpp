
#include "alloween.cpp"

#ifdef H_MAC
#include "KMiscTools.h"
#endif

#define	FLAMETAG	"flame"

ENTITY	*gEntitiesLoadedList = NULL;
int		gNumberOfLoadedEntities;
pEntity	gEntitiesList=NULL;
int		gNb_Respawn=0;
RESPAWN	RespawnTab[MAX_RESPAWN_OBJ];
hBool	PlayerOnLift;
vect_t	gStopPos = { MAX_CAMERA, MAX_CAMERA, MAX_CAMERA };


hBool o_LoadEntStruct()
{
	gEntitiesLoadedList = (ENTITY*)s_malloc(MAX_ENTITIES_TYPE*sizeof(ENTITY));
	if(!gEntitiesLoadedList)
		return false;
	memset(gEntitiesLoadedList,0,MAX_ENTITIES_TYPE*sizeof(ENTITY));
	gNumberOfLoadedEntities=0;

	return true;
}

void o_UnloadEntStruct()
{
	Face_t	*curr;
	Face_t	*pList;
	int		i;

	m_ConsPrint("o_UnloadEntStruct\n");
	if(gEntitiesLoadedList)
	{
		for(i=0 ; i<MAX_ENTITIES_TYPE ; i++)
		{
			pList = gEntitiesLoadedList[i].FaceList;
			while(pList)
			{
				curr = pList;
				pList = pList->Next;
				s_free(curr);
			}
			pList = gEntitiesLoadedList[i].Boundaries;
			while(pList)
			{
				curr = pList;
				pList = pList->Next;
				s_free(curr);
			}		
		}
		s_free(gEntitiesLoadedList);
	}
	m_ConsPrint("o_UnloadEntStruct done.\n");
}

void o_ClearEntityList()
{
	pEntity	pNewEntity = NULL;

	while(gEntitiesList)
	{
		pNewEntity = gEntitiesList;
		gEntitiesList = gEntitiesList->Next;
		o_FreeEntity(pNewEntity);
	}
}

void o_FreeEntity(pEntity Ent)
{
	if(Ent->entity_id == DEPLA)
		msk_uncloneAnimModelData(&gModel_depla,Ent->parent_id);
	else if(Ent->entity_id == MORBAK)
		msk_uncloneAnimModelData(&gModel_morbak,Ent->parent_id);
	else if(Ent->entity_id == ZEBAT)
		msk_uncloneAnimModelData(&gModel_zebat,Ent->parent_id);
	else if(Ent->entity_id == SUBZERAT)
		msk_uncloneAnimModelData(&gModel_subzerat,Ent->parent_id);
	else if(Ent->entity_id == ZEMUMMY)
		msk_uncloneAnimModelData(&gModel_zemummy,Ent->parent_id);

	s_free(Ent);
}

void o_UpdateEntities()
{
	pEntity		pEntityList;
	pEntity		pTemp;
	pEntity		PlayerEnt;
	int			FlameAnim;
	int			nb_entity;

	PlayerEnt = o_FindEntity(PERSO);
	if(!PlayerEnt)
		return;

	if(game_MatrixMode)
	{
		for(pEntityList=gEntitiesList ; pEntityList ; )
		{
			pTemp = pEntityList;
			pEntityList=pEntityList->Next;

			pTemp = SabreProcess(pTemp);
			pTemp = GunProcess(pTemp);
			pTemp = MonstreProcess(pTemp);
			pTemp = UpdateMonster(PlayerEnt,pTemp);

			if(pTemp)
			if(pTemp->parent_id == PERSO)
				pTemp = FlecheProcess(PlayerEnt,pTemp);

			if(pTemp)
			if(pTemp->parent_id == PERSO)
				pTemp = BombProcess(pTemp,PlayerEnt);
		}
		return;
	}
	
	nb_entity = 0;
	FlameAnim = 0;
	for(pEntityList=gEntitiesList ; pEntityList ; )
	{
		nb_entity++;
		pTemp = pEntityList;
		pEntityList=pEntityList->Next;

		pTemp = UpdateMonster(PlayerEnt,pTemp);
		pTemp = FlecheProcess(PlayerEnt,pTemp);
		pTemp = GunProcess(pTemp);
		pTemp = AmmoProcess(pTemp);
		pTemp = PumpkinProcess(pTemp);
		pTemp = SabreProcess(pTemp);
		pTemp = BombProcess(pTemp,PlayerEnt);
		pTemp = GibProcess(pTemp,PlayerEnt);
		pTemp = ExplosionProcess(pTemp);
		pTemp = MonstreProcess(pTemp);
		pTemp = LampeProcess(pTemp,&FlameAnim);
		pTemp = LettreProcess(pTemp);


		if(gIsMultiplayer && !gIsServer)
		{
			pTemp = PangProcess(pTemp, o_FindEntity(MORBAK));
		}
		else
		{
			pTemp = PangProcess(pTemp, PlayerEnt);
		}

		
		if(!pTemp)
			continue;

		if(phy_BoundPos(&pTemp->ent_anim.Pos))
		{
			if(	pTemp->entity_id == FLECHE		||
				pTemp->entity_id == SPIKE		||
				pTemp->entity_id == SPINE		||
				pTemp->entity_id == TOILE		||
				pTemp->entity_id == DEPLASPIKE	||
				pTemp->entity_id == BOMBE)
			{
				o_RemoveEntityFromList(pTemp);
				pEntityList = gEntitiesList;
				continue;
			}
		}

		if(	pTemp->entity_id == FLECHE		||
			pTemp->entity_id == SPIKE		||
			pTemp->entity_id == SPINE		||
			pTemp->entity_id == DEPLASPIKE	||
			pTemp->entity_id == DEPLAHEAD	||
			pTemp->entity_id == MORBAKBODY	||
			pTemp->entity_id == TOILE		||
			pTemp->entity_id == PERSO		||	// HACK en plus ...
			pTemp->entity_id == DEPLA		||	//
			pTemp->entity_id == MORBAK		||	//
			pTemp->entity_id == ZEBAT		||	//
			pTemp->entity_id == SUBZERAT	||	//
			pTemp->entity_id == ZEMUMMY		||	//
			pTemp->entity_id == BOMBE)
			continue;

		pTemp->ent_anim.Pos.X += (pTemp->ent_anim.Pos_Vel.X * gFrameTime_msec);
		pTemp->ent_anim.Pos.Y += (pTemp->ent_anim.Pos_Vel.Y * gFrameTime_msec);
		pTemp->ent_anim.Pos.Z += (pTemp->ent_anim.Pos_Vel.Z * gFrameTime_msec);

		pTemp->ent_anim.Rot.X += (pTemp->ent_anim.Rot_Vel.X * gFrameTime_msec);
		pTemp->ent_anim.Rot.Y += (pTemp->ent_anim.Rot_Vel.Y * gFrameTime_msec);
		pTemp->ent_anim.Rot.Z += (pTemp->ent_anim.Rot_Vel.Z * gFrameTime_msec);

		pTemp->ent_anim.Siz.X += (pTemp->ent_anim.Siz_Vel.X * gFrameTime_msec);
		pTemp->ent_anim.Siz.Y += (pTemp->ent_anim.Siz_Vel.Y * gFrameTime_msec);
		pTemp->ent_anim.Siz.Z += (pTemp->ent_anim.Siz_Vel.Z * gFrameTime_msec);

		pTemp->ent_anim.Rot.X = cam_RotationCycle(pTemp->ent_anim.Rot.X);
		pTemp->ent_anim.Rot.Y = cam_RotationCycle(pTemp->ent_anim.Rot.Y);
		pTemp->ent_anim.Rot.Z = cam_RotationCycle(pTemp->ent_anim.Rot.Z);

	//	if(phy_BoundPos(&pTemp->ent_anim.Pos))
	//	{
	//	}
	}

//	if(developer.value)
//		g_DispString(CONSOLE_FONT,"%d entities",nb_entity);
}

hBool o_IsVisible(pEntity Ent)
{
	hBool	Is_visible;
	int		leaf_ent;

	Is_visible = true;
	if(MapDataFormat == MAP_BSP)
	{
		if(visent.value)
			return true;
		leaf_ent = bsp_FindCamera(Ent->ent_anim.Pos);
		if(leaf_ent == -1)
		{
			Is_visible = false;
			if(Ent->entity_id != PERSO)
			{
				if(ghost.value && Ent->player_holded)
				{

				}
				else
				{
					if(	Ent->entity_id != DEPLA		&&
						Ent->entity_id != MORBAK	&&
						Ent->entity_id != ZEBAT		&&
						Ent->entity_id != SUBZERAT	&&
						Ent->entity_id != ZEMUMMY)
					{
						if(gIsMultiplayer && !gIsServer)
							return false;
						if(developer.value)
							m_ConsPrint("Outmap entity removed id: %d\n", Ent->entity_id);
						o_RemoveEntityFromList(Ent);
						return false;
					}
				}
			}
		}
		else
		{
			if(gLeafLocation != -1)
			{
				if(PVSData[LeafArray[gLeafLocation].PVSIndex+leaf_ent] == '0')
					Is_visible = false;
			}
			if(!frustrum_LeafIn(gFrustrumPlanes,leaf_ent))
				Is_visible = false;
		}
	}
	// NET HACK
	if(gIsMultiplayer && !gIsServer)
	{
		if(Ent->entity_id == MORBAK && player_view.value)
			return false;
	}
	if(!gIsMultiplayer || gIsServer)
	{
		if(Ent->entity_id == PERSO && player_view.value)
			return false;
	}
	if(Ent->player_holded)
		return true;

	return Is_visible;
}

void o_RendSkyObj()
{
	pEntity SkyDomeEnt1;
	pEntity SkyDomeEnt2;
	pEntity PlayerEnt;

	if(!gMapIntro)
		return;

	PlayerEnt = o_FindEntity(PERSO);
	if(!PlayerEnt)
		return;

	glPushMatrix();
	{
	//	glTranslatef(gCamera.Pos.X, 0, gCamera.Pos.Z);
		
		SkyDomeEnt2 = o_FindEntity(SKYDOME2);
		if(SkyDomeEnt2)
		{
			gl_DrawEntity(SkyDomeEnt2, PlayerEnt, 1, 1, 1);
		}

		SkyDomeEnt1 = o_FindEntity(SKYDOME);
		if(SkyDomeEnt1)
		{
			SkyDomeEnt1->Blending = true;
			SkyDomeEnt1->BlendVal = 0.5f;
			gl_DrawEntity(SkyDomeEnt1, PlayerEnt, 1, 1, 1);
		}
	}
	glPopMatrix();


	/*
	pEntity Ent;
	pEntity PlayerEnt;

	if(!gMapIntro)
		return;

	PlayerEnt = o_FindEntity(PERSO);
	if(!PlayerEnt)
		return;
//	Ent = o_FindEntity(SKYDOME2);
//	if(!Ent)
//		return;
//	gl_DrawEntity(Ent, PlayerEnt, 1, 1, 1);

	Ent = o_FindEntity(SKYDOME);
	if(!Ent)
		return;
	gl_DrawEntity(Ent, PlayerEnt, WorldColorR, WorldColorG, WorldColorB);
	*/
}

void o_RendCautchFlask()
{
	int	flask_id;
	pEntity Ent;
	pEntity PlayerEnt;

	if(gIsServer && net_dedicated.value)
		return;

	if(ProgramState == PS_WIN || ProgramState == PS_ENDSCORE)
		return;

	if(game_fiole1_catched == true)
	{
		flask_id = FIOLE1;
	}
	else if(game_fiole2_catched == true)
	{
		flask_id = FIOLE2;
	}
	else if(game_fiole3_catched == true)
	{
		flask_id = FIOLE3;
	}
	else
	{
		return;
	}
	PlayerEnt = o_FindEntity(PERSO);
	if(!PlayerEnt)
		return;
	Ent = o_FindHoldedEntity(flask_id);
	if(!Ent)
		return;
	gl_ProjectView(true, 800, 600);
	{
		gl_DrawEntity(Ent, PlayerEnt, 1, 1, 1);
	}
	gl_ProjectView(false, 800, 600);
}

void o_RemoveHudFlask()
{
	pEntity	Ent;

	Ent = o_FindHoldedEntity(FIOLE1);
	if(Ent)
	{
		o_RemoveEntityFromList(Ent);
	}

	Ent = o_FindHoldedEntity(FIOLE2);
	if(Ent)
	{
		o_RemoveEntityFromList(Ent);
	}

	Ent = o_FindHoldedEntity(FIOLE3);
	if(Ent)
	{
		o_RemoveEntityFromList(Ent);
	}
}

void o_AddHudFlask(int flask_id)
{
	int			id;
	ANIMENTITY	anim;

	if(flask_id == 1)
	{
		id = FIOLE1;
	}
	else if(flask_id == 2)
	{
		id = FIOLE2;
	}
	else if(flask_id == 3)
	{
		id = FIOLE3;
	}
	else
	{
		return;
	}
	o_RemoveHudFlask();
	o_ClearAnimFields(&anim);
	anim.Pos.X = 750;
	anim.Pos.Y = 535;
	anim.Pos.Z = 0;
	anim.Siz.X = 6;
	anim.Siz.Y = 6;
	anim.Siz.Z = 6;
	anim.Rot_Vel.Y = 0.15f;
	anim.Rot.X = 10.0f;
	o_AddEntityToList(id, -1, anim, true);
}

void o_RendCautchletters()
{
	int		i;
	pEntity EntLetters[9];
	pEntity PlayerEnt;

	if(gIsServer && net_dedicated.value)
		return;

	if(ProgramState == PS_WIN || ProgramState == PS_ENDSCORE)
		return;

	PlayerEnt = o_FindEntity(PERSO);
	if(!PlayerEnt)
		return;
	EntLetters[0] = o_FindHoldedEntity(H);
	EntLetters[1] = o_FindHoldedEntity(A);
	EntLetters[2] = o_FindHoldedEntity2(L,1);
	EntLetters[3] = o_FindHoldedEntity2(L,2);
	EntLetters[4] = o_FindHoldedEntity(O);
	EntLetters[5] = o_FindHoldedEntity(W);
	EntLetters[6] = o_FindHoldedEntity2(E,1);
	EntLetters[7] = o_FindHoldedEntity2(E,2);
	EntLetters[8] = o_FindHoldedEntity(N);

	gl_ProjectView(true, 800, 600);
	for(i=0 ; i<9 ; i++)
	if(EntLetters[i])
	{
		gl_DrawEntity(EntLetters[i], PlayerEnt, 1, 1, 1);
	}
	gl_ProjectView(false, 800, 600);
}

void o_RemoveHudLetters()
{
	int		i;
	pEntity EntLetters[9];

	EntLetters[0] = o_FindHoldedEntity(H);
	EntLetters[1] = o_FindHoldedEntity(A);
	EntLetters[2] = o_FindHoldedEntity2(L,1);
	EntLetters[3] = o_FindHoldedEntity2(L,2);
	EntLetters[4] = o_FindHoldedEntity(O);
	EntLetters[5] = o_FindHoldedEntity(W);
	EntLetters[6] = o_FindHoldedEntity2(E,1);
	EntLetters[7] = o_FindHoldedEntity2(E,2);
	EntLetters[8] = o_FindHoldedEntity(N);

	for(i=0 ; i<9 ; i++)
	{
		if(EntLetters[i])
		{
			o_RemoveEntityFromList(EntLetters[i]);
		}
	}
}

void o_AddHudLetter(int letter_id)
{
	int			offset[9] = { 40, 80, 120, 160, 200, 240, 280, 320, 360 };
	int			id;
	int			offset_id;
	ANIMENTITY	anim;

	switch(letter_id)
	{
		case 'H':
		id = H;
		offset_id = 0;
		break;

		case 'A':
		id = A;
		offset_id = 1;
		break;

		case 'l':
		id = L;
		offset_id = 2;
		break;

		case 'L':
		id = L;
		offset_id = 3;
		break;

		case 'O':
		id = O;
		offset_id = 4;
		break;

		case 'W':
		id = W;
		offset_id = 5;
		break;

		case 'e':
		id = E;
		offset_id = 6;
		break;

		case 'E':
		id = E;
		offset_id = 7;
		break;

		case 'N':
		id = N;
		offset_id = 8;
		break;

		default:
		return;
	}

	o_ClearAnimFields(&anim);
	anim.Pos.X = 30; //(float)offset[offset_id];
	anim.Pos.Y = 535 - (float)offset[offset_id]; //535;
	anim.Pos.Z = 0;
	anim.Siz.X = 2.5f; //3;
	anim.Siz.Y = 1.5f;
	anim.Siz.Z = 3;
	anim.Rot_Vel.Y = 0.15f;
	anim.Rot.X = 10.0f;
	o_AddEntityToList(id, -1, anim, true);
}

void o_RenderEntitiesList()
{
	pEntity	pEntityList;
	pEntity	pEntityNext;
	pEntity	PlayerEnt = NULL;
	pEntity	WeaponHoldedEnt = NULL;
	float	dist;
	int		i;
	float	R, G, B;

	if(!render_ent.value)
		return;

	if(ProgramState != PS_GAMEOVER)
	{
		PlayerEnt = o_FindEntity(PERSO);
		if(!PlayerEnt)
			return;
	}

	o_RendSkyObj();

	gNbSort = 0;
	for(pEntityList=gEntitiesList ; pEntityList ; )
	{
		pEntityNext = pEntityList->Next;

		if(	pEntityList->entity_id == SKYDOME ||
			pEntityList->entity_id == SKYDOME2)
		{
			pEntityList = pEntityNext;
			continue;
		}

		if(	pEntityList->entity_id == FIOLE1 ||
			pEntityList->entity_id == FIOLE2 ||
			pEntityList->entity_id == FIOLE3)
		{
			if(pEntityList->player_holded)
			{
				pEntityList = pEntityNext;
				continue;
			}
		}

		if(	pEntityList->entity_id >= H &&
			pEntityList->entity_id <= N)
		{
			if(pEntityList->player_holded)
			{
				pEntityList = pEntityNext;
				continue;
			}
		}

		if(	pEntityList->entity_id == PUMPKIN)
		{
			if(pEntityList->player_holded)
			{
				pEntityList = pEntityNext;
				continue;
			}
		}

		if(pEntityList->player_holded)
		{
			WeaponHoldedEnt = pEntityList;
			pEntityList = pEntityNext;
			continue;
		}

		//
		if(!o_IsVisible(pEntityList))
		{
			pEntityList = pEntityNext;
			continue;
		}
		//

		if(pEntityList->Blending && gNbSort < MAX_TABSORT)
		{
			TabSort.Ent[gNbSort] = pEntityList;
			gNbSort++;
			pEntityList = pEntityNext;
			continue;
		}

		pEntityList = pEntityNext;
	}

	// tri les objets transparents
	if(developer.value)
		if(gNbSort >= MAX_TABSORT)
		g_DispString(14,"Blend objets overflow: %d",gNbSort);
	sort_Process();

	// affiche les objets transparents
	for(i=gNbSort-1 ; i>=0 ; i--)
	{
		if(TabSort.sortedid[i] < 0 || TabSort.sortedid[i] >= gNbSort)
			continue;
		pEntityList = TabSort.Ent[TabSort.sortedid[i]];
		if(pEntityList == NULL)
			continue;
		if(pEntityList == WeaponHoldedEnt)
			continue;
		
		if(ProgramState != PS_GAMEOVER)
		{
			if(!o_IsVisible(pEntityList))
				continue;
			gForceNoDepthTest = true;
			o_GetLightningIntensity(pEntityList->entity_id, pEntityList->ent_anim.Pos, &R, &G, &B);
			gl_DrawEntity(pEntityList,PlayerEnt, R, G, B);
		}
		else
		{
			gForceNoDepthTest = true;
			gl_DrawEntity(pEntityList,PlayerEnt, 1, 1, 1);
		}
		gForceNoDepthTest = false; //

		if(draw_bound.value)
			gl_DrawBoundingBox(pEntityList);
	}

	// affiche les objets non transparents
	for(pEntityList=gEntitiesList ; pEntityList ; )
	{
		pEntityNext = pEntityList->Next;

		if(	pEntityList->entity_id == SKYDOME ||
			pEntityList->entity_id == SKYDOME2)
		{
			pEntityList = pEntityNext;
			continue;
		}

		if(	pEntityList->entity_id == FIOLE1 ||
			pEntityList->entity_id == FIOLE2 ||
			pEntityList->entity_id == FIOLE3)
		{
			if(pEntityList->player_holded)
			{
				pEntityList = pEntityNext;
				continue;
			}
		}

		if(	pEntityList->entity_id >= H &&
			pEntityList->entity_id <= N)
		{
			if(pEntityList->player_holded)
			{
				pEntityList = pEntityNext;
				continue;
			}
		}

		if(	pEntityList->entity_id == PUMPKIN)
		{
			if(pEntityList->player_holded)
			{
				pEntityList = pEntityNext;
				continue;
			}
		}

		if(pEntityList->player_holded)
		{
			WeaponHoldedEnt = pEntityList;
			pEntityList = pEntityNext;
			continue;
		}

		if(!o_IsVisible(pEntityList))
		{
			pEntityList = pEntityNext;
			continue;
		}

		if(pEntityList->Blending)
		{
			pEntityList = pEntityNext;
			continue;
		}

		if(pEntityList->parent_id == PERSO)
		if(pEntityList->entity_id == FLECHE ||
			pEntityList->entity_id == SPIKE ||
			pEntityList->entity_id == BOMBE)
		if(!pEntityList->Timing)
		{
			dist = m_Distance(PlayerEnt->ent_anim.Pos,pEntityList->ent_anim.Pos);
			if(dist < 40.0f)
			{
				pEntityList = pEntityNext;
				continue;
			}
		}

		if(ProgramState == PS_GAMEOVER)
		{
			gl_DrawEntity(pEntityList,PlayerEnt, 1, 1, 1);
			pEntityList = pEntityNext;
			continue;
		}

		o_GetLightningIntensity(pEntityList->entity_id, pEntityList->ent_anim.Pos, &R, &G, &B);
		gl_DrawEntity(pEntityList,PlayerEnt, R, G, B);
		
		if(draw_bound.value)
			gl_DrawBoundingBox(pEntityList);

		pEntityList = pEntityNext;
	}

	if(WeaponHoldedEnt)
	{
		gForceNoDepthTest = false;
		o_GetLightningIntensity(PlayerEnt->entity_id, PlayerEnt->ent_anim.Pos, &R, &G, &B);
		gl_DrawHoldedEntity(WeaponHoldedEnt,PlayerEnt, R, G, B);
	}	
}


void o_ClearAnimFields(ANIMENTITY *anim)
{
	memset(anim,0,sizeof(ANIMENTITY));
	anim->oscille_start_Y = -1;
	anim->Siz.X = anim->Siz.Y = anim->Siz.Z = 1.0f;
}

hBool o_AddEntityToList(int id, int parent_id, ANIMENTITY anim, hBool PlayerHolded)
{
	pEntity	pNewEntity = NULL;
	int		diff;

	diff = (int)difficulty.value;

	if(id == DEPLA || id == MORBAK || id == ZEBAT || id == SUBZERAT || id == ZEMUMMY)
	{
		if(gNumberOfMonsters >= 32)
			return false;
		if(!monster.value)
			return true;
		gNumberOfMonsters++;
	}

	pNewEntity = (pEntity)s_malloc(sizeof(ENTITY));
	memset(pNewEntity,0,sizeof(ENTITY));

	pNewEntity->IsBounce = true;
	pNewEntity->IsGravity = true;
	pNewEntity->SolidSpace = false;
	pNewEntity->Timing=false;
	pNewEntity->Time_msec=0;
	pNewEntity->parent_leafid = -1;
	pNewEntity->player_holded = PlayerHolded;
	pNewEntity->ent_bbox = gEntitiesLoadedList[id].ent_bbox;
	pNewEntity->Radius = gEntitiesLoadedList[id].Radius;
	pNewEntity->entity_id = id;
	pNewEntity->parent_id = parent_id;
	pNewEntity->FaceList = gEntitiesLoadedList[id].FaceList;
	pNewEntity->ent_anim = anim;
	pNewEntity->ent_anim.net_OldPos = pNewEntity->ent_anim.Pos;	//
	pNewEntity->Boundaries = gEntitiesLoadedList[id].Boundaries;
	pNewEntity->snd_channel = -1;
	pNewEntity->Animation_ID = MSK_THINK;
	pNewEntity->IsChanged_Animation = true;
	pNewEntity->BlendVal = 0;

	if(gIsMultiplayer)
	{
		pNewEntity->health = 100;

		if(gIsServer)
		{
			if(pNewEntity->entity_id == EXPLOSION)
			{
				//net_NetLaunchEnt(pNewEntity->entity_id, parent_id, anim.Pos, anim.Rot, anim.Siz_Vel, true);
			}
		}
	}

	if(id==DEPLA)
	{
		pNewEntity->parent_id = msk_cloneAnimModelData(&gModel_depla);
		pNewEntity->Monster_Life = MonsterLife[0][diff];
	}
	else if(id==MORBAK)
	{
		pNewEntity->parent_id = msk_cloneAnimModelData(&gModel_morbak);
		pNewEntity->Monster_Life = MonsterLife[1][diff];
	}
	else if(id==ZEBAT)
	{
		pNewEntity->parent_id = msk_cloneAnimModelData(&gModel_zebat);
		pNewEntity->Monster_Life = MonsterLife[2][diff];
	}
	else if(id==SUBZERAT)
	{
		pNewEntity->parent_id = msk_cloneAnimModelData(&gModel_subzerat);
		pNewEntity->Monster_Life = MonsterLife[3][diff];
	}
	else if(id==ZEMUMMY)
	{
		pNewEntity->parent_id = msk_cloneAnimModelData(&gModel_zemummy);
		pNewEntity->Monster_Life = MonsterLife[4][diff];
	}
	else if(id==PERSO)
	{
		pNewEntity->saveradiusY = pNewEntity->Radius.Y;
	}

	pNewEntity->Next = gEntitiesList;
	gEntitiesList = pNewEntity;

	return true;
}

int o_FindEntityClonePos(pEntity Ent)
{
	pEntity	pCurrEntity;
	int		number;

	if(!Ent)
		return 0;

	number = 0;
	for(pCurrEntity=gEntitiesList ; pCurrEntity ; pCurrEntity=pCurrEntity->Next)
	{
		if(pCurrEntity == Ent)
		{
			return (number + 1);
		}
		if(pCurrEntity->entity_id == Ent->entity_id)
		{
			number++;
		}
	}
	return number;
}

int o_FindEntityClone(int entid)
{
	pEntity	pCurrEntity;
	int		number;

	number = 0;
	for(pCurrEntity=gEntitiesList ; pCurrEntity ; pCurrEntity=pCurrEntity->Next)
	if(pCurrEntity->entity_id == entid)
	{
		number++;
	}
	return number;
}

pEntity o_FindEntityFromID(float id)
{
	pEntity	pTemp = NULL;

	if(!id)
		return NULL;

	if(gEntitiesList->ent_anim.id_list == id && gEntitiesList->player_holded == false)
	{
		return gEntitiesList;
	}
	else if(gEntitiesList->Next)
	{
		pTemp = gEntitiesList->Next;
		while(pTemp->ent_anim.id_list != id || pTemp->player_holded == true)
		{
			pTemp = pTemp->Next;
			if(pTemp==NULL)
				return NULL;
		}
		return pTemp;
	}
	return NULL;
}

void o_RemoveEntityFromList_Pos(vect_t pos)
{
	pEntity	pTemp = NULL;
	pEntity	pTemp2 = NULL;

	if( (gEntitiesList->ent_anim.Pos.X == pos.X) &&
		(gEntitiesList->ent_anim.Pos.Y == pos.Y) &&
		(gEntitiesList->ent_anim.Pos.Z == pos.Z) )
	{
		pTemp = gEntitiesList->Next;
		o_FreeEntity(gEntitiesList);
		gEntitiesList = pTemp;
	}
	else if(gEntitiesList->Next)
	{
		pTemp2 = gEntitiesList;
		pTemp = gEntitiesList->Next;
		while( 
			(pTemp->ent_anim.Pos.X != pos.X) ||
			(pTemp->ent_anim.Pos.Y != pos.Y) ||
			(pTemp->ent_anim.Pos.Z != pos.Z) )
		{
			pTemp2 = pTemp2->Next;
			pTemp = pTemp->Next;
			if(pTemp==NULL)
				return;
		}
		pTemp2->Next = pTemp->Next;
		o_FreeEntity(pTemp);
		pTemp=NULL;
	}
}

void o_RemoveEntityFromList_ID(float id)
{
	pEntity	pTemp = NULL;
	pEntity	pTemp2 = NULL;

	if(!id)
		return;

	if(gEntitiesList->ent_anim.id_list == id)
	{
		pTemp = gEntitiesList->Next;
		o_FreeEntity(gEntitiesList);
		gEntitiesList = pTemp;
	}
	else if(gEntitiesList->Next)
	{
		pTemp2 = gEntitiesList;
		pTemp = gEntitiesList->Next;
		while(pTemp->ent_anim.id_list != id)
		{
			pTemp2 = pTemp2->Next;
			pTemp = pTemp->Next;
			if(pTemp==NULL)
				return;
		}
		pTemp2->Next = pTemp->Next;
		o_FreeEntity(pTemp);
		pTemp=NULL;
	}
}

void o_RemoveEntityFromList(pEntity remove)
{
	pEntity	pTemp = NULL;

	if(!remove)
		return;

	if(gIsMultiplayer)
	{
		if(gIsServer)
		{
			if(remove->entity_id == BOMBE || remove->entity_id == FLECHE)
			{
				//net_RemoveClientEnt(remove);
			}
		}
	}

	if(gEntitiesList == remove)
	{
		pTemp = gEntitiesList->Next;
		o_FreeEntity(remove);
		remove=NULL;
		gEntitiesList = pTemp;
	}
	else if(gEntitiesList->Next)
	{
		pTemp = gEntitiesList;
		while(pTemp->Next != remove)
			pTemp = pTemp->Next;

		pTemp->Next = remove->Next;
		o_FreeEntity(remove);
		remove=NULL;
	}
}

ANIMENTITY * o_FindEntityAnimField(int id)
{
	pEntity	pCurrEntity;

	pCurrEntity = o_FindEntity(id);
	if(pCurrEntity==NULL)
		return NULL;

	return &(pCurrEntity->ent_anim);
}

pEntity o_FindHoldedEntity(int id)
{
	pEntity	pCurrEntity;

	pCurrEntity = gEntitiesList;
	while(pCurrEntity)
	{
		if( (pCurrEntity->entity_id == id) && (pCurrEntity->player_holded) )
			return pCurrEntity;
		pCurrEntity = pCurrEntity->Next;
	}
	return NULL;
}

pEntity o_FindHoldedEntity2(int id, int number)
{
	pEntity	pCurrEntity;

	for(pCurrEntity=gEntitiesList ; pCurrEntity ; pCurrEntity=pCurrEntity->Next)
	{
		if(pCurrEntity->entity_id == id && pCurrEntity->player_holded)
		{
			number--;
			if(number <= 0)
				return pCurrEntity;
			continue;
		}
	}
	return NULL;
}

pEntity o_FindNotHoldedEntity(int id)
{
	pEntity	pCurrEntity;

	pCurrEntity = gEntitiesList;
	while(pCurrEntity)
	{
		if( (pCurrEntity->entity_id == id) && (!pCurrEntity->player_holded) )
			return pCurrEntity;
		pCurrEntity = pCurrEntity->Next;
	}
	return NULL;
}

pEntity o_FindEntity(int id)
{
	pEntity	pCurrEntity;

	for(pCurrEntity=gEntitiesList ; pCurrEntity ; pCurrEntity=pCurrEntity->Next)
	if(pCurrEntity->entity_id == id)
		return pCurrEntity;
	return NULL;
}

pEntity o_FindEntity2(int id, int number)
{
	pEntity	pCurrEntity;

	for(pCurrEntity=gEntitiesList ; pCurrEntity ; pCurrEntity=pCurrEntity->Next)
	if(pCurrEntity->entity_id == id)
	{
		number--;
		if(number <= 0)
			return pCurrEntity;
		continue;
	}
	return NULL;
}

int	o_FindRespawnTabIndex(pEntity Ent)
{
	int		i;

	if(!Ent)
		return -1;

	for(i=0 ; i<MAX_RESPAWN_OBJ ; i++)
	{
		if(RespawnTab[i].list_id == Ent->ent_anim.id_list)
			return i;
	}
	return -1;
}

hBool o_LoadEntityToStruct(char *path, int id)
{
	FILE	*file;
	Face_t	*Poly;
	Face_t	*Poly2;
	int		i, j, k;
	char	fullpath[255];
	char	temp[255];
	Face_t	*pList=NULL;
	float	no_use;
	int		texture_id_plan[TEXTURELOADED];
	char	tmp[255];
	int		nbnorm;
	vect_t	vn;

	sprintf(fullpath,"%s%s%s",SYSDIR,ENTDIR,path);
	
#ifdef H_MAC
	file = fopen(KMiscTools::makeFilePath(fullpath),"r");
#else
	file = fopen(fullpath,"r");
#endif
	
	if(file==NULL) {
		return false;
	}

	m_ConsPrint("Loading: %s\n",fullpath);
	
//	menu_DispLoadMapSplash();
//	g_DispCenterString(14, 3, "Loading.. %s", fullpath);
//	g_DispCenterString(12, 3, "Loading..");
//	gl_SwapBuffer();

	fscanf(file,"<ENTITY>\n");

	//	Read Textures infos
	fscanf(file,"<TEXTURES> %d\n",&j);
	for(k=0 ; k<j ; k++)
	{
		fscanf(file,"%s\n",temp); // &temp

		fs_GetFileNameFromPath(temp,tmp);
		if(!strncmp(tmp,FLAMETAG,5))
		{
			texture_id_plan[k] = txid_FLAME[0];
		}
		else
		{
			texture_id_plan[k] = im_Load(temp,-1, (int)picmip.value, false);
		}

		if(texture_id_plan[k]<0)
			texture_id_plan[k]=0;
	}

	while(!feof(file))
	{
		Poly = (Face_t*)s_malloc(sizeof(Face_t));

		fscanf(file,"%d %d\n",&Poly->NumberOfVertices,&Poly->TextureID);
		Poly->TextureID = texture_id_plan[Poly->TextureID];
		if(Poly->TextureID == txid_FLAME[0])
			Poly->Type = FLAMES;
		else
			Poly->Type = -2; //id;

		if(Poly->NumberOfVertices > 8)
		{
			m_ConsPrint("Inavlide vertex with %d points\n",Poly->NumberOfVertices);
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
					&no_use,
					&no_use);
		}
                
                #ifdef H_MAC
              //  printf("## tex:%d nver:%d ", Poly->TextureID, Poly->NumberOfVertices);
              //  printf(" %f %f\n", Poly->Vertice[0].U, Poly->Vertice[0].V);
                #endif

		Poly->Plan.VecteurNormal = GetFaceNormal(Poly->Vertice[0],Poly->Vertice[1],Poly->Vertice[2]);		
		Poly->Plan.PointOnPlane.X = Poly->Vertice[0].X;
		Poly->Plan.PointOnPlane.Y = Poly->Vertice[0].Y;
		Poly->Plan.PointOnPlane.Z = Poly->Vertice[0].Z;

		Poly->Next = pList;
		pList = Poly;
	}
	fclose(file);

	// calcul la normal de chaque vertex pour l'environnemet mapping
	if(id >= H && id <= N)
	{
		for(Poly=pList ; Poly ; Poly=Poly->Next)
		{
			for(i=0 ; i<Poly->NumberOfVertices ; i++)
			{
				vn = Poly->Plan.VecteurNormal;
				nbnorm = 1;

				for(Poly2=pList ; Poly2 ; Poly2=Poly2->Next)
				{
					if(Poly2 == Poly)
						continue;
					if(Poly->Vertice[i].X == Poly2->Vertice[j].X)
					if(Poly->Vertice[i].Y == Poly2->Vertice[j].Y)
					if(Poly->Vertice[i].Z == Poly2->Vertice[j].Z)
					{
						vn.X += Poly2->Plan.VecteurNormal.X;
						vn.Y += Poly2->Plan.VecteurNormal.Y;
						vn.Z += Poly2->Plan.VecteurNormal.Z;

						nbnorm++;
					}			
				}

				vn.X /= nbnorm;
				vn.Y /= nbnorm;
				vn.Z /= nbnorm;

				Poly->Vertice[i].NX = vn.X;
				Poly->Vertice[i].NY = vn.Y;
				Poly->Vertice[i].NZ = vn.Z;
			}
		}
	}
	o_CenterAndGetBound(id,pList);
	gNumberOfLoadedEntities++;
	return true;
}

void o_Center(pFace List)
{
	BOUNDINGBOX bbox;
	vect_t		center;
	pFace		Poly;
	int			i;

	ClearBound(&bbox);
	CalculateBox(&bbox,List);
	center.X = (bbox.BoxMin.X + bbox.BoxMax.X)/2.0f;
	center.Y = (bbox.BoxMin.Y + bbox.BoxMax.Y)/2.0f;
	center.Z = (bbox.BoxMin.Z + bbox.BoxMax.Z)/2.0f;
	Poly = List;
	while(Poly)
	{
		for(i=0 ; i<Poly->NumberOfVertices ; i++)
		{
			Poly->Vertice[i].X -= center.X;
			Poly->Vertice[i].Y -= center.Y;
			Poly->Vertice[i].Z -= center.Z;
		}
		Poly = Poly->Next;
	}
}

void o_UpdateBound(BOUNDINGBOX bbox, int ID, pFace List)
{
	pFace		pList;
	pFace		pBigList;

	if(bbox.BoxMin.X < bbox.BoxMin.Z)
		bbox.BoxMin.Z = bbox.BoxMin.X;
	else
		bbox.BoxMin.X = bbox.BoxMin.Z;
	if(bbox.BoxMax.X > bbox.BoxMax.Z)
		bbox.BoxMax.Z = bbox.BoxMax.X;
	else
		bbox.BoxMax.X = bbox.BoxMax.Z;

	gEntitiesLoadedList[ID].entity_id = ID;
	gEntitiesLoadedList[ID].Radius = bbox.BoxMax;
	gEntitiesLoadedList[ID].FaceList = List;
	gEntitiesLoadedList[ID].ent_bbox = bbox;
	gEntitiesLoadedList[ID].Next = NULL;

	if(ID == PERSO)
	{
		if(gEntitiesLoadedList[ID].Radius.X < gEntitiesLoadedList[ID].Radius.Z)
			gEntitiesLoadedList[ID].Radius.Z = gEntitiesLoadedList[ID].Radius.X;
		else
			gEntitiesLoadedList[ID].Radius.X = gEntitiesLoadedList[ID].Radius.Z;
	}
	else if(ID == MORBAK && gIsMultiplayer)
	{
		gEntitiesLoadedList[ID].Radius.X *= 0.8f;
		gEntitiesLoadedList[ID].Radius.Z *= 0.8f;
		gEntitiesLoadedList[ID].Radius.Y *= 1.2f;
	}

	pBigList=NULL;

	pList = (Face_t*)s_malloc(sizeof(Face_t));
	memset(pList,0,sizeof(Face_t));
	pList->NumberOfVertices = 4;
	pList->Vertice[0].X = bbox.BoxMin.X;
	pList->Vertice[0].Y = bbox.BoxMin.Y;
	pList->Vertice[0].Z = bbox.BoxMin.Z;
	pList->Vertice[1].X = bbox.BoxMin.X;
	pList->Vertice[1].Y = bbox.BoxMin.Y;
	pList->Vertice[1].Z = bbox.BoxMax.Z;
	pList->Vertice[2].X = bbox.BoxMin.X;
	pList->Vertice[2].Y = bbox.BoxMax.Y;
	pList->Vertice[2].Z = bbox.BoxMax.Z;
	pList->Vertice[3].X = bbox.BoxMin.X;
	pList->Vertice[3].Y = bbox.BoxMax.Y;
	pList->Vertice[3].Z = bbox.BoxMin.Z;
	pList->Plan.PointOnPlane.X = pList->Vertice[0].X;
	pList->Plan.PointOnPlane.Y = pList->Vertice[0].Y;
	pList->Plan.PointOnPlane.Z = pList->Vertice[0].Z;
	pList->Plan.VecteurNormal = GetFaceNormal(pList->Vertice[0],pList->Vertice[1],pList->Vertice[2]);
	pList->Next = pBigList;
	pBigList = pList;

	pList = (Face_t*)s_malloc(sizeof(Face_t));
	memset(pList,0,sizeof(Face_t));
	pList->NumberOfVertices = 4;
	pList->Vertice[0].X = bbox.BoxMax.X;
	pList->Vertice[0].Y = bbox.BoxMin.Y;
	pList->Vertice[0].Z = bbox.BoxMin.Z;
	pList->Vertice[1].X = bbox.BoxMax.X;
	pList->Vertice[1].Y = bbox.BoxMin.Y;
	pList->Vertice[1].Z = bbox.BoxMax.Z;
	pList->Vertice[2].X = bbox.BoxMax.X;
	pList->Vertice[2].Y = bbox.BoxMax.Y;
	pList->Vertice[2].Z = bbox.BoxMax.Z;
	pList->Vertice[3].X = bbox.BoxMax.X;
	pList->Vertice[3].Y = bbox.BoxMax.Y;
	pList->Vertice[3].Z = bbox.BoxMin.Z;
	pList->Plan.PointOnPlane.X = pList->Vertice[0].X;
	pList->Plan.PointOnPlane.Y = pList->Vertice[0].Y;
	pList->Plan.PointOnPlane.Z = pList->Vertice[0].Z;
	pList->Plan.VecteurNormal = GetFaceNormal(pList->Vertice[0],pList->Vertice[1],pList->Vertice[2]);
	pList->Next = pBigList;
	pBigList = pList;

	pList = (Face_t*)s_malloc(sizeof(Face_t));
	memset(pList,0,sizeof(Face_t));
	pList->NumberOfVertices = 4;
	pList->Vertice[0].Y = bbox.BoxMin.Y;
	pList->Vertice[0].Z = bbox.BoxMin.Z;
	pList->Vertice[0].X = bbox.BoxMin.X;
	pList->Vertice[1].Y = bbox.BoxMin.Y;
	pList->Vertice[1].Z = bbox.BoxMin.Z;
	pList->Vertice[1].X = bbox.BoxMax.X;
	pList->Vertice[2].Y = bbox.BoxMin.Y;
	pList->Vertice[2].Z = bbox.BoxMax.Z;
	pList->Vertice[2].X = bbox.BoxMax.X;
	pList->Vertice[3].Y = bbox.BoxMin.Y;
	pList->Vertice[3].Z = bbox.BoxMax.Z;
	pList->Vertice[3].X = bbox.BoxMin.X;
	pList->Plan.PointOnPlane.X = pList->Vertice[0].X;
	pList->Plan.PointOnPlane.Y = pList->Vertice[0].Y;
	pList->Plan.PointOnPlane.Z = pList->Vertice[0].Z;
	pList->Plan.VecteurNormal = GetFaceNormal(pList->Vertice[0],pList->Vertice[1],pList->Vertice[2]);
	pList->Next = pBigList;
	pBigList = pList;

	pList = (Face_t*)s_malloc(sizeof(Face_t));
	memset(pList,0,sizeof(Face_t));
	pList->NumberOfVertices = 4;
	pList->Vertice[0].Y = bbox.BoxMax.Y;
	pList->Vertice[0].Z = bbox.BoxMin.Z;
	pList->Vertice[0].X = bbox.BoxMin.X;
	pList->Vertice[1].Y = bbox.BoxMax.Y;
	pList->Vertice[1].Z = bbox.BoxMin.Z;
	pList->Vertice[1].X = bbox.BoxMax.X;
	pList->Vertice[2].Y = bbox.BoxMax.Y;
	pList->Vertice[2].Z = bbox.BoxMax.Z;
	pList->Vertice[2].X = bbox.BoxMax.X;
	pList->Vertice[3].Y = bbox.BoxMax.Y;
	pList->Vertice[3].Z = bbox.BoxMax.Z;
	pList->Vertice[3].X = bbox.BoxMin.X;
	pList->Plan.PointOnPlane.X = pList->Vertice[0].X;
	pList->Plan.PointOnPlane.Y = pList->Vertice[0].Y;
	pList->Plan.PointOnPlane.Z = pList->Vertice[0].Z;
	pList->Plan.VecteurNormal = GetFaceNormal(pList->Vertice[0],pList->Vertice[1],pList->Vertice[2]);
	pList->Next = pBigList;
	pBigList = pList;

	pList = (Face_t*)s_malloc(sizeof(Face_t));
	memset(pList,0,sizeof(Face_t));
	pList->NumberOfVertices = 4;
	pList->Vertice[0].Z = bbox.BoxMin.Z;
	pList->Vertice[0].X = bbox.BoxMin.X;
	pList->Vertice[0].Y = bbox.BoxMin.Y;
	pList->Vertice[1].Z = bbox.BoxMin.Z;
	pList->Vertice[1].X = bbox.BoxMin.X;
	pList->Vertice[1].Y = bbox.BoxMax.Y;
	pList->Vertice[2].Z = bbox.BoxMin.Z;
	pList->Vertice[2].X = bbox.BoxMax.X;
	pList->Vertice[2].Y = bbox.BoxMax.Y;
	pList->Vertice[3].Z = bbox.BoxMin.Z;
	pList->Vertice[3].X = bbox.BoxMax.X;
	pList->Vertice[3].Y = bbox.BoxMin.Y;
	pList->Plan.PointOnPlane.X = pList->Vertice[0].X;
	pList->Plan.PointOnPlane.Y = pList->Vertice[0].Y;
	pList->Plan.PointOnPlane.Z = pList->Vertice[0].Z;
	pList->Plan.VecteurNormal = GetFaceNormal(pList->Vertice[0],pList->Vertice[1],pList->Vertice[2]);
	pList->Next = pBigList;
	pBigList = pList;

	pList = (Face_t*)s_malloc(sizeof(Face_t));
	memset(pList,0,sizeof(Face_t));
	pList->NumberOfVertices = 4;
	pList->Vertice[0].Z = bbox.BoxMax.Z;
	pList->Vertice[0].X = bbox.BoxMin.X;
	pList->Vertice[0].Y = bbox.BoxMin.Y;
	pList->Vertice[1].Z = bbox.BoxMax.Z;
	pList->Vertice[1].X = bbox.BoxMin.X;
	pList->Vertice[1].Y = bbox.BoxMax.Y;
	pList->Vertice[2].Z = bbox.BoxMax.Z;
	pList->Vertice[2].X = bbox.BoxMax.X;
	pList->Vertice[2].Y = bbox.BoxMax.Y;
	pList->Vertice[3].Z = bbox.BoxMax.Z;
	pList->Vertice[3].X = bbox.BoxMax.X;
	pList->Vertice[3].Y = bbox.BoxMin.Y;
	pList->Plan.PointOnPlane.X = pList->Vertice[0].X;
	pList->Plan.PointOnPlane.Y = pList->Vertice[0].Y;
	pList->Plan.PointOnPlane.Z = pList->Vertice[0].Z;
	pList->Plan.VecteurNormal = GetFaceNormal(pList->Vertice[0],pList->Vertice[1],pList->Vertice[2]);
	pList->Next = pBigList;
	pBigList = pList;

	gEntitiesLoadedList[ID].Boundaries = pBigList;
}

void o_CenterAndGetBound(int ID, pFace List)
{
	BOUNDINGBOX bbox;

	o_Center(List);
	ClearBound(&bbox);
	CalculateBox(&bbox,List);

	if(ID == DEPLA || ID == MORBAK || ID == ZEMUMMY /*|| ID == ZEBAT*/)
	{
		bbox.BoxMin.X *= 0.8f;
		bbox.BoxMin.Z *= 0.8f;
		bbox.BoxMax.X *= 0.8f;
		bbox.BoxMax.Z *= 0.8f;
	}

	o_UpdateBound(bbox,ID,List);
}

void o_RespawnProcess()
{
	int i;
	ANIMENTITY	anim;
	int		diff;
	int		leaf_ent;

	if(ProgramState != PS_GAME)
		return;
	if(!respawn.value)
		return;

	diff = (int)difficulty.value;

	for(i=0 ; i<gNb_Respawn ; i++)
	if(RespawnTab[i].pickedup)
	{
		RespawnTab[i].Timer -= gFrameTime_msec;
		if(RespawnTab[i].Timer >= 0)
			continue;

		RespawnTab[i].Timer = 0;
		RespawnTab[i].pickedup = false;

		switch(RespawnTab[i].entity_id)
		{
			case GUN1:
			o_ClearAnimFields(&anim);
			anim.id_list = RespawnTab[i].list_id;
			anim.Pos = RespawnTab[i].entity_pos;
			anim.Rot_Vel.Y = 0.15f;
			o_AddEntityToList(GUN1,-1,anim,false);
			break;

			case GUN2:
			o_ClearAnimFields(&anim);
			anim.id_list = RespawnTab[i].list_id;
			anim.Pos = RespawnTab[i].entity_pos;
			anim.Rot_Vel.Y = 0.15f;
			o_AddEntityToList(GUN2,-1,anim,false);
			break;

			case GUN3:
			o_ClearAnimFields(&anim);
			anim.id_list = RespawnTab[i].list_id;
			anim.Pos = RespawnTab[i].entity_pos;
			anim.Rot_Vel.Y = 0.15f;
			o_AddEntityToList(GUN3,-1,anim,false);
			break;

			case AMO1:
			o_ClearAnimFields(&anim);
			anim.id_list = RespawnTab[i].list_id;
			anim.Pos = RespawnTab[i].entity_pos;
			o_AddEntityToList(AMO1,-1,anim,false);
			break;

			case AMO2:
			o_ClearAnimFields(&anim);
			anim.id_list = RespawnTab[i].list_id;
			anim.Pos = RespawnTab[i].entity_pos;
			o_AddEntityToList(AMO2,-1,anim,false);
			break;

			case AMO3:
			o_ClearAnimFields(&anim);
			anim.id_list = RespawnTab[i].list_id;
			anim.Pos = RespawnTab[i].entity_pos;
			o_AddEntityToList(AMO3,-1,anim,false);
			break;

			case PUMPKIN:
			o_ClearAnimFields(&anim);
			anim.id_list = RespawnTab[i].list_id;
			anim.Pos = RespawnTab[i].entity_pos;
			//anim.Rot.Y = (float)RespawnTab[i].RotY;
			anim.Rot_Vel.Y = -0.15f;
			o_AddEntityToList(PUMPKIN,-1,anim,false);
			break;

			case FIOLE1:
			o_ClearAnimFields(&anim);
			anim.id_list = RespawnTab[i].list_id;
			anim.Pos = RespawnTab[i].entity_pos;
			anim.Rot_Vel.Y = 0.15f;
			o_AddEntityToList(FIOLE1, -1, anim, false);
			break;

			case FIOLE2:
			o_ClearAnimFields(&anim);
			anim.id_list = RespawnTab[i].list_id;
			anim.Pos = RespawnTab[i].entity_pos;
			anim.Rot_Vel.Y = 0.15f;
			o_AddEntityToList(FIOLE2,-1,anim,false);
			break;

			case FIOLE3:
			o_ClearAnimFields(&anim);
			anim.id_list = RespawnTab[i].list_id;
			anim.Pos = RespawnTab[i].entity_pos;
			anim.Rot_Vel.Y = 0.15f;
			o_AddEntityToList(FIOLE3,-1,anim,false);
			break;

			case H:
			o_ClearAnimFields(&anim);
			anim.id_list = RespawnTab[i].list_id;
			anim.Pos = RespawnTab[i].entity_pos;
			anim.Rot_Vel.Y = 0.15f;
			o_AddEntityToList(H,-1,anim,false);
			break;

			case A:
			o_ClearAnimFields(&anim);
			anim.id_list = RespawnTab[i].list_id;
			anim.Pos = RespawnTab[i].entity_pos;
			anim.Rot_Vel.Y = 0.15f;
			o_AddEntityToList(A,-1,anim,false);
			break;

			case L:
			o_ClearAnimFields(&anim);
			anim.id_list = RespawnTab[i].list_id;
			anim.Pos = RespawnTab[i].entity_pos;
			anim.Rot_Vel.Y = 0.15f;
			o_AddEntityToList(L,-1,anim,false);
			break;

			case O:
			o_ClearAnimFields(&anim);
			anim.id_list = RespawnTab[i].list_id;
			anim.Pos = RespawnTab[i].entity_pos;
			anim.Rot_Vel.Y = 0.15f;
			o_AddEntityToList(O,-1,anim,false);
			break;

			case W:
			o_ClearAnimFields(&anim);
			anim.id_list = RespawnTab[i].list_id;
			anim.Pos = RespawnTab[i].entity_pos;
			anim.Rot_Vel.Y = 0.15f;
			o_AddEntityToList(W,-1,anim,false);
			break;

			case E:
			o_ClearAnimFields(&anim);
			anim.id_list = RespawnTab[i].list_id;
			anim.Pos = RespawnTab[i].entity_pos;
			anim.Rot_Vel.Y = 0.15f;
			o_AddEntityToList(E,-1,anim,false);
			break;

			case N:
			o_ClearAnimFields(&anim);
			anim.id_list = RespawnTab[i].list_id;
			anim.Pos = RespawnTab[i].entity_pos;
			anim.Rot_Vel.Y = 0.15f;
			o_AddEntityToList(N,-1,anim,false);
			break;

			case DEPLA:
			if(monster.value && (gKilledDeplas < MapGoals[1][diff][game_MapID] || !game_TimeLimit))
			{
				RespawnTab[i].Timer = 5000.0f + (float)randInt(0,2000);
				o_ClearAnimFields(&anim);
				anim.Pos = RespawnTab[i].entity_pos;

				leaf_ent = bsp_FindCamera(anim.Pos);
				if(leaf_ent == -1)
					break;
				if(PVSData[LeafArray[gLeafLocation].PVSIndex+leaf_ent] == '1')
				{
					RespawnTab[i].pickedup = true;
					RespawnTab[i].Timer = 3000.0f;
					break;
				}

				anim.id_list = RespawnTab[i].list_id;
				if(!o_AddEntityToList(DEPLA,-1,anim,false))
				{
					RespawnTab[i].pickedup = true;
					RespawnTab[i].Timer = 0;
				}
			}
			break;

			case MORBAK:
			if(monster.value && (gKilledMorbaks < MapGoals[2][diff][game_MapID] || !game_TimeLimit))
			{
				RespawnTab[i].Timer = 5000.0f + (float)randInt(0,2000);
				o_ClearAnimFields(&anim);
				anim.Pos = RespawnTab[i].entity_pos;

				leaf_ent = bsp_FindCamera(anim.Pos);
				if(leaf_ent == -1)
					break;
				if(PVSData[LeafArray[gLeafLocation].PVSIndex+leaf_ent] == '1')
				{
					RespawnTab[i].pickedup = true;
					RespawnTab[i].Timer = 3000.0f;
					break;
				}

				anim.id_list = RespawnTab[i].list_id;
				if(!o_AddEntityToList(MORBAK,-1,anim,false))
				{
					RespawnTab[i].pickedup = true;
					RespawnTab[i].Timer = 0;
				}
			}
			break;

			case ZEBAT:
			if(monster.value && (gKilledZeubat < MapGoals[3][diff][game_MapID] || !game_TimeLimit))
			{
				RespawnTab[i].Timer = 5000.0f + (float)randInt(0,2000);
				o_ClearAnimFields(&anim);
				anim.Pos = RespawnTab[i].entity_pos;

#if 0
				leaf_ent = bsp_FindCamera(anim.Pos);
				if(leaf_ent == -1)
					break;
				if(PVSData[LeafArray[gLeafLocation].PVSIndex+leaf_ent] == '1')
				{
					RespawnTab[i].pickedup = true;
					RespawnTab[i].Timer = 3000.0f;
					break;
				}
#endif

				anim.id_list = RespawnTab[i].list_id;
				if(!o_AddEntityToList(ZEBAT,-1,anim,false))
				{
					RespawnTab[i].pickedup = true;
					RespawnTab[i].Timer = 0;
				}
			}
			break;

			case SUBZERAT:
			if(monster.value && (gKilledSubzerat < MapGoals[4][diff][game_MapID] || !game_TimeLimit))
			{
				RespawnTab[i].Timer = 5000.0f + (float)randInt(0,2000);
				o_ClearAnimFields(&anim);
				anim.Pos = RespawnTab[i].entity_pos;

				leaf_ent = bsp_FindCamera(anim.Pos);
				if(leaf_ent == -1)
					break;
				if(PVSData[LeafArray[gLeafLocation].PVSIndex+leaf_ent] == '1')
				{
					RespawnTab[i].pickedup = true;
					RespawnTab[i].Timer = 3000.0f;
					break;
				}

				anim.id_list = RespawnTab[i].list_id;
				if(!o_AddEntityToList(SUBZERAT,-1,anim,false))
				{
					RespawnTab[i].pickedup = true;
					RespawnTab[i].Timer = 0;
				}
			}
			break;

			case ZEMUMMY:
			if(monster.value && (gKilledZemummy < MapGoals[5][diff][game_MapID] || !game_TimeLimit))
			{
				RespawnTab[i].Timer = 5000.0f + (float)randInt(0,2000);
				o_ClearAnimFields(&anim);
				anim.Pos = RespawnTab[i].entity_pos;

				leaf_ent = bsp_FindCamera(anim.Pos);
				if(leaf_ent == -1)
					break;
				if(PVSData[LeafArray[gLeafLocation].PVSIndex+leaf_ent] == '1')
				{
					RespawnTab[i].pickedup = true;
					RespawnTab[i].Timer = 3000.0f;
					break;
				}

				anim.id_list = RespawnTab[i].list_id;
				if(!o_AddEntityToList(ZEMUMMY,-1,anim,false))
				{
					RespawnTab[i].pickedup = true;
					RespawnTab[i].Timer = 0;
				}
			}
			break;
		}
	}
}

float o_GetMaxRadius(vect_t radius)
{
	return m_MaxValuef(m_MaxValuef(radius.X,radius.Y),radius.Z);
}

float o_GetViewCamMaxRadius()
{
	pEntity		PlayerEnt;
	if(!player_view.value)
		return 10.0f;
	
	PlayerEnt = o_FindEntity(PERSO);
	if(!PlayerEnt)
		return 0;
	return o_GetMaxRadius(PlayerEnt->Radius);
}

void o_GetLightningIntensity(int obj_id, vect_t ObjPos, float *R, float *G, float *B)
{
	float	coef;
	float	bestcoef;
	float	dist;
	pEntity pNearestLight;
	pEntity pEntityList;
	
	if(lightmap.value == 0 || MapDataFormat != MAP_BSP)
	{
		*R = 1;
		*G = 1;
		*B = 1;
		return;
	}

	if( obj_id == FIOLE1	||
		obj_id == FIOLE2	||
		obj_id == FIOLE3	||
		(obj_id >= H && obj_id <= N)	||
		obj_id == EXPLOSION	||
		obj_id == SMOKE	||
		obj_id == BLOOD	||
		obj_id == TOILE
		)
	{
		*R = 1;
		*G = 1;
		*B = 1;
		return;
	}

	// HACK : FIXME
	if(gIsMultiplayer)
	{
		*R = 1;
		*G = 1;
		*B = 1;
		return;
	}

	bestcoef = 0;
	pNearestLight = NULL;

	for(pEntityList=gEntitiesList ; pEntityList ; pEntityList=pEntityList->Next)
	{
		if(pEntityList->entity_id != LAMPE && pEntityList->entity_id != MLAMPE)
			continue;
		dist = m_Distance(pEntityList->ent_anim.Pos, ObjPos);
		if(dist > 800) //400
			continue;
		dist *= 0.1f; // 0.2
		coef = 10.0f / ( dist + 1.0f );

		if(coef > bestcoef)
		{
			bestcoef = coef;
			pNearestLight = pEntityList;
		}
	}

	if(bestcoef > 1.0f)
		bestcoef = 1.0f;
	if(bestcoef < 0.01)
		bestcoef = 0.01f;

	if(pNearestLight)
	{
		*R = bestcoef * pNearestLight->R / 255.0f;
		*G = bestcoef * pNearestLight->G / 255.0f;
		*B = bestcoef * pNearestLight->B / 255.0f;

		// hack pour moins colorer les objets
		*R = ((*R)*2 + 1)/3.0f;
		*G = ((*G)*2 + 1)/3.0f;
		*B = ((*B)*2 + 1)/3.0f;
	}
	else
	{
		*R = bestcoef;
		*G = bestcoef;
		*B = bestcoef;
	}
}

void o_OscilleYprocess(pEntity Ent)
{
	int		offset;

	if(!Ent)
		return;
	if(Ent->ent_anim.oscille_start_Y == -1)
	{
		Ent->ent_anim.oscille_start_Y = (float)disp_GetClock();
	}

	offset = (int)((disp_GetClock()-(long)Ent->ent_anim.oscille_start_Y) * 0.15) % ROTMAX_I;
	Ent->ent_anim.oscille_Y += (gFrameTime_msec * 0.012f * Sin_i(offset));
}

