
#include "alloween.cpp"

int		gAngleView_player=0;
float	gCam_EarthQuake_time=0;
float	gCam_EarthQuake_force=0;


float cam_RotationCycle(float angle)
{	
	if(angle>ROTMAX)
		return angle-ROTMAX;
	else if(angle<-ROTMAX)
		return angle+ROTMAX;
	return angle;
}

void cam_EarthQuake_Launch(float force)
{
	gCam_EarthQuake_force = force;
	gCam_EarthQuake_time = 1000.0f;
}

void cam_EarthQuake_process()
{
	if(!gCam_EarthQuake_time)
		return;

	if(ProgramState != PS_GAME && ProgramState != PS_DIE && ProgramState != PS_WIN)
	{
		gCam_EarthQuake_time=0;
		return;
	}

	gCam_EarthQuake_time -= gFrameTime_msec;
	if(gCam_EarthQuake_time < 0)
		gCam_EarthQuake_time = 0;
	cam_EarthQuake(gCam_EarthQuake_force);
}

void cam_StopEarthQuake()
{
	gCam_EarthQuake_time = 0;
}

void cam_EarthQuake(float force)
{
	int		offset, offset2, offset3;
	float	o_sin,o_sin2, o_sin3;
	float	clock_time;

	if(!force)
		return;
	if(force>1.0f)
		force=1.0f;
	clock_time = clock() / (force*0.5f);

	force = 0.2f / force;

	offset = (int)clock_time;
	offset %= (int)ROTMAX;
	o_sin = Sin((float)offset) / force;

	offset2 = (int)(clock_time / 1.2f);
	offset2 %= (int)ROTMAX;
	o_sin2 = Sin((float)offset2) / force;

	offset3 = (int)(clock_time / 1.4f);
	offset3 %= (int)ROTMAX;
	o_sin3 = Sin((float)offset3) / force;

	gCamera.Pos.X += o_sin;
	gCamera.Pos.Y += o_sin3;
	gCamera.Pos.Z += o_sin2;
}

void cam_Move(float Direction, Camera_t *pCam)
{
	float	AngleX;
	float	AngleY;
	vect_t	Dest;
	vect_t	Dir = {0,0,-1};
	CTRACE	ClipTrace;

	AngleX = cam_RotationCycle(-pCam->Rot.X);
	AngleY = cam_RotationCycle(pCam->Rot.Y);

	Dir = m_RotateY(Dir,Direction);
	Dir = m_RotateX(Dir,AngleX);
	Dir = m_RotateY(Dir,AngleY);

	Dir.X = Dir.X * camera_vel.value * gFrameTime_msec;
	Dir.Y = Dir.Y * camera_vel.value * gFrameTime_msec;
	Dir.Z = Dir.Z * camera_vel.value * gFrameTime_msec;

	Dest.X = pCam->Pos.X + Dir.X;
	Dest.Y = pCam->Pos.Y + Dir.Y;
	Dest.Z = pCam->Pos.Z + Dir.Z;

	ClipTrace.Response = true;
	ClipTrace.DontUnstuck = false;
	ClipTrace.NoRadius = false;
	ClipTrace.ellipsoid.X = 10.0f;
	ClipTrace.ellipsoid.Y = 10.0f;
	ClipTrace.ellipsoid.Z = 10.0f;
	ClipTrace.Ent = NULL;
	col_Handle(&pCam->Pos,Dest,&ClipTrace);
	phy_BoundPos(&pCam->Pos);
}

void cam_MoveY(hBool Direction, Camera_t *pCam)
{
	vect_t	Dest;
	vect_t	Dir;
	CTRACE	ClipTrace;

	if(Direction)
		Dir.Y = 1;
	else
		Dir.Y = -1;

	Dir.Y = Dir.Y * camera_vel.value * gFrameTime_msec;

	Dest.X = pCam->Pos.X;
	Dest.Y = pCam->Pos.Y + Dir.Y;
	Dest.Z = pCam->Pos.Z;

	ClipTrace.Response = true;
	ClipTrace.DontUnstuck = false;
	ClipTrace.NoRadius = false;
	ClipTrace.ellipsoid.X = 10.0f;
	ClipTrace.ellipsoid.Y = 10.0f;
	ClipTrace.ellipsoid.Z = 10.0f;
	ClipTrace.Ent = NULL;
	col_Handle(&pCam->Pos,Dest,&ClipTrace);
	phy_BoundPos(&pCam->Pos);
}

void r_TourneY(Camera_t *pHero, float Angle, pEntity PlayerEnt)
{
	if(!PlayerEnt)
		return;

		if(player_view.value)
		{
			PlayerEnt->ent_anim.Rot.Y = cam_RotationCycle(PlayerEnt->ent_anim.Rot.Y-Angle);
		}
		else
		{
			pHero->Rot.Y = cam_RotationCycle(pHero->Rot.Y-Angle);
		}
}

void r_TourneX(Camera_t *pHero, float Angle, pEntity PlayerEnt)
{
	if(!PlayerEnt)
		return;

		if(player_view.value)
		{
					PlayerEnt->ent_anim.Rot.X -= Angle;

					if(PlayerEnt->ent_anim.Rot.X > player_look_ud.value)
						PlayerEnt->ent_anim.Rot.X = player_look_ud.value;
					else if (PlayerEnt->ent_anim.Rot.X < -player_look_ud.value)
						PlayerEnt->ent_anim.Rot.X = -player_look_ud.value;
		}
		else
		{
			pHero->Rot.X -= Angle;

			if(pHero->Rot.X > player_look_ud.value)
				pHero->Rot.X = player_look_ud.value;
			else if (pHero->Rot.X < -player_look_ud.value)
				pHero->Rot.X = -player_look_ud.value;
		}
}
