
#include <math.h>
#include "radcomp.h"
#include "maths.h"


float m_Distance(vect_t M, vect_t P)
{
	vect_t	DistanceVect;

	DistanceVect.X = P.X - M.X;
	DistanceVect.Y = P.Y - M.Y;
	DistanceVect.Z = P.Z - M.Z;
	
	return Norme(&DistanceVect);
//	return SquareNorme(&DistanceVect);
}

float SquareNorme(const vect_t *pVect)
{
 	return (pVect->X*pVect->X + pVect->Y*pVect->Y + pVect->Z*pVect->Z);
}

float Norme(const vect_t *pVect)
{
 	return (float) sqrt(pVect->X*pVect->X + pVect->Y*pVect->Y + pVect->Z*pVect->Z);
}

void Normalize(vect_t *pVect)
{
    float length, len;
  

	length=Norme(pVect);
	if(length==0)
		len = 0;
	else
		len=1.0f/length;
  
	pVect->X *= len; 
	pVect->Y *= len; 
	pVect->Z *= len;
}

float PdtScalaire(const vect_t *pVect1, const vect_t *pVect2)
{
	return (pVect1->X*pVect2->X + pVect1->Y*pVect2->Y + pVect1->Z*pVect2->Z);
}

void PdVectoriel(vect_t *Vres, vect_t *v1, vect_t *v2)
{
	Vres->X = (v1->Y * v2->Z) - (v1->Z * v2->Y);
	Vres->Y = (v1->Z * v2->X) - (v1->X * v2->Z);
	Vres->Z = (v1->X * v2->Y) - (v1->Y * v2->X);
}

vect_t GetFaceNormal(point_t Vec0, point_t Vec1, point_t Vec2)
{
	vect_t	p, q, n;

	p.X = Vec1.X - Vec0.X;
	p.Y = Vec1.Y - Vec0.Y;
	p.Z = Vec1.Z - Vec0.Z;
		
	q.X = Vec2.X - Vec0.X;  
	q.Y = Vec2.Y - Vec0.Y;  
	q.Z = Vec2.Z - Vec0.Z;  
		
	PdVectoriel(&n,&p,&q);
	Normalize(&n);

	return n;
}

float DistancePlan(const plan_t *pPlan, const point_t *pPoint)
{
	vect_t	Direction;

	Direction.X = pPlan->PointOnPlane.X - pPoint->X;
	Direction.Y = pPlan->PointOnPlane.Y - pPoint->Y;
	Direction.Z = pPlan->PointOnPlane.Z - pPoint->Z;

	return PdtScalaire(&pPlan->VecteurNormal, &Direction);
}

bool Intersect_Poly(vect_t *orig, vect_t *dest, face_t Poly, vect_t *cut, float *t)
{
	if(Poly->NumberOfVertices < 3)
		return false;

	switch(Poly->NumberOfVertices)
	{
		case 3:
			return Intersect_triangle(orig,dest,&Poly->point[0],&Poly->point[1],&Poly->point[2],cut,t);
		break;

		default:	// 4 or + HACK HACK HACK
			if( Intersect_triangle(orig,dest,&Poly->point[0],&Poly->point[1],&Poly->point[2],cut,t) ||
				Intersect_triangle(orig,dest,&Poly->point[0],&Poly->point[2],&Poly->point[3],cut,t) )
				return true;
			else
				return false;
		break;
	}
	return false;
}

bool Intersect_triangle(vect_t *orig, vect_t *dest, point_t *vert0, point_t *vert1, point_t *vert2, vect_t *cut, float *t)
{
	vect_t	dir;
	vect_t	edge1;
	vect_t	edge2;
	vect_t	tvec;
	vect_t	pvec;
	vect_t	qvec;
	float	det, u, v;


	dir.X = dest->X - orig->X;
	dir.Y = dest->Y - orig->Y;
	dir.Z = dest->Z - orig->Z;

	edge1.X = vert1->X - vert0->X;
	edge1.Y = vert1->Y - vert0->Y;
	edge1.Z = vert1->Z - vert0->Z;

	edge2.X = vert2->X - vert0->X;
	edge2.Y = vert2->Y - vert0->Y;
	edge2.Z = vert2->Z - vert0->Z;

	PdVectoriel(&pvec,&dir,&edge2);
	det=PdtScalaire(&edge1,&pvec);

	if(det < EPSILON)
		return false;

	tvec.X = orig->X - vert0->X;
	tvec.Y = orig->Y - vert0->Y;
	tvec.Z = orig->Z - vert0->Z;

	u = PdtScalaire(&tvec,&pvec);
	if (u < 0 || u > det)
		return false;

	PdVectoriel(&qvec,&tvec,&edge1);

	v = PdtScalaire(&dir,&qvec);
	if(v < 0 || u + v > det)
		return false;

	*t = PdtScalaire(&edge2,&qvec)/det;

	cut->X = orig->X + dir.X*(*t);
	cut->Y = orig->Y + dir.Y*(*t);
	cut->Z = orig->Z + dir.Z*(*t);

	return true;
}

int FindPojectionPlane(vect_t normal)
{
	if(A_absf(normal.X) > A_absf(normal.Y))
	{
		if(A_absf(normal.X) > A_absf(normal.Z))
			return YZ;
		else
			return XY;
	}
	else
	{
		if(A_absf(normal.Z) > A_absf(normal.Y))
			return XY;
		else
			return ZX;
	}
}

int FindPojectionPlane2(lm_t Lm)
{
	if(Lm->proj_axis == 'X')
		return YZ;
	else if(Lm->proj_axis == 'Y')
		return ZX;
	else return XY;
}

float A_absf(float value)
{
	if(value<0)
		return -value;
	else
		return value;
}

void arrondi(float *val)
{
	if(A_absf(*val) < 0.001f)
		*val = 0;
}

float GetXYZplane(plan_t plan, float X1, float X2, int T)
{
	float	X, Y, Z;
	vect_t	normal;
	float	Cst;

	normal.X = plan.VecteurNormal.X;
	normal.Y = plan.VecteurNormal.Y;
	normal.Z = plan.VecteurNormal.Z;
	Cst = Getconstante(plan);

	switch(T)
	{
		case 0:
		if(normal.X==0)
			return JOKER;
		X = -(normal.Y*X1 + normal.Z*X2 + Cst)/normal.X;
		return X;

		case 1:
		if(normal.Y==0)
			return JOKER;
		Y = -(normal.X*X1 + normal.Z*X2 + Cst)/normal.Y;
		return Y;

		case 2:
		if(normal.Z==0)
			return JOKER;
		Z = -(normal.X*X1 + normal.Y*X2 + Cst)/normal.Z;
		return Z;
	}
	return JOKER;
}

float Getconstante(plan_t plan)
{
	return -(plan.VecteurNormal.X*plan.PointOnPlane.X + plan.VecteurNormal.Y*plan.PointOnPlane.Y + plan.VecteurNormal.Z*plan.PointOnPlane.Z);
}

void ClearBound(BOUNDINGBOX *bbox)
{
	bbox->BoxMin.X = MAX_BOUND;
	bbox->BoxMin.Y = MAX_BOUND;
	bbox->BoxMin.Z = MAX_BOUND;

	bbox->BoxMax.X = MIN_BOUND;
	bbox->BoxMax.Y = MIN_BOUND;
	bbox->BoxMax.Z = MIN_BOUND;
}

void GetBound(face_t face, BOUNDINGBOX *Bound)
{
	int	i;

	Bound->BoxMin.X = Bound->BoxMax.X = face->point[0].X;
	Bound->BoxMin.Y = Bound->BoxMax.Y = face->point[0].Y;
	Bound->BoxMin.Z = Bound->BoxMax.Z = face->point[0].Z;

	for(i=0 ; i<face->NumberOfVertices ; i++)
	{
		if(face->point[i].X < Bound->BoxMin.X)
			Bound->BoxMin.X = face->point[i].X;
		if(face->point[i].X > Bound->BoxMax.X)
			Bound->BoxMax.X = face->point[i].X;

		if(face->point[i].Y < Bound->BoxMin.Y)
			Bound->BoxMin.Y = face->point[i].Y;
		if(face->point[i].Y > Bound->BoxMax.Y)
			Bound->BoxMax.Y = face->point[i].Y;

		if(face->point[i].Z < Bound->BoxMin.Z)
			Bound->BoxMin.Z = face->point[i].Z;
		if(face->point[i].Z > Bound->BoxMax.Z)
			Bound->BoxMax.Z = face->point[i].Z;
	}
}

bool IsVertexOnFace(vect_t V, face_t face)
{
	RECTANGLE	F;
	int		i;

	switch(face->NumberOfVertices)
	{
		case 3:
		return pointInTri(face,V);

		default:
		F = *face;
		for(i=0 ; i<face->NumberOfVertices-2 ; i++)
		{
			F.point[1] = face->point[i+1];
			F.point[2] = face->point[i+2];
			if( pointInTri(&F,V) )
				return true;
		}
		return false;
	}
	return false;
}

bool pointInTri(face_t Face, vect_t CP)
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

	v1[0] = Face->point[0].X;
	v1[1] = Face->point[0].Y;
	v1[2] = Face->point[0].Z;

	v2[0] = Face->point[1].X;
	v2[1] = Face->point[1].Y;
	v2[2] = Face->point[1].Z;

	v3[0] = Face->point[2].X;
	v3[1] = Face->point[2].Y;
	v3[2] = Face->point[2].Z;

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

bool Intersect_Plan(vect_t start, vect_t end, plan_t *Plan, vect_t *cut, float *dist, bool culling)
{
	vect_t	direction,L1;
	float	linelength,dist_from_plane;
	vect_t	*vertex;
	vect_t	*normal;

	vertex = &(Plan->PointOnPlane);
	normal = &(Plan->VecteurNormal);

	direction.X = end.X - start.X;
	direction.Y = end.Y - start.Y;
	direction.Z = end.Z - start.Z;

	linelength=PdtScalaire(&direction,normal);
	if(A_absf(linelength)<EPSILON) 
		return false; 

	L1.X = vertex->X - start.X;
	L1.Y = vertex->Y - start.Y;
	L1.Z = vertex->Z - start.Z;

	dist_from_plane = PdtScalaire(&L1,normal);
	*dist = dist_from_plane/linelength;

	if(!culling)
	{
		if(A_absf(*dist)>1.0f)
			return false;
	}
	else
	{
		// HACK: rajout des egalites dans les inequations
		if(*dist <= 0.10f /*0.40f*/) // 0.12f ... 0.25..
			return false;
		else if(*dist>1/*0.95f*/) // >= 1
			return false;
//		if(*dist<=0.15f)
//			return false;
//		else if(*dist>=0.9f)
//			return false;

		//if(*dist > 0.85f)
		//	*dist = 0.85f;
	}

	cut->X = start.X + direction.X*(*dist);
	cut->Y = start.Y + direction.Y*(*dist);
	cut->Z = start.Z + direction.Z*(*dist);

	return true;
}

bool IsLightIntersect(vect_t *start, vect_t *end, face_t face)
{
	vect_t	cut;
	float	dist;

	if(!Intersect_Plan(*start,*end,&face->Plan,&cut,&dist,true)) // culling on
		return false;

	if(IsVertexOnFace(cut,face))
	{
		//*start = cut;	
		return true;
	}
	
	return false;
}

bool IsEqualf(float A, float B)
{
	if( A_absf(A-B) < 0.0001f)
		return true;
	return false;
}

void m_VectScale(vect_t *v, float scale)
{
	v->X *= scale;
	v->Y *= scale;
	v->Z *= scale;
}