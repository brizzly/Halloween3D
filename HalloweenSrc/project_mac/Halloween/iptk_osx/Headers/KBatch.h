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

#ifndef		DEF_KBATCHGL
#define		DEF_KBATCHGL

#include <OpenGL/OpenGL.h>
#include <math.h>
#include "KGraphicGL.h"
#include "GLTextureHelper.h"





class	 KBatch
{
private:
	GLshort			*quadPtr ;
	GLfloat			*texuvPtr ;
	GLfloat			*texColor ;  //couleur des blits
	
	long			quadCount ;
	
	void							freeBuffer( void ) ;
	long							_currentQuad ;
	float							_textureSize ;
	float							_r,_g,_b,_a ;
	unsigned short 		_sourceAlpha   , _destAlpha ;
	long							_curColorIndex ;

	float							cosr_lup[360*8];
	float							sinr_lup[360*8];
	
public:

	KBatch( ) ;
	~KBatch( ) ;
	
	KGraphic						*_texturePtr ;
	
	bool		allocateBuffer( long polygoncount ) ;
	void		beginBatch( KGraphic	*surface ) ;

	void		endBatch( void ) ;
	
	
	//blit used for all the function , acts the same way as KGraphic::blitAlphaRect
	void		blitAlphaRect(long x1, long y1, long x2, long y2, float destX, float destY ) ;
	void		blitAlphaRectFx( float	x1, float y1 , float	x2 , float y2 , short	destX , short	destY, float angle, float zoom, float blend  ) ;

	void		drawRect( float	x1, float y1 , float	x2 , float y2 , float r ,float g ,float b ,  float blend ) ;
	void		setBlitColor( float r , float g , float b , float a ) ;
	void		setAlphaMode( long	alphamode ) ;

};

#endif
