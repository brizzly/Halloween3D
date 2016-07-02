#ifndef __ASEMATH__
#define	__ASEMATH__


#define	MAX_BOUND			4096


float	PdtScalaire(vect_t pVect1, vect_t pVect2);
float	Norme(vect_t *pVect);
bool	Normalize(vect_t *pVect);
vect_t	PdVectoriel(vect_t Vec1, vect_t Vec2);
vect_t	GetFaceNormal(point_t Vec0, point_t Vec1, point_t Vec2);
float	A_absf(float value);
bool	IsEqualf(float A, float B);
int		ClassifyVect(vect_t *pos, plan_t *Plan);
int		ClassifyPoint(point_t *pos, plan_t * Plan);
int		ClassifyPoly(plan_t *Plan, RECTANGLE *Poly);
bool	Get_Intersect(vect_t *linestart,vect_t *lineend,plan_t plan,vect_t * intersection, float *percentage);
void	ClearBound(BOUNDINGBOX *bbox);
void	CalculateBox(BOUNDINGBOX *Box, pFace Polylist);


#endif __ASEMATH__
