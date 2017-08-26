#include "alloween.h"


Plan_t	gFrustrumPlanes[6];


void frustrum_DispMatrix()
{
	Matrix	ViewProj;

	glGetFloatv( GL_MODELVIEW_MATRIX, ViewProj.m_matrix );
	g_DispString(14,"============================");
	g_DispString(14,"%f %f %f %f",
		ViewProj.m_matrix[0],ViewProj.m_matrix[1],ViewProj.m_matrix[2],ViewProj.m_matrix[3]);
	g_DispString(14,"%f %f %f %f",
		ViewProj.m_matrix[4],ViewProj.m_matrix[5],ViewProj.m_matrix[6],ViewProj.m_matrix[7]);
	g_DispString(14,"%f %f %f %f",
		ViewProj.m_matrix[8],ViewProj.m_matrix[9],ViewProj.m_matrix[10],ViewProj.m_matrix[11]);
	g_DispString(14,"%f %f %f %f",
		ViewProj.m_matrix[12],ViewProj.m_matrix[13],ViewProj.m_matrix[14],ViewProj.m_matrix[15]);
	g_DispString(14,"============================");
}

void frustrum_Disp(Plan_t Planes[])
{
	int		i;

	g_DispString(14,"============================");

	for(i=0 ; i<6 ; i++)
	{
		g_DispString(14,"%f %f %f : %f",
					Planes[i].VecteurNormal.X,
					Planes[i].VecteurNormal.Y,
					Planes[i].VecteurNormal.Z,
					Planes[i].PointOnPlane.X);
	}
}

void frustrum_ExtractPlanes(Plan_t Planes[])
{
	int		i;
	Matrix	ViewMatrix, ProjMatrix, ViewProj;
	float	denom;

	glGetFloatv( GL_PROJECTION_MATRIX, ProjMatrix.m_matrix );	// get the current projection matrix
	glGetFloatv( GL_MODELVIEW_MATRIX, ViewMatrix.m_matrix );

	m_Matrix_Multiply(&ProjMatrix,ViewMatrix);					// combine View and Projection matrices
	m_Matrix_set(&ViewProj,ProjMatrix.m_matrix);

	// Left clipping plane
	Planes[0].VecteurNormal.X = -(ViewProj.m_matrix[3] + ViewProj.m_matrix[0]);
	Planes[0].VecteurNormal.Y = -(ViewProj.m_matrix[7] + ViewProj.m_matrix[4]);
	Planes[0].VecteurNormal.Z = -(ViewProj.m_matrix[11] + ViewProj.m_matrix[8]);
	Planes[0].PointOnPlane.X = -(ViewProj.m_matrix[15] + ViewProj.m_matrix[12]);

	// Right clipping plane
	Planes[1].VecteurNormal.X = -(ViewProj.m_matrix[3] - ViewProj.m_matrix[0]);
	Planes[1].VecteurNormal.Y = -(ViewProj.m_matrix[7] - ViewProj.m_matrix[4]);
	Planes[1].VecteurNormal.Z = -(ViewProj.m_matrix[11] - ViewProj.m_matrix[8]);
	Planes[1].PointOnPlane.X = -(ViewProj.m_matrix[15] - ViewProj.m_matrix[12]);

	// Top clipping plane
	Planes[2].VecteurNormal.X = -(ViewProj.m_matrix[3] - ViewProj.m_matrix[1]);
	Planes[2].VecteurNormal.Y = -(ViewProj.m_matrix[7] - ViewProj.m_matrix[5]);
	Planes[2].VecteurNormal.Z = -(ViewProj.m_matrix[11] - ViewProj.m_matrix[9]);
	Planes[2].PointOnPlane.X = -(ViewProj.m_matrix[15] - ViewProj.m_matrix[13]);

	// Bottom clipping plane
	Planes[3].VecteurNormal.X = -(ViewProj.m_matrix[3] + ViewProj.m_matrix[1]);
	Planes[3].VecteurNormal.Y = -(ViewProj.m_matrix[7] + ViewProj.m_matrix[5]);
	Planes[3].VecteurNormal.Z = -(ViewProj.m_matrix[11] + ViewProj.m_matrix[9]);
	Planes[3].PointOnPlane.X = -(ViewProj.m_matrix[15] + ViewProj.m_matrix[13]);

	// Near clipping plane
	Planes[4].VecteurNormal.X = -(ViewProj.m_matrix[3] + ViewProj.m_matrix[2]);
	Planes[4].VecteurNormal.Y = -(ViewProj.m_matrix[7] + ViewProj.m_matrix[6]);
	Planes[4].VecteurNormal.Z = -(ViewProj.m_matrix[11] + ViewProj.m_matrix[10]);
	Planes[4].PointOnPlane.X = -(ViewProj.m_matrix[15] + ViewProj.m_matrix[14]);

	// Far clipping plane
	Planes[5].VecteurNormal.X = -(ViewProj.m_matrix[3] - ViewProj.m_matrix[2]);
	Planes[5].VecteurNormal.Y = -(ViewProj.m_matrix[7] - ViewProj.m_matrix[6]);
	Planes[5].VecteurNormal.Z = -(ViewProj.m_matrix[11] - ViewProj.m_matrix[10]);
	Planes[5].PointOnPlane.X = -(ViewProj.m_matrix[15] - ViewProj.m_matrix[14]);

	for(i=0 ; i<6 ; i++)
	{
		denom=1.0f/(float)Norme(Planes[i].VecteurNormal);
  
		Planes[i].VecteurNormal.X *= denom; 
		Planes[i].VecteurNormal.Y *= denom; 
		Planes[i].VecteurNormal.Z *= denom;
		Planes[i].PointOnPlane.X *= denom;
	}

}


//               LEAF IN FRUSTUM
//
// Tests if the Bounding Box surrounding the leaf is completely outside the frustum.
//
// NOTE: This check is only done on visible leafs in a Leafs PVS Set to further carve off
// more polygons than just the straight PVS Set.Remember a PVS set does not take into account
// Direction of camera so some of the PVS Set will not be visible.

hBool frustrum_LeafIn(Plan_t Plane[], int Leaf)
{
	return frustrum_BoxIn(Plane,LeafArray[Leaf].bbox);
}

hBool frustrum_BoxIn(Plan_t Plane[], BOUNDINGBOX bbox)
{
	vect_t	bMax = bbox.BoxMax;
	vect_t	bMin = bbox.BoxMin;
	vect_t	NearPoint; //, FarPoint;
	int		i;

	for(i=0 ; i<6 ; i++)
	{
		if(Plane[i].VecteurNormal.X > 0.0f)
		{
			if(Plane[i].VecteurNormal.Y > 0.0f)
			{
				if(Plane[i].VecteurNormal.Z > 0.0f)
				{
					NearPoint.X = bMin.X;
					NearPoint.Y = bMin.Y;
					NearPoint.Z = bMin.Z;
				}	
				else
				{
					NearPoint.X = bMin.X;
					NearPoint.Y = bMin.Y;
					NearPoint.Z = bMax.Z;
				}
			}
			else
			{
				if(Plane[i].VecteurNormal.Z > 0.0f)
				{
					NearPoint.X = bMin.X;
					NearPoint.Y = bMax.Y;
					NearPoint.Z = bMin.Z;
				}
				else
				{
					NearPoint.X = bMin.X;
					NearPoint.Y = bMax.Y;
					NearPoint.Z = bMax.Z;
				}
			}
		}
		else
		{
			if(Plane[i].VecteurNormal.Y > 0.0f)
			{
				if (Plane[i].VecteurNormal.Z > 0.0f)
				{
					NearPoint.X = bMax.X;
					NearPoint.Y = bMin.Y;
					NearPoint.Z = bMin.Z;
				}
				else
				{
					NearPoint.X = bMax.X;
					NearPoint.Y = bMin.Y;
					NearPoint.Z = bMax.Z;
				}
			}
			else
			{
				if(Plane[i].VecteurNormal.Z > 0.0f)
				{
					NearPoint.X = bMax.X;
					NearPoint.Y = bMax.Y;
					NearPoint.Z = bMin.Z;
				}
				else
				{
					NearPoint.X = bMax.X;
					NearPoint.Y = bMax.Y;
					NearPoint.Z = bMax.Z;
				}
			}
		}

		// near extreme point is outside, and thus
		// the AABB is Totally outside the polyhedron

		if( PdtScalaire(&Plane[i].VecteurNormal,&NearPoint) + Plane[i].PointOnPlane.X > 0)
			return false;
	}
	return true;
}
