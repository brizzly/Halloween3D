/*
 *
 *	RADcomp - Radiosity Compiler for lightmaps
 *	(C) Jadeware 2001. ALL RIGHTS RESERVED
 *
 */

#include "radcomp.h"


int			Nb_faces=0;
face_t		gFaceList = NULL;
lm_t		LmList = NULL;
vect_t		LampePos[MAX_LAMPES];
int			Nb_lightmaps;
char		LightmapsList[255][255];
char		gLighMapDir[255];


#include "bspfile.h"
#include "maths.h"
#include "patch.h"


void main(int argvc, char **argv)
{
	FILE	*file;
	char	name[255];
	lm_t	NewLm = NULL;
//	lm_t	Lm;
	int		i;
//	int		j;
	int		index;
	face_t	pCurr;
	char	path[255];
	char	temp[255];
	char	*c;
//	vect_t	pos;
	int		LMQuality;
	double	StartTime;

	printf("\nRADCOMP [Version 0.21]\n");
	printf("(C) Copyright 2001 Jadeware Corp.\n");
	printf("=================================\n");

	if(argvc < 2)
		Quit("Too few arguments");

	LMQuality = 8;
	if(argvc > 2)
	{
		LMQuality = atoi(argv[1]);
		if(LMQuality != 4 && LMQuality != 8 && LMQuality != 16 && LMQuality != 32)
			Quit("Lightmap quality parameters invalid (4,8, 16 or 32)");
	}
	printf("Lightmap Quality : %d\n",LMQuality);
	nPATCH_X = LMQuality;
	nPATCH_Y = LMQuality;
	strcpy(name,argv[argvc-1]);

	// enleve l'extension
	strcpy(temp,name);
	for(c=temp ; (*c)!='\0' ; c++)
	if((*c) == '.')
	{
		*c = '\0';
		break;
	}
	sprintf(gLighMapDir,"../textures/systex/%s/",temp);
	_mkdir(gLighMapDir);

	printf("Remove old lightmaps files\n");
	remove(DEBUGFILE);
	for(i=0 ; i<999 ; i++)
	{
		sprintf(path,"%slm_big_%.3d.tga",gLighMapDir,i);
		remove(path);
	}

	FlamePos = GetLampeFlameCenterPos();
	//printf("FlamePos(%f,%f,%f)\n",FlamePos.X,FlamePos.Y,FlamePos.Z);

	if(!LoadBSPFromFile(name))
		Quit("Bad file. Put a *.map file");
	//printf("Load map file ok\n");

	printf("Create LightMap\n");
	StartTime = FloatTime();
	index=0;
//	for(j=0 ; j<gNb_Leafs ; j++)
//	{
//		gFaceList = LeafArray[j].faceliste;
//		Nb_faces = LeafArray[j].Nb_faces;

//		for(i=0, pCurr=gFaceList ; i<Nb_faces ; i++,pCurr=pCurr->Next)
		for(i=0 ; i<gNb_faces ; i++)
		{
			pCurr = & FaceArray[i];
			NewLm = CreateLightMap(i+index);
			NewLm->face = pCurr;
			NewLm->face->LightID = i+index;
			LmList = AddToList(LmList,NewLm);
		}
		index += i;
//	}

	patch_Init();

	printf("Computing Lights");
	printf("\n");
	//printf("==================\n");
	for(i=0 ; i<Nb_Lights ; i++)
	{
		//printf(".");
		printf("Computing Light #%d / %d\n",i,Nb_Lights);
		for(NewLm=LmList ; NewLm ; NewLm=NewLm->next)
			ComputeLightMap(NewLm,Lights[i]);
	}
	printf("\n");
	//printf("==================\n");

	/*
	printf("First pass");
	for(NewLm=LmList ; NewLm ; NewLm=NewLm->next)
	{
		printf(".");
		for(j=0 ; j<nPATCH_Y ; j++)
		for(i=0 ; i<nPATCH_X ; i++)
		{
			pos = NewLm->patch[i+(j*nPATCH_X)].pos;

			for(Lm=LmList ; Lm ; Lm=Lm->next)
				ComputeLightMap(Lm,pos,NewLm->patch[i+(j*nPATCH_X)].energy * 0.05f);  // NewLm->reflection
		}
	}
	printf("\n");
	*/

	WritePtsFile(LmList);
	WriteLightMaps(LmList);

	WriteRadFile(LmList,name);
	DeleteLightMaps(LmList);

	file=fopen("output.txt","w");
	fprintf(file,"gNb_lampes: %d\n",gNb_lampes);
	fprintf(file,"Nb_lightmaps: %d\n",Nb_lightmaps);
	fclose(file);

	DispTimeElapsed(StartTime);
}

void Quit(char *txt)
{
	printf("%s\n",txt);
	exit(0);
}

void Rename(char *name, char *extension)
{
	int	Ptr=0;

	while(name[Ptr]!='.')
	{
		if(name[Ptr] == '\0')
			return;
		Ptr++;
	}
	strncpy(&name[Ptr+1],extension,3);
}

lm_t AddToList(lm_t List, lm_t Elem)
{
	if(!Elem)
		return List;
	Elem->next = List;
	return Elem;
}

lm_t CreateLightMap(int id)
{
	byte 			*pixels;
	unsigned int	surface_size=0;
	lm_t			NewLm = NULL;
	int				i;

	surface_size = (LM_X*LM_Y*3);
	pixels = (byte*)malloc(surface_size);
	memset(pixels,0,surface_size);

	NewLm = malloc(sizeof(LM));
	memset(NewLm,0,sizeof(LM));
	NewLm->id = id;
	NewLm->next = NULL;
	NewLm->surface = pixels;
	NewLm->reflection = REFLECTION;

	for(i=0 ; i<NB_PATCHS ; i++)
	{
		NewLm->patch[i].energyR = MIN_ENERGY;
		NewLm->patch[i].energyG = MIN_ENERGY;
		NewLm->patch[i].energyB = MIN_ENERGY;
	}

	return NewLm;
}

void WriteLightMaps(lm_t LmList)
{
	lm_t	pCurr;
	char	path[255];
	char	temp[255];
	FILE	*fichier;
	int		pos;
	int		line, col;
	int		i;
	byte	*buffer;
	int		Offset;
	bool	Repeat;
	int		Id;
	int		nRepeat;

	Offset=0;
	nRepeat=0;
	Repeat=false;

	printf("WriteLightMaps\n");
	Nb_lightmaps = 0;
	for(i=0 ; i<255 ; i++)
		memset(LightmapsList[i],0,255);

label:

	Nb_lightmaps++;

	buffer = malloc(18+sizeof(byte)*512*512*3);
	memset(buffer,0,18+sizeof(byte)*512*512*3);

	buffer[2] = 2;
	buffer[12] = 512 & 255;
	buffer[13] = 512 >> 8;
	buffer[14] = 512 & 255;
	buffer[15] = 512 >> 8;
	buffer[16] = 24;
	
	for(pCurr=LmList ; pCurr ; pCurr=pCurr->next)
	{
		Id = pCurr->id - Offset;
		if(Id<0)
			continue;
		if(Id > 255)
		{
			Repeat=true;
			continue;
		}
		col = Id % 16;
		line = Id / 16;
		pos = 18 + ( line * (16*32*3*32) ) + ( col * (32*3) );

		for(i=0 ; i<32 ; i++)
		{
			memcpy(buffer+pos,pCurr->surface+(i*32*3),32*3);
			pos += 32*16*3;
		}
	}

	memset(temp,0,255);
	
	sprintf(temp,"lm_big_%.3d.tga",Nb_lightmaps-1);
	sprintf(LightmapsList[Nb_lightmaps-1],temp);

	sprintf(path,"%slm_big_%.3d.tga",gLighMapDir,Nb_lightmaps-1);

	fichier=fopen(path,"wb");
	fwrite(buffer,18+sizeof(byte)*512*512*3,1,fichier);
	fclose(fichier);
	free(buffer);

	if(Repeat)
	{
		Repeat=false;
		nRepeat++;
		Offset += 256;
		goto label;
	}
}

void DeleteLightMaps(lm_t LmList)
{
	lm_t	pCurr;

	while(LmList)
	{
		pCurr = LmList;
		LmList = LmList->next;
		free(pCurr->surface);
		free(pCurr);
	}
}

//#define REAL_LIGHT

void ComputeLightMap(lm_t Lm, LIGHT Light)
{
	int		i, j, k;
	float	d;
	float	d2;
	vect_t	v;
	vect_t	w;
	RECTANGLE	Face;
//	face_t	face;
//	face_t	facelist;
	bool	vis;
	vect_t	dir;
	float	power_factor;
	float	inv_power_factor;
	vect_t	lightsrc_pos;
	float	Rfactor, Gfactor, Bfactor;

	if(!Lm)
		return;

	lightsrc_pos = Light.pos;
	power_factor = Light.I;
	if(!power_factor)
		return;
	inv_power_factor = 1.0f/power_factor;

	Rfactor = Light.R * MAX_ENERGY_INV;
	Gfactor = Light.G * MAX_ENERGY_INV;
	Bfactor = Light.B * MAX_ENERGY_INV;
//	Rfactor = Light.R / MAX_ENERGY;
//	Gfactor = Light.G / MAX_ENERGY;
//	Bfactor = Light.B / MAX_ENERGY;

//#ifdef REAL_LIGHT
	v = Lm->face->Plan.PointOnPlane;
	dir.X = v.X - lightsrc_pos.X;
	dir.Y = v.Y - lightsrc_pos.Y;
	dir.Z = v.Z - lightsrc_pos.Z;
	Normalize(&dir);
	if(PdtScalaire(&dir,&Lm->face->Plan.VecteurNormal) >= 0)
		return;
//#endif

	for(j=0 ; j<nPATCH_Y ; j++)
	for(i=0 ; i<nPATCH_X ; i++)
	{
		v = Lm->patch[i+(j*nPATCH_X)].pos;

	//	if(v.X == lightsrc_pos.X && v.Y == lightsrc_pos.Y && v.Z == lightsrc_pos.Z)
	//		continue;

		d = m_Distance(v,lightsrc_pos);
		d *= inv_power_factor;
		if(d<0)
			Quit("error #1");
		if(d>255)
			continue;

		vis = true;

//#ifdef REAL_LIGHT
		for(k=0 ; k<gNb_faces ; k++)
		{
		//	if(!vis)
		//		break;
			Face = FaceArray[k];
			if(&Face == Lm->face)
				continue;

		//	d2 = m_Distance(Face.Plan.PointOnPlane, lightsrc_pos);
		//	if(d2 > 300.0f)
		//		continue;		

			if(PdtScalaire(&dir,&Face.Plan.VecteurNormal) >= 0)
				continue;

			if(IsLightIntersect(&v,&lightsrc_pos,&Face))
			{
				vis = false;
				break;
			}
		}
//#endif

		if(!vis)
			continue;
		d = 255 - d;
		Lm->patch[i+(j*nPATCH_X)].energyR += (d*Rfactor);
		Lm->patch[i+(j*nPATCH_X)].energyG += (d*Gfactor);
		Lm->patch[i+(j*nPATCH_X)].energyB += (d*Bfactor);

		if(Lm->patch[i+(j*nPATCH_X)].energyR > MAX_ENERGY)
			Lm->patch[i+(j*nPATCH_X)].energyR = MAX_ENERGY;
		if(Lm->patch[i+(j*nPATCH_X)].energyG > MAX_ENERGY)
			Lm->patch[i+(j*nPATCH_X)].energyG = MAX_ENERGY;
		if(Lm->patch[i+(j*nPATCH_X)].energyB > MAX_ENERGY)
			Lm->patch[i+(j*nPATCH_X)].energyB = MAX_ENERGY;
	}
}

void WritePtsFile(lm_t LmList)
{
	int		i, j;
	lm_t	pLm;
	int		pos, p, k;

	printf("WritePtsFile\n");

	for(pLm=LmList ; pLm ; pLm=pLm->next)
	for(j=0 ; j<nPATCH_Y ; j++)
	for(i=0 ; i<nPATCH_X ; i++)
	{
		if(pLm->patch[i+(j*nPATCH_X)].energyR  == JOKER)
		{
			pLm->patch[i+(j*nPATCH_X)].energyR = 0;
		}
		else
		{
			// HACK, sinon on a des caracteres systemes qui font des erreurs de fichiers!!
			if(pLm->patch[i+(j*nPATCH_X)].energyR < MIN_ENERGY)
				pLm->patch[i+(j*nPATCH_X)].energyR = MIN_ENERGY;
			else if(pLm->patch[i+(j*nPATCH_X)].energyR > MAX_ENERGY)
				pLm->patch[i+(j*nPATCH_X)].energyR = MAX_ENERGY;
		}

		if(pLm->patch[i+(j*nPATCH_X)].energyG  == JOKER)
		{
			pLm->patch[i+(j*nPATCH_X)].energyG = 0;
		}
		else
		{
			// HACK, sinon on a des caracteres systemes qui font des erreurs de fichiers!!
			if(pLm->patch[i+(j*nPATCH_X)].energyG < MIN_ENERGY)
				pLm->patch[i+(j*nPATCH_X)].energyG = MIN_ENERGY;
			else if(pLm->patch[i+(j*nPATCH_X)].energyG > MAX_ENERGY)
				pLm->patch[i+(j*nPATCH_X)].energyG = MAX_ENERGY;
		}

		if(pLm->patch[i+(j*nPATCH_X)].energyB  == JOKER)
		{
			pLm->patch[i+(j*nPATCH_X)].energyB = 0;
		}
		else
		{
			// HACK, sinon on a des caracteres systemes qui font des erreurs de fichiers!!
			if(pLm->patch[i+(j*nPATCH_X)].energyB < MIN_ENERGY)
				pLm->patch[i+(j*nPATCH_X)].energyB = MIN_ENERGY;
			else if(pLm->patch[i+(j*nPATCH_X)].energyB > MAX_ENERGY)
				pLm->patch[i+(j*nPATCH_X)].energyB = MAX_ENERGY;
		}
	}

	for(pLm=LmList ; pLm ; pLm=pLm->next)
	{
		for(i=0 ; i<nPATCH_X ; i++)
		for(j=0 ; j<nPATCH_Y ; j++)
		{
			pos = (i*(32/nPATCH_X)) + (j*32*(32/nPATCH_Y));
			pos *= 3;
			for(k=0 ; k<(32/nPATCH_Y) ; k++)
			{
				for(p=0 ; p<(32/nPATCH_X)*3 ; p+=3)
				{
					pLm->surface[p+pos+0] = (byte)pLm->patch[i+(j*nPATCH_X)].energyB;
					pLm->surface[p+pos+1] = (byte)pLm->patch[i+(j*nPATCH_X)].energyG;
					pLm->surface[p+pos+2] = (byte)pLm->patch[i+(j*nPATCH_X)].energyR;
				}
				pos += 32*3;
			}
		}
	}
}

void WriteRadFile(lm_t LmList, char *name)
{
	FILE	*file, *file2;
//	lm_t	pLm;
//	face_t	pCurr;
	int		i, j, k;
	char	line[255];
	char	temp[255];
	RECTANGLE	Face;

	printf("WriteRadFile\n");

	file=fopen(name,"rb");
	Rename(name,"rad");
	remove(name);
	file2=fopen(name,"wb");

	while(!feof(file))
	{
		fgets(line,255,file);
		if(!strncmp("<LIGHTMAPS>",line,11))
			break;
		fputs(line,file2);

		if(feof(file))
			break;
	}

	fprintf(file2,"<LIGHTMAPS> %d\n",Nb_lightmaps);
	for(i=0 ; i<Nb_lightmaps ; i++)
		fprintf(file2,"%s\n",LightmapsList[i]);
	
	while(!feof(file))
	{
		fgets(line,255,file);
		if(!strncmp("<NODES>",line,7))
			break;
		if(feof(file))
			break;
	}
	fputs(line,file2);

	while(!feof(file))
	{
		fgets(line,255,file);
		if(!strncmp("<FACES>",line,7))
			break;
		fputs(line,file2);

		if(feof(file))
			break;
	}

	fputs(line,file2);

	for(k=0 ; k<gNb_faces ; k++)
	{
		Face = FaceArray[k];

		fprintf(file2,"%d %d %d %d %d\n",Face.FaceID,Face.NumberOfVertices,Face.textId,Face.LightID,Face.Type);
		for(i=0 ; i<Face.NumberOfVertices ; i++)
		{
			fprintf(file2,"%f %f %f %f %f %f %f\n",
					Face.point[i].X,
					Face.point[i].Y,
					Face.point[i].Z,
					Face.point[i].U,
					Face.point[i].V,
					Face.point[i].U_lm,
					Face.point[i].V_lm);
		}
	}
	
	while(!feof(file))
	{
		fgets(line,255,file);
		if(!strncmp("<LEAFS>",line,7))
	//	if(!strncmp("<ACTION BRUSH>",line,14))
			break;
		if(feof(file))
			break;
	}

	fputs(line,file2);

	while(!feof(file))
	{
		fgets(line,255,file);
		fputs(line,file2);
		if(feof(file))
			break;
	}

	fclose(file);
	fclose(file2);

	Rename(name,"map");
	remove(name);
	strcpy(temp,name);
	Rename(temp,"rad");
	rename(temp,name);

	return;



	/*
//	Rename(name,"tmp");
//	remove(name);

	for(pLm=LmList ; pLm ; pLm=pLm->next)
	{
		pCurr = pLm->face;
		fprintf(file2,"%d %d %d %d\n",pCurr->NumberOfVertices,pCurr->textId,pLm->id,pCurr->Type);
		for(i=0 ; i<pCurr->NumberOfVertices ; i++)
		{
			fprintf(file2,"%f %f %f %f %f %f %f\n",
				pCurr->point[i].X,
				pCurr->point[i].Y,
				pCurr->point[i].Z,
				pCurr->point[i].U,
				pCurr->point[i].V,
				pCurr->point[i].U_lm,
				pCurr->point[i].V_lm);
		}
	}
	fclose(file2);
	*/
}

double FloatTime()
{
	time_t	t;
	
	time (&t);
	
	return t;
}

void DispTimeElapsed(double TimeStart)
{
	int	hour, min, sec;

	sec = (int)(FloatTime()-TimeStart);
	min = sec/60;
	sec %= 60;
	hour = min/60;
	min %= 60;
	printf("- %d:%d'%d -\n",hour,min,sec);
}


