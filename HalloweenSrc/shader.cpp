#include "alloween.h"


void shader_Skydome(pFace Face)
{
	Face->blend_src = GL_ONE;
	Face->blend_dst = GL_ONE_MINUS_SRC_COLOR;
}

void shader_Sky(pFace Face)
{
	int		i;
	float	ofX;

	if(!Face)
		return;
	ofX = (gFrameTime_msec * 0.00001f); // 0.00005
	for(i=0 ; i<Face->NumberOfVertices ; i++)
	{
		Face->Vertice[i].U += ofX;
		if(Face->Vertice[i].U >= 10000)
			Face->Vertice[i].U -= 10000;
	}
}

void shader_Gun(pFace Face)
{
	int		i;
	int		offset;
	float	ofX;

	if(!Face)
		return;

	offset = (int)(disp_GetClock() * 0.030) % ROTMAX_I;
	ofX = (gFrameTime_msec * 0.0006f * Sin_i(offset));

	for(i=0 ; i<Face->NumberOfVertices ; i++)
	{
		Face->Vertice[i].Y += ofX;
	}
}

void shader_Lave(pFace Face)
{
	int		i;
	int		offset;
	float	LavaOff_sin;
	float	LavaOff_cos;
	float	ofX, ofY;

	if(!Face)
		return;

	if(ProgramState == PS_GAME ||
		ProgramState == PS_DIE ||
		ProgramState == PS_ENDSCORE ||
		ProgramState == PS_WIN)
	{
		offset = (int)(disp_GetClock() * 0.008) % ROTMAX_I;
		LavaOff_sin = Sin_i(offset);
		LavaOff_cos = Cos_i(offset);
	
		ofX = (gFrameTime_msec * 0.0001f * LavaOff_sin);
		ofY = (gFrameTime_msec * 0.0001f * LavaOff_cos);
	}
        else
        {
            ofX = 0;
            ofY = 0;
        }

	for(i=0 ; i<Face->NumberOfVertices ; i++)
	{
		Face->Vertice[i].U += ofX;
		Face->Vertice[i].V += ofY;
	}
}

void shader_Lightmap(pFace pCurr)
{
	int		line;
	int		col;
	int		i;

	if(!pCurr)
		return;

	if(pCurr->Type == INVISIBLE			||
		pCurr->Type == MOREINVISIBLE	||
		pCurr->Type == LAVA				||
		pCurr->Type == ACTIONBRUSH	
		)
		return;

	glEnable(GL_BLEND);
	glBlendFunc(GL_ZERO,GL_SRC_COLOR);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);

	if(IsColorScreen)
	{
		glColor4f(WorldColorR, WorldColorG, WorldColorB, 1);
	}

	gl_BindTexture((int)(txid_LIGHTMAP_N[0] + (pCurr->LightID*ONEOVER256)));

	line = (int)((pCurr->LightID%256) * ONEOVER16);
	col = (pCurr->LightID%256) % 16;


	glDepthFunc(GL_LEQUAL);
	glDepthMask(GL_FALSE);

	glEnable(GL_POLYGON_OFFSET_FILL);
	glPolygonOffset(-2.0, -2.0); // -1 -1

	glBegin(GL_TRIANGLES);
	for(i=0 ; i<3 ; i++)
	{
		glTexCoord2f((pCurr->Vertice[i].U_lm+col)*ONEOVER16,(pCurr->Vertice[i].V_lm+line)*ONEOVER16);
		glVertex3f(pCurr->Vertice[i].X,pCurr->Vertice[i].Y,pCurr->Vertice[i].Z);
	}
	glEnd();

	glDisable(GL_POLYGON_OFFSET_FILL);

	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LESS);

	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_BLEND);
}

