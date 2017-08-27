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
#include "KInput.h"
#include "KPTK.h"
#include "KJoypad.h"

#include <Carbon/Carbon.h>



int		KInput::KeyboardLayout[LAYOUTSIZE];
bool	KInput::_screenPressed = false;
int		KInput::_xScreenHit = 0;
int		KInput::_yScreenHit = 0;
int		KInput::_tapeCount = 0;
int		KInput::midX = 0;
int		KInput::midY = 0;
float	KInput::_xAcc = 0;
float	KInput::_yAcc = 0;
float	KInput::_zAcc = 0;
int		KInput::_screen_width = 0;
int		KInput::_screen_height = 0;
bool	KInput::_btPressed = ISUP;
bool	KInput::_leftMousePressed = ISUP;
bool	KInput::_rightMousePressed = ISUP;

float	KInput::_mx=0,KInput::_my=0;
sFinger	KInput::finger[2] = { 0,0,false ,0,0,false};


void KInput::initInput(int screen_width, int screen_height) 
{
	_screen_width = screen_width;
	_screen_height = screen_height;
	_screenPressed = false;
	_xScreenHit = _screen_width/2;
	for(int i=0 ; i<LAYOUTSIZE ; i++)
	{
		KeyboardLayout[i] = 0;
		//KeyStroke ks = KeyboardLayout[i];
		//ks.KeyCode = 0;
	}
	
	static KJoypad * joypad =  [[KJoypad alloc] init];
	[joypad initJoypad];
}

long KInput::readJoypad()
{
	long joyTouche = [KJoypad Get_PadStatus];
	return joyTouche;
}

void KInput::showPointer()
{
	[NSCursor unhide];
}
 
void KInput::hidePointer()
{
	[NSCursor hide];
}

int KInput::getScreenWidth()
{
	return _screen_width;
}

int KInput::getScreenHeight()
{
	return _screen_height;
}

sFinger	*KInput::getFinger( short fingerid )
{
	if ( fingerid >1 ) return 0 ;
	return &finger[fingerid] ;
	
}

void KInput::setFingerPosition( int fingerid , short	x , short y , bool pressed ) 
{
	if( fingerid > 1 ) return;

	finger[fingerid].pressed = pressed ;

	if( KPTK::_screenOrientation == K_320x480 || KPTK::_screenOrientation == K_768x1024) {
		finger[fingerid].x = x ;
	}
	else if(KPTK::_screenOrientation == K_480x320) {
		finger[fingerid].x = 480-y;
	}	
	else if(KPTK::_screenOrientation == K_1024x768) {
		finger[fingerid].x = 1024-y;
	}	
	
	if( KPTK::_screenOrientation == K_320x480 || KPTK::_screenOrientation == K_768x1024) {
		finger[fingerid].y = y;
	}
	else {
		finger[fingerid].y = x;
	}
}

void KInput::swapFingerPosition(int fingerid1, int fingerid2) 
{
	if( fingerid1 > 1 ) return;
	if( fingerid2 > 1 ) return;
	
	short x_ = finger[fingerid1].x;
	finger[fingerid1].x = finger[fingerid2].x;
	finger[fingerid2].x = x_;
	
	short y_ = finger[fingerid1].y;
	finger[fingerid1].y = finger[fingerid2].y;
	finger[fingerid2].y = y_;
	
	bool p_ = finger[fingerid1].pressed;
	finger[fingerid1].pressed = finger[fingerid2].pressed;
	finger[fingerid2].pressed = p_;
}

void KInput::setKeyPressed(int keycode)
{
	if(keycode < 0 || keycode >= LAYOUTSIZE)  {
		return;
	}
	KeyboardLayout[keycode] = 1;
	printf("KeyboardLayout[%d] = 1;\n", keycode);

//	KeyStroke ks = KeyboardLayout[keycode];
//	ks.KeyCode = keycode;
}

void KInput::setKeyUnPressed(int keycode)
{
	if(keycode < 0 || keycode >= LAYOUTSIZE) {
		return;
	}
	KeyboardLayout[keycode] = 0;
	printf("KeyboardLayout[%d] = 0;\n", keycode);
}


bool KInput::isPressed(int keyCode)
{
	if(KeyboardLayout[keyCode] == 1) {
		return true;
	}
	//unsigned short k = KeyboardLayout[keyCode].KeyCode;
	//printf("isPressed %d", (int)k);
	return false;
	//return _btPressed;
	/*
	if(keyCode == K_VK_ESCAPE) {
		return ISUP;
	}
	return _screenPressed; //ISUP;
	*/
}

void KInput::mousePointerTo(int x, int y)
{
//	KLogFile::logDebug("mousePointerTo: %d %d", x, y);
	midX = x;
	midY = y;
	
	_mx = x;
	_my = y;
	
	CGPoint pt;
	pt.x = ( float )x;
	pt.y = ( float )y;
	
	CGWarpMouseCursorPosition(  pt ) ;
	CGSetLocalEventsSuppressionInterval( 0.0 );
/*	
	CGEventSourceRef event_source_ref = 
	CGEventSourceCreate(kCGEventSourceStateCombinedSessionState); 
	CGEventSourceSetLocalEventsSuppressionInterval(event_source_ref, 0.0); 
	CFRelease(event_source_ref); 
 */
}

void KInput::setMousePos(float valX, float valY)
{
	_mx = valX;
	_my = valY;
}

void KInput::setMouseX(float val)
{
	_mx = val;
}

void KInput::setMouseY(float val)
{
	_my = val;
}

int KInput::getMouseX()
{
	return (int)_mx;
}

int KInput::getMouseY()
{
	return (int)_my;
}

bool KInput::getLeftButtonState()
{
	return _leftMousePressed;
}

bool KInput::getRightButtonState()
{
	return _rightMousePressed;
}

void KInput::setLeftButtonState(bool value)
{
	_leftMousePressed = value;
}

void KInput::setRightButtonState(bool value)
{
	_rightMousePressed = value;
}

bool KInput::getMiddleButtonState()
{
	return false;
}


// iphone specific events calls

float KInput::getAccelerationX()
{
	return _xAcc;
}

float KInput::getAccelerationY()
{
	return _yAcc;
}

float KInput::getAccelerationZ()
{
	return _zAcc;
}

void KInput::setAccelerometer(float x, float y, float z)
{
	//printf("%f %f %f\n", x, y, z);
	_xAcc = x;
	_yAcc = y;
	_zAcc = z;
}

int KInput::getScreenTap()
{
	return _tapeCount;
}

bool KInput::isScreenPressed(int *hitX, int *hitY)
{
	if(_screenPressed == false) {
		_btPressed = ISUP ;
		return false;
	}
	
	_btPressed = ISDOWN ;
	
    if(hitX) {
		*hitX = KInput::getMouseX();
	}
	if(hitY) {
		*hitY = KInput::getMouseY();
	}
	//KLogFile::logDebug("setScreenMoving: %d %d", *hitX, *hitY);
	return true;
}

bool KInput::isScreenReleased(int *hitX, int *hitY)
{
	if(_screenPressed == true) {
		return false;
	}
    if(hitX) {
		*hitX = KInput::getMouseX();
	}
	if(hitY) {
		*hitY = KInput::getMouseY();
	}	
	return true;
}

void KInput::setScreenPressed(int tapCount, int x, int y)
{
	//printf("setScreenPressed: %d %d\n", x, y);
	_btPressed = ISDOWN ;
	_screenPressed = true;
	_xScreenHit = x;
	_yScreenHit = y;
	_tapeCount = tapCount;
}

void KInput::setScreenMoving(int x, int y)
{
	//printf("setScreenMoving: %d %d\n", x, y);
	_xScreenHit = x;
	_yScreenHit = y;
}

void KInput::setScreenReleased()
{
	//printf("setScreenReleased\n");
	_screenPressed = false;
	_btPressed = ISUP ;
	//_xScreenHit = -1;
	//_yScreenHit = -1;
	_tapeCount = 0;
}








