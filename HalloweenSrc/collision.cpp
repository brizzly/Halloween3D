
#include "alloween.cpp"


const float hEpsilon=1e-8f;

#define IsZero(f)  	fabs(f)<hEpsilon
#define	MAX_PLANEBUMP	4
#define	OFFSET_DEPTH	50.0f


void HMove(vect_t *start, vect_t *end, CTRACE *ctrace)
{
	Face_t	*FaceList;
	int		leaf;
	int		i;
	int		pvs_index;
	vect_t	start2, end2;

	if(MapDataFormat == MAP_POLYGONS)
	{
		FaceList = PolyList;
		TraceMove(*start,end,FaceList,ctrace);
		if(ctrace->col == false)
			*start = *end;
		else
			*start = ctrace->impact;
	}
	else if(MapDataFormat == MAP_BSP)
	{
		leaf = 	bsp_FindCamera(*start);
		if(leaf > -1)
		{
			pvs_index = LeafArray[leaf].PVSIndex;
			for(i=0 ; i<gNb_Leafs ; i++)
			if(PVSData[pvs_index+i] != '0')
			{
				FaceList = LeafArray[i].faceliste;
				TraceMove(*start,end,FaceList,ctrace);
				if(ctrace->col)
				{
					*start = ctrace->impact;
					return;
				}
			}
		}
		else
		{
			ctrace->solidspace = true;
			ctrace->col = true;
			ctrace->impact = *start;
			return;
		}

		// collision avec les actionbrush
		if(!noscript.value && !gIsMultiplayer)
		{
			for(i=0 ; i<gNb_ActionBrush ; i++)
			{

				// HACK
		//		if(!ab_IsVisibleAB(*start,i))
		//			continue;

				start2.X = start->X - (ActionBrushTab[i].script.Pos.X);
				start2.Y = start->Y - (ActionBrushTab[i].script.Pos.Y);
				start2.Z = start->Z - (ActionBrushTab[i].script.Pos.Z);

				end2.X = end->X - (ActionBrushTab[i].script.Pos.X);
				end2.Y = end->Y - (ActionBrushTab[i].script.Pos.Y);
				end2.Z = end->Z - (ActionBrushTab[i].script.Pos.Z);
				
				TraceMove(start2,&end2,ActionBrushTab[i].FaceList,ctrace);
				if(ctrace->col)
				{
					ctrace->impact.X += ActionBrushTab[i].script.Pos.X;
					ctrace->impact.Y += ActionBrushTab[i].script.Pos.Y;
					ctrace->impact.Z += ActionBrushTab[i].script.Pos.Z;
					*start = ctrace->impact;
					return;
				}
			}
		}

		*start = *end;
	}
}

#define	CORRECT_VAL		1.1f

void CorrectSide(Face_t *ListFace, Face_t *Face, vect_t *Pos, vect_t *NewPos)
{
	float	result;
	vect_t	Direction;
	Face_t	*pCurr;
	float	Dist;
	vect_t	cut;
	vect_t	PointOnPlane;

	PointOnPlane = Face->Plan.PointOnPlane;

	Direction.X = Pos->X - PointOnPlane.X;
	Direction.Y = Pos->Y - PointOnPlane.Y;
	Direction.Z = Pos->Z - PointOnPlane.Z;

	result = PdtScalaire(&Face->Plan.VecteurNormal,&Direction);
	if(result > 0/*epsilon.value*/)
		return;

	Direction = Face->Plan.VecteurNormal;
	m_Vector_Scale(&Direction,CORRECT_VAL);
	*NewPos = m_Vector_Add(*NewPos,Direction);

	// seulement pour les coins convexes d'angle < 90 degre
	// recursif attention !!

	Direction.X = NewPos->X - Pos->X;
	Direction.Y = NewPos->Y - Pos->Y;
	Direction.Z = NewPos->Z - Pos->Z;

	for(pCurr=ListFace ; pCurr ; pCurr=pCurr->Next)
	{
		if(pCurr->Type == INVISIBLE)
			continue;
		if(PdtScalaire(&Direction,&pCurr->Plan.VecteurNormal) >= 0)
			continue;
		if(pCurr == Face)
			continue;
		if(!TriangleIntersect(*Pos,Direction,&cut,pCurr,&Dist))
			continue;

		CorrectSide(ListFace,pCurr,Pos,NewPos);
		return;
	}
}

void TraceMove(vect_t origine, vect_t *pDest, Face_t *ListFace, CTRACE *ctrace)
{
	pFace	pCurr;
	vect_t	cut, arrivee;
	vect_t	Dir;
	float	Dist;
	vect_t	NewPos;

	ctrace->col = false;
	arrivee = *pDest;

	Dir.X = arrivee.X - origine.X;
	Dir.Y = arrivee.Y - origine.Y;
	Dir.Z = arrivee.Z - origine.Z;

	for(pCurr=ListFace ; pCurr ; pCurr=pCurr->Next)
	{
		if(pCurr->Type == INVISIBLE)
			continue;
		if(PdtScalaire(&Dir,&pCurr->Plan.VecteurNormal) >= 0)
			continue;
		if(!TriangleIntersect(origine,Dir,&cut,pCurr,&Dist))
			continue;

		ctrace->col = true;
		ctrace->Plan = pCurr->Plan;
		ctrace->type = pCurr->Type;

		NewPos = cut;
		CorrectSide(ListFace,pCurr,&cut,&NewPos);
		cut = NewPos;

		ctrace->impact = cut;
		return;
	}
}

hBool TriangleIntersect(vect_t O, vect_t Dir, vect_t *cp, Face_t *face, float *Dist)
{
	vect_t	v;
	float	denom;
	float	t;
	vect_t	PointOnPlane;

	PointOnPlane = face->Plan.PointOnPlane;
	denom = PdtScalaire(&Dir,&face->Plan.VecteurNormal);

	if(IsZero(denom))
		return false;

	v.X = PointOnPlane.X - O.X;
	v.Y = PointOnPlane.Y - O.Y;
	v.Z = PointOnPlane.Z - O.Z;

	t = PdtScalaire(&v,&face->Plan.VecteurNormal)/denom;

	*Dist = t;

	if(t<0-epsilon.value)
		return false;
	if(t>1+epsilon.value)
		return false;
	
//	v = Dir;
//	Normalize(&v);
//	denom = PdtScalaire(&v,&face->Plan.VecteurNormal);
//	t -= (0.05f/denom);

//	if(t<0)
//		t=0;
//	if(t>1)
//		t=1;

//	m_ConsPrint("[%f]\n",t);

	cp->X = O.X + (t*Dir.X);
	cp->Y = O.Y + (t*Dir.Y);
	cp->Z = O.Z + (t*Dir.Z);

//	v = face->Plan.VecteurNormal;
//	m_Vector_Scale(&v,1.5f);
//	*cp = m_Vector_Add(*cp,v);

	return pointInPoly(face,*cp);
	//return pointInTri(face,*cp);
}

hBool pointInPoly(Face_t *Face, vect_t CP)
{
	Face_t	F;
	int		i;

	switch(Face->NumberOfVertices)
	{
		case 3:
		return pointInTri(Face,CP);

		default:
		F = *Face;
		for(i=0 ; i<Face->NumberOfVertices-2 ; i++)
		{
			F.Vertice[1] = Face->Vertice[i+1];
			F.Vertice[2] = Face->Vertice[i+2];
			if( pointInTri(&F,CP) )
				return true;
		}
		return false;
	}
	return false;
}

hBool pointInTri(Face_t *Face, vect_t CP)
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

	v1[0] = Face->Vertice[0].X;
	v1[1] = Face->Vertice[0].Y;
	v1[2] = Face->Vertice[0].Z;

	v2[0] = Face->Vertice[1].X;
	v2[1] = Face->Vertice[1].Y;
	v2[2] = Face->Vertice[1].Z;

	v3[0] = Face->Vertice[2].X;
	v3[1] = Face->Vertice[2].Y;
	v3[2] = Face->Vertice[2].Z;

    n = Face->Plan.VecteurNormal;

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

#define EPSILON_ 0.000001
#define CROSS(dest,v1,v2) \
          dest[0]=v1[1]*v2[2]-v1[2]*v2[1]; \
          dest[1]=v1[2]*v2[0]-v1[0]*v2[2]; \
          dest[2]=v1[0]*v2[1]-v1[1]*v2[0];
#define DOT(v1,v2) (v1[0]*v2[0]+v1[1]*v2[1]+v1[2]*v2[2])
#define SUB(dest,v1,v2) \
          dest[0]=v1[0]-v2[0]; \
          dest[1]=v1[1]-v2[1]; \
          dest[2]=v1[2]-v2[2]; 

int intersect_triangle(double orig[3], double dir[3],
                   double vert0[3], double vert1[3], double vert2[3],
                   double *t, double *u, double *v)
{
   double edge1[3], edge2[3], tvec[3], pvec[3], qvec[3];
   double det,inv_det;

   /* find vectors for two edges sharing vert0 */
   SUB(edge1, vert1, vert0);
   SUB(edge2, vert2, vert0);

   /* begin calculating determinant - also used to calculate U parameter */
   CROSS(pvec, dir, edge2);

   /* if determinant is near zero, ray lies in plane of triangle */
   det = DOT(edge1, pvec);

   if (det > -EPSILON_ && det < EPSILON_)
     return 0;
   inv_det = 1.0 / det;

   /* calculate distance from vert0 to ray origin */
   SUB(tvec, orig, vert0);

   /* calculate U parameter and test bounds */
   *u = DOT(tvec, pvec) * inv_det;
   if (*u < 0.0 || *u > 1.0)
     return 0;

   /* prepare to test V parameter */
   CROSS(qvec, tvec, edge1);

   /* calculate V parameter and test bounds */
   *v = DOT(dir, qvec) * inv_det;
   if (*v < 0.0 || *u + *v > 1.0)
     return 0;

   /* calculate t, ray intersects triangle */
   *t = DOT(edge2, qvec) * inv_det;
   return 1;
}

hBool Intersect_Face(pFace Face, vect_t start, vect_t end)
{
	double t, u, v;
	double orig[3];
	double dir[3];
	double vert0[3];
	double vert1[3];
	double vert2[3];

	orig[0] = start.X;
	orig[1] = start.Y;
	orig[2] = start.Z;

	dir[0] = end.X - start.X;
	dir[1] = end.Y - start.Y;
	dir[2] = end.Z - start.Z;

	vert0[0] = Face->Vertice[0].X;
	vert0[1] = Face->Vertice[0].Y;
	vert0[2] = Face->Vertice[0].Z;

	vert1[0] = Face->Vertice[1].X;
	vert1[1] = Face->Vertice[1].Y;
	vert1[2] = Face->Vertice[1].Z;

	vert2[0] = Face->Vertice[2].X;
	vert2[1] = Face->Vertice[2].Y;
	vert2[2] = Face->Vertice[2].Z;

	if(intersect_triangle(orig,dir,vert0,vert1,vert2,&t,&u,&v))
	{
		if(t<=1.0f)
			return true;
	}

	return false;
}

// === OLD ======



hBool Intersect_Poly(vect_t *start, vect_t *end, Face_t *Face, vect_t *cut, float *dist)
{
	if(!Intersect_Plan(start,end,&Face->Plan,cut,dist))
		return false;
	return IsVertexOnFace(cut,Face);
}

hBool Intersect_Plan(vect_t *start, vect_t *end, Plan_t *Plan, vect_t *cut, float *dist)
{
	vect_t	direction, L1;
	float	linelength,dist_from_plane;

	direction.X = end->X - start->X;
	direction.Y = end->Y - start->Y;
	direction.Z = end->Z - start->Z;

	linelength = PdtScalaire(&direction,&Plan->VecteurNormal);
	if(linelength == 0) 
		return false; 

	L1.X = Plan->PointOnPlane.X - start->X;
	L1.Y = Plan->PointOnPlane.Y - start->Y;
	L1.Z = Plan->PointOnPlane.Z - start->Z;

	dist_from_plane = PdtScalaire(&L1,&Plan->VecteurNormal);

	*dist = dist_from_plane/linelength;

	if(A_absf(*dist) > 1)
		return false;

	cut->X = start->X + (direction.X * (*dist));
	cut->Y = start->Y + (direction.Y * (*dist));
	cut->Z = start->Z + (direction.Z * (*dist));

	return true;
}

hBool IsVertexOnFace(vect_t *V, Face_t *Face)
{
	vect_t	V1, V2, V3;
	int		i, m;

	for(i=0 ; i<Face->NumberOfVertices ; i++)
	{
		m = i+1;
		if(m==Face->NumberOfVertices)
			m=0;

		V1.X =  Face->Vertice[i].X - Face->Vertice[m].X;
		V1.Y =  Face->Vertice[i].Y - Face->Vertice[m].Y;
		V1.Z =  Face->Vertice[i].Z - Face->Vertice[m].Z;

		PdVectoriel(&V2,&V1,&Face->Plan.VecteurNormal);

		V3.X = V->X - Face->Vertice[i].X;
		V3.Y = V->Y - Face->Vertice[i].Y;
		V3.Z = V->Z - Face->Vertice[i].Z;

		if( PdtScalaire(&V2,&V3) < 0 )
			return false;
	}
	return true;
}


