
#include	<windows.h>
#include	<olectl.h>
#include	<commdlg.h>
#include 	<ocidl.h>
#include 	<crtdbg.h>


typedef struct oglBitmapTAG
{
	unsigned long	*PixMapPtr;
	long			width, height ;
	char			depth;

} oglBitmap;



oglBitmap *LoadPictureFromFile(char *szFile);
oglBitmap *RenderPicture(LPPICTURE picture);

