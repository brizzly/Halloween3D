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
#include "GLTextureHelper.h"

GLuint		KGraphicGL::_oldBindedText =-1;

KGraphicGL::KGraphicGL( int _sx, int _sy )
{
	_oldBindedText = -1;
	_screenX = _sx;
	_screenY = _sy;
	textureID = 0;
	_imageWidth = 0;
	_imageHeight = 0;
	_activeck = false;
	_textureQuality = true;
	_blitColorChanged = false;	
	_textureWrap = false;
	_isText = false;
}

KGraphicGL::~KGraphicGL( )
{
	freePicture() ;
}

void KGraphicGL::freePicture( void )
{
	if ( _imageWidth > 0 )
	{
		_imageWidth = 0 ;
		glDeleteTextures( 1, &textureID );
		glDeleteBuffers(1, &pboId);
	}
}

bool KGraphicGL::loadWatermark() 
{
	_blitColorChanged = false ;
	_imageWidth = 0 ;
	_imageHeight = 0 ;
	
	glDisable(GL_DEPTH_TEST);
	
	glEnable(GL_TEXTURE_2D);							// Enable Texture Mapping
	glGenTextures(1, &textureID);
	glPixelStorei (GL_UNPACK_ALIGNMENT, 1);		// This sets the alignment requirements for the start of each pixel row in memory
	
	glBindTexture(GL_TEXTURE_2D, textureID);	// Bind the texture to the texture arrays index and init the texture
		
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); // GL_LINEAR   GL_NEAREST
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);	
		
	GLTextureHelper *helper = new GLTextureHelper();
	if(helper->LoadWatermark() == false) {
		delete helper ; // Correction PTRO
		return false;
	}
	
	_imageWidth	  = (float)helper->getWidth( ) ;
	_imageHeight  = (float)helper->getHeight( ) ;    
	_textureSize  = (float)helper->getTextureSize( ) ; 
	
	delete helper;
	helper = NULL;
	
	setAlphaMode(1);
	
	return true;
}

bool KGraphicGL::loadPicture(const char *filename ,bool u1, bool u2) 
{
	_blitColorChanged = false ;
	_imageWidth = 0;
	_imageHeight = 0;
		
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	glPixelStorei (GL_UNPACK_ALIGNMENT, 1);		// This sets the alignment requirements for the start of each pixel row in memory
	
	glEnable(GL_TEXTURE_2D);							// Enable Texture Mapping
	glGenTextures(1, &textureID);	
	glBindTexture(GL_TEXTURE_2D, textureID);	// Bind the texture to the texture arrays index and init the texture

	glGenBuffers(1, &pboId);
	glBindBuffer(GL_PIXEL_UNPACK_BUFFER, pboId);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); // GL_LINEAR   GL_NEAREST
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	GLTextureHelper *helper = new GLTextureHelper();
	if(helper->LoadTexture( filename, _activeck, _ckr, _ckg, _ckb ) == false) {
		delete helper ; // Correction PTRO
		return false;
	}
	
	_imageWidth		= (float)helper->getWidth();
	_imageHeight	= (float)helper->getHeight();   
	_textureSize	= (float)helper->getTextureSize();
	_eyeRetina		= (bool)helper->getEyeRetina();
		
	// We can delete the application copy of the texture data now
//	GLubyte * data = helper->getDatas();
//	free(data);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
	
	delete helper;
	helper = NULL;
	
	setAlphaMode( 1 );

	return true;
}

bool KGraphicGL::loadText(const char *text, const char *fontname, int fontsize)
{
	_blitColorChanged = false ;
	_imageWidth = 0 ;
	_imageHeight = 0 ;
	
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);							// Enable Texture Mapping
	glGenTextures(1, &textureID);
	glPixelStorei (GL_UNPACK_ALIGNMENT, 1);				// This sets the alignment requirements for the start of each pixel row in memory
	glBindTexture(GL_TEXTURE_2D, textureID);			// Bind the texture to the texture arrays index and init the texture
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // GL_NEAREST
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // GL_NEAREST
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);	
	
	GLTextureHelper *helper = new GLTextureHelper();
	if(helper->loadText(text, fontname, fontsize) == false) {
		return false;
	}
	
	_isText			= true;
	_imageWidth		= (float)helper->getWidth();
	_imageHeight	= (float)helper->getHeight();   
	_textureSize	= (float)helper->getTextureSize();
	_eyeRetina		= (bool)helper->getEyeRetina();
	
	delete helper;
	helper = NULL;
	setAlphaMode( 1 );
	return true;
}

void KGraphicGL::drawText(int x, int y, int alignmode, float angle, float zoom, float blend)
{
	if(_isText) {
		int alignedx = x;
		int w = this->getTextureWidth();
		if (_eyeRetina) {
			w /= 2;
		}
		if(alignmode == KGraphicTextAlignRight) {
			alignedx -= w;
		}
		else if(alignmode == KGraphicTextAlignCenter) {
			alignedx -= w / 2;			
		}
		this->blitAlphaRectFx(0, 0, this->getTextureWidth(), this->getTextureHeight(), alignedx, y, angle, zoom, blend);
	}
}

float KGraphicGL::getTextureWidth( void ) 
{
	return (float)_imageWidth;
}

float KGraphicGL::getTextureHeight( void ) 
{
	return (float)_imageHeight;
}

float KGraphicGL::getTextureSize( void ) 
{
	return (float)_textureSize;
}

void KGraphicGL::setLineWidth( short lineWidth )
{
	glLineWidth( lineWidth ) ;
}

void KGraphicGL::drawLine( short x1,short y1 , short x2 , short y2 , float r , float g , float b, float asource,float adest )
{
	glLoadIdentity();
	glOrtho(0, _screenX ,0, _screenY , -100 , 100 );
	
	
	glPushMatrix();         //sauvegarde la matrice en cours
	glShadeModel(GL_SMOOTH) ;
	
	//glColor4f( r,g,b,a ) ;
	
	glEnable(GL_COLOR_MATERIAL);
	glDisable( GL_TEXTURE_2D ) ;
 	
	glEnable( GL_BLEND ) ;
	glBlendFunc(  _sourceAlpha , _destAlpha  );
	
	//	GenericBlit( x1 , y1 , x2 , y2 , destX , destY , flipx , flipy ) ;
	
//	short dx1,dy1,dx2,dy2 ;
	GLshort w = _screenY-y1;
	GLshort h = _screenY-y2;
	GLshort	lineCoords[]=
	{
		x1,
		 w,
		x2,
		h  
	};
	GLfloat	fading[]=
	{
		r,g,b,asource,
		r,g,b,adest
	};

	glEnableClientState(GL_VERTEX_ARRAY);
	glDisableClientState( GL_TEXTURE_COORD_ARRAY );
	glEnableClientState( GL_COLOR_ARRAY );

	glDisable(GL_TEXTURE_2D );
	
	glColorPointer( 4, GL_FLOAT, 0, fading);		

	//vertice of the object on screen
	glVertexPointer(2, GL_SHORT, 0, lineCoords);
	
	glDrawArrays(GL_LINE_LOOP, 0, 2);
		
	glDisableClientState( GL_COLOR_ARRAY );

	
	glPopMatrix( ) ;
	glEnable( GL_TEXTURE_2D ) ;
	
	glColor4f( 1.0f,1.0f,1.0f,1.0f) ;
	glShadeModel(GL_FLAT);
}

void KGraphicGL::drawRect( short x1,short y1 , short x2 , short y2 , float r , float g , float b, float a )
{
	glLoadIdentity();
	glOrtho(0, _screenX ,0, _screenY , -100 , 100 );
	
	
	glPushMatrix();         //sauvegarde la matrice en cours
	glColor4f( r,g,b,a ) ;
		
	glEnable(GL_COLOR_MATERIAL);
	glDisable( GL_TEXTURE_2D ) ;
 	
	glEnable( GL_BLEND ) ;
	glBlendFunc(  _sourceAlpha , _destAlpha  );
	
//	GenericBlit( x1 , y1 , x2 , y2 , destX , destY , flipx , flipy ) ;
	
	short dx1,dy1,dx2,dy2;
	dx1 = x1 ;
	dx2 = x2 ;
	dy1 = _screenY-y1   ;
	dy2 = _screenY-y2  ;
	
	//our triangl fan
	GLshort triangleFan[] =
	{
		dx1,dy1,0,
		dx2,dy1,0,
		dx2,dy2,0,
		
		dx1,dy1,0,
		dx2,dy2,0,
		dx1,dy2,0
	};

	glEnableClientState(GL_VERTEX_ARRAY);
	glDisableClientState( GL_TEXTURE_COORD_ARRAY );
	glDisable(GL_TEXTURE_2D );
	glVertexPointer(3, GL_SHORT, 0, triangleFan);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glPopMatrix( ) ;
	glEnable( GL_TEXTURE_2D );
	glColor4f( 1.0f,1.0f,1.0f,1.0f) ;
}

void KGraphicGL::blitAlphaRect(int x1, int y1, int x2, int y2, int destX, int destY , bool flipx , bool flipy )
{	
	glLoadIdentity();
	glOrtho(0, _screenX ,0, _screenY , -100 , 100 );
	glPushMatrix();
	destY = _screenY-destY;
	if (_blitColorChanged == true) {
		glColor4f(_blitRed ,_blitGreen , _blitBlue , _blitAlpha );
	}
	else {
		glColor4f( 1.0f,1.0f,1.0f,1);
	}
	glEnable(GL_COLOR_MATERIAL);
	setTextureQuality(_textureQuality);
	glEnable( GL_BLEND ) ;
	glBlendFunc( _sourceAlpha, _destAlpha);
	GenericBlit(x1, y1, x2, y2, destX, destY, flipx, flipy);
	glPopMatrix();
	glColor4f(1.0f,1.0f,1.0f,1.0f);
}

void KGraphicGL::blitAlphaRectFx(int x1, int y1, int x2, int y2, int destX, int destY, float angle, float zoom, float blend , bool flipx , bool flipy )
{
	/*
	GLfloat _w = x2-x1;
	GLfloat _h = y2-y1;
	
	GLfloat dx1 = x1;
	GLfloat dy1 = _screenY - y1;
	GLfloat dx2 = _w + x1;
	GLfloat dy2 = _screenY - _h - y1;	
	
	if ( zoom == 0  )	return;
	if ( blend <= 0 )	return;
	
	GLfloat triangleFan[] =
	{
		dx1, dy1,
		dx2, dy1,
		dx2, dy2,
		
		dx1, dy1,
		dx2, dy2,
		dx1, dy2
	};
	
	GLfloat texcoords[] = 
	{
		0.0f,	0.0f,
		1.0f,	0.0f,
		1.0f,	1.0f,
		
		0.0f,	0.0f,
		1.0f,	1.0f,
		0.0f,	1.0f
	};
	
	float middlesizeX = (x2-x1)/2;
	float middlesizeY = (y2-y1)/2;

	glPushMatrix();
	
	if ( angle != 0 || zoom != 1 )
	{
		float tx = (float)destX+middlesizeX;
		float ty = (float)destY-middlesizeY;
		
		glTranslatef(tx, ty, 0);
		if ( angle != 0 ) {
			glRotatef( angle , 0.0f , 0.0f ,1.0f ) ;
		}		
		if ( zoom != 1 ) {
			glScalef( zoom ,zoom , 0 ) ;
		}
	}
	
	glEnable( GL_BLEND ) ;
	glBlendFunc(  _sourceAlpha , _destAlpha  );
	
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState( GL_TEXTURE_COORD_ARRAY );
	glEnable(GL_TEXTURE_2D );	
	ptk_glBindTexture();
	glTexCoordPointer(2, GL_FLOAT, 0, texcoords);
	glVertexPointer(2, GL_FLOAT, 0, triangleFan);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	
	glPopMatrix();
*/	

	glLoadIdentity();
	glOrtho(0, _screenX ,0, _screenY , -100 , 100 );
	
	if ( zoom == 0  )	return;
	if ( blend <= 0 )	return;
	
	float middlesizeX = (x2-x1)/2;
	float middlesizeY = (y2-y1)/2;
	
	glPushMatrix();
	destY = _screenY-destY ;
	if ( angle != 0 || zoom != 1 )
	{
		float tx = (float)destX+middlesizeX;
		float ty = (float)destY-middlesizeY;
		
		glTranslatef(tx, ty, 0);
		if ( angle != 0 ) {
			glRotatef( angle , 0.0f , 0.0f ,1.0f ) ;
		}		
		if ( zoom != 1 ) {
			glScalef( zoom ,zoom , 0 ) ;
		}
	}
	if ( _blitColorChanged == true ) {
		glColor4f( _blitRed ,_blitGreen , _blitBlue , _blitAlpha*blend  ) ;
	}
	else {
		glColor4f( 1.0f,1.0f,1.0f,blend) ;
	}
	glEnable(GL_COLOR_MATERIAL);
	setTextureQuality(_textureQuality);
	glEnable( GL_BLEND ) ;
	glBlendFunc(  _sourceAlpha , _destAlpha  );
	if ( angle != 0 || zoom != 1 ) {
		GenericBlit( x1 , y1 , x2 , y2 , -middlesizeX , middlesizeY , flipx , flipy ) ;
	}
	else {
		GenericBlit( x1 , y1 , x2 , y2 , destX , destY , flipx , flipy ) ;
	}
	glPopMatrix();
	glColor4f(1.0f,1.0f,1.0f,1.0f);
}

void KGraphicGL::setAlphaMode( long	alphamode )
{
	switch( alphamode) 
	{
    		//additive
		case	0:
	        _sourceAlpha 	=  GL_SRC_ALPHA ;
	        _destAlpha		=	GL_DST_ALPHA ;
    		break;
    		//normal
		case	1:
	        _sourceAlpha 	=  GL_SRC_ALPHA ;
	        _destAlpha		=	GL_ONE_MINUS_SRC_ALPHA ;
    		break ;
	 	case	2:
			_sourceAlpha 		=  GL_ONE  ;
			_destAlpha			=	GL_ONE_MINUS_SRC_ALPHA  ;
			break ;
	 	case	3:
			_sourceAlpha 		=  GL_ZERO  ;
			_destAlpha			=	GL_ONE_MINUS_SRC_ALPHA  ;
			break ;
		case	4:
	        _sourceAlpha 	=  GL_SRC_ALPHA ;
	        _destAlpha		=	GL_ONE ;
    		break ;
		case	5:
	        _sourceAlpha 	=  GL_ONE ;
	        _destAlpha		= GL_SRC_ALPHA ;
    		break ;		
		case	6:
	        _sourceAlpha 	=  GL_ONE ;
	        _destAlpha		= GL_ONE_MINUS_SRC_ALPHA;
    		break ;			
    }
}

void KGraphicGL::setBlitColor( float r , float g , float b , float a )
{
	if ( r == 1 && g == 1 && b == 1 &&a == 1 )
	{
		_blitColorChanged = false ;
		//return ;
	}
	
	if ( r >= 1 )		r = 1;
	if ( g >= 1 )		g = 1;
	if ( b >= 1 )		b = 1;
	if ( a >= 1 )		a = 1;
	if ( r <= 0 )		r = 0;
	if ( g <= 0 )		g = 0;
	if ( b <= 0 )		b = 0;
	if ( a <= 0 )		a = 0;
	
	_blitRed 		=  r;
	_blitGreen	 	= g;
	_blitBlue 		= b;
	_blitAlpha 		= a;
	_blitColorChanged = true;
}

void KGraphicGL::GenericBlit( short x1 , short y1 , short x2 , short y2 , short destX , short destY , bool flipx , bool flipy ) 
{
	float u_start, v_start;
	float u_end, v_end;
	float dx1, dy1, dx2, dy2;
	
	float fx1 = (float)x1;
	float fy1 = (float)y1;
	float fx2 = (float)x2;
	float fy2 = (float)y2;	
	
	//prevents a /0 divide operation
	if ( _textureSize <= 0 ) return ;
	
	//float tsize = _textureSize;
	
	ptk_glBindTexture();
	
	float xdiff = fx2-fx1;
	float ydiff = fy2-fy1;
	if(_eyeRetina) 
	{
		fx1 *= 2;
		fx2 *= 2;
		fy1 *= 2;
		fy2 *= 2;
	}
	dx1 = destX;
	dx2 = destX+xdiff;
	dy1 = destY;
	dy2 = destY-ydiff;
	
	if(flipx)
	{
		float dxtemp = dx1;
		dx1 = dx2;
		dx2 = dxtemp;
	}
	if(flipy)
	{
		float dytemp = dy1;
		dy1 = dy2;
		dy2 = dytemp;
	}
	
	//our triangl strip
	GLfloat triangleStrip[] =
	{
		dx2,dy1,
		dx2,dy2,
		dx1,dy1,
		dx1,dy2
	};
/*
	u_start = fx1 / _imageWidth;
	u_end = fx2 / _imageWidth;
	v_start = fy1 / _imageHeight;
	v_end = fy2 / _imageHeight;
*/
	
	u_start = (fx1+0.5f) / _imageWidth;
	u_end = (fx2-0.5f) / _imageWidth;
	v_start = (fy1+0.5f) / _imageHeight;
	v_end = (fy2-0.5f) / _imageHeight;
	
/*	
 http://stackoverflow.com/questions/7894802/opengl-texture-atlas-bleeding
	u0 = float(x+0.5f) / float(texWidth);
	v0 = float(y+0.5f) / float(texHeight);
	u1 = float(x+w-0.5f) / float(texWidth);
	v1 = float(y+h-0.5f) / float(texHeight);
	
*/
	
	GLfloat rectuv[] = { 
		u_end , v_start ,
		u_end , v_end ,
		u_start , v_start ,
		u_start, v_end
	};
	
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState( GL_TEXTURE_COORD_ARRAY );
		
	//vertice of the object on screen
	glVertexPointer(2, GL_FLOAT, 0, triangleStrip);
	
	//textures UV
	glTexCoordPointer( 2, GL_FLOAT, 0, rectuv );		
	
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

void KGraphicGL::ptk_glBindTexture(void  )
{
	if ( _oldBindedText != textureID )
	{
		glEnable( GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, textureID);
		_oldBindedText = textureID;
	}	
}

void KGraphicGL::setTextureQuality( bool quality )
{
	_textureQuality = quality;
	
	ptk_glBindTexture( ) ;
	/*
	if ( quality == true )
	{
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	}
	else
	{
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);		
	}*/
}

void KGraphicGL::setColorKey( bool activeck , unsigned char r,unsigned char g ,unsigned char b )
{
	_activeck = activeck ;
	_ckr  = r ;
	_ckg  = g ;
	_ckb  = b ;
} 


void KGraphicGL::allowTextureWrap(bool texwRap)
{
    _textureWrap = texwRap ;
    
    ptk_glBindTexture();
    
    if ( _textureWrap == false )
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,  GL_CLAMP_TO_EDGE);	
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,  GL_CLAMP_TO_EDGE);
    }
    else
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,  GL_REPEAT);	
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,  GL_REPEAT );
    }	
}

void KGraphicGL::grabToTexture(KGraphic *g)
{
	g->ptk_glBindTexture();
	glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 5, 5, 0, 0, 256, 256);	
}
