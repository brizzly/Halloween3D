#ifndef BSP2PORTAL_INCLUDED
#define	BSP2PORTAL_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bsp2math.h"
#include "bsp2list.h"

extern	int	Nb_portals;

pFace	CheckPortals(pFace PortalList);
pFace	PortalizeWorld();
pFace	portal_GetFromNode();
pFace	portal_NodeCut(pFace Portal, int node);
pFace	portal_ListCut(pFace Portal, pFace CutList);
pFace	portal_RemoveBad(pFace PortalList);
bool	portal_SendTroughtBSP_rec(pFace Portal, int node);


pFace	RemoveInvalidPortal(pFace PortalList);
pFace	ClipPortal(int Node, pFace Portal);
pFace	ExtrudePortal(int node);
pFace	BuildPortals_rec(pFace PortalList, int Node);
pFace	RemoveDoublePortal(pFace PortalList);
void	MarkNearLeaf(pFace PortalList);


#endif BSP2PORTAL_INCLUDED

