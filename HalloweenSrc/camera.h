
#ifndef Camera_t_INCLUDED
#define Camera_t_INCLUDED


#include "alloween.h"
#include <math.h>

#define	MAX_CAMERA		4096


typedef struct tag_Cam
{
	vect_t	Pos;
	vect_t	Rot;
	vect_t	Siz;

} Camera_t;


float	cam_RotationCycle(float angle);
void	cam_EarthQuake_Launch(float force);
void	cam_EarthQuake_process();
void	cam_StopEarthQuake();
void	cam_EarthQuake(float force);
void	cam_Move(float Direction, Camera_t *pCam);
void	cam_MoveY(hBool Direction, Camera_t *pCam);
void 	r_TourneX(Camera_t *Hero, float Angle, pEntity PlayerEnt);
void 	r_TourneY(Camera_t *Hero, float Angle, pEntity PlayerEnt);


#endif // Camera_t_INCLUDED
