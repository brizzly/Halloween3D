//==========================//
//	DIRECT INPUT HEADER		//
//==========================//

#ifndef INPUT_INCLUDED
#define INPUT_INCLUDED

//#include "bsptree.h"
#include "alloween.h"

typedef struct MouseInputTag
{
	hBool	button_A;
	hBool	button_B;
	hBool	button_C;
	float	offset_X;
	float	offset_Y;

}	hMOUSEINPUT;


#ifdef H_WINDOWS

#define	DIRECTINPUT_VERSION 0x800 // 0x500

#include <windows.h>
#include <dinput.h>

#endif

//==========================================//
//											//
//		DIRECTX ROUTINES POUR LES JEUX		//
//			(C) 1999 Jadeware				//
//											//
//==========================================//

#define		K_BACKSPACE		0x0E
#define		K_LCTRL			0x1D
#define		K_RCTRL			0x9D
#define		K_LSHIFT		0x2A
#define		K_RSHIFT		0x36
#define		K_LALT			0x38
#define		K_RALT			0xB8
#define		K_F1			0x3B
#define		K_F2			0x3C
#define		K_F3			0x3D
#define		K_F4			0x3E
#define		K_F5			0x3F
#define		K_F6			0x40
#define		K_F7			0x41
#define		K_F8			0x42
#define		K_F9			0x43
#define		K_F10			0x44
#define		K_F11			0x57
#define		K_F12			0x58
#define		K_PAUSE			0xC5
#define		K_HOME			0xC7
#define		K_PGUP			0xC9
#define		K_PGDN			0xD1
#define		K_UPARROW		0xC8
#define		K_DOWNARROW		0xD0
#define		K_LEFTARROW		0xCB
#define		K_RIGHTARROW	0xCD
#define		K_END			0xCF
#define		K_INS			0xD2
#define		K_DEL			0xD3

#define		KEYMASK_SIZE	128
#define		KEY_BUFFER_MAX	8

//dword		Touche;


hBool					IN_Init(HINSTANCE hInstance);
void					IN_UnInit();
void					IN_WarpMouse();

void					m_UpdateCursor(hMOUSEINPUT Mouse, float *pCursor_x, float *pCursor_y);
void					m_ProcessInputs(hMOUSEINPUT Mouse);
void					m_ReadInputs(hMOUSEINPUT *Mouse);
int						ReadKey();
hBool					ReacquireInput();
hBool					UnacquireInput();

int						di_GetIdKey();
hBool					di_IsKeyActivated(int id, hMOUSEINPUT Mouse);
void					DI_ReadKeyboard();
void					DI_ReadMouse();
void					DI_ReacquireMouse();
void					DI_ReacquireKeyboard();
void					DI_UnacquireMouse();
void					DI_UnacquireKeyboard();




#ifdef AZERTY
static byte	scantokey[128] = 
{ 
//  0           1       2       3       4       5       6       7 
//  8           9       A       B       C       D       E       F 
	0  ,    27,     '1',    '2',    '3',    '4',    '5',    '6', 
	'7',    '8',    '9',    '0',    '-',    '=',    K_BACKSPACE, 9, // 0 
	'a',    'z',    'e',    'r',    't',    'y',    'u',    'i', 
	'o',    'p',    '[',    ']',    13 ,    K_LCTRL,'q',  's',      // 1 
	'd',	'f',    'g',    'h',    'j',    'k',    'l',    'm', 
	'\'' ,	'`',    K_LSHIFT,'\\',  'w',    'x',    'c',    'v',      // 2 
	'b',    'n',    ',',    ';',    ':',    '!',    K_RSHIFT,'*', 
	K_LALT,' ',   0  ,    K_F1, K_F2, K_F3, K_F4, K_F5,   // 3 
	K_F6, K_F7, K_F8, K_F9, K_F10, K_PAUSE  ,    0  , K_HOME, 
	K_UPARROW,K_PGUP,'-',K_LEFTARROW,'5',K_RIGHTARROW,'+',K_END, //4 
	K_DOWNARROW,K_PGDN,K_INS,K_DEL,0,0,             0,              K_F11, 
	K_F12,0  ,    0  ,    0  ,    0  ,    0  ,    0  ,    0,        // 5 
	0  ,    0  ,    0  ,    0  ,    0  ,    0  ,    0  ,    0, 
	0  ,    0  ,    0  ,    0  ,    0  ,    0  ,    0  ,    0,        // 6 
	0  ,    0  ,    0  ,    0  ,    0  ,    0  ,    0  ,    0, 
	0  ,    0  ,    0  ,    0  ,    0  ,    0  ,    0  ,    0         // 7 
};
static byte	shiftscantokey[128] = 
{ 
//  0           1       2       3       4       5       6       7 
//  8           9       A       B       C       D       E       F 
	0  ,    27,     '!',    '@',    '#',    '$',    '%',    '^', 
	'&',    '_',    '(',    ')',    '°',    '+',    K_BACKSPACE, 9, // 0 
	'A',    'Z',    'E',    'R',    'T',    'Y',    'U',    'I', 
	'O',    'P',    '{',    '}',    13 ,    K_LCTRL,'Q',  'S',      // 1 
	'D',    'F',    'G',    'H',    'J',    'K',    'L',    'M', 
	'"' ,    '~',    K_LSHIFT,'|',  'W',    'X',    'C',    'V',      // 2 
	'B',    'N',    '?',    '.',    '/',    '§',    K_RSHIFT,'*', 
	K_LALT,' ',   0  ,    K_F1, K_F2, K_F3, K_F4, K_F5,   // 3 
	K_F6, K_F7, K_F8, K_F9, K_F10, K_PAUSE  ,    0  , K_HOME, 
	K_UPARROW,K_PGUP,'_',K_LEFTARROW,'%',K_RIGHTARROW,'+',K_END, //4 
	K_DOWNARROW,K_PGDN,K_INS,K_DEL,0,0,             0,              K_F11, 
	K_F12,0  ,    0  ,    0  ,    0  ,    0  ,    0  ,    0,        // 5 
	0  ,    0  ,    0  ,    0  ,    0  ,    0  ,    0  ,    0, 
	0  ,    0  ,    0  ,    0  ,    0  ,    0  ,    0  ,    0,        // 6 
	0  ,    0  ,    0  ,    0  ,    0  ,    0  ,    0  ,    0, 
	0  ,    0  ,    0  ,    0  ,    0  ,    0  ,    0  ,    0         // 7 
}; 

#else

static byte	scantokey[128] = 
{ 
//  0           1       2       3       4       5       6       7 
//  8           9       A       B       C       D       E       F 
	0  ,    27,     '1',    '2',    '3',    '4',    '5',    '6', 
	'7',    '8',    '9',    '0',    '-',    '=',    K_BACKSPACE, 9, // 0 
	'q',    'w',    'e',    'r',    't',    'y',    'u',    'i', 
	'o',    'p',    '[',    ']',    13 ,    K_LCTRL,'a',  's',      // 1 
	'd',    'f',    'g',    'h',    'j',    'k',    'l',    ';', 
	'\'' ,    '`',    K_LSHIFT,'\\',  'z',    'x',    'c',    'v',      // 2 
	'b',    'n',    'm',    ',',    '.',    '/',    K_RSHIFT,'*', 
	K_LALT,' ',   0  ,    K_F1, K_F2, K_F3, K_F4, K_F5,   // 3 
	K_F6, K_F7, K_F8, K_F9, K_F10, K_PAUSE  ,    0  , K_HOME, 
	K_UPARROW,K_PGUP,'-',K_LEFTARROW,'5',K_RIGHTARROW,'+',K_END, //4 
	K_DOWNARROW,K_PGDN,K_INS,K_DEL,0,0,             0,              K_F11, 
	K_F12,0  ,    0  ,    0  ,    0  ,    0  ,    0  ,    0,        // 5 
	0  ,    0  ,    0  ,    0  ,    0  ,    0  ,    0  ,    0, 
	0  ,    0  ,    0  ,    0  ,    0  ,    0  ,    0  ,    0,        // 6 
	0  ,    0  ,    0  ,    0  ,    0  ,    0  ,    0  ,    0, 
	0  ,    0  ,    0  ,    0  ,    0  ,    0  ,    0  ,    0         // 7 
};
static byte	shiftscantokey[128] = 
{ 
//  0           1       2       3       4       5       6       7 
//  8           9       A       B       C       D       E       F 
	0  ,    27,     '!',    '@',    '#',    '$',    '%',    '^', 
	'&',    '*',    '(',    ')',    '_',    '+',    K_BACKSPACE, 9, // 0 
	'Q',    'W',    'E',    'R',    'T',    'Y',    'U',    'I', 
	'O',    'P',    '{',    '}',    13 ,    K_LCTRL,'A',  'S',      // 1 
	'D',    'F',    'G',    'H',    'J',    'K',    'L',    ':', 
	'"' ,    '~',    K_LSHIFT,'|',  'Z',    'X',    'C',    'V',      // 2 
	'B',    'N',    'M',    '<',    '>',    '?',    K_RSHIFT,'*', 
	K_LALT,' ',   0  ,    K_F1, K_F2, K_F3, K_F4, K_F5,   // 3 
	K_F6, K_F7, K_F8, K_F9, K_F10, K_PAUSE  ,    0  , K_HOME, 
	K_UPARROW,K_PGUP,'_',K_LEFTARROW,'%',K_RIGHTARROW,'+',K_END, //4 
	K_DOWNARROW,K_PGDN,K_INS,K_DEL,0,0,             0,              K_F11, 
	K_F12,0  ,    0  ,    0  ,    0  ,    0  ,    0  ,    0,        // 5 
	0  ,    0  ,    0  ,    0  ,    0  ,    0  ,    0  ,    0, 
	0  ,    0  ,    0  ,    0  ,    0  ,    0  ,    0  ,    0,        // 6 
	0  ,    0  ,    0  ,    0  ,    0  ,    0  ,    0  ,    0, 
	0  ,    0  ,    0  ,    0  ,    0  ,    0  ,    0  ,    0         // 7 
}; 
#endif


static const char * KeyToStringName[260] =
{
	"",
	"DIK_ESCAPE",
	"1",
	"2",
	"3",
	"4",
	"5",
	"6",
	"7",
	"8",
	"9",
	"0",
	"MINUS",
	"EQUALS",
	"BACK",
	"TAB",
	"Q",
	"W",
	"E",
	"R",
	"T",
	"Y",
	"U",
	"I",
	"O",
	"P",
	"LBRACKET",
	"RBRACKET",
	"RETURN",
	"LCONTROL",
	"A",
	"S",
	"D",
	"F",
	"G",
	"H",
	"J",
	"K",
	"L",
	"SEMICOLON",
	"APOSTROPHE",
	"GRAVE",
	"LSHIFT",
	"BACKSLASH",
	"Z",
	"X",
	"C",
	"V",
	"B",
	"N",
	"M",
	"COMMA",
	"PERIOD",
	"SLASH",
	"RSHIFT",
	"MULTIPLY",
	"LMENU",
	"SPACE",
	"CAPITAL",
	"F1",
	"F2",
	"F3",
	"F4",
	"F5",
	"F6",
	"F7",
	"F8",
	"F9",
	"F10",
	"NUMLOCK",
	"SCROLL",
	"NUMPAD7",
	"NUMPAD8",
	"NUMPAD9",
	"SUBTRACT",
	"NUMPAD4",
	"NUMPAD5",
	"NUMPAD6",
	"ADD",
	"NUMPAD1",
	"NUMPAD2",
	"NUMPAD3",
	"NUMPAD0",
	"DECIMAL",	// 0x53
	"???",	// 0x54
	"???",	// 0x55
	"???",	// 0x56
	"F11",	// 0x57
	"F12",	// 0x58
	"???",	// 0x59
	"???",	// 0x5A
	"???",	// 0x5B
	"???",	// 0x5C
	"???",	// 0x5D
	"???",	// 0x5E
	"???",	// 0x5F
	"???",	// 0x60
	"???",	// 0x61
	"???",	// 0x62
	"???",	// 0x63
	"F13",	// 0x64
	"F14",	// 0x65
	"F15",	// 0x66
	"???",	// 0x67
	"???",	// 0x68
	"???",	// 0x69
	"???",	// 0x6A
	"???",	// 0x6B
	"???",	// 0x6C
	"???",	// 0x6D
	"???",	// 0x6E
	"???",	// 0x6F
	"KANA",	// 0x70
	"???",	// 0x71
	"???",	// 0x72
	"???",	// 0x73
	"???",	// 0x74
	"???",	// 0x75
	"???",	// 0x76
	"???",	// 0x77
	"???",	// 0x78
	"CONVERT",	// 0x79
	"???",	// 0x7A
	"NOCONVERT",// 0x7B
	"???",	// 0x7C
	"YEN",	// 0x7D
	"???",	// 0x7E
	"???",	// 0x7F
	"???",	// 0x80
	"???",	// 0x81
	"???",	// 0x82
	"???",	// 0x83
	"???",	// 0x84
	"???",	// 0x85
	"???",	// 0x86
	"???",	// 0x87
	"???",	// 0x88
	"???",	// 0x89
	"???",	// 0x8A
	"???",	// 0x8B
	"???",	// 0x8C
	"NUMPADEQUALS",	// 0x8D
	"???",	// 0x8E
	"???",	// 0x8F
	"CIRCUMFLEX",	// 090
	"AT",
	"COLON",
	"UNDERLINE",
	"KANJI",
	"STOP",
	"AX",
	"UNLABELED",// 0x97
	"???",	// 0x98
	"???",	// 0x99
	"???",	// 0x9A
	"???",	// 0x9B
	"NUMPADENTER",	// 0x9C
	"RCONTROL",	// 0x9D
	"???",	// 0x9E
	"???",	// 0x9F
	"???",	// 0xA0
	"???",	// 0xA1
	"???",	// 0xA2
	"???",	// 0xA3
	"???",	// 0xA4
	"???",	// 0xA5
	"???",	// 0xA6
	"???",	// 0xA7
	"???",	// 0xA8
	"???",	// 0xA9
	"???",	// 0xAA
	"???",	// 0xAB
	"???",	// 0xAC
	"???",	// 0xAD
	"???",	// 0xAE
	"???",	// 0xAF
	"???",	// 0xB0
	"???",	// 0xB1
	"???",	// 0xB2
	"NUMPADCOMMA",	// 0xB3
	"???",	// 0xB4
	"DIVIDE",	// 0xB5
	"???",	// 0xB6
	"SYSRQ",// 0xB7
	"RMENU",// 0xB8
	"???",	// 0xB9
	"???",	// 0xBA
	"???",	// 0xBB
	"???",	// 0xBC
	"???",	// 0xBD
	"???",	// 0xBE
	"???",	// 0xBF
	"???",	// 0xC0
	"???",	// 0xC1
	"???",	// 0xC2
	"???",	// 0xC3
	"???",	// 0xC4
	"PAUSE",// 0xC5
	"???",	// 0xC6
	"HOME",	// 0xC7
	"UP",	// 0xC8
	"PRIOR",// 0xC9
	"???",	// 0xCA
	"LEFT",	// 0xCB
	"???",	// 0xCC
	"RIGHT",// 0xCD
	"???",	// 0xCE
	"END",	// 0xCF
	"DOWN",	// 0xD0
	"NEXT",	// 0xD1
	"INSERT",	// 0xD2
	"DELETE",	// 0xD3
	"???",	// 0xD4
	"???",	// 0xD5
	"???",	// 0xD6
	"???",	// 0xD7
	"???",	// 0xD8
	"???",	// 0xD9
	"???",	// 0xDA
	"LWIN",	// 0xDB
	"RWIN",	// 0xDC
	"APPS",	// 0xDD
	"POWER",// 0xDE
	"SLEEP"	// 0xDF
	"???",	// 0x
	"???",	// 0x
	"???",	// 0x
	"???",	// 0x
	"???",	// 0x
	"???",	// 0x
	"???",	// 0x
	"???",	// 0x
	"???",	// 0x
	"???",	// 0x
	"???",	// 0x
	"???",	// 0x
	"???",	// 0x
	"???",	// 0x
	"???",	// 0x
	"???",	// 0x
	"???",	// 0x
	"???",	// 0x
	"???",	// 0x
	"???",	// 0x
	"???",	// 0x
	"???",	// 0x
	"???",	// 0x
	"???",	// 0x
	"???",	// 0x
	"???",	// 0x
	"???",	// 0x
	"???",	// 0x
	"???",	// 0x
	"???",	// 0x
	"???",	// 0x
	"???"	// 0xFF
	"bug1",	// 0x001
	"bug2",	//
	"MOUSE1",
	"MOUSE2",
	"MOUSE3"
};


#endif INPUT_INCLUDED
