
#ifndef SCRIPT_INCLUDED
#define SCRIPT_INCLUDED

#include "alloween.h"

typedef struct mvtdef_tag
{
	hBool	IsUsed;
	int		IsWaitForAction;
	int		SaveWaitForAction;
	int		MvtState;
	vect_t	MvtOffset;
	float	MvtTime;
	float	MvtStartTime;
	float	MvtOffsetTime;

} MVTDEF;

typedef struct txtdef_tag
{
	int		IsWaitForAction;
	int		SaveWaitForAction;
	float	TextTime;
	char	TextString[MAX_LEXEME_SIZE];

} TXTDEF;

typedef struct snddef_tag
{
	hBool	IsPlayed;
	int		IsWaitForAction;
	int		SaveWaitForAction;
	int		idsound;

} SNDDEF;

typedef struct script_tag
{
	hBool	Used;
	vect_t	FirstPos;
	//vect_t	FirstScale;
	vect_t	StartPos;
	vect_t	StartScale;
	vect_t	Pos;
	vect_t	Scale;
	int		NbMove;
	int		NbScale;
	int		NbText;
	int		NbSound;
	MVTDEF	move[MAX_MVTPERAB];
	MVTDEF	scale[MAX_MVTPERAB];
	TXTDEF	text[MAX_TEXT];
	SNDDEF	sound[MAX_SOUND];

} SCRIPT;

typedef struct actionbrush_tag
{
	BOUNDINGBOX bbox;
	vect_t	Radius;
	int		LaunchWaitForAction;
	int		MayLaunch_ab;
	float	ActivationDist;

	int		SaveResetWaitForAction;
	float	SaveResetTime;
	float	ResetTime;
	hBool	IsResetWaitForAction;

	float	R;
	float	G;
	float	B;

	int		ActionNumber;
	pFace	FaceList;
	SCRIPT	script;

} ABRUSH;


hBool	ab_Load(FILE *file);
void	ab_Unload();
hBool	ab_IsVisibleAB(vect_t campos, int ab_id);
hBool	ab_IsVisible(vect_t campos, vect_t pos);
void	ab_Draw(vect_t pos);
void	ab_Activation(int i, int unlock_id);
void	ab_OnEventActivate(vect_t pos, int event);
void	ab_PersoActivate(vect_t pos);
void	ab_FlecheActivate(vect_t pos);
void	ab_Activate(vect_t pos);
void	ab_Push(int index, float dir, char axis);
void	ab_Push_Ent(pEntity Ent, int index, float dir, char axis);

void	script_Init(char *path);
void	script_Init_parameters();
void	script_Load_Sound(int idsound);
void	script_Load_Text(char *txt);
void	script_Load();
void	script_Run();



#endif// SCRIPT_INCLUDED
