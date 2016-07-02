
#include "bsp2file.h"


bool Write_MAP(char *name, pFace PortalList, pFace FaceList)
{
	FILE		*file;
	int			i, j, k;
	pFace		pCurr=NULL;
	pFace		portal;
	int			count=0;

	Rename(name,"map");
	remove(name);

	file = fopen(name,"w");
	if(file==NULL)
		return false;

	for(j=0 ; j<Nb_leafs ; j++)
		LeafArray[j].PVSIndex = (Nb_leafs*j);

	fprintf(file,"<MAP>\n");
	fprintf(file,"<PLAYERPOS> %f %f %f\n",gStartX,gStartY,gStartZ);
	fprintf(file,"<PLAYERDIR> %d\n",gPlayerDir);

	fprintf(file,"<ENTITIES> %d\n",Nb_entities);
	for(j=0 ; j<Nb_entities ; j++)
		fprintf(file,"%d %d %f %f %f\n",Entities2[j].id,Entities2[j].angleY,Entities2[j].pos.X,Entities2[j].pos.Y,Entities2[j].pos.Z);

	fprintf(file,"<TEXTURES> %d\n",Nb_textures);
	for(k=0 ; k<Nb_textures ; k++)
		fprintf(file,"%s\n",TextureList2[k]);

	fprintf(file,"<LIGHTS> %d\n",Nb_Lights);
	for(k=0 ; k<Nb_Lights ; k++)
		fprintf(file,"%f %f %f %d %d %d %f %d\n",
					Lights[k].pos.X,
					Lights[k].pos.Y,
					Lights[k].pos.Z,
					Lights[k].R,
					Lights[k].G,
					Lights[k].B,
					Lights[k].I,
					Lights[k].type);

	fprintf(file,"<LIGHTMAPS> 0\n");

	fprintf(file,"<NODES> %d\n",Nb_nodes);
	for(i=0 ; i<Nb_nodes ; i++)
	{
		fprintf(file,"NODE[%d] LEAF[%d] FNODE[%d] BNODE[%d] POP[%f %f %f] VN[%f %f %f]\n",
			i,
			NodeArray[i].Feuille,
			NodeArray[i].Front,
			NodeArray[i].Back,
			NodeArray[i].Plan.PointOnPlane.X,
			NodeArray[i].Plan.PointOnPlane.Y,
			NodeArray[i].Plan.PointOnPlane.Z,
			NodeArray[i].Plan.VecteurNormal.X,
			NodeArray[i].Plan.VecteurNormal.Y,
			NodeArray[i].Plan.VecteurNormal.Z);
	}

	fprintf(file,"<FACES> %d\n",CountFaces(FaceList));
	for(pCurr=FaceList ; pCurr ; pCurr=pCurr->Next)
	//if(!pCurr->Cloned)
	{
		fprintf(file,"%d %d %d %d %d\n",pCurr->FaceID,pCurr->NumberOfVertices,pCurr->textId,0,pCurr->IsDetailObjet);
		//printf("%d ",pCurr->IsDetailObjet);
		for(i=0 ; i<pCurr->NumberOfVertices ; i++)
		{
			fprintf(file,"%f %f %f %f %f %f %f\n",
						pCurr->point[i].X,
						pCurr->point[i].Y,
						pCurr->point[i].Z,
						pCurr->point[i].U,
						pCurr->point[i].V,
						pCurr->point[i].U_lm,
						pCurr->point[i].V_lm);
		}
	}

	fprintf(file,"<LEAFS> %d\n",Nb_leafs);
	for(k=0 ; k<Nb_leafs ; k++)
	{
		fprintf(file,"%d %d %d\n",LeafArray[k].LeafID,LeafArray[k].Nb_faces,LeafArray[k].PVSIndex);
		fprintf(file,"%f %f %f %f %f %f\n",
								LeafArray[k].bbox.BoxMin.X,
								LeafArray[k].bbox.BoxMin.Y,
								LeafArray[k].bbox.BoxMin.Z,
								LeafArray[k].bbox.BoxMax.X,
								LeafArray[k].bbox.BoxMax.Y,
								LeafArray[k].bbox.BoxMax.Z);

		for(j=0 ; j<LeafArray[k].Nb_faces ; j++)
		{
			fprintf(file,"%d ",LeafArray[k].FaceIds[j]);
			//printf("%d ",LeafArray[k].FaceIds[j]);
		}
		fprintf(file,"\n");
		//printf("\n");
	}

	fprintf(file,"<ACTION BRUSH> %d\n",Nb_ActionBrush);
	for(i=0 ; i<Nb_ActionBrush ; i++)
	{
		fprintf(file,"ActionBrush %d faces %d\n",ActionBrushTab[i].ListFace->ActionNumber,ActionBrushTab[i].Nb_Face);
		for(pCurr=ActionBrushTab[i].ListFace ; pCurr ; pCurr=pCurr->Next)
		{
			fprintf(file,"%d %d\n", pCurr->NumberOfVertices, pCurr->textId);
			for(j=0 ; j<pCurr->NumberOfVertices ; j++)
			{
				fprintf(file,"%f %f %f %f %f\n",
								pCurr->point[j].X,
								pCurr->point[j].Y,
								pCurr->point[j].Z,
								pCurr->point[j].U,
								pCurr->point[j].V);
			}
		}
	}

	fprintf(file,"<PORTALS> %d\n",Nb_portals);
	for(portal=PortalList ; portal ; portal=portal->Next)
	{
		fprintf(file,"%d\n",portal->LeafOwnerArray[0]);
		fprintf(file,"%d\n",portal->LeafOwnerArray[1]);
		fprintf(file,"%d\n",portal->NumberOfVertices);

		fprintf(file,"%f %f %f %f %f %f\n",
					portal->Plan.VecteurNormal.X,
					portal->Plan.VecteurNormal.Y,
					portal->Plan.VecteurNormal.Z,
					portal->Plan.PointOnPlane.X,
					portal->Plan.PointOnPlane.Y,
					portal->Plan.PointOnPlane.Z);

		for(j=0 ; j<portal->NumberOfVertices ; j++)
		{
			fprintf(file,"PORTAL %d : %f %f %f\n",
				i,
				portal->point[j].X,
				portal->point[j].Y,
				portal->point[j].Z);
		}
	}

	fprintf(file,"<PVS> %d\n",Nb_leafs*Nb_leafs);
	for(i=0 ; i<Nb_leafs*Nb_leafs ; i++)
		fprintf(file,"1");
	fprintf(file,"\n");

	fprintf(file,"@");
	fclose(file);
	return true;
}

void Write_BSP(char *name)
{
	FILE	*file;
	int		i;

	Rename(name,"bsp");
	remove(name);

	file=fopen(name,"w");

	fprintf(file,"%d\n",Nb_nodes);

	for(i=0 ; i<Nb_nodes ; i++)
	{
		fprintf(file,"NODE[%d] FNODE[%d] BNODE[%d] POP[%f %f %f] VN[%f %f %f]\n",
			i,
			NodeArray[i].Front,
			NodeArray[i].Back,
			NodeArray[i].Plan.PointOnPlane.X,
			NodeArray[i].Plan.PointOnPlane.Y,
			NodeArray[i].Plan.PointOnPlane.Z,
			NodeArray[i].Plan.VecteurNormal.X,
			NodeArray[i].Plan.VecteurNormal.Y,
			NodeArray[i].Plan.VecteurNormal.Z);
	}

	fclose(file);
}

bool Write_solid_BSP(char *name)
{
	FILE		*file;
	int			j, k;
	RECTANGLE	*pCurr=NULL;
	int			count=0;

	Rename(name,"map");
	remove(name);

	for(j=0 ; j<Nb_leafs ; j++)
	{
		LeafArray[j].PVSIndex = (Nb_leafs*j);
	}

	file = fopen(name,"w");
	if(file==NULL)
		return false;

	fprintf(file,"<BSP>\n");
	fprintf(file,"<PLAYERPOS> %f %f %f\n",gStartX,gStartY,gStartZ);
	fprintf(file,"<PLAYERDIR> %d\n",gPlayerDir);

	Nb_entities = 0;	// HACK
	
	fprintf(file,"<ENTITIES> %d\n",Nb_entities);
	for(j=0 ; j<Nb_entities ; j++)
		fprintf(file,"%d %f %f %f\n",Entities2[j].id,Entities2[j].pos.X,Entities2[j].pos.Y,Entities2[j].pos.Z);

	fprintf(file,"<TEXTURES> %d\n",Nb_textures);
	for(k=0 ; k<Nb_textures ; k++)
		fprintf(file,"%s\n",TextureList2[k]);

	fprintf(file,"\n<LEAFS> %d\n",Nb_leafs);
	fprintf(file,"\n<FACES> %d\n",CountLeafyFaces());
	for(k=0 ; k<Nb_leafs ; k++)
	{
		pCurr = FaceArray[k].faceliste;
		fprintf(file,"%d %d %d\n",LeafArray[k].LeafID,LeafArray[k].Nb_faces,LeafArray[k].PVSIndex);
		fprintf(file,"%f %f %f %f %f %f\n",
								LeafArray[k].bbox.BoxMin.X,
								LeafArray[k].bbox.BoxMin.Y,
								LeafArray[k].bbox.BoxMin.Z,
								LeafArray[k].bbox.BoxMax.X,
								LeafArray[k].bbox.BoxMax.Y,
								LeafArray[k].bbox.BoxMax.Z);

		while(pCurr)
		{
			fprintf(file,"%d %d %d %d\n",pCurr->NumberOfVertices,pCurr->textId,pCurr->lightID,pCurr->IsDetailObjet);
			for(j=0 ; j<pCurr->NumberOfVertices ; j++)
			{
				fprintf(file,"%f %f %f %f %f %f %f %f %f %f\n",
						pCurr->point[j].X,
						pCurr->point[j].Y,
						pCurr->point[j].Z,
						pCurr->point[j].U,
						pCurr->point[j].V,
						pCurr->point[j].U_lm,
						pCurr->point[j].V_lm,
						pCurr->point[j].NX,
						pCurr->point[j].NY,
						pCurr->point[j].NZ);
			}
			pCurr = pCurr->Next;
		}
	}
	fclose(file);
	//printf("%s\n",name);
	return true;
}

pFace Load_Polygons(char *name)
{
	FILE		*file;
	pFace		gFaceList=NULL;
	pFace		pCurr;
	int			j, k;
	char		line[255];
	int			i;
	int			Nb_faces;

	Rename(name,"pts");

	file = fopen(name,"r");
	if(!file)
		return NULL;

	printf("Loading %s\n",name);

	fscanf(file,"%s\n",line);
	if(strncmp("<POLYGONS>",line,10) !=0)
		return NULL;

	fscanf(file,"<PLAYERPOS> %f %f %f\n",&gStartX,&gStartY,&gStartZ);
	fscanf(file,"<PLAYERDIR> %d\n",&gPlayerDir);

	fscanf(file,"<ENTITIES> %d\n",&Nb_entities);
	for(j=0 ; j<Nb_entities ; j++)
		fscanf(file,"%d %d %f %f %f\n",&Entities2[j].id,&Entities2[j].angleY,&Entities2[j].pos.X,&Entities2[j].pos.Y,&Entities2[j].pos.Z);

	fscanf(file,"<TEXTURES> %d\n",&Nb_textures);
	for(k=0 ; k<Nb_textures ; k++)
		fscanf(file,"%s\n",TextureList2[k]);

	fscanf(file,"<LIGHTS> %d\n",&Nb_Lights);
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

	while(!feof(file))
	{
		pCurr = malloc(sizeof(RECTANGLE));
		memset(pCurr,0,sizeof(RECTANGLE));

		pCurr->NonFragFace = NULL;
		pCurr->OriginalFace = pCurr;

		fscanf(file,"%d %d %d %d %d\n",
				&pCurr->NumberOfVertices,
				&pCurr->textId,
				&pCurr->ActionNumber,
				&pCurr->IsDetailObjet,
				&pCurr->BrushID);
//		printf("%d",pCurr->ActionNumber);
//		printf("%d",pCurr->IsDetailObjet);
		pCurr->point = malloc(sizeof(point_t)*pCurr->NumberOfVertices);
		memset(pCurr->point,0,sizeof(point_t)*pCurr->NumberOfVertices);

		for(i=0 ; i<pCurr->NumberOfVertices ; i++)
		{
			fscanf(file,"%f %f %f %f %f %f %f\n",
									&pCurr->point[i].X,
									&pCurr->point[i].Y,
									&pCurr->point[i].Z,
									&pCurr->point[i].U,
									&pCurr->point[i].V,
									&pCurr->point[i].U_lm,
									&pCurr->point[i].V_lm);
		}
	
		pCurr->Plan.VecteurNormal = GetFaceNormal(pCurr->point[0],pCurr->point[1],pCurr->point[2]);
		pCurr->Plan.PointOnPlane.X = pCurr->point[0].X;
		pCurr->Plan.PointOnPlane.Y = pCurr->point[0].Y;
		pCurr->Plan.PointOnPlane.Z = pCurr->point[0].Z;

		pCurr->Next = gFaceList;
		gFaceList = pCurr;
	}
	fclose(file);

	Nb_faces = CountFaces(gFaceList);
	printf("FACES: %d\n",Nb_faces);
	return gFaceList;
}

bool Write_Polygons(char *path, pFace RectList)
{
	FILE	*file;
	pFace	pRect;
	int		i, k;

	Rename(path,"map");
	remove(path);

	file = fopen(path,"w");
	if(file==NULL)
		return false;

	fprintf(file,"<POLYGONS>\n");
	fprintf(file,"<PLAYERPOS> %f %f %f\n",gStartX,gStartY,gStartZ);
	fprintf(file,"<PLAYERDIR> %d\n",gPlayerDir);

	//Nb_entities = 0; // HACK

	fprintf(file,"<ENTITIES> %d\n",Nb_entities);
	for(i=0 ; i<Nb_entities ; i++)
		fprintf(file,"%d %d %f %f %f\n",Entities2[i].id,Entities2[i].angleY,Entities2[i].pos.X,Entities2[i].pos.Y,Entities2[i].pos.Z);

	fprintf(file,"<TEXTURES> %d\n",Nb_textures);
	for(k=0 ; k<Nb_textures ; k++)
		fprintf(file,"%s\n",TextureList2[k]);

	fprintf(file,"<LIGHTS> %d\n",Nb_Lights);
	for(k=0 ; k<Nb_Lights ; k++)
		fprintf(file,"%f %f %f %d %d %d %f %d\n",
					Lights[k].pos.X,
					Lights[k].pos.Y,
					Lights[k].pos.Z,
					Lights[k].R,
					Lights[k].G,
					Lights[k].B,
					Lights[k].I,
					Lights[k].type);

	for(pRect=RectList ; pRect ; pRect=pRect->Next)
	{
		if(menu_nodetail && pRect->IsDetailObjet)
			continue;
	//	printf("%d ",pRect->IsDetailObjet);
		fprintf(file,"%d %d %d %d\n",pRect->NumberOfVertices,pRect->textId,0,pRect->IsDetailObjet);
		for(i=0 ; i<pRect->NumberOfVertices ; i++)
		{
		//	fprintf(file,"%f %f %f %f %f %f %f %f %f %f\n",
			fprintf(file,"%f %f %f %f %f %f %f\n",
						pRect->point[i].X,
						pRect->point[i].Y,
						pRect->point[i].Z,
						pRect->point[i].U,
						pRect->point[i].V,
						pRect->point[i].U_lm,
						pRect->point[i].V_lm); //,
					//	pRect->point[i].NX,
					//	pRect->point[i].NY,
					//	pRect->point[i].NZ);
		}
	}
	fclose(file);
	//printf("%s : %d FACES\n",path,CountFaces(RectList));
	return true;
}

bool Write_Portal(char *path, pFace PortalList)
{
	FILE	*file;
	pFace	portal;
	int		i, j;

	Rename(path,"prt");
	remove(path);

	if(!PortalList)
		return true;
	
	file = fopen(path,"w");
	if(file==NULL)
		return false;

	for(i=0, portal=PortalList ; portal ; portal=portal->Next, i++)
	{
		fprintf(file,"%d\n",portal->LeafOwnerArray[0]);
		fprintf(file,"%d\n",portal->LeafOwnerArray[1]);
		fprintf(file,"%d\n",portal->NumberOfVertices);

		fprintf(file,"%f %f %f %f %f %f\n",
					portal->Plan.VecteurNormal.X,
					portal->Plan.VecteurNormal.Y,
					portal->Plan.VecteurNormal.Z,
					portal->Plan.PointOnPlane.X,
					portal->Plan.PointOnPlane.Y,
					portal->Plan.PointOnPlane.Z);

		for(j=0 ; j<portal->NumberOfVertices ; j++)
		{
			fprintf(file,"PORTAL %d : %f %f %f\n",
				i,
				portal->point[j].X,
				portal->point[j].Y,
				portal->point[j].Z);
		}
	}
	fclose(file);
	//printf("%s\n",path);
	return true;
}

void Write_ActionBrush(char *path)
{
	FILE	*file;
	pFace	Face;
	int		i, j;

//	Rename(path,"abr");
//	remove(path);
	
	file = fopen(path,"a"); // "w"
	if(file==NULL)
		return;

	fprintf(file,"\n<ACTION BRUSH> %d\n",Nb_ActionBrush);
	for(i=0 ; i<Nb_ActionBrush ; i++)
	{
		fprintf(file,"ActionBrush %d faces %d\n",ActionBrushTab[i].ListFace->ActionNumber,ActionBrushTab[i].Nb_Face);
		for(Face=ActionBrushTab[i].ListFace ; Face ; Face=Face->Next)
		{
			fprintf(file,"%d %d\n", Face->NumberOfVertices, Face->textId);
			for(j=0 ; j<Face->NumberOfVertices ; j++)
			{
				fprintf(file,"%f %f %f %f %f\n",
								Face->point[j].X,
								Face->point[j].Y,
								Face->point[j].Z,
								Face->point[j].U,
								Face->point[j].V);
			}
		}

	}
	fclose(file);
}