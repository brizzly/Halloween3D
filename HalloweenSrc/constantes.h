
#ifndef CONSTANTES_INCLUDED
#define CONSTANTES_INCLUDED


#define SWAPEND16(x) ((((x) & 0xFF) << 8) | (((x) >> 8) & 0xFF))
#define SWAPEND32(x) ((((x) & 0xFF) << 24) | ((((x) >> 8) & 0xFF) << 16) | ((((x) >> 16) & 0xFF) << 8) | (((x) >> 24) & 0xFF))



//#define	DEMO_GAME
#define	FULL_GAME


#define GAMEVERSION_LITE        "Halloween3D"

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
#define GAMEVERSION             "HALLOWEEN 1.0b-Mac DEMO (C) Copyright 2001~2017 Jadeware"
#else
#define GAMEVERSION             "HALLOWEEN 1.0b-Mac (C) Copyright 2001~2017 Jadeware"
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

#define	TRACK_PRES1			"pres_1.mp3"
#define	TRACK_PRES2			"pres_2.mp3"
#define	TRACK_PRES3			"pres_3.mp3"
#define	TRACK_PRES4			"pres_4.mp3"
#define	TRACK_0				"Halloween0.mp3"
#define	TRACK_1				"Halloween1.mp3"
#define	TRACK_2				"Halloween2.mp3"
#define	TRACK_3				"Halloween3.mp3"
#define	TRACK_4				"Halloween4.mp3"
#define	TRACK_5				"Halloween5.mp3"
#define	TRACK_6				"Halloween6.mp3"
#define	TRACK_7				"Halloween7.mp3"

#define	TRACK_PB			"partyboy.mp3"

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


#endif



