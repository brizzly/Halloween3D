
#include "alloween.cpp"


#define	HIMETRIC_INCH	2540
#define	RES_OK			true 
#define	RES_FAIL		false

//oglBitmap *LoadPictureFromFile(char *szFile);
//oglBitmap *RenderPicture(LPPICTURE picture);



oglBitmap *LoadPictureFromFile(char *szFile)
{
	HRESULT 		hr;
	unsigned long 	ulFileSize = 0  , ulBytesRead = 0 ;
	HANDLE			hFile;
	char			*dataPtr ;
	HGLOBAL 		hGlobal = NULL ;
	LPSTREAM 		pictureStreamPtr = NULL  ;
	LPPICTURE 		picture = NULL;
	oglBitmap		*tmpSurface = NULL ;

	// open file
	 hFile = CreateFile(szFile, GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);
	 if ( hFile == INVALID_HANDLE_VALUE )
	 {
		 m_ConsPrint("MISSINGFILE\n");
		 goto error;
	 }
		
	// get file size
	ulFileSize = GetFileSize(hFile, NULL);
	if ( ulFileSize == 0xFFFFFFFF || ulFileSize == 0)
	{
		m_ConsPrint("FILEISCORRUPT\n") ;
		goto error;
	}

	dataPtr  = NULL;
		
	// alloc memory based on file size
	hGlobal = GlobalAlloc(GMEM_MOVEABLE, ulFileSize);
	if ( hGlobal == NULL )
	{
		m_ConsPrint("OUTOFMEMORY\n") ;
		goto error;
	}

	dataPtr = (char*) GlobalLock(hGlobal);	//our data points to the buffer prepared to receive the file
	if ( dataPtr == NULL )
	{
		m_ConsPrint("OUTOFMEMORY\n") ;
		goto error;
	}

	// read file and store in global memory
	ReadFile(hFile, dataPtr, ulFileSize, &ulBytesRead, NULL);
	if ( ulBytesRead != ulFileSize)
	{
		m_ConsPrint("FILEISCORRUPT\n") ;
		goto error;
	}

	// create IStream* from global memory and FREE the memory of the HGLOBAL ( TRUE flag )
	hr = CreateStreamOnHGlobal(hGlobal, TRUE, &pictureStreamPtr);
	if ( hr != S_OK )
	{
		m_ConsPrint("FILEISCORRUPT\n") ;
		goto error;
	}	
	
	hr = OleLoadPicture(pictureStreamPtr, ulFileSize, FALSE, IID_IPicture, (LPVOID *)&picture);
	//hr = OleLoadPicture(pictureStreamPtr, ulFileSize, FALSE, &IID_IPicture, (LPVOID *)&picture);
	if (hr != S_OK )
	{
		m_ConsPrint("FILEISCORRUPT\n") ;
		goto error;
	}

goto no_error;
error:

	if ( pictureStreamPtr )
		pictureStreamPtr->Release() ;
	if( hGlobal )					
	{	
		GlobalUnlock( hGlobal );
		GlobalFree( hGlobal );
	}

	CloseHandle(hFile);
	return	NULL;

no_error:

	tmpSurface = RenderPicture(picture);
	picture->Release();

	//cool we could decompress our picture !	
	if(pictureStreamPtr)
		pictureStreamPtr->Release();
	if(hGlobal)					
	{	
		GlobalUnlock(hGlobal);
		GlobalFree(hGlobal);
	}

	CloseHandle(hFile);
	return tmpSurface;
}

oglBitmap *RenderPicture(LPPICTURE picture) 
{
	long 		pictureWidth ,  pictureHeight ;
	RECT		r = {0,0,320,240 } ;
	HBITMAP		memBitmap , oldBitmap ;			//bitmap memoire qui va nous servir de buffer
	HDC			memDC	;				//	HDC memoire
	BITMAPINFO	bmi ;
	oglBitmap	*tmpOGLBitmap ;
	unsigned long	*pixPtr ;
	long		i ;
	int			nWidth;
	int			nHeight;

	picture->get_Width(&pictureWidth);
	picture->get_Height(&pictureHeight);

	memDC	=	CreateCompatibleDC( NULL ) ;		//on cree un DC compatible bidon

	// convert himetric to pixels
	nWidth	= MulDiv(	pictureWidth		, GetDeviceCaps(memDC, LOGPIXELSX), HIMETRIC_INCH) ;
	nHeight	= MulDiv(	pictureHeight		, GetDeviceCaps(memDC, LOGPIXELSY), HIMETRIC_INCH) ;
	
	memset( &bmi , 0  , sizeof(BITMAPINFO) ) ;
	bmi.bmiHeader.biSize					=	sizeof (BITMAPINFOHEADER ) ;
	bmi.bmiHeader.biWidth				=	nWidth ;
	bmi.bmiHeader.biHeight				=	nHeight ;
	bmi.bmiHeader.biPlanes				=	1 ;
	bmi.bmiHeader.biBitCount			=	32 ;  //16 = 16 bits  24 = 24 bits et 32 = 32 bits
	bmi.bmiHeader.biCompression		=	BI_RGB ;
	bmi.bmiHeader.biSizeImage		=	0	;
	bmi.bmiHeader.biXPelsPerMeter	=	0 ;
	bmi.bmiHeader.biYPelsPerMeter	=	0 ;
	bmi.bmiHeader.biClrUsed			=	0 ;
	bmi.bmiHeader.biClrImportant		=	0 ;
	
	tmpOGLBitmap		=	(oglBitmapTAG *)malloc(sizeof(oglBitmap));
	tmpOGLBitmap->PixMapPtr	= NULL;
	tmpOGLBitmap->width		= nWidth;
	tmpOGLBitmap->height 	= nHeight;
	tmpOGLBitmap->depth		= (char)bmi.bmiHeader.biBitCount;
	
	memBitmap	=	 CreateDIBSection( 	memDC , &bmi ,	DIB_RGB_COLORS , (void**)&pixPtr ,			//recois l'adresse des  pixels
															NULL ,
															0
															 ) ;

	if ( pixPtr == NULL )
	{
		//delete	tmpOGLBitmap ;
		s_free(tmpOGLBitmap);
		tmpOGLBitmap	=	NULL ;
		MessageBox( 0, "could not get pixel map",0,0 ) ;
		return	NULL ;
	}
	oldBitmap	=	( HBITMAP)SelectObject( memDC , memBitmap ) ;	// selectionne la bitmap sur le hdc ( important !!)


	picture->Render(memDC,		0,0,nWidth	,nHeight,						//destination
								0,pictureHeight,pictureWidth	,-pictureHeight,		//source  ( stored baCKWARDS )
								NULL  )	;
	

	//copy the bits
	
	tmpOGLBitmap->PixMapPtr = new unsigned long[ nWidth * nHeight *4 ] ;
	//tmpOGLBitmap->PixMapPtr = malloc(sizeof(unsigned long)*(nWidth*nHeight*4));

	for(i=0 ; i<nWidth*nHeight ; i++)
	{
		tmpOGLBitmap->PixMapPtr[i]	 = pixPtr[i] ;
	}
	
	//a ce point precis la carte des pixels se situe dans :	pixelPtr
	
	//on vire ce qui est inutile
	SelectObject( memDC , oldBitmap ) ;	// restaure l'ancienne bitmap sur le hdc
	DeleteObject( memBitmap ) ;					//on peu detruire
	DeleteObject( memDC ) ;				
		
	return	tmpOGLBitmap ;
}

