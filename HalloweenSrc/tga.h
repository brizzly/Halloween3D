
#ifndef TGA_INCLUDED
#define TGA_INCLUDED

#include "alloween.h"

#define TGA_FILE_NOT_FOUND		-13			// file was not found
#define TGA_BAD_IMAGE_TYPE		-14			// color mapped image or image is not uncompressed
#define TGA_BAD_DIMENSION		-15			// dimension is not a power of 2
#define TGA_BAD_BITS			-16			// image bits is not 8, 24 or 32
#define TGA_BAD_DATA			-17			// image data could not be loaded

int tga_Load(char *name, int id, int picmip, hBool map_tex);


#endif  //TGA_INCLUDED
