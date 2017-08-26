
#include "GLTextureHelper.h"
#include "KMiscTools.h"

#include <QuartzCore/QuartzCore.h>
#include <Foundation/NSBundle.h>
//#include <UIKit/UIImage.h>
#import <Cocoa/Cocoa.h>



GLTextureHelper::GLTextureHelper()
{
	_pData = NULL;
	_eyeRetina = false;
}

GLTextureHelper::~GLTextureHelper(void)
{
	if(_pData) {
		free(_pData);
		_pData = NULL;
	}
}

void GLTextureHelper::clearSurface(GLubyte * surface, int w, int h, int bpp, int R, int G, int B, int A)
{
	int pixelSize = bpp/8;
	for(int i=0 ; i<w ; i++)
	{
		for(int j=0 ; j<h ; j++)
		{
			int pos = (i + (w * j))*pixelSize;
			if(pixelSize == 3)
			{
				surface[pos+0] = R;
				surface[pos+1] = G;
				surface[pos+2] = B;
			}
			else if(pixelSize == 4)
			{
				surface[pos+0] = R;
				surface[pos+1] = G;
				surface[pos+2] = B;
				surface[pos+3] = A;
			}
		}		
	}	
}

void GLTextureHelper::colorkeySurface(GLubyte * surface, int w, int h, int bpp, int ckR, int ckG, int ckB)
{
	int pixelSize = bpp/8;
	for(int i=0 ; i<w ; i++)
	{
		for(int j=0 ; j<h ; j++)
		{
			int pos = (i + (w * j))*pixelSize;
			if(pixelSize == 3)
			{
				printf("plop\n");
			}
			else if(pixelSize == 4)
			{
				int R = surface[pos+2];
				int G = surface[pos+1];
				int B = surface[pos+0];				
				int A = 255;
			//	printf("%d %d %d\n", R, G, B);
				if(R == ckR && G == ckG && B == ckB) 
				//if(B == 255)
				{
					A = 0;
				}
				surface[pos+2] = R;
				surface[pos+1] = G;
				surface[pos+0] = B;
				surface[pos+3] = A;
			}
		}		
	}	
}

bool GLTextureHelper::LoadWatermark()
{
	return true;
}

bool GLTextureHelper::loadText(const char *text, const char * fontname, int fontsize)
{
	return true;
}

void GLTextureHelper::listFonts()
{
}

bool GLTextureHelper::isEyeRetina()
{
	return false;
}

bool GLTextureHelper::LoadTexture(const char *szTextureFile, bool colorkey, unsigned char ckr, unsigned char ckg, unsigned char ckb)
{
//	NSURL					*url = nil;
//	CGImageSourceRef		src;
	CGImageRef				image;
	CGContextRef			context = nil;
	CGColorSpaceRef			colorSpace;
	
	_nWidth = 0;
	_nHeight = 0;
	
	NSString * stringFromChar = [NSString stringWithCString:szTextureFile encoding:NSUTF8StringEncoding]; 
	NSImage * myimage = [NSImage imageNamed:stringFromChar];		
	//image = myimage.CGImage;	
//	NSRect r = NSRectFromCGRect(CGRectMake(0, 0, myimage.size.width, myimage.size.height));
//	image = [myimage CGImageForProposedRect:&r context:nil hints:nil];
	image = [myimage CGImageForProposedRect:nil context:nil hints:nil];
	if(image == NULL) {
		return false;
	}

/*	
	//NSString * str = [NSString stringWithUTF8String:szTextureFile];
	NSString * str = [NSString stringWithCString:szTextureFile encoding:NSUTF8StringEncoding]; 
	url = [NSURL fileURLWithPath: str];
	src = CGImageSourceCreateWithURL((CFURLRef)url, NULL);
	
	if (!src) {
		NSLog(@"No image");
		return false;
	}
	
	image = CGImageSourceCreateImageAtIndex(src, 0, NULL);
	CFRelease(src);
*/
	
	_nWidth = CGImageGetWidth(image);
	_nHeight = CGImageGetHeight(image);
	
	int v = 0;
	if ( _nWidth > _nHeight )
	{
		v = _nWidth;
	}
	else
	{
		v = _nHeight;
	}
	_texSize = 1;
	do
	{
		_texSize *= 2;
		if ( _texSize >= v )  
			break;
	}
	while ( _texSize < 2048  ); 
	
	//pixels = (GLubyte*) calloc(_texSize * _texSize * 4, sizeof(GLubyte));
	_pData = (GLubyte*) calloc(_nWidth * _nHeight * 4, sizeof(GLubyte));
	clearSurface(_pData, _nWidth, _nHeight, 32, 0, 0, 0, 1);	
	
	colorSpace = CGColorSpaceCreateDeviceRGB();
	//	context = CGBitmapContextCreate(data, _imageWidth, _imageHeight, 8, 4 * _texSize, colorSpace, kCGImageAlphaPremultipliedFirst | kCGBitmapByteOrder32Host);
	context = CGBitmapContextCreate(_pData, _nWidth, _nHeight, 8, 4 * _nWidth, colorSpace, kCGImageAlphaPremultipliedFirst | kCGBitmapByteOrder32Host);
	CGColorSpaceRelease(colorSpace);
	
	// Core Graphics referential is upside-down compared to OpenGL referential
	// Flip the Core Graphics context here
	// An alternative is to use flipped OpenGL texture coordinates when drawing textures
	//	CGContextTranslateCTM(context, 0.0, _imageHeight);
	//	CGContextScaleCTM(context, 1.0, -1.0);
	
	// Set the blend mode to copy before drawing since the previous contents of memory aren't used. This avoids unnecessary blending.
	CGContextSetBlendMode(context, kCGBlendModeCopy);
	
	CGContextDrawImage(context, CGRectMake(0, 0, _nWidth, _nHeight), image);
	
	CGContextRelease(context);
//	CGImageRelease(image);
	
	if(colorkey) {
		colorkeySurface(_pData, _nWidth, _nHeight, 32, ckr, ckg, ckb);
	}

	glBufferData(GL_PIXEL_UNPACK_BUFFER, _nWidth * _nHeight * 4 * sizeof(GLubyte), _pData, GL_STATIC_DRAW);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, _nWidth, _nHeight, 0, GL_BGRA, GL_UNSIGNED_BYTE/*GL_UNSIGNED_INT_8_8_8_8_REV*/, 0);

//	gluBuild2DMipmaps(GL_TEXTURE_2D, 4, _nHeight, _nHeight, GL_RGBA, GL_UNSIGNED_BYTE, _pData);
	
	return true;
}

GLubyte * GLTextureHelper::getDatas()
{
	return _pData;
}


int GLTextureHelper::LoadFileTGA(JadeImage_t *textureObject)
{    
	unsigned char		*imageData;									// Image Data (Up To 32 Bits)
	int					bpp;										// Image Color Depth In Bits Per Pixel.
	int					width;										// Image Width
	int					height;										// Image Height
	unsigned char		TGAheader[12] =		{0,0,2,0,0,0,0,0,0,0,0,0};	// Uncompressed TGA Header
	unsigned char		cTGAheader[12] =	{0,0,10,0,0,0,0,0,0,0,0,0};	// Compressed TGA Header
	unsigned char		TGAcompare[12];								// Used To Compare TGA Header
	unsigned char		header[6];									// First 6 Useful Bytes From The Header
	unsigned int		bytesPerPixel;								// Holds Number Of Bytes Per Pixel Used In The TGA File
	unsigned int		imageSize;									// Used To Store The Image Size When Setting Aside Ram
	int					temp;										// Temporary Variable
	
	if(textureObject == 0) {
		return 0;
	}
	
	//Texture_t textureObject;
	textureObject->width = 0;
	textureObject->height = 0;
	textureObject->bpp = 0;
	textureObject->pixels = 0;
	
	char	*file_path = KMiscTools::makeFilePath(textureObject->filename);
	FILE *file = fopen(file_path, "rb");
	if(	file==NULL )
	{
		return 0;
	}
	if( fread(TGAcompare,1,sizeof(TGAcompare),file)!=sizeof(TGAcompare) )	// Are There 12 Bytes To Read?
	{
		return 0;
	}
	
	bool is_compressed = false;
	
	if(memcmp(TGAheader, &TGAcompare, sizeof(TGAcompare)) == 0)				// See if header matches the predefined header of 
	{																		// an Uncompressed TGA image
		is_compressed = false;
	}
	else if(memcmp(cTGAheader, &TGAcompare, sizeof(TGAcompare)) == 0)		// See if header matches the predefined header of
	{																		// an RLE compressed TGA image
		is_compressed = true;
	}
	else																	// If header matches neither type
	{
		//logfile->logDebug("TGA file be type 2 or type 10\n");
		return 0;
	}
	
	if(is_compressed == true)
	{
		// compressed TGA loading -------------------
		
		if(	fread(header,1,sizeof(header),file)!=sizeof(header))				// If So Read Next 6 Header Bytes
		{
			if (file == NULL)									// Did The File Even Exist? *Added Jim Strong*
				return 0;									// Return False
			else
			{
				fclose(file);									// If Anything Failed, Close The File
				return 0;									// Return False
			}
		}
		
		width	= header[1] * 256 + header[0];					// Determine The TGA Width	(highbyte*256+lowbyte)
		height	= header[3] * 256 + header[2];					// Determine The TGA Height	(highbyte*256+lowbyte)
		bpp		= header[4];										// Determine Bits Per Pixel
		
		if(	width	<=0	||								// Is The Width Less Than Or Equal To Zero
		   height	<=0	||								// Is The Height Less Than Or Equal To Zero
		   (header[4]!=24 && header[4]!=32))					// Is The TGA 24 or 32 Bit?
		{
			fclose(file);										// If Anything Failed, Close The File
			return 0;										// Return False
		}
		
		bytesPerPixel	= (bpp / 8);									// Compute BYTES per pixel
		imageSize		= (bytesPerPixel * width * height);		// Compute amout of memory needed to store image
		imageData		= (unsigned char *)malloc(imageSize);					// Allocate that much memory
		
		if(imageData == NULL)											// If it wasnt allocated correctly..
		{
			//logfile->logDebug("Could not allocate memory for image\n");
			fclose(file);
			return 0;
		}
		
		unsigned int pixelcount	= height * width;							// Nuber of pixels in the image
		unsigned int currentpixel	= 0;												// Current pixel being read
		unsigned int currentbyte	= 0;												// Current byte 
		unsigned char * colorbuffer = (unsigned char *)malloc(bytesPerPixel);			// Storage for 1 pixel
		do
		{
			unsigned char chunkheader = 0;											// Storage for "chunk" header
			if(fread(&chunkheader, sizeof(unsigned char), 1, file) == 0)				// Read in the 1 byte header
			{
				//logfile->logDebug("Could not read RLE header\n");
				if(file != NULL)												// If file is open
				{
					fclose(file);												// Close file
				}
				if(imageData != NULL)									// If there is stored image data
				{
					free(imageData);									// Delete image data
				}
				return 0;													// Return failed
			}
			if(chunkheader < 128)												// If the ehader is < 128, it means the that is the number of RAW color packets minus 1
			{																	// that follow the header
				chunkheader++;													// add 1 to get number of following color values
				for(short counter = 0; counter < chunkheader; counter++)		// Read RAW color values
				{
					if(fread(colorbuffer, 1, bytesPerPixel, file) != bytesPerPixel) // Try to read 1 pixel
					{
						//logfile->logDebug("Could not read image data\n");
						if(file != NULL)													// See if file is open
						{
							fclose(file);													// If so, close file
						}
						if(colorbuffer != NULL)												// See if colorbuffer has data in it
						{
							free(colorbuffer);												// If so, delete it
						}
						if(imageData != NULL)										// See if there is stored Image data
						{
							free(imageData);										// If so, delete it too
						}
						return 0;														// Return failed
					}
					imageData[currentbyte		] = colorbuffer[2];				    // Flip R and B vcolor values around in the process 
					imageData[currentbyte + 1	] = colorbuffer[1];
					imageData[currentbyte + 2	] = colorbuffer[0];
					if(bytesPerPixel == 4)												// if its a 32 bpp image
					{
						imageData[currentbyte + 3] = colorbuffer[3];				// copy the 4th byte
					}
					currentbyte += bytesPerPixel;										// Increase thecurrent byte by the number of bytes per pixel
					currentpixel++;															// Increase current pixel by 1
					if(currentpixel > pixelcount)											// Make sure we havent read too many pixels
					{
						//logfile->logDebug("Too many pixels read\n");
						if(file != NULL)													// If there is a file open
						{
							fclose(file);													// Close file
						}	
						if(colorbuffer != NULL)												// If there is data in colorbuffer
						{
							free(colorbuffer);												// Delete it
						}
						if(imageData != NULL)										// If there is Image data
						{
							free(imageData);										// delete it
						}
						return 0;														// Return failed
					}
				}
			}
			else																			// chunkheader > 128 RLE data, next color reapeated chunkheader - 127 times
			{
				chunkheader -= 127;															// Subteact 127 to get rid of the ID bit
				if(fread(colorbuffer, 1, bytesPerPixel, file) != bytesPerPixel)		// Attempt to read following color values
				{	
					//logfile->logDebug("Could not read from file\n");
					if(file != NULL)														// If thereis a file open
					{
						fclose(file);														// Close it
					}
					if(colorbuffer != NULL)													// If there is data in the colorbuffer
					{
						free(colorbuffer);													// delete it
					}
					if(imageData != NULL)											// If thereis image data
					{
						free(imageData);											// delete it
					}
					return 0;															// return failed
				}
				
				for(short counter = 0; counter < chunkheader; counter++)					// copy the color into the image data as many times as dictated 
				{																			// by the header
					imageData[currentbyte		] = colorbuffer[2];					// switch R and B bytes areound while copying
					imageData[currentbyte + 1	] = colorbuffer[1];
					imageData[currentbyte + 2	] = colorbuffer[0];
					if(bytesPerPixel == 4)												// If TGA images is 32 bpp
					{
						imageData[currentbyte + 3] = colorbuffer[3];				// Copy 4th byte
					}
					currentbyte += bytesPerPixel;										// Increase current byte by the number of bytes per pixel
					currentpixel++;															// Increase pixel count by 1
					if(currentpixel > pixelcount)											// Make sure we havent written too many pixels
					{
						//logfile->logDebug("Too many pixels read\n");			// if there is too many... Display an error!
						if(file != NULL)													// If there is a file open
						{
							fclose(file);													// Close file
						}	
						if(colorbuffer != NULL)												// If there is data in colorbuffer
						{
							free(colorbuffer);												// Delete it
						}
						if(imageData != NULL)										// If there is Image data
						{
							free(imageData);										// delete it
						}
						return 0;														// Return failed
					}
				}
			}
		}
		while(currentpixel < pixelcount);													// Loop while there are still pixels left
	}
	else
	{
		// uncompressed TGA loading ------------------
		
		if(	fread(header,1,sizeof(header),file)!=sizeof(header))				// If So Read Next 6 Header Bytes
		{
			if (file == NULL)									// Did The File Even Exist? *Added Jim Strong*
				return 0;									// Return False
			else
			{
				fclose(file);									// If Anything Failed, Close The File
				return 0;									// Return False
			}
		}
		
		width  = header[1] * 256 + header[0];			// Determine The TGA Width	(highbyte*256+lowbyte)
		height = header[3] * 256 + header[2];			// Determine The TGA Height	(highbyte*256+lowbyte)
		
		if(	width	<=0	||								// Is The Width Less Than Or Equal To Zero
		   height	<=0	||								// Is The Height Less Than Or Equal To Zero
		   (header[4]!=24 && header[4]!=32))					// Is The TGA 24 or 32 Bit?
		{
			fclose(file);										// If Anything Failed, Close The File
			return 0;										// Return False
		}
		
		bpp	= header[4];							// Grab The TGA's Bits Per Pixel (24 or 32)
		bytesPerPixel	= bpp/8;						// Divide By 8 To Get The Bytes Per Pixel
		imageSize		= width*height*bytesPerPixel;	// Calculate The Memory Required For The TGA Data
		
		imageData=(unsigned char *)malloc(imageSize);		// Reserve Memory To Hold The TGA Data
		
		if(	imageData==NULL ||							// Does The Storage Memory Exist?
		   fread(imageData, 1, imageSize, file)!=imageSize)	// Does The Image Size Match The Memory Reserved?
		{
			if(imageData!=NULL)						// Was Image Data Loaded
				free(imageData);						// If So, Release The Image Data
			
			fclose(file);										// Close The File
			return 0;										// Return False
		}
	/*
		for(int m=0 ; m<imageSize ; m++)
		{
			unsigned char c = imageData[m];
			printf("%d ", (int)c);
		}*/
	
		for(unsigned int i=0; i<imageSize; i+=bytesPerPixel)		// Loop Through The Image Data
		{														// Swaps The 1st And 3rd Bytes ('R'ed and 'B'lue)
			temp=imageData[i];							// Temporarily Store The Value At Image Data 'i'
			imageData[i] = imageData[i + 2];	// Set The 1st Byte To The Value Of The 3rd Byte
			imageData[i + 2] = temp;					// Set The 3rd Byte To The Value In 'temp' (1st Byte Value)
		}
	}
	
	fclose (file);
	
	textureObject->originalWidth = width;
	textureObject->originalHeight = height;	
	textureObject->width = width;
	textureObject->height = height;
	textureObject->bpp = bpp;
	textureObject->pixels = imageData;
	
	return 1;		
}

