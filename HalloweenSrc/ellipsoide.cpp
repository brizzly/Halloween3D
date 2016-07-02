
#include "alloween.cpp"


vect_t	VeryLastPos;


void col_Handle(vect_t *pos, vect_t nextpos, CTRACE *trace)
{
	TCollisionPacket	collision;
	vect_t	dir;
	vect_t	tmp;
	float	MaxRadius;
	int		ent_leaf;

	if(noclip_wall.value)
	{
		*pos = nextpos;
		return;
	}

	VeryLastPos = *pos;
	collision.nPass = 0;
	collision.eRadius = trace->ellipsoid;
	collision.Ent = trace->Ent;
	collision.lastSafePosition = *pos;
	collision.stuck = false;
	collision.EmergencyQuit = false;
	if(trace->NoRadius)
	{
		collision.eRadius.X = 1.0f;
		collision.eRadius.Y = 1.0f;
		collision.eRadius.Z = 1.0f;
	}
	dir.X = nextpos.X - pos->X;
	dir.Y = nextpos.Y - pos->Y;
	dir.Z = nextpos.Z - pos->Z;

	if(MapDataFormat == MAP_BSP)
	{
		if(!trace->NoRadius)
		{
			MaxRadius = o_GetMaxRadius(trace->ellipsoid) + (float)Norme(dir);
			ent_leaf = 	bsp_FindCamera(*pos);
			if(ent_leaf < 0)
			{
				trace->solidspace = true;
				if(developer.value)
					m_ConsPrint("Solid Leaf Error\n");
				if(ghost.value)
				{
					trace->col = false;
					*pos = nextpos;
					trace->impact = *pos;
					return;
				}
				else
				{
					tmp = nextpos;
					goto ok;
				}
			}
			trace->solidspace = false;
			bsp_GetNL(&NLlist, ent_leaf, MaxRadius, collision.lastSafePosition);
		}
		else
		{
			NLlist.NbNL = 1;
			NLlist.NLData[0] = bsp_FindCamera(*pos);
		}
	}
	else if(MapDataFormat == MAP_POLYGONS)
	{
		trace->solidspace = false;
	}

	tmp = GetPosition(&collision, *pos, dir, trace);

	if(collision.stuck == false || trace->DontUnstuck)
	{
		*pos = tmp;
	}
	else
	{
		col_Unstuck(&collision, trace, pos, nextpos);		
	}

ok:
	trace->impact = *pos;

//	if(developer.value)
//		g_DispString(14,"pass: %d",collision.nPass);
}

void col_Unstuck(TCollisionPacket *collision, CTRACE *trace, vect_t *pos, vect_t nextpos)
{
	vect_t	org;
	vect_t	tmp;
	vect_t	dir;
	int		i;
	int		j;
	int		y;

	if(developer.value)
		m_ConsPrint("Stuck (%d)\n", rand()%1000);

	org = *pos;
//	for(i=-1 ; i <= 1 ; i++)
//	for(j=-1 ; j <= 1 ; j++)
//	for(y=0 ; y<10 ; y+=2)
	i = j = 0;
//	for(y=1 ; y<6 ; y=y+2)
	y = 2 + (rand() % 2);
//	i = rand() % 4;
//	j = rand() % 4;
	{
		tmp.X = org.X + i;
		tmp.Y = org.Y + y;
		tmp.Z = org.Z + j;
		collision->stuck = false;
		dir.X = nextpos.X - tmp.X;
		dir.Y = nextpos.Y - tmp.Y;
		dir.Z = nextpos.Z - tmp.Z;
		tmp = GetPosition(collision, tmp, dir, trace);
		if(collision->stuck == false)
		{
			*pos = tmp;
			if(developer.value)
				m_ConsPrint("Unstuck\n");
		}
	}
}

vect_t GetPosition(TCollisionPacket *collision, vect_t position, vect_t velocity, CTRACE *trace)
{
	vect_t scaledPosition, scaledVelocity;
	vect_t finalPosition;

	// the first thing we do is scale the player and his velocity to ellipsoid space
	scaledPosition.X = position.X / collision->eRadius.X;
	scaledPosition.Y = position.Y / collision->eRadius.Y;
	scaledPosition.Z = position.Z / collision->eRadius.Z;

	scaledVelocity.X = velocity.X / collision->eRadius.X;
	scaledVelocity.Y = velocity.Y / collision->eRadius.Y;
	scaledVelocity.Z = velocity.Z / collision->eRadius.Z;

	// call the recursive collision response function	
	finalPosition = collideWithWorld(collision, scaledPosition, scaledVelocity, trace);
 
	if(collision->EmergencyQuit)
		return finalPosition;

	// when the function returns the result is still in ellipsoid space, so
	// we have to scale it back to R3 before we return it
	finalPosition.X *= collision->eRadius.X;
	finalPosition.Y *= collision->eRadius.Y;
	finalPosition.Z *= collision->eRadius.Z;
  
	return finalPosition;
}

vect_t collideWithWorld(TCollisionPacket *collision, vect_t position, vect_t velocity, CTRACE *trace)
{
	vect_t	destinationPoint;
	double	l2;
	vect_t	V;
    vect_t	newSourcePoint;
	vect_t	slidePlaneOrigin ;
	vect_t	slidePlaneNormal;
    vect_t	newDestinationPoint;
	vect_t	newVelocityVector;
	int		i;
	int		CurrentLeaf;
	int		SaveStuckInfo;
	float	dist;
//	float	l;


//	if(lengthOfVector(velocity) < EPSILON_COL)
//		return position; //m_Vector_Add(position,velocity); //position;

	if(collision->EmergencyQuit)
		return VeryLastPos;

	// limite la recusion pour eviter un gros plantage
	collision->nPass++;
	if(collision->nPass > 4) // 6
	{
	//	if(developer.value)
		m_ConsPrint("recusive error\n");
//		collision->stuck = true;

		/*
		if(collision->foundCollision)
		{
			V = velocity;
			setLength(&V, (float)collision->nearestDistance-EPSILON_COL);
			return m_Vector_Add(position, V); //collision->sourcePoint;
		}
		else
		{
			return position;
		}
		*/

		if(trace->Ent == NULL)
			return position;

	//	if(!gIsMultiplayer)
	//	{
//			if(trace->Ent->entity_id != PERSO)
//				return position;
	//	}

		collision->stuck = false;
		collision->EmergencyQuit = true;
		return VeryLastPos;
	}

	destinationPoint = m_Vector_Add(position, velocity);
	collision->velocity = velocity;
	collision->sourcePoint = position;
	collision->foundCollision = false;
	collision->nearestDistance = -1;

	if(MapDataFormat == MAP_POLYGONS)
	{
		CheckCollision(collision,PolyList,NULL,NULL);
	}
//	else if(MapDataFormat == MAP_POLYGONS_OPT)
//	{
//		CheckCollision_2(collision,NULL,NULL);
//	}
	else if(MapDataFormat == MAP_BSP)
	{
		for(i=0 ; i<NLlist.NbNL ; i++)
		{
			CurrentLeaf = NLlist.NLData[i];
			CheckCollision(collision,LeafArray[CurrentLeaf].faceliste,NULL,NULL);
		}

		// collision avec les actionbrush
		if(!noscript.value && player_view.value && !gIsMultiplayer)
		{
			for(i=0 ; i<gNb_ActionBrush ; i++)
			{
				dist = A_absf(m_Distance(ActionBrushTab[i].script.Pos, VeryLastPos/*collision->lastSafePosition*/));
				if(trace->Ent)
				{
					if(trace->Ent->entity_id == PERSO)
					{
						if(dist > 400.0f) // 1000
							continue;
					}
					else
					{
						if(dist > 150.0f) // 300
							continue;
					}
				}

				//m_ConsPrint("%f\n",dist);				
				SaveStuckInfo = collision->stuck;
				CheckCollision(collision,ActionBrushTab[i].FaceList,&ActionBrushTab[i].script.Pos,&ActionBrushTab[i].script.Scale);
				collision->stuck = SaveStuckInfo;
			}
		}
	}
  
	if(collision->foundCollision == false)
	{
//		if(trace->Ent)
//			if(trace->Ent->entity_id != PERSO)
//				m_ConsPrint("debug - nocol\n");
		trace->col = false;
	//	l = lengthOfVector(velocity); //
		//V = velocity;
	//	setLength(&V, l-EPSILON_COL); //
     	return destinationPoint; // m_Vector_Add(position, V);
	}
	else
	{
	//	if(trace->Ent)
	//		if(trace->Ent->entity_id != PERSO)
	//			m_ConsPrint("debug - col\n");

		if(trace->IsPlan == false)
		{
			trace->IsPlan = true;
			trace->Plan = collision->Plan;
		}

		trace->col = true;
		trace->type = collision->PolygonType;
		if(trace->Response == false)
		{
			V = velocity;
			setLength(&V, (float)collision->nearestDistance-EPSILON_COL);
			return m_Vector_Add(position, V); //collision->sourcePoint;
		}

		if(collision->stuck)
		{
		//	position = collision->lastSafePosition;
		//	velocity = collision->lastSafeVelocity;
		//	collision->stuck = false;
			m_ConsPrint("stuck (!)\n");
			return position;
		}
		else
			collision->lastSafePosition = position;
		
		if(collision->nearestDistance >= 0.05f) //0.10   0.05f )
		{
			V = velocity;
			setLength(&V, (float)collision->nearestDistance-0.05f); // 0.10   0.05f
			newSourcePoint = m_Vector_Add(collision->sourcePoint,V);
		}
		else
			newSourcePoint = collision->sourcePoint;

		slidePlaneOrigin = collision->nearestPolygonIntersectionPoint;
		slidePlaneNormal = m_Vector_Sub(newSourcePoint,collision->nearestPolygonIntersectionPoint);

		l2 = intersectRayPlane(destinationPoint, slidePlaneNormal, slidePlaneOrigin, slidePlaneNormal); 

		newDestinationPoint.X = destinationPoint.X + ((float)l2 * slidePlaneNormal.X);
		newDestinationPoint.Y = destinationPoint.Y + ((float)l2 * slidePlaneNormal.Y);
		newDestinationPoint.Z = destinationPoint.Z + ((float)l2 * slidePlaneNormal.Z);

		newVelocityVector = m_Vector_Sub(newDestinationPoint,collision->nearestPolygonIntersectionPoint);
        
		return collideWithWorld(collision, newSourcePoint, newVelocityVector, trace); 
	}
}

void CheckCollision(TCollisionPacket* colPackage, pFace FaceList, vect_t *pos_offset, vect_t *siz_offset)
{
	pFace	Face;
	vect_t	posOffset = {0, 0, 0};
	vect_t	sizOffset = {1, 1, 1};
	vect_t	p1, p2, p3;
	vect_t	pNormal;
	vect_t	pOrigin;
	vect_t	source;
	vect_t	eRadius;
	vect_t	Inv_eRadius;
	vect_t	velocity;
	vect_t	normalizedVelocity;
	vect_t	InvNormalizedVelocity;
	vect_t	sIPoint;
	vect_t	pIPoint;
	vect_t	polyIPoint;
	double	distanceToTravel;
	double	distToPlaneIntersection;
	double	distToEllipsoidIntersection;
	dword	pClass;
	//vect_t	cheat_hauteur = {0, 1, 0};

	if(pos_offset)
		posOffset = *pos_offset;
	if(siz_offset)
		sizOffset = *siz_offset;

	source = colPackage->sourcePoint;
	eRadius = colPackage->eRadius;
	velocity = colPackage->velocity;	
 	normalizedVelocity = velocity;
	normalizeVector(&normalizedVelocity);
 	distanceToTravel = lengthOfVector(velocity);
	
	Inv_eRadius.X = 1.0f / eRadius.X;
	Inv_eRadius.Y = 1.0f / eRadius.Y;
	Inv_eRadius.Z = 1.0f / eRadius.Z;
   
	for(Face=FaceList ; Face ; Face=Face->Next)
	{
	//	if(	Face->Type == DETAIL)
	//		continue;

		/*
		// HACK NOCLIP WALL ENOOOORME
		if(colPackage->Ent)
		if(ghost.value && colPackage->Ent->entity_id == PERSO)
		{
			if(PdtScalaire(&cheat_hauteur,&Face->Plan.VecteurNormal) == 0)
				continue;
		}
		*/

		p1.X = ((Face->Vertice[0].X * sizOffset.X) + posOffset.X) * Inv_eRadius.X;
		p1.Y = ((Face->Vertice[0].Y * sizOffset.Y) + posOffset.Y) * Inv_eRadius.Y;
		p1.Z = ((Face->Vertice[0].Z * sizOffset.Z) + posOffset.Z) * Inv_eRadius.Z;

		p2.X = ((Face->Vertice[1].X * sizOffset.X) + posOffset.X) * Inv_eRadius.X;
		p2.Y = ((Face->Vertice[1].Y * sizOffset.Y) + posOffset.Y) * Inv_eRadius.Y;
		p2.Z = ((Face->Vertice[1].Z * sizOffset.Z) + posOffset.Z) * Inv_eRadius.Z;

		p3.X = ((Face->Vertice[2].X * sizOffset.X) + posOffset.X) * Inv_eRadius.X;
		p3.Y = ((Face->Vertice[2].Y * sizOffset.Y) + posOffset.Y) * Inv_eRadius.Y;
		p3.Z = ((Face->Vertice[2].Z * sizOffset.Z) + posOffset.Z) * Inv_eRadius.Z;

		pNormal = m_GetFaceNormal_vect(p1,p2,p3);
		pOrigin = p1;

		if(dot(pNormal, normalizedVelocity) > 0)
		{
			/*
			if(Face->Type == TRANS)
			{
				pX = p3;
				p3 = p2;
				p2 = pX;
				pNormal = m_GetFaceNormal_vect(p1,p2,p3);
			}
			else
			*/
			{
				continue;
			}
		}
		
   		// calculate sphere intersection point
		sIPoint = m_Vector_Sub(source, pNormal);
       
   		// classify point to determine if ellipsoid span the plane
   		pClass = classifyPoint(sIPoint, pOrigin, pNormal);
         
   		// find the plane intersection point
   		if(pClass == PLANE_BACKSIDE)	// plane is embedded in ellipsoid
		{ 
     		// find plane intersection point by shooting a ray from the 
      		// sphere intersection point along the planes normal.
      		distToPlaneIntersection = intersectRayPlane(sIPoint, pNormal, pOrigin, pNormal);

      		// calculate plane intersection point
      		pIPoint.X = sIPoint.X + (float)distToPlaneIntersection * pNormal.X; 
      		pIPoint.Y = sIPoint.Y + (float)distToPlaneIntersection * pNormal.Y; 
      		pIPoint.Z = sIPoint.Z + (float)distToPlaneIntersection * pNormal.Z;
     	} 
     	else
		{ 
     		// shoot ray along the velocity vector
     		distToPlaneIntersection = intersectRayPlane(sIPoint, normalizedVelocity, pOrigin, pNormal);
              
     		// calculate plane intersection point
     		pIPoint.X = sIPoint.X + (float)distToPlaneIntersection * normalizedVelocity.X; 
     		pIPoint.Y = sIPoint.Y + (float)distToPlaneIntersection * normalizedVelocity.Y; 
     		pIPoint.Z = sIPoint.Z + (float)distToPlaneIntersection * normalizedVelocity.Z;
     	}

     	// find polygon intersection point. By default we assume its equal to the plane intersection point
		polyIPoint = pIPoint;
     	distToEllipsoidIntersection = distToPlaneIntersection;
     
		if(m_pointInTri(pNormal,p1,p2,p3,pIPoint) == false)
		{ 
      		polyIPoint = closestPointOnTriangle(p1, p2, p3, pIPoint);
			InvNormalizedVelocity = normalizedVelocity;
			m_Vector_Scale(&InvNormalizedVelocity,-1);
      		distToEllipsoidIntersection = intersectRaySphere(polyIPoint, InvNormalizedVelocity, source, UNIT_SPHERE);
      		if(distToEllipsoidIntersection > 0)
			{ 	
     			// calculate true sphere intersection point
     			sIPoint.X = polyIPoint.X + ((float)distToEllipsoidIntersection * InvNormalizedVelocity.X);
     			sIPoint.Y = polyIPoint.Y + ((float)distToEllipsoidIntersection * InvNormalizedVelocity.Y);
     			sIPoint.Z = polyIPoint.Z + ((float)distToEllipsoidIntersection * InvNormalizedVelocity.Z);
     		}
     	} 

		// Here we do the error checking to see if we got ourself stuck last frame
		if(CheckPointInSphere(polyIPoint, source, UNIT_SPHERE))
		{
		//	UnstuckPointInSphere(&polyIPoint, source, UNIT_SPHERE); //
			colPackage->stuck = true;
		}
    
		// Ok, now we might update the collision data if we hit something
   		if((distToEllipsoidIntersection > 0) && (distToEllipsoidIntersection <= distanceToTravel))
		{ 
   			if(	colPackage->foundCollision == false ||
				distToEllipsoidIntersection < colPackage->nearestDistance )
			{
           		// if we are hit we have a closest hit so far. We save the information
   				colPackage->nearestDistance = distToEllipsoidIntersection;
   	       		colPackage->nearestIntersectionPoint = sIPoint;
   				colPackage->nearestPolygonIntersectionPoint = polyIPoint;
   				colPackage->foundCollision = true;
				colPackage->found_AB_collision = true; // for action brushs
				colPackage->PolygonType = Face->Type;
				colPackage->Plan = Face->Plan;
			}
   		}        
	}
}

/*
void CheckCollision_2(TCollisionPacket* colPackage, vect_t *pos_offset, vect_t *siz_offset)
{
	vect_t	posOffset={0,0,0};
	vect_t	sizOffset={1,1,1};
	vect_t	p1,p2,p3,pX;
	vect_t	pNormal;
	vect_t	pOrigin;
	vect_t	source;
	vect_t	eRadius;
	vect_t	Inv_eRadius;
	vect_t	velocity;
	vect_t	normalizedVelocity;
	vect_t	InvNormalizedVelocity;
	vect_t	sIPoint;
	vect_t	pIPoint;
	vect_t	polyIPoint;
	double	distanceToTravel;
	double	distToPlaneIntersection;
	double	distToEllipsoidIntersection;
	dword	pClass;
	vect_t	cheat_hauteur={0,1,0};
	int		i;

	if(pos_offset)
		posOffset = *pos_offset;
	if(siz_offset)
		sizOffset = *siz_offset;

	source = colPackage->sourcePoint;
	eRadius = colPackage->eRadius;
	velocity = colPackage->velocity;	
 	normalizedVelocity = velocity;
	normalizeVector(&normalizedVelocity);
 	distanceToTravel = lengthOfVector(velocity);
	
	Inv_eRadius.X = 1.0f / eRadius.X;
	Inv_eRadius.Y = 1.0f / eRadius.Y;
	Inv_eRadius.Z = 1.0f / eRadius.Z;
   
	for(i=0 ; i<gNbFaces ; i++)
	{
		if(	gFacesTab[i].FaceType == DETAIL ||
			gFacesTab[i].FaceType == FLAG)
			continue;

		p1.X = gPointsTab[gFacesTab[i].VertexID[0]].X * Inv_eRadius.X;
		p1.Y = gPointsTab[gFacesTab[i].VertexID[0]].Y * Inv_eRadius.Y;
		p1.Z = gPointsTab[gFacesTab[i].VertexID[0]].Z * Inv_eRadius.Z;

		p2.X = gPointsTab[gFacesTab[i].VertexID[1]].X * Inv_eRadius.X;
		p2.Y = gPointsTab[gFacesTab[i].VertexID[1]].Y * Inv_eRadius.Y;
		p2.Z = gPointsTab[gFacesTab[i].VertexID[1]].Z * Inv_eRadius.Z;

		p3.X = gPointsTab[gFacesTab[i].VertexID[2]].X * Inv_eRadius.X;
		p3.Y = gPointsTab[gFacesTab[i].VertexID[2]].Y * Inv_eRadius.Y;
		p3.Z = gPointsTab[gFacesTab[i].VertexID[2]].Z * Inv_eRadius.Z;


		pNormal = m_GetFaceNormal_vect(p1,p2,p3);
		pOrigin = p1;

		if(dot(pNormal, normalizedVelocity) > 0)
		{
			if(gFacesTab[i].FaceType == TRANS)
			{
				pX = p3;
				p3 = p2;
				p2 = pX;
				pNormal = m_GetFaceNormal_vect(p1,p2,p3);
			}
			else
			{
				continue;
			}
		}
		
   		// calculate sphere intersection point
		sIPoint = m_Vector_Sub(source, pNormal);
       
   		// classify point to determine if ellipsoid span the plane
   		pClass = classifyPoint(sIPoint, pOrigin, pNormal);
         
   		// find the plane intersection point
   		if(pClass == PLANE_BACKSIDE)	// plane is embedded in ellipsoid
		{ 
     		// find plane intersection point by shooting a ray from the 
      		// sphere intersection point along the planes normal.
      		distToPlaneIntersection = intersectRayPlane(sIPoint, pNormal, pOrigin, pNormal);

      		// calculate plane intersection point
      		pIPoint.X = sIPoint.X + (float)distToPlaneIntersection * pNormal.X; 
      		pIPoint.Y = sIPoint.Y + (float)distToPlaneIntersection * pNormal.Y; 
      		pIPoint.Z = sIPoint.Z + (float)distToPlaneIntersection * pNormal.Z;
     	} 
     	else
		{ 
     		// shoot ray along the velocity vector
     		distToPlaneIntersection = intersectRayPlane(sIPoint, normalizedVelocity, pOrigin, pNormal);
              
     		// calculate plane intersection point
     		pIPoint.X = sIPoint.X + (float)distToPlaneIntersection * normalizedVelocity.X; 
     		pIPoint.Y = sIPoint.Y + (float)distToPlaneIntersection * normalizedVelocity.Y; 
     		pIPoint.Z = sIPoint.Z + (float)distToPlaneIntersection * normalizedVelocity.Z;
     	}

     	// find polygon intersection point. By default we assume its equal to the plane intersection point
		polyIPoint = pIPoint;
     	distToEllipsoidIntersection = distToPlaneIntersection;
     
		if(m_pointInTri(pNormal,p1,p2,p3,pIPoint) == false)
		{ 
      		polyIPoint = closestPointOnTriangle(p1, p2, p3, pIPoint);
			InvNormalizedVelocity = normalizedVelocity;
			m_Vector_Scale(&InvNormalizedVelocity,-1);
      		distToEllipsoidIntersection = intersectRaySphere(polyIPoint, InvNormalizedVelocity, source, UNIT_SPHERE);
      		if(distToEllipsoidIntersection > 0)
			{ 	
     			// calculate true sphere intersection point
     			sIPoint.X = polyIPoint.X + ((float)distToEllipsoidIntersection * InvNormalizedVelocity.X);
     			sIPoint.Y = polyIPoint.Y + ((float)distToEllipsoidIntersection * InvNormalizedVelocity.Y);
     			sIPoint.Z = polyIPoint.Z + ((float)distToEllipsoidIntersection * InvNormalizedVelocity.Z);
     		}
     	} 

		// Here we do the error checking to see if we got ourself stuck last frame
		if(CheckPointInSphere(polyIPoint, source, UNIT_SPHERE))
		{
		//	UnstuckPointInSphere(&polyIPoint, source, UNIT_SPHERE); //
			colPackage->stuck = true;
		}
    
		// Ok, now we might update the collision data if we hit something
   		if((distToEllipsoidIntersection > 0) && (distToEllipsoidIntersection <= distanceToTravel))
		{ 
   			if(	colPackage->foundCollision == false ||
				distToEllipsoidIntersection < colPackage->nearestDistance )
			{
           		// if we are hit we have a closest hit so far. We save the information
   				colPackage->nearestDistance = distToEllipsoidIntersection;
   	       		colPackage->nearestIntersectionPoint = sIPoint;
   				colPackage->nearestPolygonIntersectionPoint = polyIPoint;
   				colPackage->foundCollision = true;
				colPackage->found_AB_collision = true; // for action brushs
				colPackage->PolygonType = gFacesTab[i].FaceType;
				colPackage->Plan.VecteurNormal = gNormalsTab[gFacesTab[i].NormalPlanID]; // Face->Plan;
				colPackage->Plan.PointOnPlane = gPointsTab[gFacesTab[i].VertexID[0]]; // Face->Plan;
			}
   		}        
	}
}
*/

// ----------------------------------------------------------------------
// Name  : intersectRayPlane()
// Input : rOrigin - origin of ray in world space
//         rVector - vector describing direction of ray in world space
//         pOrigin - Origin of plane 
//         pNormal - Normal to plane
// Notes : Normalized directional vectors expected
// Return: distance to plane in world units, -1 if no intersection.
// -----------------------------------------------------------------------  
double intersectRayPlane(vect_t rOrigin, vect_t rVector, vect_t pOrigin, vect_t pNormal)
{
	double d;
 	double numer;
	double denom;

	d = - (dot(pNormal,pOrigin));
 	numer = dot(pNormal,rOrigin) + d;
	denom = dot(pNormal,rVector);
  
	if(denom == 0)		// normal is orthogonal to vector, cant intersect
		return (-1.0f);
   
	return -(numer / denom);	
}

// ----------------------------------------------------------------------
// Name  : intersectRaySphere()
// Input : rO - origin of ray in world space
//         rV - vector describing direction of ray in world space
//         sO - Origin of sphere 
//         sR - radius of sphere
// Notes : Normalized directional vectors expected
// Return: distance to sphere in world units, -1 if no intersection.
// -----------------------------------------------------------------------  
double intersectRaySphere(vect_t rO, vect_t rV, vect_t sO, double sR)
{
	vect_t Q;
	double c;
	double v;
	double d;

	Q.X = sO.X-rO.X;
	Q.Y = sO.Y-rO.Y;
	Q.Z = sO.Z-rO.Z;

	c = lengthOfVector(Q);
	v = dot(Q,rV);
	d = sR*sR - (c*c - v*v);

	// If there was no intersection, return -1
	if(d < 0.0)
		return (-1.0f);

	// Return the distance to the [first] intersecting point
	return (v - sqrt(d));
}

// ----------------------------------------------------------------------
// Name  : closestPointOnLine()
// Input : a - first end of line segment
//         b - second end of line segment
//         p - point we wish to find closest point on line from 
// Notes : Helper function for closestPointOnTriangle()
// Return: closest point on line segment
// -----------------------------------------------------------------------  
vect_t closestPointOnLine(vect_t a, vect_t b, vect_t p)
{
   // Determine t (the length of the vector from ‘a’ to ‘p’)
	vect_t	c;
	vect_t	V; 
	double	d;
	double	t;

	c = m_Vector_Sub(p,a);
	V = m_Vector_Sub(b,a);
	d = lengthOfVector(V);
	normalizeVector(&V);  
	t = dot(V,c);
   
	// Check to see if ‘t’ is beyond the extents of the line segment
	if (t < 0.0f) return (a);
	if (t > d) return (b);
  
	// Return the point between ‘a’ and ‘b’
	//set length of V to t. V is normalized so this is easy
	V.X = V.X * (float)t;
	V.Y = V.Y * (float)t;
	V.Z = V.Z * (float)t;
       
	return m_Vector_Add(a,V);	
}

// ----------------------------------------------------------------------
// Name  : closestPointOnTriangle()
// Input : a - first vertex in triangle
//         b - second vertex in triangle 
//         c - third vertex in triangle
//         p - point we wish to find closest point on triangle from 
// Notes : 
// Return: closest point on line triangle edge
// -----------------------------------------------------------------------  
vect_t closestPointOnTriangle(vect_t a, vect_t b, vect_t c, vect_t p)
{
	vect_t	Rab;
	vect_t	Rbc;
	vect_t	Rca;
	double	dAB;
	double	dBC;
	double	dCA;
	double	min;
	vect_t	result;
	vect_t	v1,v2,v3;

	Rab = closestPointOnLine(a, b, p);
	Rbc = closestPointOnLine(b, c, p);
	Rca = closestPointOnLine(c, a, p);

	v1 = m_Vector_Sub(p,Rab);
	v2 = m_Vector_Sub(p,Rbc);
	v3 = m_Vector_Sub(p,Rca);

	dAB = lengthOfVector(v1);
	dBC = lengthOfVector(v2);
	dCA = lengthOfVector(v3);

	min = dAB;
	result = Rab;
  
	if(dBC < min)
	{
		min = dBC;
		result = Rbc;
	}
 
	if(dCA < min)
		result = Rca;
    
	return (result);
}

// ----------------------------------------------------------------------
// Name  : CheckPointInTriangle()
// Input : point - point we wish to check for inclusion
//         sO - Origin of sphere
//         sR - radius of sphere 
// Notes : 
// Return: TRUE if point is in sphere, FALSE if not.
// -----------------------------------------------------------------------  
hBool CheckPointInSphere(vect_t point, vect_t sO, double sR)
{	
	float	d;
	vect_t	v;

	v = m_Vector_Sub(point,sO);	
	d = (float)lengthOfVector(v);
 
	if(d <= sR)
		return true;
	return false;	
}

void UnstuckPointInSphere(vect_t *point, vect_t sO, double sR)
{
	vect_t	v;

	v = m_Vector_Sub(*point,sO);
	Normalize(&v);
	m_Vector_Scale(&v,(float)sR);
	*point = m_Vector_Add(sO,v);
}

// ----------------------------------------------------------------------
// Name  : tangentPlaneNormalOfEllipsoid()
// Input : point - point we wish to compute normal at 
//         eO - Origin of ellipsoid
//         eR - radius vector of ellipsoid 
// Notes : 
// Return: a unit normal vector to the tangent plane of the ellipsoid in the point.
// -----------------------------------------------------------------------  
vect_t tangentPlaneNormalOfEllipsoid(vect_t point, vect_t eO, vect_t eR)
{
	vect_t p;
	double a2;
	double b2;
	double c2;
	vect_t res;

	p = m_Vector_Sub(point,eO);
	a2 = eR.X * eR.X;
	b2 = eR.Y * eR.Y;
	c2 = eR.Z * eR.Z;

	res.X = p.X / (float)a2;
	res.Y = p.Y / (float)b2;
	res.Z = p.Z / (float)c2;

	normalizeVector(&res);	
	return (res);	
}

// ----------------------------------------------------------------------
// Name  : classifyPoint()
// Input : point - point we wish to classify 
//         pO - Origin of plane
//         pN - Normal to plane 
// Notes : 
// Return: One of 3 classification codes
// -----------------------------------------------------------------------

#define EPS_CP	0.001f

dword classifyPoint(vect_t point, vect_t pO, vect_t pN)
{
	vect_t	dir;
	double	d;

	dir = m_Vector_Sub(pO,point);
 	d = dot(dir, pN);
 
	if(d<-EPS_CP)
		return PLANE_FRONT;	
	else
		if(d>EPS_CP)
			return PLANE_BACKSIDE;
	return ON_PLANE;
}


float dot(vect_t v1, vect_t v2)
{
	return ( v1.X * v2.X + v1.Y * v2.Y + v1.Z * v2.Z );	
}

void normalizeVector(vect_t *v)
{
	float len;
	
	len = (float)sqrt(v->X*v->X + v->Y*v->Y + v->Z*v->Z);
	v->X /= len;
	v->Y /= len;
	v->Z /= len;
}

double lengthOfVector(vect_t v)
{
	return sqrt(v.X * v.X + v.Y * v.Y + v.Z * v.Z);
}

void setLength(vect_t *v, float l)
{
	float len;

	len = (float)sqrt(v->X*v->X + v->Y*v->Y + v->Z*v->Z);
	v->X *= (l/len);
	v->Y *= (l/len);
	v->Z *= (l/len);
} 








