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


#include "KSound.h"




long	KSound::_instanceCount = 0 ;


typedef ALvoid	AL_APIENTRY	(*alcMacOSXMixerOutputRateProcPtr) (const ALdouble value);
ALvoid  alcMacOSXMixerOutputRateProc(const ALdouble value)
{
	static	alcMacOSXMixerOutputRateProcPtr	proc = NULL;
    
    if (proc == NULL) {
        proc = (alcMacOSXMixerOutputRateProcPtr) alcGetProcAddress(NULL, (const ALCchar*) "alcMacOSXMixerOutputRate");
    }
    
    if (proc)
        proc(value);
	
    return;
}

void KSound::cleanUpOpenAL()
{
	alDeleteSources(1, &sourceID);
	alDeleteBuffers(1, &bufferID);
		
	// destroy the context
	alcMakeContextCurrent(NULL);
	if(mContext)
	{
	//	alcDestroyContext(mContext);
		mContext = NULL;
	}
		
	// close the device
	if(mDevice)
	{
		alcCloseDevice(mDevice);
		mDevice = NULL;
	}
}

AudioFileID KSound::openAudioFile(char *filename)
{
	AudioFileID outAFID;
	CFStringRef tStr = CFStringCreateWithCString(NULL, filename, kCFStringEncodingUTF8);
	CFURLRef fileURL = CFURLCreateWithFileSystemPath(NULL, tStr, kCFURLPOSIXPathStyle, true);
	
	// do some platform specific stuff..
#if TARGET_OS_IPHONE
	OSStatus result = AudioFileOpenURL(fileURL, kAudioFileReadPermission, 0, &outAFID);
#else
	OSStatus result = AudioFileOpenURL(fileURL, kAudioFileReadPermission/*fsRdPerm*/, 0, &outAFID);
#endif
	
	//ptro
	CFRelease(tStr);
	CFRelease(fileURL);
	
	if (result != 0) 
		printf("cannot openf file: %s", filename);
	return outAFID;
}

ALenum KSound::GetALFormat(AudioStreamBasicDescription inFileFormat)
{
	if (inFileFormat.mFormatID != kAudioFormatLinearPCM)
		return kSoundEngineErrInvalidFileFormat;
	
	if ((inFileFormat.mChannelsPerFrame > 2) || (inFileFormat.mChannelsPerFrame < 1))
		return kSoundEngineErrInvalidFileFormat;
	
	if(inFileFormat.mBitsPerChannel == 8)
		return (inFileFormat.mChannelsPerFrame == 1) ? AL_FORMAT_MONO8 : AL_FORMAT_STEREO8;
	else if(inFileFormat.mBitsPerChannel == 16)
		return (inFileFormat.mChannelsPerFrame == 1) ? AL_FORMAT_MONO16 : AL_FORMAT_STEREO16;
	
	return kSoundEngineErrInvalidFileFormat;
}

UInt32 KSound::audioFileSize(AudioFileID fileDescriptor)
{
	UInt64 outDataSize = 0;
	UInt32 thePropSize = sizeof(UInt64);
	OSStatus result = AudioFileGetProperty(fileDescriptor, kAudioFilePropertyAudioDataByteCount, &thePropSize, &outDataSize);
	if(result != 0) 
		printf("cannot find file size\n");
	return (UInt32)outDataSize;
}

OSStatus KSound::LoadFileDataInfo(const char *inFilePath, AudioFileID &outAFID, AudioStreamBasicDescription &outFormat, UInt32 &outDataSize)
{
	UInt32 thePropSize = sizeof(outFormat);				
	OSStatus result;
	//OSStatus result = OpenFile(inFilePath, outAFID);
	//printf("Error opening file", end);
	
	result = AudioFileGetProperty(outAFID, kAudioFilePropertyDataFormat, &thePropSize, &outFormat);
	if (result != 0)  {
		printf("Error getting file format\n");
		return result;
	}
	thePropSize = sizeof(UInt64);
	UInt64 outDataSize_ = 0;
	result = AudioFileGetProperty(outAFID, kAudioFilePropertyAudioDataByteCount, &thePropSize, &outDataSize_);
	if (result != 0)  {
		printf("Error getting file data size\n");
		return result;
	}
	outDataSize = (UInt32)outDataSize_;
	return result;
}

void KSound::initOpenAL()
{
	// Initialization
	mContext = NULL;
	
	mDevice = alcOpenDevice(NULL); // select the "preferred device"
	if (mDevice) {
		// use the device to make a context
		mContext=alcCreateContext(mDevice,NULL);
		
		// set my context to the currently active one
		alcMakeContextCurrent(mContext);
		
		// set the output rate value to match my sound sample rate
		//	alcMacOSXMixerOutputRateProc(22050);
		alcMacOSXMixerOutputRateProc(44100);
	}
}

KSound::KSound( )
{
	type = e_undefined ;
	sample_id = -1 ;

	if ( _instanceCount == 0 )
	{
		//SoundEngine_Initialize(44100);
		//SoundEngine_SetListenerPosition(0.0, 0.0, 1.0);
		initOpenAL();
	}
	
	_instanceCount++ ;
	//printf("%d\n",_instanceCount);
//	initOpenAL();
}

KSound::~KSound( )
{
	switch (type)
	{
		case e_sample:
			freeSample( ) ;
		break;
		case e_music:
			freeStream( ) ;
		break;
		
	}
	_instanceCount-- ;
	if ( _instanceCount <= 0 )
	{
		//SoundEngine_Teardown() ;
		cleanUpOpenAL();
	}
}

void	KSound::freeSample( void )
{
	if ( type == e_sample && sample_id != -1 )
	{
		//SoundEngine_UnloadEffect( sample_id ) ;
		type = e_undefined ;
	}
}

void	KSound::freeStream( void )
{
	if ( type == e_music )
	{
		//SoundEngine_UnloadBackgroundMusicTrack( ) ;
		type = e_undefined ;
	}
	
}

bool KSound::loadSample( const char * filename )
{
	printf("load %s\n", filename);
	
	bool loops = false;
	
	AudioStreamBasicDescription theFileFormat;	
	OSStatus result = noErr;
	AudioFileID fileID = openAudioFile((char*)filename);
	UInt32 fileSize = 0;// = audioFileSize(fileID);
	result = LoadFileDataInfo((char*)filename, fileID, theFileFormat, fileSize);
	if (result != 0) {
		printf("cannot load LoadFileDataInfo effect: %s\n", filename);
		return false;
	}
	
	// this is where the audio data will live for the moment
	unsigned char * outData = (unsigned char *)malloc(fileSize);
	
	// this where we actually get the bytes from the file and put them
	// into the data buffer
	result = AudioFileReadBytes(fileID, false, 0, &fileSize, outData);
	AudioFileClose(fileID); //close the file
	
	if (result != 0) {
		printf("cannot load effect: %s", filename);
		return false;
	}
	
	// grab a buffer ID from openAL
	alGenBuffers(1, &bufferID);
	
	// jam the audio data into the new buffer
	alBufferData(bufferID,GetALFormat(theFileFormat),outData,fileSize,theFileFormat.mSampleRate); 
		
	// grab a source ID from openAL
	alGenSources(1, &sourceID); 
	
	// attach the buffer to the source
	alSourcei(sourceID, AL_BUFFER, bufferID);
	
	// set some basic source prefs
	alSourcef(sourceID, AL_PITCH, 1.0f);
	alSourcef(sourceID, AL_GAIN, 0.5f);				//	volume half way
	if (loops) 
		alSourcei(sourceID, AL_LOOPING, AL_TRUE);
	
	// clean up the buffer
	if (outData)
	{
		free(outData);
		outData = NULL;
	}
	
	type = e_sample;
	
	return true;
}

void KSound::playSample()
{
	alSourcePlay(sourceID);
}

void KSound::stopSample()
{
	alSourceStop(sourceID);
}

void KSound::setVolume(int volume)
{
	alSourcef(sourceID, AL_GAIN, (float)volume / 100.0f);
}

void KSound::vibrate() 
{
	//AudioServicesPlaySystemSound(kSystemSoundID_Vibrate);
}
