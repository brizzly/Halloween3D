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

#ifndef	DEF_KSPLINE

#define	DEF_KSPLINE 0

struct	KPoint
{
	double	x , y ;
	double	coefx , coefy ;
};

struct	KSegment
{
	bool			active ;
	KPoint		p1 ,p2 ;
};

class		KSpline
{
private:


public:
	KSpline( ) ;
	~KSpline( ) ;
	
	//methode avec un point de controle unique
	void		getPointFromCtrlPoint( KPoint	*kPoint , KPoint	*controlP1, KPoint	*controlP2 , KPoint	*controlP3 , double position) ;
	
	//methode avec 2 points de controles
	void		getPoint( KPoint	*kPoint , double	x1,double y1	,double	x2,double y2 , double coefX1 , double coefY1 , double coefX2 , double coefY2 , double position) ;

};



#endif
