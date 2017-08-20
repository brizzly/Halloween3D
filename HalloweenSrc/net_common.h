#ifndef NETCOMMON_INCLUDED
#define NETCOMMON_INCLUDED

#include "alloween.h"


#ifdef H_WINDOWS

//#include <OpenPlay.h>

#endif


#ifdef H_LINUX

//#include <Open...
typedef NSpMessageHeader int;

#endif



#define	NET_PROTOCOL_ID			1
#define NET_MAXPLAYER			8
#define kPlayerType				1234		// just defines a way to allow/reject players of different types 
#define kMaxMessageSize			256			// the maximum datagram size we can send or retrieve 
#define kOurGameType			'Hallow'	// identifier for our games on the network

#define	CL_UPDATE_TIMING		50.0f
#define	SV_UPDATE_TIMING		500.0f

#define	SHORT2FLOATPREC0		1.0f		// le float doit etre compris entre 0 et 32000
#define	SHORT2FLOATPREC1		10.0f		// le float doit etre compris entre 0 et 3200
#define	SHORT2FLOATPREC			100.0f		// le float doit etre compris entre 0 et 320
#define	SHORT2FLOATPREC2		1000.0f		// le float doit etre compris entre 0 et 32
#define	SHORT2FLOATPREC3		10000.0f	// le float doit etre compris entre 0 et 3
#define	SHORT2FLOATPREC4		100000.0f	// le float doit etre compris entre 0 et 0.3


#define	PTYPE_1					0x01	// premiere valeure est un X
#define	PTYPE_2					0x02	// premiere valeure est un Y
#define	PTYPE_3					0x04	//		(deuxieme valeure est un X)
#define	PTYPE_4					0x08	// deuxieme valeure est un Y
#define	PTYPE_5					0x10	// c'est une update de POSITION
#define	PTYPE_6					0x20	// c'est une update de VELOCITE
#define	PTYPE_7					0x40	// c'est une update de ROTATION
#define	PTYPE_8					0x80	// c'est une update de ACCELERATION


typedef struct
{
	NSpMessageHeader	header;
	char				str[1];

} MessagePacket_t, *MessagePacketPtr;


enum
{
	// types des messages pour l'initialisation de la partie en reseau
	//
	PacketType_AskProtocol = 256,		// client demande la version de protocol du serveur
	PacketType_GiveProtocol,			// serveur donne la version du protocol utilisé
	PacketType_AskGameType,				// client demande le type de jeu joué sur le serveur
	PacketType_GiveGameType,			// serveur donne le type de jeu joué
	PacketType_AskMapID,				// client demande l'id de la map chargé sur le serveur
	PacketType_GiveMapID,				// serveur donne l'id de la map joué
	PacketType_AskEntities,				// client demande la liste des entitées du serveur
	PacketType_GiveNbEnts,				// serveur donne le nombre d'entitées
	PacketType_AskSpawnPos,				// client demande un point d'apparition dans la map
	PacketType_GiveSpawnPos,			// server envoi un point d'apparition dans la map
	
	// types des messages pour la synchronisation de la partie en reseau
	//
	PacketType_SVframe,					// server avertit de l'id de la frame qu'il envoi au client
	PacketType_CLframe,					// client avertit la frame reçu au server
	PacketType_ARent,					// client recoi un ordre d'ajout/suppression d'une entitée
	PacketType_UpdateEnt,				// client recoi une mise a jour d'une entitée
	PacketType_UpdateEnt_1val,
	PacketType_UpdateEnt_2val,
	PacketType_UpdateEnt_3val,
	PacketType_ClientHurted,
	PacketType_GetAnimID,
	PacketType_ServerSay,				// le serveur parle aux joueurs
	PacketType_ClientSay,				// un client parle aux joueurs
	PacketType_ClientInput,				// un client envoi ses inputs de control de son joueur au server
	PacketType_ServerInput,
	PacketType_SvLaunchEnt,				// le joueur serveur lance une entitee avec son arme
	PacketType_ClLaunchEnt,				// le joueur client lance une entitee avec son arme
	PacketType_ClVectorState,			// le joueur client reçoit un changement brut d'un vecteur
	PacketType_CLScore,					// le client reçoit les scores
	PacketType_CLTime					// le client reçoit le temps restant

};




//==== Add/Remove entity system ====
typedef struct arentag
{
	unsigned short	Ent_ID;
	unsigned short	Ent_Pos;
	vect_t			Pos;
	hBool	Action;				// true = add, false = remove entity

} Net_ARent;

//==== a vector has changed ====
typedef struct vstatetag
{
	unsigned short	Ent_ID;
	unsigned short	Ent_Pos;
	byte			vtype;
	float			vec[3];

} Net_VSent;

//==== Input/Mouse system ====
typedef struct mstag
{
	char	CL_new_animation_ID;
//	short	mousex;
	short	mousey;
	char	keys;

} Net_Mouse;

typedef struct svinputtag2
{
	char	keys[2];

} Net_Svinput;

//==== Update Entities system ====

typedef struct animidtag
{
	byte	Ent_ID;
	byte	Ent_Pos;
	byte	new_animation_ID;

} Net_AnimID;

typedef struct enttag
{
	byte	Ent_ID;
	byte	Ent_Pos;
	byte	Ent_parentid;

	float	pos[3];
	float	vel[3];
	float	acc[3];

	short	rot[3];

} Net_Ent;

typedef struct tag_1val
{
	byte			Ent_ID;
	byte			Ent_Pos;
	byte			type;
//	short			val;
	float			val;

} Net_Ent_1val;

typedef struct tag_2val
{
	byte			Ent_ID;
	byte			Ent_Pos;
	byte			type;
//	short			val[2];
	float			val[2];

} Net_Ent_2val;

typedef struct tag_3val
{
	byte			Ent_ID;
	byte			Ent_Pos;
	byte			type;
//	short			val[3];
	float			val[3];

} Net_Ent_3val;

//==== Score system ====
typedef struct sctag
{
	short	SV_score;
	short	CL_score;

} Net_Score;

//==== Time system ====
typedef struct tmtag
{
	short	timer;

} Net_Time;


void	net_Init();
void	net_UnInit();
void	net_StopMultiplayer();
float	net_GetElapsedTime_msec(float LastTime_msec);
void	net_SendVectorState(pEntity Ent, int vstate);
void	net_DispPodium();
void	net_DispNetInfos();
void	net_SendServerTime();
void	net_SendServerScores();
void	net_RemoveClientEnt(pEntity Ent);
void	net_NetLaunchEnt(int entid, int launcherid, vect_t startpos, vect_t rot, vect_t vel, short IsServer);
void	net_SendModelAnim(pEntity Ent, int pos);
void	net_SendClientHurt(int health);
hBool	net_HandleMessage();
void	net_SVsend_Frame();
void	net_SVsend_DeltaCompressed_Frame();
void	net_UpdateMessage();
void	net_LagometerProcess();
void	net_AddUpload(int nbupload);
void	net_AddDownload(int nbdownload);
char *	GameP2CStr(unsigned char *s, char *d);
unsigned char *GameC2PStr(char *s, unsigned char *d);



#endif // NETCOMMON_INCLUDED

