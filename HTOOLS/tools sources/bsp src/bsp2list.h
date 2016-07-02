#ifndef BSP2LIST_INCLUDED
#define	BSP2LIST_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bsp2math.h"
#include "bsp2algo.h"


pFace	AddListes(pFace FaceList, pFace AddList);
pFace	RemoveFaceFromList(pFace racine, pFace remove);
pFace	RemoveList(pFace List);
int		CountLeafyFaces();
int		CountFaces(face_t facelist);
pFace	CopyList(pFace List);
pFace	CopyFace(pFace ModelFace);
face_t	AllocFace(int Nb_Points);
void	FreeFace(face_t f);



#endif BSP2LIST_INCLUDED
