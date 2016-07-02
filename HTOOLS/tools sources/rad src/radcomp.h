#ifndef __RADCOMP___
#define	__RADCOMP___

#define	MAX_LAMPES		512
#define	REFLECTION		0.5f
#define	MIN_ENERGY		60.0f		 //80.0f
#define	MAX_ENERGY		255.0f       //255.0f
#define	MAX_ENERGY_INV	0.003921f
#define	NB_PATCHS		1024
#define	DEBUGFILE		"point.txt"
#define	LM_X			32
#define	LM_Y			32
#define	JOKER			666


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <direct.h>
#include <time.h>

typedef unsigned char byte;
typedef enum { false,true } bool;

int			nPATCH_X;
int			nPATCH_Y;

enum
{
	XY,
	YZ,
	ZX
};

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

} point_t;

typedef struct
{
	vect_t	VecteurNormal;
	vect_t	PointOnPlane;

} plan_t;

typedef struct	tagrect * face_t;
typedef struct	tagrect
{
	int		Type;
	int		NumberOfVertices;
	int		textId;
	int		LightID;
	int		FaceID;
	point_t	point[8];
	plan_t	Plan;
	struct	tagrect	*Next;

} RECTANGLE;

typedef struct patchtag
{
	vect_t	pos;
	float	energyR;
	float	energyG;
	float	energyB;

} PATCH;

typedef struct lmtag * lm_t;
typedef struct lmtag
{
	int		id;
	face_t	face;
	char	proj_axis;
	byte	*surface;
	vect_t	point[4];
	float	reflection;
	PATCH	patch[NB_PATCHS];
	struct	lmtag * next;

} LM;

typedef struct boundtag bound;
typedef struct boundtag
{
	float	Xmin, Ymin, Zmin;
	float	Xmax, Ymax, Zmax;

} BOUND;



typedef struct tag_bbox
{
	vect_t		BoxMin;
	vect_t		BoxMax;

} BOUNDINGBOX;

typedef struct		tagnoeud
{
	int				Feuille;
	int				Front;
	int				Back;
	plan_t			Plan;

} noeud_t;

typedef struct	tagleaf
{
	int				LeafID;
	int				Nb_faces;
	face_t 			faceliste;
	int				*FaceIndexList;
	int				PVSIndex;
	BOUNDINGBOX		bbox;

} leafs_t;

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


lm_t	CreateLightMap(int id);
void	WriteLightMaps(lm_t LmList);
lm_t	AddToList(lm_t List, lm_t Elem);
void	DeleteLightMaps(lm_t LmList);
void	ComputeLightMap(lm_t Lm, LIGHT Light);
void	Quit(char *txt);
void	Rename(char *name, char *extension);
void	WritePtsFile(lm_t LmList);
void	WriteRadFile(lm_t LmList, char *name);
double	FloatTime();
void	DispTimeElapsed(double TimeStart);

#endif __RADCOMP___
