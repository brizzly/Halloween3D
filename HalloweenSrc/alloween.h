#ifndef ALLOWEEN__
#define ALLOWEEN__


//#define H_MAC
//#define H_LINUX
#define H_WINDOWS



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
#include "InternetConfig.h"
#include <sys/dir.h>
#include <sys/file.h>

#endif



#define SWAPEND16(x) ((((x) & 0xFF) << 8) | (((x) >> 8) & 0xFF))
#define SWAPEND32(x) ((((x) & 0xFF) << 24) | ((((x) >> 8) & 0xFF) << 16) | ((((x) >> 16) & 0xFF) << 8) | (((x) >> 24) & 0xFF))



#define	DEMO_GAME
//#define	FULL_GAME


#define GAMEVERSION_LITE        "Htnn V2005"

#ifdef H_WINDOWS
#ifdef DEMO_GAME
#define GAMEVERSION             "HALLOWEEN 2005-Win DEMO (C) Copyright 2005 Jadeware"
#else
#define GAMEVERSION             "HALLOWEEN 2005-Win (C) Copyright 2005 Jadeware"
#endif
#endif

#ifdef H_LINUX
#ifdef DEMO_GAME
#define GAMEVERSION             "HALLOWEEN 1.999-Linux DEMO (C) Copyright 2003 Jadeware"
#else
#define GAMEVERSION             "HALLOWEEN 1.999-Linux (C) Copyright 2003 Jadeware"
#endif
#endif

#ifdef H_MAC
#ifdef DEMO_GAME
#define GAMEVERSION             "HALLOWEEN 1.999-Mac_v3 DEMO (C) Copyright 2003 Jadeware"
#else
#define GAMEVERSION             "HALLOWEEN 1.999-Mac_v3 (C) Copyright 2003 Jadeware"
#endif
#endif


#define	NAME				"HALLOWEEN, The new nightmare!"
#define TEXTURELOADED		1024
#define	TEXTURENAMEMAX		255
#define	MAX_RESPAWN_OBJ		512
#define	MAX_CLONEMODEL		32
#define	MAX_ANIMMODEL		4
#define	JOKER				666
#define	MAX_MAPFILE			20
#define	MAX_MAPFILENAME		24
#define	MAX_PINDEX_PR_LEAF	99
#define	EMPTY_SPACE			-1
#define	SOLID_SPACE			-2
#define	MAX_PART			32
#define	MAX_PARTSLOT		255
#define	MAX_SCRIPTSLOT		255
#define	MAX_MVTPERAB		16
#define	MAX_TEXT			8
#define	MAX_SOUND			8
#define	NEAR_DIST_ACTIVATE	30.0f
#define	MAX_LEXEME_SIZE		512
#define	MAX_LEXEMES			14
#define	MAX_SYMBOLES		8
#define	MAX_LIGHTMAP		64
#define	ONEOVER16			0.0625f
#define	ONEOVER256			0.00390625f
#define	MAX_WAVSIZE			256000
#define	MAX_SAMPLE			128
#define	MAX_CLONESAMPLE		16 // 128
#define	MULT_COORD			0.03f
#define	MAX_ENTITIES_TYPE	128
#define	MAX_TABSORT			256
#define	SCRIPT_TIME_DISP	3000.0f
#define	MAX_GAMEKEY			13
#define	FLAME_ANIM_MSEC		80.0f
#define	GAME_CLOCK			180
#define	GAME_DIETIME		3000.0f
#define	GAME_WINTIME		10000.0f //3500.0f
#define	STOP_DIST			50.0f
#define	CATCHED_TIME		2000.0f

#define	TRACK_PRES1			"pres_1.ogg"
#define	TRACK_PRES2			"pres_2.ogg"
#define	TRACK_PRES3			"pres_3.ogg"
#define	TRACK_PRES4			"pres_4.ogg"
#define	TRACK_0				"Halloween0.ogg"
#define	TRACK_1				"Halloween1.ogg"
#define	TRACK_2				"Halloween2.ogg"
#define	TRACK_3				"Halloween3.ogg"
#define	TRACK_4				"Halloween4.ogg"
#define	TRACK_5				"Halloween5.ogg"
#define	TRACK_6				"Halloween6.ogg"
#define	TRACK_7				"Halloween7.ogg"

#ifdef H_MAC
#define	TRACK_PB			"partyboy.ogg"
#else
#define	TRACK_PB			"partyboy.mp3"
#endif

#define	VID_INTRO			"system/video/intro.avi"
#define	VID_OUTRO			"system/video/outro.avi"
#define	VID_CREDIT			"system/video/credit.avi"


#ifdef H_MAC

#define	CONFIG				"hconfig.cfg"
#define	SYSDIR				"system\\"
#define	SYSTEX				"systex\\"
#define	MAPDIR				"maps\\"
#define	ENTDIR				"entity\\"
#define	MDLDIR				"model\\"
#define	TEXDIR				"textures\\"
#define	SNDDIR				"sound\\"
#define	MUSDIR				"music\\"
#define	DEMDIR				"demos\\"
#define	SHTDIR				"screenshots\\"
#define	LVLSHT				"levelshot\\"
#define	SVGDIR				"save\\"
#define	INTDIR				"interface\\"

#else

#define	CONFIG				"hconfig.cfg"
#define	SYSDIR				"system/"
#define	SYSTEX				"systex/"
#define	MAPDIR				"maps/"
#define	ENTDIR				"entity/"
#define	MDLDIR				"model/"
#define	TEXDIR				"textures/"
#define	SNDDIR				"sound/"
#define	MUSDIR				"music/"
#define	DEMDIR				"demos/"
#define	SHTDIR				"screenshots/"
#define	LVLSHT				"levelshot/"
#define	SVGDIR				"save/"
#define	INTDIR				"interface/"

#endif



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

#endif //ALLOWEEN__

