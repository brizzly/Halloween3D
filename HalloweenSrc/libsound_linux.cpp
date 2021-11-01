
#include "alloween.h"
#ifdef H_MAC
#include "KSound.h"
#include "KMusic.h"
#endif


int		gNb_Sounds;
SNDOBJ	gSoundObjet[MAX_SAMPLE];
int		gNbTrack;
#ifdef H_MAC
KMusic *gMusic = NULL;
#endif


hBool ds_Init()
{
	if(gIsServer && net_dedicated.value)
		return true;
                
	if(!soundsys.value)
		return true;

	m_ConsPrint("sound system ini OK\n");
	return true;
}

void ds_Cleanup( void )
{
	if(gIsServer && net_dedicated.value)
		return;

	if(!soundsys.value)
		return;
                
	lb_StopMusic();

    #ifdef H_MAC
	if(gMusic)
	{
		delete gMusic;
		gMusic = NULL;
	}
    #endif

	ds_UnloadSounds();
	//lb_UnInit();

}

hBool ds_LoadSounds()
{
	int		i;
	int		j;
	char	FolderPath[255];

	if(gIsServer && net_dedicated.value)
		return true;

	if(!soundsys.value)
		return true;

	for(i=0 ; i<MAX_SAMPLE ; i++)
	{
		gSoundObjet[i].sound3D = false;
		//gSoundObjet[i].bass_sample = 0;
		gSoundObjet[i].nChannel = 0;
		for(j=0 ; j<MAX_CLONESAMPLE ; j++)
		{
			gSoundObjet[i].channel3D[j] = 0;
			gSoundObjet[i].pos[j].X = 0;
			gSoundObjet[i].pos[j].Y = 0;
			gSoundObjet[i].pos[j].Z = 0;
		}
	}

	gNb_Sounds = 0;
	sprintf(FolderPath,"%s%s",SYSDIR,SNDDIR);
	
	ds_LoadWAVE(FolderPath,"gun_pickup.wav", 4,false);		// 0
	ds_LoadWAVE(FolderPath,"amo_pickup.wav",4,false);		// 1
	ds_LoadWAVE(FolderPath,"fleches.wav",4,false);			// 2
	ds_LoadWAVE(FolderPath,"rollover.wav",1,false);	//4//	// 3
	ds_LoadWAVE(FolderPath,"click.wav",4,false);			// 4
	ds_LoadWAVE(FolderPath,"noammo.wav",1,false);			// 5
	ds_LoadWAVE(FolderPath,"change.wav",2,false);			// 6
	ds_LoadWAVE(FolderPath,"rollover_fire.wav",1,false); 	// 7
	ds_LoadWAVE(FolderPath,"hit.wav",3,false);				// 8
	ds_LoadWAVE(FolderPath,"jump1.wav",3,false);			// 9
	ds_LoadWAVE(FolderPath,"fry.wav",1,false);				// 10
	ds_LoadWAVE(FolderPath,"pain.wav",1,false);				// 11
	ds_LoadWAVE(FolderPath,"bombbounce.wav",6,true);		// 12
	ds_LoadWAVE(FolderPath,"shot3.wav",4,false);			// 13
	ds_LoadWAVE(FolderPath,"explode.wav",4,true);			// 14
	ds_LoadWAVE(FolderPath,"toucheclavier.wav",4,true);		// 15
	ds_LoadWAVE(FolderPath,"welcome.wav",1,false);			// 16
	ds_LoadWAVE(FolderPath,"youdienow1.wav",1,false);		// 17
	ds_LoadWAVE(FolderPath,"fleche_c.wav",3,true);			// 18
	ds_LoadWAVE(FolderPath,"thunder1.wav",1,false); 		// 19
	ds_LoadWAVE(FolderPath,"fermeture_porte.wav",4,true);	// 20
	ds_LoadWAVE(FolderPath,"gameover.wav",1,false);			// 21
	ds_LoadWAVE(FolderPath,"thunder2.wav",1,false); 		// 22
	ds_LoadWAVE(FolderPath,"coup_dans_le_vent_1.wav",2,false);	// 23
	ds_LoadWAVE(FolderPath,"coup_dans_le_vent_2.wav",2,false);	// 24
	ds_LoadWAVE(FolderPath,"coup_dans_le_vent_3.wav",2,false);	// 25
	ds_LoadWAVE(FolderPath,"gourdin_aouch_1.wav",2,false);	// 26
	ds_LoadWAVE(FolderPath,"gourdin_aouch_2.wav",2,false);	// 27
	ds_LoadWAVE(FolderPath,"tromblon.wav",1,false);			// 28
	ds_LoadWAVE(FolderPath,"tromblon_shot.wav",4,false);	// 29
	ds_LoadWAVE(FolderPath,"thunder3.wav",1,false); 		// 30
	ds_LoadWAVE(FolderPath,"rire_diabolique5.wav",1,false);	// 31
	ds_LoadWAVE(FolderPath,"rire_diabolique7.wav",1,false);	// 32
	ds_LoadWAVE(FolderPath,"hahaha.wav",1,false);			// 33
	ds_LoadWAVE(FolderPath,"fire1.wav",1,true);				// 34
	ds_LoadWAVE(FolderPath,"rire_diabolique1.wav",1,false); // 35
	ds_LoadWAVE(FolderPath,"rire_diabolique2.wav",1,false);	// 36
	ds_LoadWAVE(FolderPath,"rire_diabolique3.wav",1,false);	// 37
	ds_LoadWAVE(FolderPath,"subzerat_death.wav",2,false);	// 38
	ds_LoadWAVE(FolderPath,"sound1.wav",1,false);			// 39 //
	ds_LoadWAVE(FolderPath,"uwin.wav",1,false);				// 40
	ds_LoadWAVE(FolderPath,"prepare.wav",1,false);			// 41
	ds_LoadWAVE(FolderPath,"usuck.wav",1,false);			// 42
	ds_LoadWAVE(FolderPath,"killdepla.wav",1,false);		// 43
	ds_LoadWAVE(FolderPath,"killmorbak.wav",1,false);		// 44
	ds_LoadWAVE(FolderPath,"deepla_cri.wav",2,true);		// 45
	ds_LoadWAVE(FolderPath,"deepla_normal.wav",2,true);		// 46
	ds_LoadWAVE(FolderPath,"deepla_normal2.wav",2,true);	// 47
	ds_LoadWAVE(FolderPath,"deepla_death.wav",6,true);		// 48
	ds_LoadWAVE(FolderPath,"deepla_grognement1.wav",6,true);// 49
	ds_LoadWAVE(FolderPath,"zeubat.wav",1,false);			// 50
	ds_LoadWAVE(FolderPath,"zebat_attack1.wav",2,true);		// 51
	ds_LoadWAVE(FolderPath,"zemummy_death.wav",1,false);	// 52
	ds_LoadWAVE(FolderPath,"gibs.wav",6,true);				// 53
	ds_LoadWAVE(FolderPath,"gibounce.wav",6,true);			// 54
	ds_LoadWAVE(FolderPath,"momie_attaque1.wav",2,true);	// 55
	ds_LoadWAVE(FolderPath,"momie_attaque2.wav",2,true);	// 56
	ds_LoadWAVE(FolderPath,"momie_death.wav",6,true);		// 57
	ds_LoadWAVE(FolderPath,"momie_grognement1.wav",6,true);	// 58
	ds_LoadWAVE(FolderPath,"momie_grognement2.wav",6,true);	// 59
	ds_LoadWAVE(FolderPath,"momie_grognement3.wav",6,true);	// 60
	ds_LoadWAVE(FolderPath,"impact_fleche1.wav",3,true); 	// 61
	ds_LoadWAVE(FolderPath,"impact_fleche2.wav",3,true);	// 62
	ds_LoadWAVE(FolderPath,"deepla_grognement2.wav",6,true);// 63
	ds_LoadWAVE(FolderPath,"deepla_grognement3.wav",6,true);// 64
	ds_LoadWAVE(FolderPath,"morbak_attack1.wav",2,true);	// 65
	ds_LoadWAVE(FolderPath,"morbak_attack2.wav",2,true);	// 66
	ds_LoadWAVE(FolderPath,"morbak_death.wav",5,true);		// 67
	ds_LoadWAVE(FolderPath,"morbak_grognement1.wav",6,true);// 68
	ds_LoadWAVE(FolderPath,"morbak_grognement2.wav",6,true);// 69
	ds_LoadWAVE(FolderPath,"zebat_attack2.wav",3,true);		// 70
	ds_LoadWAVE(FolderPath,"zebat_grognement1.wav",6,true);	// 71
	ds_LoadWAVE(FolderPath,"zebat_grognement2.wav",6,true);	// 72
	ds_LoadWAVE(FolderPath,"subzerat_attack.wav",2,true);	// 73
	ds_LoadWAVE(FolderPath,"subzerat_grognement1.wav",5,true);	// 74
	ds_LoadWAVE(FolderPath,"subzerat_grognement2.wav",5,true);	// 75
	ds_LoadWAVE(FolderPath,"sound1.wav",1,false);	// 76
	ds_LoadWAVE(FolderPath,"sound2.wav",1,false);	// 77
	ds_LoadWAVE(FolderPath,"sound3.wav",1,false);	// 78
	ds_LoadWAVE(FolderPath,"sound4.wav",1,false);	// 79
	ds_LoadWAVE(FolderPath,"sound5.wav",1,false);	// 80
	ds_LoadWAVE(FolderPath,"sound6.wav",1,false);	// 81
	ds_LoadWAVE(FolderPath,"sound7.wav",1,false);	// 82
	ds_LoadWAVE(FolderPath,"sound8.wav",1,false);	// 83
	ds_LoadWAVE(FolderPath,"sound9.wav",1,false);	// 84
	

	ds_AdjustSoundVolumes();
	ds_AdjustMusicVolumes();

	return true;
}

void ds_UnloadSounds()
{
	int		i;

	if(gIsServer && net_dedicated.value)
		return;

	if(!soundsys.value)
		return;

	for(i=0 ; i<gNb_Sounds ; i++)
	{
		lb_FreeSound(i);
	}
	gNb_Sounds = 0;
}

hBool ds_LoadWAVE(char *FolderPath, char *lpzFileName, int nChannel,  hBool sound3D)
{
	char	path[255];

	if(gIsServer && net_dedicated.value)
		return true;

	if(nChannel<1)
		nChannel=1;
	if(nChannel>MAX_CLONESAMPLE)
		nChannel = MAX_CLONESAMPLE;

	sprintf(path,"%s%s",FolderPath,lpzFileName);

	if(!lb_LoadSound(path,nChannel,sound3D))
	{
		m_ConsPrint("load wave error: %s\n",path);
		return false;
	}
	else
	{
		m_ConsPrint("ds_LoadWAVE %s\n",path);
		return true;
	}
}

hBool ds_LoadStreamWAVE( char *lpzFileName)
{
	char			tmp[255];
	char			path[255];

	if(gIsServer && net_dedicated.value)
		return true;
                
	if(!soundsys.value)
		return true;

	sprintf(tmp,"%s",lpzFileName);

	if(!strcmp(tmp,TRACK_0))
		gNbTrack = 0;
	if(!strcmp(tmp,TRACK_1))
		gNbTrack = 1;
	if(!strcmp(tmp,TRACK_2))
		gNbTrack = 2;
	if(!strcmp(tmp,TRACK_3))
		gNbTrack = 3;
	if(!strcmp(tmp,TRACK_4))
		gNbTrack = 4;
	if(!strcmp(tmp,TRACK_5))
		gNbTrack = 5;
	if(!strcmp(tmp,TRACK_6))
		gNbTrack = 6;
	if(!strcmp(tmp,TRACK_7))
		gNbTrack = 7;
	if(!strcmp(tmp,TRACK_PRES1))
	{
		gNbTrack = 8 + (int)pres_track.value;
		switch((int)(pres_track.value))
		{
			case 0:
			sprintf(tmp,"%s",TRACK_PRES1);
			break;

			case 1:
			sprintf(tmp,"%s",TRACK_PRES2);
			break;

			case 2:
			sprintf(tmp,"%s",TRACK_PRES3);
			break;

			case 3:
			sprintf(tmp,"%s",TRACK_PRES4);
			break;

			default:
			m_ConsPrint("trackpres error %d\n",(int)pres_track.value);
			break;
		}
	}
	if(!strcmp(tmp,TRACK_PB))
		gNbTrack = -1;

	m_ConsPrint("gNbTrack %d\n",gNbTrack);

	sprintf(path,"%s%s%s",SYSDIR,MUSDIR,tmp);
	m_ConsPrint("ds_LoadStreamWAVE %s\n",path);

	return lb_PlayMP3(path,(int)musicvol.value);
}

void ds_AdjustSoundVolumes()
{
	int	i;
        
	if(gIsServer && net_dedicated.value)
		return;
                
	if(!soundsys.value)
		return;
                
    for(i=0 ; i<MAX_SNDCHAN ; i++)
	{
		//Mix_Volume(i, (int)soundvol.value);
	}
	return;
}

void ds_AdjustMusicVolumes()
{
	if(gIsServer && net_dedicated.value)
		return;
                
	if(!soundsys.value)
		return;

	lb_SetChannelVol((int)musicvol.value);
}

void lb_SetChannelVol(int volume)
{
    #ifdef H_MAC
	if(gMusic)
	{
		gMusic->setVolume( volume );
	}
    #endif
}

hBool ds_PlaySound(int SoundID)
{
	if(gIsServer && net_dedicated.value)
		return true;

	if(soundsys.value == 0)
		return true;

	lb_PlaySound(SoundID,(int)soundvol.value);
	return true;
}

hBool ds_PlaySound3D(int SoundID, vect_t pos, vect_t vel)
{
	if(gIsServer && net_dedicated.value)
		return true;

	if(soundsys.value == 0)
		return true;
                
	lb_PlaySound3D(SoundID, pos, (int)soundvol.value);
	return true;
}

hBool ds_PlayBuffer( void )
{
	if(gIsServer && net_dedicated.value)
		return true;

	lb_UpdateSound3D((int)soundvol.value);
	lb_UpdateMP3();
	return true;
}

//====================================================


hBool lb_LoadSound(char *name, int nChannel, hBool sound3D)
{
	int		SoundID;
    #ifdef H_MAC
	KSound	*smp = NULL;
    #endif
	char	*p;
	
	if(gNb_Sounds >= MAX_SAMPLE)
		return false;
	SoundID = gNb_Sounds++;
	gSoundObjet[SoundID].nChannel = nChannel;
//	if(sound3D)

	p = KMiscTools::makeFilePath( name );
	smp = new KSound;
	smp->loadSample( p );

	gSoundObjet[SoundID].snd_chunk = smp;
	if(!gSoundObjet[SoundID].snd_chunk)
	{
   		m_ConsPrint("lb_LoadSound error\n");
		return false;
	}
	return true;
}

void lb_FreeSound(int SoundID)
{
	if(gSoundObjet[SoundID].snd_chunk)
	{
		delete gSoundObjet[SoundID].snd_chunk;
		gSoundObjet[SoundID].snd_chunk = NULL;
	}
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
	
	if(gSoundObjet[SoundID].snd_chunk)
	{
		gSoundObjet[SoundID].snd_chunk->stopSample();
		gSoundObjet[SoundID].snd_chunk->setVolume( volume );
		gSoundObjet[SoundID].snd_chunk->playSample();
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
        //int		sound_pos;
	//int		freq;
	float		dist;
	//int		channel;
	float		newvol;
	//vect_t	v1
        vect_t		v2;
	float		angle;

	dist =  m_Distance(pos, gCamera.Pos);

	newvol = (float)volume - dist * 0.30f;

	volume = (int)newvol;
	if(volume <= 5)
		return;
	if(volume > soundvol.value) //MIX_MAX_VOLUME)
		volume = soundvol.value; //MIX_MAX_VOLUME;

	lb_PlaySound( SoundID, volume );

	//m_ConsPrint("play3D vol: %d\n", volume);

//	v1.X = 0;
//	v1.Y = 0;
//	v1.Z = 1;

	v2.X = pos.X - gCamera.Pos.X;
	v2.Y = pos.Y - gCamera.Pos.Y;
	v2.Z = pos.Z - gCamera.Pos.Z;

	//angle = PdtScalaire(v1, v2);

	angle = cam_RotationCycle( (float)acos(v2.Z) * INVPIOVER180 );


//	channel = Mix_PlayChannel(-1, gSoundObjet[SoundID].snd_chunk, 0);
//	Mix_SetPosition(channel, (int)angle, 0);
//	Mix_Volume(channel, volume);


/*
	if(SoundID >= MAX_SAMPLE)
		return;
	//if(!gSoundObjet[SoundID].bass_sample)
	//	return;

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
	
//	gSoundObjet[SoundID].bass_3Dpos.x = pos.X;
//	gSoundObjet[SoundID].bass_3Dpos.y = pos.Y;
//	gSoundObjet[SoundID].bass_3Dpos.z = pos.Z;

	// HACK
	if(ProgramState == PS_GREETINGS || ProgramState == PS_INTRO || ProgramState == PS_GAMEOVER)
	{

	}
	else
	{
		volume = 0;
	}
//	gSoundObjet[SoundID].channel3D[sound_pos] = BASS_SamplePlay3D(	gSoundObjet[SoundID].bass_sample,
//									&gSoundObjet[SoundID].bass_3Dpos,
//									NULL,
//									NULL);

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
	//BASS_ChannelSetAttributes(gSoundObjet[SoundID].channel3D[sound_pos],freq,0,-101);
        */
}

void lb_StopChannel3D(int idsound, int idchan)
{
	//BASS_ChannelStop(gSoundObjet[idsound].channel3D[idchan]);
	gSoundObjet[idsound].channel3D[idchan] = 0;
	gSoundObjet[idsound].time_chan3D[idchan] = 0;
}

void lb_UpdateSound3D(int volume)
{
	int	SoundID;
	vect_t	pos;
	int	sound_pos;
	hBool	apply = false;
	int	leaf_sound;
	int	pvs_index;
        
        return;

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

		//	if(BASS_ACTIVE_STOPPED == BASS_ChannelIsActive(gSoundObjet[SoundID].channel3D[sound_pos]))
		//	{
		//		lb_StopChannel3D(SoundID, sound_pos);
		//		continue;
		//	}

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
		//	gSoundObjet[SoundID].bass_3Dpos.x = pos.X;
		//	gSoundObjet[SoundID].bass_3Dpos.y = pos.Y;
		//	gSoundObjet[SoundID].bass_3Dpos.z = pos.Z;
			//BASS_ChannelSet3DPosition(gSoundObjet[SoundID].channel3D[sound_pos],&gSoundObjet[SoundID].bass_3Dpos,NULL,NULL);
//			BASS_ChannelSetAttributes(gSoundObjet[SoundID].channel3D[sound_pos],-1,volume,-101);
		}
	}
	if(apply)
	{
		//BASS_Apply3D();
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
		//	BASS_ChannelStop(gSoundObjet[SoundID].channel3D[sound_pos]);
		//	gSoundObjet[SoundID].channel3D[sound_pos] = 0;
			lb_StopChannel3D(SoundID, sound_pos);
		}
		gSoundObjet[SoundID].sound3D = false;
	}
}

void lb_StopMusic()
{
	if(gMusic)
	{
		gMusic->stopMusic();
	}

//	if(gHstream && BASS_ChannelIsActive(gHstream) == BASS_ACTIVE_PLAYING)
//	{
		//BASS_ChannelStop(gHstream);
//	}
}

hBool lb_PlayMP3(char *name, int volume)
{
	char	*p;
	
	lb_StopMusic();
	
	gMusic = new KMusic;

	// load the MUSIC file  to play as music
	p = KMiscTools::makeFilePath( name );
	
	if(!gMusic->playMusic(p))
	{
		delete gMusic;
		gMusic = NULL;
   		return false;
	}
	
	return true;
}

void lb_UpdateMP3()
{
	if(gMusic && gMusic->isMusicEnded() == true )	// gHstream && BASS_ChannelIsActive(gHstream) == BASS_ACTIVE_STOPPED)
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




