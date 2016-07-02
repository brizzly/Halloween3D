
#ifndef PLAYER_INCLUDED
#define PLAYER_INCLUDED

#include "alloween.h"


enum
{
	DIR_FORWARD,
	DIR_BACKWARD,
	DIR_STRAFLEFT,
	DIR_STRAFRIGHT
};


void ply_Movedir(int entid, int dir);
void UpdatePlayer();
void PlayerHurt(pEntity Entity, int dammage);
void EnnemyDammage(pEntity PlayerEnt);
void PickupItems(pEntity PlayerEnt);
void LaunchPlayer(int id, float speed);
void MovePlayer_Up();
void MovePlayer_Down();
void PlayerResetStartPos(int ent_id);
void SabreWulf();
void SabreStop();
void PlayerSwapWeapon(int weapon);




#endif // PLAYER_INCLUDED
