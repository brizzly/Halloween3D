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

#include "KMusic.h"


// helper functions
double PrepareFileAU (CAAudioUnit &au, CAStreamBasicDescription &fileFormat, AudioFileID audioFile);
void MakeSimpleGraph (AUGraph &theGraph, CAAudioUnit &fileAU, CAStreamBasicDescription &fileFormat, AudioFileID audioFile);


//#import <UIKit/UIKit.h>
//#import <AVFoundation/AVFoundation.h>

id audioPlayer;
//AVAudioPlayer * audioPlayer = NULL;

KMusic::KMusic()
{
	theGraph = NULL;
	audioFile = NULL;
	_started = false;
}

KMusic::~KMusic()
{
}

AudioDeviceID KMusic::defaultOutputDeviceID()
{
	AudioDeviceID	outputDeviceID = kAudioObjectUnknown;
	
	// get output device device
	UInt32 propertySize = 0;
	OSStatus status = noErr;
	AudioObjectPropertyAddress propertyAOPA;
	propertyAOPA.mScope = kAudioObjectPropertyScopeGlobal;
	propertyAOPA.mElement = kAudioObjectPropertyElementMaster;
	propertyAOPA.mSelector = kAudioHardwarePropertyDefaultOutputDevice;
	
	if (!AudioHardwareServiceHasProperty(kAudioObjectSystemObject, &propertyAOPA))
	{
		NSLog(@"Cannot find default output device!");
		return outputDeviceID;
	}
	
	propertySize = sizeof(AudioDeviceID);
	
	status = AudioHardwareServiceGetPropertyData(kAudioObjectSystemObject, &propertyAOPA, 0, NULL, &propertySize, &outputDeviceID);
	
	if(status) 
	{
		NSLog(@"Cannot find default output device!");
	}
	return outputDeviceID;
}

float KMusic::getVolume()
{
	Float32			outputVolume;
	
	UInt32 propertySize = 0;
	OSStatus status = noErr;
	AudioObjectPropertyAddress propertyAOPA;
	propertyAOPA.mElement = kAudioObjectPropertyElementMaster;
	propertyAOPA.mSelector = kAudioHardwareServiceDeviceProperty_VirtualMasterVolume;
	propertyAOPA.mScope = kAudioDevicePropertyScopeOutput;
	
	AudioDeviceID outputDeviceID = KMusic::defaultOutputDeviceID();
	
	if (outputDeviceID == kAudioObjectUnknown)
	{
		NSLog(@"Unknown device");
		return 0.0;
	}
	
	if (!AudioHardwareServiceHasProperty(outputDeviceID, &propertyAOPA))
	{
		NSLog(@"No volume returned for device 0x%0x", outputDeviceID);
		return 0.0;
	}
	
	propertySize = sizeof(Float32);
	
	status = AudioHardwareServiceGetPropertyData(outputDeviceID, &propertyAOPA, 0, NULL, &propertySize, &outputVolume);
	
	if (status)
	{
		NSLog(@"No volume returned for device 0x%0x", outputDeviceID);
		return 0.0;
	}
	
	if (outputVolume < 0.0 || outputVolume > 1.0) return 0.0;
	
	return outputVolume;
}



// setting system volume
void KMusic::setVolume(float newVolume)
{
	if (newVolume < 0.0 || newVolume > 1.0)
	{
		//NSLog("Requested volume out of range (%.2f)", newVolume);
		return;
	}
	
	// get output device device
	UInt32 propertySize = 0;
	OSStatus status = noErr;
	AudioObjectPropertyAddress propertyAOPA;
	propertyAOPA.mElement = kAudioObjectPropertyElementMaster;
	propertyAOPA.mScope = kAudioDevicePropertyScopeOutput;
	
	if (newVolume < 0.001)
	{
		//NSLog("Requested mute");
		propertyAOPA.mSelector = kAudioDevicePropertyMute;
		
	}
	else
	{
		//NSLog("Requested volume %.2f", newVolume);
		propertyAOPA.mSelector = kAudioHardwareServiceDeviceProperty_VirtualMasterVolume;
	}
	
	AudioDeviceID outputDeviceID = KMusic::defaultOutputDeviceID();
	
	if (outputDeviceID == kAudioObjectUnknown)
	{
		NSLog(@"Unknown device");
		return;
	}
	
	if (!AudioHardwareServiceHasProperty(outputDeviceID, &propertyAOPA))
	{
		NSLog(@"Device 0x%0x does not support volume control", outputDeviceID);
		return;
	}
	
	Boolean canSetVolume = NO;
	
	status = AudioHardwareServiceIsPropertySettable(outputDeviceID, &propertyAOPA, &canSetVolume);
	
	if (status || canSetVolume == NO)
	{
		NSLog(@"Device 0x%0x does not support volume control", outputDeviceID);
		return;
	}
	
	if (propertyAOPA.mSelector == kAudioDevicePropertyMute)
	{
		propertySize = sizeof(UInt32);
		UInt32 mute = 1;
		status = AudioHardwareServiceSetPropertyData(outputDeviceID, &propertyAOPA, 0, NULL, propertySize, &mute);		
	}
	else
	{
		propertySize = sizeof(Float32);
		
		status = AudioHardwareServiceSetPropertyData(outputDeviceID, &propertyAOPA, 0, NULL, propertySize, &newVolume);	
		
		if (status)
		{
			NSLog(@"Unable to set volume for device 0x%0x", outputDeviceID);
		}
		
		// make sure we're not muted
		propertyAOPA.mSelector = kAudioDevicePropertyMute;
		propertySize = sizeof(UInt32);
		UInt32 mute = 0;
		
		if (!AudioHardwareServiceHasProperty(outputDeviceID, &propertyAOPA))
		{
			NSLog(@"Device 0x%0x does not support muting", outputDeviceID);
			return;
		}
		
		Boolean canSetMute = NO;
		
		status = AudioHardwareServiceIsPropertySettable(outputDeviceID, &propertyAOPA, &canSetMute);
		
		if (status || !canSetMute)
		{
			NSLog(@"Device 0x%0x does not support muting", outputDeviceID);
			return;
		}
		
		status = AudioHardwareServiceSetPropertyData(outputDeviceID, &propertyAOPA, 0, NULL, propertySize, &mute);
	}
	
	if (status)
	{
		NSLog(@"Unable to set volume for device 0x%0x", outputDeviceID);
	}
}

bool KMusic::playMusic(char * filename)
{	
	stopMusic();
	
	//return false;
	
	CFURLRef theURL = CFURLCreateFromFileSystemRepresentation(kCFAllocatorDefault, (UInt8*)filename, strlen(filename), false);
	
	printf("KMusic play = %s\n", filename);
	
	XThrowIfError (AudioFileOpenURL (theURL, kAudioFileReadPermission, 0, &audioFile), "AudioFileOpenURL");
	
	// get the number of channels of the file
	CAStreamBasicDescription fileFormat;
	UInt32 propsize = sizeof(CAStreamBasicDescription);
	XThrowIfError (AudioFileGetProperty(audioFile, kAudioFilePropertyDataFormat, &propsize, &fileFormat), "AudioFileGetProperty");
	
	printf ("playing file: %s\n", filename);
	printf ("format: "); fileFormat.Print();
	
	// lets set up our playing state now
	CAAudioUnit fileAU;
	
	
	// this makes the graph, the file AU and sets it all up for playing
	MakeSimpleGraph (theGraph, fileAU, fileFormat, audioFile);
	
	
	// now we load the file contents up for playback before we start playing
	// this has to be done the AU is initialized and anytime it is reset or uninitialized
	Float64 fileDuration = PrepareFileAU (fileAU, fileFormat, audioFile);
	printf ("file duration: %f secs\n", fileDuration);
	
	// start playing
	XThrowIfError (AUGraphStart (theGraph), "AUGraphStart");
/*	
	// sleep until the file is finished
	usleep ((int)(fileDuration * 1000. * 1000.));
	
	// lets clean up
	XThrowIfError (AUGraphStop (theGraph), "AUGraphStop");
	XThrowIfError (AUGraphUninitialize (theGraph), "AUGraphUninitialize");
	XThrowIfError (AudioFileClose (audioFile), "AudioFileClose");
	XThrowIfError (AUGraphClose (theGraph), "AUGraphClose");
*/
	
	/*
	if(audioPlayer != NULL) {
		return false;
	}
	NSString * fileStr = [NSString stringWithFormat:@"%s", filename];
	NSURL *url = [NSURL fileURLWithPath:fileStr];
	NSError *error;
//	audioPlayer = [[AVAudioPlayer alloc] initWithContentsOfURL:url error:&error];
//	audioPlayer.numberOfLoops = -1;
	
	if (audioPlayer == nil)
		NSLog(@"%@", [error description]);				
	else 
		[audioPlayer play];		
	 */
	
	_started = true;
	return true;
}

void KMusic::stopMusic()
{
	_started = false;
	
	if(theGraph && audioFile)
	{
		// lets clean up
		XThrowIfError (AUGraphStop (theGraph), "AUGraphStop");
		XThrowIfError (AUGraphUninitialize (theGraph), "AUGraphUninitialize");
		XThrowIfError (AudioFileClose (audioFile), "AudioFileClose");
		XThrowIfError (AUGraphClose (theGraph), "AUGraphClose");
		theGraph = NULL;
		audioFile = NULL;
	}
	
	/*if(audioPlayer) {
		[audioPlayer stop];
		[audioPlayer release];
		audioPlayer = nil;
	}*/
}

void KMusic::pauseMusic()
{
	if(audioPlayer) {
		[audioPlayer pause];
	}
}

void KMusic::resumeMusic()
{
	if(audioPlayer) {
//		[audioPlayer play];
	}
}

void KMusic::setVolume(int volume)
{
/*	if(audioPlayer) {
		audioPlayer.volume = ((float)volume / 100.0f);
	}*/
}


bool KMusic::isMusicEnded()
{
	if(audioPlayer == NULL) {
		return false;
	}
	if(_started == false) {
		return false;
	}
	/*
	//printf("audio %d %f / %f\n", (int)audioPlayer.playing, audioPlayer.currentTime, audioPlayer.duration);
	
	//if(audioPlayer.playing && audioPlayer.currentTime >= audioPlayer.duration) {
	if(audioPlayer.playing == false && audioPlayer.currentTime == 0 && audioPlayer.duration > 0)
	{
		if(audioPlayer.numberOfLoops == -1) {
			return false;
		}
		stopMusic();
		return true;
	}*/
	return false;
}




double PrepareFileAU (CAAudioUnit &au, CAStreamBasicDescription &fileFormat, AudioFileID audioFile)
{	
	// 
	// calculate the duration
	UInt64 nPackets;
	UInt32 propsize = sizeof(nPackets);
	XThrowIfError (AudioFileGetProperty(audioFile, kAudioFilePropertyAudioDataPacketCount, &propsize, &nPackets), "kAudioFilePropertyAudioDataPacketCount");
	
	Float64 fileDuration = (nPackets * fileFormat.mFramesPerPacket) / fileFormat.mSampleRate;
	
	ScheduledAudioFileRegion rgn;
	memset (&rgn.mTimeStamp, 0, sizeof(rgn.mTimeStamp));
	rgn.mTimeStamp.mFlags = kAudioTimeStampSampleTimeValid;
	rgn.mTimeStamp.mSampleTime = 0;
	rgn.mCompletionProc = NULL;
	rgn.mCompletionProcUserData = NULL;
	rgn.mAudioFile = audioFile;
	rgn.mLoopCount = -1; // 0 = do not loop, -1 = loop forever
	rgn.mStartFrame = 0;
	rgn.mFramesToPlay = UInt32(nPackets * fileFormat.mFramesPerPacket);
	
	// tell the file player AU to play all of the file
	XThrowIfError (au.SetProperty (kAudioUnitProperty_ScheduledFileRegion, 
								   kAudioUnitScope_Global, 0,&rgn, sizeof(rgn)), "kAudioUnitProperty_ScheduledFileRegion");
	
	// prime the fp AU with default values
	UInt32 defaultVal = 0;
	XThrowIfError (au.SetProperty (kAudioUnitProperty_ScheduledFilePrime, 
								   kAudioUnitScope_Global, 0, &defaultVal, sizeof(defaultVal)), "kAudioUnitProperty_ScheduledFilePrime");
	
	// tell the fp AU when to start playing (this ts is in the AU's render time stamps; -1 means next render cycle)
	AudioTimeStamp startTime;
	memset (&startTime, 0, sizeof(startTime));
	startTime.mFlags = kAudioTimeStampSampleTimeValid;
	startTime.mSampleTime = -1;
	XThrowIfError (au.SetProperty(kAudioUnitProperty_ScheduleStartTimeStamp, 
								  kAudioUnitScope_Global, 0, &startTime, sizeof(startTime)), "kAudioUnitProperty_ScheduleStartTimeStamp");
	
	return fileDuration;
}



void MakeSimpleGraph (AUGraph &theGraph, CAAudioUnit &fileAU, CAStreamBasicDescription &fileFormat, AudioFileID audioFile)
{
	XThrowIfError (NewAUGraph (&theGraph), "NewAUGraph");
	
	CAComponentDescription cd;
	
	// output node
	cd.componentType = kAudioUnitType_Output;
	cd.componentSubType = kAudioUnitSubType_DefaultOutput;
	cd.componentManufacturer = kAudioUnitManufacturer_Apple;
	
	AUNode outputNode;
	XThrowIfError (AUGraphAddNode (theGraph, &cd, &outputNode), "AUGraphAddNode");
	
	// file AU node
	AUNode fileNode;
	cd.componentType = kAudioUnitType_Generator;
	cd.componentSubType = kAudioUnitSubType_AudioFilePlayer;
	
	XThrowIfError (AUGraphAddNode (theGraph, &cd, &fileNode), "AUGraphAddNode");
	
	// connect & setup
	XThrowIfError (AUGraphOpen (theGraph), "AUGraphOpen");
	
	// install overload listener to detect when something is wrong
	AudioUnit anAU;
	XThrowIfError (AUGraphNodeInfo(theGraph, fileNode, NULL, &anAU), "AUGraphNodeInfo");
	
	fileAU = CAAudioUnit (fileNode, anAU);
	
	// prepare the file AU for playback
	// set its output channels
	XThrowIfError (fileAU.SetNumberChannels (kAudioUnitScope_Output, 0, fileFormat.NumberChannels()), "SetNumberChannels");
	
	// set the output sample rate of the file AU to be the same as the file:
	XThrowIfError (fileAU.SetSampleRate (kAudioUnitScope_Output, 0, fileFormat.mSampleRate), "SetSampleRate");
	
	// load in the file 
	XThrowIfError (fileAU.SetProperty(kAudioUnitProperty_ScheduledFileIDs, 
									  kAudioUnitScope_Global, 0, &audioFile, sizeof(audioFile)), "SetScheduleFile");
	

	XThrowIfError (AUGraphConnectNodeInput (theGraph, fileNode, 0, outputNode, 0), "AUGraphConnectNodeInput");
	
	// AT this point we make sure we have the file player AU initialized
	// this also propogates the output format of the AU to the output unit
	XThrowIfError (AUGraphInitialize (theGraph), "AUGraphInitialize");
	
	// workaround a race condition in the file player AU
	usleep (10 * 1000);
	
	// if we have a surround file, then we should try to tell the output AU what the order of the channels will be
	if (fileFormat.NumberChannels() > 2) {
		UInt32 layoutSize = 0;
		OSStatus err;
		XThrowIfError (err = AudioFileGetPropertyInfo (audioFile, kAudioFilePropertyChannelLayout, &layoutSize, NULL),
					   "kAudioFilePropertyChannelLayout");
		
		if (!err && layoutSize) {
			char* layout = new char[layoutSize];
			
			err = AudioFileGetProperty(audioFile, kAudioFilePropertyChannelLayout, &layoutSize, layout);
			XThrowIfError (err, "Get Layout From AudioFile");
			
			// ok, now get the output AU and set its layout
			XThrowIfError (AUGraphNodeInfo(theGraph, outputNode, NULL, &anAU), "AUGraphNodeInfo");
			
			err = AudioUnitSetProperty (anAU, kAudioUnitProperty_AudioChannelLayout, 
										kAudioUnitScope_Input, 0, layout, layoutSize);
			XThrowIfError (err, "kAudioUnitProperty_AudioChannelLayout");
			
			delete [] layout;
		}
	}
}



