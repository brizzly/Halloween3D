
#include "alloween.cpp"


#ifdef H_LINUX

int stricmp( const char *s1, const char *s2 )
{
     while( *s1 && *s2 )
     {
         if ( tolower(*s1) != tolower(*s2) )     
		 return 1;
         s1++;
         s2++;
     }
     if ( *s1 || *s2 ) return 1;
        return 0;
}

#endif

#ifdef H_MAC

#include <ctype.h>

int stricmp( const char *s1, const char *s2 )
{
     while( *s1 && *s2 )
     {
         if ( tolower(*s1) != tolower(*s2) )     
		 return 1;
         s1++;
         s2++;
     }
     if ( *s1 || *s2 ) return 1;
        return 0;
}

#endif

#ifdef H_WINDOWS
#define	BYTE_ORDER	1
#define	BIG_ENDIAN	2
#endif




hBool msk_LoadData(Model *msk_model, char *name)
{
	char	path_model[256];
	char	path_run[256];
	char	path_attack[256];
	char	path_die[256];
	hBool	res_model=true;
	hBool	res_think=true;
	hBool	res_run=true;
	hBool	res_attack=true;
	hBool	res_die=true;

	memset(path_model,0,256);
	memset(path_run,0,256);
	memset(path_attack,0,256);
	memset(path_die,0,256);

	sprintf(path_model,"%s%s%s%s%s%s",SYSDIR,MDLDIR,name,"/",name,".ms3d");
	sprintf(path_run,"%s%s%s%s%s%s",SYSDIR,MDLDIR,name,"/",name,"_run.ms3d");
	sprintf(path_attack,"%s%s%s%s%s%s",SYSDIR,MDLDIR,name,"/",name,"_attack.ms3d");
	sprintf(path_die,"%s%s%s%s%s%s",SYSDIR,MDLDIR,name,"/",name,"_die.ms3d");

	msk_InitModel(msk_model);
	res_model = msk_LoadModel(msk_model,path_model,true);
	res_think = msk_LoadModel(msk_model,path_model,false);
	res_run = msk_LoadModel(msk_model,path_run,false);
	res_attack = msk_LoadModel(msk_model,path_attack,false);
	res_die = msk_LoadModel(msk_model,path_die,false);

	msk_model->m_indexBoneAnim[0] = 0;
	
	if(!res_model || !res_think || !res_run || !res_attack || !res_die)
		return false;
	return true;
}

void msk_InitModel(Model *msk_model)
{
	int	i;
	int	j;

	msk_model->m_numMeshes = 0;
	msk_model->m_pMeshes = NULL;
	msk_model->m_numMaterials = 0;
	msk_model->m_pMaterials = NULL;
	msk_model->m_numTriangles = 0;
	msk_model->m_pTriangles = NULL;
	msk_model->m_numVertices = 0;
	msk_model->m_pVertices = NULL;
	msk_model->m_numBoneAnim = 0;
	for(i=0 ; i<MAX_CLONEMODEL ; i++)
	{
		msk_model->m_indexBoneAnim[i] = 0;
		for(j=0 ; j<MAX_ANIMMODEL ; j++)
		{
			msk_model->BoneAnim[i][j] = NULL;
		}
	}
}

hBool msk_LoadModel(Model *MilkshapeModel, char *filename, hBool LoadMesh)
{
	FILE		*file;
	byte		*pBuffer;
	long		fileSize=500000;
	const	byte	*pPtr;
	int		i, j, k;
	int		nVertices;
	int		nTriangles;
	int		vertexIndices[3];
	float		t[3];
	int		nGroups;
	int		*pTriangleIndices = NULL;
	char		materialIndex;
	int		nMaterials;
	int		parentIndex = -1;
	char		TextureName[255];
	float		animFPS;
	int		totalFrames;
	const	byte	*pTempPtr;
	JointNameLstRec	*pNameList;
	MS3DHeader	*pHeader;
	MS3DTriangle	*pTriangle;
	MS3DVertex	*pVertex;
	MS3DJoint	*pJoint1;
	MS3DJoint	*pJoint2;
	MS3DKeyframe	*pKeyframe1;
	MS3DKeyframe	*pKeyframe2;
	MS3DMaterial	*pMaterial;
	int		maID;
	int		AnimINDEX;
	int		loopvar, loopvar1, loopvar2;

	m_ConsPrint("Load ModelData: %s\n",filename);

	//menu_DispLoadMapSplash();
	//g_DispCenterString(14, 3, "Loading.. %s", filename);
	//gl_SwapBuffer();

#ifdef H_MAC
	filename = KMiscTools::makeFilePath(filename);
	file = fopen(filename,"r");
#else
	file = fopen(filename,"r+b");
#endif

	if(file==NULL)
	{
		return false;
	}
	pBuffer = (byte*)s_malloc(sizeof(byte)*fileSize);
	fread(pBuffer,sizeof(byte),fileSize,file);
	fclose(file);

	pPtr = pBuffer;
	pHeader = ( MS3DHeader* )pPtr;
	pPtr += sizeof( MS3DHeader );

	if ( strncmp( pHeader->m_ID, "MS3D000000", 10 ) != 0 )
	{
		m_ConsPrint("Not a valid Milkshape3D model file.\n");
		return false;
	}

#if BYTE_ORDER == BIG_ENDIAN
        pHeader->m_version = SWAPEND32( pHeader->m_version );
        //printf("pHeader->m_version: %d\n",  pHeader->m_version);
#endif

	if ( pHeader->m_version < 3 || pHeader->m_version > 4 )
	{
		m_ConsPrint("warning: MilkShape version: %d\n",pHeader->m_version);
		return false;
	}
	
#if BYTE_ORDER == BIG_ENDIAN
    nVertices = SWAPEND16( *(word *) pPtr );
    //printf("nVertices: %d\n", nVertices);
#else
    nVertices = *( word* )pPtr;
#endif     
        
	if(LoadMesh)
	{
		MilkshapeModel->m_numVertices = nVertices;
		MilkshapeModel->m_pVertices = (Vertex*)s_malloc(sizeof(Vertex)*nVertices);
	}
	pPtr += sizeof( word );

	for(i=0 ; i<nVertices; i++)
	{
		pVertex = ( MS3DVertex* )pPtr;
		if(LoadMesh)
		{
			MilkshapeModel->m_pVertices[i].m_boneID = pVertex->m_boneID;
                        
#if BYTE_ORDER == BIG_ENDIAN
                    for(loopvar=0; loopvar<3; loopvar++)
                    {
                        MilkshapeModel->m_pVertices[i].m_location[loopvar] = swapendf32( pVertex->m_vertex[loopvar] );
                        //printf("m_location: %f\n",  MilkshapeModel->m_pVertices[i].m_location[loopvar]);
                    }
#else
                    memcpy( MilkshapeModel->m_pVertices[i].m_location, pVertex->m_vertex, sizeof( float )*3 );
#endif
                }
		pPtr += sizeof( MS3DVertex );
	}

#if BYTE_ORDER == BIG_ENDIAN
        nTriangles = SWAPEND16( *(word *) pPtr );
        //printf("nTriangles: %d\n", nTriangles);
#else
	nTriangles = *( word* )pPtr;
#endif

	if(LoadMesh)
	{
		MilkshapeModel->m_numTriangles = nTriangles;
		MilkshapeModel->m_pTriangles = (Triangle*)s_malloc(sizeof(Triangle)*nTriangles);
	}
	pPtr += sizeof( word );

	for(i=0; i<nTriangles; i++)
	{
            pTriangle = ( MS3DTriangle* )pPtr;

#if BYTE_ORDER == BIG_ENDIAN

            vertexIndices[0] = SWAPEND16( pTriangle->m_vertexIndices[0] );
            vertexIndices[1] = SWAPEND16( pTriangle->m_vertexIndices[1] );
            vertexIndices[2] = SWAPEND16( pTriangle->m_vertexIndices[2] );
         
            t[0] = 1.0f-swapendf32( pTriangle->m_t[0] );
            t[1] = 1.0f-swapendf32( pTriangle->m_t[1] );
            t[2] = 1.0f-swapendf32( pTriangle->m_t[2] );
        
            if(LoadMesh)
            {
                for(loopvar1 = 0; loopvar1 < 3; loopvar1++ )
                {
                    for(loopvar2 = 0; loopvar2 < 3; loopvar2++ )
                        MilkshapeModel->m_pTriangles[i].m_vertexNormals[ loopvar1 ][ loopvar2 ] = swapendf32( pTriangle->m_vertexNormals[ loopvar1 ][ loopvar2 ] );
                    MilkshapeModel->m_pTriangles[i].m_s[ loopvar1 ] = swapendf32( pTriangle->m_s[ loopvar1 ] );
                }
            }
#else
            vertexIndices[0] = pTriangle->m_vertexIndices[0];
            vertexIndices[1] = pTriangle->m_vertexIndices[1];
            vertexIndices[2] = pTriangle->m_vertexIndices[2];
		
            t[0] = 1.0f-pTriangle->m_t[0];
            t[1] = 1.0f-pTriangle->m_t[1];
            t[2] = 1.0f-pTriangle->m_t[2];

            if(LoadMesh)
            {
                memcpy( MilkshapeModel->m_pTriangles[i].m_vertexNormals, pTriangle->m_vertexNormals, sizeof( float )*3*3 );
                memcpy( MilkshapeModel->m_pTriangles[i].m_s, pTriangle->m_s, sizeof( float )*3 );
            }
#endif
                
            if(LoadMesh)
            {
		memcpy( MilkshapeModel->m_pTriangles[i].m_t, t, sizeof( float )*3 );
		memcpy( MilkshapeModel->m_pTriangles[i].m_vertexIndices, vertexIndices, sizeof( int )*3 );
            }
            pPtr += sizeof( MS3DTriangle );
	}

#if BYTE_ORDER == BIG_ENDIAN
    nGroups = SWAPEND16( *(word *) pPtr );
#else
    nGroups = *( word* )pPtr;
#endif
	
	if(LoadMesh)
	{
		MilkshapeModel->m_numMeshes = nGroups;
		MilkshapeModel->m_pMeshes = (Mesh*)s_malloc(sizeof(Mesh)*nGroups);
	}
	pPtr += sizeof( word );
        
	for(i=0 ; i<nGroups; i++ )
	{
		pPtr += sizeof( byte );
		pPtr += 32;

#if BYTE_ORDER == BIG_ENDIAN
    nTriangles = SWAPEND16( *(word *) pPtr );
#else
    nTriangles = *( word* )pPtr;
#endif

		pPtr += sizeof( word );
		if(LoadMesh)
                {
			pTriangleIndices = (int*)s_malloc(sizeof(int)*nTriangles);
                }
		for(j=0 ; j<nTriangles; j++)
		{
			if(LoadMesh)
                        {
#if BYTE_ORDER == BIG_ENDIAN
                            pTriangleIndices[j] = SWAPEND16( *(word *) pPtr );
#else
                            pTriangleIndices[j] = *( word* )pPtr;
#endif			
                        }
			pPtr += sizeof( word );
		}

		materialIndex = *( char* )pPtr;
		pPtr += sizeof( char );
	
		if(LoadMesh)
		{
			MilkshapeModel->m_pMeshes[i].m_materialIndex = materialIndex;
			MilkshapeModel->m_pMeshes[i].m_numTriangles = nTriangles;
			MilkshapeModel->m_pMeshes[i].m_pTriangleIndices = pTriangleIndices;
		}
	}

#if BYTE_ORDER == BIG_ENDIAN
    nMaterials = SWAPEND16( *(word *) pPtr );
#else
    nMaterials = *( word* )pPtr;
#endif

	if(LoadMesh)
	{
		MilkshapeModel->m_numMaterials = nMaterials;
		MilkshapeModel->m_pMaterials = (Material*)s_malloc(sizeof(Material)*nMaterials);
	}
	pPtr += sizeof( word );
	for(i=0 ; i<nMaterials ; i++)
	{
		if(LoadMesh)
		{
			pMaterial = ( MS3DMaterial* )pPtr;

#if BYTE_ORDER == BIG_ENDIAN
                        for(loopvar = 0; loopvar < 4; loopvar++ )
                        {
                            MilkshapeModel->m_pMaterials[i].m_ambient[ loopvar ] = swapendf32( pMaterial->m_ambient[ loopvar ] );
                            MilkshapeModel->m_pMaterials[i].m_diffuse[ loopvar ] = swapendf32( pMaterial->m_diffuse[ loopvar ] );
                            MilkshapeModel->m_pMaterials[i].m_specular[ loopvar ] = swapendf32( pMaterial->m_specular[ loopvar ] );
                            MilkshapeModel->m_pMaterials[i].m_emissive[ loopvar ] = swapendf32( pMaterial->m_emissive[ loopvar ] );
                        }
                        MilkshapeModel->m_pMaterials[i].m_shininess = swapendf32( pMaterial->m_shininess );
#else  
			memcpy( MilkshapeModel->m_pMaterials[i].m_ambient, pMaterial->m_ambient, sizeof( float )*4 );
			memcpy( MilkshapeModel->m_pMaterials[i].m_diffuse, pMaterial->m_diffuse, sizeof( float )*4 );
			memcpy( MilkshapeModel->m_pMaterials[i].m_specular, pMaterial->m_specular, sizeof( float )*4 );
			memcpy( MilkshapeModel->m_pMaterials[i].m_emissive, pMaterial->m_emissive, sizeof( float )*4 );
			MilkshapeModel->m_pMaterials[i].m_shininess = pMaterial->m_shininess;
#endif
                        
			MilkshapeModel->m_pMaterials[i].m_pTextureFilename = (char*)s_malloc(sizeof(char)*(strlen( pMaterial->m_texture )+1));						strcpy( MilkshapeModel->m_pMaterials[i].m_pTextureFilename, pMaterial->m_texture );
		}
		pPtr += sizeof( MS3DMaterial );

		if(LoadMesh)
		{
			fs_GetFileNameFromPath(MilkshapeModel->m_pMaterials[i].m_pTextureFilename,TextureName);
			m_ConsPrint("%s\n",TextureName);
			MilkshapeModel->m_pMaterials[i].m_texture = im_Load(TextureName,-1, (int)picmip.value, false);
		}
	}

	if(LoadMesh)
	{
		s_free(pBuffer);
		return true;
	}
	
	maID = 0;
	AnimINDEX = MilkshapeModel->m_indexBoneAnim[maID];
        
        //printf("AnimINDEX: %d\n", AnimINDEX);

	if(AnimINDEX >= MAX_ANIMMODEL)
	{	
		m_ConsPrint("You are trying to load too much BoneAnims to the structure.\n");
		return false;
	}

	MilkshapeModel->BoneAnim[maID][AnimINDEX] = (Anim*)s_malloc(sizeof(Anim));
	memset(MilkshapeModel->BoneAnim[maID][AnimINDEX],0,sizeof(Anim));

	MilkshapeModel->BoneAnim[maID][AnimINDEX]->m_numJoints = 0;
	MilkshapeModel->BoneAnim[maID][AnimINDEX]->m_pJoints = NULL;
	MilkshapeModel->BoneAnim[maID][AnimINDEX]->m_pTimer = (Timer*)s_malloc(sizeof(Timer));
	memset(MilkshapeModel->BoneAnim[maID][AnimINDEX]->m_pTimer,0,sizeof(Timer));

	MilkshapeModel->BoneAnim[maID][AnimINDEX]->m_looping = true;
        
#if BYTE_ORDER == BIG_ENDIAN
        animFPS = swapendf32( *( float* )pPtr );
#else
	animFPS = *( float* )pPtr;
#endif
        
    //    printf("animFPS: %f\n", animFPS);
        
	pPtr += sizeof( float );
	pPtr += sizeof( float );
        
#if BYTE_ORDER == BIG_ENDIAN
        totalFrames = SWAPEND32( *( int* )pPtr );
#else
	totalFrames = *( int* )pPtr;
#endif
        
      //  printf("totalFrames: %d\n", totalFrames);
         
	pPtr += sizeof( int );
	MilkshapeModel->BoneAnim[maID][AnimINDEX]->m_totalTime = ((float)totalFrames*1000.0f)/animFPS;
#if BYTE_ORDER == BIG_ENDIAN
	MilkshapeModel->BoneAnim[maID][AnimINDEX]->m_numJoints = SWAPEND16( *( word* )pPtr );
#else
        MilkshapeModel->BoneAnim[maID][AnimINDEX]->m_numJoints = *( word* )pPtr;
#endif

     //   printf("numjoints: %d\n", MilkshapeModel->BoneAnim[maID][AnimINDEX]->m_numJoints);
        
	pPtr += sizeof( word );
	MilkshapeModel->BoneAnim[maID][AnimINDEX]->m_pJoints = (Joint*)s_malloc(sizeof(Joint)*MilkshapeModel->BoneAnim[maID][AnimINDEX]->m_numJoints);
	memset(MilkshapeModel->BoneAnim[maID][AnimINDEX]->m_pJoints,0,sizeof(Joint)*MilkshapeModel->BoneAnim[maID][AnimINDEX]->m_numJoints);
	pTempPtr = pPtr;
	pNameList = (JointNameLstRec*)s_malloc(sizeof(JointNameLstRec)*MilkshapeModel->BoneAnim[maID][AnimINDEX]->m_numJoints);
	for(i=0 ; i<MilkshapeModel->BoneAnim[maID][AnimINDEX]->m_numJoints; i++)
	{
		pJoint1 = ( MS3DJoint* )pTempPtr;
                
           #if BYTE_ORDER == BIG_ENDIAN
                pJoint1->m_numRotationKeyframes = SWAPEND16( pJoint1->m_numRotationKeyframes );
                pJoint1->m_numTranslationKeyframes = SWAPEND16( pJoint1->m_numTranslationKeyframes );
                
          //      printf("numrotkeys1: %d\n", pJoint1->m_numRotationKeyframes);
          //      printf("numtrankeys1: %d\n", pJoint1->m_numTranslationKeyframes);
                
                for(j=0 ; j<3 ; j++)
                {
                    pJoint1->m_rotation[j] = swapendf32( pJoint1->m_rotation[j] );
                    pJoint1->m_translation[j] = swapendf32( pJoint1->m_translation[j] );
          //          printf("## %f %f\n", pJoint1->m_rotation[j], pJoint1->m_translation[j]);
                }
          #endif
                
		pTempPtr += sizeof( MS3DJoint );
		pTempPtr += sizeof( MS3DKeyframe )*( pJoint1->m_numRotationKeyframes+pJoint1->m_numTranslationKeyframes );

		pNameList[i].m_jointIndex = i;
		pNameList[i].m_pName = pJoint1->m_name;
                
           //     printf("m_pName1: %s\n", pNameList[i].m_pName);
	}

	for(i=0 ; i<MilkshapeModel->BoneAnim[maID][AnimINDEX]->m_numJoints; i++)
	{
		pJoint2 = ( MS3DJoint* )pPtr;
		pPtr += sizeof( MS3DJoint );
                
		if ( strlen( pJoint2->m_parentName ) > 0 )
		{
			for(j=0 ; j<MilkshapeModel->BoneAnim[maID][AnimINDEX]->m_numJoints; j++)
			{
				if ( stricmp( pNameList[j].m_pName, pJoint2->m_parentName ) == 0 )
				{
					parentIndex = pNameList[j].m_jointIndex;
					break;
				}
			}
			if(parentIndex == -1)
			{
				m_ConsPrint("msk_LoadModel : Unable to find parent bone in MS3D file");
				return false;
			}
		}

		memcpy( MilkshapeModel->BoneAnim[maID][AnimINDEX]->m_pJoints[i].m_localRotation, pJoint2->m_rotation, sizeof(float)*3);
		memcpy( MilkshapeModel->BoneAnim[maID][AnimINDEX]->m_pJoints[i].m_localTranslation, pJoint2->m_translation, sizeof(float)*3);
		MilkshapeModel->BoneAnim[maID][AnimINDEX]->m_pJoints[i].m_parent = parentIndex;
		MilkshapeModel->BoneAnim[maID][AnimINDEX]->m_pJoints[i].m_numRotationKeyframes = pJoint2->m_numRotationKeyframes;
		MilkshapeModel->BoneAnim[maID][AnimINDEX]->m_pJoints[i].m_pRotationKeyframes = (Kframe*)s_malloc(sizeof(Kframe)*pJoint2->m_numRotationKeyframes);
		MilkshapeModel->BoneAnim[maID][AnimINDEX]->m_pJoints[i].m_numTranslationKeyframes = pJoint2->m_numTranslationKeyframes;
		MilkshapeModel->BoneAnim[maID][AnimINDEX]->m_pJoints[i].m_pTranslationKeyframes = (Kframe*)s_malloc(sizeof(Kframe)*pJoint2->m_numTranslationKeyframes);


		for(j=0 ; j<pJoint2->m_numRotationKeyframes; j++)
		{
			pKeyframe1 = (MS3DKeyframe*)pPtr;
			pPtr += sizeof(MS3DKeyframe);
                        
                        #if BYTE_ORDER == BIG_ENDIAN
                        pKeyframe1->m_time = swapendf32( pKeyframe1->m_time );
                        
                    //    printf("keyfram1: %f\n", pKeyframe1->m_time);
                        
                        for(k=0 ; k<3 ; k++)
                        {
                            pKeyframe1->m_parameter[k] = swapendf32( pKeyframe1->m_parameter[k] );
                    //        printf("mpara1 %f\n", pKeyframe1->m_parameter[k]);
                        }
                        #endif

			msk_setJointKeyframe(MilkshapeModel,i,j,AnimINDEX,pKeyframe1->m_time*1000.0f,pKeyframe1->m_parameter, true );
		}

		for(j=0 ; j<pJoint2->m_numTranslationKeyframes; j++)
		{
			pKeyframe2 = (MS3DKeyframe*)pPtr;
			pPtr += sizeof(MS3DKeyframe);
                        
                        #if BYTE_ORDER == BIG_ENDIAN
                        pKeyframe2->m_time = swapendf32( pKeyframe2->m_time );
                        
                   //     printf("keyfram2: %f\n", pKeyframe2->m_time);
                         
                        for(k=0 ; k<3 ; k++)
                        {
                            pKeyframe2->m_parameter[k] = swapendf32( pKeyframe2->m_parameter[k] );
                   //         printf("mpara2 %f\n", pKeyframe2->m_parameter[k]);
                        }
                        #endif
                        
			msk_setJointKeyframe(MilkshapeModel,i,j,AnimINDEX,pKeyframe2->m_time*1000.0f,pKeyframe2->m_parameter, false );
		}

	}

	s_free(pNameList);
	msk_setupJoints(MilkshapeModel,AnimINDEX);
	MilkshapeModel->m_indexBoneAnim[maID]++;

	s_free(pBuffer);
	return true;
}

void msk_setJointKeyframe(Model *model, int jointIndex, int keyframeIndex, int animIndex, float time, float *parameter, hBool isRotation)
{
	Kframe *keyframe;

	if(isRotation)
		keyframe = &(model->BoneAnim[0][animIndex]->m_pJoints[jointIndex].m_pRotationKeyframes[keyframeIndex]);
	else
		keyframe = &(model->BoneAnim[0][animIndex]->m_pJoints[jointIndex].m_pTranslationKeyframes[keyframeIndex]);

	keyframe->m_jointIndex = jointIndex;
	keyframe->m_time = time;
	memcpy(keyframe->m_parameter,parameter,sizeof(float)*3);
}

void msk_setupJoints(Model *model, int animIndex)
{
	int		i;
	Vertex	*vertex;
	Matrix	matrix;
	Joint	*joint;

	for(i=0 ; i<model->BoneAnim[0][animIndex]->m_numJoints ; i++)
	{
		joint = &(model->BoneAnim[0][animIndex]->m_pJoints[i]);
		
		m_Matrix_setRotationRadians(&joint->m_relative, joint->m_localRotation);
		m_Matrix_setTranslation(&joint->m_relative, joint->m_localTranslation);
		
		if(joint->m_parent != -1)
		{
			m_Matrix_set(&joint->m_absolute, model->BoneAnim[0][animIndex]->m_pJoints[joint->m_parent].m_absolute.m_matrix );
			m_Matrix_postMultiply(&joint->m_absolute, joint->m_relative);
		}
		else
			m_Matrix_set(&joint->m_absolute, joint->m_relative.m_matrix);
	}

	if(animIndex == 0)
	for(i=0 ; i<model->m_numVertices; i++)
	{
		vertex = &(model->m_pVertices[i]);

		if(vertex->m_boneID != -1)
		{
			matrix = model->BoneAnim[0][animIndex]->m_pJoints[(int)vertex->m_boneID].m_absolute;
			m_Matrix_inverseTranslateVect(&matrix,vertex->m_location);
			m_Matrix_inverseRotateVect(&matrix,vertex->m_location);
		}
	}
}

void msk_GetModelBound(Model *model, int ID)
{
	int			i;
	int			j;
	int			k;
	int			triangleIndex;
	Triangle	pTri;
	pFace		Face;
	pFace		TempList = NULL;
	int			maID;
	Matrix		final;
	Vector		newVertex;

	maID = 0;

	for(i=0 ; i<model->BoneAnim[maID][0]->m_numJoints ; i++)
	{
		model->BoneAnim[maID][0]->m_pJoints[i].m_currentRotationKeyframe = 0;
		model->BoneAnim[maID][0]->m_pJoints[i].m_currentTranslationKeyframe = 0;
		m_Matrix_set(&model->BoneAnim[maID][0]->m_pJoints[i].m_final,model->BoneAnim[maID][0]->m_pJoints[i].m_absolute.m_matrix);
	}
	Win32Timer_reset(model->BoneAnim[maID][0]->m_pTimer);

	for(i=0 ; i<model->m_numMeshes ; i++)
	{
		for(j=0 ; j<model->m_pMeshes[i].m_numTriangles; j++)
		{
			triangleIndex = model->m_pMeshes[i].m_pTriangleIndices[j];
			pTri = model->m_pTriangles[triangleIndex];

			Face = (Face_t*)s_malloc(sizeof(Face_t));
			memset(Face,0,sizeof(Face_t));
			Face->NumberOfVertices = 3;

			for(k=0 ; k<3 ; k++)
			{
				triangleIndex = pTri.m_vertexIndices[k];

				final = model->BoneAnim[maID][0]->m_pJoints[(int)model->m_pVertices[triangleIndex].m_boneID].m_final;				
						
				newVertex.m_vector[0] = model->m_pVertices[triangleIndex].m_location[0];
				newVertex.m_vector[1] = model->m_pVertices[triangleIndex].m_location[1];
				newVertex.m_vector[2] = model->m_pVertices[triangleIndex].m_location[2];
				newVertex.m_vector[3] = 0;
				m_Vector_transform( &newVertex, final);

				Face->Vertice[k].X = (float)( newVertex.m_vector[0] );
				Face->Vertice[k].Y = (float)( newVertex.m_vector[1] );
				Face->Vertice[k].Z = (float)( newVertex.m_vector[2] );
			}
			Face->Next = TempList;
			TempList = Face;
		}
	}

	gEntitiesLoadedList[ID].Boundaries = liste_Delete(gEntitiesLoadedList[ID].Boundaries);
	o_CenterAndGetBound(ID,TempList);
	gEntitiesLoadedList[ID].FaceList = NULL;
	TempList = liste_Delete(TempList);
}

int msk_cloneAnimModelData(Model *Model)
{
	int maID1, maID2;
	int	i, j, k;

	maID1 = 0;
	maID2 = 0;

	while(Model->BoneAnim[maID2][0])
	{
		maID2++;
		if(maID2 >= MAX_CLONEMODEL)
			return 0;
	}

	Model->m_numBoneAnim++;
	Model->m_indexBoneAnim[maID2] = Model->m_indexBoneAnim[maID1]; // 0

	for(k=0 ; k<MAX_ANIMMODEL ; k++)
	if(Model->BoneAnim[maID1][k])
	{
		Model->BoneAnim[maID2][k] = (Anim*)s_malloc(sizeof(Anim));
		memset(Model->BoneAnim[maID2][k],0,sizeof(Anim));
		Model->BoneAnim[maID2][k]->m_totalTime = Model->BoneAnim[maID1][k]->m_totalTime;
		Model->BoneAnim[maID2][k]->m_numJoints = Model->BoneAnim[maID1][k]->m_numJoints;
		Model->BoneAnim[maID2][k]->m_looping = true;

		Model->BoneAnim[maID2][k]->m_pTimer = (Timer*)s_malloc(sizeof(Timer));
		memcpy(Model->BoneAnim[maID2][k]->m_pTimer,Model->BoneAnim[maID1][k]->m_pTimer,sizeof(Timer));

		Model->BoneAnim[maID2][k]->m_pJoints = (Joint*)s_malloc(sizeof(Joint)*Model->BoneAnim[maID1][k]->m_numJoints);
		memset(Model->BoneAnim[maID2][k]->m_pJoints,0,sizeof(Joint)*Model->BoneAnim[maID1][k]->m_numJoints);

		for(i=0 ; i<Model->BoneAnim[maID1][k]->m_numJoints ; i++)
		{
			Model->BoneAnim[maID2][k]->m_pJoints[i].m_parent = Model->BoneAnim[maID1][k]->m_pJoints[i].m_parent;
			Model->BoneAnim[maID2][k]->m_pJoints[i].m_currentTranslationKeyframe = Model->BoneAnim[maID1][k]->m_pJoints[i].m_currentTranslationKeyframe;
			Model->BoneAnim[maID2][k]->m_pJoints[i].m_currentRotationKeyframe = Model->BoneAnim[maID1][k]->m_pJoints[i].m_currentRotationKeyframe;
			Model->BoneAnim[maID2][k]->m_pJoints[i].m_numRotationKeyframes = Model->BoneAnim[maID1][k]->m_pJoints[i].m_numRotationKeyframes;
			Model->BoneAnim[maID2][k]->m_pJoints[i].m_numTranslationKeyframes = Model->BoneAnim[maID1][k]->m_pJoints[i].m_numTranslationKeyframes;
			Model->BoneAnim[maID2][k]->m_pJoints[i].m_localRotation[0] = Model->BoneAnim[maID1][k]->m_pJoints[i].m_localRotation[0];
			Model->BoneAnim[maID2][k]->m_pJoints[i].m_localRotation[1] = Model->BoneAnim[maID1][k]->m_pJoints[i].m_localRotation[1];
			Model->BoneAnim[maID2][k]->m_pJoints[i].m_localRotation[2] = Model->BoneAnim[maID1][k]->m_pJoints[i].m_localRotation[2];
			Model->BoneAnim[maID2][k]->m_pJoints[i].m_localTranslation[0] = Model->BoneAnim[maID1][k]->m_pJoints[i].m_localTranslation[0];
			Model->BoneAnim[maID2][k]->m_pJoints[i].m_localTranslation[1] = Model->BoneAnim[maID1][k]->m_pJoints[i].m_localTranslation[1];
			Model->BoneAnim[maID2][k]->m_pJoints[i].m_localTranslation[2] = Model->BoneAnim[maID1][k]->m_pJoints[i].m_localTranslation[2];
		
			Model->BoneAnim[maID2][k]->m_pJoints[i].m_pTranslationKeyframes = Model->BoneAnim[maID1][k]->m_pJoints[i].m_pTranslationKeyframes;
			Model->BoneAnim[maID2][k]->m_pJoints[i].m_pRotationKeyframes = Model->BoneAnim[maID1][k]->m_pJoints[i].m_pRotationKeyframes;

			for(j=0 ; j<16 ; j++)
			{
				Model->BoneAnim[maID2][k]->m_pJoints[i].m_absolute.m_matrix[j] = Model->BoneAnim[maID1][k]->m_pJoints[i].m_absolute.m_matrix[j];
				Model->BoneAnim[maID2][k]->m_pJoints[i].m_relative.m_matrix[j] = Model->BoneAnim[maID1][k]->m_pJoints[i].m_relative.m_matrix[j];
				Model->BoneAnim[maID2][k]->m_pJoints[i].m_final.m_matrix[j] = Model->BoneAnim[maID1][k]->m_pJoints[i].m_final.m_matrix[j];
			}
		}
	}
	return maID2;
}

void msk_uncloneAnimModelData(Model *Model, int maID)
{
	int		i;
	int		k;

	k=0;
	for(k=0 ; k<MAX_ANIMMODEL ; k++)
	if(Model->BoneAnim[maID][k])
	{
		for(i=0 ; i<Model->BoneAnim[maID][k]->m_numJoints ; i++)
		{
			Model->BoneAnim[maID][k]->m_pJoints[i].m_pRotationKeyframes = NULL;
			Model->BoneAnim[maID][k]->m_pJoints[i].m_pTranslationKeyframes = NULL;
		}

		s_free(Model->BoneAnim[maID][k]->m_pJoints);
		Model->BoneAnim[maID][k]->m_pJoints = NULL;
		
		s_free(Model->BoneAnim[maID][k]->m_pTimer);
		Model->BoneAnim[maID][k]->m_pTimer = NULL;

		s_free(Model->BoneAnim[maID][k]);
		Model->BoneAnim[maID][k] = NULL;
	}
	Model->m_indexBoneAnim[maID] = 0;
	Model->m_numBoneAnim--;
}

void msk_unloadModelData(Model *MilkshapeModel)
{
	int		i;
	int		k;

	s_free(MilkshapeModel->m_pVertices);
	s_free(MilkshapeModel->m_pTriangles);

	for(k=0 ; k<MAX_ANIMMODEL ; k++)
	if(MilkshapeModel->BoneAnim[0][k])
	{
		for(i=0 ; i<MilkshapeModel->BoneAnim[0][k]->m_numJoints; i++)
		{
			s_free(MilkshapeModel->BoneAnim[0][k]->m_pJoints[i].m_pRotationKeyframes);
			MilkshapeModel->BoneAnim[0][k]->m_pJoints[i].m_pRotationKeyframes = NULL;

			s_free(MilkshapeModel->BoneAnim[0][k]->m_pJoints[i].m_pTranslationKeyframes);
			MilkshapeModel->BoneAnim[0][k]->m_pJoints[i].m_pTranslationKeyframes = NULL;
		}

		s_free(MilkshapeModel->BoneAnim[0][k]->m_pJoints);
		MilkshapeModel->BoneAnim[0][k]->m_pJoints = NULL;

		s_free(MilkshapeModel->BoneAnim[0][k]);
		MilkshapeModel->BoneAnim[0][k] = NULL;
	}

	for(i=0 ; i<MAX_CLONEMODEL ; i++)
	if(MilkshapeModel->BoneAnim[i][0])
		msk_uncloneAnimModelData(MilkshapeModel,i);

	for(i=0 ; i<MilkshapeModel->m_numMeshes ; i++)
	{
		s_free(MilkshapeModel->m_pMeshes[i].m_pTriangleIndices);
		MilkshapeModel->m_pMeshes[i].m_pTriangleIndices = NULL;
	}
	s_free(MilkshapeModel->m_pMeshes);
	MilkshapeModel->m_pMeshes = NULL;

	for(i=0 ; i<MilkshapeModel->m_numMaterials ; i++)
	{
		s_free(MilkshapeModel->m_pMaterials[i].m_pTextureFilename);
		MilkshapeModel->m_pMaterials[i].m_pTextureFilename = NULL;
	}
	s_free(MilkshapeModel->m_pMaterials);
	MilkshapeModel->m_pMaterials = NULL;
}


//========= ANIMATIONS ============//

void msk_ChangeAnim(pEntity MonsterEnt, Model *model, int newAnim)
{
	int		maID;

	maID = MonsterEnt->parent_id;

	if(model->m_indexBoneAnim[maID] != newAnim)
	{
		model->m_indexBoneAnim[maID] = newAnim;
		if(newAnim == MSK_DIE)
			model->BoneAnim[maID][newAnim]->m_looping = false;
		else
			model->BoneAnim[maID][newAnim]->m_looping = true;
		Model_restart(MonsterEnt,model);
	}
}

double Win32Timer_getTime(Timer *m_pTimer)
{
	double timeElapsed;

	timeElapsed = disp_GetClock() - m_pTimer->m_mmTimerStart;
	return timeElapsed;
}

void Win32Timer_reset(Timer *m_pTimer)
{
	m_pTimer->m_mmTimerStart = disp_GetClock();
}

void Model_restart(pEntity MonsterEnt, Model *model)
{
	int i;
	int maID;
	int	animINDEX;

	if(!MonsterEnt)
		return;

	maID = MonsterEnt->parent_id;
	animINDEX = model->m_indexBoneAnim[maID];

	for(i=0 ; i<model->BoneAnim[maID][animINDEX]->m_numJoints ; i++)
	{
		model->BoneAnim[maID][animINDEX]->m_pJoints[i].m_currentRotationKeyframe = 0;
		model->BoneAnim[maID][animINDEX]->m_pJoints[i].m_currentTranslationKeyframe = 0;
		m_Matrix_set(&model->BoneAnim[maID][animINDEX]->m_pJoints[i].m_final,model->BoneAnim[maID][animINDEX]->m_pJoints[i].m_absolute.m_matrix);
	}
	Win32Timer_reset(model->BoneAnim[maID][animINDEX]->m_pTimer);
}

void Model_advanceAnimation(pEntity MonsterEnt, Model *model)
{
	double	time;
	int	i;
	float	transVec[3];
	Matrix	transform; //={0};
	int	frame;
	Joint	*pJoint;
	float	timeDelta;
	float	interpValue;
        float	rotVec[3]={0,0,0};	//
	Kframe	curFrame;
	Kframe	prevFrame;
	Matrix	relativeFinal;
//	Quaternion qPrev;
//	Quaternion qCur;
//	Quaternion qFinal;
	int		maID;
	int		animINDEX;

	if(!MonsterEnt)
		return;

	maID = MonsterEnt->parent_id;
	animINDEX = model->m_indexBoneAnim[maID];

	time = Win32Timer_getTime(model->BoneAnim[maID][animINDEX]->m_pTimer);
	if(time > model->BoneAnim[maID][animINDEX]->m_totalTime * animspeed.value)
	{
		if(model->BoneAnim[maID][animINDEX]->m_looping)
		{
			Model_restart(MonsterEnt,model);
			time = 0;
		}
		else
			time = model->BoneAnim[maID][animINDEX]->m_totalTime * animspeed.value;
	}

	for(i=0 ; i<model->BoneAnim[maID][animINDEX]->m_numJoints; i++)
	{
		pJoint = &(model->BoneAnim[maID][animINDEX]->m_pJoints[i]);

		if(pJoint->m_numRotationKeyframes == 0 && pJoint->m_numTranslationKeyframes == 0)
		{
			m_Matrix_set(&pJoint->m_final,pJoint->m_absolute.m_matrix);
			continue;
		}

		frame = pJoint->m_currentTranslationKeyframe;
		while(	frame<pJoint->m_numTranslationKeyframes &&
				pJoint->m_pTranslationKeyframes[frame].m_time * animspeed.value < time )
		{
			frame++;
		}
		pJoint->m_currentTranslationKeyframe = frame;

		if(frame==0 )
			memcpy( transVec, pJoint->m_pTranslationKeyframes[0].m_parameter, sizeof ( float )*3 );
		else if ( frame == pJoint->m_numTranslationKeyframes )
			memcpy( transVec, pJoint->m_pTranslationKeyframes[frame-1].m_parameter, sizeof ( float )*3 );
		else
		{
			curFrame = pJoint->m_pTranslationKeyframes[frame];
			prevFrame = pJoint->m_pTranslationKeyframes[frame-1];

			timeDelta = (curFrame.m_time * animspeed.value)-(prevFrame.m_time * animspeed.value);
			interpValue = ( float )(( time-(prevFrame.m_time * animspeed.value) )/timeDelta );

			transVec[0] = prevFrame.m_parameter[0]+( curFrame.m_parameter[0]-prevFrame.m_parameter[0] )*interpValue;
			transVec[1] = prevFrame.m_parameter[1]+( curFrame.m_parameter[1]-prevFrame.m_parameter[1] )*interpValue;
			transVec[2] = prevFrame.m_parameter[2]+( curFrame.m_parameter[2]-prevFrame.m_parameter[2] )*interpValue; 
		}

		frame = pJoint->m_currentRotationKeyframe;
		while(	frame < pJoint->m_numRotationKeyframes &&
				pJoint->m_pRotationKeyframes[frame].m_time * animspeed.value < time )
		{
			frame++;
		}
		pJoint->m_currentRotationKeyframe = frame;

		if(frame==0)
			m_Matrix_setRotationRadians(&transform,pJoint->m_pRotationKeyframes[0].m_parameter);
		else if (frame == pJoint->m_numRotationKeyframes)
			m_Matrix_setRotationRadians(&transform,pJoint->m_pRotationKeyframes[frame-1].m_parameter);
		else
		{
			curFrame = pJoint->m_pRotationKeyframes[frame];
			prevFrame = pJoint->m_pRotationKeyframes[frame-1];

			timeDelta = (curFrame.m_time * animspeed.value) - (prevFrame.m_time * animspeed.value);
			interpValue = ( float )(( time-(prevFrame.m_time * animspeed.value) )/timeDelta );

#if 0
			m_Quaternion_fromAngles(&qPrev,prevFrame.m_parameter);
			m_Quaternion_fromAngles(&qCur,curFrame.m_parameter);
			m_Quaternion_slerp(&qFinal,qPrev,qCur,interpValue);
			m_Matrix_setRotationQuaternion(&transform,qFinal);
#else
			rotVec[0] = prevFrame.m_parameter[0]+( curFrame.m_parameter[0]-prevFrame.m_parameter[0] )*interpValue;
			rotVec[1] = prevFrame.m_parameter[1]+( curFrame.m_parameter[1]-prevFrame.m_parameter[1] )*interpValue;
			rotVec[2] = prevFrame.m_parameter[2]+( curFrame.m_parameter[2]-prevFrame.m_parameter[2] )*interpValue;
			m_Matrix_setRotationRadians(&transform,rotVec);
#endif
		}

		m_Matrix_setTranslation(&transform,transVec);
		relativeFinal = pJoint->m_relative;
		m_Matrix_postMultiply(&relativeFinal,transform);

		if ( pJoint->m_parent == -1 )
			m_Matrix_set(&pJoint->m_final,relativeFinal.m_matrix);
		else
		{
			m_Matrix_set(&pJoint->m_final,model->BoneAnim[maID][animINDEX]->m_pJoints[pJoint->m_parent].m_final.m_matrix);
			m_Matrix_postMultiply(&pJoint->m_final,relativeFinal);
		}
	}
}

void msk_draw(Model *model, pEntity Ent, float timing, float R, float G, float B)
{
	int			i, j, k;
	int			materialIndex;
	int			triangleIndex;
	Triangle	pTri;
	Matrix		final;
	Vector		newVertex;
	int			index;
	int			textID;
//	vect_t		Test;
//	vect_t		TempVect[3];
	int			animINDEX;
	float		scaling;
	int			maID;
	Render_t	rend;

	if(gIsServer && net_dedicated.value)
		return;

	maID = Ent->parent_id;
	animINDEX = model->m_indexBoneAnim[maID];


	rend.is_texture = true;
	rend.nvert = 3;
	rend.begin_mode = RENDER_TRIANGLE;
	rend.is_depthtest = true;
	rend.is_depthmask = true;
	rend.is_draw2D = false;

	if(timing < 1000.0f)
	{
		scaling = timing / 1000.0f;
		rend.is_blend = true;
		rend.blend_src_mode = GL_SRC_ALPHA;
		rend.blend_dst_mode = GL_ONE_MINUS_SRC_ALPHA;
		rend.is_color = true;
		rend.color.R = R;
		rend.color.G = G;
		rend.color.B = B;
		rend.color.A = scaling;
	}
	else
	{
	//	rend.is_color = false;
		rend.is_color = true;
		rend.color.R = R;
		rend.color.G = G;
		rend.color.B = B;
		rend.is_blend = false;
		scaling = 1.0f;
	}

	if(IsColorScreen)
	{
		rend.is_color = true;
		rend.color.R = R * WorldColorR;
		rend.color.G = G * WorldColorG;
		rend.color.B = B * WorldColorB;
	}

	for(i=0 ; i<model->m_numMeshes ; i++)
	{
		materialIndex = model->m_pMeshes[i].m_materialIndex;
		if(materialIndex < 0)
			continue;		
		textID = model->m_pMaterials[materialIndex].m_texture;
		if(textID < 0)
			continue;
		rend.texture_id = textID;
		
		for(j=0 ; j<model->m_pMeshes[i].m_numTriangles; j++)
		{
			triangleIndex = model->m_pMeshes[i].m_pTriangleIndices[j];
			pTri = model->m_pTriangles[triangleIndex];

			for(k=0 ; k<3 ; k++)
			{
				index = pTri.m_vertexIndices[k];
				if( model->m_pVertices[index].m_boneID == -1 )
				{
					rend.xyz[k][0] = model->m_pVertices[index].m_location[0];
					rend.xyz[k][1] = model->m_pVertices[index].m_location[1];
					rend.xyz[k][2] = model->m_pVertices[index].m_location[2];
					rend.uv[k][0] = pTri.m_s[k];
					rend.uv[k][1] = pTri.m_t[k];
				}
				else
				{
					final = model->BoneAnim[maID][animINDEX]->m_pJoints[(int)model->m_pVertices[index].m_boneID].m_final;

					newVertex.m_vector[0] = model->m_pVertices[index].m_location[0];
					newVertex.m_vector[1] = model->m_pVertices[index].m_location[1];
					newVertex.m_vector[2] = model->m_pVertices[index].m_location[2];
					newVertex.m_vector[3] = 0;
					m_Vector_transform( &newVertex, final);

					rend.xyz[k][0] = (float)( newVertex.m_vector[0]);
					rend.xyz[k][1] = (float)( newVertex.m_vector[1]);
					rend.xyz[k][2] = (float)( newVertex.m_vector[2]);
					rend.uv[k][0] = pTri.m_s[k];
					rend.uv[k][1] = pTri.m_t[k];
				}
			}
			gl_RenderPoly(rend);
		}
	}
}




