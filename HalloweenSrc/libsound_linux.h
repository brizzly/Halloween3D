
#ifndef LIBSON_LINUX
#define LIBSON_LINUX

#include "alloween.h"
#include "bspfile.h"

#ifdef H_MAC
#include "KSound.h"
#include "KMusic.h"
#endif
//#include "SDL_mixer.h"


#define	MAX_SNDCHAN	16


typedef struct SNDOBJTAG
{
 	int        	nChannel;
	hBool      	sound3D;
   	dword		channel3D[MAX_CLONESAMPLE];
    vect_t		pos[MAX_CLONESAMPLE];
    float		time_chan3D[MAX_CLONESAMPLE];
    #ifdef H_MAC
	KSound 		*snd_chunk;
    #endif

} SNDOBJ;


void 	fill_audio(void *udata, byte *stream, int len);
hBool	ds_Init();
void 	ds_Cleanup( void );
hBool 	ds_LoadSounds();
void 	ds_UnloadSounds();
hBool 	ds_LoadWAVE(char *FolderPath, char *lpzFileName, int nChannel,  hBool sound3D);
hBool 	ds_LoadStreamWAVE( char *lpzFileName);
void 	ds_AdjustSoundVolumes();
void 	ds_AdjustMusicVolumes();
hBool 	ds_PlaySound(int SoundID);
hBool 	ds_PlaySound3D(int SoundID, vect_t pos, vect_t vel);
hBool 	ds_PlayBuffer( void );


void    lb_StopSound3D();
hBool 	lb_LoadSound(char *name, int nChannel, hBool sound3D);
void 	lb_FreeSound(int SoundID);
void 	lb_PlaySound(int SoundID, int volume);
vect_t 	lb_Set3Dpos(vect_t pos);
void 	lb_PlaySound3D(int SoundID, vect_t pos, int volume);
void 	lb_StopChannel3D(int idsound, int idchan);
void 	lb_UpdateSound3D(int volume);
void 	lb_StopSound3D();
void 	lb_StopMusic();
hBool 	lb_PlayMP3(char *name, int volume);
void 	lb_UpdateMP3();
void 	lb_SetChannelVol(int volume);


#endif

