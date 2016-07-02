#ifndef NETSERVER_INCLUDED
#define NETSERVER_INCLUDED

#include "alloween.h"



void sv_Create();
void sv_Destroy();
void sv_BeginHost(char *name);
void sv_StopHost();
void sv_Send(char *message, int id_packet, hBool SafeSend);
void sv_Send2(char *message, int messageLen, int id_packet, hBool SafeSend);






#endif // NETSERVER_INCLUDED


