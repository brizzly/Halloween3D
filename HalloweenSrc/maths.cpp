
#include "alloween.cpp"


float	Tsin[360];
float	Tcos[360];

//
//	Vector stuff
//

void m_Vector_Scale(vect_t *v, float scale)
{
	v->X *= scale;
	v->Y *= scale;
	v->Z *= scale;
}

vect_t m_Vector_Add(vect_t v1, vect_t v2)
{
	vect_t	res;

	res.X = v1.X + v2.X;
	res.Y = v1.Y + v2.Y;
	res.Z = v1.Z + v2.Z;

	return res;
}

vect_t m_Vector_Sub(vect_t v1, vect_t v2)
{
	vect_t	res;

	res.X = v1.X - v2.X;
	res.Y = v1.Y - v2.Y;
	res.Z = v1.Z - v2.Z;

	return res;
}



//
//	Matrix stuff
//

void m_Matrix_Identity(float *Matrix)
{
	Matrix[0] = 1;
	Matrix[1] = 0;
	Matrix[2] = 0;
	Matrix[3] = 0;

	Matrix[4] = 0;
	Matrix[5] = 1;
	Matrix[6] = 0;
	Matrix[7] = 0;

	Matrix[8] = 0;
	Matrix[9] = 0;
	Matrix[10] = 1;
	Matrix[11] = 0;

	Matrix[12] = 0;
	Matrix[13] = 0;
	Matrix[14] = 0;
	Matrix[15] = 1;
}

void m_Vector_transform( Vector *vect, Matrix m )
{
	double	vector[4];
	float	*matrix = m.m_matrix;

	vector[0] = vect->m_vector[0]*matrix[0]+vect->m_vector[1]*matrix[4]+vect->m_vector[2]*matrix[8]+matrix[12];
	vector[1] = vect->m_vector[0]*matrix[1]+vect->m_vector[1]*matrix[5]+vect->m_vector[2]*matrix[9]+matrix[13];
	vector[2] = vect->m_vector[0]*matrix[2]+vect->m_vector[1]*matrix[6]+vect->m_vector[2]*matrix[10]+matrix[14];
	vector[3] = vect->m_vector[0]*matrix[3]+vect->m_vector[1]*matrix[7]+vect->m_vector[2]*matrix[11]+matrix[15];

	vect->m_vector[0] = ( float )( vector[0] );
	vect->m_vector[1] = ( float )( vector[1] );
	vect->m_vector[2] = ( float )( vector[2] );
	vect->m_vector[3] = ( float )( vector[3] );
}

void m_Vector_transform3(Vector *vect, Matrix m )
{
	double vector[3];
	float *matrix = m.m_matrix;

	vector[0] = vect->m_vector[0]*matrix[0]+vect->m_vector[1]*matrix[4]+vect->m_vector[2]*matrix[8];
	vector[1] = vect->m_vector[0]*matrix[1]+vect->m_vector[1]*matrix[5]+vect->m_vector[2]*matrix[9];
	vector[2] = vect->m_vector[0]*matrix[2]+vect->m_vector[1]*matrix[6]+vect->m_vector[2]*matrix[10];

	vect->m_vector[0] = ( float )( vector[0] );
	vect->m_vector[1] = ( float )( vector[1] );
	vect->m_vector[2] = ( float )( vector[2] );
	vect->m_vector[3] = 1;
}

void m_Matrix_set( Matrix *matrix1, const float *matrix )
{
	memcpy( matrix1->m_matrix, matrix, sizeof( float )*16 );
}

void m_Matrix_postMultiply(Matrix *matrix1, const Matrix matrix2)
{
	float newMatrix[16];
	const float *m1 = matrix1->m_matrix, *m2 = matrix2.m_matrix;

	newMatrix[0] = m1[0]*m2[0] + m1[4]*m2[1] + m1[8]*m2[2];
	newMatrix[1] = m1[1]*m2[0] + m1[5]*m2[1] + m1[9]*m2[2];
	newMatrix[2] = m1[2]*m2[0] + m1[6]*m2[1] + m1[10]*m2[2];
	newMatrix[3] = 0;

	newMatrix[4] = m1[0]*m2[4] + m1[4]*m2[5] + m1[8]*m2[6];
	newMatrix[5] = m1[1]*m2[4] + m1[5]*m2[5] + m1[9]*m2[6];
	newMatrix[6] = m1[2]*m2[4] + m1[6]*m2[5] + m1[10]*m2[6];
	newMatrix[7] = 0;

	newMatrix[8] = m1[0]*m2[8] + m1[4]*m2[9] + m1[8]*m2[10];
	newMatrix[9] = m1[1]*m2[8] + m1[5]*m2[9] + m1[9]*m2[10];
	newMatrix[10] = m1[2]*m2[8] + m1[6]*m2[9] + m1[10]*m2[10];
	newMatrix[11] = 0;

	newMatrix[12] = m1[0]*m2[12] + m1[4]*m2[13] + m1[8]*m2[14] + m1[12];
	newMatrix[13] = m1[1]*m2[12] + m1[5]*m2[13] + m1[9]*m2[14] + m1[13];
	newMatrix[14] = m1[2]*m2[12] + m1[6]*m2[13] + m1[10]*m2[14] + m1[14];
	newMatrix[15] = 1;

	m_Matrix_set( matrix1, newMatrix );
}

void m_Matrix_Multiply(Matrix *matrix1, const Matrix matrix2)
{
	float newMatrix[16];
	const float *m1 = matrix1->m_matrix, *m2 = matrix2.m_matrix;

	newMatrix[0] = m1[0]*m2[0] + m1[4]*m2[1] + m1[8]*m2[2] + m1[12]*m2[3];
	newMatrix[1] = m1[1]*m2[0] + m1[5]*m2[1] + m1[9]*m2[2] + m1[13]*m2[3];
	newMatrix[2] = m1[2]*m2[0] + m1[6]*m2[1] + m1[10]*m2[2] + m1[14]*m2[3];
	newMatrix[3] = m1[3]*m2[0] + m1[7]*m2[1] + m1[11]*m2[2] + m1[15]*m2[3];

	newMatrix[4] = m1[0]*m2[4] + m1[4]*m2[5] + m1[8]*m2[6] + m1[12]*m2[7];
	newMatrix[5] = m1[1]*m2[4] + m1[5]*m2[5] + m1[9]*m2[6] + m1[13]*m2[7];
	newMatrix[6] = m1[2]*m2[4] + m1[6]*m2[5] + m1[10]*m2[6] + m1[14]*m2[7];
	newMatrix[7] = m1[3]*m2[4] + m1[7]*m2[5] + m1[11]*m2[6] + m1[15]*m2[7];

	newMatrix[8] = m1[0]*m2[8] + m1[4]*m2[9] + m1[8]*m2[10] + m1[12]*m2[11];
	newMatrix[9] = m1[1]*m2[8] + m1[5]*m2[9] + m1[9]*m2[10] + m1[13]*m2[11];
	newMatrix[10] = m1[2]*m2[8] + m1[6]*m2[9] + m1[10]*m2[10] + m1[14]*m2[11];
	newMatrix[11] = m1[3]*m2[8] + m1[7]*m2[9] + m1[11]*m2[10] + m1[15]*m2[11];

	newMatrix[12] = m1[0]*m2[12] + m1[4]*m2[13] + m1[8]*m2[14] + m1[12]*m2[15];
	newMatrix[13] = m1[1]*m2[12] + m1[5]*m2[13] + m1[9]*m2[14] + m1[13]*m2[15];
	newMatrix[14] = m1[2]*m2[12] + m1[6]*m2[13] + m1[10]*m2[14] + m1[14]*m2[15];
	newMatrix[15] = m1[3]*m2[12] + m1[7]*m2[13] + m1[11]*m2[14] + m1[15]*m2[15];

	m_Matrix_set( matrix1, newMatrix );
}

void m_Matrix_setTranslation(Matrix *matrix, const float *translation)
{
	matrix->m_matrix[12] = translation[0];
	matrix->m_matrix[13] = translation[1];
	matrix->m_matrix[14] = translation[2];
}

void m_Matrix_setInverseTranslation(Matrix *matrix, const float *translation)
{
	matrix->m_matrix[12] = -translation[0];
	matrix->m_matrix[13] = -translation[1];
	matrix->m_matrix[14] = -translation[2];
}

void m_Matrix_setRotationDegrees(Matrix *matrix, const float *angles)
{
	float vec[3];
	vec[0] = ( float )( angles[0]*180.0/PI );
	vec[1] = ( float )( angles[1]*180.0/PI );
	vec[2] = ( float )( angles[2]*180.0/PI );
	m_Matrix_setRotationRadians( matrix, vec );
}

void m_Matrix_setInverseRotationDegrees(Matrix *matrix, const float *angles)
{
	float vec[3];
	vec[0] = ( float )( angles[0]*180.0/PI );
	vec[1] = ( float )( angles[1]*180.0/PI );
	vec[2] = ( float )( angles[2]*180.0/PI );
	m_Matrix_setInverseRotationRadians( matrix, vec );
}

void m_Matrix_setRotationRadians(Matrix *matrix, /*const*/ float *angles)
{
	double cr = Cos_Rad( angles[0] );
	double sr = Sin_Rad( angles[0] );
	double cp = Cos_Rad( angles[1] );
	double sp = Sin_Rad( angles[1] );
	double cy = Cos_Rad( angles[2] );
	double sy = Sin_Rad( angles[2] );
	double srsp = sr*sp;
	double crsp = cr*sp;

	matrix->m_matrix[0] = ( float )( cp*cy );
	matrix->m_matrix[1] = ( float )( cp*sy );
	matrix->m_matrix[2] = ( float )( -sp );

	matrix->m_matrix[4] = ( float )( srsp*cy-cr*sy );
	matrix->m_matrix[5] = ( float )( srsp*sy+cr*cy );
	matrix->m_matrix[6] = ( float )( sr*cp );

	matrix->m_matrix[8] = ( float )( crsp*cy+sr*sy );
	matrix->m_matrix[9] = ( float )( crsp*sy-sr*cy );
	matrix->m_matrix[10] = ( float )( cr*cp );
}

void m_Matrix_setInverseRotationRadians(Matrix *matrix, const float *angles)
{
	double cr = Cos_Rad( angles[0] );
	double sr = Sin_Rad( angles[0] );
	double cp = Cos_Rad( angles[1] );
	double sp = Sin_Rad( angles[1] );
	double cy = Cos_Rad( angles[2] );
	double sy = Sin_Rad( angles[2] );
	double srsp = sr*sp;
	double crsp = cr*sp;

	matrix->m_matrix[0] = ( float )( cp*cy );
	matrix->m_matrix[4] = ( float )( cp*sy );
	matrix->m_matrix[8] = ( float )( -sp );

	matrix->m_matrix[1] = ( float )( srsp*cy-cr*sy );
	matrix->m_matrix[5] = ( float )( srsp*sy+cr*cy );
	matrix->m_matrix[9] = ( float )( sr*cp );

	matrix->m_matrix[2] = ( float )( crsp*cy+sr*sy );
	matrix->m_matrix[6] = ( float )( crsp*sy-sr*cy );
	matrix->m_matrix[10] = ( float )( cr*cp );
}

void m_Matrix_inverseRotateVect(Matrix *matrix, float *pVect)
{
	float vec[3];

	vec[0] = pVect[0]*matrix->m_matrix[0]+pVect[1]*matrix->m_matrix[1]+pVect[2]*matrix->m_matrix[2];
	vec[1] = pVect[0]*matrix->m_matrix[4]+pVect[1]*matrix->m_matrix[5]+pVect[2]*matrix->m_matrix[6];
	vec[2] = pVect[0]*matrix->m_matrix[8]+pVect[1]*matrix->m_matrix[9]+pVect[2]*matrix->m_matrix[10];
	memcpy( pVect, vec, sizeof( float )*3 );
}

void m_Matrix_inverseTranslateVect(Matrix *matrix, float *pVect)
{
	pVect[0] = pVect[0]-matrix->m_matrix[12];
	pVect[1] = pVect[1]-matrix->m_matrix[13];
	pVect[2] = pVect[2]-matrix->m_matrix[14];
}

void m_Matrix_setRotationQuaternion(Matrix *matrix, const Quaternion quat)
{
	matrix->m_matrix[0] = ( float )( 1.0 - 2.0*quat.m_quat[1]*quat.m_quat[1] - 2.0*quat.m_quat[2]*quat.m_quat[2] );
	matrix->m_matrix[1] = ( float )( 2.0*quat.m_quat[0]*quat.m_quat[1] + 2.0*quat.m_quat[3]*quat.m_quat[2] );
	matrix->m_matrix[2] = ( float )( 2.0*quat.m_quat[0]*quat.m_quat[2] - 2.0*quat.m_quat[3]*quat.m_quat[1] );

	matrix->m_matrix[4] = ( float )( 2.0*quat.m_quat[0]*quat.m_quat[1] - 2.0*quat.m_quat[3]*quat.m_quat[2] );
	matrix->m_matrix[5] = ( float )( 1.0 - 2.0*quat.m_quat[0]*quat.m_quat[0] - 2.0*quat.m_quat[2]*quat.m_quat[2] );
	matrix->m_matrix[6] = ( float )( 2.0*quat.m_quat[1]*quat.m_quat[2] + 2.0*quat.m_quat[3]*quat.m_quat[0] );

	matrix->m_matrix[8] = ( float )( 2.0*quat.m_quat[0]*quat.m_quat[2] + 2.0*quat.m_quat[3]*quat.m_quat[1] );
	matrix->m_matrix[9] = ( float )( 2.0*quat.m_quat[1]*quat.m_quat[2] - 2.0*quat.m_quat[3]*quat.m_quat[0] );
	matrix->m_matrix[10] = ( float )( 1.0 - 2.0*quat.m_quat[0]*quat.m_quat[0] - 2.0*quat.m_quat[1]*quat.m_quat[1] );
}

//
//	Quaternion stuff
//

void m_Quaternion_fromAngles( Quaternion *quat, const float *angles )
{
	float angle;
	double sr, sp, sy, cr, cp, cy;
	double crcp;
	double srsp;

	angle = angles[2]*0.5f;
	sy = Sin_Rad( angle );
	cy = Cos_Rad( angle );
	angle = angles[1]*0.5f;
	sp = Sin_Rad( angle );
	cp = Cos_Rad( angle );
	angle = angles[0]*0.5f;
	sr = Sin_Rad( angle );
	cr = Cos_Rad( angle );

	crcp = cr*cp;
	srsp = sr*sp;

	quat->m_quat[0] = ( float )( sr*cp*cy-cr*sp*sy );
	quat->m_quat[1] = ( float )( cr*sp*cy+sr*cp*sy );
	quat->m_quat[2] = ( float )( crcp*sy-srsp*cy );
	quat->m_quat[3] = ( float )( crcp*cy+srsp*sy ); 
}

void m_Quaternion_inverse(Quaternion *quat)
{
	quat->m_quat[0] = -quat->m_quat[0];
	quat->m_quat[1] = -quat->m_quat[1];
	quat->m_quat[2] = -quat->m_quat[2];
	quat->m_quat[3] = -quat->m_quat[3];
}

void m_Quaternion_slerp( Quaternion *quat, const Quaternion q1, Quaternion q2, float interp )
{
	// Decide if one of the quaternions is backwards
	int i;
	float a = 0, b = 0;
	float cosom;
	double sclq1, sclq2;
	double omega;
	double sinom;

	for ( i = 0; i < 4; i++ )
	{
		a += ( q1.m_quat[i]-q2.m_quat[i] )*( q1.m_quat[i]-q2.m_quat[i] );
		b += ( q1.m_quat[i]+q2.m_quat[i] )*( q1.m_quat[i]+q2.m_quat[i] );
	}
	if ( a > b )
		m_Quaternion_inverse(&q2);

	cosom = q1.m_quat[0]*q2.m_quat[0]+q1.m_quat[1]*q2.m_quat[1]+q1.m_quat[2]*q2.m_quat[2]+q1.m_quat[3]*q2.m_quat[3];

	if (( 1.0+cosom ) > 0.00000001 )
	{
		if (( 1.0-cosom ) > 0.00000001 )
		{
			omega = acos( (double)cosom );	// acos !!!!
			sinom = (double)Sin_Rad( (float)omega );
			sclq1 = (double)Sin_Rad(( 1.0f-interp )*(float)omega )/(float)sinom;
			sclq2 = (double)Sin_Rad( interp*(float)omega )/(float)sinom;
		}
		else
		{
			sclq1 = 1.0-interp;
			sclq2 = interp;
		}
		for ( i = 0; i < 4; i++ )
			quat->m_quat[i] = ( float )( sclq1*q1.m_quat[i]+sclq2*q2.m_quat[i] );
	}
	else
	{
		quat->m_quat[0] = -q1.m_quat[1];
		quat->m_quat[1] = q1.m_quat[0];
		quat->m_quat[2] = -q1.m_quat[3];
		quat->m_quat[3] = q1.m_quat[2];

		sclq1 = (double)Sin_Rad(( 1.0f-interp )*0.5f*(float)PI );
		sclq2 = (double)Sin_Rad( interp*0.5f*(float)PI );
		for ( i = 0; i < 3; i++ )
			quat->m_quat[i] = ( float )( sclq1*q1.m_quat[i]+sclq2*quat->m_quat[i] );
	}
}

vect_t m_RotateX(vect_t v, float angle)
{
	vect_t	rv;
	float	CA = Cos(angle);
	float	SA = Sin(angle);

//	rv.X = v.X;
//	rv.Y = v.Y*Cos(angle) + v.Z*Sin(angle); 
//	rv.Z = -v.Y*Sin(angle) + v.Z*Cos(angle);
	rv.X = v.X;
	rv.Y = v.Y*CA + v.Z*SA; 
	rv.Z = -v.Y*SA + v.Z*CA;
	
	return rv;
}

vect_t m_RotateY(vect_t v, float angle)
{
	vect_t	rv;
	float	CA = Cos(angle);
	float	SA = Sin(angle);

//	rv.X = v.X*Cos(angle) + v.Z*Sin(angle); 
//	rv.Y = v.Y;
//	rv.Z = -v.X*Sin(angle) + v.Z*Cos(angle);
	rv.X = v.X*CA + v.Z*SA; 
	rv.Y = v.Y;
	rv.Z = -v.X*SA + v.Z*CA;
	
	return rv;
}

vect_t m_RotateZ(vect_t v, float angle)
{
	vect_t rv;

	rv.X = v.X*Cos(angle) + v.Y*Sin(angle); 
	rv.Y = -v.X*Sin(angle) + v.Y*Cos(angle);
	rv.Z = v.Z;
	
	return rv;
}

void InitTrigoTables()
{
	int	i;

	for(i=0 ; i<360 ; i++)
	{
		Tsin[i] = (float)sin((double)((float)i*PIOVER180));
		Tcos[i] = (float)cos((double)((float)i*PIOVER180));
	}
}

float Sin_Rad(float alpha)
{
	/*
	int	Angle_Deg;

	Angle_Deg = (int)(alpha*INVPIOVER180);
	if(Angle_Deg < 0)
		Angle_Deg += 360;

	if( Angle_Deg >= 360 || Angle_Deg < 0)
		m_ConsPrint("Sinus argument bound error: %d\n", Angle_Deg);

	return Sin_i(Angle_Deg);
	*/
	return (float)sin(alpha);
}

float Cos_Rad(float alpha)
{
	/*
	int	Angle_Deg;

	Angle_Deg = (int)(alpha*INVPIOVER180);
	if(Angle_Deg < 0)
		Angle_Deg += 360;

	if( Angle_Deg >= 360 || Angle_Deg < 0)
		m_ConsPrint("Cosinus argument bound error: %d\n", Angle_Deg);

	return Cos_i(Angle_Deg);
	*/
	return (float)cos(alpha);
}

float Sin(float alpha)
{
	return Sin_i((int)alpha);
}

float Cos(float alpha)
{
	return Cos_i((int)alpha);
}

float Sin_i(int alpha)
{
	if(alpha<0)
	{
		alpha = -alpha;
		//alpha = alpha % 360;
		return -Tsin[alpha];
	}
	else
	{
		//alpha = alpha % 360;
		return Tsin[alpha];
	}
}

float Cos_i(int alpha)
{
	if(alpha<0)
		alpha = -alpha;
	//alpha = alpha % 360;
	return Tcos[alpha];
}

void m_Swapf(float *a, float *b)
{
	float tmp;

	tmp = *a;
	*a = *b;
	*b = tmp;
}

float m_Distance(vect_t M, vect_t P)
{
	vect_t	DistanceVect;

	DistanceVect.X = P.X - M.X;
	DistanceVect.Y = P.Y - M.Y;
	DistanceVect.Z = P.Z - M.Z;
	
	return (float)Norme(DistanceVect);
}

int ClassifyVect_offset(vect_t pos, Plan_t plan, float offset)
{
	float	result;
	vect_t	Direction;
	vect_t	PointOnPlane;

	PointOnPlane.X = plan.PointOnPlane.X - (offset * plan.VecteurNormal.X);
	PointOnPlane.Y = plan.PointOnPlane.Y - (offset * plan.VecteurNormal.Y);
	PointOnPlane.Z = plan.PointOnPlane.Z - (offset * plan.VecteurNormal.Z);

	Direction.X = PointOnPlane.X - pos.X;
	Direction.Y = PointOnPlane.Y - pos.Y;
	Direction.Z = PointOnPlane.Z - pos.Z;

	result = PdtScalaire(&plan.VecteurNormal,&Direction);

	if(result>epsilon.value)
		return DEVANT;
	if(result<-epsilon.value)
		return DERRIERE;
	return CONFONDU;
}

int ClassifyVect(vect_t *pos, Plan_t *Plan)
{
	float	result;
	vect_t	Direction;

	Direction.X = Plan->PointOnPlane.X - pos->X;
	Direction.Y = Plan->PointOnPlane.Y - pos->Y;
	Direction.Z = Plan->PointOnPlane.Z - pos->Z;

	result = PdtScalaire(&Plan->VecteurNormal,&Direction);

	if(result>epsilon.value)
		return DEVANT;
	if(result<-epsilon.value)
		return DERRIERE;
	return CONFONDU;
}

void ClearBound(BOUNDINGBOX *bbox)
{
	bbox->BoxMin.X = MAX_BOUND;
	bbox->BoxMin.Y = MAX_BOUND;
	bbox->BoxMin.Z = MAX_BOUND;

	bbox->BoxMax.X = MIN_BOUND;
	bbox->BoxMax.Y = MIN_BOUND;
	bbox->BoxMax.Z = MIN_BOUND;
}

hBool IsValidBound(BOUNDINGBOX *bbox)
{
	if(bbox->BoxMin.X == MAX_BOUND)
		return false;
	if(bbox->BoxMin.Y == MAX_BOUND)
		return false;
	if(bbox->BoxMin.Z == MAX_BOUND)
		return false;
	if(bbox->BoxMax.X == MIN_BOUND)
		return false;
	if(bbox->BoxMax.Y == MIN_BOUND)
		return false;
	if(bbox->BoxMax.Z == MIN_BOUND)
		return false;

	if(bbox->BoxMin.X == bbox->BoxMax.X && bbox->BoxMin.Y == bbox->BoxMax.Y && bbox->BoxMin.Z == bbox->BoxMax.Z)
		return false;

	return true;
}

hBool IsInBox(vect_t pos, BOUNDINGBOX box)
{
	if(pos.X < box.BoxMin.X || pos.X > box.BoxMax.X)
		return false;
	if(pos.Y < box.BoxMin.Y || pos.Y > box.BoxMax.Y)
		return false;
	if(pos.Z < box.BoxMin.Z || pos.Z > box.BoxMax.Z)
		return false;
	return true;
}

void CalculateBox(BOUNDINGBOX *Box, Face_t *Polylist)
{
	Face_t	*fListe=NULL;
	int i;

	fListe = Polylist;

	while(fListe)
	{
		for (i=0 ; i<fListe->NumberOfVertices ; i++)
		{
			if(fListe->Vertice[i].X < Box->BoxMin.X) Box->BoxMin.X = fListe->Vertice[i].X;
			if(fListe->Vertice[i].Y < Box->BoxMin.Y) Box->BoxMin.Y = fListe->Vertice[i].Y;
			if(fListe->Vertice[i].Z < Box->BoxMin.Z) Box->BoxMin.Z = fListe->Vertice[i].Z;
			if(fListe->Vertice[i].X > Box->BoxMax.X) Box->BoxMax.X = fListe->Vertice[i].X;
			if(fListe->Vertice[i].Y > Box->BoxMax.Y) Box->BoxMax.Y = fListe->Vertice[i].Y;
			if(fListe->Vertice[i].Z > Box->BoxMax.Z) Box->BoxMax.Z = fListe->Vertice[i].Z; 
		}
		fListe=fListe->Next;
	}
}

float SquareNorme(vect_t v)
{
 	return (v.X*v.X + v.Y*v.Y + v.Z*v.Z);
}

double Norme(vect_t v)
{
 	return sqrt(v.X*v.X + v.Y*v.Y + v.Z*v.Z);
}

void Normalize(vect_t *pVect)
{
    float length, len;
  
	length = (float)Norme(*pVect);
	len=1.0f/length;
  
	pVect->X *= len; 
	pVect->Y *= len; 
	pVect->Z *= len;
}

float PdtScalaire(vect_t *pVect1, vect_t *pVect2)
{
	return (pVect1->X*pVect2->X + pVect1->Y*pVect2->Y + pVect1->Z*pVect2->Z);
}

void PdVectoriel(vect_t *Vres, vect_t *v1, vect_t *v2)
{
	Vres->X = (v1->Y * v2->Z) - (v1->Z * v2->Y);
	Vres->Y = (v1->Z * v2->X) - (v1->X * v2->Z);
	Vres->Z = (v1->X * v2->Y) - (v1->Y * v2->X);
}

vect_t m_GetFaceNormal_vect(vect_t Vec0, vect_t Vec1, vect_t Vec2)
{
	vect_t	p, q, n;

	p.X = Vec1.X - Vec0.X;
	p.Y = Vec1.Y - Vec0.Y;
	p.Z = Vec1.Z - Vec0.Z;
		
	q.X = Vec2.X - Vec0.X;  
	q.Y = Vec2.Y - Vec0.Y;  
	q.Z = Vec2.Z - Vec0.Z;  
		
	PdVectoriel(&n,&p,&q);
	Normalize(&n);

	return n;
}

vect_t GetFaceNormal(Point_t Vec0, Point_t Vec1, Point_t Vec2)
{
	vect_t	v1, v2, v3;

	v1.X = Vec0.X;
	v1.Y = Vec0.Y;
	v1.Z = Vec0.Z;

	v2.X = Vec1.X;
	v2.Y = Vec1.Y;
	v2.Z = Vec1.Z;

	v3.X = Vec2.X;
	v3.Y = Vec2.Y;
	v3.Z = Vec2.Z;

	return m_GetFaceNormal_vect(v1,v2,v3);
}

float m_DistancePlan(plan_t Plan, vect_t pos)
{
	vect_t	Direction;

	Direction.X = Plan.PointOnPlane.X - pos.X;
	Direction.Y = Plan.PointOnPlane.Y - pos.Y;
	Direction.Z = Plan.PointOnPlane.Z - pos.Z;

	return PdtScalaire(&Plan.VecteurNormal, &Direction);
}

float DistancePlan(Plan_t *pPlan, Point_t *pPoint)
{
	vect_t	Direction;

	Direction.X = pPlan->PointOnPlane.X - pPoint->X;
	Direction.Y = pPlan->PointOnPlane.Y - pPoint->Y;
	Direction.Z = pPlan->PointOnPlane.Z - pPoint->Z;

	return PdtScalaire(&pPlan->VecteurNormal, &Direction);
}

hBool FastCull(Face_t *pFace, vect_t EyePos)
{
	vect_t	Dummy;

	if(culling.value == false)
		return true;

	Dummy.X = pFace->Vertice[0].X - EyePos.X;
	Dummy.Y = pFace->Vertice[0].Y - EyePos.Y;
	Dummy.Z = pFace->Vertice[0].Z - EyePos.Z;

	if( PdtScalaire(&Dummy, &pFace->Plan.VecteurNormal) > epsilon.value)
		return false;
	else
		return true;
}

hBool FastCull_Ent(pEntity Entity, pEntity PlayerEnt)
{
	vect_t	Dummy;
	vect_t	PlayerViewNormal;

	PlayerViewNormal.X = -Sin(PlayerEnt->ent_anim.Rot.Y);
	PlayerViewNormal.Y = 0;
	PlayerViewNormal.Z = -Cos(PlayerEnt->ent_anim.Rot.Y);

	Dummy.X = Entity->FaceList->Vertice[0].X - PlayerEnt->ent_anim.Pos.X;
	Dummy.Y = Entity->FaceList->Vertice[0].Y - PlayerEnt->ent_anim.Pos.Y;
	Dummy.Z = Entity->FaceList->Vertice[0].Z - PlayerEnt->ent_anim.Pos.Z;

	if( PdtScalaire(&Dummy, &PlayerViewNormal) > epsilon.value)
		return false;
	else
		return true;
}

float A_absf(float n)
{
	if(n<0)
		return -n;
	else
		return n;
}

float m_MaxValuef(float a, float b)
{
	if(a>b)
		return a;
	else
		return b;
}

//#define	STOP_EPSILON	0.001

vect_t VectorBounce(vect_t *in, vect_t normal, float step)
{
	float	val;
	vect_t	out;

	val = PdtScalaire(in,&normal) * step;

	out.X = in->X - val*normal.X;
	out.Y = in->Y - val*normal.Y;
	out.Z = in->Z - val*normal.Z;

//	if(out.X > -STOP_EPSILON && out.X < STOP_EPSILON)
//		out.X = 0;
//	if(out.Y > -STOP_EPSILON && out.Y < STOP_EPSILON)
//		out.Y = 0;
//	if(out.Z > -STOP_EPSILON && out.Z < STOP_EPSILON)
//		out.Z = 0;

	return out;
}

hBool m_pointInTri(vect_t pNormal, vect_t fp1, vect_t fp2, vect_t fp3, vect_t CP)
{
	int		i1;
	int		i2;
	float	a;
	float	b;
	vect_t	u;
	vect_t	v;
//	vect_t	n;
	vect_t	A;
	float	v1[3];
	float	v2[3];
	float	v3[3];
	float	P[3];

	P[0] = CP.X;
	P[1] = CP.Y;
	P[2] = CP.Z;

	v1[0] = fp1.X;
	v1[1] = fp1.Y;
	v1[2] = fp1.Z;

	v2[0] = fp2.X;
	v2[1] = fp2.Y;
	v2[2] = fp2.Z;

	v3[0] = fp3.X;
	v3[1] = fp3.Y;
	v3[2] = fp3.Z;

 //   n = Face->Plan.VecteurNormal;

	A.X = (float)fabs(pNormal.X);
	A.Y = (float)fabs(pNormal.Y);
	A.Z = (float)fabs(pNormal.Z);

	if(A.X>A.Y)
	{
		if(A.X>A.Z)
		{
			i1=1;
			i2=2;
		}
		else
		{
			i1=0;
			i2=1;
		}
	}
	else
	{
		if(A.Y>A.Z)
		{
			i1=0;
			i2=2;
		}
		else
		{
			i1=0;
			i2=1;
		}
	}

	u.X = P[i1]-v1[i1];
	u.Y = v2[i1]-v1[i1];
	u.Z = v3[i1]-v1[i1];

	v.X = P[i2]-v1[i2];
	v.Y = v2[i2]-v1[i2];
	v.Z = v3[i2]-v1[i2];
	
    if (u.Y==0.0f)
    {
      b=u.X/u.Z;
      if(b>=0.0f && b<=1.0f)
		  a=(v.X-b*v.Z)/v.Y;
      else
		  return false;
    }
    else
    {
      b=(v.X*u.Y-u.X*v.Y)/(v.Z*u.Y-u.Z*v.Y);
      if (b>=0.0f && b<=1.0f)
		  a=(u.X-b*u.Z)/u.Y;
      else
		  return false;
    }
    return (a>=0 && (a+b)<=1);
}

int randInt( int low, int high )
{
    int range = high - low;
    int num = rand() % range;

    return( num + low );
}

float A_atof(char *str)
{
	double			val;
	int             sign;
	int             c;
	int             decimal, total;
	
	if (*str == '-')
	{
		sign = -1;
		str++;
	}
	else
		sign = 1;
		
	val = 0;

//
// check for hex
//
	if (str[0] == '0' && (str[1] == 'x' || str[1] == 'X') )
	{
		str += 2;
		while (1)
		{
			c = *str++;
			if (c >= '0' && c <= '9')
				val = (val*16) + c - '0';
			else if (c >= 'a' && c <= 'f')
				val = (val*16) + c - 'a' + 10;
			else if (c >= 'A' && c <= 'F')
				val = (val*16) + c - 'A' + 10;
			else
				return (float)(val*sign);
		}
	}
	
//
// check for character
//
	if (str[0] == '\'')
	{
		return (float)(sign * str[1]);
	}
	
//
// assume decimal
//
	decimal = -1;
	total = 0;
	while (1)
	{
		c = *str++;
		if (c == '.')
		{
			decimal = total;
			continue;
		}
		if (c <'0' || c > '9')
			break;
		val = val*10 + c - '0';
		total++;
	}

	if (decimal == -1)
		return (float)(val*sign);
	while (total > decimal)
	{
		val /= 10;
		total--;
	}
	
	return (float)(val*sign);
}

int A_strlen(char *str)
{
	int             count;
	
	count = 0;
	while (str[count])
		count++;

	return count;
}

