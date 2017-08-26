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

#include <OpenGL/OpenGL.h>
#include "KBatch.h"


#ifndef	KPI
 #define	KPI		3.14159265358979323846f
 #define	K2PI	2*KPI
#endif



KBatch::KBatch( ) 
{
	quadPtr = NULL ;
	texuvPtr = NULL ;
	texColor = NULL ;
	_curColorIndex = 0 ;
	quadCount = 0 ;
	setBlitColor( 1,1,1,1 );

	for(short i=0; i<(360*8); i++)
	{
		float angr=	( K2PI*((float)(0-i)/8.0f))/360.0f;
		cosr_lup[i]=cos(angr);
		sinr_lup[i]=sin(angr);
	}
}

KBatch::~KBatch( ) 
{
	freeBuffer( ) ;

}

void KBatch::freeBuffer( void ) 
{
	if ( texColor != NULL )
	{
		delete	[] texColor ; // Correction PTRO (manquait le [], ca fait bien exploser ca)
		texColor = NULL ;
	}
	
	if ( quadPtr != NULL )
	{
		delete [] quadPtr ;
		quadPtr = NULL ;
	}

	if ( texuvPtr != NULL )
	{
		delete [] texuvPtr ;
		texuvPtr = NULL ;
	}
}

void KBatch::setBlitColor( float r , float g , float b , float a )
{
	_r = r ;
	_g = g ;
	_b = b ;
	_a = a ;
}

void KBatch::setAlphaMode( long	alphamode )
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
	}
}

//allocate the vertex buffer
bool KBatch::allocateBuffer( long myquadCount ) 
{
	if ( myquadCount <= 0 )		return false ;
	freeBuffer( ) ;

	quadCount = myquadCount*6*2 ;
	quadPtr = new GLshort[quadCount] ;  //6 points per quad ( x2 coords - x & y )
	texuvPtr = new GLfloat[quadCount] ;
	texColor = new GLfloat[quadCount*4] ; //4 components per point 
	
	return true ;	
}

//begin batch
void		KBatch::beginBatch( KGraphic	*surface ) 
{
	_curColorIndex = 0 ;
	if ( quadCount <= 0 )
	{
		allocateBuffer( 1024 ) ;
	}
	
	if ( surface == NULL )	return ;

	_texturePtr = (KGraphic*)surface ;
	_textureSize = 	_texturePtr->getTextureSize() ;
	_currentQuad = 0 ;

	glPushMatrix( ) ;

	glEnable(GL_COLOR_MATERIAL);
	//_texturePtr->setTextureQuality(_texturePtr->_textureQuality ) ;	
//	glEnable( GL_BLEND ) ; // commentaire de julien
  //glBlendFunc(  _texturePtr->_sourceAlpha , _texturePtr->_destAlpha  );


}


//end batch
void		KBatch::endBatch( void ) 
{
	//glBlendFunc(  _sourceAlpha , _destAlpha  ); // commentaire de julien
	_texturePtr->ptk_glBindTexture( ) ;
	
	
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState( GL_TEXTURE_COORD_ARRAY );
	glEnableClientState( GL_COLOR_ARRAY );
	
	
	//julien
	glEnable( GL_BLEND ) ;
	glBlendFunc(  _sourceAlpha , _destAlpha  ); // commentaire de julien
	
	
	//vertice of the object on screen
	glVertexPointer(2, GL_SHORT, 0, quadPtr );
	//textures UV
	glTexCoordPointer( 2, GL_FLOAT, 0, texuvPtr);		
	glColorPointer( 4, GL_FLOAT, 0, texColor);		
	
	glDrawArrays(GL_TRIANGLES, 0,_currentQuad/2);

	
	//julien
	glDisable( GL_BLEND ) ;
	
	
	glPopMatrix( ) ;
	glDisableClientState( GL_COLOR_ARRAY );
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState( GL_TEXTURE_COORD_ARRAY );

}

//#######################################################################################################################################################
void		KBatch::blitAlphaRectFx( float	x1, float y1 , float	x2 , float y2 , short	destX , short	destY, float angle, float zoom, float blend  ) 
{
	float		width , height ;
	float		halfwidth , halfheight ;
	float	dx1,dy1,dx2,dy2,dx3,dy3,dx4,dy4 ;
	long	rdx,rdy , i;
	float realblend ;
	
	GLfloat u_start = x1 / _textureSize ;
	GLfloat u_end = x2 / _textureSize ;	
	GLfloat v_start = y1 / _textureSize ;
	GLfloat v_end = y2 / _textureSize ;

	realblend = _a*blend ;
	
	if( angle == 0 &&  zoom == 1 )
	{
		destY = 480-destY ;
		
		dx1 = destX ;
		dy1 = destY   ;

		dx2 = destX+(x2-x1) ;
		dy2 = destY-(y2-y1)  ;
	

		//color each vertex with the current blit color
		for ( i = 0 ; i <=5 ; i++ )
		{
			texColor[_curColorIndex++] = _r ;
			texColor[_curColorIndex++] = _g ;
			texColor[_curColorIndex++] = _b ;
			texColor[_curColorIndex++] = realblend ;		
		}
		
		texuvPtr[_currentQuad] =  u_start   ;
		quadPtr[_currentQuad++] = dx1 ;
		texuvPtr[_currentQuad] = v_start    ;
		quadPtr[_currentQuad++] = dy1 ;
		//-----------------------	
		texuvPtr[_currentQuad] = u_end    ;
		quadPtr[_currentQuad++] = dx2 ;
		texuvPtr[_currentQuad] =   v_start  ;
		quadPtr[_currentQuad++] = dy1 ;
		//-----------------------	
		texuvPtr[_currentQuad] =  u_end   ;
		quadPtr[_currentQuad++] = dx2 ;
		texuvPtr[_currentQuad] =  v_end   ;
		quadPtr[_currentQuad++] = dy2 ;
		
		//-----------------------	
		texuvPtr[_currentQuad] = u_start    ;
		quadPtr[_currentQuad++] = dx1 ;
		texuvPtr[_currentQuad] = v_start   ;
		quadPtr[_currentQuad++] = dy1 ;
		//-----------------------
		texuvPtr[_currentQuad] = u_end    ;
		quadPtr[_currentQuad++] = dx2 ;
		texuvPtr[_currentQuad] = v_end   ;
		quadPtr[_currentQuad++] = dy2 ;
		//-----------------------
		texuvPtr[_currentQuad] = u_start    ;
		quadPtr[_currentQuad++] = dx1 ;
		texuvPtr[_currentQuad] = v_end   ;
		quadPtr[_currentQuad++] = dy2 ;
		//-----------------------
		
		if(_currentQuad >= quadCount -1 )
		{
			endBatch() ;
			beginBatch( _texturePtr ) ;
		}
		return ;
	}
	else
	{
		width  = (x2-x1) ;
		height = (y2-y1) ;
		halfwidth = (width) / 2 ;
		halfheight =(height) / 2 ;
		
		
		rdx = destX+halfwidth ;
		rdy = (480-(destY+halfheight));
		
		dx1 = -halfwidth ;
		dy1 = halfheight ;

		dx2 = halfwidth ;
		dy2 = halfheight ;

		dx3 = halfwidth ;
		dy3 = -halfheight ;

		dx4 = -halfwidth ;
		dy4 = -halfheight ;

		
		if ( angle != 0 )
		{
		
			//float angle_rad = ( K2PI*(0-angle))/360.0f ;
			//float cos_angle 	= 	cos(angle_rad) ;
			//float sin_angle 	= 	sin(angle_rad) ;

			while(angle<0) angle+=360.0f;
			short angle_rad = (short)(angle*8.0f) % (360*8);
			float cos_angle 	= 	cosr_lup[angle_rad] ;
			float sin_angle 	= 	sinr_lup[angle_rad] ;
			
			//rotate our object
			float xf = dx1*cos_angle +dy1*sin_angle 	;
			float yf = dy1*cos_angle-dx1*sin_angle	 ;
			dx1 = xf ;
			dy1 = yf ;
			
			xf = dx2*cos_angle +dy2*sin_angle ;
			yf = dy2*cos_angle-dx2*sin_angle ;
			dx2 = xf ;
			dy2 = yf ;
			
			
			xf = dx3*cos_angle +dy3*sin_angle ;
			yf = dy3*cos_angle-dx3*sin_angle ;
			dx3 = xf ;
			dy3 = yf ;
			
			
			xf = dx4*cos_angle +dy4*sin_angle ;
			yf = dy4*cos_angle-dx4*sin_angle ;
			dx4 = xf ;
			dy4 = yf ;
		}	
		
		if ( zoom != 1 )
		{
			dx1 *= zoom ;
			dy1 *= zoom ;
			dx2 *= zoom ;
			dy2 *= zoom ;
			dx3 *= zoom ;
			dy3 *= zoom ;
			dx4 *= zoom ;
			dy4 *= zoom ;
		}	

		dx1 += rdx ;
		dy1 += rdy ;
		dx2 += rdx ;
		dy2 += rdy ;
		dx3 += rdx ;
		dy3 += rdy ;
		dx4 += rdx ;
		dy4 += rdy ;	
	}//if
	
		
	
	//color each vertex with the current blit color
	for ( i = 0 ; i <=5 ; i++ )
	{
		texColor[_curColorIndex++] = _r ;
		texColor[_curColorIndex++] = _g ;
		texColor[_curColorIndex++] = _b ;
		texColor[_curColorIndex++] = realblend ;		
	}
	
	texuvPtr[_currentQuad] =  u_start   ;
	quadPtr[_currentQuad++] = dx1 ;

	texuvPtr[_currentQuad] = v_start    ;
	quadPtr[_currentQuad++] = dy1 ;
	//-----------------------	
	texuvPtr[_currentQuad] = u_end    ;
	quadPtr[_currentQuad++] = dx2 ;
	texuvPtr[_currentQuad] =   v_start  ;
	quadPtr[_currentQuad++] = dy2 ;
	//-----------------------	
	texuvPtr[_currentQuad] =  u_end   ;
	quadPtr[_currentQuad++] = dx3 ;
	texuvPtr[_currentQuad] =  v_end   ;
	quadPtr[_currentQuad++] = dy3 ;
	
	//-----------------------	
	texuvPtr[_currentQuad] = u_start    ;
	quadPtr[_currentQuad++] = dx1 ;
	texuvPtr[_currentQuad] = v_start   ;
	quadPtr[_currentQuad++] = dy1 ;
	//-----------------------
	texuvPtr[_currentQuad] = u_end    ;
	quadPtr[_currentQuad++] = dx3 ;
	texuvPtr[_currentQuad] = v_end   ;
	quadPtr[_currentQuad++] = dy3 ;
	//-----------------------
	texuvPtr[_currentQuad] = u_start    ;
	quadPtr[_currentQuad++] = dx4 ;
	texuvPtr[_currentQuad] = v_end   ;
	quadPtr[_currentQuad++] = dy4 ;
	//-----------------------
	
	if(_currentQuad >= quadCount -1 )
	{
		endBatch() ;
		beginBatch( _texturePtr ) ;
	}
	
	
	
	//	glColor4f( 1.0f,1.0f,1.0f,1.0f) ;
	
	
}

//blit used for all the function
void KBatch::blitAlphaRect(long x1, long y1, long x2, long y2, float destX, float destY ) 
{
//	_texturePtr->blitAlphaRect( x1,y1,x2,y2, destX , destY ) ;	
	long i ;
	
	destY = 480-destY ;

	

	
	long dx1,dy1,dx2,dy2 ;
	
	dx1 = destX ;
	dx2 = destX+(x2-x1) ;
	dy1 = destY   ;
	dy2 = destY-(y2-y1)  ;
	
	GLfloat u_start = (float)x1 / _textureSize ;
	GLfloat u_end = (float)x2 / _textureSize ;	
	GLfloat v_start = (float)(y1) / _textureSize ;
	GLfloat v_end = (float)(y2) / _textureSize ;
	
	//color each vertex with the current blit color
	for ( i = 0 ; i <=5 ; i++ )
	{
		texColor[_curColorIndex++] = _r ;
		texColor[_curColorIndex++] = _g ;
		texColor[_curColorIndex++] = _b ;
		texColor[_curColorIndex++] = _a ;		
	}
	
	texuvPtr[_currentQuad] =  u_start   ;
	quadPtr[_currentQuad++] = dx1 ;
	texuvPtr[_currentQuad] = v_start    ;
	quadPtr[_currentQuad++] = dy1 ;
//-----------------------	
	texuvPtr[_currentQuad] = u_end    ;
	quadPtr[_currentQuad++] = dx2 ;
	texuvPtr[_currentQuad] =   v_start  ;
	quadPtr[_currentQuad++] = dy1 ;
	//-----------------------	
	texuvPtr[_currentQuad] =  u_end   ;
	quadPtr[_currentQuad++] = dx2 ;
	texuvPtr[_currentQuad] =  v_end   ;
	quadPtr[_currentQuad++] = dy2 ;
	
	//-----------------------	
	texuvPtr[_currentQuad] = u_start    ;
	quadPtr[_currentQuad++] = dx1 ;
	texuvPtr[_currentQuad] = v_start   ;
	quadPtr[_currentQuad++] = dy1 ;
	//-----------------------
	texuvPtr[_currentQuad] = u_end    ;
	quadPtr[_currentQuad++] = dx2 ;
	texuvPtr[_currentQuad] = v_end   ;
	quadPtr[_currentQuad++] = dy2 ;
	//-----------------------
	texuvPtr[_currentQuad] = u_start    ;
	quadPtr[_currentQuad++] = dx1 ;
	texuvPtr[_currentQuad] = v_end   ;
	quadPtr[_currentQuad++] = dy2 ;
	//-----------------------
	
	if(_currentQuad >= quadCount -1 )
	{
		endBatch() ;
		beginBatch( _texturePtr ) ;
	}
	
	
	
//	glColor4f( 1.0f,1.0f,1.0f,1.0f) ;

	
}

/*
 
 
 void KGraphicGL::drawRect(KGraphic *textureObject, int x1, int y1, int x2, int y2, int w, int h)
 {		
 float y2inv = KPTK::getWindowHeight() - y2;
 
 // LES COORDONNES DU SPRITE --------------------------------------
 GLfixed rect[] = { 
 Float2Fixed(x2), Float2Fixed(y2inv), 0,
 Float2Fixed(x2+w), Float2Fixed(y2inv), 0,
 Float2Fixed(x2+w), Float2Fixed(y2inv-h), 0,
 
 Float2Fixed(x2), Float2Fixed(y2inv),0,
 Float2Fixed(x2+w), Float2Fixed(y2inv-h), 0,
 Float2Fixed(x2), Float2Fixed(y2inv-h) , 0
 };
 
 GLfixed rnormal[] = {
 0, Float2Fixed(1.0f), 0,
 0, Float2Fixed(1.0f), 0,
 0, Float2Fixed(1.0f), 0,
 
 0, Float2Fixed(1.0f), 0,
 0, Float2Fixed(1.0f), 0,
 0, Float2Fixed(1.0f), 0 
 };
 
 
 // LES COORDONNEES DE TEXTURES --------------------------------------------	
 
 float u_start	= (float)x1; 
 float u_end		= u_start + (float)w;	
 u_start /= (float)textureObject->graphicParams.width; 
 u_end /= (float)textureObject->graphicParams.width; 
 
 float v_start	= (float)(textureObject->graphicParams.originalHeight - y1);
 float v_end		= v_start - (float)h;
 v_start /= (float)textureObject->graphicParams.height; 
 v_end /= (float)textureObject->graphicParams.height; 
 
 
 GLfixed rectuv[] = { 
 Float2Fixed(u_start), Float2Fixed(v_start),
 Float2Fixed(u_end), Float2Fixed(v_start),
 Float2Fixed(u_end), Float2Fixed(v_end),
 
 Float2Fixed(u_start), Float2Fixed(v_start),
 Float2Fixed(u_end), Float2Fixed(v_end),
 Float2Fixed(u_start), Float2Fixed(v_end) 
 };
 
 
 // BLIT OPENGL -----------------------------------------
 
 glEnable(GL_BLEND);
 glDisable(GL_DEPTH_TEST);
 glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
 
 glEnableClientState( GL_VERTEX_ARRAY );
 glEnableClientState( GL_NORMAL_ARRAY );
 glEnableClientState( GL_TEXTURE_COORD_ARRAY );
 glVertexPointer( 3, GL_FIXED, 0, rect );
 glNormalPointer( GL_FIXED, 0, rnormal);
 glTexCoordPointer( 2, GL_FIXED, 0, rectuv );		
 glDrawArrays( GL_TRIANGLES, 0, 6);
 
 glEnable(GL_DEPTH_TEST);
 glDisable(GL_BLEND);
 }
 
 
 */
/*
void	KGraphicGL::GenericBlit( short x1 , short y1 , short x2 , short y2 , short destX , short destY , bool flipx , bool flipy ) 
{
	float u_start  , v_start  ;
	float u_end  , v_end  ;
	
	short dx1,dy1,dx2,dy2 ;
	
	//prevents a /0 divide operation
	if ( _textureSize <= 0 ) return ;
	
	
	ptk_glBindTexture( ) ;
	
	dx1 = destX ;
	dx2 = destX+(x2-x1) ;
	dy1 = destY   ;
	dy2 = destY-(y2-y1)  ;
	
	//our triangl strip
	GLshort triangleStrip[] =
	{
		dx2,dy1,0,
		dx2,dy2,0,
		dx1,dy1,0,
		dx1,dy2,0
	};

	
	
	u_start = (float)x1 / _textureSize ;
	u_end = (float)x2 / _textureSize ;
	
	v_start = (float)(y1) / _textureSize ;
	v_end = (float)(y2) / _textureSize ;
	

	GLfloat rectuv[] = { 
		u_end , v_start ,
		u_end , v_end ,
		u_start , v_start ,
		u_start, v_end
	};
	
	
	
	
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState( GL_TEXTURE_COORD_ARRAY );
	
	//	glDisable(GL_TEXTURE_2D );
	
	//vertice of the object on screen
	glVertexPointer(3, GL_SHORT, 0, triangleStrip);
	//textures UV
	glTexCoordPointer( 2, GL_FLOAT, 0, rectuv );		
	
	glDrawArrays(GL_TRIANGLE_STRIP, 0,4);
	
}

*/
void		KBatch::drawRect( float	x1, float y1 , float	x2 , float y2 , float r ,float g ,float b ,  float blend ) 
{
 /*   y2 = KGraphicGL::screenHeight - y2 ;
    y1 = KGraphicGL::screenHeight - y1 ;
    
    KGraphicGL::_oldBindedText = -1 ;
    
    glDisable( GL_TEXTURE_2D) ;
//    glDisable( GL_TEXTURE_RECTANGLE_NV ) ;
    
    
    if (  blend != 1 )
    {
        glEnable( GL_BLEND ) ;
        glColor4f( r , g , b ,blend ) ;
        glBlendFunc(  GL_SRC_ALPHA , GL_ONE_MINUS_SRC_ALPHA  );
    }
    else
    {
        glDisable( GL_BLEND ) ;
        glColor3f( r , g , b ) ;	
    }
    //	destY	 = screenHeight -destY ;
    
    glBegin(GL_QUADS);
    glVertex3i( x1 ,y1, 0);  
	  	glVertex3i( x2 , y1, 0);
        glVertex3i( x2 , y2 , 0);
        glVertex3i( x1, y2, 0 );	    	    		
        glEnd();
        
        glDisable( GL_BLEND ) ;
        glColor4f( 1.0f , 1.0f ,1.0f ,1.0f ) ;
*/
}
