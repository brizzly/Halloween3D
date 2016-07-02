
#define		OPTIMIZE_PVS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

typedef enum { false, true } bool;
typedef	unsigned char byte;

#define	MAXRECURSION		7999
#define	MAXTIMEPERPVS		16.0
#define	MAX_ARRAY			50000
#define	EPSILON				0.01f		//0.0001f
#define	CONFONDU			0
#define	DEVANT				1
#define	DERRIERE			2
#define	AUMILIEU			3
#define	EQUAL_EPSILON		0.001f

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
} point_t;

typedef struct
{
	vect_t	VecteurNormal;
	vect_t	PointOnPlane;
} plan_t;

typedef struct	tagrect *pFace;
typedef struct	tagrect
{
	point_t	*point;
	int		NumberOfVertices;
	int		textId;
	plan_t	Plan;
	int		NumberOfLeaf;
	int		LeafOwnerArray[4];
	struct	tagrect	*Next;
} RECTANGLE;

typedef struct BB
{
	vect_t		BoxMin;
	vect_t		BoxMax;
} BOUNDINGBOX;

typedef struct		tagleafs
{
	bool			PVS_calculated;
	int				SortedID;
	int				Nb_faces;
	int				NumberOfPortals;
	int				PortalIndexList[500];
	int				NumberOfNearLeaf;
	int				NearLeafIndexList[500];
	int				PVSIndex;
	BOUNDINGBOX		bbox;
	vect_t			origin;
} leafs_t;

typedef struct CLIPPLANESTAG
{
	int		NumberOfPlanes;
	plan_t	*Planes;
} CLIPPLANES;

typedef struct ENTITYTAG
{
	vect_t	pos;
	int		id;
	int		angleY;

} ENTITY;


ENTITY		Entities[255];
leafs_t		LeafArray[MAX_ARRAY];
RECTANGLE	*PortalArray[MAX_ARRAY];
RECTANGLE	*NodeArray[MAX_ARRAY];
int			Nb_entities;
int			Nb_textures;
int			Nb_Lights;
int			Nb_portals;
int			Nb_leafs;
int			Nb_faces;
int			Nb_nodes;
double		TimeStart;
double		PVS_TimeStart, PVS_Time;
bool		PVS_ok;
char		*PVSData;
char		name[255];
char		tempname[255];
int			Recursivitee;
int			PVS_pass;

double		FloatTime();
void		DispTimeElapsed(double TimeStart);
void		Spliting(RECTANGLE *Poly, plan_t *Plan, RECTANGLE *FrontSplit, RECTANGLE *BackSplit);
void		SplitPortal(RECTANGLE *Portal, plan_t *Plan, RECTANGLE *FrontSplit, RECTANGLE *BackSplit);
void		DeletePortal(RECTANGLE *Portal);
bool		LoadBSPFromFile(char *name);
void		Quit(char *QuitMessage);
void		PrintMemory();
void		Rename(char *name, char *extension);
float		Norme(vect_t pVect);
bool		Normalize(vect_t *pVect);
float		PdtScalaire(vect_t pVect1, vect_t pVect2);
vect_t		PdVectoriel(vect_t Vec1, vect_t Vec2);
vect_t		GetFaceNormal(point_t Vec0, point_t Vec1, point_t Vec2);
int			A_abs(int value);
float		A_absf(float value);
bool		IsEqualf(float A, float B);
void		MergePVSinfos();
void		SetPVSBit(char *VisArray, int DestLeaf);
void		RecursePVS(int SourceLeaf, RECTANGLE *SrcPortal, RECTANGLE *TargetPortal,int TargetLeaf, char *LeafPVS);
RECTANGLE * ClipToAntiPenumbra(RECTANGLE *SourcePortal, RECTANGLE *TargetPortal, RECTANGLE *GeneratorPortal);
int			ClassifyVect(vect_t *pos, plan_t *Plan);
int			ClassifyPoly(plan_t *Plan, RECTANGLE *Poly);


