#ifndef BSP2SOLID_INCLUDED
#define	BSP2SOLID_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bsp2math.h"
#include "bsp2list.h"



pFace	solid_MakeGiantBox(pFace FaceList);
int		solid_AddCube(pFace ListFace);
//void	solid_CutBox(int Hull, pFace CutFace, int *ChildF, int *ChildB);
void	solid_WriteBoxes(char *name);
void	solid_GetBoundFromBox(pFace Box, bound_t Bound);
//pFace	solid_AdjustFaceToCube(pFace CubeList, pFace Face);
void	solid_ExpandFace(pFace Face);
//pFace	solid_ClipFace(pFace ClipList, pFace Face);


#endif BSP2SOLID_INCLUDED

