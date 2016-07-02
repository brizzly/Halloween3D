#ifndef	BSP2ALGO_INCLUDED
#define	BSP2ALGO_INCLUDED

#define	EMPTY_SPACE	-1
#define	SOLID_SPACE	-2


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bsp2math.h"
#include "bsp2.h"
#include "bsp2geo.h"
#include "bsp2file.h"
#include "bsp2list.h"
#include "bsp2portal.h"
#include "bsp2solid.h"


int		Nb_nodes;
int		Nb_leafs;
int		Nb_portals;
noeud_t	NodeArray[MAX_ARRAY];
leafs_t	LeafArray[MAX_ARRAY];
faces_t	FaceArray[MAX_ARRAY];
Cube_t	CubeArray[MAX_ARRAY];

BOUNDINGBOX	WorldBox;

//int		FaceIdArray[MAX_ARRAY][10000];

extern	int		Nb_Brush;
extern	int		Nb_ActionBrush;
extern	int		Nb_Cube;
extern	int		menu_visibility;
//extern	Brush_t	BrushTab[MAX_BRUSH];


bool	ProcessFile(char *name);
void	SetLeafNbFace(pFace FaceList);
pFace	SetIdFaces(pFace FaceList);
void	PrintIdFaces(pFace FaceList);
pFace	RetrieveOriginalFaces(pFace FaceList);
pFace	RemoveActionBrushFaces(pFace FaceList);
void	SetLeafFaceId(pFace FaceList);
void	Clear_BSP();
void	Compute_BSP(pFace ListFace);
void	Compute_solid_BSP(pFace ListFace);
void	rec_Compute_BSP(pFace FaceList);
void	rec_Sentinel_solid_BSP(int Node);
void	rec_Compute_solid_BSP(int Node);
pFace	ClipFacesToBSP(pFace FaceList);
pFace	ClipFaceToBSP(int node, pFace Face);
pFace	FindBestSplitter(pFace pList);
pFace	FindBestSplitter2(pFace pList);
pFace	GetFacesFromBSP();
pFace	GetFacesFromSolidBSP();
pFace	RemoveFragments(pFace FaceList);
pFace	RemovePortalFragments(pFace FaceList);
bool	IsAllCoplanar(pFace ListFace);
void	SplitLeaves();
void	GroupLeaves();
void	RemoveInvisibleLeafFaces();
void	FindInaccessibleLeaf();
int		FindLeaf_r(vect_t pos, int node);
void	MarkLeaf_r(int leaf);
pFace	RemoveInvalidCellAndPortals(pFace PortalList);


#endif	BSP2ALGO_INCLUDED