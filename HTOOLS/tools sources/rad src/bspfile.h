
#include "maths.h"

#define	FLAMETAG	"flame"

int				gNb_lampes;
int				gNb_nodes;
int				gNb_faces;
int				gNb_Leafs;
int				gPlayerDir;
int				Nb_entities;
int				Nb_textures;
int				Nb_Lights;
float			StartX=0, StartY=0, StartZ=0;
noeud_t			NodeArray[1];
RECTANGLE		FaceArray[10000];
leafs_t			LeafArray[8000];
ENTITY			Entities[255];
LIGHT			Lights[255];
vect_t			FlamePos;


face_t liste_Delete(face_t List)
{
	face_t	Face;

	while(List)
	{
		Face = List;
		List = List->Next;
		free(Face);
	}
	return NULL;
}

void CenterList(face_t List)
{
	BOUNDINGBOX bbox;
	vect_t		center;
	face_t		Poly;
	int			i;

	ClearBound(&bbox);
	GetBound(List,&bbox);
	center.X = (bbox.BoxMin.X + bbox.BoxMax.X)/2.0f;
	center.Y = (bbox.BoxMin.Y + bbox.BoxMax.Y)/2.0f;
	center.Z = (bbox.BoxMin.Z + bbox.BoxMax.Z)/2.0f;
	Poly = List;
	while(Poly)
	{
		for(i=0 ; i<Poly->NumberOfVertices ; i++)
		{
			Poly->point[i].X -= center.X;
			Poly->point[i].Y -= center.Y;
			Poly->point[i].Z -= center.Z;
		}
		Poly = Poly->Next;
	}
}

vect_t GetLampeFlameCenterPos()
{
	FILE	*file;
	vect_t	v = {0,0,0};
	int		i, j, k;
	face_t	Face;
	face_t	ListFace;
	int		FlameTextID=-1;
	float	no_use;
	char	temp[255];

	file = fopen("../entity/lampe.ent","r");
	if(!file)
		Quit("can't open lampe.ent entity file");

	fscanf(file,"<ENTITY>\n");
	fscanf(file,"<TEXTURES> %d\n",&j);
	for(k=0 ; k<j ; k++)
	{
		fscanf(file,"%s\n",&temp);
		if(!strncmp(temp,FLAMETAG,5))
			FlameTextID = k;
	}

	if(FlameTextID == -1)
		Quit("lampe.ent entity has no flame for lightling");

	ListFace = NULL;
	while(!feof(file))
	{
		Face = malloc(sizeof(RECTANGLE));

		fscanf(file,"%d %d\n",&Face->NumberOfVertices,&Face->textId);

		if(Face->NumberOfVertices > 8)
			Quit("Inavlide vertex ");

		for(i=0 ; i<Face->NumberOfVertices ; i++)
		{
			fscanf(file,"%f %f %f %f %f %f %f\n",
					&Face->point[i].X,
					&Face->point[i].Y,
					&Face->point[i].Z,
					&Face->point[i].U,
					&Face->point[i].V,
					&no_use,
					&no_use);
		}	
		Face->Next = ListFace;
		ListFace = Face;
	}
	fclose(file);

	if(!ListFace)
		Quit("erreur interne #2");

	CenterList(ListFace);

	i=0;
	for(Face=ListFace ; Face ; Face=Face->Next)
	if(Face->textId == FlameTextID)
	{
		for(j=0 ; j<Face->NumberOfVertices ; j++)
		{
			v.X += Face->point[j].X;
			v.Y += Face->point[j].Y;
			v.Z += Face->point[j].Z;
		}
		i += Face->NumberOfVertices;
	}

	if(!i)
		Quit("Erreur interne#1");

	v.X /= (float)i;
	v.Y /= (float)i;
	v.Z /= (float)i;

	ListFace = liste_Delete(ListFace);

	return v;
}


bool LoadBSPFromFile(char *name)
{
	FILE		*file;
	face_t		pCurr;
	face_t		FaceList;
	char		line[255];
	int			i, j, k;
	int			Id;
//	vect_t		pos;
//	int			id_face;
	RECTANGLE	Face;

	file = fopen(name,"r");
	if(!file)
		return false;

	fscanf(file,"%s\n",line);
	if(strncmp("<MAP>",line,10) != 0)
	{
		fclose(file);
		return false;
	}
	fscanf(file,"<PLAYERPOS> %f %f %f\n",&StartX,&StartY,&StartZ);
	fscanf(file,"<PLAYERDIR> %d\n",&gPlayerDir);

	//	Entitees
	gNb_lampes = 0;
	fscanf(file,"<ENTITIES> %d\n",&Nb_entities);
	for(j=0 ; j<Nb_entities ; j++)
	{
		fscanf(file,"%d %d %f %f %f\n",&Entities[j].id,&Entities[j].angleY,&Entities[j].pos.X,&Entities[j].pos.Y,&Entities[j].pos.Z);
		if(Entities[j].id == 11)	// lampe
		{
			/*
			// HACK : since light source is objet pos..
			pos.X = Entities[j].pos.X + FlamePos.X;
			pos.Y = Entities[j].pos.Y + FlamePos.Y;
			pos.Z = Entities[j].pos.Z + FlamePos.Z;
			LampePos[gNb_lampes] = pos;
			gNb_lampes++;
			if(gNb_lampes >= MAX_LAMPES)
				Quit("Error too much lampes!");
			*/
		}
	}

/*
	if(gNb_lampes)
		printf("lampes : %d\n",gNb_lampes);
	else
	{
		fclose(file);
		file = fopen("output.txt","w");
		fprintf(file,"No lights sources!!\n");
		fclose(file);
		Quit("No lights sources!!");
	}
*/

	//	Textures
	fscanf(file,"<TEXTURES> %d\n",&Nb_textures);
	for(k=0 ; k<Nb_textures ; k++)
		fscanf(file,"%s\n",line);

	// Lights sources
	fscanf(file,"<LIGHTS> %d\n",&Nb_Lights);
	if(Nb_Lights >= MAX_LAMPES)
		Quit("Error too much lights!");
	if(Nb_Lights)
		printf("lights : %d\n",Nb_Lights);
	else
	{
		fclose(file);
		file = fopen("output.txt","w");
		fprintf(file,"No lights sources!!\n");
		fclose(file);
		Quit("No lights sources!!");
	}
	for(k=0 ; k<Nb_Lights ; k++)
		fscanf(file,"%f %f %f %d %d %d %f %d\n",
						&Lights[k].pos.X,
						&Lights[k].pos.Y,
						&Lights[k].pos.Z,
						&Lights[k].R,
						&Lights[k].G,
						&Lights[k].B,
						&Lights[k].I,
						&Lights[k].type);

	//	Lightmaps
	fscanf(file,"<LIGHTMAPS> %d\n",&Nb_lightmaps);
	for(k=0 ; k<Nb_lightmaps ; k++)
		fscanf(file,"%s\n",line);

	fscanf(file,"<NODES> %d\n",&gNb_nodes);
	for(i=0 ; i<gNb_nodes ; i++)
	{
		fscanf(file,"NODE[%d] LEAF[%d] FNODE[%d] BNODE[%d] POP[%f %f %f] VN[%f %f %f]\n",
			&j,
			&NodeArray[0].Feuille,
			&NodeArray[0].Front,
			&NodeArray[0].Back,
			&NodeArray[0].Plan.PointOnPlane.X,
			&NodeArray[0].Plan.PointOnPlane.Y,
			&NodeArray[0].Plan.PointOnPlane.Z,
			&NodeArray[0].Plan.VecteurNormal.X,
			&NodeArray[0].Plan.VecteurNormal.Y,
			&NodeArray[0].Plan.VecteurNormal.Z);
	}

	fscanf(file,"<FACES> %d\n",&gNb_faces);
	for(k=0 ; k<gNb_faces ; k++)
	{
	//	pCurr = s_malloc(sizeof(Face_t));
	//	memset(pCurr,0,sizeof(Face_t));
		fscanf(file,"%d %d %d %d %d\n",&Face.FaceID,&Face.NumberOfVertices,&Face.textId,&Face.LightID,&Face.Type);
						
	//	if(Face.textId == -1)
	//		Face.textId = txid_SKY;
	//	else
	//		Face.textId = texture_id_plan[Face.textId];
		if(Face.NumberOfVertices>8)
		{
			printf("Too much vertices in a face\n");
			fclose(file);
			return false;
		}
		if(Face.NumberOfVertices!=3)
			printf("vertice : %d error\n",Face.NumberOfVertices);
		for(i=0 ; i<Face.NumberOfVertices ; i++)
		{
			fscanf(file,"%f %f %f %f %f %f %f\n",
					&Face.point[i].X,
					&Face.point[i].Y,
					&Face.point[i].Z,
					&Face.point[i].U,
					&Face.point[i].V,
					&Face.point[i].U_lm,
					&Face.point[i].V_lm);
		}
		Face.Plan.VecteurNormal = GetFaceNormal(Face.point[0],Face.point[1],Face.point[2]);		
		Face.Plan.PointOnPlane.X = Face.point[0].X;
		Face.Plan.PointOnPlane.Y = Face.point[0].Y;
		Face.Plan.PointOnPlane.Z = Face.point[0].Z;

		Face.Next = NULL;
		FaceArray[k] = Face;
	}


	fscanf(file,"<LEAFS> %d\n",&gNb_Leafs);

	//printf("gNb_nodes: %d\n",gNb_nodes);
	//printf("gNb_Leafs: %d\n",gNb_Leafs);
	//printf("faces: %d\n",gNb_faces);

	Nb_faces = 0;
	gFaceList = NULL;
	pCurr = NULL;

	for(k=0 ; k<gNb_Leafs ; k++)
	{		
		fscanf(file,"%d ",&Id);
		LeafArray[k].LeafID = Id;
		fscanf(file,"%d %d\n",&LeafArray[Id].Nb_faces,&LeafArray[Id].PVSIndex);
		fscanf(file,"%f %f %f %f %f %f\n",
								&LeafArray[Id].bbox.BoxMin.X,
								&LeafArray[Id].bbox.BoxMin.Y,
								&LeafArray[Id].bbox.BoxMin.Z,
								&LeafArray[Id].bbox.BoxMax.X,
								&LeafArray[Id].bbox.BoxMax.Y,
								&LeafArray[Id].bbox.BoxMax.Z);

		LeafArray[k].FaceIndexList = malloc(LeafArray[Id].Nb_faces*sizeof(int));
		memset(LeafArray[k].FaceIndexList,0,LeafArray[Id].Nb_faces*sizeof(int));
		for(j=0 ; j<LeafArray[Id].Nb_faces ; j++)
		{
			fscanf(file,"%d ",&LeafArray[k].FaceIndexList[j]);
		}
		fscanf(file,"\n");

		
		FaceList = NULL;
		for(j=0 ; j<LeafArray[Id].Nb_faces ; j++)
		{
			pCurr = malloc(sizeof(RECTANGLE));
			memset(pCurr,0,sizeof(RECTANGLE));

			pCurr->NumberOfVertices = FaceArray[LeafArray[k].FaceIndexList[j]].NumberOfVertices;
			pCurr->textId = FaceArray[LeafArray[k].FaceIndexList[j]].textId;
			pCurr->LightID = FaceArray[LeafArray[k].FaceIndexList[j]].LightID;
			pCurr->Type = FaceArray[LeafArray[k].FaceIndexList[j]].Type;
			pCurr->Plan = FaceArray[LeafArray[k].FaceIndexList[j]].Plan;


			//fscanf(file,"%d %d %d %d\n",&pCurr->NumberOfVertices,&pCurr->textId,&pCurr->LightID,&pCurr->Type);

			if(pCurr->NumberOfVertices>8)
			{
				printf("Too much vertices in a face\n");
				fclose(file);
				return false;
			}
			else if(pCurr->NumberOfVertices!=3)
			{
				printf("vertice : %d error\n",pCurr->NumberOfVertices);
				fclose(file);
				return false;
			}

			for(i=0 ; i<pCurr->NumberOfVertices ; i++)
			{
				pCurr->point[i] = FaceArray[LeafArray[k].FaceIndexList[j]].point[i];
/*
				fscanf(file,"%f %f %f %f %f %f %f\n",
						&pCurr->point[i].X,
						&pCurr->point[i].Y,
						&pCurr->point[i].Z,
						&pCurr->point[i].U,
						&pCurr->point[i].V,
						&pCurr->point[i].U_lm,
						&pCurr->point[i].V_lm);
*/
			}

//			pCurr->Plan.VecteurNormal = GetFaceNormal(pCurr->point[0],pCurr->point[1],pCurr->point[2]);
//			pCurr->Plan.PointOnPlane.X = pCurr->point[0].X;
//			pCurr->Plan.PointOnPlane.Y = pCurr->point[0].Y;
//			pCurr->Plan.PointOnPlane.Z = pCurr->point[0].Z;

			pCurr->Next = FaceList;
			FaceList = pCurr;
		}
		LeafArray[k].faceliste = FaceList;
	}

	fclose(file);
	return true;
}

