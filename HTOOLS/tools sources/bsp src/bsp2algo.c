
#include "bsp2algo.h"


bool ProcessFile(char *name)
{
	FILE	*file;
	pFace	FaceList;
	pFace	PortalList = NULL;
	int		Nb_faces=0;

	file=fopen(name,"r");
	if(!file)
	{
		sprintf(name,"%s%s",name,".pts");
		file=fopen(name,"r");
		if(!file)
			return false;
	}
	fclose(file);

	FaceList = Load_Polygons(name);

	if(!menu_visibility)
	{
		/*
		FaceList = GroupFaceList(FaceList,true);		
		FaceList = SnapWordToGrid(FaceList);
		FaceList = RemoveWrongFaces(FaceList);
		FaceList = CenterVertices(FaceList);
		FaceList = ConstructSolidGeometry(FaceList);
		Compute_solid_BSP(FaceList);
		PortalList = PortalizeWorld();
		FindInaccessibleLeaf();
		FaceList = GetFacesFromSolidBSP();
		Write_Polygons(name,FaceList);
		Nb_faces = CountFaces(FaceList);
		*/

		FaceList = RemoveActionBrushFaces(FaceList);
		FaceList = GroupMax(FaceList);		
		FaceList = SnapWordToGrid(FaceList);
		FaceList = RemoveWrongFaces(FaceList);
		FaceList = CenterVertices(FaceList);
		FaceList = SplitToTriangle(FaceList);
		Write_Polygons(name,FaceList);
		Nb_faces = CountFaces(FaceList);
	}
	else
	{
		//FaceList = RemoveActionBrushFaces(FaceList); //

		FaceList = GroupFaceList(FaceList,true);		
		FaceList = SnapWordToGrid(FaceList);
		FaceList = RemoveWrongFaces(FaceList);
		FaceList = CenterVertices(FaceList);
		FaceList = ConstructSolidGeometry(FaceList);

		Compute_solid_BSP(FaceList);
		SplitLeaves();
		PortalList = PortalizeWorld();

		FindInaccessibleLeaf();
		PortalList = RemoveInvalidCellAndPortals(PortalList);
		FaceList = GetFacesFromSolidBSP();

		Nb_portals = CountFaces(PortalList);
		Nb_faces = CountFaces(FaceList);

		FaceList = SetIdFaces(FaceList);
		SetLeafFaceId(FaceList);

		Write_MAP(name,PortalList,FaceList);
		Rename(name,"pvs");
		remove(name);
	}

	printf("=================\n");
	printf("%5d BRUSHS\n",Nb_Brush);
	printf("%5d FACES\n",Nb_faces);
	if(menu_visibility)
	{
		printf("%5d NODES\n",Nb_nodes);
		printf("%5d LEAVES\n",Nb_leafs);
		printf("%5d PORTALS\n",Nb_portals);
		printf("%5d ACTIONBRUSH\n",Nb_ActionBrush);
	}
	printf("=================\n");

	file=fopen("output.txt","w");
	fprintf(file,"%5d BRUSHS\n",Nb_Brush);
	fprintf(file,"%5d FACES\n",Nb_faces);
	if(menu_visibility)
	{
		fprintf(file,"%5d NODES\n",Nb_nodes);
		fprintf(file,"%5d LEAVES\n",Nb_leafs);
		fprintf(file,"%5d PORTALS\n",Nb_portals);
		fprintf(file,"%5d ACTIONBRUSH\n",Nb_ActionBrush);
	}
	fclose(file);

	return true;
}

void SetLeafNbFace(pFace FaceList)
{
	int		i;
	pFace	Face;

	for(i=0 ; i<Nb_leafs ; i++)
		LeafArray[i].Nb_faces = 0;

	for(Face=FaceList ; Face ; Face=Face->Next)
	{
		LeafArray[Face->LeafID].Nb_faces++;
	}
}

pFace SetIdFaces(pFace FaceList)
{
	pFace	Face;
	int		id;
	int		Nb_faces = CountFaces(FaceList);

	id = 0;
	for(Face=FaceList ; Face ; Face=Face->Next)
//	if(!Face->Cloned)
		Face->FaceID = id++;
/*
	for(Face=FaceList ; Face ; Face=Face->Next)
	if(Face->Cloned)
	{
		if(Face->CloneFace)
		{
			if(Face->CloneFace->FaceID > Nb_faces)
				Face->CloneFace->FaceID = 0;
			else if(Face->CloneFace->FaceID < 0)
				Face->CloneFace->FaceID = 0;
			printf("%d ",Face->CloneFace->FaceID);
			Face->FaceID = Face->CloneFace->FaceID;
		}
		else
			printf("X ");
	}
*/
	return FaceList;
}

void PrintIdFaces(pFace FaceList)
{
	pFace	Face;

	for(Face=FaceList ; Face ; Face=Face->Next)
//	if(Face->Next)
//		printf("%d ",Face->Next->FaceID-Face->FaceID);
	printf("%d ",Face->FaceID);
	printf("\n");
}

void SetLeafFaceId(pFace FaceList)
{
	pFace	Face;
	int		i;
	int		nbface;
	int		leafid;

	for(i=0 ; i<Nb_leafs ; i++)
	{
		LeafArray[i].Nb_faces = 0;
	}

	for(Face=FaceList ; Face ; Face=Face->Next)
	{
		leafid = Face->LeafID;
		nbface = LeafArray[leafid].Nb_faces;
		LeafArray[leafid].FaceIds[nbface] = Face->FaceID;
		LeafArray[leafid].Nb_faces++;
	}
}

pFace RetrieveOriginalFaces(pFace FaceList)
{
	pFace	Face;
	pFace	pTemp;
	pFace	pSave;
	pFace	pNew;
	pFace	NewFaceList;
	int		doublon;
	int		regen;

	printf("RetrievingOriginalFaces");

	NewFaceList = NULL;
	for(Face=FaceList ; Face ; )
	{
		pTemp = Face;
		Face=Face->Next;

		if(pTemp->OriginalFace == NULL)
			Error("NULL error #1");
		if(pTemp->OriginalFace == pTemp)
		{
			pSave = CopyFace(pTemp);
			pSave->Next = NewFaceList;
			NewFaceList = pSave;

			FaceList = RemoveFaceFromList(FaceList, pTemp);
			Face = FaceList;
		}
	}

	return NewFaceList;

	doublon = 0;
	regen = 0;

	for(Face=FaceList ; Face ; Face=Face->Next)
		Face->Cloned = false;
		
	for(Face=FaceList ; Face ; Face=Face->Next)
	{
		if(Face->Cloned)
			continue;
		for(pTemp=FaceList ; pTemp ; pTemp=pTemp->Next)
		{
			if(Face==pTemp)
				continue;
			if(pTemp->Cloned)
				continue;
			if(Face->OriginalFace == pTemp->OriginalFace)
			{
				pTemp->Cloned = true;
				pTemp->CloneID = Face->FaceID;
			}
		}
	}

	Face=FaceList;
	while(Face)
	{
		if(Face->Cloned)
		{
			FaceList = RemoveFaceFromList(FaceList,Face);
			Face = FaceList;
		}
		Face=Face->Next;
	}

	for(Face=FaceList ; Face ; )
	{
		pTemp = Face;
		Face=Face->Next;

		if(pTemp->OriginalFace != pTemp)
		{
			//printf(".");
			regen++;
			pSave = pTemp;

			pNew = CopyFace(pTemp->OriginalFace);
			pNew->OriginalFace = NULL;
			pNew->Next = NewFaceList;
			NewFaceList = pNew;

			FaceList = RemoveFaceFromList(FaceList,pTemp);
		}
	}

	printf("_%d_%d\n",doublon,regen);
	return NewFaceList;
}

pFace RemoveActionBrushFaces(pFace FaceList)
{
	pFace	Face;

	printf("RemoveActionBrushFaces\n");

	for(Face=FaceList ; Face ; Face=Face->Next)
		Face->Used = false;
		
	for(Face=FaceList ; Face ; Face=Face->Next)
	{
		if(Face->Used)
			continue;
		if(Face->IsDetailObjet == ACTION)
			Face->Used = true;
	}
	Face=FaceList;
	while(Face)
	{
		if(Face->Used)
		{
			FaceList = RemoveFaceFromList(FaceList,Face);
			Face = FaceList;
			continue;
		}
		Face=Face->Next;
	}
	return FaceList;
}

bool IsAllCoplanar(pFace ListFace)
{
	pFace	Face;
	pFace	FirstFace;
	int		NbFaces;

	NbFaces = CountFaces(ListFace);
	if(NbFaces < 2)
		return true;

	FirstFace = ListFace;
	for(Face=FirstFace->Next ; Face ; Face=Face->Next)
	if( ClassifyPoly(&FirstFace->Plan,Face) != CONFONDU )
		return false;

	return true;
}

void SplitLeaves()
{
	int		i;

	printf("SplitLeaves\n");
	for(i=0 ; i<Nb_leafs ; i++)
	{
		FaceArray[i].faceliste = SplitToTriangle(FaceArray[i].faceliste);
		LeafArray[i].Nb_faces = CountFaces(FaceArray[i].faceliste);
	}
}

void GroupLeaves()
{
	int		i;

	for(i=0 ; i<Nb_leafs ; i++)
	{
		FaceArray[i].faceliste = GroupFaceList(FaceArray[i].faceliste,false);
		LeafArray[i].Nb_faces = CountFaces(FaceArray[i].faceliste);
	}
}

void RemoveInvisibleLeafFaces()
{
	int		i;
	pFace	Face;

	printf("RemoveInvisibleLeafFaces");

label_2:
	for(i=0 ; i<Nb_leafs ; i++)
	{
label:
		for(Face=FaceArray[i].faceliste ; Face ; Face=Face->Next)
		{
			if(Face->IsDetailObjet == INVISIBLE || Face->IsDetailObjet == MOREINVISIBLE || Face->BrushID == -1)
			{
				FaceArray[i].faceliste = RemoveFaceFromList(FaceArray[i].faceliste,Face);
				LeafArray[i].Nb_faces--;
				if(LeafArray[i].Nb_faces == 0)
				{
					RemoveLeaf(i);
					goto label_2;
				}
				else
					goto label;
			}
		}
	}
	printf("\n");
}

void Clear_BSP()
{
	int		i;

	for(i=0 ; i<Nb_nodes ; i++)
	{
		if(NodeArray[i].faces)
		{
			FreeFace(NodeArray[i].faces);
			NodeArray[i].faces = NULL;
		}
	}
	Nb_nodes = 0;
}

void Compute_BSP(pFace ListFace)
{
	pFace	NewList;

	if(!ListFace)
		return;

	NewList = CopyList(ListFace);
	Nb_nodes=0;
	rec_Compute_BSP(NewList);
}

void Compute_solid_BSP(pFace ListFace)
{
	pFace	Face;

	printf("-- SolidBSP --\n");

	for(Face=ListFace ; Face ; Face=Face->Next)
	if(Face->IsDetailObjet == DETAIL ||
		Face->IsDetailObjet == INVISIBLE ||
		Face->IsDetailObjet == MOREINVISIBLE) // ||
//		Face->IsDetailObjet == BSPLESS)
		Face->BSP_used = true;
	else
		Face->BSP_used = false;

	Nb_leafs=0;
	Nb_nodes=0;
	NodeArray[0].faces = ListFace;
	rec_Compute_solid_BSP(0);
	rec_Sentinel_solid_BSP(0);
}

void rec_Compute_BSP(pFace FaceList)
{
	int		node;
	pFace	Face;
	pFace	pTemp;
	pFace	pBest;
	pFace	pRect1;
	pFace	pRect2;
	pFace	ListFaceFront=NULL;
	pFace	ListFaceBack=NULL;

	pBest = FindBestSplitter(FaceList);
	if(!pBest)
		return;

	node = Nb_nodes;
	Nb_nodes++;

	if(Nb_nodes>=MAX_ARRAY)
		Error("MAX_ARRAY Node overflow");

	NodeArray[node].Front = EMPTY_SPACE;
	NodeArray[node].Back = SOLID_SPACE;
	NodeArray[node].Plan = pBest->Plan;
	NodeArray[node].faces = pBest;

	for(Face=FaceList ; Face ; )
	{
		pTemp=Face;
		Face=Face->Next;

		if(pTemp == pBest)
			continue;

		switch(ClassifyPoly(&pBest->Plan,pTemp))
		{
			case CONFONDU:
			if(PdtScalaire(pTemp->Plan.VecteurNormal, pBest->Plan.VecteurNormal) > 0)
			{
				pTemp->Next = ListFaceFront;
				ListFaceFront = pTemp;
			}
			else
			{
				pTemp->Next = ListFaceBack;
				ListFaceBack = pTemp;
			}
			break;

			case DEVANT:
			pTemp->Next = ListFaceFront;
			ListFaceFront = pTemp;
			break;

			case DERRIERE:
			pTemp->Next = ListFaceBack;
			ListFaceBack = pTemp;
			break;

			case AUMILIEU:
			pRect1 = AllocFace(0);
			pRect2 = AllocFace(0);
		//	pRect1->NonFragFace = pTemp;
		//	pRect2->NonFragFace = pTemp;

		//	pRect1->OriginalFace = pTemp; //
		//	pRect2->OriginalFace = pTemp; //

			Spliting(pTemp,&pBest->Plan,pRect1,pRect2,NULL);
			if(pRect1)
			{
				pRect1->Next = ListFaceFront;
				ListFaceFront = pRect1;
			}
			if(pRect2)
			{
				pRect2->Next = ListFaceBack;
				ListFaceBack = pRect2;
			}
			break;
		}
	}

	if(ListFaceFront)
	{
		NodeArray[node].Front = Nb_nodes;
		rec_Compute_BSP(ListFaceFront);
	}

	if(ListFaceBack)
	{
		NodeArray[node].Back = Nb_nodes;
		rec_Compute_BSP(ListFaceBack);
	}
}

void rec_Sentinel_solid_BSP(int Node)
{
	if(NodeArray[Node].Front == Nb_nodes)
		NodeArray[Node].Front = EMPTY_SPACE;
	if(NodeArray[Node].Back == Nb_nodes)
		NodeArray[Node].Back = SOLID_SPACE;

	if(NodeArray[Node].Front)
		rec_Sentinel_solid_BSP(NodeArray[Node].Front);
	if(NodeArray[Node].Back)
		rec_Sentinel_solid_BSP(NodeArray[Node].Back);
}

void rec_Compute_solid_BSP(int Node)
{
	pFace	Face;
	pFace	pTemp;
	pFace	pBest;
	pFace	ListFaceFront=NULL;
	pFace	ListFaceBack=NULL;
	pFace	pRect1;
	pFace	pRect2;

	if(Node>=MAX_ARRAY)
		Error("MAX_ARRAY Node overflow\n");

	NodeArray[Node].Feuille = -1;
	NodeArray[Node].Front = EMPTY_SPACE;
	NodeArray[Node].Back = SOLID_SPACE;
	NodeArray[Node].nb_portal = 0;
	NodeArray[Node].portals = NULL;

	ClearBound(&NodeArray[Node].bbox);
	GetBound(NodeArray[Node].faces,&NodeArray[Node].bbox);

	if(IsFlatBound(NodeArray[Node].bbox))
	{
		//printf("f");
		//RemoveList(NodeArray[Node].faces);
		return;
	}

	pBest = FindBestSplitter2(NodeArray[Node].faces);
	if(!pBest)
	{
		NodeArray[Node].Feuille = Nb_leafs;
		LeafArray[Nb_leafs].valid = true;
		LeafArray[Nb_leafs].NumberOfNearLeaf = 0;
		LeafArray[Nb_leafs].LeafID = Nb_leafs;	// ??
		LeafArray[Nb_leafs].Nb_faces = CountFaces(NodeArray[Node].faces);
		LeafArray[Nb_leafs].bbox = NodeArray[Node].bbox;
		FaceArray[Nb_leafs].FaceID = Nb_leafs;
		FaceArray[Nb_leafs].faceliste = NodeArray[Node].faces;
		NodeArray[Node].faces = NULL;

		for(Face=FaceArray[Nb_leafs].faceliste ; Face ; Face=Face->Next)
			Face->LeafID = Nb_leafs;

		if(!LeafArray[Nb_leafs].FaceIds)
			LeafArray[Nb_leafs].FaceIds = malloc(sizeof(int)*20000);

		Nb_leafs++;
		if(Nb_leafs>=MAX_ARRAY)
			Error("MAX_ARRAY leaf overflow\n");
		return;
	}

	NodeArray[Node].Plan = pBest->Plan;
	NodeArray[Node].portals = ExtrudePortal(Node);
	NodeArray[Node].nb_portal = 1;

	Face = NodeArray[Node].faces;
	NodeArray[Node].faces = NULL;

	while(Face)
	{
		pTemp=Face;
		Face=Face->Next;

		switch(ClassifyPoly(&NodeArray[Node].Plan,pTemp))
		{
			case CONFONDU:
			if( PdtScalaire(pTemp->Plan.VecteurNormal, NodeArray[Node].Plan.VecteurNormal) > 0 )
			{
				pTemp->Next = ListFaceFront;
				ListFaceFront = pTemp;
			}
			else
			{
				pTemp->Next = ListFaceBack;
				ListFaceBack = pTemp;
			}
			break;

			case DEVANT:
			pTemp->Next = ListFaceFront;
			ListFaceFront = pTemp;
			break;

			case DERRIERE:
			pTemp->Next = ListFaceBack;
			ListFaceBack = pTemp;
			break;

			case AUMILIEU:
			pRect1 = AllocFace(0);
			pRect2 = AllocFace(0);

			pRect1->NonFragFace = pTemp;
			pRect2->NonFragFace = pTemp;
			pRect1->LeafID = pTemp->LeafID;
			pRect2->LeafID = pTemp->LeafID;
			
			pRect1->textId = pTemp->textId;
			pRect2->textId = pTemp->textId;
			pRect1->lightID = pTemp->lightID;
			pRect2->lightID = pTemp->lightID;
			pRect1->BSP_used = pTemp->BSP_used;
			pRect2->BSP_used = pTemp->BSP_used;
			pRect1->IsDetailObjet = pTemp->IsDetailObjet;
			pRect2->IsDetailObjet = pTemp->IsDetailObjet;
			pRect1->BrushID = pTemp->BrushID;
			pRect2->BrushID = pTemp->BrushID;
			pRect1->Plan = pTemp->Plan;
			pRect2->Plan = pTemp->Plan;

			Spliting(pTemp,&NodeArray[Node].Plan,pRect1,pRect2,NULL);

			if(pRect1)
			{
				pRect1->Next = ListFaceFront;
				ListFaceFront = pRect1;
			}
			if(pRect2)
			{
				pRect2->Next = ListFaceBack;
				ListFaceBack = pRect2;
			}

			break;
		}
	}

	if(ListFaceFront)
	{
		NodeArray[Node].Feuille = -1;
		Nb_nodes++;
		NodeArray[Node].Front = Nb_nodes;
		NodeArray[Nb_nodes].faces = ListFaceFront;
		rec_Compute_solid_BSP(Nb_nodes);
	}
	if(ListFaceBack)
	{
	//	if(FindBestSplitter2(ListFaceBack) == NULL)
	//	{
	//		printf("*");
	//		ListFaceBack = RemoveList(ListFaceBack);
	//		//NodeArray[Node].Back = SOLID_SPACE;
	//	}
	//	else
	//	{
			Nb_nodes++;
			NodeArray[Node].Back = Nb_nodes;
			NodeArray[Nb_nodes].faces = ListFaceBack;
			rec_Compute_solid_BSP(Nb_nodes);
	//	}
	}
}

pFace ClipFacesToBSP(pFace FaceList)
{
	pFace	Face;
	pFace	pCurr;
	pFace	NewFaceList;
	pFace	NewBrushList=NULL;

	for(pCurr=FaceList ; pCurr ; pCurr=pCurr->Next)
	{
		Face = CopyFace(pCurr);

		NewFaceList = ClipFaceToBSP(0,Face);
		NewBrushList = AddListes(NewBrushList,NewFaceList);
	}
	return NewBrushList;
}

pFace ClipFaceToBSP(int node, pFace Face)
{
	plan_t	Plan;
	pFace	pRect1;
	pFace	pRect2;

	//return Face;

	if(node>=MAX_ARRAY)
		Error("MAX_ARRAY Node overflow");

	if(node == EMPTY_SPACE)
		return Face;
	if(node == SOLID_SPACE)
	{
		if(Face->OriginalFace != Face)
			FreeFace(Face);
		return NULL;
	}

	if(Face->Next)
		printf("!");

	Plan = NodeArray[node].Plan;

	switch(ClassifyPoly(&Plan,Face))
	{
		case DEVANT:
		return ClipFaceToBSP(NodeArray[node].Front,Face);

		case DERRIERE:
		return ClipFaceToBSP(NodeArray[node].Back,Face);

		case CONFONDU:
		if(PdtScalaire(Face->Plan.VecteurNormal, Plan.VecteurNormal) > 0) // >
			return ClipFaceToBSP(NodeArray[node].Front,Face);
		else
			return ClipFaceToBSP(NodeArray[node].Back,Face);

		case AUMILIEU:
		pRect1 = AllocFace(0);
		pRect2 = AllocFace(0);
		pRect1->NonFragFace = Face;
		pRect2->NonFragFace = Face;

		pRect1->OriginalFace = Face;
		pRect2->OriginalFace = Face;

		Spliting(Face,&Plan,pRect1,pRect2,NULL);
		if(pRect1)
			pRect1 = ClipFaceToBSP(NodeArray[node].Front,pRect1);
		if(pRect2)
			pRect2 = ClipFaceToBSP(NodeArray[node].Back,pRect2);

		if(pRect1 == NULL && pRect2 == NULL)
			return NULL;
		if(pRect1 && pRect2)
			return AddListes(pRect1,pRect2);
		if(pRect1)
			return pRect1;
		return pRect2;
	}
	return Face;
}

pFace FindBestSplitter(pFace pList)
{
	pFace	pRunningFace=NULL, pRunningFace2=NULL;
	pFace	pBest=NULL;
	int		Score;
	int		BestScore;
	int		Front, Back, Cop, Split;

	if(!pList)
  		return NULL;

	BestScore=-99999;

	for(pRunningFace=pList; pRunningFace; pRunningFace=pRunningFace->Next)
	{
		Front=0;
		Back=0;
		Cop=0;
		Split=0;

		for(pRunningFace2=pList; pRunningFace2; pRunningFace2=pRunningFace2->Next)
		{			
			if(pRunningFace2 == pRunningFace)
				continue;

			switch(ClassifyPoly(&pRunningFace->Plan,pRunningFace2))
			{
				case DEVANT:
					Front++;
					break;

				case DERRIERE:
					Back++;
					break;

				case CONFONDU:
					Cop++;
					break;

				case AUMILIEU:
					Split++;
					break;
			}
		}
	//	Score = 0*Cop - 5*Split;
	//	Score = (Front+Back)-2*Split;
	//	Score = -5*Split;
		Score = -Split;
		if(Score > BestScore)
		{
			BestScore = Score;
			pBest = pRunningFace;
		}
	}

	if(BestScore == -99999)
		return NULL;

	return pBest;
}

pFace FindBestSplitter2(pFace pList)
{
	pFace	pRunningFace=NULL, pRunningFace2=NULL;
	pFace	pBest=NULL;
	int		Score;
	int		BestScore;
	int		Front, Back, Cop, Split, Axial;

	if(!pList)
  		return NULL;

	BestScore=-99999;

	for(pRunningFace=pList; pRunningFace; pRunningFace=pRunningFace->Next)
	{
		Front=0;
		Back=0;
		Cop=0;
		Split=0;
		Axial=0;

		if(pRunningFace->BSP_used)
			continue;

		for(pRunningFace2=pList; pRunningFace2; pRunningFace2=pRunningFace2->Next)
		{			
			if(pRunningFace2 == pRunningFace)
				continue;
			if(pRunningFace2->BSP_used)
				continue;

			switch(ClassifyPoly(&pRunningFace->Plan,pRunningFace2))
			{
				case DEVANT:
					Front++;
					break;

				case DERRIERE:
					Back++;
					break;

				case CONFONDU:
					Cop++;
					break;

				case AUMILIEU:
					Split++;
					break;
			}

			if( IsAxialFace(pRunningFace) )
				Axial++;
		}
//		Score = -Split;
//		Score = -2*Split + Axial;
		Score = -3*Split + Axial;
		if(Score > BestScore)
		{
			BestScore = Score;
			pBest = pRunningFace;
		}
	}

	if(BestScore == -99999)
		return NULL;
	if(pBest)
		pBest->BSP_used = true;

	return pBest;
}

pFace GetFacesFromBSP()
{
	int		i;
	pFace	pCurr;
	pFace	FaceList=NULL;

	for(i=0 ; i<Nb_nodes ; i++)
	{
		pCurr = NodeArray[i].faces;
		pCurr->Next = FaceList;
		FaceList = pCurr;
	}
	return FaceList;
}

pFace GetFacesFromSolidBSP()
{
	int		i;
	pFace	FaceList=NULL;

	for(i=0 ; i<Nb_leafs ; i++)
	{
		//if(LeafArray[i].valid)
		//if(LeafArray[i].visited == true)
		FaceList = AddListes(FaceList,FaceArray[i].faceliste);
	}
	return FaceList;
}

pFace RemoveFragments(pFace FaceList)
{
	pFace	pCurr;
	pFace	pCurr2;
	pFace	Face;

	for(pCurr=FaceList ; pCurr ; pCurr=pCurr->Next)
		pCurr->Used = false;

	for(pCurr=FaceList ; pCurr ; pCurr=pCurr->Next)
	{
		if(pCurr->Used)
			continue;
		for(pCurr2=FaceList ; pCurr2 ; pCurr2=pCurr2->Next)
		{
			if(pCurr2->Used)
				continue;
			if(pCurr == pCurr2)
				continue;
			if(pCurr->NonFragFace)
			if(pCurr2->NonFragFace)
			if(pCurr->NonFragFace == pCurr2->NonFragFace)
			{
				pCurr->Used = true;
				pCurr2->Used = true;
				Face = pCurr->NonFragFace;
				//Face->OriginalFace = Face;
				Face->Used = false;
				Face->Next = FaceList;
				FaceList = Face;
			}
		}
	}

	Face=FaceList;
	while(Face)
	{
		if(Face->Used)
		{
			FaceList = RemoveFaceFromList(FaceList,Face);
			Face = FaceList;
			continue;
		}
		Face=Face->Next;
	}
	return FaceList;
}

pFace RemovePortalFragments(pFace FaceList)
{
	pFace	pCurr;
	pFace	pCurr2;
	pFace	Face;

	for(pCurr=FaceList ; pCurr ; pCurr=pCurr->Next)
		pCurr->Used = false;

	for(pCurr=FaceList ; pCurr ; pCurr=pCurr->Next)
	{
		if(pCurr->Used)
			continue;
		for(pCurr2=FaceList ; pCurr2 ; pCurr2=pCurr2->Next)
		{
			if(pCurr2->Used)
				continue;
			if(pCurr == pCurr2)
				continue;
			if(pCurr->NonFragFace)
			if(pCurr2->NonFragFace)
			if(pCurr->NonFragFace == pCurr2->NonFragFace)
			{
			if(	((pCurr->LeafOwnerArray[0] == pCurr2->LeafOwnerArray[0]) &&
				 (pCurr->LeafOwnerArray[1] == pCurr2->LeafOwnerArray[1]))	||
				((pCurr->LeafOwnerArray[0] == pCurr2->LeafOwnerArray[1]) &&
				 (pCurr->LeafOwnerArray[1] == pCurr2->LeafOwnerArray[0])) )
				{
					pCurr->Used = true;
					pCurr2->Used = true;
					Face = pCurr->NonFragFace;
					Face->Used = false;
					Face->Next = FaceList;
					FaceList = Face;
				}
			}
		}
	}

	Face=FaceList;
	while(Face)
	{
		if(Face->Used)
		{
			FaceList = RemoveFaceFromList(FaceList,Face);
			Face = FaceList;
			continue;
		}
		Face=Face->Next;
	}
	return FaceList;
}

void FindInaccessibleLeaf()
{
	int		i;
	int		startleaf;
	vect_t	pos;
	int		count;

	printf("-- FindInaccessibleLeaf --\n");

	if(!gStartX && gStartY && !gStartZ)
		Error("Player starting pos not found");
	pos.X = gStartX;
	pos.Y = gStartY;
	pos.Z = gStartZ;

	// find player starting leaf
	startleaf = FindLeaf_r(pos,0);
	if(startleaf < 0)
		Error("Player starting pos not in a leaf");
	printf("start leaf: %d\n",startleaf);

	for(i=0 ; i<Nb_leafs ; i++)
		LeafArray[i].visited = false;

	MarkLeaf_r(startleaf);

	count = 0;
	for(i=0 ; i<Nb_leafs ; i++)
	if(LeafArray[i].visited == false)
	{
		count++;
		//RemoveLeaf(i);
	}

	printf("Removed Leaf: %d\n",count);

}

void MarkLeaf_r(int leaf)
{
	int		nextleaf;
	int		i;

	if(LeafArray[leaf].visited)
		return;
	//printf("%d ",leaf);
	LeafArray[leaf].visited = true;

	for(i=0 ; i<LeafArray[leaf].NumberOfNearLeaf ; i++)
	{
		nextleaf = LeafArray[leaf].NearLeaf[i];
		MarkLeaf_r(nextleaf);
	}
}

int FindLeaf_r(vect_t pos, int node)
{
	int		res;
	plan_t	Plan;

	if(node == EMPTY_SPACE)
		return EMPTY_SPACE;
	if(node == SOLID_SPACE)
		return SOLID_SPACE;

	if(NodeArray[node].Feuille >= 0)
		return NodeArray[node].Feuille;

	Plan = NodeArray[node].Plan;
	res = ClassifyVect(&pos,&Plan);

	if(res == CONFONDU || res == DEVANT)
		return FindLeaf_r(pos,NodeArray[node].Front);
	return FindLeaf_r(pos,NodeArray[node].Back);
}

pFace RemoveInvalidCellAndPortals(pFace PortalList)
{
//	int		i;
	pFace	Portal;
	int		count;

	printf("RemoveInvalidCellAndPortals\n");

	for(Portal=PortalList ; Portal ; Portal=Portal->Next)
		Portal->Used = false;

	for(Portal=PortalList ; Portal ; Portal=Portal->Next)
	{
		if(LeafArray[Portal->LeafOwnerArray[0]].visited == false)
			Portal->Used = true;

		if(LeafArray[Portal->LeafOwnerArray[1]].visited == false)
			Portal->Used = true;
	}

	count = 0;
	Portal=PortalList;
	while(Portal)
	{
		if(Portal->Used)
		{
			count++;
			PortalList = RemoveFaceFromList(PortalList,Portal);
			Portal = PortalList;
			continue;
		}
		Portal = Portal->Next;
	}
	printf("portal removed: %d\n",count);

	/*
	count = 0;
	for(i=0 ; i<Nb_leafs ; i++)
	{
		if(LeafArray[i].visited == false)
		{
			count++;
			for(Portal=PortalList ; Portal ; Portal=Portal->Next)
			{
				if(Portal->LeafOwnerArray[0] == Nb_leafs-1)
					Portal->LeafOwnerArray[0] = i;
				if(Portal->LeafOwnerArray[1] == Nb_leafs-1)
					Portal->LeafOwnerArray[1] = i;
			}

			LeafArray[i] = LeafArray[Nb_leafs-1];
			LeafArray[i].LeafID = i;
			FaceArray[i] = FaceArray[Nb_leafs-1];
			FaceArray[i].FaceID = i;
			// TODO: delete FaceArray[Nb_leafs-1] liste faces //
			Nb_leafs--;
		}
	}
	printf("leaf removed: %d\n",count);
	*/

	return PortalList;
}
