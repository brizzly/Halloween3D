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

#ifndef API_KWINDOW__
#define API_KWINDOW__

#include <stdio.h>
#include <stdlib.h>
#include <OpenGL/OpenGL.h>
#include <OpenGL/gl.h>


class KWindow
{
private:
	int		window_width;
	int		window_height;
	bool	window_windowed;
	float  _red,_green , _blue , _a ;
	bool	_terminated ;
	
public:
		
	KWindow();
		
	bool	createGameWindow(int w, int h, int bpp, bool windowed, char *name);
	void	setTitle(char *title);
	int		getWindowWidth();
	int		getWindowHeight();
	bool	getWindowed();
	int		getWindowHandle();
	void	setMaxFrameRate(int maxFps);
	void	setDefaultWorldView();
	void	flipBackBuffer();
	bool	isQuit();
	void	setClearColor( float r, float g , float b, float a ) ;
	
	void	setWorldView(float tranX , float transY , float angle, float zoom, bool clearworld ) ;
	void	terminate( void ) ;

	
};

#endif // API_KWINDOW__

