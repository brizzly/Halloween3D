
void CalculatePVS()
{
	char		*LeafPVS;
	int			PVSMasterPos;
	int			Leaf;
	int			SPI;
	int			DPI;
	int			TargetLeaf;
	RECTANGLE	*SourcePortal;
	RECTANGLE	*TargetPortal;
	int			Nb_PVS_notcomplet;
	int			LeafIndex[50000];
	int			i;
	int			j;
	int			k;
	int			l;
	int			m;

	printf("Computing PVS\n");
	LeafPVS = malloc(sizeof(char)*Nb_leafs);
	PVSMasterPos = 0;
	Nb_PVS_notcomplet = 0;

	for(Leaf=0 ; Leaf<Nb_leafs ; Leaf++)
	{
		LeafIndex[Leaf] = Leaf;
		LeafArray[Leaf].PVS_calculated = false;
	}
		
	for(i=0 ; i<Nb_leafs ; i++)
	{
		for(j=0 ; j<Nb_leafs ; j++)
		{
			if(i==j)
				continue;
			k = LeafIndex[i];
			l = LeafIndex[j];

			if(LeafArray[k].NumberOfPortals < LeafArray[l].NumberOfPortals)
			{
				m = LeafIndex[i];
				LeafIndex[i] = LeafIndex[j];
				LeafIndex[j] = m;
			}
		}
	}

//	for(i=0 ; i<Nb_leafs ; i++)
//		printf("%d\n",LeafArray[LeafIndex[i]].NumberOfPortals);
//	printf("go..\n");

//	for(Leaf=0 ; Leaf<Nb_leafs ; Leaf++)
	for(i=0 ; i<Nb_leafs ; i++)
	{
		Leaf = LeafIndex[i];

		PVS_ok = true;
	//	LeafArray[Leaf].PVS_calculated = false;
		LeafArray[Leaf].PVSIndex = PVSMasterPos;
		memset(LeafPVS,'0',Nb_leafs);
		SetPVSBit(LeafPVS,Leaf);

		for(SPI=0 ; SPI<LeafArray[Leaf].NumberOfPortals ; SPI++)
		{
			SourcePortal = PortalArray[LeafArray[Leaf].PortalIndexList[SPI]];
			TargetLeaf = SourcePortal->LeafOwnerArray[0];
			if(TargetLeaf==Leaf)
				TargetLeaf=SourcePortal->LeafOwnerArray[1];
			SetPVSBit(LeafPVS,TargetLeaf);

			for(DPI=0 ; DPI<LeafArray[TargetLeaf].NumberOfPortals ; DPI++)
			{
				TargetPortal = PortalArray[LeafArray[TargetLeaf].PortalIndexList[DPI]];
				if(SourcePortal==TargetPortal)
					continue;
				if(ClassifyPoly(&SourcePortal->Plan,TargetPortal) == CONFONDU)
					continue;

				PVS_TimeStart = FloatTime();
				Recursivitee=0;
				RecursePVS(Leaf,SourcePortal,TargetPortal,TargetLeaf,LeafPVS);
				if(!PVS_ok)
					break;
			}
			if(!PVS_ok)
				break;
		}

		if(PVS_ok)
		{
			printf(".");
			LeafArray[Leaf].PVS_calculated = true;
		}
		else
			Nb_PVS_notcomplet++;

	//	strncpy(&PVSData[PVSMasterPos],LeafPVS,Nb_leafs);
	//	PVSMasterPos += Nb_leafs;
		strncpy(&PVSData[Leaf*Nb_leafs],LeafPVS,Nb_leafs);
	}

	printf("\n");

	if(Nb_PVS_notcomplet)
	{
		printf("%d PVS not fully calculated\n",Nb_PVS_notcomplet);
		MergePVSinfos();
	}

	free(LeafPVS);
}

void MergePVSinfos()
{
	int		i;
	int		j;
	int		counter;
	int		old_counter;
	bool	IsFine;

	printf("MergePVSinfos..\n");

	old_counter=Nb_leafs*Nb_leafs;
	while(1)
	{
		counter=0;
		for(i=0 ; i<Nb_leafs ; i++)
		if(!LeafArray[i].PVS_calculated)
		{
			IsFine=true;
			for(j=0 ; j<Nb_leafs ; j++)
			{
				if(PVSData[LeafArray[i].PVSIndex+j]=='1')
					continue;
				
				if(j==i)
				{
					PVSData[LeafArray[i].PVSIndex+j] = '1';
					continue;
				}

				if(LeafArray[j].PVS_calculated)
				{
					PVSData[LeafArray[i].PVSIndex+j] = PVSData[LeafArray[j].PVSIndex+i];
				}
				else
				{
					IsFine = false;
					counter++;
					//printf("*");
					PVSData[LeafArray[i].PVSIndex+j] = '1';
				}
			}
		//	if(IsFine)
		//	{
		//		printf("K");
				LeafArray[i].PVS_calculated = true;
		//	}
		}
		//printf("\n");
		if(counter >= old_counter)
			break;
		old_counter = counter;
	}
}

void AddPlanToList(CLIPPLANES *ClipPlanes, plan_t plan)
{
	ClipPlanes->Planes[ClipPlanes->NumberOfPlanes] = plan;
	ClipPlanes->NumberOfPlanes++;
}

void RecursePVS(int SourceLeaf, RECTANGLE *SrcPortal, RECTANGLE *TargetPortal,int TargetLeaf, char *LeafPVS)
{
	int			GeneratorLeaf;
	vect_t		SourceLeafCenter;
	vect_t		TargetLeafCenter;
	int			SourceLeafLocation;
	int			TargetLeafLocation;
	int			GPI;
	RECTANGLE	*SourcePortal;
	RECTANGLE	*GeneratorPortal;
	int			GeneratorLocation;
	int			i;
	RECTANGLE	*pTemp;

	if(PVS_ok==false)
		return;

	PVS_Time = FloatTime() - PVS_TimeStart;
	if(PVS_Time > MAXTIMEPERPVS)
	{
		printf("T");
		PVS_ok = false;
		return;
	}

	if(Recursivitee>MAXRECURSION)
	{
		printf("R");
		PVS_ok = false;
		return;
	}
	Recursivitee++;

	GeneratorLeaf=TargetPortal->LeafOwnerArray[0];
	if(GeneratorLeaf==TargetLeaf)
		GeneratorLeaf=TargetPortal->LeafOwnerArray[1];

	SetPVSBit(LeafPVS,GeneratorLeaf);

/*
#ifdef OPTIMIZE_PVS
	if(LeafArray[TargetLeaf].PVS_calculated)
	{
		if( PVSData[ (TargetLeaf*Nb_leafs) + SourceLeaf ] == '0' )
		{
			Recursivitee--;
			return;
		}
	}
	if(LeafArray[GeneratorLeaf].PVS_calculated)
	{
		if( PVSData[ (GeneratorLeaf*Nb_leafs) + SourceLeaf ] == '0' )
		{
			Recursivitee--;
			return;
		}
	}
#endif
*/

//	SourceLeafCenter = LeafArray[SourceLeaf].origin;
//	TargetLeafCenter = LeafArray[TargetLeaf].origin;

	SourceLeafCenter.X = (LeafArray[SourceLeaf].bbox.BoxMax.X + LeafArray[SourceLeaf].bbox.BoxMin.X)/2.0f;
	SourceLeafCenter.Y = (LeafArray[SourceLeaf].bbox.BoxMax.Y + LeafArray[SourceLeaf].bbox.BoxMin.Y)/2.0f;
	SourceLeafCenter.Z = (LeafArray[SourceLeaf].bbox.BoxMax.Z + LeafArray[SourceLeaf].bbox.BoxMin.Z)/2.0f;
	TargetLeafCenter.X = (LeafArray[TargetLeaf].bbox.BoxMax.X + LeafArray[TargetLeaf].bbox.BoxMin.X)/2.0f;
	TargetLeafCenter.Y = (LeafArray[TargetLeaf].bbox.BoxMax.Y + LeafArray[TargetLeaf].bbox.BoxMin.Y)/2.0f;
	TargetLeafCenter.Z = (LeafArray[TargetLeaf].bbox.BoxMax.Z + LeafArray[TargetLeaf].bbox.BoxMin.Z)/2.0f;

	SourceLeafLocation = ClassifyVect(&SourceLeafCenter,&SrcPortal->Plan);
	TargetLeafLocation = ClassifyVect(&TargetLeafCenter,&TargetPortal->Plan);

	for(GPI=0 ; GPI<LeafArray[GeneratorLeaf].NumberOfPortals ; GPI++)
	{
		pTemp = PortalArray[LeafArray[GeneratorLeaf].PortalIndexList[GPI]];
		if(pTemp == TargetPortal)
			continue;

#ifdef OPTIMIZE_PVS
		if(pTemp->LeafOwnerArray[0] != GeneratorLeaf)
		{
			if(LeafArray[pTemp->LeafOwnerArray[0]].PVS_calculated)
			{
				if( PVSData[ (pTemp->LeafOwnerArray[0]*Nb_leafs) + SourceLeaf ] == '0')
				{
					continue;
				}
			}
		}
		else if(pTemp->LeafOwnerArray[1] != GeneratorLeaf)
		{
			if(LeafArray[pTemp->LeafOwnerArray[1]].PVS_calculated)
			{
				if( PVSData[ (pTemp->LeafOwnerArray[1]*Nb_leafs) + SourceLeaf ] == '0')
				{
					continue;
				}
			}
		}
#endif

		if(SrcPortal->point == NULL)
			printf("!");
		if(pTemp->point == NULL)
			printf("?");

		SourcePortal = malloc(sizeof(RECTANGLE));
		memset(SourcePortal,0,sizeof(RECTANGLE));
		SourcePortal->point = malloc(sizeof(point_t)*SrcPortal->NumberOfVertices);
		memset(SourcePortal->point,0,sizeof(point_t)*SrcPortal->NumberOfVertices);

		SourcePortal->NumberOfVertices = SrcPortal->NumberOfVertices;
		SourcePortal->Plan = SrcPortal->Plan;
		SourcePortal->NumberOfLeaf = SrcPortal->NumberOfLeaf;
		SourcePortal->NumberOfLeaf = SrcPortal->NumberOfLeaf;
		SourcePortal->LeafOwnerArray[0] = SrcPortal->LeafOwnerArray[0];
		SourcePortal->LeafOwnerArray[1] = SrcPortal->LeafOwnerArray[1];
		for(i=0 ; i<SrcPortal->NumberOfVertices ; i++)
		{
			SourcePortal->point[i] = SrcPortal->point[i];
		}

		GeneratorPortal = malloc(sizeof(RECTANGLE));
		memset(GeneratorPortal,0,sizeof(RECTANGLE));
		GeneratorPortal->point = malloc(sizeof(point_t)*pTemp->NumberOfVertices);
		memset(GeneratorPortal->point,0,sizeof(point_t)*pTemp->NumberOfVertices);

		GeneratorPortal->NumberOfVertices = pTemp->NumberOfVertices;
		GeneratorPortal->Plan = pTemp->Plan;
		GeneratorPortal->NumberOfLeaf = pTemp->NumberOfLeaf;
		GeneratorPortal->LeafOwnerArray[0] = pTemp->LeafOwnerArray[0];
		GeneratorPortal->LeafOwnerArray[1] = pTemp->LeafOwnerArray[1];
		for(i=0 ; i<pTemp->NumberOfVertices ; i++)
		{
			GeneratorPortal->point[i] = pTemp->point[i];
		}

		GeneratorLocation = ClassifyPoly(&SourcePortal->Plan,GeneratorPortal);
		if(GeneratorLocation==CONFONDU || GeneratorLocation==SourceLeafLocation)
		{
			DeletePortal(GeneratorPortal);
			DeletePortal(SourcePortal);
			continue;
		}

		GeneratorLocation = ClassifyPoly(&TargetPortal->Plan,GeneratorPortal);
		if(GeneratorLocation==CONFONDU || GeneratorLocation==TargetLeafLocation)
		{
			DeletePortal(GeneratorPortal);
			DeletePortal(SourcePortal);
			continue;
		}

		GeneratorPortal = ClipToAntiPenumbra(SourcePortal,TargetPortal,GeneratorPortal);
		if(GeneratorPortal==NULL) 
		{
			DeletePortal(SourcePortal);
			continue;
		}

		SourcePortal = ClipToAntiPenumbra(GeneratorPortal,TargetPortal,SourcePortal);
		if(SourcePortal==NULL)
		{
			DeletePortal(GeneratorPortal);
			continue;
		} 

		RecursePVS(SourceLeaf,SourcePortal,GeneratorPortal,GeneratorLeaf,LeafPVS);
		DeletePortal(GeneratorPortal);
		DeletePortal(SourcePortal);
	}
	Recursivitee--;
}

RECTANGLE * ClipToAntiPenumbra(RECTANGLE *SourcePortal, RECTANGLE *TargetPortal, RECTANGLE *GeneratorPortal)
{
	vect_t		EdgeVector1,EdgeVector2;
	int			PortalLocation;
	RECTANGLE	*FrontSplit;
	RECTANGLE	*BackSplit;
	RECTANGLE	*TempSource;
	RECTANGLE	*TempTarget;
	int			loop;
	CLIPPLANES	ClipPlanes;
	plan_t		TempPlane;
	int			SV;
	int			TP;
	int			i;
	int			SourcePortalLocation;
	int			NextVertex;

	ClipPlanes.NumberOfPlanes=0;
	ClipPlanes.Planes = malloc(sizeof(plan_t)*(SourcePortal->NumberOfVertices*TargetPortal->NumberOfVertices*2));

	NextVertex=0;

	for(loop=0 ; loop<2 ; loop++)
	{
		if(loop==0)
		{
			TempSource=SourcePortal;
			TempTarget=TargetPortal;
		}
		else
		{
			TempSource=TargetPortal;
			TempTarget=SourcePortal;
		}

		for(SV=0 ; SV<TempSource->NumberOfVertices ; SV++)
		{
			PortalLocation=ClassifyPoint(&TempSource->point[SV],&TempTarget->Plan);
			if(PortalLocation==CONFONDU)
				continue;

			for(TP=0 ; TP<TempTarget->NumberOfVertices ; TP++)
			{
				if(TP==TempTarget->NumberOfVertices-1)
					NextVertex=0;
				else
					NextVertex=TP+1	;

				EdgeVector1.X = TempSource->point[SV].X - TempTarget->point[TP].X;
				EdgeVector1.Y = TempSource->point[SV].Y - TempTarget->point[TP].Y;
				EdgeVector1.Z = TempSource->point[SV].Z - TempTarget->point[TP].Z;

				EdgeVector2.X = TempTarget->point[NextVertex].X - TempTarget->point[TP].X;
				EdgeVector2.Y = TempTarget->point[NextVertex].Y - TempTarget->point[TP].Y;
				EdgeVector2.Z = TempTarget->point[NextVertex].Z - TempTarget->point[TP].Z;

				TempPlane.VecteurNormal = PdVectoriel(EdgeVector1,EdgeVector2);

				if(Normalize(&TempPlane.VecteurNormal) == false)
					continue;

				TempPlane.PointOnPlane.X = TempSource->point[SV].X;
				TempPlane.PointOnPlane.Y = TempSource->point[SV].Y;
				TempPlane.PointOnPlane.Z = TempSource->point[SV].Z;

				if(ClassifyPoly(&TempPlane,TempSource) == DEVANT)
				{
					if(ClassifyPoly(&TempPlane,TempTarget) == DERRIERE)
						AddPlanToList(&ClipPlanes,TempPlane);
				}
				else if(ClassifyPoly(&TempPlane,TempSource) == DERRIERE)
				{
					if(ClassifyPoly(&TempPlane,TempTarget) == DEVANT)
						AddPlanToList(&ClipPlanes,TempPlane);
				}
			}
		}
	}

	for(i=0 ; i<ClipPlanes.NumberOfPlanes ; i++)
	{
		PortalLocation = ClassifyPoly(&ClipPlanes.Planes[i],GeneratorPortal);
		SourcePortalLocation = ClassifyPoly(&ClipPlanes.Planes[i],SourcePortal);

		if(PortalLocation == SourcePortalLocation || PortalLocation == CONFONDU)
		{
			DeletePortal(GeneratorPortal);
			free(ClipPlanes.Planes);
			return NULL;
		}
		if((PortalLocation == DERRIERE && SourcePortalLocation==DEVANT) ||
			(PortalLocation == DEVANT && SourcePortalLocation==DERRIERE))
		{
			continue;
		}
		if(PortalLocation == AUMILIEU)
		{
   			if(SourcePortalLocation==DEVANT)
			{
				BackSplit=malloc(sizeof(RECTANGLE));
				memset(BackSplit,0,sizeof(RECTANGLE));
				SplitPortal(GeneratorPortal,&ClipPlanes.Planes[i],NULL,BackSplit);
				DeletePortal(GeneratorPortal);
				GeneratorPortal=BackSplit;
			}
			else if(SourcePortalLocation==DERRIERE)
			{
				FrontSplit=malloc(sizeof(RECTANGLE));
				memset(FrontSplit,0,sizeof(RECTANGLE));
				SplitPortal(GeneratorPortal,&ClipPlanes.Planes[i],FrontSplit,NULL);
				DeletePortal(GeneratorPortal);
				GeneratorPortal=FrontSplit;
			}
		}
	}
	free(ClipPlanes.Planes);
	return GeneratorPortal;
}

void SetPVSBit(char *VisArray, int DestLeaf)
{
	VisArray[DestLeaf] = '1';
}

/////

void DeletePortal(RECTANGLE *Portal)
{
	if(Portal)
	{
		if(Portal->point)
		{
			free(Portal->point);
			Portal->point = NULL;
		}
		free(Portal);
		Portal = NULL;
	}

}

void SplitPortal(RECTANGLE *Portal, plan_t *Plan, RECTANGLE *FrontSplit, RECTANGLE *BackSplit)
{
	int	i;

	Spliting(Portal,Plan,FrontSplit,BackSplit);

	if(Portal->NumberOfLeaf<2)
		printf("@");

	for(i=0 ; i<Portal->NumberOfLeaf ; i++)
	{
		if(FrontSplit)
		{
			FrontSplit->NumberOfLeaf = Portal->NumberOfLeaf;
			FrontSplit->LeafOwnerArray[i] = Portal->LeafOwnerArray[i];
			FrontSplit->Plan = Portal->Plan;
		}
		if(BackSplit)
		{
			BackSplit->NumberOfLeaf = Portal->NumberOfLeaf;
			BackSplit->LeafOwnerArray[i] = Portal->LeafOwnerArray[i];
			BackSplit->Plan = Portal->Plan;
		}
	}
}

void Spliting(RECTANGLE *Poly, plan_t *Plan, RECTANGLE *FrontSplit, RECTANGLE *BackSplit)
{
	point_t		FrontList[400],BackList[400],FirstVertex,copy;
	vect_t		IntersectPoint,PointA,PointB,temp;
	int			FrontCounter=0,BackCounter=0,loop=0,CurrentVertex=0;
	float		percent;
	int			Result;
	float		deltax,deltay,texx,texy;

	FirstVertex=Poly->point[0];

	temp.X = FirstVertex.X;
	temp.Y = FirstVertex.Y;
	temp.Z = FirstVertex.Z;

	switch(ClassifyVect(&temp,Plan))
	{
		case DEVANT:
			FrontList[FrontCounter++]=FirstVertex;
			break;

		case DERRIERE:
			BackList[BackCounter++]=FirstVertex;
			break;

		case CONFONDU:
		BackList[BackCounter++]=FirstVertex;
		FrontList[FrontCounter++]=FirstVertex;
			break;
	}

	for(loop=1 ; loop<Poly->NumberOfVertices+1 ; loop++)
	{
		if(loop==Poly->NumberOfVertices) 
			CurrentVertex=0;
		else
			CurrentVertex=loop;

		PointA.X = Poly->point[loop-1].X;
		PointA.Y = Poly->point[loop-1].Y;
		PointA.Z = Poly->point[loop-1].Z;

		PointB.X = Poly->point[CurrentVertex].X;
		PointB.Y = Poly->point[CurrentVertex].Y;
		PointB.Z = Poly->point[CurrentVertex].Z;

		Result=ClassifyVect(&PointB,Plan);
		if(Result==CONFONDU)
		{
			BackList[BackCounter++]=Poly->point[CurrentVertex];
			FrontList[FrontCounter++]=Poly->point[CurrentVertex];
		}
		else
		{
			if(Get_Intersect(&PointA,&PointB,*Plan,&IntersectPoint, &percent)==true)
			{
				deltax = Poly->point[CurrentVertex].U - Poly->point[loop-1].U;
				deltay = Poly->point[CurrentVertex].V - Poly->point[loop-1].V;
				texx = Poly->point[loop-1].U + (deltax*percent);
				texy = Poly->point[loop-1].V + (deltay*percent);
				copy.X = IntersectPoint.X;
				copy.Y = IntersectPoint.Y;
				copy.Z = IntersectPoint.Z;
				copy.U = texx;
				copy.V = texy;

				if(Result==DEVANT)
				{
					BackList[BackCounter++]=copy;			
					FrontList[FrontCounter++]=copy;
					if(CurrentVertex!=0)
						FrontList[FrontCounter++]=Poly->point[CurrentVertex];
				}	
				if(Result==DERRIERE)
				{
					FrontList[FrontCounter++]=copy;
					BackList[BackCounter++]=copy;		
					if(CurrentVertex!=0)
						BackList[BackCounter++]=Poly->point[CurrentVertex];
				}			
			}
			else
			{
				if(Result==DEVANT)
				{
					if(CurrentVertex!=0)
						 FrontList[FrontCounter++]=Poly->point[CurrentVertex];
				}
				if(Result==DERRIERE) 
				{
					if(CurrentVertex!=0)
						BackList[BackCounter++]=Poly->point[CurrentVertex];
				}
			}
		}
	}

	if(FrontSplit)
	{
		FrontSplit->NumberOfVertices=0;
		FrontSplit->point = malloc(sizeof(point_t)*FrontCounter);
		for(loop=0 ; loop<FrontCounter ; loop++)
		{
			FrontSplit->NumberOfVertices++;
			FrontSplit->point[loop]=FrontList[loop];
		}
	}
	if(BackSplit)
	{
		BackSplit->NumberOfVertices=0;
		BackSplit->point = malloc(sizeof(point_t)*BackCounter);
		for(loop=0 ; loop<BackCounter ; loop++)
		{
			BackSplit->NumberOfVertices++;
			BackSplit->point[loop]=BackList[loop];
		}
	}
}