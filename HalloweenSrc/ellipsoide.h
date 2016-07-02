
#ifndef ELLIPSOIDE_INCLUDED
#define ELLIPSOIDE_INCLUDED


#include "alloween.h"

#define	UNIT_SPHERE		1.0f
#define EPSILON_COL		0.002f // 0.01f // 0.05f
#define PLANE_BACKSIDE	0x000001
#define PLANE_FRONT		0x000010
#define ON_PLANE		0x000100


typedef struct TCollisionPacket_tag
{ 
	int		nPass;

	// data about player movement
	pEntity	Ent;
	vect_t	velocity;
	vect_t	sourcePoint;
  
	// radius of ellipsoid.  
	vect_t	eRadius;
  
	// for error handling  
	vect_t	lastSafePosition;
	vect_t	lastSafeVelocity;
	hBool	stuck; 
	hBool	EmergencyQuit;
    
	// data for collision response 
	hBool	foundCollision;
	hBool	found_AB_collision;					// collision with action brush
	double	nearestDistance;					// nearest distance to hit
	vect_t	nearestIntersectionPoint;			// on sphere
	vect_t	nearestPolygonIntersectionPoint;	// on polygon
	int		PolygonType;						// mur, lave, etc..
	plan_t	Plan;

} TCollisionPacket;



void	col_Unstuck(TCollisionPacket *collision, CTRACE *trace, vect_t *pos, vect_t nextpos);

void	col_Handle(vect_t *pos, vect_t nextpos, CTRACE *trace);
void	CheckCollision(TCollisionPacket* colPackage, pFace FaceList, vect_t *pos_offset, vect_t *siz_offset);
vect_t	GetPosition(TCollisionPacket *collision, vect_t position, vect_t velocity, CTRACE *trace);
vect_t	collideWithWorld(TCollisionPacket *collision, vect_t position, vect_t velocity, CTRACE *trace);
double	intersectRayPlane(vect_t rOrigin, vect_t rVector, vect_t pOrigin, vect_t pNormal);
double	intersectRaySphere(vect_t rO, vect_t rV, vect_t sO, double sR);
vect_t	closestPointOnLine(vect_t a, vect_t b, vect_t p);
vect_t	closestPointOnTriangle(vect_t a, vect_t b, vect_t c, vect_t p);
hBool	CheckPointInSphere(vect_t point, vect_t sO, double sR);
void	UnstuckPointInSphere(vect_t *point, vect_t sO, double sR);
vect_t	tangentPlaneNormalOfEllipsoid(vect_t point, vect_t eO, vect_t eR);
dword	classifyPoint(vect_t point, vect_t pO, vect_t pN);
float	dot(vect_t v1, vect_t v2);
void	normalizeVector(vect_t *v);
double	lengthOfVector(vect_t v);
void	setLength(vect_t *v, float l);



#endif // ELLIPSOIDE_INCLUDED
