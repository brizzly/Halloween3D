
#include "alloween.h"



//NSpGameReference        _gameReference = NULL;
hBool                           _response;
hBool                           _approved;
//HANDLE                          hReceiveThread = NULL;
//HANDLE                          hSendThread = NULL;
//LPVOID                          lpParameter;
//LPVOID                          lpParameter2;
dword                           dwIdReceiveThread = 0;
dword                           dwIdSendThread = 0;
//DWORD WINAPI            net_Process2(LPVOID lpThreadParameter);
//DWORD WINAPI            net_Process(LPVOID lpThreadParameter);
char                            gNetBitKeys;
char                            gNetBitKeys_back;
uint                            gSV_FrameID;
uint                            gCL_FrameID;
int                                     gSV_score;
int                                     gCL_score;


void    net_SendServerScores()
{
}

void    net_SendClientHurt(int health)
{
}

void    net_NetLaunchEnt(int entid, int launcherid, vect_t startpos, vect_t rot, vect_t vel, short IsServer)
{
}

void    net_DispNetInfos()
{
}

void    net_LagometerProcess()
{
}

void    net_Init()
{
}

void    net_UnInit()
{
}
		
void    net_StopMultiplayer()
{
}

void    net_SendVectorState(pEntity Ent, int vstate)
{
}

void    net_RemoveClientEnt(pEntity Ent)
{
}

void    net_DispPodium()
{
}

hBool 	cl_Create()
{
    return true;
}

void	cl_Destroy()
{

}


void sv_Create()
{
}

void sv_Destroy()
{
}


