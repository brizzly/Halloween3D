
#include "alloween.cpp"


hBool phy_BoundPos(vect_t *Pos)
{
	hBool	IsBoundHit;

	IsBoundHit = false;

	if(Pos->X > MAX_CAMERA)
	{
		IsBoundHit = true;
		Pos->X = MAX_CAMERA;
	}
	else if(Pos->X < -MAX_CAMERA)
	{
		IsBoundHit = true;
		Pos->X = -MAX_CAMERA;
	}

	if(Pos->Y > MAX_CAMERA)
	{
		IsBoundHit = true;
		Pos->Y = MAX_CAMERA;
	}
	else if(Pos->Y < -MAX_CAMERA)
	{
		IsBoundHit = true;
		Pos->Y = -MAX_CAMERA;
	}

	if(Pos->Z > MAX_CAMERA)
	{
		IsBoundHit = true;
		Pos->Z = MAX_CAMERA;
	}
	else if(Pos->Z < -MAX_CAMERA)
	{
		IsBoundHit = true;
		Pos->Z = -MAX_CAMERA;
	}

	if(IsBoundHit)
	{
		return true;
	}
	return false;
}

pEntity Gravity(pEntity CreatureEnt, float factor)
{
	int		GroundType;
	int		index;

	if(!CreatureEnt)
		return NULL;

	GroundType = IsGround_UnderCreature(CreatureEnt, factor);
	CreatureEnt->Ground_Type = GroundType;


	if(developer.value && gIsMultiplayer && gIsServer && CreatureEnt->entity_id == MORBAK)
		g_DispString(14,"ground under client: %d",GroundType);


	if(!GroundType)
	{
		CreatureEnt->JumpPossible = false;
	}
	else
	{
		if(gIsMultiplayer && gIsServer && CreatureEnt->entity_id == MORBAK)
			CreatureEnt->JumpPossible = true;

		if(CreatureEnt->IsGravity)
		{
			CreatureEnt->IsGravity = false;
			if(	CreatureEnt->entity_id == GUN1 ||
				CreatureEnt->entity_id == GUN2 ||
				CreatureEnt->entity_id == GUN3)
			{
				CreatureEnt->ent_anim.Pos.Y += 10;
			}
		}

		if(GroundType == LAVA)
		{
			if(CreatureEnt->entity_id == PERSO)
			{
				if(!gIsMultiplayer || gIsServer)
				{
					if(ProgramState != PS_DIE)
					{
						if(CreatureEnt->Time_msec<=0)
						{
							if(ProgramState != PS_DIE && ProgramState != PS_WIN)
                                                        {
                                                            ds_PlaySound3D(10,CreatureEnt->ent_anim.Pos,CreatureEnt->ent_anim.Pos_Vel);
                                                            fx_ColorScreen(200,0.5f,1,0.2f,0.2f,false);
                                                        }
							PlayerHurt(CreatureEnt, (int)MonsterDammage[0][(int)difficulty.value]);
							CreatureEnt->Time_msec = 200.0f;
							// HACK
							if(!gMultiPodium && gIsMultiplayer && gIsServer && ProgramState == PS_DIE && gConnectedPlayer > 1)
							{
								// le serveur s'est suicidé, -1 au score
								//gSV_score--;
								//net_SendServerScores();
							}
						}
						else
							CreatureEnt->Time_msec -= gFrameTime_msec;
					}
				}
			}
			else if(CreatureEnt->entity_id == DEPLA		||
					CreatureEnt->entity_id == MORBAK	||
					CreatureEnt->entity_id == ZEBAT		||
					CreatureEnt->entity_id == SUBZERAT	||
					CreatureEnt->entity_id == ZEMUMMY)
			{
				// en mode multi les monstres sont pas dirigé par une IA donc pas de probleme de lave
				if(gIsMultiplayer)
				{
					if(gIsServer)
					{
						ds_PlaySound3D(10,CreatureEnt->ent_anim.Pos,CreatureEnt->ent_anim.Pos_Vel);
						if(CreatureEnt->Time_msec<=0)
						{
							PlayerHurt(CreatureEnt, (int)MonsterDammage[0][(int)difficulty.value]);
							CreatureEnt->Time_msec = 200.0f;
						}
						else
							CreatureEnt->Time_msec -= gFrameTime_msec;
					}
				}
				else
				{
					index = o_FindRespawnTabIndex(CreatureEnt);
					if(index >= 0)
					{
						fx_Particule(CreatureEnt->ent_anim.Pos,30,GLRED);
						RespawnTab[index].pickedup = true;
						RespawnTab[index].Timer = 5000.0f;
					}

					gNumberOfMonsters--;
					o_RemoveEntityFromList(CreatureEnt);
					return NULL;
				}
			}
		}
	}

	if(IsGround_UpCreature(CreatureEnt))
	{
		CreatureEnt->ent_anim.Pos_Vel.Y = 0;
	}
	return CreatureEnt;
}

hBool CollideEntity_Better(pEntity EntA, pEntity EntB)
{
	vect_t		pos, dst, cut;
	float		dist;
	Face_t		*curr;

	if(!EntA || !EntB)
		return false;

	pos = EntB->ent_anim.Pos;
	dst.X = pos.X + (EntB->ent_anim.Pos_Vel.X * gFrameTime_msec);
	dst.Y = pos.Y + (EntB->ent_anim.Pos_Vel.Y * gFrameTime_msec);
	dst.Z = pos.Z + (EntB->ent_anim.Pos_Vel.Z * gFrameTime_msec);

	pos.X -= EntA->ent_anim.Pos.X;
	pos.Y -= EntA->ent_anim.Pos.Y;
	pos.Z -= EntA->ent_anim.Pos.Z;

	dst.X -= EntA->ent_anim.Pos.X;
	dst.Y -= EntA->ent_anim.Pos.Y;
	dst.Z -= EntA->ent_anim.Pos.Z;

	for(curr=EntA->Boundaries ; curr ; curr=curr->Next)
	{
		if(Intersect_Poly(&pos,&dst,curr,&cut,&dist))
			return true;
	}
	return false;
}

hBool CollideEntity(pEntity EntA, pEntity EntB, float factor)
{
	vect_t	vD;
	vect_t	radius;
	float	D;
	float	A;
	float	B;

	if(!EntA || !EntB)
		return false;

	vD.X = EntA->ent_anim.Pos.X - EntB->ent_anim.Pos.X;
	vD.Y = EntA->ent_anim.Pos.Y - EntB->ent_anim.Pos.Y;
	vD.Z = EntA->ent_anim.Pos.Z - EntB->ent_anim.Pos.Z;
	D = (float)Norme(vD);

	radius.X = EntA->Radius.X * EntA->ent_anim.Siz.X;
	radius.Y = EntA->Radius.Y * EntA->ent_anim.Siz.Y;
	radius.Z = EntA->Radius.Z * EntA->ent_anim.Siz.Z;
	A = (float)Norme(radius) * factor;

	radius.X = EntB->Radius.X * EntB->ent_anim.Siz.X;
	radius.Y = EntB->Radius.Y * EntB->ent_anim.Siz.Y;
	radius.Z = EntB->Radius.Z * EntB->ent_anim.Siz.Z;
	B = (float)Norme(radius) * factor;

	if(D < A+B)
		return true;

	return false;
}

void phy_BoundVelocity(vect_t *Vel, float MaxVel)
{
	double	Speed;
	double	NewSpeed;
	vect_t	velocity;

	velocity.X = Vel->X;
	velocity.Y = 0;
	velocity.Z = Vel->Z;

	Speed = Norme(velocity);
	if(Speed < MaxVel)
		return;

	NewSpeed = MaxVel / Speed;

	Vel->X *= (float)NewSpeed;
//	Vel->Y *= (float)NewSpeed; //
	Vel->Z *= (float)NewSpeed;
}

void phy_SlowMove(vect_t *Vel, float factor)
{
	float	Speed;
	float	NewSpeed;
	vect_t	velocity;

	velocity.X = Vel->X;
	velocity.Y = 0;
	velocity.Z = Vel->Z;

	Speed = (float)Norme(velocity);
	if(!Speed)
		return;

	NewSpeed = Speed - factor;
	if(NewSpeed<0)
		NewSpeed=0;
	NewSpeed /= Speed;

	Vel->X *= NewSpeed;
//	Vel->Y *= NewSpeed; //
	Vel->Z *= NewSpeed;
}

void phy_SlowMove_2(vect_t *Vel, float factor)
{
	float	Speed;
	float	NewSpeed;
	vect_t	velocity;

	velocity.X = Vel->X;
	velocity.Y = 0;
	velocity.Z = Vel->Z;

	Speed = (float)Norme(velocity);
	if(!Speed)
		return;

	NewSpeed = Speed - factor * 0.005f;
	if(NewSpeed<0)
		NewSpeed=0;
	NewSpeed /= Speed;

	Vel->X *= NewSpeed;
//	Vel->Y *= NewSpeed;
	Vel->Z *= NewSpeed;
}

vect_t phy_Friction(hBool JumpPossible, vect_t speedvel)
{
	float	factor;

	if(JumpPossible)
		factor = groundslow.value;
	else
		factor = airslow.value;

	phy_SlowMove(&speedvel, factor);
	return speedvel;
}

float phy_GetEntityMaxSpeed(int entid, hBool JumpPossible)
{
	float	factor;

	factor = maxspeed.value;
	if(entid == PERSO && gPlayerRun)
	{
		factor *= 1.5f;
	}
	else if(entid == ZEBAT)
	{
		factor *= 0.80f;
	}
	else if(entid == DEPLA)
	{
		factor *= 0.80f;
	}
	else if(entid == ZEMUMMY)
	{
		factor *= 0.60f;
	}
	else if(entid == MORBAK)
	{
		if(!gIsMultiplayer)
		{
			if(JumpPossible == false)
				factor *= 1.2f; // 2.0
		}
	}
	return factor;
}

vect_t phy_AdjustVelocity(int entity_id, hBool JumpPossible, vect_t speedvel)
{
	float	factor;

	factor = phy_GetEntityMaxSpeed(entity_id, JumpPossible);
	phy_BoundVelocity(&speedvel, factor);
	return speedvel;
}

pEntity phy_AdjustGravity(pEntity Ent)
{
	float	factor;

	factor = gravity.value * 0.0001f;
	Ent = Gravity(Ent, factor);
	return Ent;
}

vect_t phy_MoveVelocity(int entity_id, hBool JumpPossible, vect_t SpeedVel)
{
	SpeedVel = phy_Friction(JumpPossible, SpeedVel);					// WORLD FRICTION
	SpeedVel = phy_AdjustVelocity(entity_id, JumpPossible, SpeedVel);	// ADJUST VELOCITY
	return SpeedVel;
}

hBool phy_IsVelocity(int entid)
{
	pEntity	Ent;

	Ent = o_FindEntity(entid);
	if(!Ent)
		return false;

	if(!Ent->ent_anim.Pos_Acc.X && !Ent->ent_anim.Pos_Acc.Y && !Ent->ent_anim.Pos_Acc.Z)
		return false;
	return true;
}

void phy_ResetVelocity(int entid)
{
	pEntity	Ent;

	Ent = o_FindEntity(entid);
	if(!Ent)
		return;

	if(!Ent->ent_anim.Pos_Acc.X && !Ent->ent_anim.Pos_Acc.Y && !Ent->ent_anim.Pos_Acc.Z)
		return;

	Ent->ent_anim.Pos_Acc.X = 0;
	Ent->ent_anim.Pos_Acc.Y = 0;
	Ent->ent_anim.Pos_Acc.Z = 0;
	return;
}

void phy_UpdateVelocity(pEntity CreatureEnt)
{
	if(!CreatureEnt)
		return;

	CreatureEnt->ent_anim.Pos_Vel.X += CreatureEnt->ent_anim.Pos_Acc.X;
	//CreatureEnt->ent_anim.Pos_Vel.Y += CreatureEnt->ent_anim.Pos_Acc.Y;
	CreatureEnt->ent_anim.Pos_Vel.Z += CreatureEnt->ent_anim.Pos_Acc.Z;
}

pEntity phy_UpdateMoves(pEntity CreatureEnt)
{
	float	factor;
	float	anglefactor;
	float	bobfactor;

	if(!CreatureEnt)
		return NULL;

	if(gIsMultiplayer && !gIsServer)
	{
		if(CreatureEnt->entity_id == MORBAK)
			goto no_grav;
		goto no_physic;
	}

	if(CreatureEnt->Timing == false && CreatureEnt->entity_id == ZEBAT)
		goto no_grav;

	CreatureEnt = phy_AdjustGravity(CreatureEnt);
	if(!CreatureEnt)
		return NULL;

no_grav:

	phy_UpdateVelocity(CreatureEnt);
	CreatureEnt->ent_anim.Pos_Vel = phy_MoveVelocity(CreatureEnt->entity_id, CreatureEnt->JumpPossible, CreatureEnt->ent_anim.Pos_Vel);
	MoveCreature(CreatureEnt, CreatureEnt->ent_anim.Pos_Vel);

no_physic:

	if(developer.value)
	{
		//if(CreatureEnt->entity_id == PERSO)
		//g_DispString(14,"%f %f %f",CreatureEnt->ent_anim.Pos_Vel.X, CreatureEnt->ent_anim.Pos_Vel.Y,CreatureEnt->ent_anim.Pos_Vel.Z);
		g_DispString(14,"(%f %f %f) - (%f %f %f)", gCamera.Pos.X, gCamera.Pos.Y, gCamera.Pos.Z, gCamera.Rot.X, gCamera.Rot.Y, gCamera.Rot.Z);
	}

	// Camera_t
	if(ProgramState != PS_DIE)
	{
		switch(CreatureEnt->entity_id)
		{
			case DEPLA:
				if(gIsMultiplayer && !gIsServer)
				{
					pEntity	ServerEnt;

					ServerEnt = o_FindEntity(PERSO);
					if(!ServerEnt)
						return CreatureEnt;
					CreatureEnt->ent_anim.Pos = ServerEnt->ent_anim.Pos;
					CreatureEnt->ent_anim.Pos_Vel = ServerEnt->ent_anim.Pos_Vel;
					CreatureEnt->ent_anim.Rot = ServerEnt->ent_anim.Rot;
				}
			break;

			case MORBAK:
				if(gIsMultiplayer && !gIsServer)
				{
					gCamera.Pos.X = CreatureEnt->ent_anim.Pos.X;
					gCamera.Pos.Y = CreatureEnt->ent_anim.Pos.Y + (0.8f * CreatureEnt->Radius.Y);
					gCamera.Pos.Z = CreatureEnt->ent_anim.Pos.Z;
				
					gCamera.Rot.X = CreatureEnt->ent_anim.Rot.X;
					gCamera.Rot.Y = CreatureEnt->ent_anim.Rot.Y + 180.0f;
					gCamera.Rot.Z = CreatureEnt->ent_anim.Rot.Z;

					if(gCamera.Rot.Y > ROTMAX)
						gCamera.Rot.Y -= ROTMAX;
					else if(gCamera.Rot.Y < 0)
						gCamera.Rot.Y += ROTMAX;
				}
			break;

			case ZEBAT:
			break;

			case SUBZERAT:
			break;

			case ZEMUMMY:
			break;

			case PERSO:
				if(gIsMultiplayer && !gIsServer)
				{
					break;
				}
				if(gCameraShowTime)
					break;
				if(player_view.value)
				{
					gCamera.Pos.X = CreatureEnt->ent_anim.Pos.X;
					gCamera.Pos.Y = CreatureEnt->ent_anim.Pos.Y + (0.8f * CreatureEnt->Radius.Y);
					gCamera.Pos.Z = CreatureEnt->ent_anim.Pos.Z;
					
					gCamera.Rot.X = CreatureEnt->ent_anim.Rot.X;
					gCamera.Rot.Y = CreatureEnt->ent_anim.Rot.Y + 180.0f;
					gCamera.Rot.Z = CreatureEnt->ent_anim.Rot.Z;

					if(gCamera.Rot.Y > ROTMAX)
						gCamera.Rot.Y -= ROTMAX;
					else if(gCamera.Rot.Y < 0)
						gCamera.Rot.Y += ROTMAX;

					if(!gPlayerRun)
					{
						factor = 500.0f;
						bobfactor = 0.75f; // 1.5
						anglefactor = 0.72f; // 360/500.0f
					}
					else
					{
						factor = 300.0f;
						bobfactor = 1.0f; // 2
						anglefactor = 1.20f; // 360/300.0f
					}

					if(CreatureEnt->JumpPossible == false)
						bobfactor = 0;

					if(gPlayerWalkTime >= factor)
						gPlayerWalkTime = 0;

					gPlayerBobWalkstep = bobfactor * Sin(gPlayerWalkTime * anglefactor);
					gCamera.Pos.Y += gPlayerBobWalkstep;
				}
			break;
		}
	}
	return CreatureEnt;
}

void Launch(pEntity CreatureEnt, int id, float Speed)
{
	ANIMENTITY	anim;
	ANIMENTITY	anim2;
	float		ReloadValue;
	vect_t		ProjectileStartPos;
	vect_t		ProjectileStartVel;
	float		tromb_prec = 0.004f;

	if(!CreatureEnt)
		return;

	// CHEAT MODE
	if(gCheatMode_monsterbomb)
	{
		if(	id == TOILE ||
			id == DEPLASPIKE ||
			id == SPIKE)
		{
			switch(rand()%3)
			{
				case 0:
				id = BOMBE;
				break;

				case 1:
			//	id = SPINE;
				break;

				case 2:
			//	id = FLECHE;
				break;
			}
		}
	}

	// on annule le tir quand on est en train d'afficher le resultat du duel
	if(gIsMultiplayer && gMultiPodium)
		return;

	if(!gIsMultiplayer)
	if(game_MatrixMode && CreatureEnt->entity_id != PERSO)
		return;

	if(CreatureEnt->ReloadTime>0)
	{
		return;
	}
	else
	{
		ReloadValue = WEAPON_RELOADTIME;
	}

	if(CreatureEnt->entity_id == PERSO)
	{
		if(developer.value || terminator.value)
		{
			game_fleches = 100;
			game_bombes = 100;
			game_bullet = 100;
		}

		switch(gPlayerHasGun)
		{
			case 2:
			{
				ReloadValue = WEAPON_RELOADTIME;
				CreatureEnt->ReloadTime=ReloadValue;

				if(game_fleches==0)
				{
					ds_PlaySound(5);
					return;
				}
				else
				{
					gPlayerGunRecul = 2000.0f;
					ds_PlaySound(2);
					game_fleches--;
				}
			}
			break;

			case 3:
			{
				ReloadValue = WEAPON_RELOADTIME;
				CreatureEnt->ReloadTime=ReloadValue;

				if(game_bombes==0)
				{
					ds_PlaySound(5);
					return;
				}
				else
				{
					gPlayerGunRecul = 3600.0f;
					ds_PlaySound(13);
					game_bombes--;
				}
			}
			break;

			case 4:
			{
				ReloadValue = TROMBLON_RELOADTIME;
				CreatureEnt->ReloadTime = ReloadValue;

				if(game_bullet==0)
				{
					ds_PlaySound(5);
					return;
				}
				else
				{
					gPlayerGunRecul = 4000.0f; //2300.0f;
					ds_PlaySound(29);
					game_bullet--;
				}
			}
			break;

			default:
			return;
		}
	}

	CreatureEnt->fxTime_msec = 0;

	CreatureEnt->ReloadTime=ReloadValue;
	o_ClearAnimFields(&anim);
	if(CreatureEnt->entity_id != PERSO)
		ProjectileStartPos = CreatureEnt->ent_anim.Pos;
	else
		ProjectileStartPos = gCamera.Pos;

	if(id == DEPLASPIKE)
	{
		ProjectileStartPos.Y += 10.0f;
		anim.Rot.Y = cam_RotationCycle(CreatureEnt->ent_anim.Rot.Y + 180.0f);
	}
	else if(id == TOILE)
	{
		anim.Rot.Y = cam_RotationCycle(CreatureEnt->ent_anim.Rot.Y + 180.0f);

		anim.Siz_Vel.X = 0.02f;
		anim.Siz_Vel.Y = 0.02f;
		anim.Siz_Vel.Z = 0.02f;
	}
	else
	{
		anim.Rot.X = cam_RotationCycle(-CreatureEnt->ent_anim.Rot.Z + (float)randInt(-2,3));
		anim.Rot.Y = cam_RotationCycle(CreatureEnt->ent_anim.Rot.Y + 90.0f);
		anim.Rot.Z = cam_RotationCycle(-CreatureEnt->ent_anim.Rot.X + (float)randInt(-2,3));
	}

	ProjectileStartVel.X = Sin(CreatureEnt->ent_anim.Rot.Y) * Cos(CreatureEnt->ent_anim.Rot.X) * Speed;
	ProjectileStartVel.Y = Sin(CreatureEnt->ent_anim.Rot.X) * Speed;
	ProjectileStartVel.Z = Cos(CreatureEnt->ent_anim.Rot.Y) * Cos(CreatureEnt->ent_anim.Rot.X) * Speed;

	switch(id)
	{
		case FLECHE:
		anim.Pos = ProjectileStartPos;
		anim.Pos.X += randInt(-5,5);
		anim.Pos.Z += randInt(-5,5);
		anim.Pos_Vel = ProjectileStartVel;

		if(gIsMultiplayer)
		{
			if(gIsServer)
			{
				//net_NetLaunchEnt(id, CreatureEnt->entity_id, anim.Pos, anim.Rot, ProjectileStartVel, true);
			}
			else
			{
				//net_NetLaunchEnt(id, CreatureEnt->entity_id, anim.Pos, anim.Rot, ProjectileStartVel, false);
				break;
			}
		}

		o_AddEntityToList(id,CreatureEnt->entity_id,anim,false);
		break;

		case SPINE:
			// HACK
		if(gIsMultiplayer)
			break;

		// recul du tromblon
	//	CreatureEnt->ent_anim.Pos_Acc.X -= ProjectileStartVel.X * 5.0f;
	//	CreatureEnt->ent_anim.Pos_Acc.Z -= ProjectileStartVel.Z * 5.0f;

		// fumée de l'explosion du tromblon
		if(game_MatrixMode == false)
		{
			o_ClearAnimFields(&anim2);
			anim2.Pos.X = ProjectileStartPos.X + ProjectileStartVel.X * 13;
			anim2.Pos.Y = ProjectileStartPos.Y + ProjectileStartVel.Y * 13;
			anim2.Pos.Z = ProjectileStartPos.Z + ProjectileStartVel.Z * 13;
			anim2.Pos_Vel.Y = 0.0018f;
			anim2.Siz.X = 6;
			anim2.Siz.Y = 6;
			anim2.Siz.Z = 6;
			anim2.Siz_Vel.X = 0.0014f;
			anim2.Siz_Vel.Y = 0.0014f;
			anim2.Siz_Vel.Z = 0.0014f;
		//	anim2.Rot.X = cam_RotationCycle( -CreatureEnt->ent_anim.Rot.X );
		//	anim2.Rot.Y = CreatureEnt->ent_anim.Rot.Y;
		//	anim2.Rot.Z = 0;
			o_AddEntityToList(SMOKE,-1,anim2,false);
		}

		anim.Pos = ProjectileStartPos;

		ProjectileStartVel.X *= 0.75f;
		ProjectileStartVel.Y *= 0.75f;
		ProjectileStartVel.Z *= 0.75f;

		anim.Pos_Vel = ProjectileStartVel;
		anim.Pos_Vel.X += (float)randInt(-20,20) * tromb_prec;
		anim.Pos_Vel.Y += (float)randInt(-20,20) * tromb_prec;
		anim.Pos_Vel.Z += (float)randInt(-20,20) * tromb_prec;
		o_AddEntityToList(id,CreatureEnt->entity_id,anim,false);

		anim.Pos_Vel = ProjectileStartVel;
		anim.Pos_Vel.X += (float)randInt(-20,20) * tromb_prec;
		anim.Pos_Vel.Y += (float)randInt(-20,20) * tromb_prec;
		anim.Pos_Vel.Z += (float)randInt(-20,20) * tromb_prec;
		o_AddEntityToList(id,CreatureEnt->entity_id,anim,false);

		anim.Pos_Vel = ProjectileStartVel;
		anim.Pos_Vel.X += (float)randInt(-20,20) * tromb_prec;
		anim.Pos_Vel.Y += (float)randInt(-20,20) * tromb_prec;
		anim.Pos_Vel.Z += (float)randInt(-20,20) * tromb_prec;
		o_AddEntityToList(id,CreatureEnt->entity_id,anim,false);

		anim.Pos_Vel = ProjectileStartVel;
		anim.Pos_Vel.X += (float)randInt(-20,20) * tromb_prec;
		anim.Pos_Vel.Y += (float)randInt(-20,20) * tromb_prec;
		anim.Pos_Vel.Z += (float)randInt(-20,20) * tromb_prec;
		o_AddEntityToList(id,CreatureEnt->entity_id,anim,false);

		anim.Pos_Vel = ProjectileStartVel;
		anim.Pos_Vel.X += (float)randInt(-20,20) * tromb_prec;
		anim.Pos_Vel.Y += (float)randInt(-20,20) * tromb_prec;
		anim.Pos_Vel.Z += (float)randInt(-20,20) * tromb_prec;
		o_AddEntityToList(id,CreatureEnt->entity_id,anim,false);

		anim.Pos_Vel = ProjectileStartVel;
		anim.Pos_Vel.X += (float)randInt(-20,20) * tromb_prec;
		anim.Pos_Vel.Y += (float)randInt(-20,20) * tromb_prec;
		anim.Pos_Vel.Z += (float)randInt(-20,20) * tromb_prec;
		o_AddEntityToList(id,CreatureEnt->entity_id,anim,false);

		anim.Pos_Vel = ProjectileStartVel;
		anim.Pos_Vel.X += (float)randInt(-20,20) * tromb_prec;
		anim.Pos_Vel.Y += (float)randInt(-20,20) * tromb_prec;
		anim.Pos_Vel.Z += (float)randInt(-20,20) * tromb_prec;
		o_AddEntityToList(id,CreatureEnt->entity_id,anim,false);

		anim.Pos_Vel = ProjectileStartVel;
		anim.Pos_Vel.X += (float)randInt(-20,20) * tromb_prec;
		anim.Pos_Vel.Y += (float)randInt(-20,20) * tromb_prec;
		anim.Pos_Vel.Z += (float)randInt(-20,20) * tromb_prec;
		o_AddEntityToList(id,CreatureEnt->entity_id,anim,false);
		break;

		default:
		anim.Pos = ProjectileStartPos;
		anim.Pos_Vel = ProjectileStartVel;

		if(gIsMultiplayer)
		{
			if(gIsServer)
			{
				//net_NetLaunchEnt(id, CreatureEnt->entity_id, anim.Pos, anim.Rot, ProjectileStartVel, true);
			}
			else
			{
				//net_NetLaunchEnt(id, CreatureEnt->entity_id, anim.Pos, anim.Rot, ProjectileStartVel, false);
				break;
			}
		}

		o_AddEntityToList(id,CreatureEnt->entity_id,anim,false);
		break;
	}
}

void JumpEntity(pEntity Entity)
{
	if(!Entity)
		return;
	if(!Entity->JumpPossible)
		return;

	Entity->ent_anim.Pos_Vel.Y = jumpstep.value;

	if(Entity->entity_id == PERSO)
	{
		ds_PlaySound(9);
	}
}

void JumpCreature(int id)
{
	pEntity		CreatureEnt;

	CreatureEnt = o_FindEntity(id);
	JumpEntity(CreatureEnt);
}

void MoveEntity_Forward(pEntity Entity)
{
	float	factor;
	vect_t	acceleration;

	if(!Entity)
		return;

//	if(Entity->JumpPossible)
//		ds_PlaySound3D(22,Entity->ent_anim.Pos,Entity->ent_anim.Pos_Vel);

	if(Entity->JumpPossible)
	{
		factor = groundacc.value;
	}
	else
	{
		factor = airacc.value;
	}

	factor = factor * gFrameTime_msec * 0.6f;

	acceleration.X = Sin(Entity->ent_anim.Rot.Y) * factor;
	acceleration.Y = 0;
	acceleration.Z = Cos(Entity->ent_anim.Rot.Y) * factor;
	

	Entity->ent_anim.Pos_Vel.X += acceleration.X;
	Entity->ent_anim.Pos_Vel.Y += acceleration.Y;
	Entity->ent_anim.Pos_Vel.Z += acceleration.Z;
}

void MoveCreature(pEntity CreatureEnt, vect_t MoveStep)
{
	vect_t	Dest;
	vect_t	v;
	CTRACE	ClipTrace;
	float	val;

	if(!CreatureEnt)
		return;

	if(MoveStep.X == 0 && MoveStep.Z == 0)
		return;

	MoveStep.X *= gFrameTime_msec;
	MoveStep.Y *= gFrameTime_msec;
	MoveStep.Z *= gFrameTime_msec;

	Dest.X = CreatureEnt->ent_anim.Pos.X + MoveStep.X;
	Dest.Y = CreatureEnt->ent_anim.Pos.Y ;//+ MoveStep.Y;
	Dest.Z = CreatureEnt->ent_anim.Pos.Z + MoveStep.Z;
        

	ClipTrace.Response = true;
	ClipTrace.NoRadius = false;
	ClipTrace.DontUnstuck = false;
	ClipTrace.ellipsoid = CreatureEnt->Radius;
	ClipTrace.Ent = CreatureEnt;
	ClipTrace.IsPlan = false;

	// hack
	if(gMapIntro && CreatureEnt->entity_id == ZEBAT)
	{
		CreatureEnt->ent_anim.Pos = Dest;
		goto no_col;
	}

	col_Handle(&CreatureEnt->ent_anim.Pos,Dest,&ClipTrace);
	CreatureEnt->SolidSpace = ClipTrace.solidspace;
        
no_col:
        
	if(phy_BoundPos(&CreatureEnt->ent_anim.Pos))
	{
		CreatureEnt->ent_anim.Rot.Y += 20;
		CreatureEnt->ent_anim.Rot.Y = cam_RotationCycle(CreatureEnt->ent_anim.Rot.Y);
	}

	if(	CreatureEnt->entity_id == DEPLA		||
		CreatureEnt->entity_id == ZEBAT		||
		CreatureEnt->entity_id == MORBAK	||
		CreatureEnt->entity_id == SUBZERAT	||
		CreatureEnt->entity_id == ZEMUMMY)
	{
		// HACK : pas de feinte d'IA des monstres en mode multiplayer
		if(gIsMultiplayer)
			return;

		if(ClipTrace.IsPlan)
		{
			v.Y = 0;
			v.Z = Cos(CreatureEnt->ent_anim.Rot.Y);
			v.X = Sin(CreatureEnt->ent_anim.Rot.Y);

			val = PdtScalaire(&ClipTrace.Plan.VecteurNormal,&v);

			if(val < 0)
			{
				v.X = ClipTrace.impact.X - CreatureEnt->ent_anim.Pos.X;
				v.Y = ClipTrace.impact.Y - CreatureEnt->ent_anim.Pos.Y;
				v.Z = ClipTrace.impact.Z - CreatureEnt->ent_anim.Pos.Z;
		
				val = 1.0f - PdtScalaire(&ClipTrace.Plan.VecteurNormal,&v);
				val *= 90.0f;

				//CreatureEnt->IA_wallAngle = (1.0f - PdtScalaire(&ClipTrace.Plan.VecteurNormal,&v)) * 90.0f;
				CreatureEnt->ent_anim.Rot.Y += val * 0.5f;
			}
		}
	}
}

int IsGround_UnderCreature(pEntity CreatureEnt, float factor)
{
	CTRACE	ctrace;
	vect_t	Pos;
	vect_t	Dest;

	if(!CreatureEnt)
		return false;

	if(CreatureEnt->ent_anim.Pos_Vel.Y == 0)
	{
		Pos = CreatureEnt->ent_anim.Pos;

		Dest = Pos;
		Dest.Y = Pos.Y - 5.0f; // 10

		ctrace.Response = false;
		ctrace.NoRadius = false;
		ctrace.DontUnstuck = true;
		ctrace.Ent = CreatureEnt;
		ctrace.ellipsoid = CreatureEnt->Radius;
		col_Handle(&Pos,Dest,&ctrace);
		CreatureEnt->SolidSpace = ctrace.solidspace;
		if(ctrace.col)
		{
			if(ctrace.type == 0)
				return JOKER;
			else
				return ctrace.type;
		}
	}

	CreatureEnt->ent_anim.Pos_Acc.Y = -(factor * gFrameTime_msec); // hack
	CreatureEnt->ent_anim.Pos_Vel.Y -= (factor * gFrameTime_msec);

	Pos = CreatureEnt->ent_anim.Pos;
	Dest = Pos;
	Dest.Y = Pos.Y + (CreatureEnt->ent_anim.Pos_Vel.Y * gFrameTime_msec);

	ctrace.Response = false;
	ctrace.NoRadius = false;
	ctrace.DontUnstuck = false;
	ctrace.ellipsoid = CreatureEnt->Radius;
	ctrace.Ent = CreatureEnt;
	col_Handle(&Pos,Dest,&ctrace);
	CreatureEnt->SolidSpace = ctrace.solidspace;
	if(ctrace.col)
	{
		if(CreatureEnt->entity_id == PERSO)
		if(CreatureEnt->ent_anim.Pos_Vel.Y < -0.5f)
		{
			PlayerHurt(CreatureEnt, (int)((-CreatureEnt->ent_anim.Pos_Vel.Y-0.5f) * 80.0f));
			if(game_heart <= 0)
			{
				ds_PlaySound3D(53, CreatureEnt->ent_anim.Pos, CreatureEnt->ent_anim.Pos);
			}
		}

		CreatureEnt->ent_anim.Pos_Vel.Y = 0;
	}
		
	CreatureEnt->ent_anim.Pos = ctrace.impact;

	if(ctrace.col)
	{
		if(ctrace.type == 0)
			return JOKER;
		else
			return ctrace.type;
	}
	return false;
}

hBool IsGround2_UpCreature(pEntity CreatureEnt)
{
	vect_t	Pos;
	vect_t	Dest;
	CTRACE	ctrace;

	if(!CreatureEnt)
		return false;

	Pos = CreatureEnt->ent_anim.Pos;
	Dest = Pos;
	Dest.Y = Pos.Y + 5.0f;
	
	ctrace.Response = false;
	ctrace.NoRadius = false;
	ctrace.DontUnstuck = true;
	ctrace.ellipsoid = CreatureEnt->Radius;
	ctrace.Ent = CreatureEnt;
	col_Handle(&Pos,Dest,&ctrace);
	if(ctrace.col)
		return true;
	else
		return false;
}

hBool IsGround_UpCreature(pEntity CreatureEnt)
{
	vect_t	Pos;
	vect_t	Dest;
	CTRACE	ctrace;

	if(!CreatureEnt)
		return false;
	if(CreatureEnt->ent_anim.Pos_Vel.Y <= 0)
		return false;

	Pos = CreatureEnt->ent_anim.Pos;

	Dest.X = Pos.X;
	Dest.Y = Pos.Y + CreatureEnt->ent_anim.Pos_Vel.Y * gFrameTime_msec;
	Dest.Z = Pos.Z;
	
	ctrace.Response = false;
	ctrace.NoRadius = false;
//	if(CreatureEnt->entity_id == PERSO)
		ctrace.DontUnstuck = false;
//	else
//		ctrace.DontUnstuck = true;
	ctrace.ellipsoid = CreatureEnt->Radius;
	ctrace.Ent = CreatureEnt;
	col_Handle(&Pos,Dest,&ctrace);
	CreatureEnt->SolidSpace = ctrace.solidspace;
	if(ctrace.col)
		return true;
	else
		return false;
}

hBool IsEntity_UnderCreature(pEntity CreatureEnt, pEntity Ent)
{
	return false;
}

hBool IsEntity_UpperCreature(pEntity CreatureEnt, pEntity Ent)
{
	return false;
}


