#ifndef NETCLIENT_INCLUDED
#define NETCLIENT_INCLUDED

#include "alloween.h"




hBool cl_Create();
void cl_Destroy();
void cl_Connect();
void cl_Disconnect();
void cl_Send(char *message, int id_packet, hBool SafeSend, hBool HostOnly);
void cl_Send2(char *message, int messageLen, int id_packet, hBool SafeSend, hBool HostOnly);



#endif // NETCLIENT_INCLUDED

