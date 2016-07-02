
#include "bsp2portal.h"


pFace CheckPortals(pFace PortalList)
{
	pFace	Portal;
	
	printf("CheckPortals\n");

	for(Portal=PortalList ; Portal ; Portal=Portal->Next)
		Portal->Used = false;

	for(Portal=PortalList ; Portal ; Portal=Portal->Next)
	{
		//printf("%d ",Portal->NodeID);
		if(portal_SendTroughtBSP_rec(Portal,Portal->NodeID) == false)
		{
			printf("*");
			Portal->Used = true;
		}
	}

	Portal=PortalList;
	while(Portal)
	{
		if(Portal->Used)
		{
			PortalList = RemoveFaceFromList(PortalList,Portal);
			Portal = PortalList;
			continue;
		}
		Portal=Portal->Next;
	}
	Nb_portals = CountFaces(PortalList);

	return PortalList;
}

pFace PortalizeWorld(pFace FaceList)
{
	pFace	PortalList;

	printf("-- PortalizeWorld --\n");

	PortalList = NULL;
	PortalList = BuildPortals_rec(PortalList,0);
	PortalList = RemoveDoublePortal(PortalList);
	MarkNearLeaf(PortalList);
	Nb_portals = CountFaces(PortalList);


	return PortalList;
}

pFace BuildPortals_rec(pFace PortalList, int Node)
{
	pFace	Portal;
	pFace	ClipList;


//	if(PortalList->BrushID < 0)
//		return BuildPortals_rec(PortalList->Next,Node);

	if(Node == EMPTY_SPACE || Node == SOLID_SPACE)
		return PortalList;
	if(Node > Nb_nodes-1)
		return PortalList;
	if(NodeArray[Node].Feuille >= 0)
		return PortalList;

	Portal = ExtrudePortal(Node);
	ClipList = ClipPortal(0,Portal);
	ClipList = RemoveInvalidPortal(ClipList);
	PortalList = AddListes(PortalList,ClipList);

	//	HACK for fastest generation
	if(Node % 5 == 0)
		PortalList = RemoveDoublePortal(PortalList);

	if(NodeArray[Node].Front != EMPTY_SPACE)
		PortalList = BuildPortals_rec(PortalList,NodeArray[Node].Front);
	if(NodeArray[Node].Back != SOLID_SPACE)
		PortalList = BuildPortals_rec(PortalList,NodeArray[Node].Back);

	return PortalList;
}

pFace ClipPortal(int Node, pFace Portal)
{
	pFace	PortalList=NULL;
	pFace	tempnext=NULL;
	pFace	FrontPortalList=NULL;
	pFace	BackPortalList=NULL;
	pFace	FrontSplit=NULL;
	pFace	BackSplit=NULL;
	int		i;

	if(Portal == NULL)
		Error("ClipPortal NULL portal");
	if(Node == EMPTY_SPACE || Node == SOLID_SPACE)
	{
		//Error("ClipPortal Node error #1");
		FreeFace(Portal);
		return NULL;
	}
	if(Node >= Nb_nodes)
		Error("ClipPortal Node error #2");

	switch(ClassifyPoly(&NodeArray[Node].Plan,Portal))
	{
		case CONFONDU:
		if(NodeArray[Node].Feuille >= 0)
		{
			Portal->LeafOwnerArray[Portal->NumberOfLeaf]=NodeArray[Node].Feuille;
			Portal->NumberOfLeaf++;
			Portal->Next = NULL;
			FrontPortalList = Portal;
		}
		else
		{
			FrontPortalList = ClipPortal(NodeArray[Node].Front,Portal);
		}

		if(FrontPortalList==NULL)
			return NULL;
		if(NodeArray[Node].Back == SOLID_SPACE)
			return FrontPortalList;
		
		PortalList=NULL;
		while(FrontPortalList)
		{ 
			tempnext = FrontPortalList->Next;

			BackPortalList = ClipPortal(NodeArray[Node].Back,FrontPortalList);
			PortalList = AddListes(PortalList,BackPortalList);

			FrontPortalList = tempnext;
		}
		return PortalList;

		case DEVANT:
		if(NodeArray[Node].Feuille >= 0)
		{
			Portal->LeafOwnerArray[Portal->NumberOfLeaf]=NodeArray[Node].Feuille;
			Portal->NumberOfLeaf++;
			Portal->Next = NULL;
			return Portal;
		}
		return ClipPortal(NodeArray[Node].Front,Portal);

		case DERRIERE:
		if(NodeArray[Node].Back != SOLID_SPACE)
			return ClipPortal(NodeArray[Node].Back,Portal);
		FreeFace(Portal);
		return NULL;

		case AUMILIEU:
		FrontSplit = AllocFace(0);
		BackSplit = AllocFace(0);
		Spliting(Portal,&NodeArray[Node].Plan,FrontSplit,BackSplit,NULL);
		if(FrontSplit)
			FrontSplit->NumberOfLeaf = Portal->NumberOfLeaf;
		if(BackSplit)
			BackSplit->NumberOfLeaf	= Portal->NumberOfLeaf;
		for(i=0 ; i<Portal->NumberOfLeaf ; i++)
		{
			if(FrontSplit)
				FrontSplit->LeafOwnerArray[i] = Portal->LeafOwnerArray[i];
			if(BackSplit)
				BackSplit->LeafOwnerArray[i] = Portal->LeafOwnerArray[i];
		}
		FreeFace(Portal);

		if(NodeArray[Node].Feuille >= 0)
		{
			FrontSplit->LeafOwnerArray[FrontSplit->NumberOfLeaf]=NodeArray[Node].Feuille;
			FrontSplit->NumberOfLeaf++;
			FrontSplit->Next = NULL;
			FrontPortalList = FrontSplit;
		}
		else
		{
			FrontPortalList = ClipPortal(NodeArray[Node].Front,FrontSplit);
		}

		if(NodeArray[Node].Back != SOLID_SPACE)
			BackPortalList = ClipPortal(NodeArray[Node].Back,BackSplit);
		else
		{
			FreeFace(BackSplit);
			BackSplit = NULL;
			BackPortalList = NULL;
		}
		return AddListes(FrontPortalList,BackPortalList);
	}
	FreeFace(Portal);
	return NULL;
}

pFace ExtrudePortal(int node)
{
	pFace	Portal;
	vect_t	U,V,P[4];
	int		i;
	vect_t	MaxP, MinP, CB, CP, PlaneNormal, BoxHalfLength, Dir, A;
	float	DistanceToPlane, Length;

	if(NodeArray[node].Feuille != -1)
		printf("\7");

//	if(IsValidBound(&NodeArray[node].bbox) == false)
//		Error("invalid bound\n");

	MaxP = NodeArray[node].bbox.BoxMax;
	MinP = NodeArray[node].bbox.BoxMin;

	CB.X = (MaxP.X + MinP.X)/2.0f;
	CB.Y = (MaxP.Y + MinP.Y)/2.0f;
	CB.Z = (MaxP.Z + MinP.Z)/2.0f;

	Dir.X = NodeArray[node].Plan.PointOnPlane.X - CB.X;
	Dir.Y = NodeArray[node].Plan.PointOnPlane.Y - CB.Y;
	Dir.Z = NodeArray[node].Plan.PointOnPlane.Z - CB.Z;

	PlaneNormal =	NodeArray[node].Plan.VecteurNormal;
	DistanceToPlane = PdtScalaire(Dir,PlaneNormal);

	CP.X = CB.X + (PlaneNormal.X*DistanceToPlane);
	CP.Y = CB.Y + (PlaneNormal.Y*DistanceToPlane);
	CP.Z = CB.Z + (PlaneNormal.Z*DistanceToPlane);

	A.X = A.Y = A.Z = 0;
	if(Absf(PlaneNormal.Y) > Absf(PlaneNormal.Z)) 
	{
		if(Absf(PlaneNormal.Z) < Absf(PlaneNormal.X))
			A.Z = 1;
		else 
			A.X = 1;
	}
	else 
	{
		if(Absf(PlaneNormal.Y) <= Absf(PlaneNormal.X)) 
			A.Y = 1;
		else 
			A.X = 1;
	}

	U = PdVectoriel(A,PlaneNormal);
	Normalize(&U);

	V = PdVectoriel(U,PlaneNormal);
	Normalize(&V);

	BoxHalfLength.X = MaxP.X - CB.X;
	BoxHalfLength.Y = MaxP.Y - CB.Y;
	BoxHalfLength.Z = MaxP.Z - CB.Z;
	Length = Norme(BoxHalfLength);

	Length = Length * 100.0f;

	U.X*=Length;
	U.Y*=Length;
	U.Z*=Length;
	
	V.X*=Length;
	V.Y*=Length;
	V.Z*=Length;

	P[0].X = CP.X + U.X - V.X;
	P[0].Y = CP.Y + U.Y - V.Y;
	P[0].Z = CP.Z + U.Z - V.Z;

	P[1].X = CP.X + U.X + V.X;
	P[1].Y = CP.Y + U.Y + V.Y;
	P[1].Z = CP.Z + U.Z + V.Z;

	P[2].X = CP.X - U.X + V.X;
	P[2].Y = CP.Y - U.Y + V.Y;
	P[2].Z = CP.Z - U.Z + V.Z;

	P[3].X = CP.X - U.X - V.X;
	P[3].Y = CP.Y - U.Y - V.Y;
	P[3].Z = CP.Z - U.Z - V.Z;

	Portal = AllocFace(4);

	for(i=0 ; i<4 ; i++)
	{
		Portal->point[i].X = P[i].X;
		Portal->point[i].Y = P[i].Y;
		Portal->point[i].Z = P[i].Z;
		Portal->point[i].U = 0;
		Portal->point[i].V = 0;
	}
	Portal->Next=NULL;
	Portal->NodeID = node;
	Portal->NumberOfLeaf=0;
	Portal->LeafOwnerArray[0] = 0;
	Portal->LeafOwnerArray[1] = 0;
	Portal->LeafOwnerArray[2] = 0;
	Portal->LeafOwnerArray[3] = 0;

	Portal->Plan.VecteurNormal = PlaneNormal;
	Portal->Plan.PointOnPlane.X = NodeArray[node].Plan.PointOnPlane.X;
	Portal->Plan.PointOnPlane.Y = NodeArray[node].Plan.PointOnPlane.Y;
	Portal->Plan.PointOnPlane.Z = NodeArray[node].Plan.PointOnPlane.Z;

//	SizeFaceToBound(Portal,NodeArray[node].bbox);
//	SizeFaceToBound(Portal,WorldBox);

	return Portal;
}

pFace RemoveInvalidPortal(pFace PortalList)
{
	pFace	Portal;
	int		i;

	for(Portal=PortalList ; Portal ; Portal=Portal->Next)
		Portal->Used = false;

	for(Portal=PortalList ; Portal ; Portal=Portal->Next)
	{
		if(Portal->Used)
			continue;
		if(Portal->NumberOfLeaf != 2)
		{
			Portal->Used = true;
			continue;
		}
		if(LeafArray[Portal->LeafOwnerArray[0]].valid == false)
		{
			//printf("X");
			Portal->Used = true;
			continue;
		}
		if(LeafArray[Portal->LeafOwnerArray[1]].valid == false)
		{
			//printf("Y");
			Portal->Used = true;
			continue;
		}
		/*
		for(i=0 ; i<Portal->NumberOfVertices ; i++)
		{
			if( (Absf(Portal->point[i].X) > (float)MAX_WORLD) ||
				(Absf(Portal->point[i].Y) > (float)MAX_WORLD) ||
				(Absf(Portal->point[i].Z) > (float)MAX_WORLD) )
				Portal->Used = true;
		}
		*/
	}

	Portal=PortalList;
	while(Portal)
	{
		if(Portal->Used)
		{
			PortalList = RemoveFaceFromList(PortalList,Portal);
			Portal = PortalList;
			continue;
		}
		Portal=Portal->Next;
	}

	return PortalList;
}

pFace RemoveDoublePortal(pFace PortalList)
{
	pFace	pCurr;
	pFace	Portal;
	pFace	pCurr2;
	pFace	Portal2;
	vect_t	v1,v2;
	float	NewSize;
	float	OldSize;
	BOUNDINGBOX	box1, box2;

	pCurr = PortalList;
	while(pCurr)
	{
		Portal = pCurr;
		pCurr = pCurr->Next;

		if(Portal->Used)
			continue;

		pCurr2 = PortalList;
		while(pCurr2)
		{
			Portal2 = pCurr2;
			pCurr2 = pCurr2->Next;
			
			if(Portal2->Used)
				continue;
			if(Portal == Portal2)
				continue;

			if(	((Portal->LeafOwnerArray[0] == Portal2->LeafOwnerArray[0]) &&
				 (Portal->LeafOwnerArray[1] == Portal2->LeafOwnerArray[1]))	||
				((Portal->LeafOwnerArray[0] == Portal2->LeafOwnerArray[1]) &&
				 (Portal->LeafOwnerArray[1] == Portal2->LeafOwnerArray[0])) )
			{
				ClearBound(&box1);
				ClearBound(&box2);
				GetBound(Portal,&box1);
				GetBound(Portal2,&box2);

				if(IsValidBound(&box1) == false)
					Portal->Used = true;
				if(IsValidBound(&box2) == false)
					Portal2->Used = true;

				v1.X = box1.BoxMax.X - box1.BoxMin.X;
				v1.Y = box1.BoxMax.Y - box1.BoxMin.Y;
				v1.Z = box1.BoxMax.Z - box1.BoxMin.Z;

				v2.X = box2.BoxMax.X - box2.BoxMin.X;
				v2.Y = box2.BoxMax.Y - box2.BoxMin.Y;
				v2.Z = box2.BoxMax.Z - box2.BoxMin.Z;

				NewSize = Norme(v1);
				OldSize = Norme(v2);

				if(NewSize > OldSize)
					Portal2->Used = true;
				else if(NewSize < OldSize)
					Portal->Used = true;
			}
		}
	}

	Portal = PortalList;
	while(Portal)
	{
		if(Portal->Used)
		{
			PortalList = RemoveFaceFromList(PortalList,Portal);
			Portal = PortalList;
			continue;
		}
		Portal = Portal->Next;
	}

	return PortalList;
}


bool portal_SendTroughtBSP_rec(pFace Portal, int node)
{
	plan_t	Plan;
	int		FrontNode;
	int		BackNode;
	bool	res1, res2;

	if(node == EMPTY_SPACE)
		Error("EMPTY_SPACE error");
	else if(node == SOLID_SPACE)
		Error("SOLID_SPACE error");

	if(NodeArray[node].Feuille >= 0)
	{
	//	Portal->LeafOwnerArray[Portal->NumberOfLeaf++] = NodeArray[node].Feuille;
		//printf("L");
		return true;
	}

	FrontNode = NodeArray[node].Front;
	BackNode = NodeArray[node].Back;
	Plan = NodeArray[node].Plan;

	//printf("node(%d)",node);

	switch(ClassifyPoly(&Plan,Portal))
	{
		case CONFONDU:
			//printf("C");
			if(FrontNode == EMPTY_SPACE)
				return true; // false
			else
				res1 = portal_SendTroughtBSP_rec(Portal,FrontNode);

			if(BackNode == SOLID_SPACE)
				return false;
			else
				res2 = portal_SendTroughtBSP_rec(Portal,BackNode);
		
			return (res1 && res2);
		//	return (res1 || res2);
		break;

		case DEVANT:
			//printf("F");
			if(FrontNode == EMPTY_SPACE)
				return true; // false
			return portal_SendTroughtBSP_rec(Portal,FrontNode);
		break;

		case DERRIERE:
			//printf("B");
			if(BackNode == SOLID_SPACE)
				return false;
			return portal_SendTroughtBSP_rec(Portal,BackNode);
		break;

		case AUMILIEU:
			//printf("M");
			//printf("(%d)",node);
			//Error("ClassifyPortal error");
			printf("M");
			//return false;
			return true;
		break;
	}
	
	Error("ClassifyPortal #1 error");
	return true;
}

void MarkNearLeaf(pFace PortalList)
{
	pFace	Portal;
	int		lo1, lo2;
	int		i;

	printf("MarkNearLeaf\n");

	for(Portal=PortalList ; Portal ; Portal=Portal->Next)
	{
		lo1 = Portal->LeafOwnerArray[0];
		lo2 = Portal->LeafOwnerArray[1];

		for(i=0 ; i<LeafArray[lo1].NumberOfNearLeaf ; i++)
		{
			if(LeafArray[lo1].NearLeaf[i] ==  lo2)
				break;
		}
		if(i == LeafArray[lo1].NumberOfNearLeaf)
		{
			LeafArray[lo1].NearLeaf[LeafArray[lo1].NumberOfNearLeaf] = lo2;
			if(LeafArray[lo1].NumberOfNearLeaf >= 500)
				Error("#01");
			LeafArray[lo1].NumberOfNearLeaf++;
		}

		for(i=0 ; i<LeafArray[lo2].NumberOfNearLeaf ; i++)
		{
			if(LeafArray[lo2].NearLeaf[i] ==  lo1)
				break;
		}
		if(i == LeafArray[lo2].NumberOfNearLeaf)
		{
			LeafArray[lo2].NearLeaf[LeafArray[lo2].NumberOfNearLeaf] = lo1;
			if(LeafArray[lo2].NumberOfNearLeaf >= 500)
				Error("#02");
			LeafArray[lo2].NumberOfNearLeaf++;
		}
	}
}