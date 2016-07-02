
#include "bsp2math.h"


float Norme(vect_t pVect)
{
	return (float) sqrt(pVect.X*pVect.X + pVect.Y*pVect.Y + pVect.Z*pVect.Z);
}

bool Normalize(vect_t *pVect)
{
	float length, len;
  
	length=Norme(*pVect);
	if(length==0)
		return false;
	len=1.0f/length;
  	pVect->X *= len; 
	pVect->Y *= len; 
	pVect->Z *= len;
	return true;
}

float PdtScalaire(vect_t pVect1, vect_t pVect2)
{
	return pVect1.X*pVect2.X + pVect1.Y*pVect2.Y + pVect1.Z*pVect2.Z;
}

vect_t PdVectoriel(vect_t Vec1, vect_t Vec2)
{
	vect_t Vec3;

	Vec3.X = Vec1.Y * Vec2.Z - Vec1.Z * Vec2.Y; 
	Vec3.Y = Vec1.Z * Vec2.X - Vec1.X * Vec2.Z; 
	Vec3.Z = Vec1.X * Vec2.Y - Vec1.Y * Vec2.X;

	return Vec3;
}

vect_t GetFaceNormal(point_t Vec0, point_t Vec1, point_t Vec2)
{
	vect_t	p, q, n;

	p.X = Vec2.X - Vec0.X;
	p.Y = Vec2.Y - Vec0.Y;
	p.Z = Vec2.Z - Vec0.Z;
		
	q.X = Vec1.X - Vec0.X;  
	q.Y = Vec1.Y - Vec0.Y;  
	q.Z = Vec1.Z - Vec0.Z;  
		
	n = PdVectoriel(p,q);
	Normalize(&n);

	return n;
}

int ClassifyVect(vect_t *pos, plan_t *Plan)
{
	float	result;
	vect_t	Direction;

	Direction.X = Plan->PointOnPlane.X - pos->X;
	Direction.Y = Plan->PointOnPlane.Y - pos->Y;
	Direction.Z = Plan->PointOnPlane.Z - pos->Z;

	result = PdtScalaire(Plan->VecteurNormal,Direction);

	if(result>EPSILON)
		return DEVANT;

	if(result<-EPSILON)
		return DERRIERE;

	return CONFONDU;
}

int ClassifyPoint(point_t *pos, plan_t * Plan)
{
	vect_t	v;

	v.X = pos->X;
	v.Y = pos->Y;
	v.Z = pos->Z;

	return ClassifyVect(&v,Plan);
}

int ClassifyPoly(plan_t *Plan, RECTANGLE *Poly)
{
	int		Infront=0;
	int		Behind=0;
	int		OnPlane=0;
	int		i;

	for(i=0 ; i<Poly->NumberOfVertices ; i++)
	{
		switch(ClassifyPoint(&Poly->point[i],Plan))
		{
			case DEVANT:
			Infront++;
			break;

			case DERRIERE:
			Behind++;
			break;

			default:
			OnPlane++;
			Infront++;
			Behind++;
			break;
		}
	}

	if(OnPlane==Poly->NumberOfVertices)
		return CONFONDU;
	if(Behind==Poly->NumberOfVertices)
		return DERRIERE;
	if(Infront==Poly->NumberOfVertices)
		return DEVANT;

	return AUMILIEU;
}

int NombreDePointsEnCommunEntreDeuxPoly(pFace f1, pFace f2)
{
	int		count;
	int		i;
	int		j;
	point_t	p1, p2, p3, p4;

	count = 0;

	// trouve une arrete commune
	for(i=0 ; i<f1->NumberOfVertices ; i++)
	{
		p1 = f1->point[i];
		p2 = f1->point[(i+1)%f1->NumberOfVertices];
		for(j=0 ; j<f2->NumberOfVertices ; j++)
		{
			p3 = f2->point[j];
			p4 = f2->point[(j+1)%f2->NumberOfVertices];

			if(IsEqualf(p1.X,p4.X))
			if(IsEqualf(p1.Y,p4.Y))
			if(IsEqualf(p1.Z,p4.Z))
			if(IsEqualf(p2.X,p3.X))
			if(IsEqualf(p2.Y,p3.Y))
			if(IsEqualf(p2.Z,p3.Z))
				count++;
		}
	}

	/*

	for(i=0 ; i<f1->NumberOfVertices ; i++)
	for(j=0 ; j<f2->NumberOfVertices ; j++)
	{
		if(IsEqualf(f1->point[i].X,f2->point[j].X))
		if(IsEqualf(f1->point[i].Y,f2->point[j].Y))
		if(IsEqualf(f1->point[i].Z,f2->point[j].Z))
	//	if(IsEqualf(f1->point[i].U,f2->point[j].U))
	//	if(IsEqualf(f1->point[i].V,f2->point[j].V))
			count++;
	}
	*/
	return count*2;
}

bool IsConvexPolytope(pFace Poly)
{
	pFace	pCurr1;
	pFace	pCurr2;
	int		pec;
	bool	b;

	b = false;
	for(pCurr1=Poly ; pCurr1 ; pCurr1=pCurr1->Next)
	{
		for(pCurr2=Poly ; pCurr2 ; pCurr2=pCurr2->Next)
		{
			if(pCurr1 == pCurr2)
				continue;
			pec = NombreDePointsEnCommunEntreDeuxPoly(pCurr1,pCurr2);
			//printf("%d ",pec);
			if(pec >= 2)
			{
				b = true;
				if(!IsConvexPoly(pCurr1,pCurr2))
					return false;
			}
		}
	}
	return b;
}

bool IsConvexPoly(pFace f1, pFace f2)
{
	int		i;

//	if(NombreDePointsEnCommunEntreDeuxPoly(f1,f2) != 2)
//		return false;

	for(i=0 ; i<f1->NumberOfVertices ; i++)
	if( ClassifyPoint(&f1->point[i],&f2->Plan) == DEVANT)
		return false;
	return true;
}

void Spliting(pFace Poly, plan_t *Plan, pFace FrontSplit, pFace BackSplit, pFace Cuts)
{
	point_t		FrontList[40],BackList[40],FirstVertex,copy;
	vect_t		IntersectPoint,PointA,PointB,temp;
	int			FrontCounter=0,BackCounter=0,loop=0,CurrentVertex=0;
	float		percent;
	int			Result;
	float		deltax,deltay,texx,texy, lmx, lmy;
	
	int			InterCounter=0;
	point_t		Inter[40];
	point_t		*NewPointList = NULL;

	FirstVertex=Poly->point[0];

	FirstVertex.NX = 0;
	FirstVertex.NY = 0;
	FirstVertex.NZ = 0;

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

				deltax = Poly->point[CurrentVertex].U_lm - Poly->point[loop-1].U_lm;
				deltay = Poly->point[CurrentVertex].V_lm - Poly->point[loop-1].V_lm;
				lmx = Poly->point[loop-1].U_lm + (deltax*percent);
				lmy = Poly->point[loop-1].V_lm + (deltay*percent);
				copy.U_lm = lmx;
				copy.V_lm = lmy;

				Inter[InterCounter] = copy;
				InterCounter++;

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

	if(FrontSplit && FrontCounter && !BackCounter)
	{
		FrontSplit = Poly;
	//	FrontSplit->OriginalFace = Poly->OriginalFace;

		if(BackSplit)
		{
			free(BackSplit);
			BackSplit = NULL;
		}
		return;
	}
	if(BackSplit && BackCounter && !FrontCounter)
	{
		BackSplit = Poly;
	//	BackSplit->OriginalFace = Poly;

		if(FrontSplit)
		{
			free(FrontSplit);
			FrontSplit = NULL;
		}
		return;
	}

	if(FrontSplit && FrontCounter)
	{
		FrontSplit->NumberOfVertices=0;
		FrontSplit->point = malloc(sizeof(point_t)*FrontCounter);
		for(loop=0 ; loop<FrontCounter ; loop++)
		{
			FrontSplit->NumberOfVertices++;
			FrontSplit->point[loop]=FrontList[loop];
		}
		FrontSplit->Plan = Poly->Plan;
		FrontSplit->textId = Poly->textId;
		FrontSplit->IsDetailObjet = Poly->IsDetailObjet;
		FrontSplit->BrushID = Poly->BrushID;
		FrontSplit->NodeID = Poly->NodeID;

		FrontSplit->OriginalFace = Poly->OriginalFace;
	}
	if(BackSplit && BackCounter)
	{
		BackSplit->NumberOfVertices=0;
		BackSplit->point = malloc(sizeof(point_t)*BackCounter);
		for(loop=0 ; loop<BackCounter ; loop++)
		{
			BackSplit->NumberOfVertices++;
			BackSplit->point[loop]=BackList[loop];
		}
		BackSplit->Plan = Poly->Plan;
		BackSplit->textId = Poly->textId;
		BackSplit->IsDetailObjet = Poly->IsDetailObjet;
		BackSplit->BrushID = Poly->BrushID;
		BackSplit->NodeID = Poly->NodeID;

		BackSplit->OriginalFace = Poly->OriginalFace;
	}
//	if(Poly->point)
//		free(Poly->point);
//	free(Poly);
}

bool Get_Intersect(vect_t *linestart,vect_t *lineend,plan_t plan,vect_t * intersection, float *percentage)
{
	vect_t direction,L1;
	float linelength,dist_from_plane;
	vect_t *vertex;
	vect_t *normal;

	vertex = &(plan.PointOnPlane);
	normal = &(plan.VecteurNormal);

	direction.X = lineend->X - linestart->X;
	direction.Y = lineend->Y - linestart->Y;
	direction.Z = lineend->Z - linestart->Z;

	linelength=PdtScalaire(direction,*normal);
	if(Absf(linelength)<EPSILON) 
		return false; 

	L1.X = vertex->X - linestart->X;
	L1.Y = vertex->Y - linestart->Y;
	L1.Z = vertex->Z - linestart->Z;

	dist_from_plane = PdtScalaire(L1,*normal);
	*percentage = dist_from_plane/linelength;

	if(*percentage<0.0f)
		return false;
	else if(*percentage>1.0f)
		return false;
	else
	{
		intersection->X = linestart->X + direction.X*(*percentage);
		intersection->Y = linestart->Y + direction.Y*(*percentage);
		intersection->Z = linestart->Z + direction.Z*(*percentage);
	}
	return true;
}

float Absf(float value)
{
	if(value<0)
		return -value;
	else
		return value;
}

bool IsEqualf(float A, float B)
{
	if(Absf(A-B) <= EQUAL_EPSILON)
		return true;
	else
		return false;
}

bool IsVectInRect(pFace Face, vect_t CP)
{
	int		i;
	vect_t	pt1[3];

	for(i=0 ; i<Face->NumberOfVertices-2 ; i++)
	{
		pt1[0].X = Face->point[0].X;
		pt1[0].Y = Face->point[0].Y;
		pt1[0].Z = Face->point[0].Z;

		pt1[1].X = Face->point[1+i].X;
		pt1[1].Y = Face->point[1+i].Y;
		pt1[1].Z = Face->point[1+i].Z;
		
		pt1[2].X = Face->point[2+i].X;
		pt1[2].Y = Face->point[2+i].Y;
		pt1[2].Z = Face->point[2+i].Z;

		if(IsPointInTri(pt1,Face->Plan.VecteurNormal,CP) )
			return true;
	}
	return false;
}

bool IsPointInTri(vect_t point[3], vect_t Normal, vect_t CP)
{
	int		i1;
	int		i2;
	float	a;
	float	b;
	vect_t	u;
	vect_t	v;
	vect_t	n;
	vect_t	A;
	float	v1[3];
	float	v2[3];
	float	v3[3];
	float	P[3];

	P[0] = CP.X;
	P[1] = CP.Y;
	P[2] = CP.Z;

	v1[0] = point[0].X;
	v1[1] = point[0].Y;
	v1[2] = point[0].Z;

	v2[0] = point[1].X;
	v2[1] = point[1].Y;
	v2[2] = point[1].Z;

	v3[0] = point[2].X;
	v3[1] = point[2].Y;
	v3[2] = point[2].Z;

    n = Normal;

	A.X = (float)fabs(n.X);
	A.Y = (float)fabs(n.Y);
	A.Z = (float)fabs(n.Z);

	if(A.X>A.Y)
	{
		if(A.X>A.Z)
		{
			i1=1;
			i2=2;
		}
		else
		{
			i1=0;
			i2=1;
		}
	}
	else
	{
		if(A.Y>A.Z)
		{
			i1=0;
			i2=2;
		}
		else
		{
			i1=0;
			i2=1;
		}
	}

	u.X = P[i1]-v1[i1];
	u.Y = v2[i1]-v1[i1];
	u.Z = v3[i1]-v1[i1];

	v.X = P[i2]-v1[i2];
	v.Y = v2[i2]-v1[i2];
	v.Z = v3[i2]-v1[i2];
	
    if (u.Y==0.0f)
    {
      b=u.X/u.Z;
      if(b>=0.0f && b<=1.0f)
		  a=(v.X-b*v.Z)/v.Y;
      else
		  return false;
    }
    else
    {
      b=(v.X*u.Y-u.X*v.Y)/(v.Z*u.Y-u.Z*v.Y);
      if (b>=0.0f && b<=1.0f)
		  a=(u.X-b*u.Z)/u.Y;
      else
		  return false;
    }
    return (a>=0 && (a+b)<=1);
}

void GetBound(face_t face, BOUNDINGBOX *Bound)
{
	int		i;
	pFace	Face;

	Bound->BoxMin.X = Bound->BoxMax.X = face->point[0].X;
	Bound->BoxMin.Y = Bound->BoxMax.Y = face->point[0].Y;
	Bound->BoxMin.Z = Bound->BoxMax.Z = face->point[0].Z;

	for(Face=face ; Face ; Face=Face->Next)
	{
		if(Face->IsDetailObjet)
			continue;
		for(i=0 ; i<Face->NumberOfVertices ; i++)
		{
			if(Face->point[i].X < Bound->BoxMin.X)
				Bound->BoxMin.X = Face->point[i].X;
			if(Face->point[i].X > Bound->BoxMax.X)
				Bound->BoxMax.X = Face->point[i].X;

			if(Face->point[i].Y < Bound->BoxMin.Y)
				Bound->BoxMin.Y = Face->point[i].Y;
			if(Face->point[i].Y > Bound->BoxMax.Y)
				Bound->BoxMax.Y = Face->point[i].Y;

			if(Face->point[i].Z < Bound->BoxMin.Z)
				Bound->BoxMin.Z = Face->point[i].Z;
			if(Face->point[i].Z > Bound->BoxMax.Z)
				Bound->BoxMax.Z = Face->point[i].Z;
		}
	}
}

bool IsFlatBound(BOUNDINGBOX Bound)
{
	if(Bound.BoxMin.X == Bound.BoxMax.X ||
		Bound.BoxMin.Y == Bound.BoxMax.Y ||
		Bound.BoxMin.Z == Bound.BoxMax.Z)
		return true;
	return false;
}

void ClearBound(BOUNDINGBOX *bbox)
{
	bbox->BoxMin.X = MAX_WORLD;
	bbox->BoxMin.Y = MAX_WORLD;
	bbox->BoxMin.Z = MAX_WORLD;

	bbox->BoxMax.X = -MAX_WORLD;
	bbox->BoxMax.Y = -MAX_WORLD;
	bbox->BoxMax.Z = -MAX_WORLD;
}

bool IsValidBound(BOUNDINGBOX *bbox)
{
	if(bbox->BoxMin.X == MAX_WORLD)
		return false;
	if(bbox->BoxMin.Y == MAX_WORLD)
		return false;
	if(bbox->BoxMin.Z == MAX_WORLD)
		return false;
	if(bbox->BoxMax.X == -MAX_WORLD)
		return false;
	if(bbox->BoxMax.Y == -MAX_WORLD)
		return false;
	if(bbox->BoxMax.Z == -MAX_WORLD)
		return false;

	if(bbox->BoxMin.X == bbox->BoxMax.X && bbox->BoxMin.Y == bbox->BoxMax.Y && bbox->BoxMin.Z == bbox->BoxMax.Z)
		return false;

	return true;
}

void SizeFaceToBound(pFace Face, BOUNDINGBOX bbox)
{
	int		i;

	for(i=0 ; i<Face->NumberOfVertices ; i++)
	{
		if(Face->point[i].X > bbox.BoxMax.X)
			Face->point[i].X = bbox.BoxMax.X;
		if(Face->point[i].Y > bbox.BoxMax.Y)
			Face->point[i].Y = bbox.BoxMax.Y;
		if(Face->point[i].Z > bbox.BoxMax.Z)
			Face->point[i].Z = bbox.BoxMax.Z;

		if(Face->point[i].X < bbox.BoxMin.X)
			Face->point[i].X = bbox.BoxMin.X;
		if(Face->point[i].Y < bbox.BoxMin.Y)
			Face->point[i].Y = bbox.BoxMin.Y;
		if(Face->point[i].Z < bbox.BoxMin.Z)
			Face->point[i].Z = bbox.BoxMin.Z;
	}
}

bool IsAxialFace(pFace Face)
{
	vect_t	Normal;

	Normal = Face->Plan.VecteurNormal;
	if( (Normal.X == 0 && Normal.Y == 0) ||
		(Normal.Y == 0 && Normal.Z == 0) ||
		(Normal.Z == 0 && Normal.X == 0) )
		return true;

	return false;
}
