
#include "alloween.cpp"

int		gNumberOfMonsters=0;
int		gKilledDeplas=0;
int		gKilledMorbaks=0;
int		gKilledZeubat=0;
int		gKilledSubzerat=0;
int		gKilledZemummy=0;

enum
{
	MONSTER_FORWARD,
	MONSTER_TURNLEFT,
	MONSTER_TURNRIGHT,
	MONSTER_JUMP
};


pEntity monster_Die(pEntity MonsterEnt, Model *model, hBool OkGoreF)
{
	ANIMENTITY	anim;

	if(!MonsterEnt)
		return NULL;

	// HACK
	/*
	if(gIsMultiplayer && gIsServer && gConnectedPlayer > 1)
	{
		if(!gMultiPodium && MonsterEnt->entity_id == DEPLA)
		{
			gCL_score++;
		}
		else if(!gMultiPodium && MonsterEnt->entity_id == MORBAK)
		{
			gSV_score++;
		}
		//net_SendServerScores();
	}

	// HACK
	if(gIsMultiplayer && gIsServer && MonsterEnt->entity_id == MORBAK)
	{
		PlayerResetStartPos(MORBAK);
		MonsterEnt->health = 100;
		MonsterEnt->Monster_Life = 100;
		//MonsterEnt->IsChanged_Animation = true;
		return MonsterEnt;
	}
	*/

	if(gAllGoalCleared)
	{
		gScore++;
		game_clock += 5;
		if(game_clock > 999)
			game_clock = 999;
	}

	monster_UpdateCounter(MonsterEnt);
	fx_Particule(MonsterEnt->ent_anim.Pos,20,GLRED);	//
	msk_ChangeAnim(MonsterEnt, model,MSK_DIE);
	MonsterEnt->Timing = true;
	MonsterEnt->Time_msec = 8000.0f;
/*
	// ajoute des munitions
	o_ClearAnimFields(&anim);
	anim.Pos = MonsterEnt->ent_anim.Pos;
	anim.Pos_Vel.X = 0.01f * (float)randInt(-20,20);
	anim.Pos_Vel.Y = 0.25f;
	anim.Pos_Vel.Z = 0.01f * (float)randInt(-20,20);
	switch(rand()%3)
	{
		case 0:
		o_AddEntityToList(AMO1,-1,anim,true);
		break;

		case 1:
		o_AddEntityToList(AMO2,-1,anim,true);
		break;

		case 2:
		o_AddEntityToList(AMO3,-1,anim,true);
		break;
	}
*/
	// gore frag
	if(OkGoreF)
	if(MonsterEnt->Monster_Life < -10 || randInt(0,2)==0 )
	{
		switch(MonsterEnt->entity_id)
		{
			case DEPLA:
			ds_PlaySound3D(53,MonsterEnt->ent_anim.Pos,MonsterEnt->ent_anim.Pos);
			MonsterEnt->Time_msec = 0;

			o_ClearAnimFields(&anim);
			anim.Pos = MonsterEnt->ent_anim.Pos;
			anim.Pos.Y += 10.0f;
			anim.Pos_Vel.X = 0.01f * (float)randInt(-20,20);
			anim.Pos_Vel.Z = 0.01f * (float)randInt(-20,20);
			anim.Pos_Vel.Y = 0.25f;
			o_AddEntityToList(DEPLAHEAD,-1,anim,false);




			o_ClearAnimFields(&anim);
			anim.Pos = MonsterEnt->ent_anim.Pos;
			anim.Pos.Y += 5.0f;
			anim.Pos_Vel.X = 0.005f * (float)randInt(-20,20);
			anim.Pos_Vel.Z = 0.005f * (float)randInt(-20,20);
			anim.Pos_Vel.Y = 0.20f;
			o_AddEntityToList(DEPLAHEAD,-1,anim,false);

			o_ClearAnimFields(&anim);
			anim.Pos = MonsterEnt->ent_anim.Pos;
			anim.Pos.Y += 7.0f;
			anim.Pos_Vel.X = 0.002f * (float)randInt(-20,20);
			anim.Pos_Vel.Z = 0.002f * (float)randInt(-20,20);
			anim.Pos_Vel.Y = 0.15f;
			o_AddEntityToList(DEPLAHEAD,-1,anim,false);





			o_ClearAnimFields(&anim);
			anim.Pos = MonsterEnt->ent_anim.Pos;
			anim.Siz_Vel.X = 0.08f; //0.04f;
			anim.Siz_Vel.Y = 0.08f; //0.04f;
			anim.Siz_Vel.Z = 0.08f; //0.04f;
			o_AddEntityToList(BLOOD,-1,anim,false);

			break;

			case MORBAK:
			ds_PlaySound3D(53,MonsterEnt->ent_anim.Pos,MonsterEnt->ent_anim.Pos);
			MonsterEnt->Time_msec = 0;

			o_ClearAnimFields(&anim);
			anim.Pos = MonsterEnt->ent_anim.Pos;
			anim.Pos_Vel.X = 0.01f * (float)randInt(-15,15);
			anim.Pos_Vel.Z = 0.01f * (float)randInt(-15,15);
			anim.Pos_Vel.Y = 0.20f;
			o_AddEntityToList(MORBAKBODY,-1,anim,false);

			o_ClearAnimFields(&anim);
			anim.Pos = MonsterEnt->ent_anim.Pos;
			anim.Siz_Vel.X = 0.04f;
			anim.Siz_Vel.Y = 0.04f;
			anim.Siz_Vel.Z = 0.04f;
			o_AddEntityToList(BLOOD,-1,anim,false);

			break;

			case SUBZERAT:
			ds_PlaySound3D(53,MonsterEnt->ent_anim.Pos,MonsterEnt->ent_anim.Pos);
			MonsterEnt->Time_msec = 0;

			o_ClearAnimFields(&anim);
			anim.Pos = MonsterEnt->ent_anim.Pos;
			anim.Siz_Vel.X = 0.03f;
			anim.Siz_Vel.Y = 0.03f;
			anim.Siz_Vel.Z = 0.03f;
			o_AddEntityToList(BLOOD,-1,anim,false);

			break;

			case ZEMUMMY:
			ds_PlaySound3D(53,MonsterEnt->ent_anim.Pos,MonsterEnt->ent_anim.Pos);
			MonsterEnt->Time_msec = 0;

			o_ClearAnimFields(&anim);
			anim.Pos = MonsterEnt->ent_anim.Pos;
			anim.Siz_Vel.X = 0.03f;
			anim.Siz_Vel.Y = 0.03f;
			anim.Siz_Vel.Z = 0.03f;
			o_AddEntityToList(BLOOD,-1,anim,false);

			break;

			case ZEBAT:
			ds_PlaySound3D(53,MonsterEnt->ent_anim.Pos,MonsterEnt->ent_anim.Pos);
		//	MonsterEnt->Time_msec = 0;

			o_ClearAnimFields(&anim);
			anim.Pos = MonsterEnt->ent_anim.Pos;
			anim.Siz_Vel.X = 0.02f;
			anim.Siz_Vel.Y = 0.02f;
			anim.Siz_Vel.Z = 0.02f;
			o_AddEntityToList(BLOOD,-1,anim,false);

			break;
		}
	}

	return MonsterEnt;
}

void monster_UpdateCounter(pEntity MonsterEnt)
{
	int		value;
//	int		mask;
	int		diff;
	int		Max_DeplaToKill;
	int		Max_MorbakToKill;
	int		Max_ZeBatToKill;
	int		Max_SubZeratToKill;
	int		Max_ZeMummyToKill;
	float	zoom = 3.0f; // 1.6f

	if(gIsMultiplayer)
		return;

	if(gAllGoalCleared)
		return;

	diff = (int)difficulty.value;

	Max_DeplaToKill = MapGoals[1][diff][game_MapID];
	Max_MorbakToKill = MapGoals[2][diff][game_MapID];
	Max_ZeBatToKill = MapGoals[3][diff][game_MapID];
	Max_SubZeratToKill = MapGoals[4][diff][game_MapID];
	Max_ZeMummyToKill = MapGoals[5][diff][game_MapID];

	if(MonsterEnt->entity_id == DEPLA)
	{
		gDeplaHudSize = zoom;
		game_catchedinit(11);
		gKilledDeplas++;
		if(gKilledDeplas == Max_DeplaToKill)
		{
			ds_PlaySound(43);		 
			KillDepla_Time = 2500;
			value = (int)kill_depla.value;
			game_SetBit(&value, game_MapID);
			kill_depla.value = (float)value;
			game_CheckMapCompleted();
		}
		else
			monster_DieSound(MonsterEnt);
	}
	else if(MonsterEnt->entity_id == MORBAK)
	{
		gMorbakHudSize = zoom;
		game_catchedinit(12);
		gKilledMorbaks++;
		if(gKilledMorbaks == Max_MorbakToKill)
		{
			ds_PlaySound(44);
			KillMorbak_Time = 2500;
			value = (int)kill_morbak.value;
			game_SetBit(&value, game_MapID);
			kill_morbak.value = (float)value;
			game_CheckMapCompleted();
		}
		else
			monster_DieSound(MonsterEnt);
	}
	else if(MonsterEnt->entity_id == ZEBAT)
	{
		gZebatHudSize = zoom;
		game_catchedinit(13);
		gKilledZeubat++;
		if(gKilledZeubat == Max_ZeBatToKill)
		{
			ds_PlaySound(50);
			KillZebat_Time = 2500;
			value = (int)kill_zeubat.value;
			game_SetBit(&value, game_MapID);
			kill_zeubat.value = (float)value;
			game_CheckMapCompleted();
		}
		else
			monster_DieSound(MonsterEnt);
	}
	else if(MonsterEnt->entity_id == SUBZERAT)
	{
		gZeratHudSize = zoom;
		game_catchedinit(14);
		gKilledSubzerat++;
		if(gKilledSubzerat == Max_SubZeratToKill)
		{
			ds_PlaySound(38);
			KillSubzerat_Time = 2500;
			value = (int)kill_subzerat.value;
			game_SetBit(&value, game_MapID);
			kill_subzerat.value = (float)value;
			game_CheckMapCompleted();
		}
		else
			monster_DieSound(MonsterEnt);
	}
	else if(MonsterEnt->entity_id == ZEMUMMY)
	{
		gMummyHudSize = zoom;
		game_catchedinit(15);
		gKilledZemummy++;
		if(gKilledZemummy == Max_ZeMummyToKill)
		{
			ds_PlaySound(52);
			KillZemummy_Time = 2500;
			value = (int)kill_zemummy.value;
			game_SetBit(&value, game_MapID);
			kill_zemummy.value = (float)value;
			game_CheckMapCompleted();
		}
		else
			monster_DieSound(MonsterEnt);
	}
}

void monster_DieSound(pEntity MonsterEnt)
{
	switch(MonsterEnt->entity_id)
	{
		case DEPLA:
		switch(randInt(0,5))
		{
			case 4:
			ds_PlaySound3D(49,MonsterEnt->ent_anim.Pos,MonsterEnt->ent_anim.Pos);
			break;

			default:
			ds_PlaySound3D(48,MonsterEnt->ent_anim.Pos,MonsterEnt->ent_anim.Pos_Vel);
			break;
		}
		break;

		case MORBAK:
		ds_PlaySound3D(67,MonsterEnt->ent_anim.Pos,MonsterEnt->ent_anim.Pos);
		break;

		case ZEBAT:
		break;

		case SUBZERAT:
		break;

		case ZEMUMMY:
		ds_PlaySound3D(57,MonsterEnt->ent_anim.Pos,MonsterEnt->ent_anim.Pos);
		break;
	}
}

void monster_AttackSound(pEntity MonsterEnt)
{
//	return;

	switch(MonsterEnt->entity_id)
	{
		case DEPLA:
		switch(randInt(0,12))
		{
			case 3:
			ds_PlaySound3D(46,MonsterEnt->ent_anim.Pos,MonsterEnt->ent_anim.Pos);
			break;

			case 5:
			ds_PlaySound3D(47,MonsterEnt->ent_anim.Pos,MonsterEnt->ent_anim.Pos);
			break;
		}
		break;

		case MORBAK:
		switch(randInt(0,12))
		{
			case 3:
			ds_PlaySound3D(65,MonsterEnt->ent_anim.Pos,MonsterEnt->ent_anim.Pos);
			break;

			case 5:
			ds_PlaySound3D(66,MonsterEnt->ent_anim.Pos,MonsterEnt->ent_anim.Pos);
			break;
		}
		break;

		case ZEBAT:
		switch(randInt(0,12))
		{
			case 3:
			ds_PlaySound3D(51,MonsterEnt->ent_anim.Pos,MonsterEnt->ent_anim.Pos);
			break;

			case 5:
			ds_PlaySound3D(70,MonsterEnt->ent_anim.Pos,MonsterEnt->ent_anim.Pos);
			break;
		}
		break;

		case SUBZERAT:
		switch(randInt(0,6))
		{
			case 2:
			ds_PlaySound3D(73,MonsterEnt->ent_anim.Pos,MonsterEnt->ent_anim.Pos);
			break;
		}
		break;

		case ZEMUMMY:
		switch(randInt(0,12)) // 128
		{
			case 3:
			ds_PlaySound3D(55,MonsterEnt->ent_anim.Pos,MonsterEnt->ent_anim.Pos);
			break;

			case 5:
			ds_PlaySound3D(56,MonsterEnt->ent_anim.Pos,MonsterEnt->ent_anim.Pos);
			break;
		}
		break;
	}
}

void monster_NormalSound(pEntity MonsterEnt)
{
//	return;

	switch(MonsterEnt->entity_id)
	{
		case DEPLA:
		switch(randInt(0,8))
		{
			case 3:
			ds_PlaySound3D(49,MonsterEnt->ent_anim.Pos,MonsterEnt->ent_anim.Pos);
			break;

			case 4:
			ds_PlaySound3D(63,MonsterEnt->ent_anim.Pos,MonsterEnt->ent_anim.Pos);
			break;

			case 5:
			ds_PlaySound3D(64,MonsterEnt->ent_anim.Pos,MonsterEnt->ent_anim.Pos);
			break;
		}
		break;

		case MORBAK:
		switch(randInt(0,8))
		{
			case 3:
			ds_PlaySound3D(68,MonsterEnt->ent_anim.Pos,MonsterEnt->ent_anim.Pos);
			break;

			case 4:
			ds_PlaySound3D(69,MonsterEnt->ent_anim.Pos,MonsterEnt->ent_anim.Pos);
			break;
		}
		break;

		case ZEBAT:
		switch(randInt(0,8))
		{
			case 3:
			ds_PlaySound3D(71,MonsterEnt->ent_anim.Pos,MonsterEnt->ent_anim.Pos);
			break;

			case 4:
			ds_PlaySound3D(72,MonsterEnt->ent_anim.Pos,MonsterEnt->ent_anim.Pos);
			break;
		}
		break;

		case SUBZERAT:
		switch(randInt(0,8))
		{
			case 3:
			ds_PlaySound3D(74,MonsterEnt->ent_anim.Pos,MonsterEnt->ent_anim.Pos);
			break;

			case 4:
			ds_PlaySound3D(75,MonsterEnt->ent_anim.Pos,MonsterEnt->ent_anim.Pos);
			break;
		}
		break;

		case ZEMUMMY:
		switch(randInt(0,8))
		{
			case 3:
			ds_PlaySound3D(58,MonsterEnt->ent_anim.Pos,MonsterEnt->ent_anim.Pos);
			break;

			case 4:
			ds_PlaySound3D(59,MonsterEnt->ent_anim.Pos,MonsterEnt->ent_anim.Pos);
			break;

			case 5:
			ds_PlaySound3D(60,MonsterEnt->ent_anim.Pos,MonsterEnt->ent_anim.Pos);
			break;
		}
		break;
	}
}

pEntity MonsterDamage(pEntity MonsterEnt, Model *model)
{
	pEntity	pTemp;
	pEntity	pCurr;
	int		diff;

	if(!MonsterEnt)
		return NULL;

	diff = (int)difficulty.value;


	for(pCurr=gEntitiesList ; pCurr ; )
	{
		pTemp = pCurr;
		pCurr = pCurr->Next;

		if(pTemp->entity_id == FLECHE && pTemp->parent_id == PERSO && !pTemp->Timing )
		if(CollideEntity(MonsterEnt,pTemp,0.5f))
		{
			ds_PlaySound(8);

			MonsterEnt->ent_anim.Pos_Vel.X += pTemp->ent_anim.Pos_Vel.X * 10.0f;
			//MonsterEnt->ent_anim.Pos_Vel.Y += pTemp->ent_anim.Pos_Vel.Y * 10.0f;
			MonsterEnt->ent_anim.Pos_Vel.Z += pTemp->ent_anim.Pos_Vel.Z * 10.0f;

			// ajout de sang enorme
			{
				ANIMENTITY anim;

				o_ClearAnimFields(&anim);
				anim.Pos = MonsterEnt->ent_anim.Pos;
				anim.Siz_Vel.X = 0.010f;
				anim.Siz_Vel.Y = 0.010f;
				anim.Siz_Vel.Z = 0.010f;
				o_AddEntityToList(BLOOD,-1,anim,false);
			}


			o_RemoveEntityFromList(pTemp);
			fx_Particule(MonsterEnt->ent_anim.Pos,10,GLRED);

			MonsterEnt->Monster_Life -= WeaponDammage[1][diff];
			if(MonsterEnt->Monster_Life <= 0)
				return monster_Die(MonsterEnt, model, false);
		}

		if(pTemp->entity_id == SPINE/*SPIKE*/ && pTemp->parent_id == PERSO && !pTemp->Timing)
		if(CollideEntity(MonsterEnt,pTemp,0.5f))
		{
			ds_PlaySound(8);

			MonsterEnt->ent_anim.Pos_Vel.X += pTemp->ent_anim.Pos_Vel.X * 10.0f;
			//MonsterEnt->ent_anim.Pos_Vel.Y += pTemp->ent_anim.Pos_Vel.Y * 10.0f;
			MonsterEnt->ent_anim.Pos_Vel.Z += pTemp->ent_anim.Pos_Vel.Z * 10.0f;
		
			// ajout de sang enorme
			{
				ANIMENTITY anim;

				o_ClearAnimFields(&anim);
				anim.Pos = MonsterEnt->ent_anim.Pos;
				anim.Siz_Vel.X = 0.02f;
				anim.Siz_Vel.Y = 0.02f;
				anim.Siz_Vel.Z = 0.02f;
				o_AddEntityToList(BLOOD,-1,anim,false);
			}

			o_RemoveEntityFromList(pTemp);
			fx_Particule(MonsterEnt->ent_anim.Pos,20,GLRED);

			MonsterEnt->Monster_Life -= WeaponDammage[3][diff];
			if(MonsterEnt->Monster_Life <= 0)
				return monster_Die(MonsterEnt, model, true);
		}

		if(pTemp->entity_id == EXPLOSION)
		if(pTemp->parent_id == PERSO)
		if(CollideEntity(MonsterEnt,pTemp,0.5f))
		{
			ds_PlaySound(8);

			MonsterEnt->Monster_Life -= WeaponDammage[2][diff];
			if(MonsterEnt->Monster_Life <= 0)
				return monster_Die(MonsterEnt, model, true);
		}

		if(pTemp->entity_id == BOMBE)
		if(pTemp->parent_id == PERSO)
		if(CollideEntity(MonsterEnt,pTemp,0.5f))
		{
			ds_PlaySound(8);

			MonsterEnt->ent_anim.Pos_Vel.X += pTemp->ent_anim.Pos_Vel.X * 10.0f;
		//	MonsterEnt->ent_anim.Pos_Vel.Y += pTemp->ent_anim.Pos_Vel.Y * 10.0f;
			MonsterEnt->ent_anim.Pos_Vel.Z += pTemp->ent_anim.Pos_Vel.Z * 10.0f;

			pTemp->Time_msec=0;
		}
	}
	return MonsterEnt;
}

pEntity UpdateMonster(pEntity PlayerEnt, pEntity MonsterEnt)
{
	pEntity	pTemp;
	float	Player_Monster_Distance;
	float	eDistance;
	vect_t	dir;
	vect_t	Dest;
	Model	*model = NULL;
	int		index;
	CTRACE	ClipTrace;
	int		diff;
	float	m_dist;
	int		monster_leaf;

	if(PlayerEnt == NULL || MonsterEnt == NULL)
		return NULL;


	// pas d'IA sur les monstres pendant une partie en reseau
	if(gIsMultiplayer)
	{
		if(gIsServer && MonsterEnt->entity_id == PERSO)
		{
			return MonsterEnt;
		}
		else if(gIsServer && MonsterEnt->entity_id == MORBAK)
		{
			model = &gModel_morbak;

			if(MonsterEnt->IsChanged_Animation)
			{
				MonsterEnt->IsChanged_Animation = false;
			//	msk_ChangeAnim(MonsterEnt, model, MSK_RUN);//MonsterEnt->Animation_ID);
				msk_ChangeAnim(MonsterEnt, model, MonsterEnt->Animation_ID);
			}
			Model_advanceAnimation(MonsterEnt, model);
			MonsterEnt = MonsterDamage(MonsterEnt, model);
		}
		else if(!gIsServer && MonsterEnt->entity_id == DEPLA)
		{
			model = &gModel_depla;

			if(MonsterEnt->IsChanged_Animation)
			{
				MonsterEnt->IsChanged_Animation = false;
				msk_ChangeAnim(MonsterEnt, model, MonsterEnt->Animation_ID);
			}
			Model_advanceAnimation(MonsterEnt, model);
		}

		if(	MonsterEnt->entity_id != PERSO		&&
			MonsterEnt->entity_id != DEPLA		&&
			MonsterEnt->entity_id != MORBAK		&&
			MonsterEnt->entity_id != ZEBAT		&&
			MonsterEnt->entity_id != SUBZERAT	&&
			MonsterEnt->entity_id != ZEMUMMY)
			return MonsterEnt;

		MonsterEnt = phy_UpdateMoves(MonsterEnt);
		return MonsterEnt;
	}


	if(	MonsterEnt->entity_id != DEPLA		&&
		MonsterEnt->entity_id != MORBAK		&&
		MonsterEnt->entity_id != ZEBAT		&&
		MonsterEnt->entity_id != SUBZERAT	&&
		MonsterEnt->entity_id != ZEMUMMY)
		return MonsterEnt;

	diff = (int)difficulty.value;

	if(MonsterEnt->SolidSpace)
	{
		index = o_FindRespawnTabIndex(MonsterEnt);
		if(index >= 0)
		{
			RespawnTab[index].pickedup = true;
			RespawnTab[index].Timer = 5000.0f;
		}

		o_RemoveEntityFromList(MonsterEnt);
		gNumberOfMonsters--;
		return NULL;
	}

	if(MonsterEnt->entity_id == DEPLA)
		model = &gModel_depla;
	else if(MonsterEnt->entity_id == MORBAK)
		model = &gModel_morbak;
	else if(MonsterEnt->entity_id == ZEBAT)
		model = &gModel_zebat;
	else if(MonsterEnt->entity_id == SUBZERAT)
		model = &gModel_subzerat;
	else if(MonsterEnt->entity_id == ZEMUMMY)
		model = &gModel_zemummy;
	else
		return MonsterEnt;

	Player_Monster_Distance = m_Distance(MonsterEnt->ent_anim.Pos, PlayerEnt->ent_anim.Pos);
	eDistance = PlayerEnt->Radius.X + MonsterEnt->Radius.X;

	if(MonsterEnt->Timing == false && MonsterEnt->entity_id == ZEBAT)
	{
		if(ProgramState != PS_GAMEOVER)
		{
			switch(randInt(0,6))
			{
				case 1:
				MonsterEnt->ent_anim.Pos.Y += gFrameTime_msec * 0.1f;
				break;

				case 3:
				MonsterEnt->ent_anim.Pos.Y -= gFrameTime_msec * 0.1f;
				break;
			}
		}
	}

	if(ProgramState != PS_GAMEOVER)
	{
		if(MapDataFormat == MAP_BSP)
		{
			// HACK : les monstres qui sont pas visibles dans le PVS ne se deplacent pas. (speed hack)
			monster_leaf = bsp_FindCamera(MonsterEnt->ent_anim.Pos);
			if(monster_leaf < 0 || gLeafLocation < 0)
				goto nomove_hack;
			if(PVSData[(gLeafLocation*gNb_Leafs)+monster_leaf] != '1')
				goto nomove_hack;
		}
		else if(MapDataFormat == MAP_POLYGONS)
		{
		}

		if(game_MatrixMode)
			goto matrix_mode;
	
		MonsterEnt = phy_UpdateMoves(MonsterEnt);
		if(!MonsterEnt)
		{
			return NULL;
		}
	}

//BUG_ENORME:

	Model_advanceAnimation(MonsterEnt, model);

nomove_hack:	
matrix_mode:

	if(MonsterEnt->Timing)
	{
		MonsterEnt->ent_anim.Pos_Vel.X *= 0.8f;
		//MonsterEnt->ent_anim.Pos_Vel.Y *= 0.8f;
		MonsterEnt->ent_anim.Pos_Vel.Z *= 0.8f;
		return MonsterEnt;
	}

	//	On ne doit pas traverser le monstre
	if(Player_Monster_Distance < eDistance)
	{
		dir.X = MonsterEnt->ent_anim.Pos.X - PlayerEnt->ent_anim.Pos.X;
		dir.Y = MonsterEnt->ent_anim.Pos.Y - PlayerEnt->ent_anim.Pos.Y;
		dir.Z = MonsterEnt->ent_anim.Pos.Z - PlayerEnt->ent_anim.Pos.Z;
		Normalize(&dir);
		m_Vector_Scale(&dir,(eDistance-Player_Monster_Distance)*4.0f); // * 1.0f

		ClipTrace.Response = true;
		ClipTrace.NoRadius = false;
		ClipTrace.DontUnstuck = false;

		Dest = m_Vector_Add(MonsterEnt->ent_anim.Pos,dir);
		ClipTrace.ellipsoid = MonsterEnt->Radius;
		ClipTrace.Ent = MonsterEnt; //
		col_Handle(&MonsterEnt->ent_anim.Pos,Dest,&ClipTrace);
		MonsterEnt->SolidSpace = ClipTrace.solidspace;

		m_Vector_Scale(&dir,-1);
		Dest = m_Vector_Add(PlayerEnt->ent_anim.Pos,dir);
		ClipTrace.ellipsoid = PlayerEnt->Radius;
		ClipTrace.Ent = PlayerEnt; //
		col_Handle(&PlayerEnt->ent_anim.Pos,Dest,&ClipTrace);
		PlayerEnt->SolidSpace = ClipTrace.solidspace;
	}

	// Heavy stealth Hit Monster?
	if(gPlayerHasGun==1 && MassueCanHurt)
	{
		pTemp = o_FindHoldedEntity(SABRE);
		if(pTemp)
		{
			if(pTemp->Timing)
			if(pTemp->Time_msec < 300.0f)
			if(Player_Monster_Distance < (eDistance*1.2f))
			{
				//ds_PlaySound(8);
				fx_Particule(MonsterEnt->ent_anim.Pos,20,GLRED);

				// ajout de sang enorme
				{
					ANIMENTITY anim;

					o_ClearAnimFields(&anim);
					anim.Pos = MonsterEnt->ent_anim.Pos;
					anim.Siz_Vel.X = 0.02f;
					anim.Siz_Vel.Y = 0.02f;
					anim.Siz_Vel.Z = 0.02f;
					o_AddEntityToList(BLOOD,-1,anim,false);
				}


				ds_PlaySound(26+randInt(0,2));
				MonsterEnt->Monster_Life -= WeaponDammage[0][diff];

				MassueCanHurt = false;

				if(MonsterEnt->Monster_Life <= 0)
					return monster_Die(MonsterEnt, model, false);
			}
		}
	}
	

	MonsterEnt = MonsterDamage(MonsterEnt, model);
	if(!MonsterEnt)
		return NULL;
	if(MonsterEnt->Monster_Life <= 0)
	{
		return MonsterEnt;
	}

	
	MonsterEnt->Time_msec -= gFrameTime_msec;
	if(MonsterEnt->Time_msec<=0)
	{
		MonsterEnt->Time_msec = 1000.0f;
		MonsterEnt->Monster_MoveState = randInt(0,8);
	}

	/*
	if(ProgramState == PS_GAMEOVER)
	{
		if(rand()%32 == 8)
		{
			Launch(MonsterEnt,TOILE,0.2f);
		}
	}
	*/

	if(ProgramState != PS_GAMEOVER)
	if(!game_MatrixMode)
	{
		if(game_InjuryMode)
			m_dist = 600.0f;
		else
			m_dist = 200.0f;

		if(Player_Monster_Distance < m_dist)
		{
			dir.X = MonsterEnt->ent_anim.Pos.X - PlayerEnt->ent_anim.Pos.X;
			dir.Y = MonsterEnt->ent_anim.Pos.Y - PlayerEnt->ent_anim.Pos.Y;
			dir.Z = MonsterEnt->ent_anim.Pos.Z - PlayerEnt->ent_anim.Pos.Z;
			Normalize(&dir);
			MonsterEnt->ent_anim.Rot.Y = cam_RotationCycle( (float)acos(dir.Z) * INVPIOVER180 );
			MonsterEnt->ent_anim.Rot.Y = cam_RotationCycle( PlayerEnt->ent_anim.Rot.Y + 180.0f );
				

			if(MonsterEnt->entity_id == MORBAK)
			{
				Launch(MonsterEnt,TOILE,0.2f);
				if(rand()%10 == 2)
				{
					MonsterEnt->JumpPossible = true;
					JumpEntity(MonsterEnt);
					if(A_absf(dir.X) > A_absf(dir.Z))
						MonsterEnt->ent_anim.Pos_Vel.X *= 10.0f;
					else
						MonsterEnt->ent_anim.Pos_Vel.Z *= 10.0f;
				}
			}
			else if(MonsterEnt->entity_id == DEPLA)
			{
				Launch(MonsterEnt,DEPLASPIKE,0.7f);
			}
			else
			{
				Launch(MonsterEnt,SPIKE,0.7f);
			}
			monster_AttackSound(MonsterEnt);
		}
		else
		{
			monster_NormalSound(MonsterEnt);
		}
	}

	switch(MonsterEnt->Monster_MoveState)
	{
		case 0:
		msk_ChangeAnim(MonsterEnt,model,MSK_THINK);
		break;

		case 1:
		msk_ChangeAnim(MonsterEnt,model,MSK_RUN);
		MoveEntity_Forward(MonsterEnt);
		TurnLeft_Monster(MonsterEnt);
		break;

		case 2:
		msk_ChangeAnim(MonsterEnt,model,MSK_ATTACK);
		MoveEntity_Forward(MonsterEnt);
		TurnRight_Monster(MonsterEnt);
		break;

		case 3:
		msk_ChangeAnim(MonsterEnt,model,MSK_RUN);
		MoveEntity_Forward(MonsterEnt);
		break;

		case 4:
		msk_ChangeAnim(MonsterEnt,model,MSK_THINK);
		break;

		case 5:
		msk_ChangeAnim(MonsterEnt,model,MSK_RUN);
		MoveEntity_Forward(MonsterEnt);
		break;

		default:
		msk_ChangeAnim(MonsterEnt,model,MSK_ATTACK);
		MoveEntity_Forward(MonsterEnt);
		break;
	}
	return MonsterEnt;
}

void TurnLeft_Monster(pEntity MonsterEnt)
{
	if(!MonsterEnt)
		return;
	if(game_MatrixMode)
		return;
	MonsterEnt->ent_anim.Rot.Y += (0.3f * player_rvel.value * gFrameTime_msec);
}

void TurnRight_Monster(pEntity MonsterEnt)
{
	if(!MonsterEnt)
		return;
	if(game_MatrixMode)
		return;
	MonsterEnt->ent_anim.Rot.Y -= (0.3f * player_rvel.value * gFrameTime_msec);
}
