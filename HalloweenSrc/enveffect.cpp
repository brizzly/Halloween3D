#include "alloween.h"


PARTICULE	ParticuleTab[MAX_PARTSLOT];
hBool		IsColorScreen=false;
float		CS_trans;
float		CS_trans2;
float		CS_time;
float		CS_R;
float		CS_G;
float		CS_B;
float		CS_R_step;
float		CS_G_step;
float		CS_B_step;


void fx_InitParticule()
{
	int	i;

	for(i=0 ; i<MAX_PARTSLOT ; i++)
		ParticuleTab[i].used = false;
}

void fx_Particule(vect_t pos, int n, int color)
{
	int		i;
	int		j;

	if(n<=0 || n >= MAX_PART)
		return;

	for(i=0 ; i<MAX_PARTSLOT ; i++)
	if(!ParticuleTab[i].used)
		break;
	if(i>=MAX_PARTSLOT)
		return;

	ParticuleTab[i].used = true;
	ParticuleTab[i].nParticule = n;
	ParticuleTab[i].color = color;
	for(j=0 ; j<n ; j++)
	{
		ParticuleTab[i].pos[j] = pos;
		ParticuleTab[i].dir[j].X = randInt(-100,100) * 0.0004f;
		ParticuleTab[i].dir[j].Y = randInt(20,100) * 0.0004f;
		ParticuleTab[i].dir[j].Z = randInt(-100,100) * 0.0004f;
		ParticuleTab[i].lifetime_msec[j] = 1000.0f;
	}
}

void fx_UpdateParticule()
{
	int		i;
	int		j;
	vect_t	dir;

	for(i=0 ; i<MAX_PARTSLOT ; i++)
	if(ParticuleTab[i].used)
	{
		for(j=0 ; j<ParticuleTab[i].nParticule ; j++)
		{
			ParticuleTab[i].lifetime_msec[j] -= gFrameTime_msec;
			if(ParticuleTab[i].lifetime_msec[j] <= 0)
			{
				ParticuleTab[i].used = false;
				continue;
			}
			// gravity
			ParticuleTab[i].dir[j].Y -= gFrameTime_msec * 0.0001f;

			dir = ParticuleTab[i].dir[j];
			m_Vector_Scale(&dir,gFrameTime_msec);
			ParticuleTab[i].pos[j] = m_Vector_Add(ParticuleTab[i].pos[j],dir);
		}
	}
}

void fx_DrawParticule(vect_t campos)
{
	int		i;
	int		j;
	float	dist;
//	vect_t	v;

	for(i=0 ; i<MAX_PARTSLOT ; i++)
	if(ParticuleTab[i].used)
	{
		for(j=0 ; j<ParticuleTab[i].nParticule ; j++)
		{
		//	v = m_Vector_Sub(campos,ParticuleTab[i].pos[j]);
		//	dist = Norme(v);
		//	if(dist < 1)
		//		dist = 1;
		//	dist = 150.0f / dist;
		//	if(dist<1)
				dist = 1;
			disp_Particule(ParticuleTab[i].pos[j],ParticuleTab[i].color,dist);
		}
	}
}

//==============================================================

void fx_ColorScreen(float trans, float timing, float R, float G, float B, hBool force)
{
	if(!force)
	if(IsColorScreen)
		return;
	IsColorScreen = true;

	CS_trans = trans;
	CS_trans2 = trans;
	CS_time = timing;
	CS_R = R;
	CS_G = G;
	CS_B = B;

	if(CS_trans <= 0)
	{
		CS_trans = 1;
		CS_trans2 = 1;
	}
	if(CS_R < 0)
		CS_R = 0;
	if(CS_G < 0)
		CS_G = 0;
	if(CS_B < 0)
		CS_B = 0;
	if(CS_R > 1)
		CS_R = 1;
	if(CS_G > 1)
		CS_G = 1;
	if(CS_B > 1)
		CS_B = 1;

	CS_R_step = (CS_R - 1.0f) * 1000.0f / trans;
	CS_G_step = (CS_G - 1.0f) * 1000.0f / trans;
	CS_B_step = (CS_B - 1.0f) * 1000.0f / trans;
}

void fx_UpdateColorScreen()
{
	if(!IsColorScreen)
		return;

	// HACK
	if(CS_trans == -666)
		return;

	if(CS_trans)
	{
		CS_trans -= gFrameTime_msec;
		if(CS_trans < 0)
			CS_trans = 0;

		WorldColorR += CS_R_step * gFrameTime_msec * 0.001f;
		WorldColorG += CS_G_step * gFrameTime_msec * 0.001f;
		WorldColorB += CS_B_step * gFrameTime_msec * 0.001f;

		if(WorldColorR < 0)
			WorldColorR = 0;
		else if(WorldColorR > 1)
			WorldColorR = 1;

		if(WorldColorG < 0)
			WorldColorG = 0;
		else if(WorldColorG > 1)
			WorldColorG = 1;

		if(WorldColorB < 0)
			WorldColorB = 0;
		else if(WorldColorB > 1)
			WorldColorB = 1;

		return;
	}

	WorldColorR = CS_R;
	WorldColorG = CS_G;
	WorldColorB = CS_B;

	if(CS_time)
	{
		CS_time -= gFrameTime_msec;
		if(CS_time < 0)
			CS_time = 0;

		return;
	}

	CS_R_step = (1.0f - WorldColorR) * 1000.0f / CS_trans2;
	CS_G_step = (1.0f - WorldColorG) * 1000.0f / CS_trans2;
	CS_B_step = (1.0f - WorldColorB) * 1000.0f / CS_trans2;

	if(CS_trans2)
	{
		CS_trans2 -= gFrameTime_msec;
		if(CS_trans2 < 0)
			CS_trans2 = 0;

		WorldColorR += CS_R_step * gFrameTime_msec * 0.001f;
		WorldColorG += CS_G_step * gFrameTime_msec * 0.001f;
		WorldColorB += CS_B_step * gFrameTime_msec * 0.001f;

		if(WorldColorR < 0)
			WorldColorR = 0;
		else if(WorldColorR > 1)
			WorldColorR = 1;

		if(WorldColorG < 0)
			WorldColorG = 0;
		else if(WorldColorG > 1)
			WorldColorG = 1;

		if(WorldColorB < 0)
			WorldColorB = 0;
		else if(WorldColorB > 1)
			WorldColorB = 1;

		return;
	}

	WorldColorR = 1.0f;
	WorldColorG = 1.0f;
	WorldColorB = 1.0f;
	IsColorScreen = false;
}

