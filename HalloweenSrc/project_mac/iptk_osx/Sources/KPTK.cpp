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


#include "KPTK.h"
#include "KGraphic.h"
#include "KGraphicGL.h"
#include "KGraphicGLLandscape.h"
#include "KLogFile.h"

#include "KWindow.h"

screenOrientation	KPTK::_screenOrientation;
int					KPTK::_window_width;
int					KPTK::_window_height;


KWindow * KPTK::createKWindow(screenOrientation orientation, int win_width, int win_height)
{
	_screenOrientation = orientation;
	_window_width = win_width;
	_window_height = win_height;
	KWindow *ptk_Window = new KWindow();
	return ptk_Window;
}

KGraphic * KPTK::createKGraphic()
{
	if ( _screenOrientation == K_MAC )
	{
		KGraphicGL  *kgl;
		kgl = new KGraphicGL(_window_width, _window_height);
		return kgl;
	}
	
	if ( _screenOrientation == K_320x480 )
	{
		KGraphicGL  *kgl ;
		kgl = new KGraphicGL(320, 480);
		return kgl ;
	}
	else if ( _screenOrientation == K_768x1024 )
	{
		KGraphicGL  *kgl ;
		kgl = new KGraphicGL(768, 1024);
		return kgl ;
	}
	
	if ( _screenOrientation == K_480x320 )
	{
		KGraphicGLLAND  *kgll ;
		kgll = new KGraphicGLLAND(480, 320);
		return kgll ;
	}
	else if( _screenOrientation == K_1024x768 )
	{
		KGraphicGLLAND  *kgll ;
		kgll = new KGraphicGLLAND(1024, 768);
		return kgll ;		
	}
	
	return 0;
}

/*KBatch * KPTK::createKBatch()
{
	if ( _screenOrientation == K_320x480 || _screenOrientation == K_768x1024 )
	{
		KBatch  *kgl ;
		kgl = new KBatch ;
		return kgl ;
	}
	
	if ( _screenOrientation == K_480x320 )
	{
		KBatch  *kgll ;
		kgll = new KBatch ;
		return kgll ;
	}
	return 0;
}*/

