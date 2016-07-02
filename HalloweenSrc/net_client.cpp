
#include "alloween.cpp"
#include "net_client.h"



#include <OpenPlay.h>


extern NSpGameReference		_gameReference;
extern hBool	_response;
extern hBool	_approved;


hBool cl_Create()
{
	unsigned char	playerNameStr[32];
	char			theIPAddress[256];
	char			thePlayer[256];
	char			thePort[32];
	NMUInt32		port;
	NSpAddressReference addRef;
	NMErr			err;
	NMUInt32		startTime, currentTime;
	time_t			seconds;
	NSpGameInfo		gameInfo;
	char			str[256];
	unsigned char	Pass[32];

	sprintf(theIPAddress,"%s",net_ip.string);
	port = (NMUInt32)net_port.value;
	sprintf( thePort, "%li", port );
	sprintf(thePlayer,"%s",net_clname.string);

	GameC2PStr( thePlayer, playerNameStr );
	GameC2PStr( net_password.string, Pass );


	err = NSpInitialize( 0, 0, 0, 'NSe1', 0 );
	if( err )
	{
		m_ConsPrint("Network Client Initialization failed\n");
		if(!IsConsoleActive) IsConsoleActive = true;
		return false;
	}

	addRef = NSpCreateIPAddressReference( theIPAddress, thePort );
	if(!addRef )
	{
		m_ConsPrint("Network kNMParameterErr Client error\n");
		if(!IsConsoleActive) IsConsoleActive = true;
		return false;		
	}

	m_ConsPrint( "Attempting to join game...\n" );

	// look for a server on the IP/Port given and see if we can connect
	err = NSpGame_Join( &_gameReference, addRef, playerNameStr, Pass, kPlayerType, NULL, 0, 0 );
	if(err)
	{
		m_ConsPrint("Network NSpGame_Join Client error\n");
		if(!IsConsoleActive) IsConsoleActive = true;
		return false;		
	}

	m_ConsPrint("connected, Waiting for approval to join game...\n");

	time(&seconds);
	startTime = seconds;

	_response = _approved = false;

	// We connected, now we have to wait for the server to approve our join request

	while( !_response )
	{
		// Check for a time out in connecting to server
		// this is before the event handler so that we are not approved, but time out anyway (small chance, but...)

		time(&seconds);
		currentTime = seconds;

		if( (currentTime - startTime > 60) && (!_response) )
		{
			m_ConsPrint( "ERROR: Connection timed out!\n" );

			_response = true;
			_approved = false;
		}

		// Handle system messages and allow user to quit via 'q'
		// This also gets and handles network messages, like accept/deny

		net_HandleMessage();
	}

	// if we were not approved, we must dispose of the game object here
	if( !_approved )
	{
		m_ConsPrint( "Network client connection not approved\n" );

		err = NSpGame_Dispose( _gameReference, kNSpGameFlag_ForceTerminateGame );
		_gameReference = NULL;
		if(!IsConsoleActive) IsConsoleActive = true;
		return false;
	}
	else	// let the user know that they were accepted to the game
	{
		err = NSpGame_GetInfo( _gameReference, &gameInfo );
		if( !err )
		{
			GameP2CStr( gameInfo.name, str );
			m_ConsPrint( "Welcome to the game '%s', with %d players\n", str, (int)gameInfo.currentPlayers );
		}
	}
	m_ConsPrint("Network Client ok\n");
	return true;
}

void cl_Destroy()
{
	NMErr err = kNMNoError;

	m_ConsPrint("Network UnInitialization\n");

	if( _gameReference )
	{
		err = NSpGame_Dispose( _gameReference, kNSpGameFlag_ForceTerminateGame );
	}
}

void cl_Connect()
{
}

void cl_Disconnect()
{
}

void cl_Send(char *message, int id_packet, hBool SafeSend, hBool HostOnly)
{
	MessagePacketPtr	qpp;
	unsigned long		messageLen, size;
	NMErr				err;
	NSpFlags			Flag;

	if( !message )
		return; //( kNSpInvalidParameterErr );

	messageLen = strlen( message );
	size = sizeof(MessagePacket_t) + messageLen + 1;	/* header + num_chars + terminator */

	qpp = (MessagePacketPtr)malloc( size );
	if( !qpp )
	{
		m_ConsPrint(" *** ERROR *** Unable to allocate message buffer!\n");
		fflush(stdout);
		return; //( kNSpMemAllocationErr );
	}

	NSpClearMessageHeader( &qpp->header );

	qpp->header.what = (NMSInt32)id_packet;
	if(HostOnly)
		qpp->header.to = kNSpHostOnly;
	else
		qpp->header.to = kNSpAllPlayers;
	qpp->header.messageLen = size;

	// fill in the data section
	strcpy( qpp->str, message );

	// tell NetSprocket to send the message
	if(SafeSend)
		Flag = kNSpSendFlag_Registered;
	else
		Flag = /*kNSpSendFlag_Junk;//*/kNSpSendFlag_Normal;

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

void cl_Send2(char *message, int messageLen, int id_packet, hBool SafeSend, hBool HostOnly)
{
	MessagePacketPtr	qpp;
	unsigned long		size;
	NMErr				err;
	NSpFlags			Flag;

	if( !message )
		return; //( kNSpInvalidParameterErr );

	size = sizeof(MessagePacket_t) + messageLen + 1;	/* header + num_chars + terminator */

	qpp = (MessagePacketPtr)malloc( size );
	if( !qpp )
	{
		m_ConsPrint(" *** ERROR *** Unable to allocate message buffer!\n");
		fflush(stdout);
		return; //( kNSpMemAllocationErr );
	}

	NSpClearMessageHeader( &qpp->header );

	qpp->header.what = (NMSInt32)id_packet;
	if(HostOnly)
		qpp->header.to = kNSpHostOnly;
	else
		qpp->header.to = kNSpAllPlayers;
	qpp->header.messageLen = size;

	// fill in the data section
	memcpy( qpp->str, message, messageLen );

	// tell NetSprocket to send the message
	if(SafeSend)
		Flag = kNSpSendFlag_Registered;
	else
		Flag = /*kNSpSendFlag_Junk;//*/kNSpSendFlag_Normal;

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



