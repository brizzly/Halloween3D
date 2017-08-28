
#include "alloween.cpp"


//==================== GLOBALS ========================
//=====================================================

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

hBool	ProcessScriptFile(char *path);
hBool	CheckLexeme();
hBool	CheckSymbole(char c);
int		CheckSyntax();
int		Instruction();
int		Block();

FILE	*file = NULL;
char	instruction[MAX_LEXEME_SIZE];
int		inst_type;
int		gDigit;
char	gText[MAX_LEXEME_SIZE];
char	gSymbole;
char	*Lexemes[MAX_LEXEMES] = {	"move", "scale", "rotate", "reset", "actionbrush",
									"ifAction", "Launchactionbrush", "ACTIONKEY",
									"FLECHE", "PERSO", "WAITACTION", "ActivationDist",
									"DispText", "PlaySound" };
char	*Symboles[MAX_SYMBOLES] = { "(", ")", "[", "]", ",", "{", "}", ";" };


int		gIDactionBrush;
int		gActivationDist;
int		gNumLigne=1;
int		gNbErrors=0;
int		gWaitForAction;
int		gMayLaunch_ab;
hBool	gKey_Action=false;
int		gIDsound;
float	ScriptTimeText;

enum
{
	I_LEXEME=1,
	I_SYMBOLE,
	I_DIGIT,
	I_TEXT,
	I_EOF,
	I_UNKNOW
};

enum
{
	AB_NOSTARTED,
	AB_RUNNING,
	AB_FINISHED
};

struct
{
	int	type;
	int	offset[3];
	int	time[2];

} Commands;

int		gNb_ActionBrush;
ABRUSH	ActionBrushTab[MAX_SCRIPTSLOT];


//=====================================================
//=====================================================


hBool ab_Load(FILE *file)
{
	pFace	Face;
	pFace	ABrushList = NULL;
	int		i, j, k;
	int		index;
	int		Nb_ActionBrush;
	int		ActionNumber;
	int		nbface;
	vect_t	center;
	int		divider;
	
	if(file==NULL)
		return false;

	gNb_ActionBrush = 0;

	for(i=0 ; i<MAX_SCRIPTSLOT ; i++)
	{
		ActionBrushTab[i].ResetTime = -1;
		ActionBrushTab[i].SaveResetWaitForAction = 0;
	}

	fscanf(file,"<ACTION BRUSH> %d\n", &Nb_ActionBrush);
	m_ConsPrint("Nb_ActionBrush: %d\n",Nb_ActionBrush); //
	for(i=0 ; i<Nb_ActionBrush ; i++)
	{
		ABrushList = NULL;
		fscanf(file,"ActionBrush %d faces %d\n",&ActionNumber,&nbface);
		for(j=0 ; j<nbface ; j++)
		{
			Face = (Face_t*)s_malloc(sizeof(Face_t));
			memset(Face,0,sizeof(Face_t));

			Face->Type = ACTIONBRUSH;
			Face->actionbrush_id = i;

			fscanf(file,"%d %d\n", &Face->NumberOfVertices, &Face->TextureID);
			Face->TextureID = texture_id_plan[Face->TextureID];
			for(k=0 ; k<Face->NumberOfVertices ; k++)
			{
				fscanf(file,"%f %f %f %f %f\n",
								&Face->Vertice[k].X,
								&Face->Vertice[k].Y,
								&Face->Vertice[k].Z,
								&Face->Vertice[k].U,
								&Face->Vertice[k].V);
			}
			Face->Next = ABrushList;
			ABrushList = Face;
		}
		ActionBrushTab[i].ActionNumber = ActionNumber;
		ActionBrushTab[i].FaceList = ABrushList;
		ActionBrushTab[i].MayLaunch_ab = -1;
		ActionBrushTab[i].script.Used = false;
		ActionBrushTab[i].script.NbMove = 0;
		ActionBrushTab[i].script.NbScale = 0;
		ActionBrushTab[i].script.NbText = 0;
		ActionBrushTab[i].script.NbSound = 0;

		gNb_ActionBrush++;

		for(index=0 ; index<MAX_MVTPERAB ; index++)
		{
			// movement script initialisation
			ActionBrushTab[i].script.move[index].IsUsed = false;
			ActionBrushTab[i].script.move[index].MvtState = AB_NOSTARTED;
			ActionBrushTab[i].script.move[index].MvtOffset.X = 0;
			ActionBrushTab[i].script.move[index].MvtOffset.Y = 0;
			ActionBrushTab[i].script.move[index].MvtOffset.Z = 0;
			ActionBrushTab[i].script.move[index].MvtTime = 0;
			ActionBrushTab[i].script.move[index].MvtStartTime = 0;
			ActionBrushTab[i].script.move[index].MvtOffsetTime = 0;

			// scale script initialisation
			ActionBrushTab[i].script.scale[index].IsUsed = false;
			ActionBrushTab[i].script.scale[index].MvtState = AB_NOSTARTED;
			ActionBrushTab[i].script.scale[index].MvtOffset.X = 1;
			ActionBrushTab[i].script.scale[index].MvtOffset.Y = 1;
			ActionBrushTab[i].script.scale[index].MvtOffset.Z = 1;
			ActionBrushTab[i].script.scale[index].MvtTime = 0;
			ActionBrushTab[i].script.scale[index].MvtStartTime = 0;
			ActionBrushTab[i].script.scale[index].MvtOffsetTime = 0;
		}
	}

	for(i=0 ; i<Nb_ActionBrush ; i++)
	{
		center.X = 0;
		center.Y = 0;
		center.Z = 0;
		divider = 0;
		for(Face=ActionBrushTab[i].FaceList ; Face ; Face=Face->Next)
		{
			for(j=0 ; j<Face->NumberOfVertices ; j++)
			{
				center.X += Face->Vertice[j].X;
				center.Y += Face->Vertice[j].Y;
				center.Z += Face->Vertice[j].Z;
			}
			divider += Face->NumberOfVertices;
		}

		center.X /= divider;
		center.Y /= divider;
		center.Z /= divider;

#if 0
		if(developer.value)
			m_ConsPrint("ab(%f %f %f)\n",center.X,center.Y,center.Z);
#endif

		ActionBrushTab[i].script.Pos = center;
		ActionBrushTab[i].script.StartPos = center;
		ActionBrushTab[i].script.FirstPos = center;
		ActionBrushTab[i].script.Scale.X = 1;
		ActionBrushTab[i].script.Scale.Y = 1;
		ActionBrushTab[i].script.Scale.Z = 1;
		ActionBrushTab[i].script.StartScale.X = 1;
		ActionBrushTab[i].script.StartScale.Y = 1;
		ActionBrushTab[i].script.StartScale.Z = 1;

		for(Face=ActionBrushTab[i].FaceList ; Face ; Face=Face->Next)
		{
			for(j=0 ; j<Face->NumberOfVertices ; j++)
			{
				Face->Vertice[j].X -= center.X;
				Face->Vertice[j].Y -= center.Y;
				Face->Vertice[j].Z -= center.Z;
			}
			
			Face->Plan.VecteurNormal = GetFaceNormal(Face->Vertice[0],Face->Vertice[1],Face->Vertice[2]);		
			Face->Plan.PointOnPlane.X = Face->Vertice[0].X;
			Face->Plan.PointOnPlane.Y = Face->Vertice[0].Y;
			Face->Plan.PointOnPlane.Z = Face->Vertice[0].Z;
		}
		CalculateBox(&ActionBrushTab[i].bbox,ActionBrushTab[i].FaceList);
		ActionBrushTab[i].Radius = ActionBrushTab[i].bbox.BoxMax;
	}

	return true;
}

void ab_Unload()
{
	int		i;
	int		k;

	for(i=0 ; i<gNb_ActionBrush ; i++)
	{
		for(k=0 ; k<ActionBrushTab[i].script.NbText ; k++)
			ActionBrushTab[i].script.text[k].TextTime = 0;

		ActionBrushTab[i].FaceList = liste_Delete(ActionBrushTab[i].FaceList);
	}
}

hBool ab_IsVisibleAB(vect_t campos, int ab_id)
{
	if(ab_id<0)
		return false;

	if( !ab_IsVisible(campos,ActionBrushTab[ab_id].script.Pos) && !ab_IsVisible(campos,ActionBrushTab[ab_id].script.FirstPos))
		return false;
	return true;
}

hBool ab_IsVisible(vect_t campos, vect_t pos)
{
	hBool	Is_visible;
	int		leaf_ent;
	int		leaf_cam;

	Is_visible = true;
	if(MapDataFormat == MAP_BSP)
	{
		if(visent.value)
			return true;
		leaf_ent = bsp_FindCamera(pos);
		leaf_cam = bsp_FindCamera(campos);
		if(leaf_ent == -1 || leaf_cam == -1)
			Is_visible = true; //false;
		else
		{
			if(/*gLeafLocation*/leaf_cam != -1)
			{
				if(PVSData[LeafArray[leaf_cam/*gLeafLocation*/].PVSIndex+leaf_ent] == '0')
					Is_visible = false;
			}
		//	if(!frustrum_LeafIn(gFrustrumPlanes,leaf_ent))
		//		Is_visible = false;
		}
	}
	return Is_visible;
}

void ab_Draw(vect_t pos)
{
	int		i;
	float	R, G, B;
	vect_t	Mov = {0,0,0};
	vect_t	Rot = {0,0,0};
	vect_t	Siz = {1,1,1};

	if(noscript.value || gIsMultiplayer)
		return;

	for(i=0 ; i<gNb_ActionBrush ; i++)
	{
		// HACK
		if(!ab_IsVisibleAB(gCamera.Pos,i))
		//if( !ab_IsVisible(ActionBrushTab[i].script.Pos) && !ab_IsVisible(ActionBrushTab[i].script.FirstPos))
			continue;
		Mov = ActionBrushTab[i].script.Pos;
		Siz = ActionBrushTab[i].script.Scale;
		o_GetLightningIntensity(-1, Mov,&R,&G,&B);
		ActionBrushTab[i].R = R;
		ActionBrushTab[i].G = G;
		ActionBrushTab[i].B = B;
		gl_DrawActionBrush(ActionBrushTab[i].FaceList,pos,Mov,Rot,Siz);
	}
}

// we activate here the action brush
void ab_Activation(int i, int unlock_id)
{
	int		k;

	if(ActionBrushTab[i].IsResetWaitForAction == unlock_id)
	{
		ActionBrushTab[i].SaveResetWaitForAction = unlock_id;
		ActionBrushTab[i].IsResetWaitForAction = 0;
	}

	for(k=0 ; k<ActionBrushTab[i].script.NbMove ; k++)
	if(ActionBrushTab[i].script.move[k].IsWaitForAction == unlock_id)
	{
		ActionBrushTab[i].script.move[k].SaveWaitForAction = unlock_id;
		ActionBrushTab[i].script.move[k].IsWaitForAction = 0;
	}

	for(k=0 ; k<ActionBrushTab[i].script.NbScale ; k++)
	if(ActionBrushTab[i].script.scale[k].IsWaitForAction == unlock_id)
	{
		ActionBrushTab[i].script.scale[k].SaveWaitForAction = unlock_id;
		ActionBrushTab[i].script.scale[k].IsWaitForAction = 0;
	}

	for(k=0 ; k<ActionBrushTab[i].script.NbText ; k++)
	if(ActionBrushTab[i].script.text[k].IsWaitForAction == unlock_id)
	{
		ActionBrushTab[i].script.text[k].SaveWaitForAction = unlock_id;
		ActionBrushTab[i].script.text[k].IsWaitForAction = 0;
	}

	for(k=0 ; k<ActionBrushTab[i].script.NbSound ; k++)
	if(ActionBrushTab[i].script.sound[k].IsWaitForAction == unlock_id)
	{
		ActionBrushTab[i].script.sound[k].SaveWaitForAction = unlock_id;
		ActionBrushTab[i].script.sound[k].IsWaitForAction = 0;
	}

	// si on est en presence d'un interrupteur (launcher)... !
	if(ActionBrushTab[i].MayLaunch_ab > -1 && ActionBrushTab[i].MayLaunch_ab < MAX_SCRIPTSLOT)
	if(ActionBrushTab[i].LaunchWaitForAction == unlock_id)
	{
		ActionBrushTab[i].LaunchWaitForAction = 0;
	}
}

void ab_OnEventActivate(vect_t pos, int event)
{
	int		i;
	int		j;
	float	d;
	vect_t	ab_pos;

	for(i=0 ; i<gNb_ActionBrush ; i++)
	{
		ab_pos = ActionBrushTab[i].script.Pos;
		d = m_Distance(pos,ab_pos);
		if( d <= ActionBrushTab[i].ActivationDist )
		{
			if(ActionBrushTab[i].IsResetWaitForAction == event)
				ab_Activation(i,event);

			for(j=0 ; j<ActionBrushTab[i].script.NbMove ; j++)
			if(ActionBrushTab[i].script.move[j].IsWaitForAction == event)
				ab_Activation(i,event);

			for(j=0 ; j<ActionBrushTab[i].script.NbScale ; j++)
			if(ActionBrushTab[i].script.scale[j].IsWaitForAction == event)
				ab_Activation(i,event);

			for(j=0 ; j<ActionBrushTab[i].script.NbText ; j++)
			if(ActionBrushTab[i].script.text[j].IsWaitForAction == event)
				ab_Activation(i,event);

			for(j=0 ; j<ActionBrushTab[i].script.NbSound ; j++)
			if(ActionBrushTab[i].script.sound[j].IsWaitForAction == event)
				ab_Activation(i,event);
		}
	}
}

void ab_PersoActivate(vect_t pos)
{
	pEntity	PlayerEnt;

	if(noscript.value || gIsMultiplayer)
		return;

	PlayerEnt = o_FindEntity(PERSO);
	if(!PlayerEnt)
		return;
	ab_OnEventActivate(PlayerEnt->ent_anim.Pos,3);	// IsMoveWaitForAction == 3 mean we are waiting for a PERSO near ACTION
}

void ab_FlecheActivate(vect_t pos)
{
	if(noscript.value || gIsMultiplayer)
		return;
	ab_OnEventActivate(pos,2);	// IsMoveWaitForAction == 2 mean we are waiting for a FLECHE ACTION
}

void ab_Activate(vect_t pos)
{
	if(noscript.value || gIsMultiplayer)
		return;
	ab_OnEventActivate(pos,1);	// IsMoveWaitForAction == 1 mean we are waiting for a KEYACTION
}

//	On ne doit pas traverser les actionbrush en mouvement
void ab_Push(int index, float dir, char axis)
{
	pEntity	Entity;

	if(MapDataFormat != MAP_BSP)
		return;

	for(Entity=gEntitiesList ; Entity ; Entity=Entity->Next)
	if(	Entity->entity_id == PERSO		||
		Entity->entity_id == DEPLA		||
		Entity->entity_id == MORBAK		||
		Entity->entity_id == ZEBAT		||
		Entity->entity_id == SUBZERAT	||
		Entity->entity_id == ZEMUMMY)
	{
		ab_Push_Ent(Entity,index,dir,axis);
	}
}

void ab_Push_Ent(pEntity Ent, int index, float dir, char axis)
{
	CTRACE	ClipTrace;
	vect_t	Dest;
	float	Player_AB_Distance;
	float	eDistance;
	vect_t	eDistA;
	vect_t	eDistB;
	vect_t	Dir1;
	vect_t	Dir;

	ClipTrace.Response = true;
	ClipTrace.NoRadius = false;
	ClipTrace.DontUnstuck = false;
	ClipTrace.Ent = Ent;
	ClipTrace.ellipsoid = Ent->Radius;

	Dir1.X = Dir1.Y = Dir1.Z = 0;

	if(Ent->Ground_Type == ACTIONBRUSH)
	{
//		Dir1.X = Dir1.Y = Dir1.Z = 0;
		switch(axis)
		{
			case 'X':
			Dir1.X += dir;
			break;

			case 'Y':
			Dir1.Y += dir;
			break;

			case 'Z':
			Dir1.Z += dir;
			break;

			default:
			break;
			//goto nomove;
		}
	//	Dest = m_Vector_Add(Ent->ent_anim.Pos, Dir1);
//		col_Handle(&Ent->ent_anim.Pos, Dest, &ClipTrace);
//		Ent->SolidSpace = ClipTrace.solidspace;
	//	return;
	}

//nomove:

	if(Dir1.X || Dir1.Y || Dir1.Z)
	{
		Dest = m_Vector_Add(Ent->ent_anim.Pos, Dir1);
		col_Handle(&Ent->ent_anim.Pos, Dest, &ClipTrace);
		Ent->SolidSpace = ClipTrace.solidspace;

		ClipTrace.Response = true;
		ClipTrace.NoRadius = false;
		ClipTrace.DontUnstuck = false;
		ClipTrace.Ent = Ent;
		ClipTrace.ellipsoid = Ent->Radius;
	}

	Player_AB_Distance = m_Distance(ActionBrushTab[index].script.Pos,Ent->ent_anim.Pos);

	Dir.X = ActionBrushTab[index].script.Pos.X - Ent->ent_anim.Pos.X;
	Dir.Y = ActionBrushTab[index].script.Pos.Y - Ent->ent_anim.Pos.Y;
	Dir.Z = ActionBrushTab[index].script.Pos.Z - Ent->ent_anim.Pos.Z;
	Normalize(&Dir);

	eDistA.X = Dir.X * ActionBrushTab[index].Radius.X * ActionBrushTab[index].script.Scale.X;
	eDistA.Y = Dir.Y * ActionBrushTab[index].Radius.Y * ActionBrushTab[index].script.Scale.Y;
	eDistA.Z = Dir.Z * ActionBrushTab[index].Radius.Z * ActionBrushTab[index].script.Scale.Z;

	eDistB.X = Dir.X * Ent->Radius.X;
	eDistB.Y = Dir.Y * Ent->Radius.Y;
	eDistB.Z = Dir.Z * Ent->Radius.Z;

	eDistance = (float)Norme(eDistA) + (float)Norme(eDistB);

	if(Player_AB_Distance >= eDistance)
		return;

	m_Vector_Scale(&Dir,Player_AB_Distance-eDistance);

	Dest = m_Vector_Add(Ent->ent_anim.Pos,Dir);
//	ClipTrace.Ent = Ent;
	col_Handle(&Ent->ent_anim.Pos, Dest, &ClipTrace);
	Ent->SolidSpace = ClipTrace.solidspace;
}

// ======================================================================================= //

void script_Init(char *path)
{
	gIDactionBrush = -1;
	script_Init_parameters();
	ProcessScriptFile(path);
}

void script_Init_parameters()
{
	gWaitForAction = 0;
	gMayLaunch_ab = -1;
	gActivationDist = -1;
	ScriptTimeText = SCRIPT_TIME_DISP;
}

void script_Load_Sound(int idsound)
{
	int		i;
	int		index;

	if(gIDactionBrush < 0)
		return;
	if(idsound<0)
		return;
	for(i=0 ; i<gNb_ActionBrush ; i++)
	if(ActionBrushTab[i].ActionNumber == gIDactionBrush)
	{
		ActionBrushTab[i].script.Used = true;

		index = ActionBrushTab[i].script.NbSound;
		ActionBrushTab[i].script.sound[index].IsWaitForAction = gWaitForAction;
		ActionBrushTab[i].script.sound[index].IsPlayed = false;
		ActionBrushTab[i].script.sound[index].idsound = idsound;			
		ActionBrushTab[i].script.NbSound++;
	}
}

void script_Load_Text(char *txt)
{
	int		i;
	int		index;

	if(gIDactionBrush < 0)
		return;
	if(!txt)
		return;
	for(i=0 ; i<gNb_ActionBrush ; i++)
	if(ActionBrushTab[i].ActionNumber == gIDactionBrush)
	{
		ActionBrushTab[i].script.Used = true;

		index = ActionBrushTab[i].script.NbText;
		ActionBrushTab[i].script.text[index].IsWaitForAction = gWaitForAction;
		ActionBrushTab[i].script.text[index].TextTime = ScriptTimeText; //2500.0f;
		ScriptTimeText *= 2.0f; //
		memset(ActionBrushTab[i].script.text[index].TextString,0,MAX_LEXEME_SIZE);
		strcpy(ActionBrushTab[i].script.text[index].TextString,txt);			
		ActionBrushTab[i].script.NbText++;
	}
}

void script_Load()
{
	int		i;
	int		index;

	if(gIDactionBrush < 0)
		return;
	for(i=0 ; i<gNb_ActionBrush ; i++)
	if(ActionBrushTab[i].ActionNumber == gIDactionBrush)
	{
		ActionBrushTab[i].LaunchWaitForAction = gWaitForAction;

		// distance d'activation au niveau de l'action brush
		if(gActivationDist > -1)
			ActionBrushTab[i].ActivationDist = (float)gActivationDist;
		else
			ActionBrushTab[i].ActivationDist = NEAR_DIST_ACTIVATE;

		// activateur d'action externe au niveau de l'action brush
		if(gMayLaunch_ab > -1)
		{
			ActionBrushTab[i].MayLaunch_ab = gMayLaunch_ab;
			gMayLaunch_ab = -1;
			return;
		}

		if(Commands.type == 0)	// move
		{
			ActionBrushTab[i].script.Used = true;
			index = ActionBrushTab[i].script.NbMove;
			if(index >= MAX_MVTPERAB)
			{
				m_ConsPrint("Move error : too much instruction (max:%d)\n",MAX_MVTPERAB);
				return;
			}
			ActionBrushTab[i].script.NbMove++;
			ActionBrushTab[i].script.move[index].IsUsed = true;
			ActionBrushTab[i].script.move[index].IsWaitForAction = gWaitForAction;
			ActionBrushTab[i].script.move[index].MvtOffset.X = (float)Commands.offset[0];
			ActionBrushTab[i].script.move[index].MvtOffset.Y = (float)Commands.offset[1];
			ActionBrushTab[i].script.move[index].MvtOffset.Z = (float)Commands.offset[2];
			ActionBrushTab[i].script.move[index].MvtStartTime = (float)Commands.time[0];
			ActionBrushTab[i].script.move[index].MvtOffsetTime = (float)Commands.time[1] - (float)Commands.time[0];
		}
		else if(Commands.type == 1)	// scale
		{
			ActionBrushTab[i].script.Used = true;
			index = ActionBrushTab[i].script.NbScale;
			if(index >= MAX_MVTPERAB)
			{
				m_ConsPrint("Scale error : too much instruction (max:%d)\n",MAX_MVTPERAB);
				return;
			}
			ActionBrushTab[i].script.NbScale++;
			ActionBrushTab[i].script.scale[index].IsUsed = true;
			ActionBrushTab[i].script.scale[index].IsWaitForAction = gWaitForAction;
			ActionBrushTab[i].script.scale[index].MvtOffset.X = (float)(Commands.offset[0])/100.0f;
			ActionBrushTab[i].script.scale[index].MvtOffset.Y = (float)(Commands.offset[1])/100.0f;
			ActionBrushTab[i].script.scale[index].MvtOffset.Z = (float)(Commands.offset[2])/100.0f;
			ActionBrushTab[i].script.scale[index].MvtStartTime = (float)Commands.time[0];
			ActionBrushTab[i].script.scale[index].MvtOffsetTime = (float)Commands.time[1] - (float)Commands.time[0];
		}
		else if(Commands.type == 2) // rotate
		{

		}
		else if(Commands.type == 3) // reset
		{
			ActionBrushTab[i].script.Used = true;
			ActionBrushTab[i].IsResetWaitForAction = gWaitForAction;
			ActionBrushTab[i].ResetTime = (float)Commands.time[0];
			ActionBrushTab[i].SaveResetTime = (float)Commands.time[0];
			//m_ConsPrint("VALTEST : %d\n",Commands.time[0]);
		}
	}
}

void script_Run()
{
	int		i;
	int		j;
	int		k;
	int		m;
	int		n;
	int		index;
	vect_t	SpeedMove;
	vect_t	ScaleMove;
	vect_t	MoveOffset;
	vect_t	ScaleOffset;
	hBool	IsAllFinised;
	int		TextLine;
	hBool	IsDrawCadre;

	if(noscript.value || gIsMultiplayer)
		return;
	if(ProgramState != PS_GAME && ProgramState != PS_DIE && ProgramState != PS_WIN && ProgramState != PS_PAUSEDGAME)
		return;

	ab_PersoActivate(gCamera.Pos);
	IsAllFinised = true;

	TextLine = 23; //20 12;
	IsDrawCadre = false;

	for(i=0 ; i<gNb_ActionBrush ; i++)
	if(ActionBrushTab[i].script.Used)
	{
		//==========================================//
		//					RESET					//
		//==========================================//
		if(ActionBrushTab[i].ResetTime >= 0 && ActionBrushTab[i].IsResetWaitForAction == 0)
		{
			ActionBrushTab[i].ResetTime -= gFrameTime_msec;
			if(ActionBrushTab[i].ResetTime < 0)
			{
				ActionBrushTab[i].ResetTime = ActionBrushTab[i].SaveResetTime;
				ActionBrushTab[i].IsResetWaitForAction = ActionBrushTab[i].SaveResetWaitForAction;
			
				for(n=0 ; n<ActionBrushTab[i].script.NbMove ; n++)
					ActionBrushTab[i].script.move[n].IsWaitForAction = ActionBrushTab[i].script.move[n].SaveWaitForAction;

				for(n=0 ; n<ActionBrushTab[i].script.NbScale ; n++)
					ActionBrushTab[i].script.scale[n].IsWaitForAction = ActionBrushTab[i].script.scale[n].SaveWaitForAction;

				for(n=0 ; n<ActionBrushTab[i].script.NbText ; n++)
					ActionBrushTab[i].script.text[n].IsWaitForAction = ActionBrushTab[i].script.text[n].SaveWaitForAction;
			
				for(k=0 ; k<ActionBrushTab[i].script.NbSound ; k++)		//
					ActionBrushTab[i].script.sound[k].IsPlayed = false;	//


				//m_ConsPrint("loop!\n");

				// we may have to loop all animations
				for(j=0 ; j<ActionBrushTab[i].script.NbMove ; j++)
				{
					ActionBrushTab[i].script.move[j].MvtTime = 0;
					ActionBrushTab[i].script.move[j].MvtState = AB_NOSTARTED;
				}
				for(j=0 ; j<ActionBrushTab[i].script.NbScale ; j++)
				{
					ActionBrushTab[i].script.scale[j].MvtTime = 0;
					ActionBrushTab[i].script.scale[j].MvtState = AB_NOSTARTED;
				}

				ActionBrushTab[i].script.Pos = ActionBrushTab[i].script.FirstPos;
				ActionBrushTab[i].script.Scale.X = 1;
				ActionBrushTab[i].script.Scale.Y = 1;
				ActionBrushTab[i].script.Scale.Z = 1;
				ActionBrushTab[i].script.StartPos = ActionBrushTab[i].script.FirstPos;
				ActionBrushTab[i].script.StartScale.X = 1;
				ActionBrushTab[i].script.StartScale.Y = 1;
				ActionBrushTab[i].script.StartScale.Z = 1;
			}
		}

		//==========================================//
		//					LAUNCHER				//
		//==========================================//	
		m = ActionBrushTab[i].MayLaunch_ab;
		if(m > -1 && m < MAX_SCRIPTSLOT)
		if(ActionBrushTab[i].LaunchWaitForAction == 0)
		{
			ActionBrushTab[i].MayLaunch_ab = -1;

			for(k=0 ; k<gNb_ActionBrush ; k++)
			if(ActionBrushTab[k].script.Used)
			if(ActionBrushTab[k].ActionNumber == m)
			{
				if(ActionBrushTab[k].IsResetWaitForAction == 4)
				{
					ActionBrushTab[k].SaveResetWaitForAction = 4;
					ActionBrushTab[k].IsResetWaitForAction = 0;
				}

				for(n=0 ; n<ActionBrushTab[k].script.NbMove ; n++)
				if(ActionBrushTab[k].script.move[n].IsWaitForAction == 4)
				{
					ActionBrushTab[k].script.move[n].SaveWaitForAction = 4;
					ActionBrushTab[k].script.move[n].IsWaitForAction = 0;
				}

				for(n=0 ; n<ActionBrushTab[k].script.NbScale ; n++)
				if(ActionBrushTab[k].script.scale[n].IsWaitForAction == 4)
				{
					ActionBrushTab[k].script.scale[n].SaveWaitForAction = 4;
					ActionBrushTab[k].script.scale[n].IsWaitForAction = 0;
				}

				for(n=0 ; n<ActionBrushTab[k].script.NbText ; n++)
				if(ActionBrushTab[k].script.text[n].IsWaitForAction == 4)
				{
					ActionBrushTab[k].script.text[n].SaveWaitForAction = 4;
					ActionBrushTab[k].script.text[n].IsWaitForAction = 0;
				}
			}
		}

		//==========================================//
		//					SOUND					//
		//==========================================//
		for(k=0 ; k<ActionBrushTab[i].script.NbSound ; k++)
		if(ActionBrushTab[i].script.sound[k].IsWaitForAction == 0)
		{
			if(!ActionBrushTab[i].script.sound[k].IsPlayed)
			{
				ActionBrushTab[i].script.sound[k].IsPlayed = true;
			//	ds_PlaySound(ActionBrushTab[i].script.sound[k].idsound);
				ds_PlaySound3D(ActionBrushTab[i].script.sound[k].idsound,ActionBrushTab[i].script.Pos,ActionBrushTab[i].script.Pos);
			}
		}

		//==========================================//
		//					TEXT					//
		//==========================================//
		for(k=0 ; k<ActionBrushTab[i].script.NbText ; k++)
		if(ActionBrushTab[i].script.text[k].IsWaitForAction == 0)
		{
			if(ActionBrushTab[i].script.text[k].TextTime >= 0)
			{
				if(IsDrawCadre == false)
				{
					IsDrawCadre = true;
					Story_DispCadreAlpha();
				}
				ActionBrushTab[i].script.text[k].TextTime -= gFrameTime_msec;
				g_DispCenterString(16,TextLine,"%s",ActionBrushTab[i].script.text[k].TextString);
				TextLine++;
			}
		}

		//==========================================//
		//					MOVE					//
		//==========================================//	
		for(index=0 ; index<ActionBrushTab[i].script.NbMove ; index++)
		if(ActionBrushTab[i].script.move[index].IsUsed)
		if(ActionBrushTab[i].script.move[index].IsWaitForAction)
		{
			IsAllFinised = false;
		}
		else
		{
			ActionBrushTab[i].script.move[index].MvtTime += gFrameTime_msec;
			if(ActionBrushTab[i].script.move[index].MvtTime < ActionBrushTab[i].script.move[index].MvtStartTime)
			{
				IsAllFinised = false;
			}
			else
			{
				if(ActionBrushTab[i].script.move[index].MvtState == AB_FINISHED)
					continue;
				IsAllFinised = false;
				if(ActionBrushTab[i].script.move[index].MvtState == AB_NOSTARTED)
					ActionBrushTab[i].script.move[index].MvtState = AB_RUNNING;
				if(ActionBrushTab[i].script.move[index].MvtTime >= 
					ActionBrushTab[i].script.move[index].MvtStartTime + 
					ActionBrushTab[i].script.move[index].MvtOffsetTime)
				{
					ActionBrushTab[i].script.move[index].MvtState = AB_FINISHED;
					ActionBrushTab[i].script.Pos.X = ActionBrushTab[i].script.StartPos.X + ActionBrushTab[i].script.move[index].MvtOffset.X;
					ActionBrushTab[i].script.Pos.Y = ActionBrushTab[i].script.StartPos.Y + ActionBrushTab[i].script.move[index].MvtOffset.Y;
					ActionBrushTab[i].script.Pos.Z = ActionBrushTab[i].script.StartPos.Z + ActionBrushTab[i].script.move[index].MvtOffset.Z;

					ActionBrushTab[i].script.StartPos = ActionBrushTab[i].script.Pos;
					continue;
				}

				MoveOffset = m_Vector_Sub(ActionBrushTab[i].script.Pos,ActionBrushTab[i].script.StartPos);
				SpeedMove.X = (ActionBrushTab[i].script.move[index].MvtOffset.X * gFrameTime_msec) / ActionBrushTab[i].script.move[index].MvtOffsetTime;
				SpeedMove.Y = (ActionBrushTab[i].script.move[index].MvtOffset.Y * gFrameTime_msec) / ActionBrushTab[i].script.move[index].MvtOffsetTime;
				SpeedMove.Z = (ActionBrushTab[i].script.move[index].MvtOffset.Z * gFrameTime_msec) / ActionBrushTab[i].script.move[index].MvtOffsetTime;

				if(ActionBrushTab[i].script.move[index].MvtOffset.X > 0)
				{
					if(MoveOffset.X >= ActionBrushTab[i].script.move[index].MvtOffset.X)
					{
						ActionBrushTab[i].script.Pos.X = ActionBrushTab[i].script.StartPos.X + ActionBrushTab[i].script.move[index].MvtOffset.X;
					}
					else
					{
						ab_Push(i,SpeedMove.X,'X');
						ActionBrushTab[i].script.Pos.X += SpeedMove.X;
					}
				}
				else
				{
					if(MoveOffset.X <= ActionBrushTab[i].script.move[index].MvtOffset.X)
					{
						ActionBrushTab[i].script.Pos.X = ActionBrushTab[i].script.StartPos.X + ActionBrushTab[i].script.move[index].MvtOffset.X;
					}
					else
					{
						ab_Push(i,SpeedMove.X,'X');
						ActionBrushTab[i].script.Pos.X += SpeedMove.X;
					}
				}


				if(ActionBrushTab[i].script.move[index].MvtOffset.Y > 0)
				{
					if(MoveOffset.Y >= ActionBrushTab[i].script.move[index].MvtOffset.Y)
					{
						ActionBrushTab[i].script.Pos.Y = ActionBrushTab[i].script.StartPos.Y + ActionBrushTab[i].script.move[index].MvtOffset.Y;
					}
					else
					{
						ab_Push(i,SpeedMove.Y,'Y');
						ActionBrushTab[i].script.Pos.Y += SpeedMove.Y;
					}
				}
				else
				{
					if(MoveOffset.Y <= ActionBrushTab[i].script.move[index].MvtOffset.Y)
					{
						ActionBrushTab[i].script.Pos.Y = ActionBrushTab[i].script.StartPos.Y + ActionBrushTab[i].script.move[index].MvtOffset.Y;
					}
					else
					{
						ab_Push(i,SpeedMove.Y,'Y');
						ActionBrushTab[i].script.Pos.Y += SpeedMove.Y;
					}
				}


				if(ActionBrushTab[i].script.move[index].MvtOffset.Z > 0)
				{
					if(MoveOffset.Z >= ActionBrushTab[i].script.move[index].MvtOffset.Z)
					{
						ActionBrushTab[i].script.Pos.Z = ActionBrushTab[i].script.StartPos.Z + ActionBrushTab[i].script.move[index].MvtOffset.Z;
					}
					else
					{
						ab_Push(i,SpeedMove.Z,'Z');
						ActionBrushTab[i].script.Pos.Z += SpeedMove.Z;
					}
				}
				else
				{
					if(MoveOffset.Z <= ActionBrushTab[i].script.move[index].MvtOffset.Z)
					{
						ActionBrushTab[i].script.Pos.Z = ActionBrushTab[i].script.StartPos.Z + ActionBrushTab[i].script.move[index].MvtOffset.Z;
					}
					else
					{
						ab_Push(i,SpeedMove.Z,'Z');
						ActionBrushTab[i].script.Pos.Z += SpeedMove.Z;
					}
				}
			}
		}

		//==========================================//
		//					SCALE					//
		//==========================================//
		for(index=0 ; index<ActionBrushTab[i].script.NbScale ; index++)
		if(ActionBrushTab[i].script.scale[index].IsUsed)
		if(ActionBrushTab[i].script.scale[index].IsWaitForAction)
		{
			IsAllFinised = false;
		}
		else
		{
			ActionBrushTab[i].script.scale[index].MvtTime += gFrameTime_msec;
			if(ActionBrushTab[i].script.scale[index].MvtTime < ActionBrushTab[i].script.scale[index].MvtStartTime)
			{
				IsAllFinised = false;
			}
			else
			{
				if(ActionBrushTab[i].script.scale[index].MvtState == AB_FINISHED)
					continue;
				IsAllFinised = false;
				if(ActionBrushTab[i].script.scale[index].MvtState == AB_NOSTARTED)
					ActionBrushTab[i].script.scale[index].MvtState = AB_RUNNING;
				if(ActionBrushTab[i].script.scale[index].MvtTime >= 
					ActionBrushTab[i].script.scale[index].MvtStartTime + 
					ActionBrushTab[i].script.scale[index].MvtOffsetTime)
				{
					ActionBrushTab[i].script.scale[index].MvtState = AB_FINISHED;
					ActionBrushTab[i].script.Scale.X = ActionBrushTab[i].script.scale[index].MvtOffset.X;
					ActionBrushTab[i].script.Scale.Y = ActionBrushTab[i].script.scale[index].MvtOffset.Y;
					ActionBrushTab[i].script.Scale.Z = ActionBrushTab[i].script.scale[index].MvtOffset.Z;

					ActionBrushTab[i].script.StartScale = ActionBrushTab[i].script.Scale;
					continue;
				}

				ScaleOffset = m_Vector_Sub(ActionBrushTab[i].script.Scale,ActionBrushTab[i].script.StartScale);
				ScaleMove.X = ((ActionBrushTab[i].script.scale[index].MvtOffset.X-ActionBrushTab[i].script.StartScale.X) * gFrameTime_msec) / ActionBrushTab[i].script.scale[index].MvtOffsetTime;
				ScaleMove.Y = ((ActionBrushTab[i].script.scale[index].MvtOffset.Y-ActionBrushTab[i].script.StartScale.Y) * gFrameTime_msec) / ActionBrushTab[i].script.scale[index].MvtOffsetTime;
				ScaleMove.Z = ((ActionBrushTab[i].script.scale[index].MvtOffset.Z-ActionBrushTab[i].script.StartScale.Z) * gFrameTime_msec) / ActionBrushTab[i].script.scale[index].MvtOffsetTime;

				if(ActionBrushTab[i].script.scale[index].MvtOffset.X-ActionBrushTab[i].script.StartScale.X < 0)
				{
					if(ScaleOffset.X <= ActionBrushTab[i].script.scale[index].MvtOffset.X-ActionBrushTab[i].script.StartScale.X)
					{
						ActionBrushTab[i].script.Scale.X = ActionBrushTab[i].script.scale[index].MvtOffset.X;
					}
					else
					{
						ab_Push(i,ScaleMove.X,'S');
						ActionBrushTab[i].script.Scale.X += ScaleMove.X;
					}
				}
				else
				{
					if(ScaleOffset.X >= ActionBrushTab[i].script.scale[index].MvtOffset.X-ActionBrushTab[i].script.StartScale.X)
					{
						ActionBrushTab[i].script.Scale.X = ActionBrushTab[i].script.scale[index].MvtOffset.X;
					}
					else
					{
						ab_Push(i,ScaleMove.X,'S');
						ActionBrushTab[i].script.Scale.X += ScaleMove.X;
					}
				}


				if(ActionBrushTab[i].script.scale[index].MvtOffset.Y-ActionBrushTab[i].script.StartScale.Y < 0)
				{
					if(ScaleOffset.Y <= ActionBrushTab[i].script.scale[index].MvtOffset.Y-ActionBrushTab[i].script.StartScale.Y)
					{
						ActionBrushTab[i].script.Scale.Y = ActionBrushTab[i].script.scale[index].MvtOffset.Y;
					}
					else
					{
						ab_Push(i,ScaleMove.Y,'S');
						ActionBrushTab[i].script.Scale.Y += ScaleMove.Y;
					}
				}
				else
				{
					if(ScaleOffset.Y >= ActionBrushTab[i].script.scale[index].MvtOffset.Y-ActionBrushTab[i].script.StartScale.Y)
					{
						ActionBrushTab[i].script.Scale.Y = ActionBrushTab[i].script.scale[index].MvtOffset.Y;
					}
					else
					{
						ab_Push(i,ScaleMove.Y,'S');
						ActionBrushTab[i].script.Scale.Y += ScaleMove.Y;
					}
				}


				if(ActionBrushTab[i].script.scale[index].MvtOffset.Z-ActionBrushTab[i].script.StartScale.Z < 0)
				{
					if(ScaleOffset.Z <= ActionBrushTab[i].script.scale[index].MvtOffset.Z-ActionBrushTab[i].script.StartScale.Z)
					{
						ActionBrushTab[i].script.Scale.Z = ActionBrushTab[i].script.scale[index].MvtOffset.Z;
					}
					else
					{
						ab_Push(i,ScaleMove.Z,'S');
						ActionBrushTab[i].script.Scale.Z += ScaleMove.Z;
					}
				}
				else
				{
					if(ScaleOffset.Z >= ActionBrushTab[i].script.scale[index].MvtOffset.Z-ActionBrushTab[i].script.StartScale.Z)
					{
						ActionBrushTab[i].script.Scale.Z = ActionBrushTab[i].script.scale[index].MvtOffset.Z;
					}
					else
					{
						ab_Push(i,ScaleMove.Z,'S');
						ActionBrushTab[i].script.Scale.Z += ScaleMove.Z;
					}
				}
			}
		}
	}
}

//================================== HSL ===============================
//======================================================================

hBool ProcessScriptFile(char *path)
{
	int		i;
	int		res;
	char	*p = NULL;

	file = NULL;
	m_ConsPrint("### Load script: %s\n", path);

#ifdef H_MAC
	p = KMiscTools::makeFilePath(path);
	m_ConsPrint("### Load script: %s\n", p);
	file = fopen(p, "r"); // r+b
#else
	file = fopen(path,"r+b");
#endif
	
	if(!file) {
		return false;
	}
	memset(instruction,0,MAX_LEXEME_SIZE);

	i = 0;
	while(1)
	{
		i++;
		//if(i > 15)	break;

		res = Instruction();
		if(res)
			break;
	}
	m_ConsPrint("%d Error(s).\n",gNbErrors);
	fclose(file);
	return true;
}

int AnalyseLexeme()
{
	int		ActionType;

	if(!CheckLexeme())
	{
		gNbErrors++;
		m_ConsPrint("Line %d : %s : lexeme error\n",gNumLigne,instruction);
	}

	//	actionbrush INTEGER { }
	if(!strcmp(instruction,Lexemes[4]))
	{
		script_Init_parameters(); // re init all bloc parameters (loop, activatedist, etc..)

		inst_type = CheckSyntax();
		if(inst_type != I_DIGIT)
		{
			gNbErrors++;
			m_ConsPrint("Line %d : lexeme with no integer argument error\n",gNumLigne);
			return 1;
		}
		gIDactionBrush = gDigit;
				
		inst_type = CheckSyntax();
		if(inst_type != I_SYMBOLE)
		{
			gNbErrors++;
			m_ConsPrint("Line %d : { block missing error 1\n",gNumLigne);
		}
		else
		{
			if(gSymbole != *Symboles[5])
			{
				gNbErrors++;
				m_ConsPrint("Line %d : { block missing error 2\n",gNumLigne);
			}
		}

		if(Block()==1)
		{
			gNbErrors++;
			m_ConsPrint("Line %d : } block missing error\n",gNumLigne);
			return 1;
		}
	}
	// move/scale/rotate (INT,INT,INT) [INT] INT INT INT
	else if( !strcmp(instruction,Lexemes[0]) || !strcmp(instruction,Lexemes[1]) || !strcmp(instruction,Lexemes[2]))
	{
		if(!strcmp(instruction,Lexemes[0]))
			Commands.type = 0;
		if(!strcmp(instruction,Lexemes[1]))
			Commands.type = 1;
		if(!strcmp(instruction,Lexemes[2]))
			Commands.type = 2;

		inst_type = CheckSyntax();
		if(inst_type != I_SYMBOLE || gSymbole != *Symboles[0])
		{
			gNbErrors++;
			m_ConsPrint("Line %d : lexeme with ?,,) syntax error\n",gNumLigne);
			return 1;
		}
		inst_type = CheckSyntax();
		if(inst_type != I_DIGIT)
		{
			gNbErrors++;
			m_ConsPrint("Line %d : lexeme with bad (?,,) argument error\n",gNumLigne);
			return 1;
		}
		Commands.offset[0] = gDigit;
		inst_type = CheckSyntax();
		if(inst_type != I_SYMBOLE || gSymbole != *Symboles[4])
		{
			gNbErrors++;
			m_ConsPrint("Line %d : lexeme with (?,) syntax error\n",gNumLigne);
			return 1;
		}
		inst_type = CheckSyntax();
		if(inst_type != I_DIGIT)
		{
			gNbErrors++;
			m_ConsPrint("Line %d : lexeme with bad (,?,) argument error\n",gNumLigne);
			return 1;
		}
		Commands.offset[1] = gDigit;
		inst_type = CheckSyntax();
		if(inst_type != I_SYMBOLE || gSymbole != *Symboles[4])
		{
			gNbErrors++;
			m_ConsPrint("Line %d : lexeme with (,?) syntax error\n",gNumLigne);
			return 1;
		}
		inst_type = CheckSyntax();
		if(inst_type != I_DIGIT)
		{
			gNbErrors++;
			m_ConsPrint("Line %d : lexeme with bad (,,?) argument error\n",gNumLigne);
			return 1;
		}
		Commands.offset[2] = gDigit;
		inst_type = CheckSyntax();
		if(inst_type != I_SYMBOLE || gSymbole != *Symboles[1])
		{
			gNbErrors++;
			m_ConsPrint("Line %d : lexeme with (,,? syntax error\n",gNumLigne);
			return 1;
		}

		inst_type = CheckSyntax();
		if(inst_type != I_SYMBOLE || gSymbole != *Symboles[2])
		{
			gNbErrors++;
			m_ConsPrint("Line %d : lexeme with [ syntax error\n",gNumLigne);
			return 1;
		}
		inst_type = CheckSyntax();
		if(inst_type != I_DIGIT)
		{
			gNbErrors++;
			m_ConsPrint("Line %d : lexeme with bad [?.] argument error\n",gNumLigne);
			return 1;
		}
		Commands.time[0] = gDigit;
		inst_type = CheckSyntax();
		if(inst_type != I_DIGIT)
		{
			gNbErrors++;
			m_ConsPrint("Line %d : lexeme with bad [.?] argument error\n",gNumLigne);
			return 1;
		}
		Commands.time[1] = gDigit;
		inst_type = CheckSyntax();
		if(inst_type != I_SYMBOLE || gSymbole != *Symboles[3])
		{
			gNbErrors++;
			m_ConsPrint("Line %d : lexeme with ] syntax error\n",gNumLigne);
			return 1;
		}

		if(gIDactionBrush < MAX_SCRIPTSLOT)
			script_Load();
	}
	// reset [INT]
	else if(!strcmp(instruction,Lexemes[3]))
	{
		Commands.type = 3;	// reset instruction type
		inst_type = CheckSyntax();
		if(inst_type != I_SYMBOLE || gSymbole != *Symboles[2])
		{
			gNbErrors++;
			m_ConsPrint("Line %d : lexeme with [ syntax error\n",gNumLigne);
			return 1;
		}

		inst_type = CheckSyntax();
		if(inst_type != I_DIGIT)
		{
			gNbErrors++;
			m_ConsPrint("Line %d : lexeme with bad [?] argument error\n",gNumLigne);
			return 1;
		}
		Commands.time[0] = gDigit;
		//m_ConsPrint("TEST : %d\n",gDigit);

		inst_type = CheckSyntax();
		if(inst_type != I_SYMBOLE || gSymbole != *Symboles[3])
		{
			gNbErrors++;
			m_ConsPrint("Line %d : lexeme with ] syntax error\n",gNumLigne);
			return 1;
		}
		script_Load();
	}
	//	ifAction( TOKEN )
	else if(!strcmp(instruction,Lexemes[5]))
	{
		inst_type = CheckSyntax();
		if(inst_type != I_SYMBOLE || gSymbole != *Symboles[0])
		{
			gNbErrors++;
			m_ConsPrint("Line %d : lexeme with ? ) syntax error\n",gNumLigne);
			return 1;
		}
		inst_type = CheckSyntax();
		if(inst_type != I_LEXEME ||
			(	strcmp(instruction,Lexemes[7]) != 0 && 
				strcmp(instruction,Lexemes[8]) != 0 &&
				strcmp(instruction,Lexemes[9]) != 0	&&
				strcmp(instruction,Lexemes[10]) != 0 ) )
		{
			gNbErrors++;
			m_ConsPrint("Line %d : lexeme error (?) error\n",gNumLigne);
			return 1;
		}
		
		ActionType = 0;
		if(!strcmp(instruction,Lexemes[7]))
			ActionType = 1;
		if(!strcmp(instruction,Lexemes[8]))
			ActionType = 2;
		if(!strcmp(instruction,Lexemes[9]))
			ActionType = 3;
		if(!strcmp(instruction,Lexemes[10]))
			ActionType = 4;

		inst_type = CheckSyntax();
		if(inst_type != I_SYMBOLE || gSymbole != *Symboles[1])
		{
			gNbErrors++;
			m_ConsPrint("Line %d : lexeme with ( ? syntax error\n",gNumLigne);
			return 1;
		}

		gWaitForAction = ActionType;
	}
	//	Launchactionbrush ID
	else if(!strcmp(instruction,Lexemes[6]))
	{
		inst_type = CheckSyntax();
		if(inst_type != I_DIGIT)
		{
			gNbErrors++;
			m_ConsPrint("Line %d : lexeme with no integer argument error\n",gNumLigne);
			return 1;
		}
		gMayLaunch_ab = gDigit;
		//m_ConsPrint("sun %d\n",gMayLaunch_ab);
		if(gIDactionBrush < MAX_SCRIPTSLOT)
			script_Load();
	}
	//	ActivationDist ID
	else if(!strcmp(instruction,Lexemes[11]))
	{
		inst_type = CheckSyntax();
		if(inst_type != I_DIGIT)
		{
			gNbErrors++;
			m_ConsPrint("Line %d : lexeme ActivationDist argument error\n",gNumLigne);
			return 1;
		}
		gActivationDist = gDigit;
	}
	//	DispText STRING
	else if(!strcmp(instruction,Lexemes[12]))
	{
		inst_type = CheckSyntax();
		if(inst_type != I_TEXT)
		{
			gNbErrors++;
			m_ConsPrint("Line %d : lexeme << DispText >> with no text argument error\n",gNumLigne);
			return 1;
		}
		//printf("%s\n",gText);
		script_Load_Text(gText);
	}
	//	PlaySound STRING
	else if(!strcmp(instruction,Lexemes[13]))
	{
		inst_type = CheckSyntax();
		if(inst_type != I_DIGIT)
		{
			gNbErrors++;
			m_ConsPrint("Line %d : lexeme << DispText >> with no text argument error\n",gNumLigne);
			return 1;
		}
		script_Load_Sound(gDigit);
	}
	return 0;
}

int Instruction()
{
	inst_type = CheckSyntax();

	if(inst_type == I_LEXEME)
	{
		if(AnalyseLexeme())
			return 1;
	}

	if(inst_type == I_EOF)
		return 1;
	return 0;
}

int Block()
{
	while(1)
	{
		inst_type = CheckSyntax();

		if(inst_type == I_LEXEME)
		{
			if(AnalyseLexeme())
				return 1;
		}
		else if(inst_type == I_SYMBOLE)
		{
			if(gSymbole == *Symboles[6])	// } token
			{
				script_Init_parameters(); // re init all bloc parameters (loop, activatedist, etc..)
				break;
			}
		}
		else if(inst_type == I_EOF)
		{
			return 1;
		}
	}
	return 0;
}

hBool CheckSymbole(char c)
{
	int		i;

	for(i=0 ; i<MAX_SYMBOLES ; i++)
	{
		if(!strcmp(&c,Symboles[i]))
			return true;
	}
	return false;
}

hBool CheckLexeme()
{
	int		i;

	for(i=0 ; i<MAX_LEXEMES ; i++)
	{
		if(!strcmp(instruction,Lexemes[i]))
			return true;
	}
	return false;
}

int CheckSyntax()
{
	char	c;
	int		index;
	hBool	Neg = false;

	if(!file)
		return I_EOF;

	while(1)
	{
		c = fgetc(file);
		if(c == EOF)
			return I_EOF;

		if(c == ' ' || c == '\t' || c == 13) // 13 truc win
		{
			continue;
		}
		else if(c == '\n')
		{
			gNumLigne++;
			continue;
		}
		else if(isdigit(c))
		{ 
			ungetc(c, file);
			fscanf(file,"%d",&gDigit);
			if(Neg)
				gDigit = -gDigit;
			return I_DIGIT;
		}
		else if(isalpha (c))
		{
			index = 0;
			while(isalnum(c))
			{
				instruction[index] = c;
				c=fgetc(file);
				if(c == EOF)
					return I_EOF;
				index++;
				if(index>= MAX_LEXEME_SIZE)
				{
					m_ConsPrint("Erreur du compilateur #1");
					return I_EOF;
				}
			}
			instruction[index] = 0;
			return I_LEXEME;
		}
		else
		{
			if(c == '\"')
			{
				index = 0;
				while(1)
				{
					c = fgetc(file);
					if(c == EOF)
						return I_EOF;
					if(c == '\"')
						break;
					if(c == '\n')
					{
						m_ConsPrint("Erreur du compilateur #2");
						return I_EOF;
					}
					gText[index++] = c;
					if(index >= MAX_LEXEME_SIZE)
					{
						m_ConsPrint("Erreur du compilateur #3");
						return I_EOF;
					}
				}
				gText[index] = 0;
				return I_TEXT;
			}
			else if(c == '-')
			{
				Neg = !Neg;
				continue;
			}
			else if(c == ';')
			{
				while(c != '\n')
				{
					c = fgetc(file);
					if(c == EOF)
						return I_EOF;
				}
				continue;
			}
			
			gSymbole = c;
	//		m_ConsPrint("gSymbole: %c\n", gSymbole);
			return I_SYMBOLE;
		}
	}
	return I_UNKNOW;
}



