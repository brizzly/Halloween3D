
#ifndef BSPFILE_INCLUDED
#define BSPFILE_INCLUDED

#include "constantes.h"
#include "alloween.h"


typedef struct tag_NL
{
	int		NbNL;
	int		*NLData;

} NL_t;

typedef struct tag_Point_t
{
	float X;
	float Y;
	float Z;
	float U;
	float V;
	float U_lm;
	float V_lm;
	float NX;
	float NY;
	float NZ;
	int marker;

} Point_t;

typedef struct tag_vect
{
	float X;
	float Y;
	float Z;

} vect_t;

typedef struct tag_Plan_t	plan_t;
typedef struct tag_Plan_t
{
	vect_t		VecteurNormal;  
	vect_t		PointOnPlane;

} Plan_t;

typedef struct tag_Face_t *pFace;
typedef struct tag_Face_t
{
	Point_t 	Vertice[8];
	int		NumberOfVertices;
	Plan_t		Plan;
	int		FACEID;
	int		TextureID;
	int		LightID;
	int		Type;
	int		switched;
	int		blend_src;
	int		blend_dst;
	int		actionbrush_id;
	struct tag_Face_t	*Next;

} Face_t;

typedef struct tag_bsp_t
{
	Plan_t		Plan;
	int			NodeId;
	int			Front;
	int			Back;
	Face_t 		*faceliste;

} Bsp_t;

typedef struct LT
{
	Point_t		*Vertice;
	int			*Indices;
	Plan_t		Plan;
	int			NumberOfVertices;
	int			NumberOfIndices;
	int			LeafOwnerArray[2];
	struct LT	*Next;

} portal_t;

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
	Plan_t			Plan;

} noeud_t;

typedef struct	tagleaf
{
	int				LeafID;
	int				Nb_faces;
	Face_t 			*faceliste;
	int				*FaceIndexList;
	int				Nb_portals;
	int				portalIndex[MAX_PINDEX_PR_LEAF];
	int				PVSIndex;
	BOUNDINGBOX		bbox;

} leafs_t;

typedef struct facetabtag *pTabFace;
typedef struct facetabtag
{
	pFace	FacePtr;
	int		id;
	int		sorted_id;
	
} Facetab_t;


enum
{
	WALL,
	DETAIL,
	BSPLESS,
	LAVA,
	DOOR,
	INVISIBLE,
	ACTIONBRUSH,
	MOREINVISIBLE,
	FLAMES=666
};


hBool	LoadMap();
void	UnloadMap();

void	bsp_InitStartPosition(vect_t pos, int AngleView);
void	bsp_LoadStartPosition(FILE *file);
int		bsp_LoadEntity(FILE *file);
void	bsp_LoadLightmaps(char *name, FILE *file);
void	bsp_LoadTextures(FILE *file, int *texture_id_plan);
void	bsp_LoadLights(FILE *file);
hBool	bsp_Load_MAPFILE(char *name);
void	bsp_Unload_MAPFILE();
hBool	bsp_Load_POLYFILE(char *path);


#endif // BSPFILE_INCLUDED

