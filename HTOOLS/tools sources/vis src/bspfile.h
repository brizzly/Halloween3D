
void WritePVS(char *name)
{
	FILE	*file;

	Rename(name,"tmp");
	
	file = fopen(name,"a");  // w
	if(file==NULL)
		return;

	PVSData = realloc(PVSData,Nb_leafs*Nb_leafs);
	fprintf(file,"<PVS> %d\n",Nb_leafs*Nb_leafs);
	fprintf(file,"%s\n",PVSData);
	fprintf(file,"@");
	fclose(file);
	printf("%s\n",name);
}

bool LoadBSPFromFile(char *name)
{
	FILE		*file;
	FILE		*Destfile;
	int			count=0;
	char		line[255];
	char		temp[255];
	int			Nb_ActionBrush;
	vect_t		pos;
	int			R, G, B, type;
	float		I;
	int			bidon, bidon2, bidon3, bidon4, bidon5;
	float		bidon_f, bidon_f2, bidon_f3, bidon_f4, bidon_f5, bidon_f6, bidon_f7;
	float		StartX=0, StartY=0, StartZ=0;
	int			i, j, k, l, m;
	pFace		pCurr=NULL;
	int			Owner1;
	int			Owner2;
	int			nVertices;
	RECTANGLE	*Portal;
	int			n;
	int			leaf1;
	int			leaf2;
	int			Id;

	file = fopen(name,"r");
	if(file==NULL)
		return false;
	printf("Loading %s\n",name);

	Rename(name,"tmp");
	Destfile = fopen(name,"w");


	fscanf(file,"%s\n",line);
	fprintf(Destfile,"%s\n",line);

	if(strncmp("<MAP>",line,5)!=0)
	{
		fclose(file);
		fclose(Destfile);
		return false;
	}
	fscanf(file,"<PLAYERPOS> %f %f %f\n",&StartX,&StartY,&StartZ);
	fprintf(Destfile,"<PLAYERPOS> %f %f %f\n",StartX,StartY,StartZ);

	fscanf(file,"<PLAYERDIR> %d\n",&bidon);
	fprintf(Destfile,"<PLAYERDIR> %d\n",bidon);

	fscanf(file,"<ENTITIES> %d\n",&Nb_entities);
	fprintf(Destfile,"<ENTITIES> %d\n",Nb_entities);
	for(j=0 ; j<Nb_entities ; j++)
	{
		fscanf(file,"%d %d %f %f %f\n",&Entities[j].id,&Entities[j].angleY,&Entities[j].pos.X,&Entities[j].pos.Y,&Entities[j].pos.Z);
		fprintf(Destfile,"%d %d %f %f %f\n",Entities[j].id,Entities[j].angleY,Entities[j].pos.X,Entities[j].pos.Y,Entities[j].pos.Z);
	}

	fscanf(file,"<TEXTURES> %d\n",&Nb_textures);
	fprintf(Destfile,"<TEXTURES> %d\n",Nb_textures);
	for(k=0 ; k<Nb_textures ; k++)
	{
		fscanf(file,"%s\n",temp);
		fprintf(Destfile ,"%s\n",temp);
	}

	fscanf(file,"<LIGHTS> %d\n",&Nb_Lights);
	fprintf(Destfile,"<LIGHTS> %d\n",Nb_Lights);
	for(k=0 ; k<Nb_Lights ; k++)
	{
		fscanf(file,"%f %f %f %d %d %d %f %d\n",
							&pos.X,
							&pos.Y,
							&pos.Z,
							&R,
							&G,
							&B,
							&I,
							&type);
		fprintf(Destfile,"%f %f %f %d %d %d %f %d\n",
							pos.X,
							pos.Y,
							pos.Z,
							R,
							G,
							B,
							I,
							type);
	}

	fscanf(file,"<LIGHTMAPS> %d\n",&i);
	fprintf(Destfile,"<LIGHTMAPS> %d\n",i);
	for(j=0 ; j<i ; j++)
	{
		fscanf(file,"%s\n",temp);
		fprintf(Destfile,"%s\n",temp);
	}

	fscanf(file,"<NODES> %d\n",&Nb_nodes);
	fprintf(Destfile,"<NODES> %d\n",Nb_nodes);
	for(i=0 ; i<Nb_nodes ; i++)
	{
		fscanf(file,"NODE[%d] LEAF[%d] FNODE[%d] BNODE[%d] POP[%f %f %f] VN[%f %f %f]\n",
			&bidon,
			&bidon2,
			&bidon3,
			&bidon4,
			&bidon_f,
			&bidon_f2,
			&bidon_f3,
			&bidon_f4,
			&bidon_f5,
			&bidon_f6);
		fprintf(Destfile,"NODE[%d] LEAF[%d] FNODE[%d] BNODE[%d] POP[%f %f %f] VN[%f %f %f]\n",
			bidon,
			bidon2,
			bidon3,
			bidon4,
			bidon_f,
			bidon_f2,
			bidon_f3,
			bidon_f4,
			bidon_f5,
			bidon_f6);
	}

	fscanf(file,"<FACES> %d\n",&Nb_faces);
	fprintf(Destfile,"<FACES> %d\n",Nb_faces);
	printf("<FACES> %d\n",Nb_faces); //
	for(k=0 ; k<Nb_faces ; k++)
	{
			fscanf(file,"%d %d %d %d %d\n",&bidon5,&l,&bidon,&bidon2,&bidon3);
			fprintf(Destfile,"%d %d %d %d %d\n",bidon5,l,bidon,bidon2,bidon3);

			for(i=0 ; i<l ; i++)
			{
				fscanf(file,"%f %f %f %f %f %f %f\n",
						&bidon_f,
						&bidon_f2,
						&bidon_f3,
						&bidon_f4,
						&bidon_f5,
						&bidon_f6,
						&bidon_f7);

				fprintf(Destfile,"%f %f %f %f %f %f %f\n",
						bidon_f,
						bidon_f2,
						bidon_f3,
						bidon_f4,
						bidon_f5,
						bidon_f6,
						bidon_f7);
			}
	}

	fscanf(file,"<LEAFS> %d\n",&Nb_leafs);
	fprintf(Destfile,"<LEAFS> %d\n",Nb_leafs);
	for(k=0 ; k<Nb_leafs ; k++)
	{
		fscanf(file,"%d ",&Id);
		fprintf(Destfile,"%d ",Id);

		LeafArray[k].SortedID = Id;

		fscanf(file,"%d %d\n",&LeafArray[Id].Nb_faces,&LeafArray[Id].PVSIndex);
		fprintf(Destfile,"%d %d\n",LeafArray[Id].Nb_faces,LeafArray[Id].PVSIndex);

		fscanf(file,"%f %f %f %f %f %f\n",
								&LeafArray[Id].bbox.BoxMin.X,
								&LeafArray[Id].bbox.BoxMin.Y,
								&LeafArray[Id].bbox.BoxMin.Z,
								&LeafArray[Id].bbox.BoxMax.X,
								&LeafArray[Id].bbox.BoxMax.Y,
								&LeafArray[Id].bbox.BoxMax.Z);

		fprintf(Destfile,"%f %f %f %f %f %f\n",
								LeafArray[Id].bbox.BoxMin.X,
								LeafArray[Id].bbox.BoxMin.Y,
								LeafArray[Id].bbox.BoxMin.Z,
								LeafArray[Id].bbox.BoxMax.X,
								LeafArray[Id].bbox.BoxMax.Y,
								LeafArray[Id].bbox.BoxMax.Z);

		for(m=0 ; m<LeafArray[Id].Nb_faces ; m++)
		{
			fscanf(file,"%d ",&bidon);
			fprintf(Destfile,"%d ",bidon);
			/*
			fscanf(file,"%d %d %d %d\n",&l,&bidon,&bidon2,&bidon3);
			fprintf(Destfile,"%d %d %d %d\n",l,bidon,bidon2,bidon3);

			for(i=0 ; i<l ; i++)
			{
				fscanf(file,"%f %f %f %f %f %f %f\n",
						&bidon_f,
						&bidon_f2,
						&bidon_f3,
						&bidon_f4,
						&bidon_f5,
						&bidon_f6,
						&bidon_f7);

				fprintf(Destfile,"%f %f %f %f %f %f %f\n",
						bidon_f,
						bidon_f2,
						bidon_f3,
						bidon_f4,
						bidon_f5,
						bidon_f6,
						bidon_f7);
			}
			*/
		}
		fscanf(file,"\n");
		fprintf(Destfile,"\n");
	}

	fscanf(file,"<ACTION BRUSH> %d\n",&Nb_ActionBrush);
	fprintf(Destfile,"<ACTION BRUSH> %d\n",Nb_ActionBrush);
	for(i=0 ; i<Nb_ActionBrush ; i++)
	{
		fscanf(file,"ActionBrush %d faces %d\n",&bidon,&bidon2);
		fprintf(Destfile,"ActionBrush %d faces %d\n",bidon,bidon2);
		for(j=0 ; j<bidon2 ; j++)
		//for(pCurr=ActionBrushTab[i].ListFace ; pCurr ; pCurr=pCurr->Next)
		{
			fscanf(file,"%d %d\n", &bidon3, &bidon4);
			fprintf(Destfile,"%d %d\n", bidon3, bidon4);
			for(k=0 ; k<bidon3 ; k++)
			{
				fscanf(file,"%f %f %f %f %f\n",
								&bidon_f,
								&bidon_f2,
								&bidon_f3,
								&bidon_f4,
								&bidon_f5);

				fprintf(Destfile,"%f %f %f %f %f\n",
								bidon_f,
								bidon_f2,
								bidon_f3,
								bidon_f4,
								bidon_f5);
			}
		}
	}

	Nb_portals=0;

	fscanf(file,"<PORTALS> %d\n",&Nb_portals);
	fprintf(Destfile,"<PORTALS> %d\n",Nb_portals);

	for(i=0 ; i<Nb_portals ; i++)
	{
		fscanf(file,"%d\n",&Owner1);
		fprintf(Destfile,"%d\n",Owner1);
		fscanf(file,"%d\n",&Owner2);
		fprintf(Destfile,"%d\n",Owner2);
		fscanf(file,"%d\n",&nVertices);
		fprintf(Destfile,"%d\n",nVertices);

		Portal = malloc(sizeof(RECTANGLE));
		memset(Portal,0,sizeof(RECTANGLE));
		Portal->point = malloc(nVertices*sizeof(point_t));
		Portal->Next = NULL;

		Portal->NumberOfVertices = nVertices;
		Portal->NumberOfLeaf = 2;
		Portal->LeafOwnerArray[0] = Owner1;
		Portal->LeafOwnerArray[1] = Owner2;

		fscanf(file,"%f %f %f %f %f %f\n",
					&Portal->Plan.VecteurNormal.X,
					&Portal->Plan.VecteurNormal.Y,
					&Portal->Plan.VecteurNormal.Z,
					&Portal->Plan.PointOnPlane.X,
					&Portal->Plan.PointOnPlane.Y,
					&Portal->Plan.PointOnPlane.Z);

		fprintf(Destfile,"%f %f %f %f %f %f\n",
					Portal->Plan.VecteurNormal.X,
					Portal->Plan.VecteurNormal.Y,
					Portal->Plan.VecteurNormal.Z,
					Portal->Plan.PointOnPlane.X,
					Portal->Plan.PointOnPlane.Y,
					Portal->Plan.PointOnPlane.Z);

		for(j=0 ; j<Portal->NumberOfVertices ; j++)
		{
			fscanf(file,"PORTAL %d : %f %f %f\n",
				&n,
				&Portal->point[j].X,
				&Portal->point[j].Y,
				&Portal->point[j].Z);

			fprintf(Destfile,"PORTAL %d : %f %f %f\n",
				n,
				Portal->point[j].X,
				Portal->point[j].Y,
				Portal->point[j].Z);
		}
		PortalArray[i] = Portal;
		Portal->Next = NULL;
	}

	fclose(file);


	for(i=0 ; i<Nb_leafs ; i++)
	{
		LeafArray[i].NumberOfPortals = 0;
	}
	for(i=0 ; i<Nb_portals ; i++)
	{
		Portal = PortalArray[i];
		leaf1 = LeafArray[ Portal->LeafOwnerArray[0] ].SortedID;
		leaf2 = LeafArray[ Portal->LeafOwnerArray[1] ].SortedID;

		Portal->LeafOwnerArray[0] = leaf1;
		Portal->LeafOwnerArray[1] = leaf2;
	}
	for(i=0 ; i<Nb_portals ; i++)
	{
		Portal = PortalArray[i];
		
		leaf1 = Portal->LeafOwnerArray[0];
		leaf2 = Portal->LeafOwnerArray[1];

		LeafArray[leaf1].PortalIndexList[LeafArray[leaf1].NumberOfPortals] = i;
		LeafArray[leaf2].PortalIndexList[LeafArray[leaf2].NumberOfPortals] = i;

		LeafArray[leaf1].NumberOfPortals++;
		LeafArray[leaf2].NumberOfPortals++;

		LeafArray[leaf1].origin.X = (LeafArray[leaf1].bbox.BoxMin.X + LeafArray[leaf1].bbox.BoxMax.X)/2.0f;
		LeafArray[leaf1].origin.Y = (LeafArray[leaf1].bbox.BoxMin.Y + LeafArray[leaf1].bbox.BoxMax.Y)/2.0f;
		LeafArray[leaf1].origin.Z = (LeafArray[leaf1].bbox.BoxMin.Z + LeafArray[leaf1].bbox.BoxMax.Z)/2.0f;
		LeafArray[leaf2].origin.X = (LeafArray[leaf2].bbox.BoxMin.X + LeafArray[leaf2].bbox.BoxMax.X)/2.0f;
		LeafArray[leaf2].origin.Y = (LeafArray[leaf2].bbox.BoxMin.Y + LeafArray[leaf2].bbox.BoxMax.Y)/2.0f;
		LeafArray[leaf2].origin.Z = (LeafArray[leaf2].bbox.BoxMin.Z + LeafArray[leaf2].bbox.BoxMax.Z)/2.0f;
	}

	fclose(Destfile);
	return true;
}