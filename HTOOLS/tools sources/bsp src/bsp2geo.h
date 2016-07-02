#ifndef BSP2GEO_INCLUDED
#define	BSP2GEO_INCLUDED


#define	MAX_BRUSH	32000
#define	BOGUS_RANGE	18000
#define	MAXEDGES	32
#define	EPS_ENORME	0

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bsp2math.h"
#include "bsp2algo.h"
#include "bsp2.h"
#include "bsp2list.h"


typedef struct brush_tag
{
	bool	Valid;
	int		Nb_Face;
	pFace	ListFace;
	pFace	ClipedFace;

} Brush_t;


int		Nb_Brush;
Brush_t	BrushTab[MAX_BRUSH];

typedef struct actionbrush_tag
{
	bool	Valid;
	int		Nb_Face;
	pFace	ListFace;
	pFace	ClipedFace;

} ABrsh_t;

int		Nb_ActionBrush;
ABrsh_t	ActionBrushTab[MAX_BRUSH];


extern float	gStartX, gStartY, gStartZ;


void	GetWorldBox(pFace FaceList);
pFace	ConstructSolidGeometry(pFace FaceList);
pFace	FindBrush(pFace FaceList);
pFace	FindActionBrush(pFace FaceList);
pFace	SnapWordToGrid(pFace FaceList);
void	AdjustVerxtex(float *vertex, int grid);
pFace	CenterVertices(pFace FaceList);
pFace	GroupMax(pFace PolyList);
bool	IsConvexFace(pFace Face);
bool	EqualVertice(pFace Face);
void	RemoveColinearVertice(pFace Face);
bool	CheckGroupedFace(pFace Face);
bool	CheckFace(pFace Face);
pFace	GroupFaceList(pFace FaceList, bool disp);
pFace	GroupFaces(pFace pTemp, pFace pTemp2);
pFace	RemoveHiddenFaces(pFace FaceList);
bool	IsFacesOverlap(pFace Fa, pFace Fb);
bool	IsRectangle(pFace Face);
bool	IsVertexEgalFace(vect_t v, pFace Face);
bool	IsVertexEgal(vect_t v, vect_t a);
bool	IsVertexOnEdgeFace(vect_t v, pFace Face);
bool	IsVertexOnEdge(vect_t v, vect_t a, vect_t b);
void	RemoveInvalidLeaf(pFace PortalList);
int		RemoveLeafValidity();
int		SetLeafValidity();
void	RemoveLeaf(int leaf);
void	GetBrushNormals();
void	GetVertNormals_2(pFace List);
void	GetVertNormals(pFace List);
pFace	SplitToTriangle(pFace PolyList);
pFace	RemoveWrongFaces(pFace PolyList);


#endif BSP2GEO_INCLUDED