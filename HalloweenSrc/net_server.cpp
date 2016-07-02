
#include "alloween.cpp"
#include "net_server.h"


#include <OpenPlay.h>


static NSpProtocolListReference	_protocolListRef = NULL;
hBool	gNetworkInitialised = false;


extern NSpGameReference		_gameReference;


void sv_Create()
{
   	NMErr err;
	unsigned char gameNameStr[32], playerNameStr[32];
	char gameName[32], playerName[32];
	NMUInt32 port;
	int players;
	NSpProtocolReference protocolRef;
	unsigned char Pass[32];

	err = NSpInitialize( 0, 0, 0, 'NSe1', 0 );
	if( err )
	{
		m_ConsPrint("Network Initialization failed\n");
		return;
	}

	port = (NMUInt32)(int)net_port.value;				// 1000->32767
	players = (NMUInt32)NET_MAXPLAYER;					// 1->32
	sprintf(gameName,"%s",net_svname.string);
	sprintf(playerName,"%s",net_clname.string);
	
	GameC2PStr( gameName, gameNameStr );
	GameC2PStr( playerName, playerNameStr );
	GameC2PStr( net_password.string, Pass );

	err = NSpProtocolList_New( NULL, &_protocolListRef );
	if( err )
	{
		m_ConsPrint("Network Protocol Initialization failed\n");
		return;
	}
	
	protocolRef = NSpProtocol_CreateIP( (NMUInt16)port, 0, 0 );
	if(!protocolRef)
	{
		m_ConsPrint("Network kNSpInvalidProtocolRefErr error\n");
		return;
	}

	err = NSpProtocolList_Append( _protocolListRef, protocolRef );
	if(err)
	{
		m_ConsPrint("Network NSpProtocolList_Append error\n");
		return;
	}
	
	err = NSpGame_Host( &_gameReference, _protocolListRef, players, gameNameStr,
									Pass, playerNameStr, kPlayerType, kNSpClientServer, 0 );
	if(err)
	{
		m_ConsPrint("Network NSpGame_Host error\n");
		return;
	}

	gNetworkInitialised = true;
	m_ConsPrint("Network Initialization ok\n");
}

void sv_Destroy()
{
	NMUInt32 refCount;
	NMErr err = kNMNoError;
	NSpProtocolReference pRef;

	if(!gNetworkInitialised)
		return;
	m_ConsPrint("Network UnInitialization\n");

	if( _gameReference )
	{
		err = NSpGame_Dispose( _gameReference, kNSpGameFlag_ForceTerminateGame );
	}

	if( _protocolListRef )
	{
		refCount = NSpProtocolList_GetCount( _protocolListRef );	/* get number of protocols */

		while( refCount-- && !err )
		{
			pRef = NSpProtocolList_GetIndexedRef( _protocolListRef, refCount );	/* get currect reference */
			err = NSpProtocolList_RemoveIndexed( _protocolListRef, refCount );	/* then remove it from the list */

			/* now, we can dispose of the reference safely */
			NSpProtocol_Dispose( pRef );	/* this should have an NMErr return, but has a void return... */
		}

		/* once all of the protocols are disposed, we can dispose of the containing reference list */
		NSpProtocolList_Dispose( _protocolListRef );
	}
	gNetworkInitialised = false;
}

void sv_BeginHost(char *name)
{
}

void sv_StopHost()
{
}

void sv_Send(char *message, int id_packet, hBool SafeSend)
{
	MessagePacketPtr	qpp;
	unsigned long		messageLen, size;
	NMErr				err;
	NSpFlags			Flag;

	if( !message )
		return; //( kNSpInvalidParameterErr );

	// get size of message string & total size of network packet
	messageLen = strlen( message );
	size = sizeof(MessagePacket_t) + messageLen + 1;	/* header + num_chars + terminator */

	// allocate the memory for the packet
	qpp = (MessagePacketPtr)malloc( size );
	if( !qpp )
	{
		m_ConsPrint(" *** ERROR *** Unable to allocate message buffer!\n");
		fflush(stdout);
		return; //( kNSpMemAllocationErr );
	}

	// init the NetSprocket portion of the packet
	NSpClearMessageHeader( &qpp->header );

	qpp->header.what = (NMSInt32)id_packet;
	qpp->header.to = kNSpAllPlayers;
	qpp->header.messageLen = size;

	// fill in the data section
	strcpy( qpp->str, message );

	// tell NetSprocket to send the message
	if(SafeSend)
		Flag = kNSpSendFlag_Registered;
	else
		Flag = /*kNSpSendFlag_Junk; //*/kNSpSendFlag_Normal;

	err = NSpMessage_Send( _gameReference, &qpp->header, Flag );
	if( !err )
	{
		net_AddUpload(size);
//		qpp->header.to = kNSpHostOnly;		// allow host to play as well!
//		err = NSpMessage_Send( _gameReference, &qpp->header, kNSpSendFlag_Registered );
	}

	if( err )
	{
		m_ConsPrint("*** ERROR *** Unable to send message packet, error # %d\n", err );
		fflush(stdout);
	}

	// clean up after ourselves
	s_free( qpp );
}

void sv_Send2(char *message, int messageLen, int id_packet, hBool SafeSend)
{
	MessagePacketPtr	qpp;
	unsigned long		size;
	NMErr				err;
	NSpFlags			Flag;

	if(!message)
		return; //( kNSpInvalidParameterErr );

	size = sizeof(MessagePacket_t) + messageLen + 1;
	qpp = (MessagePacketPtr)malloc( size );
	if( !qpp )
	{
		m_ConsPrint(" *** ERROR *** Unable to allocate message buffer!\n");
		fflush(stdout);
		return; //( kNSpMemAllocationErr );
	}

	NSpClearMessageHeader( &qpp->header );
	qpp->header.what = (NMSInt32)id_packet;
	qpp->header.to = kNSpAllPlayers;
	qpp->header.messageLen = size;
	memcpy( qpp->str, message, messageLen);

	if(SafeSend)
		Flag = kNSpSendFlag_Registered;
	else
		Flag = /*kNSpSendFlag_Junk; //*/kNSpSendFlag_Normal;

	err = NSpMessage_Send( _gameReference, &qpp->header, Flag );
	if( err )
	{
		m_ConsPrint("*** ERROR *** Unable to send message packet, error # %d\n", err );
		fflush(stdout);
	}
	else
	{
		net_AddUpload(size);
	}
	s_free( qpp );
}


