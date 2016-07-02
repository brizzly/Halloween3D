
#define	EPSILON				0.0001f
#define PI					3.141592653589793
#define	MIN_BOUND			-4096
#define	MAX_BOUND			4096


float	m_Distance(vect_t M, vect_t P);
float	SquareNorme(const vect_t *pVect);
float	Norme(const vect_t *pVect);
void	Normalize(vect_t *pVect);
float	PdtScalaire(const vect_t *pVect1, const vect_t *pVect2);
void	PdVectoriel(vect_t *Vres, vect_t *v1, vect_t *v2);
vect_t	GetFaceNormal(point_t Vec0, point_t Vec1, point_t Vec2);
float	DistancePlan(const plan_t *pPlan, const point_t *pPoint);
bool	Intersect_Poly(vect_t *orig, vect_t *dest, face_t Poly, vect_t *cut, float *t);
bool	Intersect_triangle(vect_t *orig, vect_t *dest, point_t *vert0, point_t *vert1, point_t *vert2, vect_t *cut, float *t);
int		FindPojectionPlane(vect_t normal);
int		FindPojectionPlane2(lm_t Lm);
float	A_absf(float value);
float	GetXYZplane(plan_t plan, float X1, float X2, int T);
float	Getconstante(plan_t plan);
void	ClearBound(BOUNDINGBOX *bbox);
void	GetBound(face_t face, BOUNDINGBOX *Bound);
bool	IsLightIntersect(vect_t *start, vect_t *end, face_t face);
bool	IsVertexOnFace(vect_t V, face_t face);
bool	pointInTri(face_t Face, vect_t CP);
bool	IsEqualf(float A, float B);
void	m_VectScale(vect_t *v, float scale);
