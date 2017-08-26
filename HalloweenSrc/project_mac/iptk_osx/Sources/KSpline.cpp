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

#include	"KSpline.h"

#define	B1(t)		(t*t)
#define	B2(t)		(2*t*(1-t))
#define	B3(t)		((1-t)*(1-t))


KSpline::KSpline( ) 
{

}


KSpline::~KSpline( ) 
{

}


//methode avec 1 point de controle unique
void		KSpline::getPointFromCtrlPoint( KPoint	*kPoint , KPoint	*controlP1, KPoint	*controlP2 , KPoint	*controlP3 , double position) 
{
double	b1,b2,b3 ;

	if ( kPoint == 0 )		return ;
	
	b1 = B1(position ) ;
	b2 = B2(position ) ;
	b3 = B3(position ) ;
	
	kPoint->x  = 	controlP1->x*b1 	+ controlP2->x*b2 + controlP3->x*b3;
	kPoint->y 	= 	controlP1->y*b1 	+ controlP2->y*b2 + controlP3->y*b3;
}


//position = position
void		KSpline::getPoint( KPoint	*kPoint , double	x1,double y1	,double	x2,double y2 , double coefX1 , double coefY1 , double coefX2 , double coefY2 , double position) 
{
double a ;
double b ;

	if ( kPoint == 0 )		return ;

	a = 1-position ;
 	b = position;

	// Get a point on the curve
	kPoint->x 	= (x1*a*a*a + coefX1*3*a*a*b + coefX2*3*a*b*b + x2*b*b*b) ;
	kPoint->y	= (y1*a*a*a + coefY1*3*a*a*b + coefY2*3*a*b*b + y2*b*b*b) ;
	

}



