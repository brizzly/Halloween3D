#ifndef BSP2FILE_INCLUDED
#define	BSP2FILE_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bsp2math.h"
#include "bsp2algo.h"
#include "bsp2geo.h"
#include "bsp2.h"


extern	int		Nb_nodes;
extern	noeud_t	NodeArray[MAX_ARRAY];
//extern	Brush_t	BrushTab[MAX_BRUSH];

float	gStartX, gStartY, gStartZ;
int		gPlayerDir;
int		Nb_entities;
ENTITY	Entities2[255];
int		Nb_textures;
int		Nb_Lights;
char	TextureList2[255][255];
LIGHT	Lights[255];


void	Write_BSP(char *name);
bool	Write_solid_BSP(char *name);
bool	Write_MAP(char *name, pFace PortalList, pFace FaceList);
pFace	Load_Polygons(char *name);
bool	Write_Polygons(char *path, pFace RectList);
bool	Write_Portal(char *path, pFace RectList);
void	Write_ActionBrush(char *path);


#endif BSP2FILE_INCLUDED