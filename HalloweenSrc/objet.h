
#ifndef OBJET_INCLUDED
#define OBJET_INCLUDED

#include "alloween.h"
#include "bspfile.h"

enum
{
	GUN1,
	GUN2,
	PERSO,
	FLECHE,
	DEPLA,
	AMO1,
	AMO2,
	SPIKE,
	SABRE,
	BOMBE,
	EXPLOSION,
	JADE,
	LIFT,
	PANG,
	GUN3,
	AMO3,
	FIOLE1,
	SMOKE,
	LAMPE,
	MORBAK,
	SKYDOME,
	PUMPKIN,
	MLAMPE,
	H,
	A,
	L,
	O,
	W,
	E,
	N,
	ZEBAT,
	FIOLE2,
	FIOLE3,
	SUBZERAT,
	ZEMUMMY,
	TOILE,
	DEPLASPIKE,
	GUN1B,
	GUN3B,
	DEPLAHEAD,
	BLOOD,
	MORBAKBODY,
	SPINE,
	STOP2,
	SKYDOME2
};

enum
{
	NOTHOLDED,
	HOLDED
};

typedef struct tag_AnimEntity
{
	vect_t	Pos;
	vect_t	Pos_Vel;
	vect_t	Pos_Acc;

	vect_t	Rot;
	vect_t	Rot_Vel;

	vect_t	Siz;
	vect_t	Siz_Vel;

	float	id_list;
	float	oscille_start_Y;
	float	oscille_Y;

	// needed for delta compression issues
	vect_t	net_OldPos;
	vect_t	net_OldVel;
	vect_t	net_OldAcc;

	vect_t	net_OldRot;

} ANIMENTITY;


typedef struct tag_entity *pEntity;
typedef struct tag_entity
{
	hBool		IsBounce;
	hBool		IsGravity;
	hBool		SolidSpace;
	hBool		JumpPossible;
	hBool		Blending;
	hBool		Timing;
	hBool		player_holded;
	hBool		IsChanged_Animation;
	hBool		net_SendNewAnim;
	int			Ground_Type;
	int			Monster_MoveState;
	int			Animation_ID;
	int			parent_leafid;
	int			entity_id;
	int			parent_id;
	int			IA_wallAngle;
	int			health;
	float		Monster_Life;
	float		saveradiusY;
	float		ReloadTime;
	float		BlendVal;
	float		Time_msec;
	float		fxTime_msec;
	float		R;
	float		G;
	float		B;
	ANIMENTITY	ent_anim;
	vect_t		Radius;
	Face_t		*FaceList;
	Face_t		*Boundaries;
	BOUNDINGBOX	ent_bbox;
	dword		snd_channel;
	struct tag_entity	*Next;

} ENTITY;

typedef struct tag_respawn
{
	float	list_id;
	hBool	used;
	hBool	pickedup;
	int		entity_id;
	int		RotY;
	vect_t	entity_pos;
	float	Timer;

} RESPAWN;

void		o_RespawnProcess();
ANIMENTITY *o_FindEntityAnimField(int id);
pEntity		o_FindEntity(int id);
pEntity		o_FindEntity2(int id, int number);
pEntity		o_FindHoldedEntity(int id);
pEntity		o_FindHoldedEntity2(int id, int number);
pEntity		o_FindNotHoldedEntity(int id);
int			o_FindEntityClonePos(pEntity Ent);
int			o_FindEntityClone(int entid);
pEntity		o_FindEntityFromID(float id);
void		o_RemoveEntityFromList_Pos(vect_t pos);
void		o_RemoveEntityFromList_ID(float id);
void		o_RemoveEntityFromList(pEntity remove);
void		o_ClearAnimFields(ANIMENTITY *anim);
void		o_UpdateEntities();
hBool		o_IsVisible(pEntity Ent);
void		o_RendSkyObj();
void		o_RenderEntitiesList();
hBool		o_AddEntityToList(int id, int parent_id, ANIMENTITY anim, hBool PlayerHolded);
void		o_ClearEntityList();
int			o_FindRespawnTabIndex(pEntity Ent);
hBool		o_LoadEntityToStruct(char *path, int id);
void		o_FreeEntity(pEntity Ent);
hBool		o_LoadEntStruct();
void		o_UnloadEntStruct();
void		o_CenterAndGetBound(int ID, pFace List);
void		o_UpdateBound(BOUNDINGBOX bbox, int ID, pFace List);
void		o_Center(pFace List);
float		o_GetMaxRadius(vect_t radius);
float		o_GetViewCamMaxRadius();
void		o_GetLightningIntensity(int obj_id, vect_t ObjPos, float *R, float *G, float *B);
void		o_OscilleYprocess(pEntity Ent);
void		o_RendCautchFlask();
void		o_RemoveHudFlask();
void		o_AddHudFlask(int flask_id);
void		o_RendCautchletters();
void		o_RemoveHudLetters();
void		o_AddHudLetter(int letter_id);

#endif //OBJET_INCLUDED


