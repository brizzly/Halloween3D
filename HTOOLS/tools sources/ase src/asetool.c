
#include "asecomp.h"
#include "asereader.h"
#include "asetool.h"
#include "asemath.h"

extern	int		PlayerAngle;
extern	bool	menu_groupface;
extern	int		gNb_Faces;
extern	int		Nb_textures;
extern	int		Nb_object;
extern	char	TextureList2[TEXTURELOADED][TEXTURENAMEMAX];
extern	float	gStartX,gStartY,gStartZ;
extern	int		Nb_entities;
extern	bool	menu_makemap;


pFace AddBoundFaces(pFace List)
{
	pFace	pList;
	pFace	pBigList;
	BOUNDINGBOX	bbox;

	ClearBound(&bbox);
	CalculateBox(&bbox,List);

	pBigList=NULL;

	pList = AllocFace(4,false);
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
	pList->Next = pBigList;
	pBigList = pList;

	pList = AllocFace(4,false);
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
	pList->Next = pBigList;
	pBigList = pList;

	pList = AllocFace(4,false);
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
	pList->Next = pBigList;
	pBigList = pList;

	pList = AllocFace(4,false);
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
	pList->Next = pBigList;
	pBigList = pList;

	pList = AllocFace(4,false);
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
	pList->Next = pBigList;
	pBigList = pList;

	pList = AllocFace(4,false);
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
	pList->Next = pBigList;
	pBigList = pList;

	List = AddListes(List,pBigList);

	return List;
}

void ComputeNormals(RECTANGLE *gFaceList)
{
	RECTANGLE	*pCurr;
	RECTANGLE	*pCurr2;
	int			i;
	int			j;
	int			cnt;

	printf("ComputeNormals..");

	for(pCurr=gFaceList ; pCurr ; pCurr=pCurr->Next)
	{
		for(i=0 ; i<pCurr->NumberOfVertices ; i++)
		{
			pCurr->point[i].NX = pCurr->Plan.VecteurNormal.X;
			pCurr->point[i].NY = pCurr->Plan.VecteurNormal.Y;
			pCurr->point[i].NZ = pCurr->Plan.VecteurNormal.Z;

			cnt=1;

			for(pCurr2=gFaceList ; pCurr2 ; pCurr2=pCurr2->Next)
			{
				if(pCurr == pCurr2)
					continue;

			//	if(ClassifyPoly(&pCurr->Plan,pCurr2) == CONFONDU)
			//	if(ClassifyPoly(&pCurr2->Plan,pCurr) == CONFONDU)
			//	if(pCurr->textId == pCurr2->textId)
			//		continue;

				for(j=0 ; j<pCurr2->NumberOfVertices ; j++)
				{
					if(IsEqualf(pCurr->point[i].X,pCurr2->point[j].X))
					if(IsEqualf(pCurr->point[i].Y,pCurr2->point[j].Y))
					if(IsEqualf(pCurr->point[i].Z,pCurr2->point[j].Z))
					{
						cnt++;
						pCurr->point[i].NX += pCurr2->Plan.VecteurNormal.X;
						pCurr->point[i].NY += pCurr2->Plan.VecteurNormal.Y;
						pCurr->point[i].NZ += pCurr2->Plan.VecteurNormal.Z;
						break;
					}
				}			
			}

			pCurr->point[i].NX /= -(float)cnt;
			pCurr->point[i].NY /= -(float)cnt;
			pCurr->point[i].NZ /= -(float)cnt;
		}
	}

	printf("\n");
}

void Overlaps(RECTANGLE *gFaceList)
{
	RECTANGLE *	faceA;
	RECTANGLE *	faceB;

	printf("Remove Overlaps Faces");

	for(faceA=gFaceList ; faceA ; faceA=faceA->Next)
		faceA->Used = false;

	for(faceA=gFaceList ; faceA ; faceA=faceA->Next)
	{
		if(!IsRectangle(faceA))
			continue;

		for(faceB=gFaceList ; faceB ; faceB=faceB->Next)
		{
			if(faceA==faceB)
				continue;

			if(faceA->Used)
				continue;
			if(faceB->Used)
				continue;

			if(!IsRectangle(faceB))
				continue;

			if( ClassifyPoly(&faceA->Plan,faceB) != CONFONDU )
				continue;
			if( IsFacesOverlap(faceA,faceB) == true )
				printf(".");
		}
	}

	printf("\n");

	for(faceA=gFaceList ; faceA ; faceA=faceA->Next)
	{
		if(faceA->Used)
		{
			gFaceList = RemoveFaceFromList(gFaceList,faceA);
			faceA = gFaceList;
		}
	}

	for(faceA=gFaceList ; faceA ; faceA=faceA->Next)
		faceA->Used = false;
}

bool IsFacesOverlap(RECTANGLE *Fa, RECTANGLE *Fb)
{
	int		i;
	vect_t	v;
	int		count;

	count=0;
	for(i=0 ; i<Fa->NumberOfVertices ; i++)
	{
		v.X = Fa->point[i].X;
		v.Y = Fa->point[i].Y;
		v.Z = Fa->point[i].Z;

		if( IsVertexOnFace_2(v,Fb) || IsVertexOnEdgeFace(v,Fb) || IsVertexEgalFace(v,Fb) )
		{
			count++;
		}
	}

	if(count==Fa->NumberOfVertices)
	{
		Fa->Used=true;
		return true;
	}

	if(count>=1)//2
	{
		if(CutSemiOverlapFaces(Fa,Fb))
			return true;
	}

	count=0;
	for(i=0 ; i<Fb->NumberOfVertices ; i++)
	{
		v.X = Fb->point[i].X;
		v.Y = Fb->point[i].Y;
		v.Z = Fb->point[i].Z;

		if( IsVertexOnFace_2(v,Fa) || IsVertexOnEdgeFace(v,Fa) || IsVertexEgalFace(v,Fa) )
		{
			count++;
		}
	}

	if(count==Fb->NumberOfVertices)
	{
		Fb->Used=true;
		return true;
	}

	if(count>=1)//2
	{
		if(CutSemiOverlapFaces(Fb,Fa))
			return true;
	}

	return false;
}

bool IsRectangle(RECTANGLE *Face)
{
	int		i, m, n;
	vect_t	v, w;

	for(i=0 ; i<Face->NumberOfVertices ; i++)
	{
		m=i+1;
		m = m % Face->NumberOfVertices;
		n=m+1;
		n = n % Face->NumberOfVertices;

		v.X = Face->point[m].X - Face->point[i].X;
		v.Y = Face->point[m].Y - Face->point[i].Y;
		v.Z = Face->point[m].Z - Face->point[i].Z;

		w.X = Face->point[n].X - Face->point[m].X;
		w.Y = Face->point[n].Y - Face->point[m].Y;
		w.Z = Face->point[n].Z - Face->point[m].Z;

		if(PdtScalaire(v,w) != 0)
			return false;
	}
	return true;
}

bool CutSemiOverlapFaces(RECTANGLE *Fa, RECTANGLE *Fb)
{
	vect_t	v;
	plan_t	plan_cut;
	RECTANGLE *face_cut_A;
	RECTANGLE *face_cut_B;
	RECTANGLE *temp;
	int		i;
	int		m;

	return false;

	if(!IsRectangle(Fa))
		return false;
	if(!IsRectangle(Fb))
		return false;

	for(i=0 ; i<Fb->NumberOfVertices ; i++)
	{
		m=i+1;
		m = m % Fb->NumberOfVertices;

		v.X = Fb->point[m].X - Fb->point[i].X;
		v.Y = Fb->point[m].Y - Fb->point[i].Y;
		v.Z = Fb->point[m].Z - Fb->point[i].Z;

		plan_cut.VecteurNormal = PdVectoriel(v,Fb->Plan.VecteurNormal);
		plan_cut.PointOnPlane.X = Fb->point[i].X;
		plan_cut.PointOnPlane.Y = Fb->point[i].Y;
		plan_cut.PointOnPlane.Z = Fb->point[i].Z;

		if(ClassifyPoly(&plan_cut,Fa) != AUMILIEU)
			continue;

		printf("*");

		face_cut_A = malloc(sizeof(RECTANGLE));
		memset(face_cut_A,0,sizeof(RECTANGLE));
		face_cut_B = malloc(sizeof(RECTANGLE));
		memset(face_cut_B,0,sizeof(RECTANGLE));

		Spliting(Fa,&plan_cut,face_cut_A,face_cut_B);
		gPolyList = RemoveFaceFromList(gPolyList,Fa);

//		face_cut_A->textId = 50;
//		face_cut_B->textId = 50;

		temp=Fb->Next;
		Fb->Next = face_cut_A;
		face_cut_A->Next = temp;

		temp=Fb->Next;
		Fb->Next = face_cut_B;
		face_cut_B->Next = temp;

		return true;
	}
	return false;
}

void Spliting(RECTANGLE *Poly, plan_t *Plan, RECTANGLE *FrontSplit, RECTANGLE *BackSplit)
{
	point_t		FrontList[40],BackList[40],FirstVertex,copy;
	vect_t		IntersectPoint,PointA,PointB,temp;
	int			FrontCounter=0,BackCounter=0,loop=0,CurrentVertex=0;
	float		percent;
	int			Result;
	float		deltax,deltay,texx,texy; //, lmx, lmy;

	FirstVertex=Poly->point[0];

	temp.X = FirstVertex.X;
	temp.Y = FirstVertex.Y;
	temp.Z = FirstVertex.Z;

	switch(ClassifyVect(&temp,Plan))
	{
		case DEVANT:
			FrontList[FrontCounter++]=FirstVertex;
			break;

		case DERRIERE:
			BackList[BackCounter++]=FirstVertex;
			break;

		case CONFONDU:
		BackList[BackCounter++]=FirstVertex;
		FrontList[FrontCounter++]=FirstVertex;
			break;
	}

	for(loop=1 ; loop<Poly->NumberOfVertices+1 ; loop++)
	{
		if(loop==Poly->NumberOfVertices) 
			CurrentVertex=0;
		else
			CurrentVertex=loop;

		PointA.X = Poly->point[loop-1].X;
		PointA.Y = Poly->point[loop-1].Y;
		PointA.Z = Poly->point[loop-1].Z;

		PointB.X = Poly->point[CurrentVertex].X;
		PointB.Y = Poly->point[CurrentVertex].Y;
		PointB.Z = Poly->point[CurrentVertex].Z;

		Result=ClassifyVect(&PointB,Plan);
		if(Result==CONFONDU)
		{
			BackList[BackCounter++]=Poly->point[CurrentVertex];
			FrontList[FrontCounter++]=Poly->point[CurrentVertex];
		}
		else
		{
			if(Get_Intersect(&PointA,&PointB,*Plan,&IntersectPoint, &percent)==true)
			{
				deltax = Poly->point[CurrentVertex].U - Poly->point[loop-1].U;
				deltay = Poly->point[CurrentVertex].V - Poly->point[loop-1].V;
				texx = Poly->point[loop-1].U + (deltax*percent);
				texy = Poly->point[loop-1].V + (deltay*percent);
				copy.X = IntersectPoint.X;
				copy.Y = IntersectPoint.Y;
				copy.Z = IntersectPoint.Z;
				copy.U = texx;
				copy.V = texy;

		//		deltax = Poly->point[CurrentVertex].U_lm - Poly->point[loop-1].U_lm;
		//		deltay = Poly->point[CurrentVertex].V_lm - Poly->point[loop-1].V_lm;
		//		lmx = Poly->point[loop-1].U_lm + (deltax*percent);
		//		lmy = Poly->point[loop-1].V_lm + (deltay*percent);
		//		copy.U_lm = lmx;
		//		copy.V_lm = lmy;

				if(Result==DEVANT)
				{
					BackList[BackCounter++]=copy;			
					FrontList[FrontCounter++]=copy;
					if(CurrentVertex!=0)
						FrontList[FrontCounter++]=Poly->point[CurrentVertex];
				}	
				if(Result==DERRIERE)
				{
					FrontList[FrontCounter++]=copy;
					BackList[BackCounter++]=copy;		
					if(CurrentVertex!=0)
						BackList[BackCounter++]=Poly->point[CurrentVertex];
				}			
			}
			else
			{
				if(Result==DEVANT)
				{
					if(CurrentVertex!=0)
						 FrontList[FrontCounter++]=Poly->point[CurrentVertex];
				}
				if(Result==DERRIERE) 
				{
					if(CurrentVertex!=0)
						BackList[BackCounter++]=Poly->point[CurrentVertex];
				}
			}
		}
	}

	if(FrontSplit)
	{
		FrontSplit->NumberOfVertices=0;
		FrontSplit->point = malloc(sizeof(point_t)*FrontCounter);
		for(loop=0 ; loop<FrontCounter ; loop++)
		{
			FrontSplit->NumberOfVertices++;
			FrontSplit->point[loop]=FrontList[loop];
		}
	}
	if(BackSplit)
	{
		BackSplit->NumberOfVertices=0;
		BackSplit->point = malloc(sizeof(point_t)*BackCounter);
		for(loop=0 ; loop<BackCounter ; loop++)
		{
			BackSplit->NumberOfVertices++;
			BackSplit->point[loop]=BackList[loop];
		}
	}
}

// MARCHE PAS !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
bool IsVertexOnFace(vect_t V, RECTANGLE *Face)
{
	vect_t	V1, V2, V3;
	int		i, m;

	for(i=0 ; i<Face->NumberOfVertices ; i++)
	{
		m = i+1;
		if(m==Face->NumberOfVertices)
			m=0;

		V1.X =  Face->point[i].X - Face->point[m].X;
		V1.Y =  Face->point[i].Y - Face->point[m].Y;
		V1.Z =  Face->point[i].Z - Face->point[m].Z;

		V2 = PdVectoriel(V1,Face->Plan.VecteurNormal);

		V3.X = V.X - Face->point[i].X;
		V3.Y = V.Y - Face->point[i].Y;
		V3.Z = V.Z - Face->point[i].Z;

		//printf("%f\n",PdtScalaire(V2,V3));

		if( PdtScalaire(V2,V3) < 0 )
			return false;
	}
	return true;
}

bool IsVertexOnFace_2(vect_t V, RECTANGLE *Face)
{
	float	Teta;
	vect_t	V1, V2;
	int		i, m;

	if(!Face)
		return false;

	Teta = 0;
	for(i=0 ; i<Face->NumberOfVertices ; i++)
	{
		m = i+1;
		if(m==Face->NumberOfVertices)
			m=0;

		V1.X =  Face->point[i].X - V.X;
		V1.Y =  Face->point[i].Y - V.Y;
		V1.Z =  Face->point[i].Z - V.Z;

		V2.X =  Face->point[m].X - V.X;
		V2.Y =  Face->point[m].Y - V.Y;
		V2.Z =  Face->point[m].Z - V.Z;

		Normalize(&V1);
		Normalize(&V2);
		Teta = Teta + 1 - PdtScalaire(V1,V2);
	}

//	printf("Teta: %f\n",Teta);

//	if(Teta >= 3.99f)
	if(Teta >= 3.9f)
		return true;

	return false;
}

bool IsVertexEgalFace(vect_t v, RECTANGLE *Face)
{
	int		i;
	vect_t	v1;

	for(i=0 ; i<Face->NumberOfVertices ; i++)
	{
		v1.X = Face->point[i].X;
		v1.Y = Face->point[i].Y;
		v1.Z = Face->point[i].Z;

		if( IsVertexEgal(v,v1) == true )
			return true;
	}
	return false;
}

bool IsVertexEgal(vect_t v, vect_t a)
{
	if(IsEqualf(v.X,a.X))
	if(IsEqualf(v.Y,a.Y))
	if(IsEqualf(v.Z,a.Z))
		return true;
	return false;
}

bool IsVertexOnEdgeFace(vect_t v, RECTANGLE *Face)
{
	int		i;
	int		m;
	vect_t	v1, v2;

	for(i=0 ; i<Face->NumberOfVertices ; i++)
	{
		m=i+1;
		if(m==Face->NumberOfVertices)
			m=0;

		v1.X = Face->point[i].X;
		v1.Y = Face->point[i].Y;
		v1.Z = Face->point[i].Z;

		v2.X = Face->point[m].X;
		v2.Y = Face->point[m].Y;
		v2.Z = Face->point[m].Z;

		if( IsVertexOnEdge(v,v1,v2) == true )
			return true;
	}
	return false;
}

bool IsVertexOnEdge(vect_t v, vect_t a, vect_t b)
{
	vect_t	v1, v2;
	
	v1.X = a.X - v.X;
	v1.Y = a.Y - v.Y;
	v1.Z = a.Z - v.Z;

	v2.X = b.X - v.X;
	v2.Y = b.Y - v.Y;
	v2.Z = b.Z - v.Z;

	Normalize(&v1);
	Normalize(&v2);

	if(PdtScalaire(v1,v2) != -1)
		return false;

	return true;
}

RECTANGLE * Load_XYZ(char *nom, char *data)
{
	FILE		*s;
	char		path[255];
	int			i, j;
	int			nTextures;
	RECTANGLE	*FaceList=NULL;
	RECTANGLE	*ObjectList=NULL;
	RECTANGLE	*pFace=NULL;
	char		IsDetail;
	bool		DoorList;
	int			Nb_faces;

	Rename(nom,"xyz");
	s = fopen(nom, "r");
	if(s==NULL)
		return NULL;

	//
	//	START
	//
	fscanf(s,"%d\n",&gNb_Faces);
	fscanf(s,"PLAYERPOS %f %f %f\n",&gStartX,&gStartY,&gStartZ);

	//
	//	ENTITEE
	//
	fscanf(s,"%d\n",&Nb_entities);
	i=Nb_entities;
	while(i--)
	{
		fscanf(s,"%d %d %f %f %f\n",&Entities2[i].id,&Entities2[i].angleY,&Entities2[i].pos.X,&Entities2[i].pos.Y,&Entities2[i].pos.Z);
	//	Entities[i].id = Entity_ID;
	}

	//
	//	TEXTURES
	//
	fscanf(s,"%d",&nTextures);
	fgets(path,255,s);
	while(nTextures--)
	{
		fgets(path,255,s);

		i=0;
		while(path[i])
		{
			i++;
			if(i>=255)
			{
				printf("Texture name error\n");
				return NULL;
			}
		}
		while(path[i] != 92) // '\'
		{
			if(path[i]=='"')
				path[i]='\0';
			i--;

			if(!i)
				break;

			if(i<0)
			{
				printf("Texture name error #2\n");
				return NULL;
			}
		}
		sprintf(TextureList2[Nb_textures],&path[i+1]);
	//	sprintf(tmp,&path[i+1]);
		Nb_textures++;
	}

	fscanf(s,"\n");
	Nb_faces=0;
	DoorList=false;

	for(i=0 ; i<gNb_Faces ; i++)
	{
		pFace = malloc(sizeof(RECTANGLE));
		memset(pFace,0,sizeof(RECTANGLE));

		pFace->point = malloc(sizeof(point_t)*3);
		memset(pFace->point,0,sizeof(point_t)*3);

		pFace->NumberOfVertices = 3;
		
	//	fscanf(s, "%c\n",&IsDetail);
		fscanf(s, "%c",&IsDetail);
		switch(IsDetail)
		{
		case 'W':
			pFace->IsDetailObjet=WALL;
			break;

		case 'L':
			pFace->IsDetailObjet=BSPLESS;
			break;

		case 'D':
			pFace->IsDetailObjet=DETAIL;
			break;

		case 'A':
			fscanf(s,"%d",&ActionNumber);
			//printf("%d",ActionNumber);
			pFace->IsDetailObjet = ACTION;
			pFace->ActionNumber = ActionNumber;
			break;

		case 'V':
			pFace->IsDetailObjet=LAVA;
			break;

		case 'I':
			pFace->IsDetailObjet=INVI;
			break;

		case 'N':
			pFace->IsDetailObjet=INVI2;
			break;

		case 'O':
			pFace->IsDetailObjet=DOOR;
			break;

		default:
			pFace->IsDetailObjet=WALL;
			break;
		}

		fscanf(s,"\n");


		for(j=0 ; j<pFace->NumberOfVertices ; j++)
		{
			fscanf(s, "%f %f %f %f %f\n",
						&pFace->point[j].X,
						&pFace->point[j].Y,
						&pFace->point[j].Z,
						&pFace->point[j].U,
						&pFace->point[j].V);
		}

		fscanf(s,"%d\n",&pFace->textId);
		fscanf(s,"%d\n",&pFace->BrushID);

		if(pFace->IsDetailObjet == DETAIL)
			pFace->BrushID = -2;
	//	if(pFace->IsDetailObjet == BSPLESS)
	//		pFace->BrushID = -2;
		if(pFace->IsDetailObjet == INVI)
			pFace->BrushID = -3;
		if(pFace->IsDetailObjet == INVI2)
			pFace->BrushID = -12345;
		if(pFace->IsDetailObjet == ACTION)
			pFace->BrushID = -pFace->BrushID-4;

		pFace->BSP_used = false;
		pFace->Used = false;

		pFace->Plan.VecteurNormal = GetFaceNormal(pFace->point[0],pFace->point[1],pFace->point[2]);		
		pFace->Plan.PointOnPlane.X = pFace->point[0].X;
		pFace->Plan.PointOnPlane.Y = pFace->point[0].Y;
		pFace->Plan.PointOnPlane.Z = pFace->point[0].Z;

	//	printf("%f %f %f\n",pFace->point[0].X,pFace->point[0].Y,pFace->point[0].Z);
	//	printf("%f %f %f\n",pFace->point[1].X,pFace->point[1].Y,pFace->point[1].Z);
	//	printf("%f %f %f\n",pFace->point[2].X,pFace->point[2].Y,pFace->point[2].Z);
	
	//	printf("%f %f %f\n",pFace->Plan.VecteurNormal.X,pFace->Plan.VecteurNormal.Y,pFace->Plan.VecteurNormal.Z);

		pFace->Next = FaceList;
		FaceList = pFace;
		Nb_faces++;

		if(feof(s) == true)
			break;
	}

	fclose(s);
	gNb_Faces = Nb_faces;

//	PutObjectToFile(nom);

	if(FaceList==NULL)
		Error("Not a valid XYZ file\n",data);
	return FaceList;
}

void AddObject(RECTANGLE *list)
{
	if(Nb_object>=255)
		return;
	Objets[Nb_object].type = 0;
	Objets[Nb_object].liste = list;
	Nb_object++;
}

bool PutObjectToFile(char *path)
{
	FILE		*file;
	RECTANGLE	*pRect;
	RECTANGLE	*list;
	int			i;
	int			j;
	int			face;

//	if(menu_EntityComp)
//		return true;

	Rename(path,"mel");
	remove(path);

	file = fopen(path,"w");
	if(file==NULL)
		return false;

	for(j=0 ; j<Nb_object ; j++)
	{
		list = Objets[j].liste;
		face = CountFaces(list);
	//	fprintf(file,"%d\n",face);

		pRect = list;
		while(pRect)
		{
			fprintf(file,"%d\n",pRect->NumberOfVertices);
			fprintf(file,"%d\n",pRect->textId);

			for(i=0 ; i<pRect->NumberOfVertices ; i++)
			{
				fprintf(file,"%f %f %f %f %f\n",
						pRect->point[i].X,
						pRect->point[i].Y,
						pRect->point[i].Z,
						pRect->point[i].U,
						pRect->point[i].V);
			}

			pRect = pRect->Next;
		}
	}

	fclose(file);
	printf("%s\n",path);
	return true;
}

int CountFaces(face_t facelist)
{
	int		count;
	face_t	pCurr;

	for(count=0,pCurr=facelist ; pCurr ; pCurr=pCurr->Next)
		count++;

	return count;
}

face_t AllocFace(int Nb_Points, bool track)
{
	face_t	f;

//	if(track)
//		Nb_faces++;

	f = malloc(sizeof(RECTANGLE));
	memset(f,0,sizeof(RECTANGLE));
	
	if(Nb_Points)
	{
		f->NumberOfVertices = Nb_Points;
		f->point = malloc(sizeof(point_t)*Nb_Points);
		memset(f->point,0,sizeof(point_t)*Nb_Points);
		f->BrushID = -1;
	}

	return f;
}

void FreeFace(face_t f, bool track)
{
//	if(track)
//		Nb_faces--;
	
	if(f)
	{
		if(f->point)
			free(f->point);
		free(f);
	}
}


RECTANGLE *RemoveFaceFromList(RECTANGLE *racine, RECTANGLE *remove)
{
	RECTANGLE *temp;

	if(remove==NULL)
		return racine;

	if(remove == racine)
	{
		temp = racine->Next;
		FreeFace(racine,false);
		return temp;
	}
	else if(racine->Next)
	{
		temp = racine;
		while(temp->Next != remove)
		{
			if(temp->Next==NULL)
				return racine;
			temp = temp->Next;
		}

		temp->Next = remove->Next;

		FreeFace(remove,false);
	}
	return racine;
}

pFace AddListes(pFace PortalList, pFace AddList)
{
	pFace	Portal;

	if(AddList==NULL)
		return PortalList;

	if(PortalList==NULL)
		return AddList;
	
	Portal = AddList;
	while(AddList->Next)
		AddList = AddList->Next;
	AddList->Next = PortalList;
	PortalList = Portal;

	return PortalList;
}

face_t GroupFaces(face_t PolyList)
{
	RECTANGLE	*pFace;
	RECTANGLE	*pFace2;
	RECTANGLE	*pTemp;
	RECTANGLE	*pTemp2;
	RECTANGLE	*pNew;
	RECTANGLE	*pNewList=NULL;
	int			i, j, real;
	int			score;
	int			score1;
	int			score2;
	vect_t		v1,v2,v3;
	int			i1,i2;
	float		val;
	bool		IsValid;
//	int			tmp;

//	if(!menu_groupface)
//		return PolyList;

	printf("Group Faces(%d)..\n",CountFaces(PolyList));
	
//	tmp = gNb_Faces;

	for(pFace=PolyList ; pFace ; pFace=pFace->Next)
		pFace->Used = false;
		
	pFace = PolyList;
	while(pFace)
	{
		pTemp = pFace;
		pFace = pFace->Next;

		if(pTemp->Used)
			continue;
		if(pTemp->IsDetailObjet)
			continue;

		pFace2 = PolyList;
		while(pFace2)
		{
			pTemp2 = pFace2;
			pFace2 = pFace2->Next;

			if(pTemp2->Used)
				continue;
			if(pTemp2->IsDetailObjet)
				continue;
			if(pTemp == pTemp2)
				continue;
			if(pTemp->textId != pTemp2->textId)
				continue;
			if(ClassifyPoly(&pTemp->Plan,pTemp2) != CONFONDU)
				continue;
			if(ClassifyPoly(&pTemp2->Plan,pTemp) != CONFONDU)	// en plus
				continue;

			score=0;
			for(i=0 ; i<pTemp->NumberOfVertices ; i++)
			for(j=0 ; j<pTemp2->NumberOfVertices ; j++)
			{
				if(IsEqualf(pTemp->point[i].X,pTemp2->point[j].X))
				if(IsEqualf(pTemp->point[i].Y,pTemp2->point[j].Y))
				if(IsEqualf(pTemp->point[i].Z,pTemp2->point[j].Z))
				if(IsEqualf(pTemp->point[i].U,pTemp2->point[j].U))
				if(IsEqualf(pTemp->point[i].V,pTemp2->point[j].V))
				{
					pTemp->point[i].GroupUsed = true;
					pTemp2->point[j].GroupUsed = true;
					score++;
				}
			}

			score1=0;
			for(i=0 ; i<pTemp->NumberOfVertices ; i++)
			if(pTemp->point[i].GroupUsed)
					score1++;
			
			score2=0;
			for(i=0 ; i<pTemp2->NumberOfVertices ; i++)
			if(pTemp2->point[i].GroupUsed)
					score2++;

			if( (score != 2) || (score1 != 2) || (score2 != 2) )
			{
				for(i=0 ; i<pTemp->NumberOfVertices ; i++)
					pTemp->point[i].GroupUsed = false;

				for(i=0 ; i<pTemp2->NumberOfVertices ; i++)
					pTemp2->point[i].GroupUsed = false;
				
				continue;
			}

			pTemp->Used = true;
			pTemp2->Used = true;

			pNew = AllocFace(pTemp->NumberOfVertices + pTemp2->NumberOfVertices - 2, true);
			pNew->Plan = pTemp->Plan;

			real=0;

			for(i=0 ; i<pTemp->NumberOfVertices ; i++)
			if(pTemp->point[i].GroupUsed && !pTemp->point[(i+1)%pTemp->NumberOfVertices].GroupUsed)
				break;

			for(j=0 ; j<pTemp->NumberOfVertices ; j++)
			{
				pNew->point[real++] = pTemp->point[(j+i)%pTemp->NumberOfVertices];
			}

			for(i=0 ; i<pTemp2->NumberOfVertices ; i++)
			if(pTemp2->point[i].GroupUsed && !pTemp2->point[(i+1)%pTemp2->NumberOfVertices].GroupUsed)
				break;
		
			for(j=0 ; j<pTemp2->NumberOfVertices-2 ; j++)
				pNew->point[real++] = pTemp2->point[(j+i+1)%pTemp2->NumberOfVertices];

			// regarde si c'est bien un polygonne convexe
			IsValid=true;
			for(i=0 ; i<pNew->NumberOfVertices ; i++)
			{
				if(i==0)
					i1 = pNew->NumberOfVertices-1;
				else
					i1 = i-1;

				if(i==pNew->NumberOfVertices-1)
					i2=0;
				else
					i2=i+1;

				v1.X = pNew->point[i1].X - pNew->point[i].X;
				v1.Y = pNew->point[i1].Y - pNew->point[i].Y;
				v1.Z = pNew->point[i1].Z - pNew->point[i].Z;

				v2.X = pNew->point[i2].X - pNew->point[i].X;
				v2.Y = pNew->point[i2].Y - pNew->point[i].Y;
				v2.Z = pNew->point[i2].Z - pNew->point[i].Z;

				v3 = PdVectoriel(v1,v2);
				val=PdtScalaire(v3,pNew->Plan.VecteurNormal);
				if(val<0)
				{
					//printf("%f\n",val);
					IsValid=false;
				}
			}
			if(!IsValid)
			{
				FreeFace(pNew,0);
				pTemp->Used = false;
				pTemp2->Used = false;
				continue;
			}

			pNew->textId = pTemp->textId;
			pNew->IsDetailObjet = pTemp->IsDetailObjet;
			pNew->Next = pNewList;
			pNewList = pNew;
		}
	}

	pFace = PolyList;
	while(pFace)
	{
		if(pFace->Used)
		{
			PolyList = RemoveFaceFromList(PolyList,pFace);
			pFace = PolyList;
			continue;
		}
		pFace = pFace->Next;
	}

	PolyList = AddListes(PolyList,pNewList);
	gNb_Faces = CountFaces(PolyList);

//	if(gNb_Faces<tmp)
//		PolyList = GroupFaces(PolyList);

	return PolyList;
}

bool PutPolyToFile(char *path, RECTANGLE *RectList)
{
	FILE		*file;
	RECTANGLE	*pRect;
	int			i, k;
	float		zero=0;

	if(menu_makemap)
		Rename(path,"pts");
	else
		Rename(path,"ent");
	remove(path);

	file = fopen(path,"w");
	if(file==NULL)
		return false;

	if(!menu_makemap)
		fprintf(file,"<ENTITY>\n");
	else
	{
		fprintf(file,"<POLYGONS>\n");
		fprintf(file,"<PLAYERPOS> %f %f %f\n",gStartX,gStartY,gStartZ);
		fprintf(file,"<PLAYERDIR> %d\n",PlayerAngle);

		// Entitees
		fprintf(file,"<ENTITIES> %d\n",Nb_entities);
		for(i=0 ; i<Nb_entities ; i++)
			fprintf(file,"%d %d %f %f %f\n",Entities2[i].id,Entities2[i].angleY,Entities2[i].pos.X,Entities2[i].pos.Y,Entities2[i].pos.Z);
	}

	// Textures
	fprintf(file,"<TEXTURES> %d\n",Nb_textures);
	for(k=0 ; k<Nb_textures ; k++)
		fprintf(file,"%s\n",TextureList2[k]);

	// Lights sources
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

	pRect = RectList;
	while(pRect)
	{
		//printf("%d ",pRect->ActionNumber);
		//printf("%d ",pRect->BrushID);
		if(menu_makemap)
			fprintf(file,"%d %d %d %d %d\n",
					pRect->NumberOfVertices,
					pRect->textId,
					pRect->ActionNumber,
					pRect->IsDetailObjet,
					pRect->BrushID);
		else
			fprintf(file,"%d %d\n",pRect->NumberOfVertices,pRect->textId);
		for(i=0 ; i<pRect->NumberOfVertices ; i++)
		{
			fprintf(file,"%f %f %f %f %f %f %f\n",
					pRect->point[i].X,
					pRect->point[i].Y,
					pRect->point[i].Z,
					pRect->point[i].U,
					pRect->point[i].V,
					zero,
					zero);
		}

		pRect = pRect->Next;
	}
	fclose(file);
	printf("%s\n",path);
	return true;
}

face_t SplitToTriangle(face_t PolyList)
{
	RECTANGLE	*pFace;
	RECTANGLE	*pTemp;
	RECTANGLE	*pNew;
	RECTANGLE	*pNewList=NULL;
	int			Nb_new_faces=0;
	int			Nb_split;
	int			loop;

	pFace = PolyList;
	while(pFace)
	{
		pTemp = pFace;
		pFace = pFace->Next;

		if(pTemp->NumberOfVertices == 3)
		{
			pTemp->Next = pNewList;
			pNewList = pTemp;
			Nb_new_faces++;
		}
		else if(pTemp->NumberOfVertices > 3)
		{
			Nb_split = pTemp->NumberOfVertices - 2;

			loop=1;
			while(Nb_split--)
			{
				pNew = AllocFace(3,true);

				pNew->Plan = pTemp->Plan;
				pNew->textId = pTemp->textId;
				pNew->IsDetailObjet = pTemp->IsDetailObjet;

				pNew->point[0].X = pTemp->point[0].X;
				pNew->point[0].Y = pTemp->point[0].Y;
				pNew->point[0].Z = pTemp->point[0].Z;
				pNew->point[0].U = pTemp->point[0].U;
				pNew->point[0].V = pTemp->point[0].V;

				pNew->point[1].X = pTemp->point[loop].X;
				pNew->point[1].Y = pTemp->point[loop].Y;
				pNew->point[1].Z = pTemp->point[loop].Z;
				pNew->point[1].U = pTemp->point[loop].U;
				pNew->point[1].V = pTemp->point[loop].V;

				pNew->point[2].X = pTemp->point[loop+1].X;
				pNew->point[2].Y = pTemp->point[loop+1].Y;
				pNew->point[2].Z = pTemp->point[loop+1].Z;
				pNew->point[2].U = pTemp->point[loop+1].U;
				pNew->point[2].V = pTemp->point[loop+1].V;

				loop++;

			//	if(CheckFace(pNew))
			//	{
					pNew->Next = pNewList;
					pNewList = pNew;
				//	Nb_new_faces++;
					gNb_Faces++;
			//	}
			//	else
			//		FreeFace(pNew,false);
			}
		}
	}
	return pNewList;
}

