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

#include "KLogFile.h"


#define MAX_PATH	256
#define	LOGFILENAME1	"/Users/julienmeyer/Desktop/iphonelog.txt"
#define	LOGFILENAME2	"/Users/sunhp/Desktop/iphonelog.txt"
#define NOLOG

FILE * KLogFile::f = NULL;


KLogFile::~KLogFile()
{
	Close();
}

void KLogFile::Init()
{
#ifdef NOLOG
	return;
#endif
	
	if(f == NULL) {
		f = fopen(LOGFILENAME1,"w");
		if(f == NULL) {
			f = fopen(LOGFILENAME2,"w");
		}
	}
}

void KLogFile::Close()
{
	if(f) {
		fclose(f);
		f = 0;
	}
}

void KLogFile::logDebug(const char *msg, ...)
{
#ifdef NOLOG
	return;
#endif
	
	Init();
	if(f == NULL) {
		return;
	}
	
	char buf[MAX_PATH];
	memset((void*)buf, 0, MAX_PATH);
	va_list args;
	va_start( args, msg );
	vsnprintf( buf, MAX_PATH-1, msg, args );
	va_end( args);
	
	fprintf(f, "%s\n", buf);	
	fflush(f);
	
}


