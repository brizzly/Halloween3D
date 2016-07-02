
#include "asecomp.h"
#include "asereader.h"
#include "asetool.h"


extern	int		PlayerAngle;
extern	int		Nb_entities;
extern	int		gNb_Brush;
extern	int		gNb_ActionBrush;
extern	int		gNb_Faces;
extern	int		Nb_textures;
extern	int		Nb_object;
extern	char	TextureList2[TEXTURELOADED][TEXTURENAMEMAX];
extern	float	gStartX,gStartY,gStartZ;

bool			NewObjetProcessing=false;
bool			DetailObjet=false;
bool			ActionObjet=false;
bool			LavaObjet=false;
bool			InvisibleObjet=false;
bool			MoreInvisibleObjet=false;
bool			NoBSP=false;
int				EntityObjet=0;
int				EntityAngle=0;
TOBJET_TYPE		Entities[255];
int				NumVertex;
int				NumFace;
int				NumTVertex;
int				NumTFace;
int				pVertex;
int				iFace;
int				pTVertex;
int				pTFace;
int				FaceMaterialID;
float			StartX=0, StartY=0, StartZ=0;
int				Nb_Faces=0;
int				Nb_Textures_Loaded=0;
bool			Sub_Mat;
int				gVertice=0;
int				gNobsp_count=0;
int				gDetail=0;
int				gLava_count=0;
int				gInvisible=0;
int				gGun1=0;
int				gGun2=0;
int				gGun3=0;
int				gMonster1=0;
int				gAmmo1=0;
int				gAmmo2=0;
int				gAmmo3=0;
int				gPumpkin=0;
int				gFiole=0;
int				gLampe=0;
int				gLettre=0;

// ?? bizarre ??

float			uTile,vTile;
char			objetname[255];

void aseGetVertex(FILE *s, FILE *DestFile)
{
	int		index;
	float	X,Y,Z;

	fscanf (s, "%d", &index);
	fscanf (s, "%f %f %f",&X,&Z,&Y);
	Z = -Z;
	fprintf(DestFile,"%s %f %f %f\n",ASEVERTEX,X,Y,Z);
}

void aseGetTVertex (FILE *s, FILE *DestFile)
{
	int		index;
	float	U,V;

	fscanf (s, "%d", &index);
	fscanf (s, "%f %f",&U,&V);
	fprintf(DestFile,"%s %f %f\n",TVERTEX,U,V);
}

void aseGetFace(FILE *s, FILE *DestFile)
{
	int index;
	int A,B,C;
	int AB,BC,CA;
	int	smooth,mid;

	fscanf (s, "%d:", &index);
	fscanf (s, "\tA:\t%d B:\t%d C:\t%d \tAB:\t%d BC:\t%d CA:\t%d\t *MESH_SMOOTHING %d \t*MESH_MTLID %d",
		&A,&B,&C,&AB,&BC,&CA,&smooth,&mid);

	if(!Sub_Mat)
		mid = 666;

	fprintf(DestFile,"%s %d %d %d %d\n",ASEFACE,A,B,C,mid);
}

void aseGetTFace(FILE *s, FILE *DestFile)
{
	int index;
	int A,B,C;

	fscanf (s, "%d:", &index);
	fscanf (s, "%d %d %d",&A,&B,&C);
	fprintf(DestFile,"%s %d %d %d\n",TFACE,A,B,C);
}

float aseGetFloatVal (FILE *s)
{
	float v;

	fscanf (s, " %f", &v);
	return v;
}

void asePickData(FILE *file, FILE *DestFile)
{
	char	data[512];
	int		value;
	bool	DispDoublon=true;

	rewind(file);
	Nb_Textures_Loaded=0;
	Sub_Mat=false;

	while(!feof(file))
	{
		fscanf(file, "%s", &data);

		if(!strcmp(data, MATLIST))
		{
			fprintf(DestFile,"%s\n",MATLIST);
		}
		else if(!strcmp(data,SUBMAT))
		{
			Sub_Mat = true;
		}
		else if(!strcmp(data, NTEXTURES))
		{
			fscanf (file, "%d", &value);
			fprintf(DestFile,"%s %d\n",NTEXTURES,value);
		}
		else if(!strcmp(data, TEXTURE))
		{
			fgets(objetname,255,file);
			fputs(objetname,DestFile);
			Nb_Textures_Loaded++;
		}
		else if(!strcmp(data, NEWOBJET))
		{
			fprintf(DestFile,"\n%s\n",NEWOBJET);
		}
		else if(!strcmp(data,NODENAME))
		{
			DispDoublon = !DispDoublon;
			if(DispDoublon)
			{
				fprintf(DestFile,"%s ",NODENAME);
				fscanf(file,"%s", &objetname);
				fprintf(DestFile,"%s\n",objetname);

				//printf("%s\n",objetname);
			}
		}
		else if(!strcmp(data,NUM_VERTEX))
		{
			fscanf (file, "%d", &value);
			fprintf(DestFile,"%s %d\n",NUM_VERTEX,value);
		}
		else if(!strcmp(data,NUM_FACES))
		{
			fscanf (file, "%d", &value);
			fprintf(DestFile,"%s %d\n",NUM_FACES,value);
		}
		else if(!strcmp(data,VERTEX_LIST))
		{
			fprintf(DestFile,"%s\n",VERTEX_LIST);
		}
		else if (!strcmp (data, ASEVERTEX))
		{
			aseGetVertex(file,DestFile);
		}
		else if(!strcmp(data,FACE_LIST))
		{
			fprintf(DestFile,"%s\n",FACE_LIST);
		}
		else if (!strcmp (data, ASEFACE))
		{
			aseGetFace(file,DestFile);
		}
		else if(!strcmp(data,NUM_TVERTEX))
		{
			fscanf (file, "%d", &value);
			fprintf(DestFile,"%s %d\n",NUM_TVERTEX,value);
		}
		else if(!strcmp(data,TVERTEX_LIST))
		{
			fprintf(DestFile,"%s\n",TVERTEX_LIST);
		}
		else if(!strcmp (data, TVERTEX))
		{
			aseGetTVertex (file,DestFile);
		}
		else if(!strcmp(data,NUM_TFACES))
		{
			fscanf(file, "%d", &value);
			fprintf(DestFile,"%s %d\n",NUM_TFACES,value);
		}
		else if(!strcmp (data, TVFACE_LIST))
		{
			fprintf(DestFile,"%s\n",TVFACE_LIST);
		}
		else if(!strcmp (data, TFACE))
		{
			aseGetTFace(file,DestFile);
		}
		else if(!strcmp(data,OBJETTEXTID))
		{
			Sub_Mat = false;

			fscanf(file, "%d", &value);
			fprintf(DestFile,"%s %d\n",OBJETTEXTID,value);

			//printf("%d\n",value);
		}
		else if (!strcmp (data, NVERTEX))
		{
		}
		else if (!strcmp (data, FACE_NORMAL))
		{
		}
		else if (!strcmp (data, UTILE))
			uTile = aseGetFloatVal(file);
		else if (!strcmp (data, VTILE))
			vTile = aseGetFloatVal(file);
		else if (!strcmp (data, UOFFSET))
		{
		}
		else if (!strcmp (data, VOFFSET))
		{
		}
//		else
//			fgets (data, sizeof (data), file);
	}
}

void InitializePointers()
{
	NumVertex=0;
	NumFace=0;
	NumTVertex=0;
	NumTFace=0;

	pVertex=0;
	iFace=0;
	pTVertex=0;
	pTFace=0;
}

void ComputeStartPosition(FILE *DestFile)
{
	int		i;
	float	X=0, Y=0, Z=0;

	for(i=0;i<NumFace;i++)
	{
		X += vertex[face[i].id_vertex1].x;
		Y += vertex[face[i].id_vertex1].y;
		Z += vertex[face[i].id_vertex1].z;

		X += vertex[face[i].id_vertex2].x;
		Y += vertex[face[i].id_vertex2].y;
		Z += vertex[face[i].id_vertex2].z;

		X += vertex[face[i].id_vertex3].x;
		Y += vertex[face[i].id_vertex3].y;
		Z += vertex[face[i].id_vertex3].z;
	}

	StartX = X / (float)(NumFace*3);
	StartY = Y / (float)(NumFace*3);
	StartZ = Z / (float)(NumFace*3);
}

void ComputeEntityPosition(int EntityPos, int angle, int ID)
{
	int			i;
	float		X=0, Y=0, Z=0;
	VERTEX_TYPE	pos;

	memset(&pos,0,sizeof(VERTEX_TYPE));

	for(i=0;i<NumFace;i++)
	{
		X += vertex[face[i].id_vertex1].x;
		Y += vertex[face[i].id_vertex1].y;
		Z += vertex[face[i].id_vertex1].z;

		X += vertex[face[i].id_vertex2].x;
		Y += vertex[face[i].id_vertex2].y;
		Z += vertex[face[i].id_vertex2].z;

		X += vertex[face[i].id_vertex3].x;
		Y += vertex[face[i].id_vertex3].y;
		Z += vertex[face[i].id_vertex3].z;
	}

	pos.x = X / (float)(NumFace*3);
	pos.y = Y / (float)(NumFace*3);
	pos.z = Z / (float)(NumFace*3);

	Entities[EntityPos].Entity_Pos = pos;
	Entities[EntityPos].Entity_ID = ID;
	Entities[EntityPos].Entity_RotY = angle;

	if(ID == IDLAMPE || ID == IDMLAMPE)
	{
		Lights[Nb_Lights-1].pos.X = pos.x;
		Lights[Nb_Lights-1].pos.Y = pos.y;
		Lights[Nb_Lights-1].pos.Z = pos.z;
	}
}

void AddObjetData(FILE *DestFile, int type)
{
	int	i;

	for(i=0;i<NumFace;i++)
	{
		switch(type)
		{
		case DETAIL:
			fprintf(DestFile,"D\n");
			break;

		case ACTION:
			fprintf(DestFile,"A%d\n",ActionNumber);
			break;

		case WALL:
			fprintf(DestFile,"W\n");
			break;

		case BSPLESS:
			fprintf(DestFile,"L\n");
			break;

		case LAVA:
			fprintf(DestFile,"V\n");
			break;

		case INVI:
			fprintf(DestFile,"I\n");
			break;

		case INVI2:
			fprintf(DestFile,"N\n");
			break;

		default:
			fprintf(DestFile,"W\n");
			break;
		}

		gVertice+=3;

		fprintf(DestFile,"%f %f %f %f %f\n",
		vertex[face[i].id_vertex1].x,
		vertex[face[i].id_vertex1].y,
		vertex[face[i].id_vertex1].z,
		tvertex[tface[i].id_tface1].u,
		tvertex[tface[i].id_tface1].v);

		fprintf(DestFile,"%f %f %f %f %f\n",
		vertex[face[i].id_vertex2].x,
		vertex[face[i].id_vertex2].y,
		vertex[face[i].id_vertex2].z,
		tvertex[tface[i].id_tface2].u,
		tvertex[tface[i].id_tface2].v);

		fprintf(DestFile,"%f %f %f %f %f\n",
		vertex[face[i].id_vertex3].x,
		vertex[face[i].id_vertex3].y,
		vertex[face[i].id_vertex3].z,
		tvertex[tface[i].id_tface3].u,
		tvertex[tface[i].id_tface3].v);

		if(face[i].mat_id != 666)
			fprintf(DestFile,"%d\n",face[i].mat_id);
		else
			fprintf(DestFile,"%d\n",FaceMaterialID);

		if(type == ACTION)
			fprintf(DestFile,"%d\n",gNb_ActionBrush);
		else
			fprintf(DestFile,"%d\n",gNb_Brush);
	}
	Nb_Faces+=NumFace;
}

void aseOrganiseData(FILE *file, FILE *DestFile)
{
	char	data[255];
	int		i;

	fprintf(DestFile,"%d\n",Nb_Faces);
	fprintf(DestFile,"PLAYERPOS %f %f %f\n",StartX,StartY,StartZ);

	fprintf(DestFile,"%d\n",Nb_entities);
	for(i=0 ; i<Nb_entities ; i++)
	{
		fprintf(DestFile,"%d %d %f %f %f\n",
					Entities[i].Entity_ID,
					Entities[i].Entity_RotY,
					Entities[i].Entity_Pos.x,
					Entities[i].Entity_Pos.y,
					Entities[i].Entity_Pos.z);
	}
	
	while(!feof(file))
	{
		fgets(data,255,file);
		if(feof(file))
			break;
		fputs(data,DestFile);
	}
}

void aseComputeData(FILE *file, FILE *DestFile)
{
	char	data[255];
	char	name[255];
	char	texturename[255];
	int		Nb_objets=0;
	int		value;
	bool	start_position=false;
	int		mid;
	int		R, G, B;
	float	I;

	PlayerAngle=0;
	EntityAngle=0;
	Nb_entities = 0;
	gNb_ActionBrush=0;

	NewObjetProcessing=false;
	DetailObjet=false;
	ActionObjet=false;
	LavaObjet=false;
	InvisibleObjet=false;
	MoreInvisibleObjet=false;
	EntityObjet=0;
	InitializePointers();
	rewind(file);
	rewind(DestFile);

	while(!feof(file))
	{
		fscanf(file, "%s", &data);

		if(!strcmp(data,NODENAME))
		{
			fscanf(file, "%s", &name);

			if(!strncmp(name+1,STARTPLAYER,5))
			{
				sscanf(name+1+5,"%d",&PlayerAngle);
				printf("STARTPOS FOUND: %d\n",PlayerAngle);
				start_position=true;
			}
			if(!strncmp(name+1,STOPPLAYER,4))
			{
				printf("STOPPOS FOUND\n");
				EntityObjet = IDSTOP;
			}
			else if(!strncmp(name+1,ACTIONBRUSH,6))
			{
				sscanf(name+1+6,"%d",&ActionNumber);
				//printf("ACTION NUMBER: %d\n",ActionNumber);
				ActionObjet=true;
			}
			else if(!strncmp(name+1,DETAILOBJET,6))
			{
				gDetail++;
				DetailObjet=true;
			}
			else if(!strncmp(name+1,NOBSP,5))
			{
				NoBSP=true;
				gNobsp_count++;
			}
			else if(!strncmp(name+1,GUN1,4))
			{
				gGun1++;
				EntityObjet=IDGUN1;
			}
			else if(!strncmp(name+1,GUN2,4))
			{
				gGun2++;
				EntityObjet=IDGUN2;
			}
			else if(!strncmp(name+1,GUN3,4))
			{
				gGun3++;
				EntityObjet=IDGUN3;
			}
			else if(!strncmp(name+1,AMO1,4))
			{
				gAmmo1++;
				EntityObjet=IDAMO1;
			}
			else if(!strncmp(name+1,AMO2,4))
			{
				gAmmo2++;
				EntityObjet=IDAMO2;
			}
			else if(!strncmp(name+1,AMO3,4))
			{
				gAmmo3++;
				EntityObjet=IDAMO3;
			}
			else if(!strncmp(name+1,PUMPKIN,7))
			{
				if(name[1+7] != '\n')
				{
					sscanf(name+1+7,"%d",&EntityAngle);
					//printf("pumpkin angle: %d\n",EntityAngle);
				}
				gPumpkin++;
				EntityObjet=IDPUMPKIN;

			}
			else if(!strncmp(name+1,FIOLE1,6))
			{
				gFiole++;
				EntityObjet=IDFIOLE1;
			}
			else if(!strncmp(name+1,FIOLE2,6))
			{
				gFiole++;
				EntityObjet=IDFIOLE2;
			}
			else if(!strncmp(name+1,FIOLE3,6))
			{
				gFiole++;
				EntityObjet=IDFIOLE3;
			}
			else if(!strncmp(name+1,letterH,3))
			{
				gLettre++;
				EntityObjet=IDH;
			}
			else if(!strncmp(name+1,letterA,3))
			{
				gLettre++;
				EntityObjet=IDA;
			}
			else if(!strncmp(name+1,letterL,3))
			{
				gLettre++;
				EntityObjet=IDL;
			}
			else if(!strncmp(name+1,letterO,3))
			{
				gLettre++;
				EntityObjet=IDO;
			}
			else if(!strncmp(name+1,letterW,3))
			{
				gLettre++;
				EntityObjet=IDW;
			}
			else if(!strncmp(name+1,letterE,3))
			{
				gLettre++;
				EntityObjet=IDE;
			}
			else if(!strncmp(name+1,letterN,3))
			{
				gLettre++;
				EntityObjet=IDN;
			}
			else if(!strncmp(name+1,LAMPE,5))
			{
				gLampe++;
				if(name[1+5] == '_')
				{
					sscanf(name+1+5,"_(%d,%d,%d,%f)",&R,&G,&B,&I);
					//printf("lampe: %d %d %d %f\n",R,G,B,I);
				}
				Lights[Nb_Lights].R = R;
				Lights[Nb_Lights].G = G;
				Lights[Nb_Lights].B = B;
				Lights[Nb_Lights].I = I;
				Lights[Nb_Lights].type = 0;
				Nb_Lights++;
				EntityObjet=IDLAMPE;
			}
			else if(!strncmp(name+1,MLAMPE,6))
			{
				gLampe++;
				if(name[1+6] == '_')
				{
					sscanf(name+1+6,"_(%d,%d,%d,%f)",&R,&G,&B,&I);
					//printf("mlampe: %d %d %d %f\n",R,G,B,I);
				}
				Lights[Nb_Lights].R = R;
				Lights[Nb_Lights].G = G;
				Lights[Nb_Lights].B = B;
				Lights[Nb_Lights].I = I;
				Lights[Nb_Lights].type = 0;
				Nb_Lights++;
				EntityObjet=IDMLAMPE;
			}
			else if(!strncmp(name+1,MONSTRE,7))
			{
				gMonster1++;
				EntityObjet=IDMONSTRE1;
			}
			else if(!strncmp(name+1,MORBAK,6))
			{
				gMonster1++;
				EntityObjet=IDMORBAK;
			}
			else if(!strncmp(name+1,ZEBAT,5))
			{
				gMonster1++;
				EntityObjet=IDZEBAT;
			}
			else if(!strncmp(name+1,SUBZERAT,5))
			{
				gMonster1++;
				EntityObjet=IDSUBZERAT;
			}
			else if(!strncmp(name+1,ZEMUMMY,5))
			{
				gMonster1++;
				EntityObjet=IDZEMUMMY;
			}
			else if(!strncmp(name+1,LAVETAG,4))
			{
				gLava_count++;
				LavaObjet=true;
			}
			else if(!strncmp(name+1,INVISIBLE,9))
			{
				gInvisible++;
				InvisibleObjet=true;
			}
			else if(!strncmp(name+1,MOREINVISIBLE,13))
			{
				gInvisible++;
				MoreInvisibleObjet=true;
			}
		}
		else if(!strcmp(data, NEWOBJET))
		{
			if(!NewObjetProcessing)
			{
				NewObjetProcessing=true;
				//DetailObjet=false;
			}
			else
			{
				if(start_position)
				{
					start_position=false;
					ComputeStartPosition(DestFile);
				}
				else
				{
					if(DetailObjet)
					{
						DetailObjet=false;
						AddObjetData(DestFile,DETAIL);
					}
					else if(ActionObjet)
					{
						ActionObjet=false;
						AddObjetData(DestFile,ACTION);
						gNb_ActionBrush++;
					}
					else if(LavaObjet)
					{
						LavaObjet=false;
						AddObjetData(DestFile,LAVA);
					}
					else if(InvisibleObjet)
					{
						InvisibleObjet=false;
						AddObjetData(DestFile,INVI);
					}
					else if(MoreInvisibleObjet)
					{
						MoreInvisibleObjet=false;
						AddObjetData(DestFile,INVI2);
					}
					else if(NoBSP)
					{
						NoBSP=false;
						AddObjetData(DestFile,BSPLESS);
					}
					else if(EntityObjet)
					{
						ComputeEntityPosition(Nb_entities,EntityAngle,EntityObjet);
						EntityObjet=0;
						EntityAngle=0;
						Nb_entities++;
					}
					else
					{
						AddObjetData(DestFile,WALL);
						gNb_Brush++;
					}
				}
			}

			InitializePointers();
			Nb_objets++;
		}
		else if(!strcmp(data,NUM_VERTEX))
		{
			fscanf(file," %d",&NumVertex);
		}
		else if(!strcmp(data,NUM_FACES))
		{
			fscanf(file," %d",&NumFace);
		}
		else if(!strcmp(data,ASEVERTEX))
		{
			fscanf(file, "%f %f %f", &vertex[pVertex].x, &vertex[pVertex].y, &vertex[pVertex].z);
			pVertex++;
		}
		else if(!strcmp(data,ASEFACE))
		{
			fscanf(file, "%d %d %d %d", &face[iFace].id_vertex1, &face[iFace].id_vertex2, &face[iFace].id_vertex3,&mid);
			face[iFace].mat_id = mid;
			iFace++;
		}
		else if(!strcmp(data,TVERTEX))
		{
			fscanf(file, "%f %f", &tvertex[pTVertex].u, &tvertex[pTVertex].v);
			pTVertex++;
		}
		else if(!strcmp(data,TFACE))
		{
			fscanf(file, "%d %d %d", &tface[pTFace].id_tface1, &tface[pTFace].id_tface2, &tface[pTFace].id_tface3);
			pTFace++;
		}
		else if(!strcmp(data,OBJETTEXTID))
		{
			fscanf(file, "%d", &FaceMaterialID);
		}
		else if(!strcmp(data,NTEXTURES))
		{
			fscanf(file, "%d", &value);

			fgets(texturename,255,file);

			value = Nb_Textures_Loaded;
			fprintf(DestFile,"%d\n",value);
			while(value--)
			{
				fgets(texturename,255,file);
				fputs(texturename,DestFile);
			}
			fprintf(DestFile,"\n");
		}
	}

	if(start_position)
	{
		start_position=false;
		ComputeStartPosition(DestFile);
	}
	else
	{	
		if(DetailObjet)
		{
			DetailObjet=false;
			AddObjetData(DestFile,DETAIL);
		}
		else if(ActionObjet)
		{
			ActionObjet=false;
			AddObjetData(DestFile,ACTION);
			gNb_ActionBrush++;
		}
		else if(LavaObjet)
		{
			LavaObjet=false;
			AddObjetData(DestFile,LAVA);
		}
		else if(InvisibleObjet)
		{
			InvisibleObjet=false;
			AddObjetData(DestFile,INVI);
		}
		else if(MoreInvisibleObjet)
		{
			MoreInvisibleObjet=false;
			AddObjetData(DestFile,INVI2);
		}
		else if(NoBSP)
		{
			NoBSP=false;
			AddObjetData(DestFile,BSPLESS);
		}
		else if(EntityObjet)
		{
			ComputeEntityPosition(Nb_entities,EntityAngle,EntityObjet);
			EntityObjet=0;
			EntityAngle=0;
			Nb_entities++;
		}
		else
		{
			AddObjetData(DestFile,WALL);
			gNb_Brush++;
		}
	}
}

bool f_ChargerASE(char *nom, int objet)
{
	FILE	*file;
	FILE	*DestFile;

	file = fopen (nom, "r");
	if(file==NULL)
		return false;

	Rename(nom,"tmp");
	remove(nom);
	DestFile = fopen(nom,"wt");
	if(DestFile==NULL)
		return false;

	asePickData(file,DestFile);

	fclose(file);
	fclose(DestFile);
	file = fopen(nom,"r");
	if(file==NULL)
		return false;

	Rename(nom,"ver");
	remove(nom);
	DestFile = fopen(nom,"wt");
	if(DestFile==NULL)
		return false;

	gVertice=0;
	aseComputeData(file,DestFile);

	fclose(file);
	fclose(DestFile);
	file = fopen(nom,"r");
	if(file==NULL)
		return false;

	Rename(nom,"xyz");
	remove(nom);
	DestFile = fopen(nom,"wt");
	if(DestFile==NULL)
		return false;

	aseOrganiseData(file,DestFile);

	fclose(file);
	fclose(DestFile);

	Rename(nom,"ver");
	remove(nom);
	Rename(nom,"tmp");
	remove(nom);

	return true;
}