
#ifndef MILKSHAPEMODEL_H
#define MILKSHAPEMODEL_H

#include "alloween.h"

enum
{
	MSK_THINK,
	MSK_RUN,
	MSK_ATTACK,
	MSK_DIE
	
};


typedef struct TimerTag
{
	int		m_frequency;
	float	m_resolution;
	double	m_mmTimerStart;
	int		m_pcTimerStart;

} Timer;

typedef struct MeshTAG
{
	int m_materialIndex;
	int m_numTriangles;
	int *m_pTriangleIndices;

} Mesh;

typedef	struct MaterialTAG
{
	float	m_ambient[4], m_diffuse[4], m_specular[4], m_emissive[4];
	float	m_shininess;
	int		m_texture;
	char	*m_pTextureFilename;

} Material;

typedef	struct TriangleTAG
{
	float	m_vertexNormals[3][3];
	float	m_s[3], m_t[3];
	int		m_vertexIndices[3];

} Triangle;

typedef	struct VertexTAG
{
	char	m_boneID;
	float	m_location[3];

} Vertex;

typedef	struct KeyframeTAG
{
	int		m_jointIndex;
	float	m_time;
	float	m_parameter[3];

} Kframe;

typedef	struct JointTAG
{
	float	m_localRotation[3];
	float	m_localTranslation[3];
	Matrix	m_absolute, m_relative;
	int		m_numRotationKeyframes, m_numTranslationKeyframes;
	Kframe *m_pTranslationKeyframes;
	Kframe *m_pRotationKeyframes;
	int		m_currentTranslationKeyframe, m_currentRotationKeyframe;
	Matrix	m_final;
	int		m_parent;

} Joint;

typedef struct AnimTAG
{
	int		m_numJoints;
	Joint	*m_pJoints;
	Timer	*m_pTimer;
	float	m_totalTime;
	hBool	m_looping;

} Anim;

typedef struct ModelTAG
{
	int		m_numMeshes;
	Mesh	*m_pMeshes;
	int		m_numMaterials;
	Material *m_pMaterials;
	int		m_numTriangles;
	Triangle *m_pTriangles;
	int		m_numVertices;
	Vertex	*m_pVertices;

	int		m_numBoneAnim;
	int		m_indexBoneAnim[MAX_CLONEMODEL];
	Anim	*BoneAnim[MAX_CLONEMODEL][MAX_ANIMMODEL];

} Model;

// 
//	MS3D STRUCTURES 
//

// byte-align structures
#ifdef _MSC_VER
#	pragma pack( push, packing )
#	pragma pack( 1 )
#	define PACK_STRUCT
#elif defined( __GNUC__ )
#	define PACK_STRUCT	__attribute__((packed))
#else
//#	error you must byte-align these structures with the appropriate compiler directives

#	pragma pack( push, packing )
#	pragma pack( 1 )
#	define PACK_STRUCT

#endif


// File header
typedef struct MS3DHeaderTAG
{
	char m_ID[10];
	int m_version;

} PACK_STRUCT MS3DHeader;

// Vertex information
typedef struct MS3DVertexTAG
{
	byte m_flags;
	float m_vertex[3];
	char m_boneID;
	byte m_refCount;

} PACK_STRUCT MS3DVertex;

// Triangle information
typedef struct MS3DTriangleTAG
{
	word m_flags;
	word m_vertexIndices[3];
	float m_vertexNormals[3][3];
	float m_s[3], m_t[3];
	byte m_smoothingGroup;
	byte m_groupIndex;

} PACK_STRUCT MS3DTriangle;

// Material information
typedef struct MS3DMaterialTAG
{
    char	m_name[32];
    float	m_ambient[4];
    float	m_diffuse[4];
    float	m_specular[4];
    float	m_emissive[4];
    float	m_shininess;	// 0.0f - 128.0f
    float	m_transparency;	// 0.0f - 1.0f
    byte	m_mode;	// 0, 1, 2 is unused now
    char	m_texture[128];
    char	m_alphamap[128];

} PACK_STRUCT MS3DMaterial;

//	Joint information
typedef struct MS3DJointTAG
{
	byte m_flags;
	char m_name[32];
	char m_parentName[32];
	float m_rotation[3];
	float m_translation[3];
	word m_numRotationKeyframes;
	word m_numTranslationKeyframes;

} PACK_STRUCT MS3DJoint;

// Keyframe data
typedef struct MS3DKeyframeTAG
{
	float m_time;
	float m_parameter[3];

} PACK_STRUCT MS3DKeyframe;

typedef struct JointNameLstRecTAG
{
	int m_jointIndex;
	const char *m_pName;

} PACK_STRUCT JointNameLstRec;

// Default alignment
#ifdef _MSC_VER
#	pragma pack( pop, packing )
#endif

#undef PACK_STRUCT


hBool	msk_LoadData(Model *msk_model, char *name);
void	msk_InitModel(Model *msk_model);
hBool	msk_LoadModel(Model *MilkshapeModel, char *filename, hBool LoadMesh);
void	msk_GetModelBound(Model *model, int ID);
void	msk_setJointKeyframe(Model *model, int jointIndex, int keyframeIndex, int animIndex, float time, float *parameter, hBool isRotation);
void	msk_setupJoints(Model *model, int animIndex);
int		msk_cloneAnimModelData(Model *Model);
void	msk_uncloneAnimModelData(Model *Model, int maID);
void	msk_unloadModelData(Model *MilkshapeModel);
void	msk_draw(Model *model, pEntity Ent, float timing, float R, float G, float B);
void	msk_ChangeAnim(pEntity MonsterEnt, Model *model, int newAnim);
double	Win32Timer_getTime(Timer *m_pTimer);
void	Win32Timer_reset(Timer *m_pTimer);
void	Model_restart(pEntity MonsterEnt, Model *model);
void	Model_advanceAnimation(pEntity MonsterEnt, Model *model);


#endif
