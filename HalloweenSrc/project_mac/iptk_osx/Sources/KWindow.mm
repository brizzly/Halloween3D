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

#include "KGraphicGL.h"
#include "KGraphicGLLandscape.h"
#include "KWindow.h"


KWindow::KWindow()
{
	setClearColor( 1,1,1,1 ) ;
	_terminated = false ;
	
}

void KWindow::setClearColor( float r, float g , float b, float a ) 
{
	_red = r ;
	_green = g ;
	_blue = b ;
	_a = a ;
}


bool KWindow::createGameWindow(int w, int h, int bpp, bool windowed, char *name)
{
	window_width = w;
	window_height = h;
	window_windowed = windowed;

	/*
	float version = [[[UIDevice currentDevice] systemVersion] floatValue];
	if(version >= 4.0) 
	{
		window_width *= 2;
		window_height *= 2;
	}*/		
	return true;
}

void KWindow::setTitle(char *title)
{
}

int KWindow::getWindowWidth()
{
	return window_width;
}

int KWindow::getWindowHeight()
{
	return window_height;
}

bool KWindow::getWindowed()
{
	return window_windowed;
}

int KWindow::getWindowHandle()
{
	return 0;
}

void KWindow::setMaxFrameRate(int maxFps)
{

}

void KWindow::terminate( void ) 
{
	_terminated = true ;
}

void KWindow::setWorldView(float tranX , float transY , float angle, float zoom, bool clearworld )
{
	/*glDisable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);	
	if ( clearworld == true )
	{
		glClearColor( _red , _green , _blue , _a) ;
		glClear(GL_COLOR_BUFFER_BIT);	
	}
	glLoadIdentity();
	//glOrthof(0, 320 ,0, 480 , -100 , 100 );
	glOrthof(0, window_width ,0, window_height , -100 , 100 );*/
}

void KWindow::setDefaultWorldView()
{
	glClearColor( _red, _green, _blue, _a );
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);	
	//glClear(GL_COLOR_BUFFER_BIT);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	glOrtho(0, window_width ,0, window_height , -100 , 100 );
}

void KWindow::flipBackBuffer()
{
	//swapBuffers();
}

bool KWindow::isQuit()
{
	return _terminated ;
}


