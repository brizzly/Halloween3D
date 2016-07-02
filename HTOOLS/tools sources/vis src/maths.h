
float Norme(vect_t pVect)
{
	float	carre;

	carre = PdtScalaire(pVect,pVect);
	return (float)sqrt((double)carre);
}

bool Normalize(vect_t *pVect)
{
	float length, len;
  
	length=Norme(*pVect);
	if(length==0)
	{
	//	printf("normalize error\n");
	//	pVect->X = 0;
	//	pVect->Y = 0;
	//	pVect->Z = 0;
		return false;
	}
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
	if(A_absf(linelength)<EPSILON) 
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

int A_abs(int value)
{
	if(value<0)
		return -value;
	else
		return value;
}

float A_absf(float value)
{
	if(value<0)
		return -value;
	else
		return value;
}

bool IsEqualf(float A, float B)
{
	if(A_absf(A-B) <= EQUAL_EPSILON)
		return true;
	else
		return false;
}

