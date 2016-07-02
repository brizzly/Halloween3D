
#include <windows.h>
#include "collision2.h"

#include "console.h"
//#include "bspfile.h"
//#include "objet.h"


CollisionModel3D * world_ColDetModel = NULL;


void col2_init_World()
{
	if(world_ColDetModel)
	{
		delete world_ColDetModel;
		world_ColDetModel = NULL;
	}
	world_ColDetModel = newCollisionModel3D();
}

void col2_AddFace_Word(float x1,	float y1, float z1,
					   float x2,	float y2, float z2,
					   float x3,	float y3, float z3 )
{
	world_ColDetModel->addTriangle(x1, y1, z1, x2, y2, z2, x3, y3, z3);
}

void col2_FinalizeWorld()
{
	world_ColDetModel->finalize ();
}


void col2_Process(float x, float y, float z)
{
	float origin[3];
	float radius = 10;


	origin[0] = x;
	origin[1] = y;
	origin[2] = z;
	radius = 30;

	if(world_ColDetModel)
	{
		bool colres = world_ColDetModel->sphereCollision(origin, radius);
		if(colres)
		{
			m_ConsPrint("ColDet TRUE\n");
		}
	}
}