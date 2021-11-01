#ifndef DSOUND_INCLUDED
#define DSOUND_INCLUDED

#include "alloween.h"
#include "bspfile.h"

//#include <windows.h>
//#include <bass.h>
#include "misc/bass.h"

typedef struct SNDOBJTAG
{
	int					nChannel;
	hBool				sound3D;
	BASS_3DVECTOR		bass_3Dpos;
	HSAMPLE				bass_sample;
	DWORD				channel3D[MAX_CLONESAMPLE];
	vect_t				pos[MAX_CLONESAMPLE];
	float				time_chan3D[MAX_CLONESAMPLE];

} SNDOBJ;

#ifdef H_LINUX
hBool ds_Init();
#else
hBool	ds_Init(HWND hWnd, HINSTANCE hInstance);
#endif
hBool	ds_LoadSounds();
void	ds_UnloadSounds();
hBool	ds_LoadWAVE(char *FolderPath, char *lpzFileName, int nChannel,  hBool sound3D);
hBool	ds_LoadStreamWAVE(char *lpzFileName);
hBool	ds_PlayBuffer( void );
void	ds_AdjustSoundVolumes();
void	ds_AdjustMusicVolumes();
hBool	ds_PlaySound(int SoundID);
hBool	ds_PlaySound3D(int SoundID, vect_t pos, vect_t vel);
void	ds_Cleanup( void );




#endif DSOUND_INCLUDED
