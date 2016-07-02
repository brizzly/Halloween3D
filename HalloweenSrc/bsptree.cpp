
#include "alloween.cpp"


int	LastUsedPVS = -1;



int bsp_FindCamera(vect_t Pos)
{
	if(MapDataFormat != MAP_BSP)
		return -1;
	return bsp_FindCamera_rec(0,Pos);
}

int bsp_FindCamera_rec(int Node, vect_t Pos)
{
	int	res;

//	if(Node == -1 || Node == -2 || Node >= gNb_nodes)
	if(Node < 0 || Node >= gNb_nodes)
	{
		if(developer.value)
			m_ConsPrint("Node error\n");
		return -1;
	}

	if(NodeArray[Node].Feuille != -1)
		return NodeArray[Node].Feuille;

	res = ClassifyVect(&Pos,&NodeArray[Node].Plan);
	if(res == DEVANT || res == CONFONDU)
	{
		if(NodeArray[Node].Front == EMPTY_SPACE)
		{
			//m_ConsPrint("Empty Space\n");
			return -1;
		}
		return bsp_FindCamera_rec(NodeArray[Node].Front,Pos);
	}
	if(NodeArray[Node].Back == SOLID_SPACE)
	{
		//m_ConsPrint("Solid Space\n");
		return -1;
	}
	return bsp_FindCamera_rec(NodeArray[Node].Back,Pos);
}

vect_t bsp_CorrectSide_rec(int Node, CTRACE *trace, vect_t Pos, vect_t Dest)
{
	int		resF;
	int		resB;
	vect_t	dir;
	float	val;

	if(Node == -1 || Node == -2 || Node >= gNb_nodes)
	{
		m_ConsPrint("Node Error\n");
		return Pos;
	}

	if(NodeArray[Node].Feuille != -1)
		return Pos;

	resF = ClassifyVect(&Dest,&NodeArray[Node].Plan);
	if(resF == DEVANT || resF == CONFONDU)
	{
		if(NodeArray[Node].Front == EMPTY_SPACE)
			return Pos;
		return bsp_CorrectSide_rec(NodeArray[Node].Front,trace,Pos,Dest);
	}
	else
	{
		resB = ClassifyVect(&Pos,&NodeArray[Node].Plan);
		if(resB == DEVANT)
		{
			trace->Plan = NodeArray[Node].Plan;
			if(Intersect_Plan(&Pos,&Dest,&trace->Plan,&trace->impact,&val))
				trace->IsPlan = true;
		}
	}

	if(NodeArray[Node].Back == SOLID_SPACE)
	{
		if(trace->IsPlan)
		{
			Pos = trace->impact;
			dir = trace->Plan.VecteurNormal;
			m_Vector_Scale(&dir,-epsilon.value);
			Pos = m_Vector_Sub(Pos,dir);
			m_ConsPrint("Solid Space : Corrected\n");
		}
		else
			m_ConsPrint("Can't Corret : no plan\n");
		return Pos;
	}
	return bsp_CorrectSide_rec(NodeArray[Node].Back,trace,Pos,Dest);
}

vect_t bsp_CorrectSide(vect_t Pos, vect_t Dest, CTRACE	ctrace)
{
	if(MapDataFormat != MAP_BSP)
		return Pos;
	ctrace.IsPlan = false;
	return bsp_CorrectSide_rec(0,&ctrace,Pos,Dest);
}


void bsp_DrawMap(Camera_t *pCam)
{
	int		drawed;
	int		portal_drawed;

	if(MapDataFormat == MAP_POLYGONS)
	{
		//bsp_DrawLeaves(PolyList, pCam->Pos);
		bsp_DrawTabFaces();
	}
	else if(MapDataFormat == MAP_BSP)
	{
		gLeafLocation = bsp_FindCamera(pCam->Pos);
		drawed = bsp_DrawPVS(gLeafLocation,pCam);

	//	if(drawed==0)
	//		g_DispString(14,"Out of Leaf");
		if(draw_portal.value)
		{
			portal_drawed = bsp_DrawPORTALS(gLeafLocation); // -1 pour tout afficher
			g_DispString(12,"portal_drawed: %d",portal_drawed);
		}
	//	if(developer.value)
	//	{
	//		g_DispString(14,"Drawed Leaf: %d",drawed);
	//	}
	}
}

int bsp_DrawPVS(int leaf, const Camera_t *pHero)
{
	int		i, pvs_index, count=0;

	if(leaf>=0)
		LastUsedPVS = leaf;
	else
		leaf = LastUsedPVS;

	gLeafLocation = leaf;

	if(leaf<0)
		return 0;

	if(draw_one.value)
	{
			bsp_DrawLeaves(LeafArray[leaf].faceliste, pHero->Pos);
			count++;
		//	if(developer.value)
		//		g_DispString(14,"LeafID: %d",leaf);

		return count;
	}

	pvs_index = LeafArray[leaf].PVSIndex;
	for(i=0 ; i<gNb_Leafs ; i++)
	{
		if(PVSData[pvs_index+i] != '1')
			continue;
		if(frustrum_LeafIn(gFrustrumPlanes,i) == false)
			continue;
		bsp_DrawLeaves(LeafArray[i].faceliste, pHero->Pos);
		count++;
	}

	/*
	if(draw_tris.value)
	{
		pvs_index = LeafArray[leaf].PVSIndex;
		for(i=0 ; i<gNb_Leafs ; i++)
		{
			if(PVSData[pvs_index+i] != '1')
				continue;
			bsp_DrawLeaves(LeafArray[i].faceliste, pHero->Pos);
			count++;
		}
	}
	*/

	return count;
}

void bsp_GetNL_rec(int recdepth, NL_t *pNL, int centerLeaf, vect_t pos, float radius)
{
	int			i;
	int			j;
	int			portalID;
	portal_t	Portal;
	float		val;
	int			NearLeaf;

//	if(centerLeaf < 0 || centerLeaf >= gNb_Leafs)
//		return;
	if(recdepth > 5) //32)
	{
		m_ConsPrint("rec_depth overflow\n");
		return;
	}

	for(i=0 ; i<LeafArray[centerLeaf].Nb_portals ; i++)
	{
		portalID = LeafArray[centerLeaf].portalIndex[i];
		Portal = PortalArray[portalID];
		val = A_absf(m_DistancePlan(Portal.Plan,pos));
		if(val > radius)
			continue;
		NearLeaf = Portal.LeafOwnerArray[0];
		if(NearLeaf == centerLeaf)
			NearLeaf = Portal.LeafOwnerArray[1];
		for(j=0 ; j<pNL->NbNL ; j++)
		{
			if(pNL->NLData[j] == NearLeaf)
				goto label_02;
		}
		pNL->NLData[pNL->NbNL++] = NearLeaf;
		bsp_GetNL_rec(recdepth+1,pNL,NearLeaf,pos,radius);
label_02:;
	}
}

void bsp_GetNL(NL_t *pNL, int leaf, float radius, vect_t pos)
{
	if(!pNL)
		return;

	pNL->NbNL = 0;
	pNL->NLData[pNL->NbNL++] = leaf;
	bsp_GetNL_rec(0,pNL,leaf,pos,radius);
}

int bsp_DrawPORTALS(int mode)
{
	int			i, j, count;
	int			portalID;
	Render_t	rend;

	rend.is_texture = false;
	rend.is_blend = true;
	rend.blend_src_mode = GL_SRC_ALPHA;
	rend.blend_dst_mode = GL_ONE_MINUS_SRC_ALPHA;
	rend.is_color = true;
	rend.color.R = 1;
	rend.color.G = 0;
	rend.color.B = 0;
	rend.begin_mode = RENDER_LINELOOP;
	rend.is_depthtest = false;
	rend.is_depthmask = true;
	rend.is_draw2D = false;

	count = 0;
	if(mode == -1)
	{
		for(i=0 ; i<gNb_Portals ; i++)
		{
			count++;

			rend.nvert = PortalArray[i].NumberOfVertices;
			for(j=0 ; j<rend.nvert ; j++)
			{
				rend.xyz[j][0] = PortalArray[i].Vertice[j].X;
				rend.xyz[j][1] = PortalArray[i].Vertice[j].Y;
				rend.xyz[j][2] = PortalArray[i].Vertice[j].Z;
			}
			gl_RenderPoly(rend);
		}
	}
	else
	{
		for(i=0 ; i<LeafArray[mode].Nb_portals ; i++)
		{
			portalID = LeafArray[mode].portalIndex[i];
			count++;
			
			rend.nvert = PortalArray[portalID].NumberOfVertices;
			for(j=0 ; j<rend.nvert ; j++)
			{
				rend.xyz[j][0] = PortalArray[i].Vertice[j].X;
				rend.xyz[j][1] = PortalArray[i].Vertice[j].Y;
				rend.xyz[j][2] = PortalArray[i].Vertice[j].Z;
			}
			gl_RenderPoly(rend);
		}
	}
	return count;
}

void bsp_DrawLeaves(Face_t *FaceList, vect_t pos)
{
	if(gIsServer && net_dedicated.value)
		return;

	gl_DrawFaces(FaceList,pos);

	if(draw_normal.value)
		gl_DrawVectors(FaceList);
}

void bsp_DrawLeaves_2(int id_face, vect_t pos)
{
	Face_t Face;

	if(gIsServer && net_dedicated.value)
		return;

	Face = FaceArray[id_face];
	gl_DrawFaces(&Face,pos);

	if(draw_normal.value)
		gl_DrawVectors(&Face);
}

void bsp_DrawTabFaces()
{
	int		i;
	int		j;
	
	for(i=0 ; i<nbTabFaces ; i++)
	{
		j = gTabFaces[i].id;
	
		if(!FastCull(gTabFaces[j].FacePtr, gCamera.Pos))
			continue;				
		
		disp_Face(gTabFaces[j].FacePtr);
	}
}


