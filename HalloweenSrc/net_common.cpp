
#include "alloween.cpp"
#include "net_common.h"

#include <OpenPlay.h>


NSpGameReference	_gameReference = NULL;
hBool				_response;
hBool				_approved;
HANDLE				hReceiveThread = NULL;
HANDLE				hSendThread = NULL;
LPVOID				lpParameter;
LPVOID				lpParameter2;
DWORD				dwIdReceiveThread = 0;
DWORD				dwIdSendThread = 0;
DWORD WINAPI		net_Process2(LPVOID lpThreadParameter);
DWORD WINAPI		net_Process(LPVOID lpThreadParameter);
char				gNetBitKeys;
char				gNetBitKeys_back;
uint				gSV_FrameID;
uint				gCL_FrameID;
int					gSV_score;
int					gCL_score;








void net_Init()
{
	gMultiPodium = false;
	gSV_FrameID = -1;
	gCL_FrameID = -1;
	gSV_score = 0;
	gCL_score = 0;
	gNetBitKeys = 0;
	gNetBitKeys_back = 0;
	hReceiveThread = CreateThread(NULL,0,net_Process,lpParameter,0,&dwIdReceiveThread);
	hSendThread = CreateThread(NULL,0,net_Process2,lpParameter2,0,&dwIdSendThread);
}

void net_UnInit()
{
	CloseHandle(hReceiveThread);
	CloseHandle(hSendThread);
}

void net_StopMultiplayer()
{
//	if(gIsMultiplayer)
//	{
		gIsMultiplayer = false;
		net_UnInit();
		if(gIsServer)
			sv_Destroy();
		else
			cl_Destroy();
		game_Init();
//	}
}

DWORD WINAPI net_Process2(LPVOID lpThreadParameter)
{
	while(1)
	{
		if(!gIsMultiplayer)
			break;
		Sleep(1);
		net_UpdateMessage();
	}

	ExitThread(0);
	return 0;
}

DWORD WINAPI net_Process(LPVOID lpThreadParameter)
{
	while(1)
	{
		if(!gIsMultiplayer)
			break;
		Sleep(10); // 17 20
		while(1)
		{
			if(!gIsMultiplayer)
				break;
			if(!net_HandleMessage())
				break;
		}
	}

	ExitThread(0);
	return 0;
}

float net_GetElapsedTime_msec(float LastTime_msec)
{
	return (float)clock() - LastTime_msec;
}

void net_DispPodium()
{
	// en cas d'egalite on remet du temps
	if(gSV_score == gCL_score)
	{
		if(gIsServer)
		{
			game_clock = 60;
			net_SendServerTime();
		}
		return;
	}

	gMultiPodium = true;
	g_DispCenterString(26,4,"TOURNAMENT RESULT");

	if(gSV_score > gCL_score)
	{
		g_DispCenterString(26,8,"DEPLA won the battle    (%d)", gSV_score);
		g_DispCenterString(26,10,"MORBAK lost the battle    (%d)", gCL_score);
	}
	else
	{
		g_DispCenterString(26,8,"MORBAK won the battle    (%d)", gCL_score);
		g_DispCenterString(26,10,"DEPLA lost the battle    (%d)", gSV_score);
	}
}

void net_DispNetInfos()
{
	// HACK
	if(developer.value)
		g_DispStringLine(14, 24, "time left: %d", (int)game_clock);

	if(gIsServer)
	{
		if(ProgramState == PS_GAME && gConnectedPlayer <= 1)
			g_DispCenterString(26,4,"Awaiting Player");

		if(ProgramState == PS_GAME || ProgramState == PS_DIE)
		if(gConnectedPlayer > 1)
		{
			g_DispStringLine(14, 26, "Depla: %d  Morbak: %d", gSV_score, gCL_score);
		}

		if(developer.value)
		if(ProgramState == PS_GAME)
			g_DispString(14, "SV_Frame : %d", (int)gSV_FrameID);
	}
	else
	{
		if(ProgramState == PS_GAME || ProgramState == PS_DIE)
		if(gConnectedPlayer > 1)
		{
			g_DispStringLine(14, 26, "Morbak: %d  Depla: %d", gCL_score, gSV_score);
		}

		if(developer.value)
		if(ProgramState == PS_GAME)
			g_DispString(14, "CL_Frame : %d", (int)gCL_FrameID);
	}
}

void net_SendServerTime()
{
	Net_Time	NetTime;
	char		buffer[40];

	if(!gIsMultiplayer || !gIsServer)
		return;

	NetTime.timer = (short)game_clock;
	memcpy(buffer, &NetTime, sizeof(Net_Time));
	sv_Send2(buffer, sizeof(Net_Time), PacketType_CLTime, true);
}

void net_SendServerScores()
{
	Net_Score	NetScore;
	char		buffer[40];

	if(!gIsMultiplayer || !gIsServer)
		return;

	NetScore.SV_score = gSV_score;
	NetScore.CL_score = gCL_score;
	memcpy(buffer, &NetScore, sizeof(Net_Score));
	sv_Send2(buffer, sizeof(Net_Score), PacketType_CLScore, true);
}

void net_SendVectorState(pEntity Ent, int vstate)
{
	Net_VSent	NetVSent;
	char		buffer[40];

	if(!Ent)
		return;
	if(!gIsServer)
		return;

	NetVSent.Ent_ID = Ent->entity_id;
	NetVSent.Ent_Pos = o_FindEntityClonePos(Ent);
	NetVSent.vtype = vstate;
	if(vstate == 1)
	{
		NetVSent.vec[0] = Ent->ent_anim.Pos_Acc.X;
		NetVSent.vec[1] = Ent->ent_anim.Pos_Acc.Y;
		NetVSent.vec[2] = Ent->ent_anim.Pos_Acc.Z;
	}
	memcpy(buffer, &NetVSent, sizeof(Net_VSent));
	sv_Send2(buffer, sizeof(Net_VSent), PacketType_ClVectorState, true);
}

void net_RemoveClientEnt(pEntity Ent)
{
	Net_ARent	NetARent;
	char		buffer[40];

	NetARent.Action = false;
	NetARent.Ent_ID = Ent->entity_id;
	NetARent.Ent_Pos = o_FindEntityClonePos(Ent);
	memcpy(buffer, &NetARent, sizeof(Net_ARent));
	sv_Send2(buffer, sizeof(Net_ARent), PacketType_ARent, true);
}

void net_NetLaunchEnt(int entid, int launcherid, vect_t startpos, vect_t rot, vect_t vel, short IsServer)
{
	Net_Ent		NetEnt;
	char		buffer[40];

	NetEnt.Ent_ID = entid;
	NetEnt.Ent_parentid = (byte)launcherid;
	NetEnt.Ent_Pos = 1 + o_FindEntityClone(entid);

	NetEnt.pos[0] = startpos.X;
	NetEnt.pos[1] = startpos.Y;
	NetEnt.pos[2] = startpos.Z;

	NetEnt.rot[0] = (short)net_Float2Short(rot.X, SHORT2FLOATPREC);
	NetEnt.rot[1] = (short)net_Float2Short(rot.Y, SHORT2FLOATPREC);
	NetEnt.rot[2] = (short)net_Float2Short(rot.Z, SHORT2FLOATPREC);

	NetEnt.vel[0] = net_Float2Short(vel.X, SHORT2FLOATPREC);
	NetEnt.vel[1] = net_Float2Short(vel.Y, SHORT2FLOATPREC);
	NetEnt.vel[2] = net_Float2Short(vel.Z, SHORT2FLOATPREC);

	memcpy(buffer,&NetEnt,sizeof(Net_Ent));

	if(IsServer)
	{
		sv_Send2(buffer, sizeof(Net_Ent), PacketType_SvLaunchEnt, true);
	}
	else
	{
		sv_Send2(buffer, sizeof(Net_Ent), PacketType_ClLaunchEnt, true);
	}
}

void net_SendModelAnim(pEntity Ent, int pos)
{
	Net_AnimID	NetAnimID;
	char		buffer[40];
	int			ent_id;

	if(!Ent)
		return;

	ent_id = Ent->entity_id;
	if(ent_id == PERSO)
		ent_id = DEPLA;
	NetAnimID.Ent_ID = ent_id;
	NetAnimID.Ent_Pos = pos;
	NetAnimID.new_animation_ID = Ent->Animation_ID;

	memcpy(buffer,&NetAnimID,sizeof(Net_AnimID));
	sv_Send2(buffer, sizeof(Net_AnimID), PacketType_GetAnimID, true);
}

void net_SendClientHurt(int health)
{
	char			buffer[40];
	Net_Ent_1val	NetEnt_1;

	if(!gIsMultiplayer || !gIsServer)
		return;

	NetEnt_1.val = (short)health;
	memcpy(buffer,&NetEnt_1,sizeof(Net_Ent_1val));
	sv_Send2(buffer, sizeof(Net_Ent_1val), PacketType_ClientHurted, true);
}




void net_SVsend_Frame()
{
	int		nbPerso;
	int		nbMorbak;
	int		nbBombe;
//	vect_t	next_pos;
//	vect_t	speedvel;
	pEntity	Ent;
	Net_Ent	NetEnt;
	float	Step;
	char	buffer[40];


	gSV_FrameID++;
	sprintf(buffer, "%d", (int)gSV_FrameID);
	sv_Send(buffer, PacketType_SVframe, !false);

	// serveur envoi le timer
	net_SendServerTime();

	// serveur envoi les entitee
	nbPerso = 0;
	nbMorbak = 0;
	nbBombe = 0;
	Step = SHORT2FLOATPREC;

	for(Ent=gEntitiesList ; Ent ; Ent=Ent->Next)
	{
		// reinit les deltas
		Ent->ent_anim.net_OldPos = Ent->ent_anim.Pos;
		Ent->ent_anim.net_OldVel = Ent->ent_anim.Pos_Vel;
		Ent->ent_anim.net_OldAcc = Ent->ent_anim.Pos_Acc;
		Ent->ent_anim.net_OldRot = Ent->ent_anim.Rot;

		if(Ent->entity_id == PERSO)
		{
			nbPerso++;

			NetEnt.Ent_Pos = (byte)nbPerso;
			NetEnt.Ent_ID = (byte)Ent->entity_id;

			NetEnt.pos[0] = Ent->ent_anim.Pos.X;
			NetEnt.pos[1] = Ent->ent_anim.Pos.Y;
			NetEnt.pos[2] = Ent->ent_anim.Pos.Z;

			NetEnt.rot[0] = (short)net_Float2Short(Ent->ent_anim.Rot.X, SHORT2FLOATPREC0);
			NetEnt.rot[1] = (short)net_Float2Short(Ent->ent_anim.Rot.Y, SHORT2FLOATPREC0);
			NetEnt.rot[2] = (short)net_Float2Short(Ent->ent_anim.Rot.Z, SHORT2FLOATPREC0);

			memcpy(buffer,&NetEnt,sizeof(Net_Ent));
			sv_Send2(buffer, sizeof(Net_Ent), PacketType_UpdateEnt, !false);

			if(Ent->net_SendNewAnim)
			{
				Ent->net_SendNewAnim = false;
				if(ProgramState != PS_PAUSEDGAME)
					net_SendModelAnim(Ent, nbPerso);
			}
		}
		else if(Ent->entity_id == MORBAK)
		{
			nbMorbak++;

			NetEnt.Ent_Pos = (byte)nbMorbak;
			NetEnt.Ent_ID = (byte)Ent->entity_id;

			NetEnt.pos[0] = Ent->ent_anim.Pos.X;
			NetEnt.pos[1] = Ent->ent_anim.Pos.Y;
			NetEnt.pos[2] = Ent->ent_anim.Pos.Z;

			NetEnt.vel[0] = Ent->ent_anim.Pos_Vel.X;
			NetEnt.vel[1] = Ent->ent_anim.Pos_Vel.Y;
			NetEnt.vel[2] = Ent->ent_anim.Pos_Vel.Z;

			NetEnt.acc[0] = Ent->ent_anim.Pos_Acc.X;
			NetEnt.acc[1] = Ent->ent_anim.Pos_Acc.Y;
			NetEnt.acc[2] = Ent->ent_anim.Pos_Acc.Z;

			NetEnt.rot[0] = 0;
			NetEnt.rot[1] = 0;
			NetEnt.rot[2] = 0;

			memcpy(buffer,&NetEnt,sizeof(Net_Ent));
			sv_Send2(buffer, sizeof(Net_Ent), PacketType_UpdateEnt, !false);
		}
		else if(Ent->entity_id == BOMBE)
		{
			nbBombe++;

			NetEnt.Ent_Pos = (byte)nbBombe;
			NetEnt.Ent_ID = (byte)Ent->entity_id;

			NetEnt.pos[0] = Ent->ent_anim.Pos.X;
			NetEnt.pos[1] = Ent->ent_anim.Pos.Y;
			NetEnt.pos[2] = Ent->ent_anim.Pos.Z;

			NetEnt.vel[0] = Ent->ent_anim.Pos_Vel.X;
			NetEnt.vel[1] = Ent->ent_anim.Pos_Vel.Y;
			NetEnt.vel[2] = Ent->ent_anim.Pos_Vel.Z;

			NetEnt.rot[0] = 0;
			NetEnt.rot[1] = 0;
			NetEnt.rot[2] = 0;

			memcpy(buffer,&NetEnt,sizeof(Net_Ent));
			sv_Send2(buffer, sizeof(Net_Ent), PacketType_UpdateEnt, true);
		}
	}
	if(!nbPerso)
	{
		net_StopMultiplayer();
		m_ConsPrint("No player found in the game, exiting server\n");
	}
}

void net_SVsend_DeltaCompressed_Frame()
{
	int		nbPerso;
	int		nbMorbak;
	int		nbBombe;
	pEntity	Ent;
	vect_t	delta_pos;
	vect_t	delta_vel;
	vect_t	delta_acc;
	vect_t	delta_rot;

	nbPerso = 0;
	nbMorbak = 0;
	nbBombe = 0;

	for(Ent=gEntitiesList ; Ent ; Ent=Ent->Next)
	{
		//
		// calcul les deltas des vecteurs position, velocitee et accelerations

		delta_pos.X = Ent->ent_anim.Pos.X - Ent->ent_anim.net_OldPos.X;
		delta_pos.Y = Ent->ent_anim.Pos.Y - Ent->ent_anim.net_OldPos.Y;
		delta_pos.Z = Ent->ent_anim.Pos.Z - Ent->ent_anim.net_OldPos.Z;

		delta_vel.X = Ent->ent_anim.Pos_Vel.X - Ent->ent_anim.net_OldVel.X;
		delta_vel.Y = Ent->ent_anim.Pos_Vel.Y - Ent->ent_anim.net_OldVel.Y;
		delta_vel.Z = Ent->ent_anim.Pos_Vel.Z - Ent->ent_anim.net_OldVel.Z;

		delta_acc.X = Ent->ent_anim.Pos_Acc.X - Ent->ent_anim.net_OldAcc.X;
		delta_acc.Y = Ent->ent_anim.Pos_Acc.Y - Ent->ent_anim.net_OldAcc.Y;
		delta_acc.Z = Ent->ent_anim.Pos_Acc.Z - Ent->ent_anim.net_OldAcc.Z;

		delta_rot.X = 0;
		delta_rot.Y = Ent->ent_anim.Rot.Y - Ent->ent_anim.net_OldRot.Y;
		delta_rot.Z = 0;


		if(Ent->entity_id == PERSO)
		{
			nbPerso++;
			net_SendDeltaCompressedPos(nbPerso, Ent->entity_id, delta_pos, PTYPE_5, false);
		//	net_SendDeltaCompressedPos(nbPerso, Ent->entity_id, speedvel, PTYPE_6, false);
			net_SendDeltaCompressedPos(nbPerso, Ent->entity_id, delta_rot, PTYPE_7, false);

			if(Ent->net_SendNewAnim)
			{
				Ent->net_SendNewAnim = false;
				if(ProgramState != PS_PAUSEDGAME)
					net_SendModelAnim(Ent, nbPerso);
			}
		}
		else if(Ent->entity_id == MORBAK)
		{
			nbMorbak++;
			net_SendDeltaCompressedPos(nbMorbak, Ent->entity_id, delta_pos, PTYPE_5, false); //
			net_SendDeltaCompressedPos(nbMorbak, Ent->entity_id, delta_vel, PTYPE_6, true);
			net_SendDeltaCompressedPos(nbMorbak, Ent->entity_id, delta_acc, PTYPE_8, true);
		}
		else if(Ent->entity_id == BOMBE)
		{

		}
	}
}

void net_UpdateMessage()
{
	char	buffer[40];
	Net_Mouse	mstype;
	pEntity		Ent;
	static	float	CL_Timing = -1;
	static	float	SV_Timing = -1;

	if(ProgramState == PS_GAME || ProgramState == PS_DIE || ProgramState == PS_PAUSEDGAME)
	{
			if(!gIsServer)
			{
				// gestion du timing CLIENT
				if(CL_Timing == -1)
					CL_Timing = net_GetElapsedTime_msec(0);
				if(net_GetElapsedTime_msec(CL_Timing) >= CL_UPDATE_TIMING)
				{
					CL_Timing = net_GetElapsedTime_msec(0);

					// envoi au serveur les inputs control joueur client
					Ent = o_FindEntity(MORBAK);
					if(!Ent)
						return;

					if(Ent->net_SendNewAnim)
					{
						Ent->net_SendNewAnim = false;
						mstype.CL_new_animation_ID = Ent->Animation_ID;
					}
					else
					{
						mstype.CL_new_animation_ID = -1;
					}

				//	mstype.mousex = Ent->ent_anim.Rot.X;
					mstype.mousey = (short)Ent->ent_anim.Rot.Y;
					mstype.keys = gNetBitKeys;

					memcpy(buffer,&mstype,sizeof(Net_Mouse));
					sv_Send2(buffer, sizeof(Net_Mouse), PacketType_ClientInput, !false);

					// reini le tampon des input client
					gNetBitKeys = 0;
				}
			}
			else
			{
				// SERVER : REINIT DELTA AND RESEND WHOLE FRAME
				if(SV_Timing == -1)
					SV_Timing = net_GetElapsedTime_msec(0);
				if(net_GetElapsedTime_msec(SV_Timing) >= SV_UPDATE_TIMING)
				{
					SV_Timing = net_GetElapsedTime_msec(0);
					net_SVsend_Frame();
				}
				else
				{
					// SERVER : SEND DELTA COMPRESSED FRAME UPDATE
					if(CL_Timing == -1)
						CL_Timing = net_GetElapsedTime_msec(0);
					if(net_GetElapsedTime_msec(CL_Timing) >= CL_UPDATE_TIMING)
					{
						CL_Timing = net_GetElapsedTime_msec(0);
						net_SVsend_DeltaCompressed_Frame();
					}
				}
			}
	}
}

hBool net_HandleMessage()
{
	NSpMessageHeader	*mhp;
	char				str[256];
	NSpPlayerInfoPtr	pip;
	char				cname[kNSpStr32Len];
	NMErr				err;


	// Ask NetSprocket to give us a message if any are waiting
	mhp = NSpMessage_Get( _gameReference );
	if( mhp )
	{
		net_AddDownload((int)mhp->messageLen);

		switch(mhp -> what)
		{
			//generic messages
			case kNSpJoinApproved:
				m_ConsPrint( "Approved!\n", str );
				_response = true;
				_approved = true;	// tell our waiting loop that we were approved
				break;
			
			case kNSpJoinDenied:
				GameP2CStr( ((NSpJoinDeniedMessage *)mhp)->reason, str );
				m_ConsPrint( "Denied!\n   Reason: %s\n", str );
				_response = true;
				_approved = false;	// tell our waiting loop we were denied
				net_StopMultiplayer();
				return true;

			case kNSpError:
				m_ConsPrint( "*** ERROR *** (reported value: %d)\n", ((NSpErrorMessage *)mhp)->error );
				_response = true;
				_approved = false;
				net_StopMultiplayer();
				return true;
			
			case kNSpGameTerminated:
				m_ConsPrint( "--- Host Terminated Game ---\n" );
				_response = true;
				_approved = false;
				net_StopMultiplayer();
				return true;

			case kNSpPlayerJoined:
			{
				NSpGameInfo gameInfo;
				char gamename[256];
				NMErr err;

				err = NSpGame_GetInfo( _gameReference, &gameInfo );
				if( !err )
					GameP2CStr( gameInfo.name, gamename );
				else
					strcpy( gamename, "ERROR" );

				gConnectedPlayer = (short)((NSpPlayerJoinedMessage *)mhp)->playerCount;

				GameP2CStr( ((NSpPlayerJoinedMessage *)mhp)->playerInfo.name, str );
				m_ConsPrint( "%s has joined game '%s', %d players now!\n",str, gamename, gConnectedPlayer );

				if(gIsServer)
				{
					game_clock = MULTI_TIMELIMIT;
				}
			}
			break;

			case kNSpPlayerLeft:
			{
				char	mess[255];

				gConnectedPlayer = (short)((NSpPlayerJoinedMessage *)mhp)->playerCount;

				GameP2CStr( ((NSpPlayerLeftMessage *)mhp)->playerName, str );
				m_ConsPrint( "===> Player, %s, Left game, leaving %d players!\n",str, gConnectedPlayer );

				// supprime l'entitee du client qui vient de quitter le jeu
				sprintf(mess,"%s has disconnected..", ((NSpPlayerLeftMessage *)mhp)->playerName);
				game_catchedinit2(mess);

				gMultiCLleftgame = true;
				if(gIsServer && gConnectedPlayer < 2)
				{
					game_clock = MULTI_TIMELIMIT;
				}
			}
			break;


// --- Handle our game specific messages 

			case PacketType_SVframe:
				if(!gIsServer)
				{
					char	buffer[40];

					strcpy(buffer, ((MessagePacketPtr)mhp)->str);
					sscanf(buffer, "%d", &gCL_FrameID);
				}

			break;

			case PacketType_CLframe:

			break;

			case PacketType_AskProtocol:
				if(gIsServer)
				{
					char	idprotocol[4];
					char	mess[255];

					m_ConsPrint("Le serveur recoit une requete de la version du protocol\n");
					m_ConsPrint("Le serveur envoi la version du protocol au client\n");

					sprintf(idprotocol, "%d", NET_PROTOCOL_ID);
					sv_Send(idprotocol, PacketType_GiveProtocol, true);

					// on indique au serveur qu'un joueur se connecte
					err = NSpPlayer_GetInfo( _gameReference, mhp->from, &pip );
					if( !err )
					{		
						GameP2CStr( pip->name, cname );
						NSpPlayer_ReleaseInfo( _gameReference, pip );

						sprintf(mess,"%s is connecting..", cname);
						game_catchedinit2(mess);
					}					
				}
			break;

			case PacketType_GiveProtocol:
				if(!gIsServer)
				{
					char	sv_protocol[4];
					int		idprotocol;

					gClientAskProtocol = false;

					strcpy(sv_protocol, ((MessagePacketPtr)mhp)->str);
					sscanf(sv_protocol, "%d", &idprotocol);

					m_ConsPrint("Le client recoit la version du protocol du serveur : %d\n",idprotocol);
					if(NET_PROTOCOL_ID == idprotocol)
					{
						m_ConsPrint("Les protocols sont concordants!\n");
						m_ConsPrint("Le client envoi une demande de l'id de la map en cours au serveur\n");
						cl_Send("rofl", PacketType_AskMapID, true, true);
					}
					else
					{
						if(!IsConsoleActive)
							IsConsoleActive = true;
						m_ConsPrint("Server uses protocol : %d !!\n", sv_protocol);
						net_StopMultiplayer();
						return true;
					}
				}
			break;

			case PacketType_AskMapID:
				if(gIsServer)
				{
					char	idmap[2];

				//	if(developer.value)
					{
						m_ConsPrint("Le serveur recoit une requete de l'id de la map en cours\n");
						m_ConsPrint("Le serveur envoi l'id de la map en cours au client\n");
					}
					
					sprintf(idmap, "%d", gMultiMapID);
					sv_Send(idmap, PacketType_GiveMapID, true);
				}
			break;

			case PacketType_GiveMapID:
				if(!gIsServer)
				{
					char	sv_mapid[2];
					int		idmap;

				//	if(developer.value)
						m_ConsPrint("Le client recoit l'id de la map en cours du serveur\n");

					strcpy(sv_mapid, ((MessagePacketPtr)mhp)->str);
					sscanf(sv_mapid, "%d", &idmap);

					gMultiMapPlayed = idmap;
				}
			break;

			case PacketType_AskEntities:
				if(gIsServer)
				{
					int			nbents;
					pEntity		Ent;
					Net_ARent	NetARent;
					char		buffer[32];

				//	if(developer.value)
						m_ConsPrint("Le serveur recoit une requete des entitees dans la map en cours\n");

				//	if(developer.value)
						m_ConsPrint("Le serveur envoi le nombre d'entitees dans la map en cours\n");

					nbents = 0;
					for(Ent=gEntitiesList ; Ent ; Ent=Ent->Next)
					if(Ent->entity_id == PERSO || Ent->entity_id == DEPLA)
					{
						nbents++;
					}
					sprintf(buffer, "%d", nbents);
					sv_Send(buffer, PacketType_GiveNbEnts, true);
				
				//	if(developer.value)
						m_ConsPrint("Le serveur envoi les entitees dans la map en cours au client\n");

					for(Ent=gEntitiesList ; Ent ; Ent=Ent->Next)
					if(Ent->entity_id == PERSO || Ent->entity_id == DEPLA)
					{
						NetARent.Action = true;
						NetARent.Ent_ID = PERSO;
						NetARent.Pos = Ent->ent_anim.Pos;
						memcpy(buffer, &NetARent, sizeof(Net_ARent));
						sv_Send2(buffer, sizeof(Net_ARent), PacketType_ARent, true);
					}
					m_ConsPrint("Le serveur a fini d'envoyer les entitees dans la map en cours au client\n");
					// envoie le timer
					//net_SendServerTime();
				}
			break;

			case PacketType_GiveNbEnts:
				if(!gIsServer)
				{
					char	buffer[4];
					int		maxents;

					strncpy(buffer, ((MessagePacketPtr)mhp)->str, 4);
					sscanf(buffer, "%d", &maxents);
					gMultiNbEntsMax = (short)maxents;

				//	if(developer.value)
						m_ConsPrint("Le client a recu le nombre d'entitees du serveur : %d\n",maxents);
				}
			break;

			case PacketType_GiveSpawnPos:
				if(!gIsServer)
				{
			//		Net_Ent	NetEnt;
			//		char	buffer[40];

					m_ConsPrint("Le client recoi le point d'apparition dans le jeu\n");

					gMultiClientReady = true;
					
			//		NetEnt.pos = gStart;
			//		memcpy(buffer, &NetEnt, sizeof(NetEnt));

				}
			break;

			case PacketType_AskSpawnPos:
				if(gIsServer)
				{
					int			index;
					Net_Ent		NetEnt;
					Net_ARent	NetARent;
					char		buffer[40];
					ANIMENTITY	anim;

					m_ConsPrint("Le serveur recoi une demande d'apparition du client dans le jeu\n");
					m_ConsPrint("Le serveur envoi un point apparition dans le jeu au client\n");
					
					// d'office on pose un MORBAK sans tenir compte du choix du model du client
					gMultiSV_clentid = MORBAK;
					gMultiSV_clentn = gConnectedPlayer-1;
					index = rand() % gNb_Respawn;
					gMultiSV_clentstartpos = RespawnTab[index].entity_pos; //gStart;					

					
					// ajoute le joueur client dans la map du serveur
					o_ClearAnimFields(&anim);
					anim.Pos = gMultiSV_clentstartpos;
					o_AddEntityToList(gMultiSV_clentid, -1, anim, false);

					NetARent.Action = true;
					NetARent.Ent_ID = gMultiSV_clentid;
					NetARent.Pos = anim.Pos;
					memcpy(buffer, &NetARent, sizeof(Net_ARent));
					sv_Send2(buffer, sizeof(Net_ARent), PacketType_ARent, true);

					NetEnt.pos[0] = anim.Pos.X;
					NetEnt.pos[1] = anim.Pos.Y;
					NetEnt.pos[2] = anim.Pos.Z;

					memcpy(buffer, &NetEnt, sizeof(NetEnt));
					sv_Send2(buffer, sizeof(NetEnt), PacketType_GiveSpawnPos, true);

					// envoi la syncro du timer par la meme occasion au client
					//net_SendServerTime();
				}
			break;

			case PacketType_ARent:
				if(!gIsServer)
				{
					Net_ARent	NetARent;
					ANIMENTITY	anim;
					pEntity		Ent;

					memcpy(&NetARent, ((MessagePacketPtr)mhp)->str, sizeof(Net_ARent));
					if(NetARent.Action)
					{
						o_ClearAnimFields(&anim);
						anim.Pos = NetARent.Pos;
						o_AddEntityToList(NetARent.Ent_ID, -1, anim, false);

						// HACK : on veut voir un joueur server DEPLA et non ROBOT en FER :)
						if(NetARent.Ent_ID == PERSO)
						{
							o_AddEntityToList(DEPLA, -1, anim, false);
						}

						gMultiNbEnts++;
					//	if(developer.value)
							m_ConsPrint("Net_entity added\n");
					}
					else
					{
						//if(developer.value)
						//	m_ConsPrint("Le client supprime une entitee du serveur\n");

						Ent = o_FindEntity2(NetARent.Ent_ID, NetARent.Ent_Pos);
						if(Ent)
						{
							o_RemoveEntityFromList(Ent);
							gMultiNbEnts--;
						//	if(developer.value)
								m_ConsPrint("Net_entity removed\n");
						}
						else
							m_ConsPrint("Error : Can't remove a ghost entity\n");
					}
				}
			break;

			case PacketType_ServerSay:
				if(!gIsServer)
				{
					char	buffer[255];

					err = NSpPlayer_GetInfo( _gameReference, mhp->from, &pip );
					if( !err )
					{		
						GameP2CStr( pip->name, cname );
						NSpPlayer_ReleaseInfo( _gameReference, pip );

						sprintf(buffer, "%s : %s\n",cname, ((MessagePacketPtr)mhp)->str);
						m_ConsPrint(buffer);
						game_catchedinit2(buffer);
					}
					else
					{
						m_ConsPrint("UNKOWN2 -- error\n");
						break;
					}
				}
			break;

			case PacketType_ClientSay:
				if(gIsServer)
				{
					char	buffer[255];
					//char	mess[255];

					//strcpy(mess, ((MessagePacketPtr)mhp)->str);

					err = NSpPlayer_GetInfo( _gameReference, mhp->from, &pip );
					if( !err )
					{		
						GameP2CStr( pip->name, cname );
						NSpPlayer_ReleaseInfo( _gameReference, pip );

						sprintf(buffer, "%s : %s\n",cname, ((MessagePacketPtr)mhp)->str);
						m_ConsPrint(buffer);
						game_catchedinit2(buffer);
					}
					else
					{
						m_ConsPrint("UNKOWN3 -- error\n");
						break;
					}
				}
			break;

			case PacketType_UpdateEnt_1val:
				if(!gIsServer && (ProgramState == PS_GAME || ProgramState == PS_DIE || ProgramState == PS_PAUSEDGAME) )
				{
					pEntity			Ent;
					Net_Ent_1val	NetEnt1;
					vect_t			dc_vec;

					memcpy(&NetEnt1, ((MessagePacketPtr)mhp)->str, sizeof(Net_Ent_1val));

					Ent = o_FindEntity2((int)NetEnt1.Ent_ID, (int)NetEnt1.Ent_Pos);
					if(!Ent)
					{
						net_StopMultiplayer();
						m_ConsPrint("Update Entity not found in the game, exiting client\n");
						return true;
					}

					dc_vec.X = NetEnt1.val;
					dc_vec.Y = 0;
					dc_vec.Z = 0;
					net_UpdateEntity_DeltaCompressed(Ent, NetEnt1.type, dc_vec, 1);
				}
			break;

			case PacketType_UpdateEnt_2val:
				if(!gIsServer && (ProgramState == PS_GAME || ProgramState == PS_DIE || ProgramState == PS_PAUSEDGAME) )
				{
					pEntity	Ent;
					Net_Ent_2val	NetEnt2;
					vect_t			dc_vec;

					memcpy(&NetEnt2, ((MessagePacketPtr)mhp)->str, sizeof(Net_Ent_2val));

					Ent = o_FindEntity2((int)NetEnt2.Ent_ID, (int)NetEnt2.Ent_Pos);
					if(!Ent)
					{
						net_StopMultiplayer();
						m_ConsPrint("Update Entity not found in the game, exiting client\n");
						return true;
					}

					dc_vec.X = NetEnt2.val[0];
					dc_vec.Y = NetEnt2.val[1];
					dc_vec.Z = 0;
					net_UpdateEntity_DeltaCompressed(Ent, NetEnt2.type, dc_vec, 2);
				}
			break;

			case PacketType_UpdateEnt_3val:
				if(!gIsServer && (ProgramState == PS_GAME || ProgramState == PS_DIE || ProgramState == PS_PAUSEDGAME) )
				{
					pEntity	Ent;
					Net_Ent_3val	NetEnt3;
					vect_t			dc_vec;

					memcpy(&NetEnt3, ((MessagePacketPtr)mhp)->str, sizeof(Net_Ent_3val));

					Ent = o_FindEntity2((int)NetEnt3.Ent_ID, (int)NetEnt3.Ent_Pos);
					if(!Ent)
					{
						net_StopMultiplayer();
						m_ConsPrint("Update Entity not found in the game, exiting client\n");
						return true;
					}

					dc_vec.X = NetEnt3.val[0];
					dc_vec.Y = NetEnt3.val[1];
					dc_vec.Z = NetEnt3.val[2];
					net_UpdateEntity_DeltaCompressed(Ent, NetEnt3.type, dc_vec, 3);
				}
			break;


			case PacketType_UpdateEnt:
				if(!gIsServer && (ProgramState == PS_GAME || ProgramState == PS_DIE || ProgramState == PS_PAUSEDGAME) )
				{
					pEntity	Ent;
					Net_Ent	NetEnt;

					memcpy(&NetEnt, ((MessagePacketPtr)mhp)->str, sizeof(Net_Ent));

					Ent = o_FindEntity2((int)NetEnt.Ent_ID, (int)NetEnt.Ent_Pos);
					if(!Ent)
					{
						net_StopMultiplayer();
						m_ConsPrint("Update Entity not found in the game, exiting client\n");
						return true;
					}

					Ent->ent_anim.Pos.X = NetEnt.pos[0];
					Ent->ent_anim.Pos.Y = NetEnt.pos[1];
					Ent->ent_anim.Pos.Z = NetEnt.pos[2];

					Ent->ent_anim.net_OldPos = Ent->ent_anim.Pos;



					Ent->ent_anim.Pos_Vel.X = NetEnt.vel[0];
					Ent->ent_anim.Pos_Vel.Y = NetEnt.vel[1];
					Ent->ent_anim.Pos_Vel.Z = NetEnt.vel[2];

					Ent->ent_anim.net_OldVel = Ent->ent_anim.Pos_Vel;



					Ent->ent_anim.Pos_Acc.X = NetEnt.acc[0];
					Ent->ent_anim.Pos_Acc.Y = NetEnt.acc[1];
					Ent->ent_anim.Pos_Acc.Z = NetEnt.acc[2];

					Ent->ent_anim.net_OldAcc = Ent->ent_anim.Pos_Acc;



					// HACK : le client ne met pas a jour sa rotation de perso d'apres les infos du serveur
					if(Ent->entity_id == MORBAK)
					{
					}
					else if(Ent->entity_id == PERSO)
					{
						Ent->ent_anim.Rot.Y = net_Short2Float(NetEnt.rot[1], SHORT2FLOATPREC0);
						Ent->ent_anim.net_OldRot = Ent->ent_anim.Rot;
					}
					else if(Ent->entity_id == BOMBE)
					{
					}
					else
					{
						Ent->ent_anim.Rot.X = cam_RotationCycle(net_Short2Float(NetEnt.rot[0], SHORT2FLOATPREC0));
						Ent->ent_anim.Rot.Y = cam_RotationCycle(net_Short2Float(NetEnt.rot[1], SHORT2FLOATPREC0));
						Ent->ent_anim.Rot.Z = cam_RotationCycle(net_Short2Float(NetEnt.rot[2], SHORT2FLOATPREC0));

						Ent->ent_anim.net_OldRot = Ent->ent_anim.Rot;
					}
				}
			break;

			case PacketType_ClientHurted:
				{
					Net_Ent_1val	NetEnt_1;
					pEntity			ClientEnt;

					if(!gIsServer)
					{
						ClientEnt = o_FindEntity(MORBAK);
						if(ClientEnt)
						{
							memcpy(&NetEnt_1,((MessagePacketPtr)mhp)->str,sizeof(Net_Ent_1val));
							PlayerHurt(ClientEnt, ClientEnt->health - (int)NetEnt_1.val);
							ClientEnt->health = (int)NetEnt_1.val;

							if(ProgramState != PS_DIE && ProgramState != PS_WIN)
								fx_ColorScreen(100,0.5f,1,0.5f,0.5f,false);
						}
					}
				}
			break;

			case PacketType_ServerInput:
				{
					Net_Svinput	svintype;

					memcpy(&svintype,((MessagePacketPtr)mhp)->str,sizeof(Net_Svinput));
					gNetBitKeys_back = svintype.keys[0];
				}
			break;

			case PacketType_ClientInput:
				if(gIsServer && (ProgramState == PS_GAME || ProgramState == PS_DIE || ProgramState == PS_PAUSEDGAME))
				{
					Net_Mouse	mstype;
	//				Net_Svinput	svintype;
					pEntity		ClientEnt;
	///				char		buffer[40];

					memcpy(&mstype,((MessagePacketPtr)mhp)->str,sizeof(Net_Mouse));
					//m_ConsPrint("%f %f\n",mstype.mousex,mstype.mousey);

					gNetBitKeys = mstype.keys;

					// renvoi au client l'acceptation des input du client
				//	svintype.keys[0] = gNetBitKeys;
				//	memcpy(buffer, &svintype, sizeof(svintype));
				//	sv_Send2(buffer, sizeof(svintype), PacketType_ServerInput, true);

					if(gMultiSV_clentid != -1 && gMultiSV_clentn != -1)
					{
						ClientEnt = o_FindEntity2(gMultiSV_clentid, gMultiSV_clentn);
						if(ClientEnt)
						{
					//		ClientEnt->ent_anim.Rot.X = mstype.mousex;
							ClientEnt->ent_anim.Rot.Y = (float)mstype.mousey;

							if(mstype.CL_new_animation_ID != -1)
							{
								ClientEnt->IsChanged_Animation = true;
								ClientEnt->Animation_ID = (int)mstype.CL_new_animation_ID;
							}
						}
					}
				}
			break;

			case PacketType_GetAnimID:
				{
					Net_AnimID	NetAnimID;
					pEntity		Ent;

					memcpy(&NetAnimID,((MessagePacketPtr)mhp)->str,sizeof(Net_AnimID));

					if(!gIsServer)
					{
						Ent = o_FindEntity2((int)NetAnimID.Ent_ID, (int)NetAnimID.Ent_Pos);
						if(Ent)
						{
							Ent->IsChanged_Animation = true;
							Ent->Animation_ID = NetAnimID.new_animation_ID;
						}
					}
				}
			break;

			case PacketType_SvLaunchEnt:
				if(!gIsServer && (ProgramState == PS_GAME || ProgramState == PS_DIE || ProgramState == PS_PAUSEDGAME))
				{
					Net_Ent		NetEnt;
					ANIMENTITY	anim;

					memcpy(&NetEnt,((MessagePacketPtr)mhp)->str,sizeof(Net_Ent));

					o_ClearAnimFields(&anim);
					anim.Pos.X = NetEnt.pos[0];
					anim.Pos.Y = NetEnt.pos[1];
					anim.Pos.Z = NetEnt.pos[2];
					anim.Rot.X = net_Short2Float(NetEnt.rot[0], SHORT2FLOATPREC);
					anim.Rot.Y = net_Short2Float(NetEnt.rot[1], SHORT2FLOATPREC);
					anim.Rot.Z = net_Short2Float(NetEnt.rot[2], SHORT2FLOATPREC);
					anim.Pos_Vel.X = net_Short2Float(NetEnt.vel[0], SHORT2FLOATPREC);
					anim.Pos_Vel.Y = net_Short2Float(NetEnt.vel[1], SHORT2FLOATPREC);
					anim.Pos_Vel.Z = net_Short2Float(NetEnt.vel[2], SHORT2FLOATPREC);

					if(NetEnt.Ent_ID == EXPLOSION)
					{
						anim.Siz_Vel = anim.Pos_Vel;
						anim.Pos_Vel.X = anim.Pos_Vel.Y = anim.Pos_Vel.Z = 0;
					}

					o_AddEntityToList((int)NetEnt.Ent_ID, (int)NetEnt.Ent_parentid, anim, false);
				}
			break;

			case PacketType_ClLaunchEnt:
				if(gIsServer && (ProgramState == PS_GAME || ProgramState == PS_DIE || ProgramState == PS_PAUSEDGAME))
				{
					Net_Ent		NetEnt;
					ANIMENTITY	anim;

					memcpy(&NetEnt,((MessagePacketPtr)mhp)->str,sizeof(Net_Ent));

					o_ClearAnimFields(&anim);
					anim.Pos.X = NetEnt.pos[0];
					anim.Pos.Y = NetEnt.pos[1];
					anim.Pos.Z = NetEnt.pos[2];
					anim.Rot.X = net_Short2Float(NetEnt.rot[0], SHORT2FLOATPREC);
					anim.Rot.Y = net_Short2Float(NetEnt.rot[1], SHORT2FLOATPREC);
					anim.Rot.Z = net_Short2Float(NetEnt.rot[2], SHORT2FLOATPREC);
					anim.Pos_Vel.X = net_Short2Float(NetEnt.vel[0], SHORT2FLOATPREC);
					anim.Pos_Vel.Y = net_Short2Float(NetEnt.vel[1], SHORT2FLOATPREC);
					anim.Pos_Vel.Z = net_Short2Float(NetEnt.vel[2], SHORT2FLOATPREC);
					o_AddEntityToList((int)NetEnt.Ent_ID, (int)NetEnt.Ent_parentid, anim, false);

					net_NetLaunchEnt((int)NetEnt.Ent_ID, (int)NetEnt.Ent_parentid, anim.Pos, anim.Rot, anim.Pos_Vel, true);
				}
			break;

			case PacketType_ClVectorState:
				if(!gIsServer)
				{
					Net_VSent	NetVSent;
				//	ANIMENTITY	anim;
					pEntity		Ent;

					memcpy(&NetVSent, ((MessagePacketPtr)mhp)->str, sizeof(Net_VSent));

					Ent = o_FindEntity2((int)NetVSent.Ent_ID, (int)NetVSent.Ent_Pos);
					if(Ent)
					{
						if(NetVSent.vtype == 0)
						{
							Ent->ent_anim.Pos_Acc.X = 0;
							Ent->ent_anim.Pos_Acc.Y = 0;
							Ent->ent_anim.Pos_Acc.Z = 0;
						}
						else if(NetVSent.vtype == 1)
						{
							Ent->ent_anim.Pos_Acc.X = NetVSent.vec[0];
							Ent->ent_anim.Pos_Acc.Y = NetVSent.vec[1];
							Ent->ent_anim.Pos_Acc.Z = NetVSent.vec[2];
						}
					}
				}
			break;

			case PacketType_CLScore:
				if(!gIsServer)
				{
					Net_Score	NetScore;

					memcpy(&NetScore, ((MessagePacketPtr)mhp)->str, sizeof(Net_Score));
					gSV_score = NetScore.SV_score;
					gCL_score = NetScore.CL_score;
				}
			break;

			case PacketType_CLTime:
				if(!gIsServer)
				{
					Net_Time	NetTime;

					memcpy(&NetTime, ((MessagePacketPtr)mhp)->str, sizeof(Net_Time));
					game_clock = NetTime.timer;
					game_TimeLimit = true;
				}
			break;

			default:
				break;
		}

		NSpMessage_Release( _gameReference, mhp );
		return true;
	}
	return false;
}

float lago_time = 0;
float UL_debit = 0;
float UL_debit_tmp = 0;
float DL_debit = 0;
float DL_debit_tmp = 0;

void net_AddUpload(int nbupload)
{
	UL_debit_tmp += nbupload;
}

void net_AddDownload(int nbdownload)
{
	DL_debit_tmp += nbdownload;
}

void net_LagometerProcess()
{
	if(developer.value)
	{
		g_DispString(14, "Upload : %f Ko/Sec", UL_debit);
		g_DispString(14, "Download : %f Ko/Sec", DL_debit);
	}

	lago_time += gFrameTime_msec;
	if(lago_time >= 1000.0f)
	{
		lago_time = 0;

		DL_debit = DL_debit_tmp*0.001f;
		UL_debit = UL_debit_tmp*0.001f;
	//	DL_debit = (int)(DL_debit_tmp*0.01f)*0.1f;
	//	UL_debit = (int)(UL_debit_tmp*0.01f)*0.1f;

		DL_debit_tmp = 0;
		UL_debit_tmp = 0;
	}
}

char *GameP2CStr(unsigned char *s, char *d)
{
	if( s && d )
	{
		memcpy( d, s+1, *s );	/* pretty simple */
		d[*s] = 0;
	}
	return( d );
}

unsigned char *GameC2PStr(char *s, unsigned char *d)
{
	int c = 0;

	if( s && d )	/* make sure we got got inputs */
	{
		while( *s )
			d[++c] = (unsigned char)*s++;	/* no blockmove, copy & get length in one shot */
	}
	*d = (unsigned char)c;

	return( d );
}

