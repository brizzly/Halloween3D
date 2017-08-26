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

#ifndef API_KGRAPHIC__
#define API_KGRAPHIC__

#include <OpenGL/OpenGL.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>


enum {
	KGraphicTextAlignLeft = 0,
	KGraphicTextAlignCenter,
	KGraphicTextAlignRight
};

class KGraphic
{

public:
	
	KGraphic() {} ;
	virtual	~KGraphic()  {} ;
	

	virtual	bool	loadWatermark()=0;
	virtual	bool	loadPicture(const char *filename , bool  unused=false , bool unused2=false) = 0;
	virtual	bool	loadText(const char *text, const char *fontname, int fontsize) = 0;
	virtual void	drawText(int x, int y, int alignmode, float angle, float zoom, float blend) = 0;

	virtual	void	blitAlphaRectFx(int x1, int y1, int x2, int y2, int dx, int dy, float rotate, float zoom, float alpha , bool flipx=false , bool flipy=false)=0;	
	virtual	void	blitAlphaRect(int x1, int y1, int x2, int y2, int dx, int dy , bool flipx=false , bool flipy=false)=0;	
	virtual	void	drawRect( short x1,short y1 , short x2 , short y2 , float r , float g , float b, float a ) =0 ;

	virtual	void	setBlitColor( float r , float g , float b , float a ) =0;
	virtual	void	setAlphaMode( long	alphamode ) = 0 ;
	virtual	void	setColorKey( bool activeck , unsigned char r,unsigned char g ,unsigned char b ) = 0;
	virtual	void	setTextureQuality( bool quality ) = 0;
	virtual	float	getTextureSize( void ) = 0 ;
	virtual	float	getTextureWidth( void ) = 0 ;
	virtual	float	getTextureHeight( void ) = 0 ;
	
	virtual	void	drawLine( short x1,short y1 , short x2 , short y2 , float r , float g , float b, float asource, float adest ) = 0 ;
	virtual	void	setLineWidth( short lineWidth ) = 0;
	virtual	void	ptk_glBindTexture( void ) = 0 ;
	
	virtual	void	grabToTexture(KGraphic *g) = 0;

	
};


#endif // API_KGRAPHIC__
