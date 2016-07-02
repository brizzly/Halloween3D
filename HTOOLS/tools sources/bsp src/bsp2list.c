
#include "bsp2list.h"



pFace AddListes(pFace FaceList, pFace AddList)
{
	pFace	Face;

	if(AddList==NULL)
		return FaceList;
	if(FaceList==NULL)
		return AddList;
	
	Face = AddList;
	while(AddList->Next)
		AddList = AddList->Next;
	AddList->Next = FaceList;
	FaceList = Face;

	return FaceList;
}

pFace RemoveList(pFace List)
{
	while(List)
		List = RemoveFaceFromList(List,List);
	return NULL;
}

pFace RemoveFaceFromList(pFace racine, pFace remove)
{
	pFace temp;

	if(remove==NULL)
		return racine;

	if(remove == racine)
	{
		temp = racine->Next;
		FreeFace(racine);
		return temp;
	}
	else if(racine->Next)
	{
		temp = racine;
		while(temp->Next != remove)
		{
			if(temp->Next==NULL)
				return racine;
			temp = temp->Next;
		}

		temp->Next = remove->Next;

		FreeFace(remove);
	}
	return racine;
}

int CountLeafyFaces()
{
	int			i;
	int			count;

	count=0;
	for(i=0 ; i<Nb_leafs ; i++)
		count+=CountFaces(FaceArray[i].faceliste);

	return count;
}

int CountFaces(face_t facelist)
{
	int		count;
	face_t	pCurr;

	for(count=0,pCurr=facelist ; pCurr ; pCurr=pCurr->Next)
//	if(!pCurr->Cloned)
		count++;

	return count;
}

pFace CopyList(pFace List)
{
	pFace	pCurr;
	pFace	pCurr2;
	pFace	List2 = NULL;

	for(pCurr=List ; pCurr ; pCurr=pCurr->Next)
	{
		pCurr2 = CopyFace(pCurr);
		pCurr2->Next = List2;
		List2 = pCurr2;
	}
	return List2;
}

pFace CopyFace(pFace ModelFace)
{
	face_t	f;
	int		i;

	f = AllocFace(ModelFace->NumberOfVertices);
	for(i=0 ; i<ModelFace->NumberOfVertices ; i++)
		f->point[i] = ModelFace->point[i];
	f->Plan = ModelFace->Plan;
	f->textId = ModelFace->textId;
	f->IsDetailObjet = ModelFace->IsDetailObjet;
	f->ActionNumber = ModelFace->ActionNumber;
	f->BrushID = ModelFace->BrushID;
//	f->NodeID = ModelFace->NodeID;
	f->OriginalFace = f; //ModelFace->OriginalFace;
	f->Next = NULL;

	return f;
}

face_t AllocFace(int Nb_Points)
{
	face_t	f;

	f = malloc(sizeof(RECTANGLE));
	memset(f,0,sizeof(RECTANGLE));
	
	if(Nb_Points)
	{
		f->NumberOfVertices = Nb_Points;
		f->point = malloc(sizeof(point_t)*Nb_Points);
		memset(f->point,0,sizeof(point_t)*Nb_Points);
	}
	return f;
}

void FreeFace(face_t f)
{
	if(f)
	{
		if(f->point)
			free(f->point);
		free(f);
	}
}