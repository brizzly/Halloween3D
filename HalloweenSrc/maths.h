
#ifndef MATHS_INCLUDED
#define MATHS_INCLUDED

#include "alloween.h"

#define PI					3.141592653589793
#define ROTMAX				360.0f
#define ROTMAX_I			360
#define PIOVER180			0.01745329251f
#define	INVPIOVER180		57.2957795131f
#define	MIN_BOUND			-4096
#define	MAX_BOUND			4096

enum
{
	CONFONDU,
	DEVANT,
	DERRIERE,
	AUMILIEU
};

typedef struct VectorTag
{
	double	m_vector[4];

} Vector;

typedef struct MatrixTag
{
	float m_matrix[16];

} Matrix;

typedef struct QuatTag
{
	float m_quat[4];

} Quaternion;

void	m_Vector_Scale(vect_t *v, float scale);
vect_t	m_Vector_Add(vect_t v1, vect_t v2);
vect_t	m_Vector_Sub(vect_t v1, vect_t v2);
void	m_Vector_transform(Vector *vect, Matrix m);
void	m_Vector_transform3(Vector *vect, Matrix m);
void	m_Matrix_Identity(float *Matrix);
void	m_Matrix_set(Matrix *matrix1, const float *matrix);
void	m_Matrix_postMultiply(Matrix *matrix1, const Matrix matrix2);
void	m_Matrix_Multiply(Matrix *matrix1, const Matrix matrix2);
void	m_Matrix_setTranslation(Matrix *matrix, const float *translation);
void	m_Matrix_setInverseTranslation(Matrix *matrix, const float *translation);
void	m_Matrix_setRotationDegrees(Matrix *matrix, const float *angles);
void	m_Matrix_setInverseRotationDegrees(Matrix *matrix, const float *angles);
void	m_Matrix_setRotationRadians(Matrix *matrix, float *angles);
void	m_Matrix_setInverseRotationRadians(Matrix *matrix, const float *angles);
void	m_Matrix_inverseRotateVect(Matrix *matrix, float *pVect);
void	m_Matrix_inverseTranslateVect(Matrix *matrix, float *pVect);
void	m_Matrix_setRotationQuaternion(Matrix *matrix, const Quaternion quat);
void	m_Quaternion_fromAngles(Quaternion *quat, const float *angles);
void	m_Quaternion_inverse(Quaternion *quat);
void	m_Quaternion_slerp(Quaternion *quat, const Quaternion q1, Quaternion q2, float interp);
vect_t	m_RotateX(vect_t v, float angle);
vect_t	m_RotateY(vect_t v, float angle);
vect_t	m_RotateZ(vect_t v, float angle);
void	m_Swapf(float *a, float *b);
float	m_Distance(vect_t M, vect_t P);
hBool	m_pointInTri(vect_t pNormal, vect_t fp1, vect_t fp2, vect_t fp3, vect_t CP);
float	m_MaxValuef(float a, float b);
vect_t	m_GetFaceNormal_vect(vect_t Vec0, vect_t Vec1, vect_t Vec2);
float	m_DistancePlan(plan_t Plan, vect_t pos);

void	InitTrigoTables();
float	Sin_Rad(float alpha);
float	Cos_Rad(float alpha);
float	Sin(float alpha);
float	Cos(float alpha);
float	Sin_i(int alpha);
float	Cos_i(int alpha);
int		ClassifyVect_offset(vect_t pos, Plan_t plan, float offset);
int		ClassifyVect(vect_t *pos, Plan_t *Plan);
void	ClearBound(BOUNDINGBOX *bbox);
hBool	IsValidBound(BOUNDINGBOX *bbox);
hBool	IsInBox(vect_t pos, BOUNDINGBOX box);
void	CalculateBox(BOUNDINGBOX *Box, Face_t *Polylist);
vect_t	VectorBounce(vect_t *in, vect_t normal, float step);
vect_t	GetFaceNormal(Point_t Vec0, Point_t Vec1, Point_t Vec2);
float	SquareNorme(vect_t v);
double	Norme(vect_t v);
float	PdtScalaire (vect_t *pVect1, vect_t *pVect2);
void	PdVectoriel(vect_t *Vres, vect_t *v1, vect_t *v2);
hBool	FastCull(Face_t *pFace, vect_t EyePos);
hBool	FastCull_Ent(pEntity Entity, pEntity PlayerEnt);
float	DistancePlan(Plan_t *pPlan, Point_t *pPoint);
void	Normalize(vect_t *pVect);
float	A_atof(char *str);
int		A_strlen(char *str);
float	A_absf(float n);
int		randInt( int low, int high );


#endif // MATHS_INCLUDED
