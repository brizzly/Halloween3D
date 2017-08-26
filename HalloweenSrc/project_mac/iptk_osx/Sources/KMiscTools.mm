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

#include "KMiscTools.h"
#include "Foundation/Foundation.h"
#include <assert.h>
//#include <CoreServices/CoreServices.h>
#include <mach/mach.h>
#include <mach/mach_time.h>
#include <unistd.h>
//#include "ddcodeAppDelegate.h"

char	KMiscTools::applicationName[MAX_PATH];

char	KMiscTools::currentAppPath[MAX_PATH];
char	KMiscTools::currentFilePath[MAX_PATH];
char	KMiscTools::currentTempPath[MAX_PATH];
char	KMiscTools::currentDocHomePath[MAX_PATH];
bool	KMiscTools::_inPackage;

char	KMiscTools::currentPath[MAX_PATH*2];

uint64_t 	t0;		// Start time for getMillisecond()
static mach_timebase_info_data_t timebase;


bool KMiscTools::iPad()
{
	return false; //( UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad );
}


//#################################################################################	
static inline unsigned int getCString(NSString * str, char * cstr, unsigned int bufSize)
{
    const char * utf8 = [str UTF8String];
	unsigned int size = [str length];
	if ( size > bufSize )
		size = bufSize-1;
    memcpy(cstr, utf8, size);
	return size;
}

//#################################################################################	
unsigned long KMiscTools::getMilliseconds( void )
{
	return (unsigned long)(((float)(mach_absolute_time() - t0)) * ((float)timebase.numer) / ((float)timebase.denom) / 1000000.0f);
}

//#################################################################################	
long KMiscTools::getSeed( void )
{
	return (long)mach_absolute_time();
}

//#################################################################################	
void KMiscTools::getSerial( char *s )
{
	memset(s, 0, 45);
//	NSString *uniqueIdentifier;
	/*
	UIDevice *myCurrentDevice = [UIDevice currentDevice];
	uniqueIdentifier = [myCurrentDevice uniqueIdentifier];
	getCString(uniqueIdentifier, s, 40);
	//[myCurrentDevice release];
	//[uniqueIdentifier release];
	 */
}


//#################################################################################	
void KMiscTools::initMiscTools( const char *appName ) 
{
	memset(applicationName, sizeof(char*), MAX_PATH);
	strcpy(applicationName, appName);

	_inPackage = true;
	
	t0 = mach_absolute_time();			// Init hires timer
	mach_timebase_info(&timebase);	// Get mach timbase info to get real time

	memset(currentAppPath, 0, MAX_PATH);
	getAppHome(currentAppPath, MAX_PATH);

	memset(currentFilePath, 0, MAX_PATH);
	getUserHome(currentFilePath, MAX_PATH);
	
	memset(currentTempPath, 0, MAX_PATH);
	getTempHome(currentTempPath, MAX_PATH);
	
	memset(currentDocHomePath, 0, MAX_PATH);
	getDocHome(currentDocHomePath, MAX_PATH);
}
/*
 // find the path to ~/Library/Applicaiton Support/
 NSString *applicationSupportFolder =
 [[NSFileManager defaultManager] findSystemFolderType:kApplicationSupportFolderType forDomain:kUserDomain];
 
 // find /Library/Application Support/  <--- not sure if I can just assume it's always at that path
 NSString *applicationSupportFolder =
 [[NSFileManager defaultManager] findSystemFolderType:kApplicationSupportFolderType forDomain:kOnSystemDisk];
 */

const char * KMiscTools::findSystemFolderType(int folderType, int domain)
{
    FSRef folder;
    OSErr err = noErr;
    CFURLRef url;
    NSString *result = nil;
	
    err = FSFindFolder(domain, folderType, kCreateFolder, &folder);
    if (err == noErr) {
        url = CFURLCreateFromFSRef(kCFAllocatorDefault, &folder);
		result = [(__bridge NSURL *)url path];
    }
	const char* cString = [result cStringUsingEncoding:NSASCIIStringEncoding]; 
    return cString;
}


//#################################################################################	

char * KMiscTools::makeHomeDocFilePath(const char *filename)
{
	NSFileManager *fileManager = [NSFileManager defaultManager];	
	NSString * folder = [[NSString alloc] initWithFormat:@"~/Library/Application Support/%s/", applicationName];
	folder = [folder stringByExpandingTildeInPath];
	if ([fileManager fileExistsAtPath: folder] == NO)
	{
		[fileManager createDirectoryAtPath: folder attributes: nil];
	}
	NSString * fullPath = [NSString stringWithFormat:@"%s/%s", folder, filename];	
	const char* cString = [fullPath cStringUsingEncoding:NSASCIIStringEncoding]; 
	return (char *)cString;
	
	/*
	const char *applicationSupportFolder =	findSystemFolderType(kApplicationSupportFolderType, kUserDomain);
	snprintf(builtPath, MAX_PATH, "%s/%s/%s", applicationSupportFolder, applicationName, filename);
	
	OSErr err = FSFindFolder(builtPath, folderType, kCreateFolder, &folder);
    if (err == noErr) {
        url = CFURLCreateFromFSRef(kCFAllocatorDefault, &folder);
        result = [(NSURL *)url path];
    }
	const char* cString = [result cStringUsingEncoding:NSASCIIStringEncoding]; 

	return	builtPath;*/
/*	
	long		i =0 ;
	long		index ;
	char *builtPath = currentPath; //new char [MAX_PATH*2]; //[MAX_PATH*];
	memset(builtPath, 0, MAX_PATH*2);
	if ( filename[0] == '\\' )	
		filename++ ;
	if( filename[0] == '/' ) {
		snprintf( builtPath, MAX_PATH, "%s", filename );		
	}
	else {	
		snprintf(builtPath, MAX_PATH, "%s/%s", currentDocHomePath, filename);
	}
	//pour mac, conversion de fichier en :
	i = strlen(builtPath ) ;
	for ( index = 0 ; index <  i ; index++ ) 
	{
		if ( builtPath[index] == '\\' )		
			builtPath[index] = '/' ;			
	}	
	return	builtPath;	*/
}

char * KMiscTools::makeTempFilePath(const	char *filename)
{
	NSFileManager *fileManager = [NSFileManager defaultManager];	
	NSString * folder = [[NSString alloc] initWithFormat:@"~/Library/Application Support/%s/", applicationName];
	NSString * fullPath = [[NSString alloc] initWithFormat:@"%@%s", folder, filename];	
	folder = [folder stringByExpandingTildeInPath];
	if ([fileManager fileExistsAtPath: folder] == NO)
	{
		[fileManager createDirectoryAtPath: folder attributes: nil];
	}
	const char *applicationSupportFolder =	findSystemFolderType(kApplicationSupportFolderType, kUserDomain);
	char *builtPath = new char [MAX_PATH];
	memset(builtPath, 0, MAX_PATH);
	snprintf(builtPath, MAX_PATH, "%s/%s/%s", applicationSupportFolder, applicationName, filename);
	return	builtPath;
	
//	const char* cString = [fullPath cStringUsingEncoding:NSASCIIStringEncoding]; 
//	return (char *)cString;
	/*
	char *builtPath = new char [MAX_PATH];
	memset(builtPath, 0, MAX_PATH);
	const char *applicationSupportFolder =	findSystemFolderType(kApplicationSupportFolderType, kUserDomain);
	snprintf(builtPath, MAX_PATH, "%s/%s/%s", applicationSupportFolder, applicationName, filename);
	return	builtPath;*/
/*
	long		i =0 ;
	long		index ;
	char *builtPath = currentPath; //new char [MAX_PATH*2]; //[MAX_PATH*];
	memset(builtPath, 0, MAX_PATH*2);
	if ( filename[0] == '\\' )	
		filename++ ;
	if( filename[0] == '/' ) {
		snprintf( builtPath, MAX_PATH, "%s", filename );		
	}
	else {	
		snprintf(builtPath, MAX_PATH, "%s/%s", currentTempPath, filename);
	}
	//pour mac, conversion de fichier en :
	i = strlen(builtPath ) ;
	for ( index = 0 ; index <  i ; index++ ) 
	{
		if ( builtPath[index] == '\\' )		
			builtPath[index] = '/' ;			
	}	
	printf("%s\n", builtPath);
	return	builtPath;*/
}

char * KMiscTools::makeFilePath(const	char *filename)
{
	long		i =0 ;
	long		index ;
	char *builtPath = currentPath; //new char [MAX_PATH*2]; //[MAX_PATH*];
	memset(builtPath, 0, MAX_PATH*2);
	if ( filename[0] == '\\' )	
		filename++ ;
	if( filename[0] == '/' ) 
	{
		snprintf( builtPath, MAX_PATH, "%s", filename );		
	}
	else {	
		//snprintf(builtPath, MAX_PATH, "%s/%s", currentAppPath, filename);
		snprintf(builtPath, MAX_PATH, "%s/Contents/Resources/%s", currentAppPath, filename);
		//snprintf( builtPath, MAX_PATH , "%s/Contents/Resources" ,currentAppPath ) ;	
	}
	//pour mac, conversion de fichier en :
	i = strlen(builtPath ) ;
	for ( index = 0 ; index <  i ; index++ ) 
	{
		if ( builtPath[index] == '\\' )		
			builtPath[index] = '/' ;			
	}	
	return	builtPath;
}



//#################################################################################	
int KMiscTools::getCurrentDir(char * path, unsigned int maxLen)
{
   NSString * cur = [[NSFileManager defaultManager] currentDirectoryPath];
   return getCString(cur, path, maxLen);    
}



//#################################################################################	
int KMiscTools::getUserHome(char * path, unsigned int maxLen)
{
    NSString * home = NSHomeDirectory();
	return getCString(home, path, maxLen);
}



//#################################################################################	
int KMiscTools::getDocHome(char * path, unsigned int maxLen)
{
		NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES); //creates paths so that you can pull the app's path from it
		NSString *doc = [paths objectAtIndex:0]; //gets the applications directory on the users iPhone
   // NSString * doc = NSDocumentDirectory();
	return getCString(doc, path, maxLen);
}



//#################################################################################	
int KMiscTools::getTempHome(char * path, unsigned int maxLen)
{
    NSString * home = NSTemporaryDirectory();
	return getCString(home, path, maxLen);
}



//#################################################################################	
int KMiscTools::getAppHome(char * path, unsigned int maxLen)
{
    NSBundle * mb = [NSBundle mainBundle];
	NSString * mbp = [mb bundlePath];
	return getCString(mbp, path, maxLen);
}


//#################################################################################	
unsigned short KMiscTools::pathSeparator(void)
{
    return '/';
}


//#################################################################################	
void KMiscTools::appendPath(char * path, char * component)
{
    unsigned int len = strlen(path);
	if ( path[len] != pathSeparator() )
	{
		path[len] = (char) pathSeparator();
		path[len+1] = 0;
	}
    char * p = component;
	if ( *p == pathSeparator() ) p++;
	strcat(path, p);
}


//#################################################################################	
char * KMiscTools::concatePath(char * path1, char * path2)
{
     size_t len1 = strlen(path1);
	 size_t len2 = strlen(path2);
     char * resultPath = (char * ) malloc( len1 + len2 + 1 );
	 strcpy(resultPath, path1);
	 
	 appendPath(resultPath, path2);
	 
	 return resultPath;	 
}

