
#include "alloween.cpp"


pEntity ExplosionProcess(pEntity ExplosionEnt)
{
	if(!ExplosionEnt)
		return NULL;
	if(ExplosionEnt->entity_id != EXPLOSION)
		return ExplosionEnt;

	if(!ExplosionEnt->Timing)
	{
		ExplosionEnt->Blending=true;
		ExplosionEnt->Timing=true;
		ExplosionEnt->Time_msec=1000.0f;
	}
	ExplosionEnt->Time_msec -= gFrameTime_msec;
	ExplosionEnt->BlendVal = ExplosionEnt->Time_msec/1000.0f;
	if(ExplosionEnt->Time_msec<=0)
	{
		o_RemoveEntityFromList(ExplosionEnt);
		return NULL;
	}
	return ExplosionEnt;
}

pEntity BombProcess(pEntity BombEnt, pEntity PlayerEnt)
{
	float		factor;
	ANIMENTITY	*panim;
	ANIMENTITY	anim;
	vect_t		Start, Dest;
	float		Distance;
	CTRACE		ctrace;
	pEntity		pEnt;
	vect_t		v;
	float		bounce_fact;
	hBool		stop_bnc;

	if(!BombEnt)
		return NULL;
	if(BombEnt->entity_id != BOMBE)
		return BombEnt;

	panim = &(BombEnt->ent_anim);

	if(!BombEnt->Timing)
	{
		BombEnt->Timing=true;
		BombEnt->Time_msec=3000.0f;
		BombEnt->fxTime_msec = 0;
	}

	if( BombEnt->IsBounce )
	{
		BombEnt->fxTime_msec += gFrameTime_msec;
		if(BombEnt->fxTime_msec >= 80.0f)
		{
			BombEnt->fxTime_msec = 0;
			o_ClearAnimFields(&anim);
			anim.Pos = BombEnt->ent_anim.Pos;
			anim.Siz.X = 0;
			anim.Siz.Y = 0;
			anim.Siz.Z = 0;
			anim.Siz_Vel.X = 0.02f;
			anim.Siz_Vel.Y = 0.02f;
			anim.Siz_Vel.Z = 0.02f;
			o_AddEntityToList(SMOKE,-1,anim,false);
		}
	}

	BombEnt->Time_msec -= gFrameTime_msec;
	if(BombEnt->Time_msec<=0)
	{
		// HACK !!!!!!!!!!!!!!!!!!!!!!
		//if(gIsMultiplayer)
		//	goto no_souffle;

		if(gIsMultiplayer)
		{
			if(gIsServer)
			{
				//net_RemoveClientEnt(BombEnt);
				// ajout de l'explosion

			}
			else
			{
				BombEnt->Time_msec = 1;
				return BombEnt;
			}
		}

		// souffle de l'explosion sur les entitées
		for(pEnt=gEntitiesList ; pEnt ; pEnt=pEnt->Next)
		{
			if(pEnt->entity_id == PERSO		||
				pEnt->entity_id == DEPLA	||
				pEnt->entity_id == MORBAK	||
				pEnt->entity_id == ZEBAT	||
				pEnt->entity_id == SUBZERAT	||
				pEnt->entity_id == ZEMUMMY)
			{
				Distance = m_Distance(pEnt->ent_anim.Pos,BombEnt->ent_anim.Pos);
				if(Distance < 150.0f && Distance > 0.001f)
				{
					v.X = pEnt->ent_anim.Pos.X - BombEnt->ent_anim.Pos.X;
					v.Y = pEnt->ent_anim.Pos.Y - BombEnt->ent_anim.Pos.Y;
					v.Z = pEnt->ent_anim.Pos.Z - BombEnt->ent_anim.Pos.Z;

					m_Vector_Scale(&v,2000.0f/Distance);

					if(pEnt->entity_id != ZEBAT)
					{
						v.Y = 0;
						if(pEnt->entity_id == PERSO)
							v.Y = pEnt->ent_anim.Pos_Vel.Y;

						if(v.Y >= 0)
							v.Y += jumpstep.value * (1.5f - Distance/150.0f);
						else
							v.Y += -jumpstep.value * (1.5f - Distance/150.0f);
					}
					else
					{
						v.X = v.Y = v.Z = 0;
					}

					pEnt->ent_anim.Pos_Vel.X += v.X;
					pEnt->ent_anim.Pos_Vel.Y += v.Y;
					pEnt->ent_anim.Pos_Vel.Z += v.Z;
				}
			}
		}

//no_souffle:

		Distance = m_Distance(BombEnt->ent_anim.Pos,PlayerEnt->ent_anim.Pos);
		Distance *= 0.01f;
		cam_EarthQuake_Launch(1.0f/(Distance+1.0f));

		fx_Particule(BombEnt->ent_anim.Pos,30,GLYELLOW);

		o_ClearAnimFields(&anim);
		anim.Pos = BombEnt->ent_anim.Pos;
		anim.Siz_Vel.X = 0.04f;
		anim.Siz_Vel.Y = 0.04f;
		anim.Siz_Vel.Z = 0.04f;
		anim.Rot.Y = PlayerEnt->ent_anim.Rot.Y + 90.0f;
		o_AddEntityToList(EXPLOSION,BombEnt->parent_id,anim,false);
		ds_PlaySound3D(14,BombEnt->ent_anim.Pos,BombEnt->ent_anim.Pos_Vel);

		o_RemoveEntityFromList(BombEnt);
		return NULL;
	}

	ab_OnEventActivate(BombEnt->ent_anim.Pos,3);	// hack

	//	GRAVITY
	factor = gravity.value * gFrameTime_msec * 0.00005f;
	BombEnt->ent_anim.Pos_Vel.Y -= factor;

	//	FRICTION
	phy_SlowMove_2(&BombEnt->ent_anim.Pos_Vel,bombslow.value);

	//	BOUNCE
	Start = BombEnt->ent_anim.Pos;
	Dest.X = Start.X + BombEnt->ent_anim.Pos_Vel.X * gFrameTime_msec;
	Dest.Y = Start.Y + BombEnt->ent_anim.Pos_Vel.Y * gFrameTime_msec;
	Dest.Z = Start.Z + BombEnt->ent_anim.Pos_Vel.Z * gFrameTime_msec;

	HMove(&Start,&Dest,&ctrace);

	if(BombEnt->IsBounce)
		bounce_fact = 1.7f;
	else
		bounce_fact = 1.0f;

	if(ctrace.col)
	{
		if(ctrace.type == LAVA)
		{
			if(gIsMultiplayer && !gIsServer)
				goto cl_noremove_ent;

			fx_Particule(BombEnt->ent_anim.Pos, 10,GLYELLOW);
			o_RemoveEntityFromList(BombEnt);
			return NULL;
		}

cl_noremove_ent:

		if( BombEnt->IsBounce )
			BombEnt->ent_anim.Pos = ctrace.impact;
		else
		{
			// HACK : pour stopper les oscillations infimes en Y
			BombEnt->ent_anim.Pos.X = ctrace.impact.X;
			BombEnt->ent_anim.Pos.Z = ctrace.impact.Z;
		}

		if( BombEnt->IsBounce && BombEnt->ent_anim.Pos_Vel.Y < 0 && BombEnt->ent_anim.Pos_Vel.Y >= -0.08f)
			stop_bnc = true;
		else
			stop_bnc = false;

		BombEnt->ent_anim.Pos_Vel = VectorBounce(&BombEnt->ent_anim.Pos_Vel,ctrace.Plan.VecteurNormal,bounce_fact);
		
		if( stop_bnc && BombEnt->ent_anim.Pos_Vel.Y > 0 && BombEnt->ent_anim.Pos_Vel.Y <= 0.08f)
		{
			BombEnt->IsBounce = false;
			BombEnt->ent_anim.Pos_Vel.Y = 0;
		}
		else
		{
			if( BombEnt->IsBounce )
			{
				//fx_Particule(BombEnt->ent_anim.Pos,12,GLRED);
				ds_PlaySound3D(12,BombEnt->ent_anim.Pos,BombEnt->ent_anim.Pos_Vel);
			}
			else
			{
				if( BombEnt->ent_anim.Pos_Vel.Y > 0 )
					BombEnt->ent_anim.Pos_Vel.Y = 0;
			}
		}
	}
	else
		BombEnt->ent_anim.Pos = Dest;

	if(!gIsMultiplayer || gIsServer)
	{
		if(MapDataFormat == MAP_BSP)
		if( bsp_FindCamera(BombEnt->ent_anim.Pos) < 0)
		{
			m_ConsPrint("bomb entity in solid space error\n");
			o_RemoveEntityFromList(BombEnt);
			return NULL;
		}
	}

	return BombEnt;
}

pEntity GibProcess(pEntity GibEnt, pEntity PlayerEnt)
{
	float		factor;
	ANIMENTITY	*panim;
	ANIMENTITY	anim;
	vect_t		Start, Dest;
	CTRACE		ctrace;
	float		bounce_fact;
	hBool		stop_bnc;

	if(!GibEnt)
		return NULL;
	if(GibEnt->entity_id != DEPLAHEAD && GibEnt->entity_id != MORBAKBODY)
		return GibEnt;

	panim = &(GibEnt->ent_anim);

	if(!GibEnt->Timing)
	{
		GibEnt->Timing=true;
		GibEnt->Time_msec=5000.0f;
		GibEnt->fxTime_msec = 0;
	}

	if( GibEnt->IsBounce )
	{
		GibEnt->fxTime_msec += gFrameTime_msec;
		if(GibEnt->fxTime_msec >= 80.0f)
		{
			GibEnt->fxTime_msec = 0;
		
			o_ClearAnimFields(&anim);
			anim.Pos = GibEnt->ent_anim.Pos;
			anim.Siz.X = 0;
			anim.Siz.Y = 0;
			anim.Siz.Z = 0;
			anim.Siz_Vel.X = 0.02f;
			anim.Siz_Vel.Y = 0.02f;
			anim.Siz_Vel.Z = 0.02f;
			o_AddEntityToList(BLOOD,-1,anim,false);	
		}
	}

	GibEnt->Time_msec -= gFrameTime_msec;
	if(GibEnt->Time_msec<=0)
	{
		o_RemoveEntityFromList(GibEnt);
		return NULL;
	}

	//	GRAVITY
	factor = gravity.value * gFrameTime_msec * 0.00005f;
	GibEnt->ent_anim.Pos_Vel.Y -= factor;

	// FROTTEMENTS
	phy_SlowMove_2(&GibEnt->ent_anim.Pos_Vel,bombslow.value);

	//	BOUNCE
	Start = GibEnt->ent_anim.Pos;
	Dest.X = Start.X + GibEnt->ent_anim.Pos_Vel.X * gFrameTime_msec;
	Dest.Y = Start.Y + GibEnt->ent_anim.Pos_Vel.Y * gFrameTime_msec;
	Dest.Z = Start.Z + GibEnt->ent_anim.Pos_Vel.Z * gFrameTime_msec;

	HMove(&Start,&Dest,&ctrace);

	if(GibEnt->IsBounce)
		bounce_fact = 1.65f;
	else
		bounce_fact = 1.0f;

	if(ctrace.col)
	{
		if(ctrace.type == LAVA)
		{
			fx_Particule(GibEnt->ent_anim.Pos, 10,GLYELLOW);
			o_RemoveEntityFromList(GibEnt);
			return NULL;
		}

		if( GibEnt->IsBounce )
			GibEnt->ent_anim.Pos = ctrace.impact;
		else
		{
			// HACK : pour stopper les oscillations infimes en Y
			GibEnt->ent_anim.Pos.X = ctrace.impact.X;
			GibEnt->ent_anim.Pos.Z = ctrace.impact.Z;
		}

		if( GibEnt->IsBounce && GibEnt->ent_anim.Pos_Vel.Y < 0 && GibEnt->ent_anim.Pos_Vel.Y >= -0.08f)
			stop_bnc = true;
		else
			stop_bnc = false;

		GibEnt->ent_anim.Pos_Vel = VectorBounce(&GibEnt->ent_anim.Pos_Vel,ctrace.Plan.VecteurNormal,bounce_fact);
	
		if( stop_bnc && GibEnt->ent_anim.Pos_Vel.Y >= 0 && GibEnt->ent_anim.Pos_Vel.Y <= 0.08f)
		{
			GibEnt->IsBounce = false;
			GibEnt->ent_anim.Pos_Vel.Y = 0;
		}
		else
		{
			if( GibEnt->IsBounce )
			{
				fx_Particule(GibEnt->ent_anim.Pos,12,GLRED);
				ds_PlaySound3D(54,GibEnt->ent_anim.Pos,GibEnt->ent_anim.Pos_Vel);

				// ROTATION
			//	GibEnt->ent_anim.Rot_Vel.X = randInt(0,100) * 0.1f;
			//	GibEnt->ent_anim.Rot_Vel.Y = randInt(0,100) * 0.1f;
			//	GibEnt->ent_anim.Rot_Vel.Z = randInt(0,100) * 0.1f;
			}
			else
			{
				if( GibEnt->ent_anim.Pos_Vel.Y > 0 )
					GibEnt->ent_anim.Pos_Vel.Y = 0;
			}
		}
	}
	else
		GibEnt->ent_anim.Pos = Dest;

	if(MapDataFormat == MAP_BSP)
	if( bsp_FindCamera(GibEnt->ent_anim.Pos) < 0)
	{
		m_ConsPrint("gib entity in solid space error\n");
		o_RemoveEntityFromList(GibEnt);
		return NULL;
	}

	return GibEnt;
}

pEntity SabreProcess(pEntity SabreEnt)
{
	if(!SabreEnt)
		return NULL;
	if(SabreEnt->entity_id != SABRE)
		return SabreEnt;

	if(!SabreEnt->Timing)
		return SabreEnt;
	SabreEnt->Time_msec -= gFrameTime_msec;
	if(SabreEnt->Time_msec<=0)
	{
		SabreEnt->Time_msec=0;
		SabreEnt->Timing=false;
		SabreStop();
	}
	else if(SabreEnt->Time_msec < 75.0f)
	{
		SabreEnt->ent_anim.Rot.Y = 53.0f;
		SabreEnt->ent_anim.Rot.X = 0.0f;
	}
	else if(SabreEnt->Time_msec < 100.0f)
	{
		SabreEnt->ent_anim.Rot.Y = 52.0f;
		SabreEnt->ent_anim.Rot.X = 5.0f;
	}
	else if(SabreEnt->Time_msec < 125.0f)
	{
		SabreEnt->ent_anim.Rot.Y = 50.0f;
		SabreEnt->ent_anim.Rot.X = 10.0f;
	}
	else if(SabreEnt->Time_msec < 150.0f)
	{
		SabreEnt->ent_anim.Rot.Y = 30.0f;
		SabreEnt->ent_anim.Rot.X = 15.0f;
	}
	else if(SabreEnt->Time_msec < 175.0f)
	{
		SabreEnt->ent_anim.Rot.Y = 15.0f;
		SabreEnt->ent_anim.Rot.X = 20.0f;
	}
	else if(SabreEnt->Time_msec < 200.0f)
	{
		SabreEnt->ent_anim.Rot.Y = 0.0f;
		SabreEnt->ent_anim.Rot.X = 25.0f;
	}
	else if(SabreEnt->Time_msec < 225.0f)
	{
		SabreEnt->ent_anim.Rot.Y = -10.0f;
		SabreEnt->ent_anim.Rot.X = 30.0f;
	}
	else if(SabreEnt->Time_msec < 250.0f)
	{
		SabreEnt->ent_anim.Rot.Y = -20.0f;
		SabreEnt->ent_anim.Rot.X = 35.0f;
	}
	else if(SabreEnt->Time_msec < 275.0f)
	{
		SabreEnt->ent_anim.Rot.Y = -30.0f;
		SabreEnt->ent_anim.Rot.X = 40.0f;
	}
	else if(SabreEnt->Time_msec < 300.0f)
	{
		SabreEnt->ent_anim.Rot.Y = -35.0f;
		SabreEnt->ent_anim.Rot.X = 45.0f;
	}
	else if(SabreEnt->Time_msec < 400.0f)
	{
		SabreEnt->ent_anim.Rot.Y = -40.0f;
		SabreEnt->ent_anim.Rot.X = 50.0f;
	}
	return SabreEnt;
}

pEntity AmmoProcess(pEntity AmmoEnt)
{
	float	factor;

	if(!AmmoEnt)
		return NULL;
	if(AmmoEnt->player_holded)
		return AmmoEnt;
	if(AmmoEnt->entity_id != AMO1 && 
		AmmoEnt->entity_id != AMO2 &&
		AmmoEnt->entity_id != AMO3  &&
		AmmoEnt->entity_id != GUN1 &&
		AmmoEnt->entity_id != GUN2 &&
		AmmoEnt->entity_id != GUN3)
		return AmmoEnt;

	if(AmmoEnt->IsGravity)
	{
		factor = gravity.value * gFrameTime_msec * 0.000001f;
		AmmoEnt = Gravity(AmmoEnt,factor);
	}
	return AmmoEnt;
}

pEntity PumpkinProcess(pEntity PumpkinEnt)
{
	float	factor;

	if(!PumpkinEnt)
		return NULL;
	if(PumpkinEnt->entity_id != PUMPKIN)
		return PumpkinEnt;
	if(PumpkinEnt->player_holded)
		return PumpkinEnt;

	if(PumpkinEnt->IsGravity)
	{
		factor = gravity.value * 0.000001f;
		PumpkinEnt = Gravity(PumpkinEnt,factor);
	}

	return PumpkinEnt;
}

pEntity GunProcess(pEntity GunEnt)
{
	if(!GunEnt)
		return NULL;
	if( GunEnt->entity_id != PERSO		&&
		GunEnt->entity_id != DEPLA		&&
		GunEnt->entity_id != MORBAK		&&
		GunEnt->entity_id != ZEBAT		&&
		GunEnt->entity_id != SUBZERAT	&&
		GunEnt->entity_id != ZEMUMMY )
	return GunEnt;

	if(GunEnt->entity_id == PERSO)
	if(gPlayerGunRecul)
	{
		gPlayerGunRecul -= gFrameTime_msec * 8.0f;
		if(gPlayerGunRecul < 0)
			gPlayerGunRecul = 0;
	}

	if(GunEnt->ReloadTime)
	{
		GunEnt->ReloadTime -= gFrameTime_msec;
		if(GunEnt->ReloadTime<0)
			GunEnt->ReloadTime=0;
	}
	return GunEnt;
}

pEntity FlecheProcess(pEntity PlayerEnt, pEntity FlecheEnt)
{
	vect_t		Start, Dest;
	CTRACE		ctrace;
//	ANIMENTITY	anim;

	if(!FlecheEnt)
		return NULL;

	if(	FlecheEnt->entity_id != FLECHE
		&& FlecheEnt->entity_id != DEPLASPIKE
		&& FlecheEnt->entity_id != SPIKE
		&& FlecheEnt->entity_id != SPINE
		&& FlecheEnt->entity_id != TOILE
		)
		return FlecheEnt;

	// PROPRIETEES PROPRES A L'ENTITEE LANCEE
	switch(FlecheEnt->entity_id)
	{
		case FLECHE:
		// GRAVITEE SUR LA FLECHE
	//	if(!FlecheEnt->Timing)
	//		Gravity(FlecheEnt, gravity.value * 0.00002f);

		ab_FlecheActivate(FlecheEnt->ent_anim.Pos);	// If a fleche activate a button (look at scripts)
		break;

		case TOILE:
		FlecheEnt->ent_anim.Siz.X += (FlecheEnt->ent_anim.Siz_Vel.X * gFrameTime_msec);
		FlecheEnt->ent_anim.Siz.Y += (FlecheEnt->ent_anim.Siz_Vel.Y * gFrameTime_msec);
		FlecheEnt->ent_anim.Siz.Z += (FlecheEnt->ent_anim.Siz_Vel.Z * gFrameTime_msec);

		FlecheEnt->ent_anim.Rot.X = cam_RotationCycle( -PlayerEnt->ent_anim.Rot.X );
		FlecheEnt->ent_anim.Rot.Y = PlayerEnt->ent_anim.Rot.Y;
		FlecheEnt->ent_anim.Rot.Z = 0;
		break;
	}

	if( FlecheEnt->entity_id == TOILE)
	{
		// CAS SPECIAL DE LA TOILE DE MORBAK
		if(!FlecheEnt->Timing)
		{
			FlecheEnt->Timing = true;
			FlecheEnt->Time_msec = 1100.0f;
			FlecheEnt->Blending = true;
		}
		FlecheEnt->Time_msec -= gFrameTime_msec;
		if(FlecheEnt->Time_msec <= 0)
		{
			o_RemoveEntityFromList(FlecheEnt);
			return NULL;
		}
		FlecheEnt->BlendVal = FlecheEnt->Time_msec/3000.0f;
	}
	else
	{
		// GERE LA DISPARITION DES FLECHES PLANTEES DANS LES MURS
  		if(FlecheEnt->Timing)
		{
			FlecheEnt->Time_msec -= gFrameTime_msec;
			if(FlecheEnt->Time_msec < 0)
			{
				o_RemoveEntityFromList(FlecheEnt);
				return NULL;
			}
			if(FlecheEnt->Time_msec <= 1000.0f)
			{
				FlecheEnt->Blending=true;
				FlecheEnt->BlendVal = FlecheEnt->Time_msec/1000.0f;
			}
			return FlecheEnt;
		}

		/*
		// ADD SMOKE ALONG TRAJECTORY
		FlecheEnt->fxTime_msec += gFrameTime_msec;
		if(FlecheEnt->fxTime_msec >= 10.0f)
		{
			FlecheEnt->fxTime_msec = 0;
			if(FlecheEnt->parent_id != PERSO || !game_MatrixMode)
			{
				o_ClearAnimFields(&anim);
				anim.Pos = FlecheEnt->ent_anim.Pos;
				anim.Siz.X = 0.4f;
				anim.Siz.Y = 0.4f;
				anim.Siz.Z = 0.4f;
				anim.Pos_Vel.Y = 0.001f; // 0.005
				o_AddEntityToList(SMOKE,-1,anim,false);
			}
		}
		*/
	}

	// ON DEPLACE LA FLECHE ET ON GERE LES COLLISIONS
	Start = FlecheEnt->ent_anim.Pos;
	Dest.X = Start.X + (FlecheEnt->ent_anim.Pos_Vel.X * gFrameTime_msec);
	Dest.Y = Start.Y + (FlecheEnt->ent_anim.Pos_Vel.Y * gFrameTime_msec);
	Dest.Z = Start.Z + (FlecheEnt->ent_anim.Pos_Vel.Z * gFrameTime_msec);

	HMove(&Start,&Dest,&ctrace);

	if(!ctrace.col)	
	{
		FlecheEnt->ent_anim.Pos = Dest;
	}
	else
	{
		ab_OnEventActivate(FlecheEnt->ent_anim.Pos,3);	// hack

		if(FlecheEnt->entity_id == TOILE)
		{
			o_RemoveEntityFromList(FlecheEnt);
			return NULL;
		}

		if(ctrace.type == LAVA)
		{
			fx_Particule(FlecheEnt->ent_anim.Pos, 4,GLYELLOW);
			o_RemoveEntityFromList(FlecheEnt);
			return NULL;
		}
		else if(ctrace.type == ACTIONBRUSH)
		{
			//ds_PlaySound3D(18,FlecheEnt->ent_anim.Pos,FlecheEnt->ent_anim.Pos);
			// HACK HACK HACK
			FlecheEnt->Timing = true;
			FlecheEnt->Time_msec = 1000.0f;
			FlecheEnt->ent_anim.Pos = ctrace.impact;
		}
		else
		{
		//	if(FlecheEnt->parent_id == PERSO)
		//	{
				if(randInt(0,100) % 2 == 0)
					ds_PlaySound3D(61,FlecheEnt->ent_anim.Pos,FlecheEnt->ent_anim.Pos);
				else
					ds_PlaySound3D(62,FlecheEnt->ent_anim.Pos,FlecheEnt->ent_anim.Pos);
		//	}
			FlecheEnt->Timing = true;
			FlecheEnt->Time_msec = 5000.0f;
			FlecheEnt->ent_anim.Pos = ctrace.impact;
		}
	}

	// GROS HACK DE DEP POUR RETIRE LES OBJETS EN DEHORS DU BSP
	if(MapDataFormat == MAP_BSP)
	if(bsp_FindCamera(FlecheEnt->ent_anim.Pos) < 0)
	{
		m_ConsPrint("entity in solid space error\n");
		o_RemoveEntityFromList(FlecheEnt);
		return NULL;
	}

	return FlecheEnt;
}

pEntity MonstreProcess(pEntity MonstreEnt)
{
	int			index;

	if(!MonstreEnt)
		return NULL;

	if(MonstreEnt->entity_id != DEPLA		&&
		MonstreEnt->entity_id != MORBAK		&&
		MonstreEnt->entity_id != ZEBAT		&&
		MonstreEnt->entity_id != SUBZERAT	&&
		MonstreEnt->entity_id != ZEMUMMY)
		return MonstreEnt;

	if(!MonstreEnt->Timing)
		return MonstreEnt;

	MonstreEnt->Time_msec -= gFrameTime_msec;
	if(MonstreEnt->Time_msec<0)
	{
		// HACK
		if(gIsMultiplayer)
		{
			PlayerResetStartPos(MORBAK);
			MonstreEnt->health = 100;
			MonstreEnt->IsChanged_Animation = true;
			//msk_ChangeAnim(MonstreEnt, MonstreEnt->model,MSK_THINK);
			return MonstreEnt;
		}

		index = o_FindRespawnTabIndex(MonstreEnt);
		if(index >= 0)
		{
			RespawnTab[index].pickedup = true;
			RespawnTab[index].Timer = 5000.0f;
		}
		gNumberOfMonsters--;
		o_RemoveEntityFromList(MonstreEnt);
		return NULL;
	}
	else if(MonstreEnt->Time_msec<=1000.0f)
	{
		MonstreEnt->Blending=true;
		MonstreEnt->BlendVal=MonstreEnt->Time_msec/1000.0f;
	}
	return MonstreEnt;
}

// le truc c'est que les objets n'ont pas chacun une liste de face mais partage la meme
// par exemple ici pour les flames ya 2 listes : lampe et mlampe.

pEntity LampeProcess(pEntity LampeEnt, int *isanim)
{
	Face_t		*pFace;
	vect_t		pos;
//	ANIMENTITY	anim;

	if(!LampeEnt)
		return LampeEnt;
	if(LampeEnt->entity_id != LAMPE && LampeEnt->entity_id != MLAMPE)
		return LampeEnt;

	if(o_IsVisible(LampeEnt))
	if(randInt(0,64) == 5)
	{
		pos = LampeEnt->ent_anim.Pos;

		if(LampeEnt->entity_id == LAMPE)
			pos.Y += 20.0f;
		else
			pos.Y += 10.0f;

		if(randInt(0,2) == 1)
			fx_Particule(pos,1,GLYELLOW);
		else
			fx_Particule(pos,1,GLRED);

	//	pos.X += randInt(-4,4);
	//	pos.Z += randInt(-4,4);
/*
		o_ClearAnimFields(&anim);
		anim.Pos = pos;
		anim.Siz.X = 0.2f;
		anim.Siz.Y = 0.2f;
		anim.Siz.Z = 0.2f;
		anim.Siz_Vel.X = 0.004f;
		anim.Siz_Vel.Y = 0.004f;
		anim.Siz_Vel.Z = 0.004f;
		anim.Pos_Vel.Y = 0.018f;

		anim.Pos_Vel.X = 0.002f;
		anim.Pos_Vel.Z = 0.002f;
		
		o_AddEntityToList(SMOKE,-1,anim,false);
*/
	}

	switch(*isanim)
	{
		case 0:
			if(LampeEnt->entity_id == LAMPE)
				*isanim = 1;
			else if(LampeEnt->entity_id == MLAMPE)
				*isanim = 2;
		break;

		case 1:
			if(LampeEnt->entity_id == MLAMPE)
				*isanim = 3;
			else
				return LampeEnt;
		break;

		case 2:
			if(LampeEnt->entity_id == LAMPE)
				*isanim = 3;
			else
				return LampeEnt;
		break;

		default:
		return LampeEnt;
	}

	LampeEnt->fxTime_msec += gFrameTime_msec;
	if(LampeEnt->fxTime_msec >= FLAME_ANIM_MSEC)
	{
		LampeEnt->fxTime_msec = 0;

		for(pFace=LampeEnt->FaceList ; pFace ; pFace=pFace->Next)
		if(pFace->Type == FLAMES)
		{
			pFace->TextureID++;
			if(pFace->TextureID >= txid_FLAME[7])
				pFace->TextureID = txid_FLAME[0];
			else
				pFace->TextureID++;
		}
	}

	return LampeEnt;
}

pEntity LettreProcess(pEntity LettreEnt)
{
	if(!LettreEnt)
		return LettreEnt;
	if(LettreEnt->entity_id < H || LettreEnt->entity_id > N)
		return LettreEnt;

	return LettreEnt;
}

pEntity PangProcess(pEntity PangEnt, pEntity PlayerEnt)
{
	if(!PangEnt)
		return NULL;
	if(PangEnt->entity_id != PANG
		&& PangEnt->entity_id != SMOKE
		&& PangEnt->entity_id != BLOOD
	//	&& PangEnt->entity_id != TOILE
		)
		return PangEnt;

	PangEnt->ent_anim.Rot.X = cam_RotationCycle( -PlayerEnt->ent_anim.Rot.X );
	PangEnt->ent_anim.Rot.Y = PlayerEnt->ent_anim.Rot.Y;
	PangEnt->ent_anim.Rot.Z = 0;

	if(!PangEnt->Timing)
	{
		PangEnt->Blending=true;
		PangEnt->Timing=true;
		if(PangEnt->entity_id == BLOOD)
			PangEnt->Time_msec = 1500.0f;
		else
			PangEnt->Time_msec = 1100.0f;
	}
	PangEnt->Time_msec -= gFrameTime_msec;
	if(PangEnt->Time_msec <= 0)
	{
		o_RemoveEntityFromList(PangEnt);
		return NULL;
	}
	if(PangEnt->entity_id == BLOOD)
		PangEnt->BlendVal = PangEnt->Time_msec/1600.0f;
	else
		PangEnt->BlendVal = PangEnt->Time_msec/3000.0f;

	return PangEnt;
}

//============================================================


void JadeProcess(pEntity JadeEnt)
{
	float	factor;

	if(!JadeEnt)
		return;
	if(JadeEnt->entity_id != JADE)
		return;

	if(!JadeEnt->Timing)
	{
		JadeEnt->Timing=true;
		JadeEnt->Time_msec=8000.0f;
	}
	JadeEnt->Time_msec -= gFrameTime_msec;
	if(JadeEnt->Time_msec<=0)
	{
		//music_playFX(20,JadeEnt->ent_anim.Pos,JadeEnt->ent_anim.Pos_Vel);
		o_RemoveEntityFromList(JadeEnt);
		return;
	}
	else if(JadeEnt->Time_msec<=1000.0f)
	{
		JadeEnt->Blending=true;
		JadeEnt->BlendVal=JadeEnt->Time_msec/1000.0f;
	}

	factor = gravity.value * gFrameTime_msec * 0.001f;
	JadeEnt = Gravity(JadeEnt,factor);
}

void FioleProcess(pEntity FioleEnt, pEntity PlayerEnt)
{
	/*
	float		factor;
	ANIMENTITY	anim;

	if(!FioleEnt)
		return;
	if(FioleEnt->entity_id != FIOLE1)
		return;

	FioleEnt->Blending=true;
	FioleEnt->BlendVal = 0.8f;
	FioleEnt->Time_msec -= gFrameTime_msec;

	//	GRAVITY
	factor = gravity.value * gFrameTime_msec * 0.001f;
	FioleEnt = Gravity(FioleEnt,factor);

	//	SMOKE
	if(FioleEnt->Time_msec <= 0)
	{
		FioleEnt->Time_msec = 1000.0f + (float)randInt(0,1000);
		o_ClearAnimFields(&anim);
		anim.Pos = FioleEnt->ent_anim.Pos;
		anim.Pos.Y = FioleEnt->ent_anim.Pos.Y + 0.01f;
		anim.Pos_Vel.Y = 0.00002f;
		anim.Siz_Vel.X = 0.01f;
		anim.Siz_Vel.Y = 0.005f;
		anim.Siz_Vel.Z = 0.01f;
		o_AddEntityToList(SMOKE,-1,anim,false);
		ds_PlaySound3D(32,FioleEnt->ent_anim.Pos,FioleEnt->ent_anim.Pos_Vel);
	}
	*/
}
