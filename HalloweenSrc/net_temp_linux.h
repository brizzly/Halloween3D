




void    net_SendServerScores();
void    net_SendClientHurt(int health);
void    net_NetLaunchEnt(int entid, int launcherid, vect_t startpos, vect_t rot, vect_t vel, short IsServer);
void    net_DispNetInfos();
void    net_LagometerProcess();
void    net_Init();
void    net_UnInit();
void    net_StopMultiplayer();
void    net_SendVectorState(pEntity Ent, int vstate);
void    net_RemoveClientEnt(pEntity Ent);
void    net_DispPodium();
hBool 	cl_Create();
void    cl_Destroy();
void 	sv_Create();
void 	sv_Destroy();

