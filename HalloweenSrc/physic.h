
#ifndef PHYSIC_INCLUDED
#define PHYSIC_INCLUDED

#include "alloween.h"


#define	WEAPON_RELOADTIME		500.0f
#define	TROMBLON_RELOADTIME		800.0f


vect_t	phy_Friction(hBool JumpPossible, vect_t speedvel);
hBool	phy_BoundPos(vect_t *Pos);
float	phy_GetEntityMaxSpeed(int entid, hBool JumpPossible);
void	phy_BoundVelocity(vect_t *Vel, float MaxVel);
vect_t	phy_AdjustVelocity(int entity_id, hBool JumpPossible, vect_t speedvel);
pEntity	phy_AdjustGravity(pEntity Ent);
vect_t	phy_MoveVelocity(int entity_id, hBool JumpPossible, vect_t SpeedVel);
hBool	phy_IsVelocity(int entid);
void	phy_ResetVelocity(int entid);
void	phy_UpdateVelocity(pEntity CreatureEnt);
pEntity	phy_UpdateMoves(pEntity CreatureEnt);
void	phy_SlowMove(vect_t *Vel, float factor);
void	phy_SlowMove_2(vect_t *Vel, float factor);

pEntity Gravity(pEntity CreatureEnt, float factor);
hBool	CollideEntity(pEntity EntA, pEntity EntB, float factor);
hBool	CollideEntity_Better(pEntity EntA, pEntity EntB);
void	Launch(pEntity CreatureEnt, int id, float Speed);
void	JumpEntity(pEntity Entity);
void	JumpCreature(int id);
void	MoveEntity_Forward(pEntity Entity);
void	MoveCreature(pEntity CreatureEnt, vect_t MoveStep);
hBool	IsGround_UpCreature(pEntity CreatureEnt);
int		IsGround_UnderCreature(pEntity CreatureEnt, float factor);
hBool	IsGround2_UpCreature(pEntity CreatureEnt);
hBool	IsEntity_UnderCreature(pEntity CreatureEnt, pEntity Ent);
hBool	IsEntity_UpperCreature(pEntity CreatureEnt, pEntity Ent);




#endif // PHYSIC_INCLUDED
