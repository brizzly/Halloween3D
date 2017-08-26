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

#ifndef PTK_KGRAPHICGLLAND__
#define PTK_KGRAPHICGLLAND__

//#include <OpenGL/OpenGL.h>
#include "KGraphic.h"



// opengl 1.2 ( standart is ms' opengl 1.1 !!! )
#ifndef GL_CLAMP_TO_EDGE  
#define GL_CLAMP_TO_EDGE					0x812F 
#endif

#ifndef GL_CLAMP_TO_BORDER
#define GL_CLAMP_TO_BORDER                0x812D
#endif



class KGraphicGLLAND : public KGraphic
{
	private:
		
		GLuint      		textureID;
		GLuint				pboId;
		static GLuint		_oldBindedText;
		int					_screenX, _screenY;
		float				_imageWidth;
		float				_imageHeight;
		float				_textureSize;
		bool				_textureQuality;
		bool				_activeck;
		unsigned char		_ckr;
		unsigned char		_ckg;
		unsigned char		_ckb;
		bool				_eyeRetina;
		bool				_isText;	
		float				_blitRed ,_blitGreen,_blitBlue;
		float				_blitAlpha;
		bool				_blitColorChanged;
		bool				_textureWrap;
		unsigned short 		_sourceAlpha, _destAlpha;

	
	private:
	
		void	GenericBlit( short x1 , short y1 , short x2 , short y2 , short destX , short destY , bool flipx , bool flipy )  ;


	public:

		KGraphicGLLAND(int _sx, int _sy);
		~KGraphicGLLAND();

		bool	loadWatermark();
		bool	loadPicture(const char *filename,bool u1,bool u2);
		bool	loadText(const char *text, const char *fontname, int fontsize);
		void	drawText(int x, int y, int alignmode, float angle, float zoom, float blend);
		void	blitAlphaRectFx(int x1, int y1, int x2, int y2, int destX, int destY, float angle, float zoom, float blend , bool flipx=false , bool flipy=false ) ;
		void	blitAlphaRect(int x1, int y1, int x2, int y2, int destX, int destY , bool flipx=false , bool flipy=false ) ;
		void	drawRect( short x1,short y1 , short x2 , short y2 , float r , float g , float b, float a ) ;
	
		void	freePicture( void ) ;
		void	setTextureQuality( bool quality ) ;
		void	setColorKey( bool activeck , unsigned char r,unsigned char g ,unsigned char b );

		void	setBlitColor( float r , float g , float b , float a ) ;
		void	setAlphaMode( long	alphamode ) ;
	
		float	getTextureWidth( void ) ;
		float	getTextureHeight( void ) ;
		float	getTextureSize( void ) ;

		void	drawLine( short x1,short y1 , short x2 , short y2 , float r , float g , float b, float asource, float adest ) ;
		void	setLineWidth( short lineWith );
		void	ptk_glBindTexture( void  );
		void	allowTextureWrap(bool texwRap);
		void	grabToTexture(KGraphic *g);

};

#endif // PTK_KGRAPHICGL

