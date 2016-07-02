#ifndef ENVEFFECT__
#define ENVEFFECT__

#include "alloween.h"


typedef struct particule_tag
{
	hBool	used;
	int		nParticule;
	int		color;
	vect_t	pos[MAX_PART];
	vect_t	dir[MAX_PART];
	float	lifetime_msec[MAX_PART];

}	PARTICULE;



void fx_InitParticule();
void fx_Particule(vect_t pos, int n, int color);
void fx_UpdateParticule();
void fx_DrawParticule(vect_t campos);
void fx_ColorScreen(float trans, float timing, float R, float G, float B, hBool force);
void fx_UpdateColorScreen();



#endif // ENVEFFECT__
