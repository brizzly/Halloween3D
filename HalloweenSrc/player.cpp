
#include "alloween.h"

//#include "collision2.h"



float	gPlayerGunRecul;
float	gPlayerBobWalkstep;
float	gPlayerWalkTime;
hBool	gPlayerWalk;
hBool	IsLaunchPossible=true;
int		gPlayerHasGun=-1;
pFace	Gun1LaunchedFaceList;
pFace	Gun3FaceList;


#define	WEAPON_RELOADTIME		500.0f
#define	WEAPON_MAXAMMO			50
#define	PICKUP_DIST				0.02f


void UpdatePlayer()
{
	pEntity	PlayerEnt;
	float	dist;


	if(gIsMultiplayer && !gIsServer)
		return;


	PlayerEnt = o_FindEntity(PERSO);
	if(!PlayerEnt)
		return;


	// DEBUG
//	col2_Process(PlayerEnt->ent_anim.Pos.X, PlayerEnt->ent_anim.Pos.Y, PlayerEnt->ent_anim.Pos.Z);


	// found a stop
	dist = m_Distance(PlayerEnt->ent_anim.Pos, gStopPos);
//	if(developer.value)
//		g_DispString(14,"dist : %f\n",dist);
	if(dist <= STOP_DIST)
	{
		game_InitYouWin();
		return;
	}

	// HACK
	if(MapDataFormat != MAP_POLYGONS)
	if(PlayerEnt->SolidSpace)
	{
		if(ghost.value)
		{
			if(PlayerEnt->ent_anim.Pos.Y <= MIN_BOUND+100)
			{
				PlayerResetStartPos(PERSO);
			}
		}
		else
		{
			ds_PlaySound(42);
			if(!player_view.value)
			{
				if(!gIsMultiplayer || gIsServer)
				{
					gCamera.Pos.X = gStart.X;
					gCamera.Pos.Y = gStart.Y;
					gCamera.Pos.Z = gStart.Z;
					gCamera.Rot.X = 0;
					gCamera.Rot.Y = (float)gAngleView_player + 179.9f; //180.0f; // BUG BLOCKED
					gCamera.Rot.Z = 0;
				}
			}
			else
			{
				PlayerResetStartPos(PERSO);
			}
		}
	}

	EnnemyDammage(PlayerEnt);
	PlayerEnt = phy_UpdateMoves(PlayerEnt);
	if(!PlayerEnt)
		return;
	PickupItems(PlayerEnt);
}

void PlayerHurt(pEntity Entity, int dammage)
{
	if(ProgramState == PS_DIE ||
		ProgramState == PS_ENDSCORE ||
		ProgramState == PS_WIN)
		return;
	
	ds_PlaySound(11);
	
	if(god.value)
		return;

	if(gIsMultiplayer && Entity->entity_id == MORBAK)
	{
		Entity->health -= dammage;
		if(Entity->health <= 0)
		{
			if( dammage >= 50 )
			{
				ds_PlaySound(53);
			}

			switch(randInt(0,3))
			{
			case 0:
				ds_PlaySound(42);
				break;
			case 1:
				ds_PlaySound(17);
				break;
			default:
				ds_PlaySound(33);
				break;
			}

			// le client s'est suicidé, -1 au score
			if(!gMultiPodium && gConnectedPlayer > 1)
			{
				//gCL_score--;
				//net_SendServerScores();
			}

			PlayerResetStartPos(MORBAK);
			Entity->health = 100;
		}

		//net_SendClientHurt(Entity->health);
		return;
	}

	gHeartSize = 1.5f; // en plus..

	game_heart -= dammage;
	if(game_heart <= 0)
	{
		game_heart = 0;
		if(gIsMultiplayer && !gIsServer)
			goto no_hitsound;

		
	//	if( dammage >= 50 )
	//	{
	//		ds_PlaySound(53);
	//	}

		switch(randInt(0,3))
		{
		case 0:
			ds_PlaySound(42);
			break;
		case 1:
			ds_PlaySound(17);
			break;
		default:
			ds_PlaySound(33);
			break;
		}

		/*
		if(!gMultiPodium && gIsMultiplayer && gIsServer && Entity->entity_id == PERSO)
		{
			if(gConnectedPlayer > 1)
			{
				// le serveur s'est suicidé, -1 au score
				gSV_score--;
				net_SendServerScores();
			}
		}
		*/

		gHeartSize = 1.5f;

		PlayerSwapWeapon(0);
		ProgramState = PS_DIE;
	//	fx_ColorScreen(1800,200,0.1f,0.1f,0.1f,true);
		fx_ColorScreen(1800,200,0,0,0,true);
	//	PlayerResetStartPos(PERSO);
	//	game_heart=100;
		
no_hitsound:;
	}
}

void EnnemyDammage(pEntity PlayerEnt)
{
	pEntity	pTemp;

	if(!PlayerEnt)
		return;

	if(ProgramState == PS_DIE)
		return;

	for(pTemp=gEntitiesList ; pTemp ; pTemp=pTemp->Next)
	{
		if(pTemp->entity_id == EXPLOSION)
		if(CollideEntity(PlayerEnt,pTemp,0.5f))
		{
			if(PlayerEnt->Time_msec <= 0)
			{
				if(!gIsMultiplayer || gIsServer)
				if(ProgramState != PS_DIE && ProgramState != PS_WIN)
					fx_ColorScreen(100,0.5f,1,0.2f,0.2f,false);
				PlayerHurt(PlayerEnt, (int)MonsterDammage[3][(int)difficulty.value]);
				PlayerEnt->Time_msec = 100.0f;
			}
			else
				PlayerEnt->Time_msec -= gFrameTime_msec;
			continue;
		}

		if(!pTemp->Timing)
		if(pTemp->entity_id == SPIKE || pTemp->entity_id == DEPLASPIKE)
		if(	pTemp->parent_id == DEPLA		||
			pTemp->parent_id == MORBAK		||
			pTemp->parent_id == ZEBAT		||
			pTemp->parent_id == SUBZERAT	||
			pTemp->parent_id == ZEMUMMY)
		if(CollideEntity(PlayerEnt,pTemp,0.5f))
		{
			PlayerEnt->ent_anim.Pos_Vel.X += pTemp->ent_anim.Pos_Vel.X * 10.0f;
		//	PlayerEnt->ent_anim.Pos_Vel.Y += pTemp->ent_anim.Pos_Vel.Y * 10.0f;
			PlayerEnt->ent_anim.Pos_Vel.Z += pTemp->ent_anim.Pos_Vel.Z * 10.0f;

			if(!gIsMultiplayer || gIsServer)
			if(ProgramState != PS_DIE && ProgramState != PS_WIN)
				fx_ColorScreen(100,0.5f,1,0.2f,0.2f,false);
			PlayerHurt(PlayerEnt, (int)MonsterDammage[1][(int)difficulty.value]);
			o_RemoveEntityFromList(pTemp);
			pTemp=gEntitiesList;
		}

		if(!pTemp->Timing)
		if(pTemp->entity_id == TOILE)
		if(	//pTemp->parent_id == DEPLA		||
			pTemp->parent_id == MORBAK	//	||
			//pTemp->parent_id == ZEBAT		||
			//pTemp->parent_id == SUBZERAT	||
			//pTemp->parent_id == ZEMUMMY
			)
		if(CollideEntity(PlayerEnt,pTemp,2.5f)) // 3
		{
			PlayerEnt->ent_anim.Pos_Vel.X += pTemp->ent_anim.Pos_Vel.X * 10.0f;
		//	PlayerEnt->ent_anim.Pos_Vel.Y += pTemp->ent_anim.Pos_Vel.Y * 10.0f;
			PlayerEnt->ent_anim.Pos_Vel.Z += pTemp->ent_anim.Pos_Vel.Z * 10.0f;

			if(!gIsMultiplayer || gIsServer)
			if(ProgramState != PS_DIE && ProgramState != PS_WIN)
				fx_ColorScreen(100,0.5f,1,0.2f,0.2f,false);
			PlayerHurt(PlayerEnt, (int)MonsterDammage[2][(int)difficulty.value]);
			o_RemoveEntityFromList(pTemp);
			pTemp=gEntitiesList;
		}


		if(!pTemp->Timing)
		if(pTemp->entity_id == FLECHE)
		if(pTemp->parent_id == MORBAK)
		if(CollideEntity(PlayerEnt,pTemp,0.5f))
		{
			PlayerEnt->ent_anim.Pos_Vel.X += pTemp->ent_anim.Pos_Vel.X * 10.0f;
		//	PlayerEnt->ent_anim.Pos_Vel.Y += pTemp->ent_anim.Pos_Vel.Y * 10.0f;
			PlayerEnt->ent_anim.Pos_Vel.Z += pTemp->ent_anim.Pos_Vel.Z * 10.0f;

			if(!gIsMultiplayer || gIsServer)
			if(ProgramState != PS_DIE && ProgramState != PS_WIN)
				fx_ColorScreen(100,0.5f,1,0.2f,0.2f,false);
			PlayerHurt(PlayerEnt, (int)MonsterDammage[2][(int)difficulty.value]);
			o_RemoveEntityFromList(pTemp);
			pTemp=gEntitiesList;
		}

		if(pTemp->entity_id == BOMBE)
		if(pTemp->parent_id == MORBAK)
		if(CollideEntity(PlayerEnt,pTemp,0.5f))
		{
		//	PlayerEnt->ent_anim.Pos_Vel.X += pTemp->ent_anim.Pos_Vel.X * 10.0f;
		//	//PlayerEnt->ent_anim.Pos_Vel.Y += pTemp->ent_anim.Pos_Vel.Y * 10.0f;
		//	PlayerEnt->ent_anim.Pos_Vel.Z += pTemp->ent_anim.Pos_Vel.Z * 10.0f;

			pTemp->Time_msec = 0;
			if(!gIsMultiplayer || gIsServer)
			if(ProgramState != PS_DIE && ProgramState != PS_WIN)
				fx_ColorScreen(100,0.5f,1,0.2f,0.2f,false);
		}
	}

	// HACK
	if(!gMultiPodium && gIsMultiplayer && gIsServer && ProgramState == PS_DIE)
	{
		// le serveur s'est fait butté
		//gCL_score++;
		//net_SendServerScores();
	}
}

void PickupItems(pEntity PlayerEnt)
{
	int			i;
	pEntity		RespawnEnt;
	pEntity		Ent;
	float		RespawnTime = 15000.0f;

	if(!PlayerEnt)
		return;

	for(i=0 ; i<gNb_Respawn ; i++)
	if(!RespawnTab[i].pickedup)
	{
		RespawnEnt = o_FindEntityFromID(RespawnTab[i].list_id);
		if(!RespawnEnt)
			continue;

		if(m_Distance(PlayerEnt->ent_anim.Pos,RespawnEnt->ent_anim.Pos) >= 30.0f)
			continue;

		switch(RespawnTab[i].entity_id)
		{
			case GUN1:
			game_catchedinit(1);
			game_weapon1=true;
			o_RemoveEntityFromList(RespawnEnt);
			RespawnTab[i].pickedup = true;
			RespawnTab[i].Timer = RespawnTime;
			ds_PlaySound(0);
			if(autoswitch.value)
				PlayerSwapWeapon(2);
			game_fleches+=10;
			if(game_fleches>WEAPON_MAXAMMO)
				game_fleches=WEAPON_MAXAMMO;
			break;

			case GUN2:
			game_catchedinit(2);
			game_weapon2=true;
			o_RemoveEntityFromList(RespawnEnt);
			RespawnTab[i].pickedup = true;
			RespawnTab[i].Timer = RespawnTime;
			ds_PlaySound(0);
			if(autoswitch.value)
				PlayerSwapWeapon(3);
			game_bombes+=5;
			if(game_bombes>WEAPON_MAXAMMO)
				game_bombes=WEAPON_MAXAMMO;
			break;

			case GUN3:
			game_catchedinit(3);
			game_weapon3=true;
			o_RemoveEntityFromList(RespawnEnt);
			ds_PlaySound(28);
			RespawnTab[i].pickedup = true;
			RespawnTab[i].Timer = RespawnTime;
			if(autoswitch.value)
				PlayerSwapWeapon(4);
			game_bullet+=10;
			if(game_bullet>WEAPON_MAXAMMO)
				game_bullet=WEAPON_MAXAMMO;
			break;

			case AMO1:
			game_catchedinit(4);
			o_RemoveEntityFromList(RespawnEnt);
			RespawnTab[i].pickedup = true;
			RespawnTab[i].Timer = RespawnTime;
			ds_PlaySound(1);
			game_fleches+=10;
			if(game_fleches>WEAPON_MAXAMMO)
				game_fleches=WEAPON_MAXAMMO;
			break;

			case AMO2:
			game_catchedinit(5);
			o_RemoveEntityFromList(RespawnEnt);
			RespawnTab[i].pickedup = true;
			RespawnTab[i].Timer = RespawnTime;
			ds_PlaySound(1);
			game_bombes+=10;
			if(game_bombes>WEAPON_MAXAMMO)
				game_bombes=WEAPON_MAXAMMO;
			break;

			case AMO3:
			game_catchedinit(6);
			o_RemoveEntityFromList(RespawnEnt);
			RespawnTab[i].pickedup = true;
			RespawnTab[i].Timer = RespawnTime;
			ds_PlaySound(1);
			game_bullet+=10;
			if(game_bullet>WEAPON_MAXAMMO)
				game_bullet=WEAPON_MAXAMMO;
			break;

			case PUMPKIN:
			if(gMapIntro)
			{
				Ent = o_FindEntityFromID(RespawnEnt->ent_anim.id_list);
				if(o_FindEntity2(PUMPKIN, 2) == NULL)
				{
					if(Ent == o_FindEntity2(PUMPKIN, 1))
					{
						game_LoadMap(-3);
					}
					else
					{
						game_LoadMap(-2);
					}
				}
				else
				{
					if(Ent == o_FindEntity2(PUMPKIN, 1))
					{
						game_LoadMap(-2);
					}
					else
					{
						game_LoadMap(-3);
					}
				}
			}
			else
			{
				if(game_heart < 100)
				{
					game_catchedinit(7);
					o_RemoveEntityFromList(RespawnEnt);
					RespawnTab[i].pickedup = true;
					RespawnTab[i].Timer = RespawnTime;
					gHeartSize = 1.2f;
					ds_PlaySound(1);
					game_heart+=25;
					if(game_heart>100)
						game_heart=100;
				}
			}
			break;

			case FIOLE1:
			if(!game_fiole1_catched && !game_fiole2_catched && !game_fiole3_catched)
			{
				game_catchedinit(8);
				o_RemoveEntityFromList(RespawnEnt);
				RespawnTab[i].pickedup = true;
				RespawnTab[i].Timer = 3000;
				ds_PlaySound(1);
				game_fiole1_catched = true;
				o_AddHudFlask(1);
			}
			break;

			case FIOLE2:
			if(!game_fiole1_catched && !game_fiole2_catched && !game_fiole3_catched)
			{
				game_catchedinit(9);
				o_RemoveEntityFromList(RespawnEnt);
				RespawnTab[i].pickedup = true;
				RespawnTab[i].Timer = RespawnTime;
				ds_PlaySound(1);
				game_fiole2_catched = true;
				o_AddHudFlask(2);
			}
			break;

			case FIOLE3:
			if(!game_fiole1_catched && !game_fiole2_catched && !game_fiole3_catched)
			{
				game_catchedinit(10);
				o_RemoveEntityFromList(RespawnEnt);
				RespawnTab[i].pickedup = true;
				RespawnTab[i].Timer = 3000.0f;
				ds_PlaySound(1);
				game_fiole3_catched = true;
				o_AddHudFlask(3);
			}
			break;

			case H:
			ds_PlaySound(1);
			if(gMapIntro)
			{
				game_LoadMap(0);
			}
			else
			{
				o_RemoveEntityFromList(RespawnEnt);
				HLetter_Time = 2500;
				game_AddHLetter('H');
			}
			break;

			case A:
			ds_PlaySound(1);
			if(gMapIntro)
			{
				game_LoadMap(1);
			}
			else
			{
				o_RemoveEntityFromList(RespawnEnt);
				HLetter_Time = 2500;
				game_AddHLetter('A');
			}
			break;

			case L:
			ds_PlaySound(1);
			if(gMapIntro)
			{
				Ent = o_FindEntityFromID(RespawnEnt->ent_anim.id_list);
				if(o_FindEntity2(L, 2) == NULL)
				{
					if(Ent == o_FindEntity2(L, 1))
					{
						game_LoadMap(2);
					}
					else
					{
						game_LoadMap(3);
					}
				}
				else
				{
					if(Ent == o_FindEntity2(L, 1))
					{
						game_LoadMap(3);
					}
					else
					{
						game_LoadMap(2);
					}
				}
			}
			else
			{
				o_RemoveEntityFromList(RespawnEnt);
				HLetter_Time = 2500;
				game_AddHLetter('L');
			}
			break;

			case O:
			ds_PlaySound(1);
			if(gMapIntro)
			{
				game_LoadMap(4);
			}
			else
			{
				o_RemoveEntityFromList(RespawnEnt);
				HLetter_Time = 2500;
				game_AddHLetter('O');
			}
			break;

			case W:
			ds_PlaySound(1);
			if(gMapIntro)
			{
				game_LoadMap(5);
			}
			else
			{
				o_RemoveEntityFromList(RespawnEnt);
				HLetter_Time = 2500;
				game_AddHLetter('W');
			}
			break;

			case E:
			ds_PlaySound(1);
			if(gMapIntro)
			{
				Ent = o_FindEntityFromID(RespawnEnt->ent_anim.id_list);
				if(o_FindEntity2(E, 2) == NULL)
				{
					if(Ent == o_FindEntity2(E, 1))
					{
						game_LoadMap(6);
					}
					else
					{
						game_LoadMap(7);
					}
				}
				else
				{
					if(Ent == o_FindEntity2(E, 1))
					{
						game_LoadMap(7);
					}
					else
					{
						game_LoadMap(6);
					}
				}
			}
			else
			{
				o_RemoveEntityFromList(RespawnEnt);
				HLetter_Time = 2500;
				game_AddHLetter('E');
			}
			break;

			case N:
			ds_PlaySound(1);
			if(gMapIntro)
			{
				game_LoadMap(8);
			}
			else
			{
				o_RemoveEntityFromList(RespawnEnt);
				HLetter_Time = 2500;
				game_AddHLetter('N');
			}
			break;
		}
	}
}

void LaunchPlayer(int id, float Speed)
{
	pEntity	PlayerEnt;

	if(gIsMultiplayer && !gIsServer)
	{
		PlayerEnt = o_FindEntity(MORBAK);
	}
	else
	{
		PlayerEnt = o_FindEntity(PERSO);
	}
	Launch(PlayerEnt,id,Speed);
}

void ply_Movedir(int entid, int dir)
{
	pEntity		PlayerEnt;
	float		AccFactor;
	int			AngleY;
	vect_t		acceleration;

	if(gIsMultiplayer && !gIsServer && IsConsoleActive)
		return;
	if(entid == PERSO && gIsMultiplayer && gIsServer && IsConsoleActive)
		return;

	PlayerEnt = o_FindEntity(entid);
	if(!PlayerEnt)
		return;

	if(PlayerEnt->JumpPossible)
		AccFactor = groundacc.value;
	else
		AccFactor = airacc.value;

	AngleY = (int)PlayerEnt->ent_anim.Rot.Y;

	switch(dir)
	{
		case DIR_FORWARD:
		acceleration.X = Sin_i(AngleY) * AccFactor;
		acceleration.Y = 0;
		acceleration.Z = Cos_i(AngleY) * AccFactor;
		break;

		case DIR_BACKWARD:
		acceleration.X = -Sin_i(AngleY) * AccFactor;
		acceleration.Y = 0;
		acceleration.Z = -Cos_i(AngleY) * AccFactor;
		break;

		case DIR_STRAFLEFT:
		acceleration.X = Cos_i(AngleY) * AccFactor * 0.7f;
		acceleration.Y = 0;
		acceleration.Z = -Sin_i(AngleY) * AccFactor * 0.7;
		break;

		case DIR_STRAFRIGHT:
		acceleration.X = -Cos_i(AngleY) * AccFactor * 0.7;
		acceleration.Y = 0;
		acceleration.Z = Sin_i(AngleY) * AccFactor * 0.7;
		break;

		default:
		return;
	}

	if(!gIsMultiplayer || gIsServer)
	{
		PlayerEnt->ent_anim.Pos_Acc.X += acceleration.X;
		PlayerEnt->ent_anim.Pos_Acc.Y += acceleration.Y;
		PlayerEnt->ent_anim.Pos_Acc.Z += acceleration.Z;
	}
}

void MovePlayer_Down()
{
	pEntity		PlayerEnt;
	vect_t		PlayerDir;

	PlayerEnt = o_FindEntity(PERSO);
	if(!PlayerEnt)
		return;

	PlayerDir.X = 0;
	PlayerDir.Y = - player_vel.value * groundacc.value * gFrameTime_msec;
	PlayerDir.Z = 0;

	if(player_view.value == 0)
		PlayerDir.Y = -PlayerDir.Y;

	MoveCreature(PlayerEnt,PlayerDir);
}

void MovePlayer_Up()
{
	pEntity		PlayerEnt;
	vect_t		PlayerDir;

	PlayerEnt = o_FindEntity(PERSO);
	if(!PlayerEnt)
		return;

	PlayerDir.X = 0;
	PlayerDir.Y = player_vel.value * groundacc.value * gFrameTime_msec;
	PlayerDir.Z = 0;

	if(player_view.value == 0)
		PlayerDir.Y = -PlayerDir.Y;

	MoveCreature(PlayerEnt,PlayerDir);
}

void PlayerResetStartPos(int ent_id)
{
	ANIMENTITY	*anim;
	int			index;

	if(gIsMultiplayer)
	{
		index = rand() % gNb_Respawn;
		gStart = RespawnTab[index].entity_pos;
	}

	anim = o_FindEntityAnimField(ent_id);
	if(anim)
	{
		o_ClearAnimFields(anim);
		anim->Pos.X = gStart.X;
		anim->Pos.Y = gStart.Y;
		anim->Pos.Z = gStart.Z;
		anim->Rot.Y = (float)gAngleView_player;

		anim->Pos_Vel.Y = 0.1f;

		if(player_view.value)
			gCamera.Pos = anim->Pos;
	}
}

void SabreWulf()
{
	pEntity	SabreEnt;

	SabreEnt = o_FindHoldedEntity(SABRE);
	if(!SabreEnt)
		return;

	if(!SabreEnt->Timing)
	{
		SabreEnt->Timing = true;
		SabreEnt->Time_msec = 400.0f;
		switch(randInt(0,3))
		{
			case 0:
			ds_PlaySound(23);
			break;

			case 1:
			ds_PlaySound(24);
			break;

			case 2:
			ds_PlaySound(25);
			break;
		}
	}
}

void SabreStop()
{
	pEntity	SabreEnt;

	SabreEnt = o_FindHoldedEntity(SABRE);
	if(!SabreEnt)
		return;

	SabreEnt->ent_anim.Rot_Vel.X = 0;
	SabreEnt->ent_anim.Rot_Vel.Y = 0;

	SabreEnt->ent_anim.Rot.X = 0;
	SabreEnt->ent_anim.Rot.Y = 15.0f;

	MassueCanHurt = true;
}

void PlayerSwapWeapon(int weapon)
{
	pEntity		GunEnt;
	pEntity		PlayerEnt;
	ANIMENTITY	Anim;

	if(IsConsoleActive)
		return;

	if(gCameraShowTime)
		return;

	// hack
	if(gMapIntro && weapon == 1)
		weapon = 0;

	if(!gIsMultiplayer || gIsServer)
	{
		PlayerEnt = o_FindEntity(PERSO);
		if(!PlayerEnt)
			return;
	}
	else
	{
		PlayerEnt = o_FindEntity(MORBAK);
		if(!PlayerEnt)
			return;
	}

	if(gPlayerHasGun == weapon)
		return;

	if(gIsMultiplayer)
	{
		if(weapon == 1)
			return;	
		if(weapon == 4)
			return;
	}

	if(developer.value == false && terminator.value == false)
//	if(gIsDemo == false) // hack
	{
		if(weapon==2 && !game_weapon1)
			return;
		if(weapon==3 && !game_weapon2)
			return;
		if(weapon==4 && !game_weapon3)
			return;
	}

	// si on change d'arme alors on a plus le recul de l'arme : HACK
	gPlayerGunRecul = 0;

	gPlayerHasGun = weapon;
	
	GunEnt = o_FindHoldedEntity(SABRE);
	if(GunEnt)	o_RemoveEntityFromList(GunEnt);

	GunEnt = o_FindHoldedEntity(GUN1);
	if(GunEnt)	o_RemoveEntityFromList(GunEnt);

	GunEnt = o_FindHoldedEntity(GUN2);
	if(GunEnt)	o_RemoveEntityFromList(GunEnt);

	GunEnt = o_FindHoldedEntity(GUN3);
	if(GunEnt)	o_RemoveEntityFromList(GunEnt);

	switch(weapon)
	{
		case 1:
		ds_PlaySound(6);
		o_ClearAnimFields(&Anim);
		Anim.Rot.Y = 15.0f;
		Anim.Pos.X = +4.0f;
		Anim.Pos.Y = -6.0f; //-5.0f;
		Anim.Pos.Z = -6.0f;
		o_AddEntityToList(SABRE, -1, Anim, true);
		break;

		case 2:
		ds_PlaySound(6);
		o_ClearAnimFields(&Anim);
		Anim.Rot.Y = 270.0f;
		Anim.Pos.Y = -12.0f;
		Anim.Pos.Z = -8.0f;
		o_AddEntityToList(GUN1,-1,Anim,true);
		break;

		case 3:
		ds_PlaySound(6);
		o_ClearAnimFields(&Anim);
	//	Anim.Pos.Y = -12.0f;
	//	Anim.Pos.Z = -10.0f;
		//Anim.Rot.Y = 180.0f;
		Anim.Pos.Y = -14.0f;
		Anim.Pos.Z = -15.0f;
		Anim.Pos.X = +2.0f;
		o_AddEntityToList(GUN2,-1,Anim,true);
		break;

		case 4:
		ds_PlaySound(6);
		o_ClearAnimFields(&Anim);
		Anim.Rot.Y = 270.0f;
		Anim.Pos.Y = -12.0f;
		Anim.Pos.Z = -24.0f;
		o_AddEntityToList(GUN3,-1,Anim,true);
		break;
	}
}


