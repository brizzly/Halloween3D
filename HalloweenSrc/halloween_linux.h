
#ifndef HALLOW_HAHA_INC
#define HALLOW_HAHA_INC

void MACStarter();
bool MACLoop();
void MACend();
int main_Alternate(int argc, char **argv);

float 	swapendf32( float floatValue );

int	key_IsValid(char *name, char *key);
void	LoadingError(char *message);
void	key_CheckGame();


bool 	sys_GameEvent();
void 	sys_SwapBufferGLX();
void	sys_RestartVID();
int	sys_setVID();
void	sys_killVID();
void	sys_killGL();
void	sys_GetKeyDatas();
void	sys_AntiPirate();
void	sys_end();
void	sys_setGamma(float gamma);

void	w32_setGammaRamp(unsigned short *newgammaramp);



#endif // HALLOW_HAHA_INC

