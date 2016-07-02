
#include "radcomp.h"
#include "maths.h"
#include "patch.h"


void patch_Init()
{
	face_t	pCurr;
	lm_t	NewLm;
	int		i, j;
	BOUNDINGBOX	FaceBound;
	vect_t	PlaneNormal;
	vect_t	U, V, N;
	vect_t	W, Z;
	vect_t	CB;
	vect_t	edge1, edge2, newedge1, newedge2;
	float	S1, S2;
	float	deltaX, deltaY, deltaZ;
	int		ind;
	vect_t	e1 = {1,0,0};
	vect_t	e2 = {0,1,0};
	vect_t	e3 = {0,0,1};
	float	dX, dY, dZ;

	printf("Initialize patchs\n");
	for(NewLm=LmList ; NewLm ; NewLm=NewLm->next)
	{
		pCurr = NewLm->face;
		GetBound(pCurr,&FaceBound);
		CB.X = (FaceBound.BoxMin.X + FaceBound.BoxMax.X)*0.5f;
		CB.Y = (FaceBound.BoxMin.Y + FaceBound.BoxMax.Y)*0.5f;
		CB.Z = (FaceBound.BoxMin.Z + FaceBound.BoxMax.Z)*0.5f;
		PlaneNormal = pCurr->Plan.VecteurNormal;

		if(PlaneNormal.X == 0 && PlaneNormal.Y == 0 && PlaneNormal.Z == 0)
			printf("Face normal null error\n");

		switch(FindPojectionPlane(PlaneNormal))
		{
			case XY:
			U = e1;
			V = e2;
			N = e3;
			break;

			case YZ:
			U = e2;
			V = e3;
			N = e1;
			break;

			case ZX:
			U = e3;
			V = e1;
			N = e2;
			break;

			default:
			printf("Error!\n");
			while(1);
			break;
		}

		S1 = A_absf(PdtScalaire(&N,&PlaneNormal));
		if(S1 != 1)
		{
			PdVectoriel(&W,&PlaneNormal,&N);
			Normalize(&W);

			PdVectoriel(&Z,&W,&PlaneNormal);
			Normalize(&Z);

			S1 = PdtScalaire(&U,&W);
			S2 = PdtScalaire(&V,&Z);

			if(IsEqualf(S1,0) || IsEqualf(S2,0))
			{
				N = W;
				W = Z;
				Z = N;
			}

			S1 = PdtScalaire(&U,&W);
			S2 = PdtScalaire(&V,&Z);

			if(S1<0)
				m_VectScale(&W,-1);
			if(S2<0)
				m_VectScale(&Z,-1);

			//S1 = PdtScalaire(&U,&W);
			//S2 = PdtScalaire(&V,&Z);
			//printf("S1:%f S2:%f\n",S1,S2);

			U = W;
			V = Z;
		}

		dX = FaceBound.BoxMax.X - CB.X;
		dY = FaceBound.BoxMax.Y - CB.Y;
		dZ = FaceBound.BoxMax.Z - CB.Z;

		U.X*=dX;
		U.Y*=dY;
		U.Z*=dZ;

		V.X*=dX;
		V.Y*=dY;
		V.Z*=dZ;
		
		NewLm->point[0].X = CB.X - U.X - V.X;
		NewLm->point[0].Y = CB.Y - U.Y - V.Y;
		NewLm->point[0].Z = CB.Z - U.Z - V.Z;

		NewLm->point[1].X = CB.X + U.X - V.X;
		NewLm->point[1].Y = CB.Y + U.Y - V.Y;
		NewLm->point[1].Z = CB.Z + U.Z - V.Z;

		NewLm->point[2].X = CB.X + U.X + V.X;
		NewLm->point[2].Y = CB.Y + U.Y + V.Y;
		NewLm->point[2].Z = CB.Z + U.Z + V.Z;

		NewLm->point[3].X = CB.X - U.X + V.X;
		NewLm->point[3].Y = CB.Y - U.Y + V.Y;
		NewLm->point[3].Z = CB.Z - U.Z + V.Z;
	}

	for(NewLm=LmList ; NewLm ; NewLm=NewLm->next)
	{
		ind = 0;
		edge1.X = NewLm->point[(ind+3)%4].X - NewLm->point[(ind+0)%4].X;
		edge1.Y = NewLm->point[(ind+3)%4].Y - NewLm->point[(ind+0)%4].Y;
		edge1.Z = NewLm->point[(ind+3)%4].Z - NewLm->point[(ind+0)%4].Z;

		edge2.X = NewLm->point[(ind+1)%4].X - NewLm->point[(ind+0)%4].X;
		edge2.Y = NewLm->point[(ind+1)%4].Y - NewLm->point[(ind+0)%4].Y;
		edge2.Z = NewLm->point[(ind+1)%4].Z - NewLm->point[(ind+0)%4].Z;

		for(i=0 ; i<nPATCH_X ; i++)
		for(j=0 ; j<nPATCH_Y ; j++)
		{
			// position patch pos centered
			S1 = (1.0f/(float)nPATCH_X) * (i+0.5f);
			S2 = (1.0f/(float)nPATCH_Y) * (j+0.5f);

		//	S1 = (float)i/(float)nPATCH_X;
		//	S2 = (float)j/(float)nPATCH_Y;

			newedge1.X = S1 * edge1.X;
			newedge1.Y = S1 * edge1.Y;
			newedge1.Z = S1 * edge1.Z;
			newedge2.X = S2 * edge2.X;
			newedge2.Y = S2 * edge2.Y;
			newedge2.Z = S2 * edge2.Z;

			NewLm->patch[i+(j*nPATCH_X)].pos.X = NewLm->point[0].X + newedge1.X + newedge2.X;
			NewLm->patch[i+(j*nPATCH_X)].pos.Y = NewLm->point[0].Y + newedge1.Y + newedge2.Y;
			NewLm->patch[i+(j*nPATCH_X)].pos.Z = NewLm->point[0].Z + newedge1.Z + newedge2.Z;
		}
	}
	
	for(NewLm=LmList ; NewLm ; NewLm=NewLm->next)
	{
		pCurr = NewLm->face;
		GetBound(pCurr,&FaceBound);
		V.X = FaceBound.BoxMin.X;
		V.Y = FaceBound.BoxMin.Y;
		V.Z = FaceBound.BoxMin.Z;
		deltaX = FaceBound.BoxMax.X - FaceBound.BoxMin.X;
		deltaY = FaceBound.BoxMax.Y - FaceBound.BoxMin.Y;
		deltaZ = FaceBound.BoxMax.Z - FaceBound.BoxMin.Z;
		if(deltaX == 0)	deltaX=1;
		if(deltaY == 0)	deltaY=1;
		if(deltaZ == 0)	deltaZ=1;
		switch(FindPojectionPlane(pCurr->Plan.VecteurNormal))
		{
			case XY:
			for(i=0 ; i<pCurr->NumberOfVertices ; i++)
			{
				pCurr->point[i].V_lm = (pCurr->point[i].X - V.X)/deltaX;
				pCurr->point[i].U_lm = (pCurr->point[i].Y - V.Y)/deltaY;
			}
			break;

			case YZ:
			for(i=0 ; i<pCurr->NumberOfVertices ; i++)
			{
				pCurr->point[i].V_lm = (pCurr->point[i].Y - V.Y)/deltaY;
				pCurr->point[i].U_lm = (pCurr->point[i].Z - V.Z)/deltaZ;
			}
			break;

			case ZX:
			for(i=0 ; i<pCurr->NumberOfVertices ; i++)
			{
				pCurr->point[i].V_lm = (pCurr->point[i].Z - V.Z)/deltaZ;
				pCurr->point[i].U_lm = (pCurr->point[i].X - V.X)/deltaX;
			}
			break;
		}

	// fix texture junctions
//#define		ONEOVER32	0.03125f
#define		ONEOVER32	0.04f

		for(i=0 ; i<pCurr->NumberOfVertices ; i++)
		{
			if( pCurr->point[i].V_lm < 0 || pCurr->point[i].V_lm > 1)
				printf("-");
			if( pCurr->point[i].U_lm < 0 || pCurr->point[i].U_lm > 1)
				printf("+");

			if(pCurr->point[i].U_lm <= 0)
				pCurr->point[i].U_lm += ONEOVER32;
			if(pCurr->point[i].V_lm <= 0)
				pCurr->point[i].V_lm += ONEOVER32;

			if(pCurr->point[i].U_lm >= 1)
				pCurr->point[i].U_lm -= ONEOVER32;
			if(pCurr->point[i].V_lm >= 1)
				pCurr->point[i].V_lm -= ONEOVER32;
		}

	}
}
