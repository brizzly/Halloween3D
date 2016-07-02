
#include "bsp2solid.h"


int		Nb_Cube;



pFace solid_MakeGiantBox(pFace FaceList)
{
	pFace		pList;
	BOUNDINGBOX	bbox;
	pFace		GiantBox;

	ClearBound(&bbox);
	GetBound(FaceList,&bbox);
	GiantBox=NULL;

	pList = AllocFace(4);
	pList->point[3].X = bbox.BoxMin.X;
	pList->point[3].Y = bbox.BoxMin.Y;
	pList->point[3].Z = bbox.BoxMin.Z;
	pList->point[3].U = 0;
	pList->point[3].V = 0;
	pList->point[2].X = bbox.BoxMin.X;
	pList->point[2].Y = bbox.BoxMin.Y;
	pList->point[2].Z = bbox.BoxMax.Z;
	pList->point[2].U = 1;
	pList->point[2].V = 0;
	pList->point[1].X = bbox.BoxMin.X;
	pList->point[1].Y = bbox.BoxMax.Y;
	pList->point[1].Z = bbox.BoxMax.Z;
	pList->point[1].U = 1;
	pList->point[1].V = 1;
	pList->point[0].X = bbox.BoxMin.X;
	pList->point[0].Y = bbox.BoxMax.Y;
	pList->point[0].Z = bbox.BoxMin.Z;
	pList->point[0].U = 0;
	pList->point[0].V = 1;
	pList->Plan.PointOnPlane.X = pList->point[0].X;
	pList->Plan.PointOnPlane.Y = pList->point[0].Y;
	pList->Plan.PointOnPlane.Z = pList->point[0].Z;
	pList->Plan.VecteurNormal = GetFaceNormal(pList->point[0],pList->point[1],pList->point[2]);
	pList->Next = GiantBox;
	GiantBox = pList;

	pList = AllocFace(4);
	pList->point[0].X = bbox.BoxMax.X;
	pList->point[0].Y = bbox.BoxMin.Y;
	pList->point[0].Z = bbox.BoxMin.Z;
	pList->point[0].U = 0;
	pList->point[0].V = 0;
	pList->point[1].X = bbox.BoxMax.X;
	pList->point[1].Y = bbox.BoxMin.Y;
	pList->point[1].Z = bbox.BoxMax.Z;
	pList->point[1].U = 1;
	pList->point[1].V = 0;
	pList->point[2].X = bbox.BoxMax.X;
	pList->point[2].Y = bbox.BoxMax.Y;
	pList->point[2].Z = bbox.BoxMax.Z;
	pList->point[2].U = 1;
	pList->point[2].V = 1;
	pList->point[3].X = bbox.BoxMax.X;
	pList->point[3].Y = bbox.BoxMax.Y;
	pList->point[3].Z = bbox.BoxMin.Z;
	pList->point[3].U = 0;
	pList->point[3].V = 1;
	pList->Plan.PointOnPlane.X = pList->point[0].X;
	pList->Plan.PointOnPlane.Y = pList->point[0].Y;
	pList->Plan.PointOnPlane.Z = pList->point[0].Z;
	pList->Plan.VecteurNormal = GetFaceNormal(pList->point[0],pList->point[1],pList->point[2]);
	pList->Next = GiantBox;
	GiantBox = pList;

	pList = AllocFace(4);
	pList->point[3].Y = bbox.BoxMin.Y;
	pList->point[3].Z = bbox.BoxMin.Z;
	pList->point[3].X = bbox.BoxMin.X;
	pList->point[3].U = 0;
	pList->point[3].V = 0;
	pList->point[2].Y = bbox.BoxMin.Y;
	pList->point[2].Z = bbox.BoxMin.Z;
	pList->point[2].X = bbox.BoxMax.X;
	pList->point[2].U = 1;
	pList->point[2].V = 0;
	pList->point[1].Y = bbox.BoxMin.Y;
	pList->point[1].Z = bbox.BoxMax.Z;
	pList->point[1].X = bbox.BoxMax.X;
	pList->point[1].U = 1;
	pList->point[1].V = 1;
	pList->point[0].Y = bbox.BoxMin.Y;
	pList->point[0].Z = bbox.BoxMax.Z;
	pList->point[0].X = bbox.BoxMin.X;
	pList->point[0].U = 0;
	pList->point[0].V = 1;
	pList->Plan.PointOnPlane.X = pList->point[0].X;
	pList->Plan.PointOnPlane.Y = pList->point[0].Y;
	pList->Plan.PointOnPlane.Z = pList->point[0].Z;
	pList->Plan.VecteurNormal = GetFaceNormal(pList->point[0],pList->point[1],pList->point[2]);
	pList->Next = GiantBox;
	GiantBox = pList;

	pList = AllocFace(4);
	pList->point[0].Y = bbox.BoxMax.Y;
	pList->point[0].Z = bbox.BoxMin.Z;
	pList->point[0].X = bbox.BoxMin.X;
	pList->point[0].U = 0;
	pList->point[0].V = 0;
	pList->point[1].Y = bbox.BoxMax.Y;
	pList->point[1].Z = bbox.BoxMin.Z;
	pList->point[1].X = bbox.BoxMax.X;
	pList->point[1].U = 1;
	pList->point[1].V = 0;
	pList->point[2].Y = bbox.BoxMax.Y;
	pList->point[2].Z = bbox.BoxMax.Z;
	pList->point[2].X = bbox.BoxMax.X;
	pList->point[2].U = 1;
	pList->point[2].V = 1;
	pList->point[3].Y = bbox.BoxMax.Y;
	pList->point[3].Z = bbox.BoxMax.Z;
	pList->point[3].X = bbox.BoxMin.X;
	pList->point[3].U = 0;
	pList->point[3].V = 1;
	pList->Plan.PointOnPlane.X = pList->point[0].X;
	pList->Plan.PointOnPlane.Y = pList->point[0].Y;
	pList->Plan.PointOnPlane.Z = pList->point[0].Z;
	pList->Plan.VecteurNormal = GetFaceNormal(pList->point[0],pList->point[1],pList->point[2]);
	pList->Next = GiantBox;
	GiantBox = pList;

	pList = AllocFace(4);
	pList->point[3].Z = bbox.BoxMin.Z;
	pList->point[3].X = bbox.BoxMin.X;
	pList->point[3].Y = bbox.BoxMin.Y;
	pList->point[3].U = 0;
	pList->point[3].V = 0;
	pList->point[2].Z = bbox.BoxMin.Z;
	pList->point[2].X = bbox.BoxMin.X;
	pList->point[2].Y = bbox.BoxMax.Y;
	pList->point[2].U = 1;
	pList->point[2].V = 0;
	pList->point[1].Z = bbox.BoxMin.Z;
	pList->point[1].X = bbox.BoxMax.X;
	pList->point[1].Y = bbox.BoxMax.Y;
	pList->point[1].U = 1;
	pList->point[1].V = 1;
	pList->point[0].Z = bbox.BoxMin.Z;
	pList->point[0].X = bbox.BoxMax.X;
	pList->point[0].Y = bbox.BoxMin.Y;
	pList->point[0].U = 0;
	pList->point[0].V = 1;
	pList->Plan.PointOnPlane.X = pList->point[0].X;
	pList->Plan.PointOnPlane.Y = pList->point[0].Y;
	pList->Plan.PointOnPlane.Z = pList->point[0].Z;
	pList->Plan.VecteurNormal = GetFaceNormal(pList->point[0],pList->point[1],pList->point[2]);
	pList->Next = GiantBox;
	GiantBox = pList;

	pList = AllocFace(4);
	pList->point[0].Z = bbox.BoxMax.Z;
	pList->point[0].X = bbox.BoxMin.X;
	pList->point[0].Y = bbox.BoxMin.Y;
	pList->point[0].U = 0;
	pList->point[0].V = 0;
	pList->point[1].Z = bbox.BoxMax.Z;
	pList->point[1].X = bbox.BoxMin.X;
	pList->point[1].Y = bbox.BoxMax.Y;
	pList->point[1].U = 1;
	pList->point[1].V = 0;
	pList->point[2].Z = bbox.BoxMax.Z;
	pList->point[2].X = bbox.BoxMax.X;
	pList->point[2].Y = bbox.BoxMax.Y;
	pList->point[2].U = 1;
	pList->point[2].V = 1;
	pList->point[3].Z = bbox.BoxMax.Z;
	pList->point[3].X = bbox.BoxMax.X;
	pList->point[3].Y = bbox.BoxMin.Y;
	pList->point[3].U = 0;
	pList->point[3].V = 1;
	pList->Plan.PointOnPlane.X = pList->point[0].X;
	pList->Plan.PointOnPlane.Y = pList->point[0].Y;
	pList->Plan.PointOnPlane.Z = pList->point[0].Z;
	pList->Plan.VecteurNormal = GetFaceNormal(pList->point[0],pList->point[1],pList->point[2]);
	pList->Next = GiantBox;
	GiantBox = pList;

	return GiantBox;
}

int solid_AddCube(pFace ListFace)
{
	CubeArray[Nb_Cube].facelist = ListFace;
	Nb_Cube++;
	return Nb_Cube-1;
}

/*
void solid_CutBox(int Hull, pFace CutFace, int *ChildF, int *ChildB)
{
	pFace	Face;
	pFace	Temp;
	pFace	InitialBox;
	pFace	pRect1;
	pFace	pRect2;
	pFace	ChildBoxF;
	pFace	ChildBoxB;
	pFace	CopyF=NULL;
	pFace	CopyB=NULL;
	bool	IsCut;
	int		nCut;

	if(!CutFace)
	{
		*ChildF = -1;
		*ChildB = -1;
		return;
	}

	IsCut = false;
	nCut = 0;
	ChildBoxF = NULL;
	ChildBoxB = NULL;

	InitialBox = CubeArray[Hull].facelist;
	if(!InitialBox)
	{
		*ChildF = -1;
		*ChildB = -1;
		return;
	}

	for(Face=InitialBox ; Face ; )
	{
		Temp = Face;
		Face=Face->Next;

		switch( ClassifyPoly(&CutFace->Plan,Temp) )
		{
			case CONFONDU:
	//			printf("C");
			if( PdtScalaire(Temp->Plan.VecteurNormal, CutFace->Plan.VecteurNormal) > 0 )
			{
				Temp->Next = ChildBoxF;
				ChildBoxF = Temp;
			}
			else
			{
				Temp->Next = ChildBoxB;
				ChildBoxB = Temp;
			}
			break;

			case DEVANT:
			Temp->Next = ChildBoxF;
			ChildBoxF = Temp;
			break;

			case DERRIERE:
			Temp->Next = ChildBoxB;
			ChildBoxB = Temp;
			break;

			case AUMILIEU:
			pRect1 = AllocFace(0);
			pRect2 = AllocFace(0);
			pRect1->Plan = Temp->Plan;
			pRect2->Plan = Temp->Plan;
			Spliting(Temp,&CutFace->Plan,pRect1,pRect2,NULL);

			if(pRect1 && pRect2)
			{
				nCut++;
				IsCut = true;
			}

			if(pRect1)
			{
				pRect1->Next = ChildBoxF;
				ChildBoxF = pRect1;
			}
			if(pRect2)
			{
				pRect2->Next = ChildBoxB;
				ChildBoxB = pRect2;
			}
			break;
		}
	}

//	if(nCut < 3)
//		IsCut = false;

	if(ChildBoxF)
	{
		if(IsCut)
		{
			CopyF = CopyFace(CutFace);
		//	CopyF->Plan.VecteurNormal.X *= -1.0f;
		//	CopyF->Plan.VecteurNormal.Y *= -1.0f;
		//	CopyF->Plan.VecteurNormal.Z *= -1.0f;
			CopyF = solid_AdjustFaceToCube(InitialBox,CopyF);
			CopyF->Next = ChildBoxF;
			ChildBoxF = CopyF;
		}
		*ChildF = solid_AddCube(ChildBoxF);
	}
	else
		*ChildF = -1;

	if(ChildBoxB)
	{
		if(IsCut)
		{
			CopyB = CopyFace(CutFace);
			CopyB->Plan.VecteurNormal.X *= -1.0f;
			CopyB->Plan.VecteurNormal.Y *= -1.0f;
			CopyB->Plan.VecteurNormal.Z *= -1.0f;
			CopyB = solid_AdjustFaceToCube(InitialBox,CopyB);
			CopyB->Next = ChildBoxB;
			ChildBoxB = CopyB;
		}
		*ChildB = solid_AddCube(ChildBoxB);
	}
	else
		*ChildB = -1;
}
*/
void solid_WriteBoxes(char *name)
{
	FILE	*file;
	int		i;
	int		j;
	int		NbFace;
	int		NbVert;
	pFace	Face;
	pFace	FaceList;
	int		Nb;

	printf("solid_WriteBoxes\n");
	Rename(name,"box");
	remove(name);
	file = fopen(name,"w");
	if(file==NULL)
		return;

	Nb = 0;
	for(i=0 ; i<Nb_leafs ; i++)
	{
		if( LeafArray[i].HullID == -1 )
			continue;
		Nb++;
	}

	fprintf(file,"<CUBES> %d\n",Nb);
	
	for(i=0 ; i<Nb_leafs ; i++)
	{
		if( LeafArray[i].HullID == -1 )
			continue;
		FaceList = CubeArray[ LeafArray[i].HullID ].facelist;
		NbFace = CountFaces(FaceList);
		fprintf(file,"%d\n",NbFace);

		for(Face=FaceList ; Face ; Face=Face->Next)
		{
			NbVert = Face->NumberOfVertices;
			fprintf(file,"%d\n",NbVert);
			for(j=0 ; j<NbVert ; j++)
			{
				fprintf(file,"%f %f %f\n",Face->point[j].X,Face->point[j].Y,Face->point[j].Z);
			}
		}
	}

	fclose(file);
}

void solid_GetBoundFromBox(pFace Box, bound_t Bound)
{

}

/*
pFace solid_AdjustFaceToCube(pFace CubeList, pFace Face)
{
	solid_ExpandFace(Face);
	Face = solid_ClipFace(CubeList,Face);
	return Face;
}
*/

void solid_ExpandFace(pFace Face)
{
	vect_t	center;
	vect_t	dir;
	int		i;
//	float	STEP = 4000.0f;
	float	STEP = 10.0f;

	center.X = center.Y = center.Z = 0;
	for(i=0 ; i<Face->NumberOfVertices ; i++)
	{
		center.X += Face->point[i].X;
		center.Y += Face->point[i].Y;
		center.Z += Face->point[i].Z;
	}
	center.X /= Face->NumberOfVertices;
	center.Y /= Face->NumberOfVertices;
	center.Z /= Face->NumberOfVertices;

	for(i=0 ; i<Face->NumberOfVertices ; i++)
	{
		dir.X = Face->point[i].X - center.X;
		dir.Y = Face->point[i].Y - center.Y;
		dir.Z = Face->point[i].Z - center.Z;
		Normalize(&dir);
		Face->point[i].X = center.X + (dir.X * STEP);
		Face->point[i].Y = center.Y + (dir.Y * STEP);
		Face->point[i].Z = center.Z + (dir.Z * STEP);
	}
}

/*
pFace solid_ClipFace(pFace ClipList, pFace Face)
{
	pFace	Temp;
	pFace	pRect1;
	pFace	pRect2;

	return Face;

	for(Temp=ClipList ; Temp ; Temp=Temp->Next)
	{
		switch( ClassifyPoly(&Temp->Plan,Face) )
		{
			case CONFONDU:
		//	printf("?");
			break;

			case DEVANT:
		//	printf("+");
			break;

			case DERRIERE:
			printf("-");
			break;

			case AUMILIEU:
			pRect1 = AllocFace(0);
			pRect2 = AllocFace(0);
			pRect1->Plan = Face->Plan;
			pRect2->Plan = Face->Plan;
			Spliting(Face,&Temp->Plan,pRect1,pRect2,NULL);
			if(pRect2)
				FreeFace(pRect2);
			FreeFace(Face);
			Face = pRect1;
			if(!Face)
				return NULL;
			break;
		}
	}
	return Face;
}
*/
