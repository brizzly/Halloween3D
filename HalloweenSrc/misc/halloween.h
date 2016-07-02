
#include "misc/bass.h"


BOOL CALLBACK DialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam,LPARAM lParam);
BOOL CALLBACK DialogProc2(HWND hwndDlg, UINT uMsg, WPARAM wParam,LPARAM lParam);
BOOL CALLBACK DialogProc3(HWND hwndDlg, UINT uMsg, WPARAM wParam,LPARAM lParam);
BOOL CALLBACK DialogProc4(HWND hwndDlg, UINT uMsg, WPARAM wParam,LPARAM lParam);

BOOL key_IsValid(char *name, char *key);


void	sys_RestartVID();
BOOL	sys_setVID();
void	sys_killVID();
void	sys_GetKeyDatas();
void	sys_AntiPirate();
void	LoadingError(char *message);
void	key_CheckGame();
void	end();


void	w32_setGammaRamp(unsigned short *newgammaramp);
void	w32_setGamma(double gamma);

//void ErrorMessage(char *msg);
