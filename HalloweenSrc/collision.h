
#ifndef COLLISION_INCLUDED
#define COLLISION_INCLUDED


#include "alloween.h"

typedef struct cliptrace_tag
{
	// inputs
	hBool	Response;
	vect_t	ellipsoid;
	hBool	DontUnstuck;
	hBool	NoRadius;
	pEntity Ent;
	
	//outputs
	hBool	col;
	vect_t	impact;
	hBool	solidspace;
	float	dist;
	int		type;
	int		node;
	plan_t	Plan;
	hBool	IsPlan;


} CTRACE;


void	HMove(vect_t *start, vect_t *end, CTRACE *ctrace);
void	CorrectSide(Face_t *ListFace, Face_t *Face, vect_t *Pos, vect_t *NewPos);
void	TraceMove(vect_t origine, vect_t *pDest, Face_t *ListFace, CTRACE *ctrace);
hBool	TriangleIntersect(vect_t O, vect_t Dir, vect_t *cp, Face_t *face, float *Dist);
hBool	pointInTri(Face_t *Face, vect_t CP);
hBool	pointInPoly(Face_t *Face, vect_t CP);

hBool	IsVertexOnFace(vect_t *V, Face_t *Face);
hBool	Intersect_Plan(vect_t *start, vect_t *end, Plan_t *Plan, vect_t *cut, float *dist);
hBool	Intersect_Poly(vect_t *start, vect_t *end, Face_t *Face, vect_t *cut, float *dist);

hBool	Intersect_Face(pFace Face, vect_t start, vect_t end);


#endif // COLLISION_INCLUDED
