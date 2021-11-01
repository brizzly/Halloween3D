
#ifndef LIBASS_INCLUDED
#define LIBASS_INCLUDED


#include "alloween.h"

#include "bspfile.h"
//#include <bass.h>
#include "bass.h"


//hBool	lb_Init(HWND hwnd);
#ifdef H_LINUX
hBool lb_Init();
#else
hBool	lb_Init(HWND hwnd);
#endif
void	lb_UnInit();
hBool	lb_LoadSound(char *name, int nChannel, hBool sound3D);
void	lb_FreeSound(int SoundID);
void	lb_PlaySound(int SoundID, int volume);
vect_t	lb_Set3Dpos(vect_t pos);
void	lb_PlaySound3D(int SoundID, vect_t pos, int volume);
void	lb_StopChannel3D(int idsound, int idchan);
void	lb_UpdateSound3D(int volume);
void	lb_StopSound3D();
void	lb_StopMusic();
hBool	lb_PlayMP3(char *name, int volume);
void	lb_UpdateMP3();
void	lb_SetChannelVol(int volume);




#endif LISTE_INCLUDED
