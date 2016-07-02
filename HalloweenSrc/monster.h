
#ifndef MONSTER_INCLUDED
#define MONSTER_INCLUDED

#include "alloween.h"

pEntity monster_Die(pEntity MonsterEnt, Model *model, hBool OkGoreF);
void	monster_DieSound(pEntity MonsterEnt);
void	monster_AttackSound(pEntity MonsterEnt);
void	monster_NormalSound(pEntity MonsterEnt);
void	monster_UpdateCounter(pEntity MonsterEnt);
pEntity	MonsterDamage(pEntity MonsterEnt, Model *model);
pEntity UpdateMonster(pEntity PlayerEnt, pEntity MonsterEnt);
void	TurnLeft_Monster(pEntity MonsterEnt);
void	TurnRight_Monster(pEntity MonsterEnt);


#endif // MONSTER_INCLUDED
