
#include "alloween.cpp"
//#include <bass.h>
#include "bass.h"

HSTREAM		gHstream = 0;


//hBool lb_Init(HWND hwnd)
#ifdef H_LINUX
hBool lb_Init()
#else
hBool	lb_Init(HWND hwnd)
#endif
{
	long ver = BASS_GetVersion();
//	if(ver!=MAKELONG(2,0))
//		return false;

	//if(!BASS_Init(1, 44100, BASS_DEVICE_3D, hwnd, NULL))
    #ifdef H_LINUX
    if(!BASS_Init(-1,44100,BASS_DEVICE_3D,NULL, NULL))
    #else
	if(!BASS_Init(-1,44100,BASS_DEVICE_3D,hwnd, NULL))
    #endif
		return false;

	BASS_Set3DFactors(0.00001f,0.015f,0);

	if(!BASS_Start())
		return false;

	return true;	
}

void lb_UnInit()
{
	BASS_Free();
}

hBool lb_LoadSound(char *name, int nChannel, hBool sound3D)
{
	int	SoundID;
	HSAMPLE	smp;

	if(gNb_Sounds >= MAX_SAMPLE)
		return false;
	SoundID = gNb_Sounds++;
	gSoundObjet[SoundID].nChannel = nChannel;
	if(sound3D)
	{
		//HACK
		if(nChannel >= 64)
			smp = BASS_SampleLoad(false,name,0,0,nChannel,BASS_SAMPLE_3D|BASS_SAMPLE_OVER_POS|BASS_SAMPLE_LOOP);
		else
			smp = BASS_SampleLoad(false,name,0,0,nChannel,BASS_SAMPLE_3D|BASS_SAMPLE_OVER_POS);
	}
	else
	{
		smp = BASS_SampleLoad(false,name,0,0,nChannel,BASS_SAMPLE_OVER_POS); // VOL
	}
	if (smp == 0) {
		int errorBass = BASS_ErrorGetCode();
		m_ConsPrint("[%s]err Bass %d\n", name, errorBass);
	}
	gSoundObjet[SoundID].bass_sample = smp;
	if(!gSoundObjet[SoundID].bass_sample)
		return false;
	return true;
}

void lb_FreeSound(int SoundID)
{
	if(gSoundObjet[SoundID].bass_sample)
		BASS_SampleFree(gSoundObjet[SoundID].bass_sample);
}

void lb_PlaySound(int SoundID, int volume)
{
	int		freq;

	if(volume <= 0)
		return;

	if(gCheatMode_son)
	{
		freq = 44100;
	}
	else
	{
		if(game_MatrixMode)
		{
			freq = 11025;
		}
		else
		{
			freq = -1;
		}
	}
	if (gSoundObjet[SoundID].bass_sample) {
		
		HCHANNEL channel = BASS_SampleGetChannel(gSoundObjet[SoundID].bass_sample, false);
		BASS_ChannelPlay(channel, true);

	//	BASS_SamplePlayEx(gSoundObjet[SoundID].bass_sample, 0, freq, volume, -101, -1);
	}
}

vect_t lb_Set3Dpos(vect_t pos)
{
	pos.X -= gCamera.Pos.X;
	pos.Y -= gCamera.Pos.Y;
	pos.Z -= gCamera.Pos.Z;
	pos.Z = - pos.Z;
	pos = m_RotateX(pos,gCamera.Rot.X);
	pos = m_RotateY(pos,gCamera.Rot.Y);
	return pos;
}

void lb_PlaySound3D(int SoundID, vect_t pos, int volume)
{
	int		sound_pos;
	int		freq;

	if(SoundID >= MAX_SAMPLE)
		return;
	if(!gSoundObjet[SoundID].bass_sample)
		return;

	for(sound_pos=0 ; sound_pos<gSoundObjet[SoundID].nChannel ; sound_pos++)
		if(gSoundObjet[SoundID].channel3D[sound_pos] == 0)
			break;
	if(sound_pos >= gSoundObjet[SoundID].nChannel)
		return;
	if(sound_pos >= MAX_CLONESAMPLE)
		return;

	gSoundObjet[SoundID].sound3D = true;
	gSoundObjet[SoundID].pos[sound_pos] = pos;
	gSoundObjet[SoundID].time_chan3D[sound_pos] = 0;

	pos = lb_Set3Dpos(pos);
	
	gSoundObjet[SoundID].bass_3Dpos.x = pos.X;
	gSoundObjet[SoundID].bass_3Dpos.y = pos.Y;
	gSoundObjet[SoundID].bass_3Dpos.z = pos.Z;

	// HACK
	if(ProgramState == PS_GREETINGS || ProgramState == PS_INTRO || ProgramState == PS_GAMEOVER)
	{

	}
	else
	{
		volume = 0;
	}

	HCHANNEL channel = BASS_SampleGetChannel(gSoundObjet[SoundID].bass_sample, false);
	//BASS_ChannelSet3DAttributes(channel, &gSoundObjet[SoundID].bass_3Dpos
	BOOL res = BASS_ChannelSet3DPosition(channel, &gSoundObjet[SoundID].bass_3Dpos, NULL, NULL);
	if (res == FALSE) {
		m_ConsPrint("err 3D sound\n");
	}
	res = BASS_ChannelPlay(channel, true);

	gSoundObjet[SoundID].channel3D[sound_pos] = channel; //  BASS_SamplePlay3D(gSoundObjet[SoundID].bass_sample, &gSoundObjet[SoundID].bass_3Dpos, NULL, NULL);
	
	if(gCheatMode_son)
	{
		freq = 44100;
	}
	else
	{
		if(game_MatrixMode)
		{
			freq = 11025;
		}
		else
		{
			freq = -1;
		}
	}
	//BASS_ChannelSetAttributes(gSoundObjet[SoundID].channel3D[sound_pos],freq,0/*volume*/,-101);

	if (freq != -1)
	{
		res = BASS_ChannelSetAttribute(gSoundObjet[SoundID].channel3D[sound_pos],
			BASS_ATTRIB_FREQ,
			(float)freq);
		if (res == FALSE) {
			m_ConsPrint("err 3D sound\n");
		}
	}

	res = BASS_ChannelSetAttribute(gSoundObjet[SoundID].channel3D[sound_pos],
		BASS_ATTRIB_VOL,
 		volume);
	if (res == FALSE) {
		m_ConsPrint("err 3D sound\n");
	}

}

void lb_StopChannel3D(int idsound, int idchan)
{
//	int	last_sound;

	BOOL res = BASS_ChannelStop(gSoundObjet[idsound].channel3D[idchan]);
	if (res == FALSE) {
		//m_ConsPrint("err 3D sound\n");
	}

//	BASS_Apply3D();
/*	last_sound = gSoundObjet[idsound].nChannel - 1;
	if(last_sound < 0)
	{
		gSoundObjet[idsound].nChannel = 0;
		gSoundObjet[idsound].channel3D[idchan] = 0;
		return;
	}
	else if(last_sound == 0)
	{
		gSoundObjet[idsound].nChannel = 0;
		gSoundObjet[idsound].channel3D[idchan] = 0;
		return;
	}
	else
	{
		gSoundObjet[idsound].channel3D[idchan] = gSoundObjet[idsound].channel3D[last_sound];
		gSoundObjet[idsound].time_chan3D[idchan] = gSoundObjet[idsound].time_chan3D[last_sound];
		gSoundObjet[idsound].channel3D[last_sound] = 0;
		gSoundObjet[idsound].time_chan3D[last_sound] = 0;
		gSoundObjet[idsound].nChannel--;
	}
*/
	gSoundObjet[idsound].channel3D[idchan] = 0;
	gSoundObjet[idsound].time_chan3D[idchan] = 0;
}

void lb_UpdateSound3D(int volume)
{
	int				SoundID;
	vect_t			pos;
	int				sound_pos;
	hBool			apply = false;
	int				leaf_sound;
	int				pvs_index;

	if(ProgramState != PS_GAME &&
		ProgramState != PS_DIE &&
		ProgramState != PS_WIN &&
		ProgramState != PS_PAUSEDGAME)
		return;

	for(SoundID=0 ; SoundID<gNb_Sounds ; SoundID++)
	if(gSoundObjet[SoundID].sound3D)
	{		
		for(sound_pos=0 ; sound_pos<gSoundObjet[SoundID].nChannel ; sound_pos++)
		if(gSoundObjet[SoundID].channel3D[sound_pos])
		{
			apply = true;

			// expiration du son (hack)
			gSoundObjet[SoundID].time_chan3D[sound_pos] += gFrameTime_msec;
			if(gSoundObjet[SoundID].time_chan3D[sound_pos] >= 8000.0f)
			{
				lb_StopChannel3D(SoundID, sound_pos);
				continue;
			}

			if(BASS_ACTIVE_STOPPED == BASS_ChannelIsActive(gSoundObjet[SoundID].channel3D[sound_pos]))
			{
				lb_StopChannel3D(SoundID, sound_pos);
				continue;
			}

			pos = gSoundObjet[SoundID].pos[sound_pos];
			if(m_Distance(pos,gCamera.Pos) > 500)
			{
				lb_StopChannel3D(SoundID, sound_pos);
				continue;
			}
			if(MapDataFormat == MAP_BSP)
			{
				leaf_sound = bsp_FindCamera(pos);
				if(leaf_sound < 0)
				{
					lb_StopChannel3D(SoundID, sound_pos);
					continue;
				}
				pvs_index = LeafArray[leaf_sound].PVSIndex;
				if(pvs_index < 0)
				{
					lb_StopChannel3D(SoundID, sound_pos);
					continue;
				}
				if(PVSData[pvs_index+leaf_sound] != '1')
				{
					lb_StopChannel3D(SoundID, sound_pos);
					continue;
				}
			}
			pos = lb_Set3Dpos(pos);
			gSoundObjet[SoundID].bass_3Dpos.x = pos.X;
			gSoundObjet[SoundID].bass_3Dpos.y = pos.Y;
			gSoundObjet[SoundID].bass_3Dpos.z = pos.Z;
			BASS_ChannelSet3DPosition(gSoundObjet[SoundID].channel3D[sound_pos],&gSoundObjet[SoundID].bass_3Dpos,NULL,NULL);
			//BASS_ChannelSetAttributes(gSoundObjet[SoundID].channel3D[sound_pos],-1,volume,-101);

			BASS_ChannelSetAttribute(gSoundObjet[SoundID].channel3D[sound_pos],
				BASS_ATTRIB_VOL,
				(float)volume);

		}
	}
	if(apply)
	{
		BASS_Apply3D();
	}
}

void lb_StopSound3D()
{
	int		SoundID;
	int		sound_pos;

	for(SoundID=0 ; SoundID<gNb_Sounds ; SoundID++)
	if(gSoundObjet[SoundID].sound3D)
	{
		for(sound_pos=0 ; sound_pos<gSoundObjet[SoundID].nChannel ; sound_pos++)
		{
			//BASS_ChannelStop(gSoundObjet[SoundID].channel3D[sound_pos]);
			//gSoundObjet[SoundID].channel3D[sound_pos] = 0;
			lb_StopChannel3D(SoundID, sound_pos);
		}
		gSoundObjet[SoundID].sound3D = false;
	}
}

void lb_StopMusic()
{
	if(gHstream && BASS_ChannelIsActive(gHstream) == BASS_ACTIVE_PLAYING)
	{
		BASS_ChannelStop(gHstream);
	}
}

hBool lb_PlayMP3(char *name, int volume)
{
	lb_StopMusic();

	gHstream = BASS_StreamCreateFile(false,name,0,0,BASS_STREAM_AUTOFREE);
	if(gHstream)
	{
		BASS_ChannelPlay(gHstream, true);
		//BASS_StreamPlay(gHstream,0,0);
		lb_SetChannelVol(volume);
		return true;
	}
	else
		return false;
}

void lb_UpdateMP3()
{
	if(gHstream && BASS_ChannelIsActive(gHstream) == BASS_ACTIVE_STOPPED)
	{
		m_ConsPrint("stream music loop\n");
		if(gNbTrack >= 8 && gNbTrack <= 11)
		{					
			if(pres_track.value >= 3)
				pres_track.value = 0;
			else
				pres_track.value++;

			game_LaunchPresMusic();
			return;
		}
		else
		{
			ds_LoadStreamWAVE(game_GetMusicString((int)music_track.value));
			music_track.value++;
			music_track.value = (float)((int)music_track.value % 8);
		}
	}
}

void lb_SetChannelVol(int volume)
{
	if (gHstream) {
		//BASS_ChannelSetAttributes(gHstream, -1, volume, -101);
		BASS_ChannelSetAttribute(gHstream,
			BASS_ATTRIB_VOL,
			(float)volume);
	}
}



