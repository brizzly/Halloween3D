
#include "alloween.h"

#ifdef H_WIN
#include "misc/Glext.h"
#endif


#include "display.h"




float	WorldColorR;
float	WorldColorG;
float	WorldColorB;

float	gLavaOff_sin;
float	gLavaOff_cos;

hBool	gIsFog = false;
float	gIsEclair;

const char *gl_vendor;
const char *gl_renderer;
const char *gl_version;
const char *gl_extensions;


#define __ARB_ENABLE true		// Used To Disable ARB Extensions Entirely

/*
PFNGLMULTITEXCOORD1FARBPROC		glMultiTexCoord1fARB	= NULL;
PFNGLMULTITEXCOORD2FARBPROC		glMultiTexCoord2fARB	= NULL;
PFNGLMULTITEXCOORD3FARBPROC		glMultiTexCoord3fARB	= NULL;
PFNGLMULTITEXCOORD4FARBPROC		glMultiTexCoord4fARB	= NULL;
PFNGLACTIVETEXTUREARBPROC		glActiveTextureARB		= NULL;
PFNGLCLIENTACTIVETEXTUREARBPROC	glClientActiveTextureARB= NULL;	

*/
hBool	gMultiTexturing = false;



hBool gl_isInString(char *string, const char *search)
{
	int		pos=0;
	int		maxpos=strlen(search)-1;
	int		len=strlen(string);	
	char	*other;
	int		i;

	for(i=0; i<len; i++) {
		if ((i==0) || ((i>1) && string[i-1]=='\n')) {				// New Extension Begins Here!
			other=&string[i];			
			pos=0;													// Begin New Search
			while (string[i]!='\n') {								// Search Whole Extension-String
				if (string[i]==search[pos]) pos++;					// Next Position
				if ((pos>maxpos) && string[i+1]=='\n') return true; // We Have A Winner!
				i++;
			}			
		}
	}	
	return false;													// Sorry, Not Found!
}

hBool gl_initMultitexture()
{
	return true;

	/*
	char	*extensions;
	int		len;
	int		i;

	int		maxTexelUnits = 1;										// Number Of Texel-Pipelines. This Is At Least 1.

	return true;	// HACK


	m_ConsPrint("gl_initMultitexture\n");

	extensions = strdup((char *) glGetString(GL_EXTENSIONS));			// Fetch Extension String
	len = strlen(extensions);
	for (i=0 ; i<len ; i++)										// Separate It By Newline Instead Of Blank
		if (extensions[i]==' ')
			extensions[i]='\n';

	if (gl_isInString(extensions,"GL_ARB_multitexture")				// Is Multitexturing Supported?
		&& __ARB_ENABLE												// Override-Flag
		&& gl_isInString(extensions,"GL_EXT_texture_env_combine"))		// Is texture_env_combining Supported?
	{	
		glGetIntegerv(GL_MAX_TEXTURE_UNITS_ARB,&maxTexelUnits);
		glMultiTexCoord1fARB	= (PFNGLMULTITEXCOORD1FARBPROC)		wglGetProcAddress("glMultiTexCoord1fARB");
		glMultiTexCoord2fARB	= (PFNGLMULTITEXCOORD2FARBPROC)		wglGetProcAddress("glMultiTexCoord2fARB");
		glMultiTexCoord3fARB	= (PFNGLMULTITEXCOORD3FARBPROC)		wglGetProcAddress("glMultiTexCoord3fARB");
		glMultiTexCoord4fARB	= (PFNGLMULTITEXCOORD4FARBPROC)		wglGetProcAddress("glMultiTexCoord4fARB");
		glActiveTextureARB		= (PFNGLACTIVETEXTUREARBPROC)		wglGetProcAddress("glActiveTextureARB");
		glClientActiveTextureARB= (PFNGLCLIENTACTIVETEXTUREARBPROC)	wglGetProcAddress("glClientActiveTextureARB");		

		m_ConsPrint("The GL_ARB_multitexture extension will be used. feature supported!\n",extensions);
	
		return true;
	}
	return false;
*/
}

uint gl_GetBlendMode(int mode)
{
	switch (mode)
	{
		case 0:	 return GL_ZERO;
		case 1:	 return GL_ONE;
		case 2:	 return GL_SRC_COLOR;
		case 3:	 return GL_ONE_MINUS_SRC_COLOR;
		case 4:	 return GL_SRC_ALPHA;
		case 5:	 return GL_ONE_MINUS_SRC_ALPHA;
		case 6:	 return GL_DST_ALPHA;
		case 7:	 return GL_ONE_MINUS_DST_ALPHA;
		default: return GL_ZERO;
	}
}

void gl_SetFog()
{
	GLuint fogMode[]= { GL_EXP, GL_EXP2, GL_LINEAR }; // Storage For Three Types Of Fog 
	GLuint fogfilter; // Which Fog To Use 
	GLfloat fogColor[4]= {0.0f, 0.0f, 0.0f, 1.0f}; // Fog Color
	
	fogfilter = (int)fogmode.value % 3;

 	glFogi(GL_FOG_MODE, fogMode[fogfilter]); 	// Fog Mode 
 	glFogfv(GL_FOG_COLOR, fogColor); 			// Set Fog Color 
 	glFogf(GL_FOG_DENSITY, fogdensity.value); 	// How Dense Will The Fog Be 
 	glHint(GL_FOG_HINT, GL_DONT_CARE); 			// Fog Hint Value 
 	glFogf(GL_FOG_START, 50.0f); 				// Fog Start Depth 
 	glFogf(GL_FOG_END,  MAX_CAMERA); 			// Fog End Depth 
 	glEnable(GL_FOG); 							// Enables GL_FOG 
	gIsFog = true;
}

void gl_StopFog()
{
	glDisable(GL_FOG);
	gIsFog = false;
}



void gl_DrawNormal(Face_t *pCurr)
{
	vect_t	StartPos;
	int		i;
	float	divn = 1.0f;

	if(gIsServer && net_dedicated.value)
		return;

	glDisable(GL_TEXTURE_2D);
	glColor3f(WorldColorR,WorldColorG,WorldColorB);
	glPointSize(4);

	if(pCurr)
	{
		StartPos.X = 0;
		StartPos.Y = 0;
		StartPos.Z = 0;
		for(i=0 ; i<pCurr->NumberOfVertices ; i++)
		{
			StartPos.X += pCurr->Vertice[i].X;
			StartPos.Y += pCurr->Vertice[i].Y;
			StartPos.Z += pCurr->Vertice[i].Z;
		}
		StartPos.X /= (float)pCurr->NumberOfVertices;
		StartPos.Y /= (float)pCurr->NumberOfVertices;
		StartPos.Z /= (float)pCurr->NumberOfVertices;

		glBegin(GL_LINES);
			glVertex3f(StartPos.X,StartPos.Y,StartPos.Z);
			glVertex3f(StartPos.X+pCurr->Plan.VecteurNormal.X/divn,StartPos.Y+pCurr->Plan.VecteurNormal.Y/divn,StartPos.Z+pCurr->Plan.VecteurNormal.Z/divn);
		glEnd();

		glBegin(GL_POINTS);
			glVertex3f(StartPos.X+pCurr->Plan.VecteurNormal.X/divn,StartPos.Y+pCurr->Plan.VecteurNormal.Y/divn,StartPos.Z+pCurr->Plan.VecteurNormal.Z/divn);
		glEnd();
	}

	glPointSize(1);
}

void gl_DrawVectors(Face_t *ListFace)
{
	Face_t	*pCurr;

	if(gIsServer && net_dedicated.value)
		return;

	for(pCurr=ListFace ; pCurr ; pCurr=pCurr->Next)
		gl_DrawNormal(pCurr);
}

void gl_DrawVector(vect_t *pos, vect_t *v, float scale)
{
	if(gIsServer && net_dedicated.value)
		return;

	glDisable(GL_TEXTURE_2D);
	glPointSize(4);
	glColor3f(WorldColorR,WorldColorG,WorldColorB);

	if(v)
	{
		v->X *= scale;
		v->Y *= scale;
		v->Z *= scale;
		glBegin(GL_LINES);
			glVertex3f(pos->X,pos->Y,pos->Z);
			glVertex3f(pos->X+v->X,pos->Y+v->Y,pos->Z+v->Z);
		glEnd();

		glBegin(GL_POINTS);
			glVertex3f(pos->X+v->X,pos->Y+v->Y,pos->Z+v->Z);
		glEnd();
	}

	glPointSize(1);
}

void gl_DrawBoundingBox(pEntity	Entity)
{
	BOUNDINGBOX	bbox;
	vect_t		Pos={0,0,0};
	vect_t		Dir={0,0,1};

	if(gIsServer && net_dedicated.value)
		return;

	glPointSize(4);
	glLineWidth(1);
	glDisable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);
	glColor3f(WorldColorR,WorldColorG,WorldColorB);

	bbox = Entity->ent_bbox;

	glPushMatrix();
	glTranslatef(Entity->ent_anim.Pos.X, Entity->ent_anim.Pos.Y, Entity->ent_anim.Pos.Z);	  

	glBegin(GL_LINES);
		glVertex3f(bbox.BoxMin.X,bbox.BoxMin.Y,bbox.BoxMin.Z);
		glVertex3f(bbox.BoxMin.X,bbox.BoxMin.Y,bbox.BoxMax.Z);
		glVertex3f(bbox.BoxMin.X,bbox.BoxMin.Y,bbox.BoxMin.Z);
		glVertex3f(bbox.BoxMin.X,bbox.BoxMax.Y,bbox.BoxMin.Z);
		glVertex3f(bbox.BoxMin.X,bbox.BoxMax.Y,bbox.BoxMin.Z);
		glVertex3f(bbox.BoxMin.X,bbox.BoxMax.Y,bbox.BoxMax.Z);
		glVertex3f(bbox.BoxMin.X,bbox.BoxMin.Y,bbox.BoxMax.Z);
		glVertex3f(bbox.BoxMin.X,bbox.BoxMax.Y,bbox.BoxMax.Z);

		glVertex3f(bbox.BoxMax.X,bbox.BoxMin.Y,bbox.BoxMin.Z);
		glVertex3f(bbox.BoxMax.X,bbox.BoxMin.Y,bbox.BoxMax.Z);
		glVertex3f(bbox.BoxMax.X,bbox.BoxMin.Y,bbox.BoxMin.Z);
		glVertex3f(bbox.BoxMax.X,bbox.BoxMax.Y,bbox.BoxMin.Z);
		glVertex3f(bbox.BoxMax.X,bbox.BoxMax.Y,bbox.BoxMin.Z);
		glVertex3f(bbox.BoxMax.X,bbox.BoxMax.Y,bbox.BoxMax.Z);
		glVertex3f(bbox.BoxMax.X,bbox.BoxMin.Y,bbox.BoxMax.Z);
		glVertex3f(bbox.BoxMax.X,bbox.BoxMax.Y,bbox.BoxMax.Z);

		glVertex3f(bbox.BoxMin.X,bbox.BoxMin.Y,bbox.BoxMin.Z);
		glVertex3f(bbox.BoxMax.X,bbox.BoxMin.Y,bbox.BoxMin.Z);
		glVertex3f(bbox.BoxMin.X,bbox.BoxMax.Y,bbox.BoxMax.Z);
		glVertex3f(bbox.BoxMax.X,bbox.BoxMax.Y,bbox.BoxMax.Z);
		glVertex3f(bbox.BoxMin.X,bbox.BoxMin.Y,bbox.BoxMax.Z);
		glVertex3f(bbox.BoxMax.X,bbox.BoxMin.Y,bbox.BoxMax.Z);
		glVertex3f(bbox.BoxMin.X,bbox.BoxMax.Y,bbox.BoxMin.Z);
		glVertex3f(bbox.BoxMax.X,bbox.BoxMax.Y,bbox.BoxMin.Z);
	glEnd();

	// draw entity dir vector
	glRotatef(Entity->ent_anim.Rot.X, 1.0, 0.0, 0.0);
	glRotatef(Entity->ent_anim.Rot.Y, 0.0, 1.0, 0.0);
	glRotatef(Entity->ent_anim.Rot.Z, 0.0, 0.0, 1.0);
	gl_DrawVector(&Pos,&Dir,10);

	glPopMatrix();
	glPointSize(1);
}




void gl_DrawFaces(Face_t *FaceList, vect_t pos)
{
	Face_t	*pCurr;

	if(gIsServer && net_dedicated.value)
		return;

	for(pCurr=FaceList ; pCurr ; pCurr=pCurr->Next)
	{
		if(!game_FalconMode)
		if(pCurr->Type == INVISIBLE || pCurr->Type == MOREINVISIBLE)
			continue;

		if(pCurr->Type == ACTIONBRUSH)
		{
			pCurr->Plan.PointOnPlane.X = pCurr->Vertice[0].X;
			pCurr->Plan.PointOnPlane.Y = pCurr->Vertice[0].Y;
			pCurr->Plan.PointOnPlane.Z = pCurr->Vertice[0].Z;
		}
		else
		{
			if(!FastCull(pCurr, pos))
				continue;
		}

		switch(pCurr->Type)
		{
			case LAVA:
			shader_Lave(pCurr);
			break;

		//	case GUN1:
		//	case GUN2:
		//	case GUN3:
		//	shader_Gun(pCurr);
		//	break;
		}

		disp_Face(pCurr);

		if(lightmap.value && gMapLightened)
		{
			shader_Lightmap(pCurr);
		}
	}
}



void gl_RenderFace_ENV(Face_t *pFace, int angleY)
{
	int			i;
	vect_t		vn;
	Render_t	rend;

	if(gIsServer && net_dedicated.value)
		return;
	if(!pFace)
		return;
	if(!draw_face.value)
		return;

	rend.is_blend = false;
	rend.is_color = true;
	rend.color.R = WorldColorR;
	rend.color.G = WorldColorG;
	rend.color.B = WorldColorB;
	rend.color.A = 1;
	rend.is_texture = true;
	rend.texture_id = pFace->TextureID;
	rend.nvert = pFace->NumberOfVertices;
	rend.begin_mode = GL_POLYGON;
	rend.is_depthtest = true;
	rend.is_depthmask = true;
	rend.is_draw2D = false;

	for(i=0 ; i<pFace->NumberOfVertices ; i++)
	{
		vn.X = pFace->Vertice[i].NX;
		vn.Y = pFace->Vertice[i].NY;
		vn.Z = pFace->Vertice[i].NZ;

		vn = m_RotateY(vn, (float)angleY);

		rend.uv[i][0] = vn.X;
		rend.uv[i][1] = vn.Z;
		rend.xyz[i][0] = pFace->Vertice[i].X;
		rend.xyz[i][1] = pFace->Vertice[i].Y;
		rend.xyz[i][2] = pFace->Vertice[i].Z;
	}

	gl_RenderPoly(rend);

/*
	glColor3f(WorldColorR,WorldColorG,WorldColorB);
	gl_BindTexture(pFace->TextureID);

	glBegin(GL_POLYGON);
	for(i=0 ; i<pFace->NumberOfVertices ; i++)
	{
		vn.X = pFace->Vertice[i].NX;
		vn.Y = pFace->Vertice[i].NY;
		vn.Z = pFace->Vertice[i].NZ;

		vn = m_RotateY(vn, (float)angleY);

		glTexCoord2f(vn.X, vn.Z);
	//	glTexCoord2f(vn.X * pFace->Vertice[i].U, vn.Z * pFace->Vertice[i].V);
		glVertex3f(pFace->Vertice[i].X,pFace->Vertice[i].Y,pFace->Vertice[i].Z);
	}
	glEnd();

	glColor3f(WorldColorR,WorldColorG,WorldColorB);
	*/
}

void gl_RenderEntity(pEntity Entity, pEntity PlayerEnt, float R, float G, float B)
{
	Face_t	*pCurr;
	Face_t	*FaceList;
	float	timing;
	float	A;
	vect_t	w;
	vect_t	v;
	float	angle;
	float	val;
	hBool	IsBlend;
	float	R_copy, G_copy, B_copy;
	float	DistanceObjCam;

	if(gIsServer && net_dedicated.value)
		return;
	if(!Entity)
		return;

	IsBlend = false;

	if(Entity->Timing)
		timing = Entity->Time_msec;
	else
		timing = 2000.0f;

	if(gIsMultiplayer && !gIsServer && Entity->entity_id == PERSO)
	{
		return;
	}

	if(Entity->entity_id == DEPLA)
	{
		msk_draw(&gModel_depla, Entity,timing, R, G, B);
		return;
	}
	else if(Entity->entity_id == MORBAK)
	{
		msk_draw(&gModel_morbak, Entity,timing, R, G, B);
		return;
	}
	else if(Entity->entity_id == ZEBAT)
	{
		msk_draw(&gModel_zebat, Entity,timing, R, G, B);
		return;
	}
	else if(Entity->entity_id == SUBZERAT)
	{
		msk_draw(&gModel_subzerat, Entity,timing, R, G, B);
		return;
	}
	else if(Entity->entity_id == ZEMUMMY)
	{
		msk_draw(&gModel_zemummy, Entity,timing, R, G, B);
		return;
	}
//	else if(Entity->entity_id == FIOLE1 ||
//			Entity->entity_id == FIOLE2 ||
//			Entity->entity_id == FIOLE3)
//	{
//	}



	if(Entity->entity_id == SKYDOME)
	{
		IsBlend = true;
		R = 0.83f;
		G = 1.0f;
		B = 0.25f;

		/*
		// disparition du ciel nuageux la nuit
		if(gHeure >= 22*3600 && gHeure < 23*3600)
		{
			float fval = (1-(gHeure - 22*3600) / 3600.0f);
			R = fval*WorldColorR;
			G = fval*WorldColorG;
			B = fval*WorldColorB;
		}
		// nuit sans nuage
		else if(gHeure >= 23*3600 || gHeure < 4*3600)
		{
			R = G = B = 0;
		}
		// aparition du ciel nuageux la nuit
		else if(gHeure >= 4*3600 && gHeure < 5*3600)
		{
			float fval = ((gHeure - 4*3600) / 3600.0f);
			R = fval*WorldColorR;
			G = fval*WorldColorG;
			B = fval*WorldColorB;
		}
		else
		{
			R = WorldColorR;
			G = WorldColorG;
			B = WorldColorB;
		}
		*/
	}

	DistanceObjCam = m_Distance(Entity->ent_anim.Pos, gCamera.Pos);
	
	// hack
	if(Entity->player_holded == false)
	if(Entity->entity_id != SKYDOME)
	if(DistanceObjCam > 1500)
	{
		return;
	}

	if(Entity->Blending)
	{
		IsBlend = true;
		A = Entity->BlendVal;
	}
	else
	{
		A = 1;
	}

	if(	Entity->entity_id != FIOLE1 && 
		Entity->entity_id != FIOLE2 && 
		Entity->entity_id != FIOLE3)
	{
		if(Entity->player_holded)
		{
			IsBlend = false;
			gForceNoDepthTest = false;
			glClear(GL_DEPTH_BUFFER_BIT);
		}
	}

	if(Entity->entity_id == GUN1 && Entity->player_holded && PlayerEnt->ReloadTime > 0)
	{
		FaceList = Gun1LaunchedFaceList;
	}
	else if(Entity->entity_id == GUN3 && Entity->player_holded)
	{
		FaceList = Gun3FaceList;
	}
	else
	{
		FaceList = Entity->FaceList;
	}

	// CHEAT MODE
	if(gCheatMode_pumpkin)
	if(	Entity->entity_id == BOMBE ||
		Entity->entity_id == FLECHE ||
		Entity->entity_id == EXPLOSION ||
		Entity->entity_id == SPIKE)
	{
		if(o_FindEntity(PUMPKIN))
		{
			FaceList = o_FindEntity(PUMPKIN)->FaceList;
		}
	}
	
	for(pCurr=FaceList ; pCurr ; pCurr = pCurr->Next)
	{
		if( Entity->entity_id >= H && Entity->entity_id <= N)
		{
			IsBlend = false;
			if(DistanceObjCam < 500)
			{
				gl_RenderFace_ENV(pCurr, (int)Entity->ent_anim.Rot.Y);
				continue;
			}
		}
		else if(Entity->entity_id == SKYDOME)
		{
			shader_Sky(pCurr);
			shader_Skydome(pCurr);
		}

		R_copy = R;
		G_copy = G;
		B_copy = B;
		
		// eclairage selon la normale face a la camera
		/*
		if(DistanceObjCam < 500)
		if(lightmap.value)
		if(Entity->player_holded == false)
		if(	Entity->entity_id == GUN1 ||
			Entity->entity_id == GUN2 ||
			Entity->entity_id == GUN3 || 
			Entity->entity_id == AMO1 ||
			Entity->entity_id == AMO2 ||
			Entity->entity_id == AMO3) 
		{
			v = pCurr->Plan.VecteurNormal;
			angle = cam_RotationCycle(Entity->ent_anim.Rot.Y);
			v = m_RotateY(v, angle); // rotate Y,X,Z
				
			angle = cam_RotationCycle(Entity->ent_anim.Rot.X);
			v = m_RotateX(v, angle); // rotate Y,X,Z
				
			angle = cam_RotationCycle(Entity->ent_anim.Rot.Z);
			v = m_RotateZ(v, angle); // rotate Y,X,Z

			w.X = gCamera.Pos.X - Entity->ent_anim.Pos.X;
			w.Y = gCamera.Pos.Y - Entity->ent_anim.Pos.Y;
			w.Z = gCamera.Pos.Z - Entity->ent_anim.Pos.Z;

			Normalize(&w);
				
			val = 0.20f + (0.80f * PdtScalaire(&v, &w));
	
			R = R * val;
			G = G * val;
			B = B * val;
			IsBlend = false;
		}
		*/

		if(IsBlend)
		{
			pCurr->blend_src = GL_SRC_ALPHA;
			pCurr->blend_dst = GL_ONE_MINUS_SRC_ALPHA; //GL_DST_ALPHA;
		}
		if(pCurr->Type == FLAMES)
		{
			IsBlend = true;
                    #ifdef H_MAC
                        pCurr->blend_src = GL_ONE; //gl_GetBlendMode((int)bms.value);
			pCurr->blend_dst = GL_ONE_MINUS_SRC_COLOR; //gl_GetBlendMode((int)bmd.value); 
                    #else
			pCurr->blend_src = GL_SRC_ALPHA;	//gl_GetBlendMode((int)bms.value);
			pCurr->blend_dst = GL_DST_ALPHA;	//gl_GetBlendMode((int)bmd.value);
                    #endif
		}
		switch(Entity->entity_id)
		{
			case FIOLE1:
			case FIOLE2:
			case FIOLE3:
			case EXPLOSION:
			IsBlend = true;
            #ifdef H_MAC
            pCurr->blend_src = GL_ONE; 			//gl_GetBlendMode((int)bms.value);
			pCurr->blend_dst = GL_ONE_MINUS_SRC_COLOR; 	//gl_GetBlendMode((int)bmd.value); 
            #else
			pCurr->blend_src = GL_ONE;			//gl_GetBlendMode((int)bms.value);
			pCurr->blend_dst = GL_DST_ALPHA;		// gl_GetBlendMode((int)bmd.value);
            #endif
			break;
		}
		
		if(DistanceObjCam > 600)
		if(Entity->player_holded == false)
		if(Entity->entity_id != SMOKE)
		if(Entity->entity_id != EXPLOSION)
		{
			IsBlend = false;
		}
				
		gl_RenderFace(pCurr, PlayerEnt, IsBlend, R, G, B, A);

		R = R_copy;
		G = G_copy;
		B = B_copy;
		
		// speed hack
	//	if(IsBlend)
	//	if(m_Distance(Entity->ent_anim.Pos, gCamera.Pos) > 1500)
	//		break;
	}
	gForceNoDepthTest = false;
}

void gl_DrawHoldedEntity(pEntity Entity, pEntity PlayerEnt, float R, float G, float B)
{
	float	Xrecul;
	float	Zrecul;

	if(gIsServer && net_dedicated.value)
		return;

	if(!Entity)
		return;
	if(!player_view.value)
		return;
	if(!drawgun.value)
		return;

	if(gAllGoalCleared && ProgramState == PS_ENDSCORE)
		return;

	Xrecul = 0;

	if(gPlayerGunRecul)
		Zrecul = gPlayerGunRecul * 0.004f;
	else
		Zrecul = 0;

	if(Entity->entity_id == GUN1 || Entity->entity_id == GUN1B)
	{
		Zrecul -= 10.0f; // 15
	}
	else if(Entity->entity_id == GUN3 || Entity->entity_id == GUN3B)
	{
		Entity->ent_anim.Rot.Y = 274.0f; //280.0f;
		Zrecul *= 2.0f;
		Xrecul = 4.0f;
	}

	glPushMatrix();
	glLoadIdentity();

	glTranslatef(-gPlayerBobWalkstep*0.1f + Xrecul, -gPlayerBobWalkstep*0.3f, -gPlayerBobWalkstep*0.2f + Zrecul);

	gl_DrawEntity(Entity,PlayerEnt, R, G, B);
	glPopMatrix();
}

void gl_DrawActionBrush(pFace FaceList, vect_t campos, vect_t Mov, vect_t Rot, vect_t Siz)
{
	if(gIsServer && net_dedicated.value)
		return;

	glPushMatrix();
	{
		glTranslatef(Mov.X, Mov.Y, Mov.Z);
		glRotatef(Rot.X, 1.0, 0.0, 0.0);
		glRotatef(Rot.Y, 0.0, 1.0, 0.0);
		glRotatef(Rot.Z, 0.0, 0.0, 1.0);
		glScalef(Siz.X,Siz.Y,Siz.Z);
		gl_DrawFaces(FaceList,campos);
	}
	glPopMatrix();
}

void gl_DrawEntity(pEntity Entity, pEntity PlayerEnt, float R, float G, float B)
{
	float	Y_offset = 0;

	if(gIsServer && net_dedicated.value)
		return;

	gl_PushMatrix();
	{
		if(Entity->player_holded == false)
		if(	(Entity->entity_id >= H && Entity->entity_id <= N) ||
			(Entity->entity_id == GUN1 || Entity->entity_id == GUN2 || Entity->entity_id == GUN3) )
		{
			o_OscilleYprocess(Entity);
			Y_offset = Entity->ent_anim.oscille_Y;
		}
		else
		{
			Y_offset = 0;
		}
		glTranslatef(Entity->ent_anim.Pos.X, Entity->ent_anim.Pos.Y+Y_offset, Entity->ent_anim.Pos.Z);
	
		// hack
		if(Entity->entity_id == PERSO)
		{
			glRotatef(Entity->ent_anim.Rot.Y, 0.0, 1.0, 0.0);
		//	glRotatef(Entity->ent_anim.Rot.X, 1.0, 0.0, 0.0);
		//	glRotatef(Entity->ent_anim.Rot.Z, 0.0, 0.0, 1.0);
		}
		else
		{
			glRotatef(Entity->ent_anim.Rot.Y, 0.0, 1.0, 0.0);
			glRotatef(Entity->ent_anim.Rot.X, 1.0, 0.0, 0.0);
			glRotatef(Entity->ent_anim.Rot.Z, 0.0, 0.0, 1.0);
		}
		
		glScalef(Entity->ent_anim.Siz.X,Entity->ent_anim.Siz.Y,Entity->ent_anim.Siz.Z);
		gl_RenderEntity(Entity, PlayerEnt, R, G, B);
	}
	gl_PopMatrix();
}








void gl_RenderFace(Face_t *pFace, pEntity PlayerEnt, hBool isBlend, float R, float G, float B, float A)
{
	int		i;
	vect_t		rotmat;
	Color_t		color;
	Render_t	rend;

	if(!pFace)
		return;

	if(!draw_face.value)
		return;

	rend.is_blend = false;

	if(pFace->Type == FLAMES)
	{
		gl_PushMatrix();
		rotmat.X = 0;
		rotmat.Y = PlayerEnt->ent_anim.Rot.Y+90.0f;
		rotmat.Z = 0;
		gl_RotateMatrix(rotmat);
	}

	if(isBlend)
	{
		rend.is_blend = true;
		rend.blend_src_mode = pFace->blend_src; //gl_GetBlendMode((int)bms.value); //GL_SRC_ALPHA;
		rend.blend_dst_mode = pFace->blend_dst; //gl_GetBlendMode((int)bmd.value); //GL_DST_ALPHA;
	}
	else
	{
		rend.is_blend = false;
	}

	rend.is_texture = true;
	rend.texture_id = pFace->TextureID;
	rend.nvert = pFace->NumberOfVertices;

	rend.is_color = true;
	rend.color.R = R;
	rend.color.G = G;
	rend.color.B = B;
	rend.color.A = A;

	if(IsColorScreen)
	{
		rend.color.R *= WorldColorR;
		rend.color.G *= WorldColorG;
		rend.color.B *= WorldColorB;
	}

	rend.begin_mode = RENDER_TRIANGLE; //RENDER_POLYGON;
	rend.is_depthtest = true;
	if(gForceNoDepthTest)
	{
		rend.is_depthmask = false;
	}
	else
	{
		rend.is_depthmask = true;
	}
	rend.is_draw2D = false;

	for(i=0 ; i<rend.nvert ; i++)
	{
		rend.xyz[i][0] = pFace->Vertice[i].X;
		rend.xyz[i][1] = pFace->Vertice[i].Y;
		rend.xyz[i][2] = pFace->Vertice[i].Z;
		rend.uv[i][0] = pFace->Vertice[i].U;
		rend.uv[i][1] = pFace->Vertice[i].V;
	}
	gl_RenderPoly(rend);


	if(pFace->Type == FLAMES)
	{
		gl_PopMatrix();
	}

	color.R = WorldColorR;
	color.G = WorldColorG;
	color.B = WorldColorB;
	gl_SetGlobalColor(color);
}








int gl_InitGL()
{
#ifdef H_MAC
	GlobalTextureBind = -1;
	glClearColor(0,0,0,0);
	glColor3f(WorldColorR,WorldColorG,WorldColorB);
	glClearDepth(1.0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glShadeModel(GL_FLAT);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_TEXTURE_2D);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
	glClear(GL_DEPTH_BUFFER_BIT);
	glClear(GL_COLOR_BUFFER_BIT);
	gl_ChangeFOV(fov.value);
	gMultiTexturing = gl_initMultitexture();
#else
	GlobalTextureBind = -1;
	glClearColor(0,0,0,0);
	glColor3f(WorldColorR,WorldColorG,WorldColorB);
	glClearDepth(1.0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glShadeModel(GL_FLAT);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_TEXTURE_2D);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
	glClear(GL_DEPTH_BUFFER_BIT);
	glClear(GL_COLOR_BUFFER_BIT);
	gl_ChangeFOV(fov.value);
	gMultiTexturing = gl_initMultitexture();
#endif
	
	gl_vendor = (const char *) glGetString (GL_VENDOR);
	m_ConsPrint("GL_VENDOR: %s\n", gl_vendor);
	gl_renderer = (const char *) glGetString (GL_RENDERER);
	m_ConsPrint ("GL_RENDERER: %s\n", gl_renderer);
	gl_version = (const char *) glGetString (GL_VERSION);
	m_ConsPrint ("GL_VERSION: %s\n", gl_version);
	gl_extensions = (const char *) glGetString (GL_EXTENSIONS);
	m_ConsPrint ("GL_EXTENSIONS: %s\n", gl_extensions);

	return true;								
}

void gl_Perspective( GLdouble fovY, GLdouble aspect, GLdouble zNear, GLdouble zFar )
{
	const GLdouble pi = 3.1415926535897932384626433832795;
	GLdouble fW, fH;
	
	//fH = tan( (fovY / 2) / 180 * pi ) * zNear;
	fH = tan( fovY / 360 * pi ) * zNear;
	fW = fH * aspect;
	
	glFrustum( -fW, fW, -fH, fH, zNear, zFar );
}

void gl_ReSizeGLScene(int Width, int Height)
{
	if(Height == 0)
		Height = 1;
	glViewport(0, 0, Width, Height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gl_Perspective(45.0f,(GLfloat)Width/(GLfloat)Height,0.1f,100.0f);
	glMatrixMode(GL_MODELVIEW);
}

void gl_ChangeFOV(float fov)
{
	float	aspect;
	double  znear = 4.0f;
	double	zfar = MAX_CAMERA + 500; // hack
	double	left, right, top, bottom;

	if(fov>=175.0f)
		fov = 175.0f;
	else if(fov<40.0f)
		fov = 40.0f;

	aspect = (float)ScreenX[(int)videomode.value] / (float)ScreenY[(int)videomode.value];
	glViewport(0, 0, ScreenX[(int)videomode.value], ScreenY[(int)videomode.value]);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glMatrixMode(GL_PROJECTION);
		top = znear * tan( fov*0.5f * PI/360.0f );
		bottom = -top;
		left = bottom * aspect;
		right = top * aspect;
		glFrustum(left, right, bottom, top, znear, zfar);
	glMatrixMode(GL_MODELVIEW);							
}

void gl_ClearScreen()
{
	if(gIsServer && net_dedicated.value)
		return;

	if(gMenu_InitFirstSel)
		return;

	if(draw_tris.value)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		gPrintLine=0;
		return;
	}

	// HACK
	if(ProgramState == PS_GAME && gMapIntro == false)
	{
		glClear(GL_DEPTH_BUFFER_BIT);
	}
	else
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
	gPrintLine=0;
}

void gl_SwapBuffer()
{
	if(gIsServer && net_dedicated.value)
		return;
	/*
	if(ProgramState != PS_LOGO)
	{
		if(gMenu_InitFirstSel)
			return;
	}
	*/

#ifdef H_WINDOWS
	SwapBuffers(hDC);
#else
	//SDL_GL_SwapBuffers();
	//KWindow::flipBackBuffer();
#endif

//#ifdef H_LINUX
	//glutSwapBuffers();
//	sys_SwapBufferGLX();
	
//#endif

}

void gl_LoadSurfaceTexture(byte *surf, uint sformat, uint scolor_format1, uint scolor_format2, int w, int h, int id)
{
	GLuint	tid;
    
	tid = (GLuint)id;
	glGenTextures(1, &tid);
	glBindTexture(GL_TEXTURE_2D, tid);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	
	gluBuild2DMipmaps(GL_TEXTURE_2D, scolor_format1, w, h, scolor_format2, sformat, surf);
	//glTexImage2D( GL_TEXTURE_2D,0, scolor_format1, w, h, 0, scolor_format2, sformat, surf );

//	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, _nWidth, _nHeight, 0, GL_BGRA, GL_UNSIGNED_BYTE/*GL_UNSIGNED_INT_8_8_8_8_REV*/, 0);
	//	gluBuild2DMipmaps(GL_TEXTURE_2D, 4, _nHeight, _nHeight, GL_RGBA, GL_UNSIGNED_BYTE, _pData);

	
	
	s_free(surf);
}

void gl_BindTexture(int id)
{
	if(gIsServer && net_dedicated.value)
		return;
                
      //  #ifdef H_MAC
      //  id = id - 1;
      //  #endif

	if(GlobalTextureBind == id)
		return;
	GlobalTextureBind = id;
	glBindTexture(GL_TEXTURE_2D, id);
}

void gl_CameraView()
{
	glLoadIdentity();
	glRotatef(-gCamera.Rot.X, 1.0, 0.0, 0.0);
	glRotatef(-gCamera.Rot.Y, 0.0, 1.0, 0.0);
	glRotatef(-gCamera.Rot.Z, 0.0, 0.0, 1.0);
	glTranslatef(-gCamera.Pos.X, -gCamera.Pos.Y, -gCamera.Pos.Z);
}

void gl_SetClearColor(Color_t color)
{
	glClearColor(color.R, color.G, color.B, color.A);
}

void gl_SetGlobalColor(Color_t color)
{
	glColor3f(color.R, color.G, color.B);
}

void gl_SetGlobalColorA(Color_t color)
{
	glColor4f(color.R, color.G, color.B, color.A);
}

void gl_PushMatrix()
{
	glPushMatrix();
}

void gl_PopMatrix()
{
	glPopMatrix();
}

void gl_TranslateMatrix(vect_t trans)
{
	glTranslatef(trans.X, trans.Y, trans.Z);
}

void gl_RotateMatrix(vect_t rot)
{
	glRotatef(rot.X, 1.0, 0.0, 0.0);
	glRotatef(rot.Y, 0.0, 1.0, 0.0);
	glRotatef(rot.Z, 0.0, 0.0, 1.0);
}

void gl_ScaleMatrix(vect_t scale)
{
	glScalef(scale.X, scale.Y, scale.Z);
}

void gl_ProjectView(hBool IsProjOn, int ortho_w, int ortho_h)
{
	if(IsProjOn)
	{
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
		glOrtho(0, ortho_w, 0, ortho_h, -100, 100);
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();
	}
	else
	{
		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);
		glPopMatrix();
	}
}

void gl_RenderPoly(Render_t render)
{
	int index;

	if(gIsServer && net_dedicated.value)
		return;

	if(render.is_draw2D)
	{
		gl_ProjectView(true, render.ortho_w, render.ortho_h);
	}

	// texture information
	if(render.is_texture)
	{
		if(notextures.value)
			return;
		glEnable(GL_TEXTURE_2D);
		//glBindTexture(GL_TEXTURE_2D, render.texture_id);
		gl_BindTexture(render.texture_id);
	}
	else
		glDisable(GL_TEXTURE_2D);

	// depthtest information
	if(render.is_depthtest)
		glEnable(GL_DEPTH_TEST);
	else
		glDisable(GL_DEPTH_TEST);

	if(render.is_depthmask)
		glDepthMask(GL_TRUE);
	else
		glDepthMask(GL_FALSE);

	// blending information
	if(render.is_blend)
	{
		glEnable(GL_BLEND);
		glBlendFunc(render.blend_src_mode, render.blend_dst_mode);
	}
	else
		glDisable(GL_BLEND);

	// color information
	if(render.is_color)
	{
		glColor4f(render.color.R,render.color.G,render.color.B,render.color.A);
	}
	else
	{
		glColor4f(1,1,1,1);
	}

	// draw it
        if(render.is_draw2D || draw_face.value)
        {
            glBegin(render.begin_mode);
            for(index=0 ; index<render.nvert ; index++)
            {
		if(render.is_texture)
			glTexCoord2fv(render.uv[index]);
		glVertex3fv(render.xyz[index]);
            }
            glEnd();
        }

	// mode fils de fer
	if(draw_tris.value)
	{
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_TEXTURE_2D);
		glColor4f(1,1,1,1);
		glBegin(GL_LINE_LOOP);
		for(index=0 ; index<render.nvert ; index++)
			glVertex3fv(render.xyz[index]);
		glEnd();
	}


	if(render.is_draw2D)
	{
		gl_ProjectView(false, render.ortho_w, render.ortho_h);
	}
}


