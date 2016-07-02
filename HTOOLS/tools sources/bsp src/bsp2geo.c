
#include "bsp2geo.h"

void GetWorldBox(pFace FaceList)
{
	printf("GetWorldBox\n");
	ClearBound(&WorldBox);
	GetBound(FaceList,&WorldBox);
}

pFace ConstructSolidGeometry(pFace FaceList)
{
	pFace	WorldList;
	pFace	ClipList;
	pFace	NonBrushList;
	pFace	NonActionBrushList;
	int		i;
	int		j;

	printf("-- CSGeometry --\n");

	NonActionBrushList = FindActionBrush(FaceList);
	NonBrushList = FindBrush(NonActionBrushList);

/*
	for(i=0 ; i<Nb_Brush ; i++)
	{
		if(BrushTab[i].Valid == false)
		{
			BrushTab[i].ListFace = RemoveList(BrushTab[i].ListFace);
			BrushTab[i] = BrushTab[Nb_Brush-1];
			Nb_Brush--;
		}
	}
*/

	printf("RemoveBrushUnions\n");
	WorldList = NULL;
	for(i=0 ; i<Nb_Brush ; i++)
	{
	//	if(!BrushTab[i].Valid)	//
	//		continue;			//
		ClipList = BrushTab[i].ListFace;
		for(j=0 ; j<Nb_Brush ; j++)
		{
			if(i==j)
				continue;
	//		if(!BrushTab[j].Valid)	//
	//			continue;			//
			Compute_BSP(BrushTab[j].ListFace);
			ClipList = ClipFacesToBSP(ClipList);
			ClipList = RemoveFragments(ClipList);
			Clear_BSP();
		}
		BrushTab[i].ClipedFace = ClipList;
		WorldList = AddListes(WorldList,ClipList);
	}
	if(menu_nodetail == false)
		WorldList = AddListes(WorldList,NonBrushList);
	return WorldList;
}

pFace FindBrush(pFace FaceList)
{
	pFace	pCurr;
	pFace	pCurr3;
	pFace	BrushList;
	pFace	NonBrushList;
	pFace	Face;
	int		InvalidBrush;

	printf("FindBrushes\n");

	if(FaceList==NULL)
		Error("FaceList is NULL !");

	Nb_Brush = 0;
	InvalidBrush = 0;
	NonBrushList = NULL;

	while(FaceList)
	{
		// BrushID :
		// -1 <=> map bound
		// -2 <=> detail objets
		// -3 <=> invisible objet	
		if(FaceList->BrushID < 0 && FaceList->IsDetailObjet)
		{	
			Face = CopyFace(FaceList);
			//Face->OriginalFace = Face;

			Face->Next = NonBrushList;
			NonBrushList = Face;
			FaceList = RemoveFaceFromList(FaceList,FaceList);
			continue;
		}
	
		if(Nb_Brush >= MAX_BRUSH)
			Error("Too Much Brush !");

		BrushList = NULL;
		for(pCurr=FaceList ; pCurr ; pCurr=pCurr->Next)
		{
			if(pCurr->BrushID != Nb_Brush)
				continue;
			pCurr3 = CopyFace(pCurr);
			//pCurr3->OriginalFace = pCurr3;


			if(pCurr3->IsDetailObjet != LAVA)
				pCurr3->IsDetailObjet = 0;
			pCurr3->Next = BrushList;
			BrushList = pCurr3;
		}
		for(pCurr=FaceList ; pCurr ; pCurr=pCurr->Next)
		{
			if(pCurr->BrushID == Nb_Brush)
				FaceList = RemoveFaceFromList(FaceList,pCurr);
		}

		BrushTab[Nb_Brush].ListFace = BrushList;
		BrushTab[Nb_Brush].Nb_Face = CountFaces(BrushList);
		BrushTab[Nb_Brush].Valid = IsConvexPolytope(BrushList);
		if(BrushTab[Nb_Brush].Valid == false)
		{
			InvalidBrush++;
		}
	//	else
	//	{
			Nb_Brush++;
	//	}
	}
	printf("Brush(es): %d\n",Nb_Brush);
	printf("NonConvex Brush(es): %d\n",InvalidBrush);
	return NonBrushList;
}

pFace FindActionBrush(pFace FaceList)
{
	pFace	pCurr;
	pFace	pCurr3;
	pFace	ActionBrushList;
	pFace	NonActionBrushList;
	pFace	Face;
	int		tab;

	printf("FindActionBrushes\n");
	if(FaceList==NULL)
		Error("FaceList is NULL !");
	tab = 0;
	NonActionBrushList = NULL;
	while(FaceList)
	{
		if(FaceList->IsDetailObjet != ACTION)
		{	
			Face = CopyFace(FaceList);
			//Face->OriginalFace = Face;

			Face->Next = NonActionBrushList;
			NonActionBrushList = Face;
			FaceList = RemoveFaceFromList(FaceList,FaceList);
			continue;
		}
		Nb_ActionBrush = FaceList->BrushID;
		if(Nb_ActionBrush >= MAX_BRUSH)
			Error("Too Much ActionBrush !");
		ActionBrushList = NULL;
		for(pCurr=FaceList ; pCurr ; pCurr=pCurr->Next)
		{
			if(pCurr->BrushID != Nb_ActionBrush)
				continue;
			pCurr3 = CopyFace(pCurr);
			//pCurr3->OriginalFace = pCurr3;

			pCurr3->Next = ActionBrushList;
			ActionBrushList = pCurr3;
		}
		for(pCurr=FaceList ; pCurr ; pCurr=pCurr->Next)
		{
			if(pCurr->BrushID == Nb_ActionBrush)
				FaceList = RemoveFaceFromList(FaceList,pCurr);
		}
		ActionBrushList = SplitToTriangle(ActionBrushList);	// pour les collisions halloween a besoin de triangles
		ActionBrushTab[tab].ListFace = ActionBrushList;
		ActionBrushTab[tab].Nb_Face = CountFaces(ActionBrushList);
		tab++;
	}
	Nb_ActionBrush = tab;
	return NonActionBrushList;
}

pFace SnapWordToGrid(pFace FaceList)
{
	pFace	pCurr;
	int		i;

	printf("SnapWordToGrid\n");

	AdjustVerxtex(&gStartX,10);
	AdjustVerxtex(&gStartY,10);
	AdjustVerxtex(&gStartZ,10);

	for(pCurr=FaceList ; pCurr ; pCurr=pCurr->Next)
	{
//		if(pCurr->IsDetailObjet == BSPLESS)
//			continue;
		AdjustVerxtex(&pCurr->Plan.PointOnPlane.X,10);
		AdjustVerxtex(&pCurr->Plan.PointOnPlane.Y,10);
		AdjustVerxtex(&pCurr->Plan.PointOnPlane.Z,10);

		//pCurr->Plan.VecteurNormal = GetFaceNormal(pCurr->point[0],pCurr->point[1],pCurr->point[2]);

		for(i=0 ; i<pCurr->NumberOfVertices ; i++)
		{
			AdjustVerxtex(&pCurr->point[i].X,10);
			AdjustVerxtex(&pCurr->point[i].Y,10);
			AdjustVerxtex(&pCurr->point[i].Z,10);
		}

		pCurr->Plan.VecteurNormal = GetFaceNormal(pCurr->point[0],pCurr->point[1],pCurr->point[2]);
	}
	return FaceList;
}

void AdjustVerxtex(float *vertex, int grid)
{
	float	val;
	int		coef;

	if(*vertex>=0)
	{
		val = *vertex;
		coef = (int)(val/(float)grid);
		*vertex = (float)(coef*grid);
	}
	else
	{
		val = *vertex - (float)grid;
		coef = (int)(val/(float)grid);
		*vertex = (float)(coef*grid);
	}
}

pFace CenterVertices(pFace FaceList)
{
	BOUNDINGBOX	bbox;
	vect_t		center;
	pFace		Face;
	int			i;
	int			j;

	printf("CenterVertices\n");

	ClearBound(&bbox);
	GetBound(FaceList,&bbox);

	center.X = (bbox.BoxMin.X + bbox.BoxMax.X)/2.0f;
	center.Y = (bbox.BoxMin.Y + bbox.BoxMax.Y)/2.0f;
	center.Z = (bbox.BoxMin.Z + bbox.BoxMax.Z)/2.0f;

	gStartX -= center.X;
	gStartY -= center.Y;
	gStartZ -= center.Z;

	for(j=0 ; j<Nb_entities ; j++)
	{
		Entities2[j].pos.X -= center.X;
		Entities2[j].pos.Y -= center.Y;
		Entities2[j].pos.Z -= center.Z;
	}

	for(j=0 ; j<Nb_Lights ; j++)
	{
		Lights[j].pos.X -= center.X;
		Lights[j].pos.Y -= center.Y;
		Lights[j].pos.Z -= center.Z;
	}

	for(Face=FaceList ; Face ; Face=Face->Next)
	{
		Face->Plan.PointOnPlane.X -= center.X;
		Face->Plan.PointOnPlane.Y -= center.Y;
		Face->Plan.PointOnPlane.Z -= center.Z;

		for(i=0 ; i<Face->NumberOfVertices ; i++)
		{
			Face->point[i].X -= center.X;
			Face->point[i].Y -= center.Y;
			Face->point[i].Z -= center.Z;
		}
	}
	return FaceList;
}

pFace GroupMax(pFace PolyList)
{
	int		count1;
	int		count2;

	while(1)
	{
		count1 = CountFaces(PolyList);
		PolyList = GroupFaceList(PolyList,true);
		count2 = CountFaces(PolyList);
		if(count2>=count1)
			break;
		//printf(".");
	}
	//printf("\n");

	return PolyList;
}

bool IsConvexFace(pFace Face)
{
	bool	IsValid;
	int		i;
	vect_t	v1, v2, v3;
	float	val;
	int		i1, i2;

	IsValid=true;
	for(i=0 ; i<Face->NumberOfVertices ; i++)
	{
		if(i==0)
			i1 = Face->NumberOfVertices-1;
		else
			i1=i-1;

		if(i==Face->NumberOfVertices-1)
			i2=0;
		else
			i2=i+1;

		v1.X = Face->point[i1].X - Face->point[i].X;
		v1.Y = Face->point[i1].Y - Face->point[i].Y;
		v1.Z = Face->point[i1].Z - Face->point[i].Z;

		v2.X = Face->point[i2].X - Face->point[i].X;
		v2.Y = Face->point[i2].Y - Face->point[i].Y;
		v2.Z = Face->point[i2].Z - Face->point[i].Z;

		v3 = PdVectoriel(v1,v2);
		val=PdtScalaire(v3,Face->Plan.VecteurNormal);
		if(val<0)
		{
			IsValid=false;
			break;
		}
	}
	return IsValid;
}

bool EqualVertice(pFace Face)
{
	int		i;
	int		j;

	for(i=0 ; i<Face->NumberOfVertices ; i++)
	for(j=0 ; j<Face->NumberOfVertices ; j++)
	{
		if(i==j)
			continue;

		if(IsEqualf(Face->point[j].X,Face->point[i].X))
		if(IsEqualf(Face->point[j].Y,Face->point[i].Y))
		if(IsEqualf(Face->point[j].Z,Face->point[i].Z))
		if(IsEqualf(Face->point[j].U,Face->point[i].U))
		if(IsEqualf(Face->point[j].V,Face->point[i].V))
			return true;
	}
	return false;
}

void RemoveColinearVertice(pFace Face)
{
	int		i;
	vect_t	v1, v2;
	float	val;
	int		i1, i2;
	point_t	p[15];
	int		npoint;
	bool	ok;

	npoint = 0;
	for(i=0 ; i<Face->NumberOfVertices ; i++)
	{
		ok=true;
		if(i==0)
			i1 = Face->NumberOfVertices-1;
		else
			i1=i-1;

		if(i==Face->NumberOfVertices-1)
			i2=0;
		else
			i2=i+1;

		v1.X = Face->point[i1].X - Face->point[i].X;
		v1.Y = Face->point[i1].Y - Face->point[i].Y;
		v1.Z = Face->point[i1].Z - Face->point[i].Z;

		v2.X = Face->point[i2].X - Face->point[i].X;
		v2.Y = Face->point[i2].Y - Face->point[i].Y;
		v2.Z = Face->point[i2].Z - Face->point[i].Z;

		Normalize(&v1);
		Normalize(&v2);
		val = PdtScalaire(v1,v2);
		if(val == -1)
			ok = false;
		if(ok)
		{
			p[npoint] = Face->point[i];
			npoint++;
		}
	}

	Face->NumberOfVertices = npoint;
	for(i=0 ; i<npoint ; i++)
		Face->point[i] = p[i];
}

bool CheckGroupedFace(pFace Face)
{
	int		i;
	int		a, b;
	vect_t	v, w;

	if(Face->NumberOfVertices != 4)
		return false;
//	if(Face->NumberOfVertices < 3)
//		return false;
//	if(Face->NumberOfVertices > 5) // 5
//		return false;

	for(i=0 ; i<Face->NumberOfVertices ; i++)
	{
		if(i==0)
			a = Face->NumberOfVertices-1;
		else
			a = i-1;

		if(i==Face->NumberOfVertices-1)
			b = 0;
		else
			b = i+1;

		v.X = Face->point[a].X - Face->point[i].X;
		v.Y = Face->point[a].Y - Face->point[i].Y;
		v.Z = Face->point[a].Z - Face->point[i].Z;

		w.X = Face->point[b].X - Face->point[i].X;
		w.Y = Face->point[b].Y - Face->point[i].Y;
		w.Z = Face->point[b].Z - Face->point[i].Z;

		if(PdtScalaire(v,w) != 0)
			return false;
	}
	return true;
}

bool CheckFace(pFace Face)
{
	int		i;

	if(Face->NumberOfVertices < 3)
		return false;
	if(Face->NumberOfVertices > 6) // 5
		return false;

	for(i=0 ; i<Face->NumberOfVertices ; i++)
	if( (Face->point[i].X > BOGUS_RANGE || Face->point[i].X < -BOGUS_RANGE)  ||
		(Face->point[i].Y > BOGUS_RANGE || Face->point[i].Y < -BOGUS_RANGE)  ||
		(Face->point[i].Z > BOGUS_RANGE || Face->point[i].Z < -BOGUS_RANGE) )
	return false;

	if(!IsConvexFace(Face))
		return false;

	if(EqualVertice(Face))
		return false;

	return true;
}

pFace GroupFaceList(pFace FaceList, bool disp)
{
	pFace	Face;
	pFace	Face2;
	pFace	pNew;
	pFace	NewList;

	if(disp)
		printf("--GroupFaces--");

	NewList = NULL;

	for(Face=FaceList ; Face ; Face=Face->Next)
		Face->Used = false;

	for(Face=FaceList ; Face ; Face=Face->Next)
	{
		if(Face->Used)
			continue;
		if(Face->IsDetailObjet == ACTION)
			continue;
//		if(Face->IsDetailObjet == BSPLESS)
//			continue;
	//	if(Face->IsDetailObjet == DETAIL)	//
	//		continue;						//
		for(Face2=FaceList ; Face2 ; Face2=Face2->Next)
		{
			if(Face2->Used)
				continue;
			if(Face2->IsDetailObjet == ACTION)
				continue;
	//		if(Face2->IsDetailObjet == BSPLESS)
	//			continue;
	//		if(Face2->IsDetailObjet == DETAIL)	//
	//			continue;						//
			if(Face==Face2)
				continue;
			pNew = GroupFaces(Face,Face2);
			if(pNew)
			{
				Face->Used = true;
				Face2->Used = true;
				pNew->Next = NewList;
				NewList = pNew;
			}
		}
	}

	for(Face=FaceList ; Face ; )
	{
		if(Face->Used)
		{
			FaceList = RemoveFaceFromList(FaceList,Face);
			Face = FaceList;
			continue;
		}
		Face=Face->Next;
	}
	FaceList = AddListes(FaceList,NewList);
	if(disp)
		printf("\n");
//	if(disp)
//		printf("\nFACES: %d\n",CountFaces(FaceList));
	return FaceList;
	//return NewList;
}

pFace GroupFaces(pFace pTemp, pFace pTemp2)
{
	pFace	pNew;
	int		i, j, k, l;
	point_t	p1, p2, p3, p4, back;
	vect_t	v1, v2, normal;
	float	dot;
	bool	keep1, keep2;
	int		nvert;

	if(pTemp->textId != pTemp2->textId)
		return NULL;
	if(!IsEqualf(PdtScalaire(pTemp->Plan.VecteurNormal,pTemp2->Plan.VecteurNormal),1.0f))
		return NULL;

	// trouve une arrete commune
	for(i=0 ; i<pTemp->NumberOfVertices ; i++)
	{
		p1 = pTemp->point[i];
		p2 = pTemp->point[(i+1)%pTemp->NumberOfVertices];
		for(j=0 ; j<pTemp2->NumberOfVertices ; j++)
		{
			p3 = pTemp2->point[j];
			p4 = pTemp2->point[(j+1)%pTemp2->NumberOfVertices];

			if(IsEqualf(p1.X,p4.X))
			if(IsEqualf(p1.Y,p4.Y))
			if(IsEqualf(p1.Z,p4.Z))
			if(IsEqualf(p2.X,p3.X))
			if(IsEqualf(p2.Y,p3.Y))
			if(IsEqualf(p2.Z,p3.Z))
				break;
		}
		if(j<pTemp2->NumberOfVertices)
			break;
	}
	if(i==pTemp->NumberOfVertices)
		return NULL;

	// regarde le contour des lignes connectées, si colineaire alors on peut enlever le point
	back = pTemp->point[(i+pTemp->NumberOfVertices-1)%pTemp->NumberOfVertices];
	v1.X = back.X - p1.X;
	v1.Y = back.Y - p1.Y;
	v1.Z = back.Z - p1.Z;
	normal = PdVectoriel(v1,pTemp->Plan.VecteurNormal);
	Normalize(&normal);

	back = pTemp2->point[(j+2)%pTemp2->NumberOfVertices];
	v2.X = back.X - p1.X;
	v2.Y = back.Y - p1.Y;
	v2.Z = back.Z - p1.Z;
	dot = PdtScalaire(normal,v2);
	//printf("dot1 : %f\n",dot);
	if(dot > 0)
		return NULL;	// not a convex polygon
	keep1 = dot < 0;

	back = pTemp->point[(i+2)%pTemp->NumberOfVertices];
	v1.X = back.X - p2.X;
	v1.Y = back.Y - p2.Y;
	v1.Z = back.Z - p2.Z;
	normal = PdVectoriel(v1,pTemp->Plan.VecteurNormal);
	Normalize(&normal);

	back = pTemp2->point[(j+pTemp2->NumberOfVertices-1)%pTemp2->NumberOfVertices];
	v2.X = back.X - p2.X;
	v2.Y = back.Y - p2.Y;
	v2.Z = back.Z - p2.Z;
	dot = -PdtScalaire(normal,v2);
	//printf("dot2 : %f\n",dot);
	if(dot > 0)
		return NULL;	// not a convex polygon
	keep2 = dot < 0;

	// construit le nouveau polygon
	if(pTemp->NumberOfVertices + pTemp2->NumberOfVertices > MAXEDGES)
	{
		//printf("too much edges");
		return NULL;
	}

	nvert = pTemp->NumberOfVertices + pTemp2->NumberOfVertices;
	nvert = nvert - 2;
	if(!keep1)
		nvert--;
	if(!keep2)
		nvert--;
	pNew = AllocFace(nvert);

	nvert = 0;
	// copy first polygon
	for(k=(i+1)%pTemp->NumberOfVertices ; k != i ; k=(k+1)%pTemp->NumberOfVertices)
	{
		if(k==(i+1)%pTemp->NumberOfVertices && !keep2)
			continue;				
		pNew->point[nvert] = pTemp->point[k];
		nvert++;
	}

	// copy second polygon
	for(l=(j+1)%pTemp2->NumberOfVertices ; l != j ; l=(l+1)%pTemp2->NumberOfVertices)
	{
		if (l==(j+1)%pTemp2->NumberOfVertices && !keep1)
			continue;
		pNew->point[nvert] = pTemp2->point[l];
		nvert++;
	}

	pNew->Plan = pTemp->Plan;
	pNew->Plan.PointOnPlane.X = pNew->point[0].X;
	pNew->Plan.PointOnPlane.Y = pNew->point[0].Y;
	pNew->Plan.PointOnPlane.Z = pNew->point[0].Z;

	pNew->OriginalFace = pNew;

	pNew->BrushID = pTemp->BrushID;
	pNew->textId = pTemp->textId;
	pNew->IsDetailObjet = pTemp->IsDetailObjet;
	pNew->ActionNumber = pTemp->ActionNumber;

	return pNew;
}

pFace RemoveHiddenFaces(pFace FaceList)
{
	pFace	faceA;
	pFace	faceB;

//	printf("RemoveHiddenFaces");

	for(faceA=FaceList ; faceA ; faceA=faceA->Next)
		faceA->Used = false;

	for(faceA=FaceList ; faceA ; faceA=faceA->Next)
	{
	//	if(!IsRectangle(faceA))
	//		continue;
		for(faceB=FaceList ; faceB ; faceB=faceB->Next)
		{
			if(faceA==faceB)
				continue;
			if(faceA->Used)
				continue;
			if(faceB->Used)
				continue;
		//	if(!IsRectangle(faceB))
		//		continue;
			if( ClassifyPoly(&faceA->Plan,faceB) != CONFONDU )
				continue;
			//printf("@");
			if( IsFacesOverlap(faceA,faceB) == true )
			{
				printf(".");
			}
		}
	}
//	printf("\n");

	for(faceA=FaceList ; faceA ; faceA=faceA->Next)
	{
		if(faceA->Used)
		{
			FaceList = RemoveFaceFromList(FaceList,faceA);
			faceA = FaceList;
		}
	}

	for(faceA=FaceList ; faceA ; faceA=faceA->Next)
		faceA->Used = false;

	return FaceList;
}

bool IsFacesOverlap(pFace Fa, pFace Fb)
{
	int		i;
	vect_t	v;
	int		count;

	count=0;
	for(i=0 ; i<Fa->NumberOfVertices ; i++)
	{
		v.X = Fa->point[i].X;
		v.Y = Fa->point[i].Y;
		v.Z = Fa->point[i].Z;

		if( IsVectInRect(Fb,v) || IsVertexOnEdgeFace(v,Fb) || IsVertexEgalFace(v,Fb) )
			count++;
	}

	if(count==Fa->NumberOfVertices)
	{
		Fa->Used=true;
		return true;
	}

	count=0;
	for(i=0 ; i<Fb->NumberOfVertices ; i++)
	{
		v.X = Fb->point[i].X;
		v.Y = Fb->point[i].Y;
		v.Z = Fb->point[i].Z;

		if( IsVectInRect(Fa,v) || IsVertexOnEdgeFace(v,Fa) || IsVertexEgalFace(v,Fa) )
			count++;
	}

	if(count==Fb->NumberOfVertices)
	{
		Fb->Used=true;
		return true;
	}

	return false;
}

bool IsRectangle(pFace Face)
{
	int		i, m, n;
	vect_t	v, w;

	for(i=0 ; i<Face->NumberOfVertices ; i++)
	{
		m=i+1;
		m = m % Face->NumberOfVertices;
		n=m+1;
		n = n % Face->NumberOfVertices;

		v.X = Face->point[m].X - Face->point[i].X;
		v.Y = Face->point[m].Y - Face->point[i].Y;
		v.Z = Face->point[m].Z - Face->point[i].Z;

		w.X = Face->point[n].X - Face->point[m].X;
		w.Y = Face->point[n].Y - Face->point[m].Y;
		w.Z = Face->point[n].Z - Face->point[m].Z;

		if(PdtScalaire(v,w) != 0)
			return false;
	}
	return true;
}

bool IsVertexEgalFace(vect_t v, pFace Face)
{
	int		i;
	vect_t	v1;

	for(i=0 ; i<Face->NumberOfVertices ; i++)
	{
		v1.X = Face->point[i].X;
		v1.Y = Face->point[i].Y;
		v1.Z = Face->point[i].Z;

		if( IsVertexEgal(v,v1) == true )
			return true;
	}
	return false;
}

bool IsVertexEgal(vect_t v, vect_t a)
{
	if(IsEqualf(v.X,a.X))
	if(IsEqualf(v.Y,a.Y))
	if(IsEqualf(v.Z,a.Z))
		return true;
	return false;
}

bool IsVertexOnEdgeFace(vect_t v, pFace Face)
{
	int		i;
	int		m;
	vect_t	v1, v2;

	for(i=0 ; i<Face->NumberOfVertices ; i++)
	{
		m=i+1;
		if(m==Face->NumberOfVertices)
			m=0;

		v1.X = Face->point[i].X;
		v1.Y = Face->point[i].Y;
		v1.Z = Face->point[i].Z;

		v2.X = Face->point[m].X;
		v2.Y = Face->point[m].Y;
		v2.Z = Face->point[m].Z;

		if( IsVertexOnEdge(v,v1,v2) == true )
			return true;
	}
	return false;
}

bool IsVertexOnEdge(vect_t v, vect_t a, vect_t b)
{
	vect_t	v1, v2;
	
	v1.X = a.X - v.X;
	v1.Y = a.Y - v.Y;
	v1.Z = a.Z - v.Z;

	v2.X = b.X - v.X;
	v2.Y = b.Y - v.Y;
	v2.Z = b.Z - v.Z;

	Normalize(&v1);
	Normalize(&v2);

	if(!IsEqualf(PdtScalaire(v1,v2),1.0f))
	//if(PdtScalaire(v1,v2) != -1)
		return false;

	return true;
}

void RemoveInvalidLeaf(pFace PortalList)
{
	int			i;
	pFace		Face;

	printf("RemoveInvalidLeaf\n");
haha:
	for(i=0 ; i<Nb_leafs ; i++)
	if(!LeafArray[i].valid)
	{
		for(Face=PortalList ; Face ; Face=Face->Next)
		{
			if(Face->LeafOwnerArray[0] > i)
				Face->LeafOwnerArray[0]--;
			if(Face->LeafOwnerArray[1] > i)
				Face->LeafOwnerArray[1]--;
		}
		RemoveLeaf(i);
		goto haha;
	}
}

// outdoor removing
int RemoveLeafValidity()
{
	int			i;
	int			count;
	BOUNDINGBOX	box;

	count=0;
	printf("RemoveLeafValidity");

	for(i=0 ; i<Nb_leafs ; i++)
	{
		box = LeafArray[i].bbox;

	//	if(Absf(box.BoxMin.X) == MAX_WORLD ||
	//		Absf(box.BoxMin.Y) == MAX_WORLD ||
	//		Absf(box.BoxMin.Z) == MAX_WORLD ||
	//		Absf(box.BoxMax.X) == MAX_WORLD ||
	//		Absf(box.BoxMax.Y) == MAX_WORLD ||
	//		Absf(box.BoxMax.Z) == MAX_WORLD)
		if( (box.BoxMin.X == box.BoxMax.X) ||
			(box.BoxMin.Y == box.BoxMax.Y) ||
			(box.BoxMin.Z == box.BoxMax.Z) )
		{
			count++;
			//RemoveLeaf(i);
			LeafArray[i].valid = false;
		}
		else
			LeafArray[i].valid = true;
	}
	printf("[%d]\n",count);
	return count;
}

int SetLeafValidity()
{
	int			i;
	int			count;
	BOUNDINGBOX	box;
//	pFace	Face;
//	pFace	FaceList;
//	int		nbface;
//	int		leafid;

	count=0;
	printf("SetLeafValidity");
/*
	for(i=0 ; i<Nb_leafs ; i++)
		LeafArray[i].Nb_faces = 0;

	for(i=0 ; i<Nb_leafs ; i++)
	{
		FaceList = FaceArray[i].faceliste;

		for(Face=FaceList ; Face ; Face=Face->Next)
		{
			leafid = Face->LeafID;
			nbface = LeafArray[leafid].Nb_faces;
			LeafArray[leafid].FaceIds[nbface] = Face->FaceID;
			LeafArray[leafid].Nb_faces++;
		}
	}
*/
	for(i=0 ; i<Nb_leafs ; i++)
	{
		box = LeafArray[i].bbox;

		if(
			
			(Absf(box.BoxMin.X) == MAX_WORLD ||
			Absf(box.BoxMin.Y) == MAX_WORLD ||
			Absf(box.BoxMin.Z) == MAX_WORLD ||
			Absf(box.BoxMax.X) == MAX_WORLD ||
			Absf(box.BoxMax.Y) == MAX_WORLD ||
			Absf(box.BoxMax.Z) == MAX_WORLD)
			
		//	||
			

//		if( ((box.BoxMin.X == box.BoxMax.X) && (box.BoxMin.Y == box.BoxMax.Y)) ||
//			((box.BoxMin.X == box.BoxMax.X) && (box.BoxMin.Z == box.BoxMax.Z)) ||
//			((box.BoxMin.Z == box.BoxMax.Z) && (box.BoxMin.Y == box.BoxMax.Y)) )

	//	( (box.BoxMin.X == box.BoxMax.X) ||
	//		(box.BoxMin.Y == box.BoxMax.Y) ||
	//		(box.BoxMin.Z == box.BoxMax.Z) )
			
			)
		{
			count++;
			RemoveLeaf(i);
		//	LeafArray[i].valid = false;
		}
		//else
		//	LeafArray[i].valid = true;
	}
	printf("[%d]\n",count);
	return count;
}

void RemoveLeaf(int leaf)
{
	LeafArray[leaf] = LeafArray[Nb_leafs-1];
	LeafArray[leaf].LeafID = leaf;
	FaceArray[leaf] = FaceArray[Nb_leafs-1];
	FaceArray[leaf].FaceID = leaf;
	// TODO: delete FaceArray[Nb_leafs-1] liste faces //
	Nb_leafs--;
}

void GetBrushNormals()
{
	int		i;

	for(i=0 ; i<Nb_Brush ; i++)
	{
		GetVertNormals(BrushTab[i].ListFace);
	//	GetVertNormals_2(BrushTab[i].ListFace);
	}
}

void GetVertNormals_2(pFace List)
{
	pFace	Face1;
	vect_t	center;
	vect_t	normal;
	int		i;
	float	divider;

	divider = 1;
	center.X = 0;
	center.Y = 0;
	center.Z = 0;
	for(Face1=List ; Face1 ; Face1=Face1->Next)
	{
		for(i=0 ; i<Face1->NumberOfVertices ; i++)
		{
			divider++;
			center.X += Face1->point[i].X;
			center.Y += Face1->point[i].Y;
			center.Z += Face1->point[i].Z;
		}
	}

	center.X /= divider;
	center.Y /= divider;
	center.Z /= divider;

	for(Face1=List ; Face1 ; Face1=Face1->Next)
	{
		for(i=0 ; i<Face1->NumberOfVertices ; i++)
		{
			normal.X = Face1->point[i].X - center.X;
			normal.Y = Face1->point[i].Y - center.Y;
			normal.Z = Face1->point[i].Z - center.Z;

			Normalize(&normal);

			Face1->point[i].NX = normal.X;
			Face1->point[i].NY = normal.Y;
			Face1->point[i].NZ = normal.Z;
		}
	}
}

void GetVertNormals(pFace List)
{
	pFace	Face1;
	pFace	Face2;
	int		i;
	int		j;
	int		count;
	vect_t	normal;
	float	divider;

	for(Face1=List ; Face1 ; Face1=Face1->Next)
	{
		for(i=0 ; i<Face1->NumberOfVertices ; i++)
		{
			divider = 1;
			normal.X = Face1->Plan.VecteurNormal.X;
			normal.Y = Face1->Plan.VecteurNormal.Y;
			normal.Z = Face1->Plan.VecteurNormal.Z;

			for(Face2=List ; Face2 ; Face2=Face2->Next)
			{
				if(Face1==Face2)
					continue;

				count = 0;
				for(j=0 ; j<Face2->NumberOfVertices ; j++)
				{
					if(IsEqualf(Face1->point[i].X,Face2->point[j].X))
					if(IsEqualf(Face1->point[i].Y,Face2->point[j].Y))
					if(IsEqualf(Face1->point[i].Z,Face2->point[j].Z))
						count++;
				}

				if(count)
				{
					divider++;
					normal.X += Face2->Plan.VecteurNormal.X;
					normal.Y += Face2->Plan.VecteurNormal.Y;
					normal.Z += Face2->Plan.VecteurNormal.Z;
				}
			}

			normal.X = -(normal.X / divider);
			normal.Y = -(normal.Y / divider);
			normal.Z = -(normal.Z / divider);

		//	Normalize(&normal);

			Face1->point[i].NX = normal.X;
			Face1->point[i].NY = normal.Y;
			Face1->point[i].NZ = normal.Z;
		}
	}
}

pFace SplitToTriangle(pFace PolyList)
{
	pFace	Face;
	pFace	pTemp;
	pFace	pNew;
	pFace	pNewList=NULL;
	int		Nb_split;
	int		loop;

	Face = PolyList;
	while(Face)
	{
		pTemp = Face;
		Face = Face->Next;

		if(pTemp->NumberOfVertices == 3)
		{
			pTemp->Next = pNewList;
			pNewList = pTemp;
		}
		else if(pTemp->NumberOfVertices > 3)
		{
			Nb_split = pTemp->NumberOfVertices - 2;

		//	LeafArray[pTemp->LeafID].Nb_faces += (Nb_split-1);

			loop=1;
			while(Nb_split--)
			{
				pNew = AllocFace(3);

				pNew->Plan = pTemp->Plan;
				pNew->textId = pTemp->textId;
				pNew->lightID = pTemp->lightID;
				pNew->BrushID = pTemp->BrushID;
				pNew->ActionNumber = pTemp->ActionNumber;
				pNew->IsDetailObjet = pTemp->IsDetailObjet;

				pNew->OriginalFace = pTemp;

				pNew->LeafID = pTemp->LeafID;

				pNew->point[0].X = pTemp->point[0].X;
				pNew->point[0].Y = pTemp->point[0].Y;
				pNew->point[0].Z = pTemp->point[0].Z;
				pNew->point[0].U = pTemp->point[0].U;
				pNew->point[0].V = pTemp->point[0].V;
				pNew->point[0].U_lm = pTemp->point[0].U_lm;
				pNew->point[0].V_lm = pTemp->point[0].V_lm;

				pNew->point[1].X = pTemp->point[loop].X;
				pNew->point[1].Y = pTemp->point[loop].Y;
				pNew->point[1].Z = pTemp->point[loop].Z;
				pNew->point[1].U = pTemp->point[loop].U;
				pNew->point[1].V = pTemp->point[loop].V;
				pNew->point[1].U_lm = pTemp->point[loop].U_lm;
				pNew->point[1].V_lm = pTemp->point[loop].V_lm;

				pNew->point[2].X = pTemp->point[loop+1].X;
				pNew->point[2].Y = pTemp->point[loop+1].Y;
				pNew->point[2].Z = pTemp->point[loop+1].Z;
				pNew->point[2].U = pTemp->point[loop+1].U;
				pNew->point[2].V = pTemp->point[loop+1].V;
				pNew->point[2].U_lm = pTemp->point[loop+1].U_lm;
				pNew->point[2].V_lm = pTemp->point[loop+1].V_lm;
		
				loop++;

				if(CheckFace(pNew))
				{
					pNew->Next = pNewList;
					pNewList = pNew;
				}
				else
					FreeFace(pNew);
			}
		}
	}
	return pNewList;
}

pFace RemoveWrongFaces(pFace PolyList)
{
	pFace	Face;
	pFace	pTemp;
	vect_t	normal;
	int		count;

	printf("RemoveWrongFaces");

	count = 0;
	for(Face=PolyList ; Face ; )
	{
		pTemp = Face;
		Face=Face->Next;

		normal = pTemp->Plan.VecteurNormal;
		if(normal.X == 0 && normal.Y == 0 && normal.Z == 0)
		{
			count++;
			PolyList = RemoveFaceFromList(PolyList,pTemp);
			Face = PolyList;
		}
	}
	printf("\n");
//	printf("\nFACES: %d\n",CountFaces(PolyList));
	return PolyList;
}

