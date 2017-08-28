
#ifndef JPEG_INCLUDED
#define JPEG_INCLUDED


#include "alloween.h"

extern "C" {
#include "jpeglib.h"
#include <setjmp.h>
}


/*
#include <ijl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
*/

//Color modes
#define C16BIT  16
#define C24BIT  24
#define C32BIT  32

//Display Mode
#define CLRMODE        24  //bit size


int jpeg_Load(char *Name, int id, int picmip, hBool map_tex);

int LoadFromJPEG(char *Name, char *filename, int text_id, hBool map_tex);


#endif // JPEG_INCLUDED
