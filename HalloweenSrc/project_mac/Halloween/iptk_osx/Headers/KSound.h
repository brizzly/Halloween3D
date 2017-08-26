// Released under MIT License
/*
 Copyright (c) 2010 by Julien Meyer. Web: http://www.jmApp.com
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 */

#ifndef API_KSOUND__
#define API_KSOUND__

//#include "SoundEngine.h"
//#include "MyOpenALSupport.h"



#import <OpenAL/al.h>
#import <OpenAL/alc.h>
#import <AudioToolbox/AudioToolbox.h>
#import <AudioToolbox/ExtendedAudioFile.h>

enum {
	kSoundEngineErrUnitialized			= 1,
	kSoundEngineErrInvalidID			= 2,
	kSoundEngineErrFileNotFound			= 3,
	kSoundEngineErrInvalidFileFormat	= 4,
	kSoundEngineErrDeviceNotFound		= 5
};


enum EsoundType
{
	e_sample ,
	e_music ,
	e_undefined
};

class KSound
{
private:
	
	int						sample_id;
	EsoundType				type;
	static	long			_instanceCount;

	UInt32					soundID;
	ALuint					sourceID;
	ALuint					bufferID;	
	ALCcontext	*			mContext;
	ALCdevice	*			mDevice;
	
//	AVAudioPlayer *			audioPlayer;
	
	
public:
	
	KSound() ; 
	~KSound() ;
	
	void			initOpenAL();
	void			cleanUpOpenAL();
	UInt32			audioFileSize(AudioFileID fileDescriptor);
	AudioFileID		openAudioFile(char *filename);
	OSStatus		LoadFileDataInfo(const char *inFilePath, AudioFileID &outAFID, AudioStreamBasicDescription &outFormat, UInt32 &outDataSize);
	ALenum			GetALFormat(AudioStreamBasicDescription inFileFormat);

	bool	loadSample(const char * filename);
	void	playSample();	
	void	stopSample();
	void	setVolume(int volume);
	void	freeStream( void );
	void	freeSample( void );
	void	vibrate();

};

#endif // API_KSOUND__