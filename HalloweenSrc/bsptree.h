
#ifndef BSP_INCLUDED
#define BSP_INCLUDED

#include "alloween.h"
#include "camera.h"

int		bsp_FindCamera(vect_t Pos);
int		bsp_FindCamera_rec(int Node, vect_t Pos);
void	bsp_DrawMap(Camera_t *pCam);
int		bsp_DrawPVS(int leaf, const Camera_t *pHero);
void	bsp_GetNL(NL_t *pNL, int leaf, float radius, vect_t pos);
void	bsp_GetNL_rec(int recdepth, NL_t *pNL, int centerLeaf, vect_t pos, float radius);
vect_t	bsp_CorrectSide_rec(int Node, CTRACE *trace, vect_t Pos, vect_t Dest);
vect_t	bsp_CorrectSide(vect_t Pos, vect_t Dest, CTRACE	ctrace);
int		bsp_DrawPORTALS(int mode);
void	bsp_DrawLeaves(Face_t *FaceList, vect_t pos);
void	bsp_DrawLeaves_2(int id_face, vect_t pos);
void	bsp_DrawTabFaces();

#endif // BSP_INCLUDED
