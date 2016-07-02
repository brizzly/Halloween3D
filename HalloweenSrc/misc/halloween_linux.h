
#ifndef MAIN_LINUX__
#define MAIN_LINUX__


void 	sys_RestartVID();
int   	sys_setVID();
void    sys_killVID();
void    sys_GetKeyDatas();
void    sys_AntiPirate();
void    LoadingError(char *message);
void    key_CheckGame();
void    end();
 
 
void    w32_setGammaRamp(unsigned short *newgammaramp);
void    w32_setGamma(double gamma);










#endif //MAIN_LINUX__



