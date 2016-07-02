#ifndef BSP2MATH_INCLUDED
#define	BSP2MATH_INCLUDED

#define	EPSILON				0.001f
#define	EQUAL_EPSILON		0.001f
#define	CONFONDU			0
#define	DEVANT				1
#define	DERRIERE			2
#define	AUMILIEU			3

#define	MAX_WORLD			4096

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum { false, true } bool;


typedef struct
{
	float	X;
	float	Y;
	float	Z;

} vect_t;

typedef struct
{
	float	X;
	float	Y;
	float	Z;
	float	U;
	float	V;
	float	U_lm;
	float	V_lm;
	float	NX;
	float	NY;
	float	NZ;
	bool	Used_Opacity;
	bool	Used;
	bool	GroupUsed;

} point_t;

typedef struct
{
	vect_t	VecteurNormal;
	vect_t	PointOnPlane;

} plan_t;

typedef struct tagrect *portal_t;
typedef struct tagrect *face_t;
typedef struct tagrect *pFace;
typedef struct	tagrect
{
	int		IsDetailObjet;
	int		ActionNumber;
	bool	Used;
	bool	Cloned;
	int		CloneID;
	struct	tagrect	*CloneFace;
	bool	BSP_used;
	int		NumberOfVertices;
	int		NumberOfLeaf;
	int		LeafOwnerArray[4];
	int		textId;
	int		lightID;
	int		BrushID;
	int		NodeID;
	int		LeafID;
	int		FaceID;
	point_t	*point;
	plan_t	Plan;
	struct	tagrect	*OriginalFace;
	struct	tagrect	*NonFragFace;
	struct	tagrect	*Next;

} RECTANGLE;

typedef struct BB *bound_t;
typedef struct BB
{
	vect_t	BoxMin;
	vect_t	BoxMax;

} BOUNDINGBOX;

typedef struct		tagnoeud
{
	int				Feuille;
	plan_t			Plan;
	BOUNDINGBOX		bbox;
	pFace			faces;
	int				nb_portal;
	pFace			portals;
	int				Front;
	int				Back;

} noeud_t;

typedef struct		tagleafs
{
	bool			PVS_calculated;
	bool			valid;
	bool			visited;
	int				LeafID;
	int				HullID;
	int				Nb_faces;
	int				*FaceIds;
	int				NumberOfPortals;
	int				NumberOfNearLeaf;
	int				NearLeaf[500];
	int				PVSIndex;
	BOUNDINGBOX		bbox;

} leafs_t;

typedef struct		tagfaces
{
	int				FaceID;
	RECTANGLE 		*faceliste;

} faces_t;

typedef struct CubeTag
{
	pFace	facelist;

} Cube_t;

typedef struct ENTITYTAG
{
	vect_t	pos;
	int		id;
	int		angleY;

} ENTITY;

typedef struct LIGHTTAG
{
	vect_t	pos;
	int		R;
	int		G;
	int		B;
	float	I;
	int		type;
	
} LIGHT;

enum
{
	WALL,
	DETAIL,
	BSPLESS,
	LAVA,
	DOOR,
	INVISIBLE,
	ACTION,
	MOREINVISIBLE
};

enum
{
	MAP_POL,
	MAP_RADPOL
};


float	Norme(vect_t pVect);
bool	Normalize(vect_t *pVect);
float	PdtScalaire(vect_t pVect1, vect_t pVect2);
vect_t	PdVectoriel(vect_t Vec1, vect_t Vec2);
vect_t	GetFaceNormal(point_t Vec0, point_t Vec1, point_t Vec2);
int		ClassifyVect(vect_t *pos, plan_t *Plan);
int		ClassifyPoint(point_t *pos, plan_t * Plan);
int		ClassifyPoly(plan_t *Plan, RECTANGLE *Poly);
bool	Get_Intersect(vect_t *linestart,vect_t *lineend,plan_t plan,vect_t * intersection, float *percentage);
float	Absf(float value);
bool	IsEqualf(float A, float B);
bool	IsVectInRect(pFace Face, vect_t CP);
bool	IsPointInTri(vect_t point[3], vect_t Normal, vect_t CP);
int		NombreDePointsEnCommunEntreDeuxPoly(pFace f1, pFace f2);
bool	IsConvexPolytope(pFace Poly);
bool	IsConvexPoly(pFace f1, pFace f2);
void	Spliting(pFace Poly, plan_t *Plan, pFace FrontSplit, pFace BackSplit, pFace Cuts);
void	GetBound(face_t face, BOUNDINGBOX *Bound);
bool	IsFlatBound(BOUNDINGBOX Bound);
void	ClearBound(BOUNDINGBOX *bbox);
bool	IsValidBound(BOUNDINGBOX *bbox);
void	SizeFaceToBound(pFace Face, BOUNDINGBOX bbox);
bool	IsAxialFace(pFace Face);


#endif BSP2MATH_INCLUDED
