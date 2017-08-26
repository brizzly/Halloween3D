#ifndef ALLOWEEN__
#define ALLOWEEN__


#define H_MAC
//#define H_LINUX
//#define H_WINDOWS




typedef int		hBool;
#define true	1
#define false	0

typedef unsigned char	byte;
typedef unsigned short	word;
typedef unsigned long	dword;
#ifdef H_WINDOWS	// hack
typedef	unsigned int	uint;
#endif

enum
{
	TEX_TGA,
	TEX_JPG,
	TEX_UNDEF
};

typedef struct
{
	hBool	used;
	hBool	picmip_locked;
	hBool	map_tex;
	int		type;
	int		id;
	char	*name;
	
}	TEXLIST;


enum
{
	MAP_NULL,
	MAP_POLYGONS,
	MAP_BSP
};


#ifdef H_WINDOWS
static int		GlobalTextureBind=-1;
static int		GlobalEntities=0;
static int		ScreenX[] = { 512, 640, 800, 1024, 1152, 1280, 1600 };
static int		ScreenY[] = { 384, 480, 600, 768, 864, 960, 1200 };
#endif






#ifdef H_WINDOWS

#include <windows.h>
#include <Richedit.h>
#include <math.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <fstream> // .h removed in visual 8
#include <direct.h>
#include <GL\gl.h>
#include <GL\glu.h>

#endif


#ifdef H_LINUX

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdarg.h>
#include <string.h>
#include "GL/gl.h"
#include "GL/glu.h"
#include "SDL.h"

#endif


#ifdef H_MAC

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdarg.h>
#include <string.h>
#include "KWindow.h"
#include "KGraphic.h"
#include "KSound.h"
#include "KMiscTools.h"
#include <sys/dir.h>
#include <sys/file.h>

#endif


#include "constantes.h"


#ifdef H_WINDOWS
#include "halloween.h"
#include "input_win.h"
#endif

#ifdef H_LINUX
#include "halloween_linux.h"
#include "input.h"
#endif

#ifdef H_MAC
#include "halloween_linux.h"
#include "input.h"
#endif


#include "interface.h"
#include "bspfile.h"
#include "objet.h"
#include "objet_proc.h"
#include "menu_init.h"
#include "menu_sel.h"
#include "menu.h"



#ifdef H_WINDOWS
#include "libbass.h"
#include "dsound.h"
#include "imageloader_win.h"
//#include "net_client.h"
//#include "net_server.h"
//#include "net_dcomp.h"
//#include "net_common.h"
#endif


#ifdef H_LINUX
#include "libsound_linux.h"
#include "loadjpeg_linux.h"
#include "net_temp_linux.h"
#endif


#ifdef H_MAC
#include "libsound_linux.h"
#include "loadjpeg_linux.h"
#include "net_temp_linux.h"
#endif



#include "enveffect.h"
#include "sorting.h"
#include "collision.h"
//#include "collision2.h"
#include "bsptree.h"
#include "script.h"
#include "player.h"
#include "maths.h"
#include "physic.h"
#include "timer.h"
#include "tga.h"
#include "jpeg.h"
#include "image.h"
#include "opengl.h"
#include "mskmodel.h"
#include "monster.h"
#include "textdraw.h"
#include "camera.h"
#include "console.h"
#include "filesys.h"
#include "ellipsoide.h"
#include "liste.h"
#include "frustrum.h"
#include "demo.h"
#include "story.h"
#include "game.h"
#include "gameover.h"
#include "shader.h"




#endif





#endif //ALLOWEEN__

