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


#ifndef API_KINPUT__
#define API_KINPUT__



#define PAD_NONE		0
#define PAD_UP			0x00000001l
#define PAD_DOWN		0x00000002l
#define PAD_LEFT		0x00000004l
#define PAD_RIGHT		0x00000008l
#define PAD_BUTTON1		0x00000010l
#define PAD_BUTTON2		0x00000020l
#define PAD_BUTTON3		0x00000040l
#define PAD_BUTTON4		0x00000080l
#define PAD_BUTTON5		0x00000100l
#define PAD_BUTTON6		0x00000200l
#define PAD_BUTTON7		0x00000400l
#define PAD_BUTTON8		0x00000800l
#define PAD_BUTTON9		0x00001000l
#define PAD_BUTTON10	0x00002000l


#define		ISDOWN			true
#define		ISUP			false

#define LAYOUTSIZE		101



enum		EKeyboardLayout	{ 
	K_VK_LEFT=0  ,
	K_VK_UP ,
	K_VK_DOWN ,
	K_VK_RIGHT	,
	K_VK_SPACE , 
	K_VK_L_SHIFT , 
	K_VK_R_SHIFT ,
	K_VK_RETURN ,
	K_VK_R_CONTROL , 
	K_VK_L_CONTROL ,
	K_VK_F1 ,
	K_VK_F2 ,
	K_VK_F3 , K_VK_F4 , K_VK_F5 , K_VK_F6 , K_VK_F7 , K_VK_F8 , K_VK_F9 , K_VK_F10 ,K_VK_F11 , K_VK_F12 ,
	K_VK_BACK , K_VK_TAB ,
	K_VK_ESCAPE ,
	K_VK_A ,
	K_VK_B ,
	K_VK_C ,
	K_VK_D ,
	K_VK_E ,
	K_VK_F ,
	K_VK_G ,
	K_VK_H ,
	K_VK_I ,
	K_VK_J ,
	K_VK_K ,
	K_VK_L ,
	K_VK_M ,
	K_VK_N ,
	K_VK_O ,
	K_VK_P ,
	K_VK_Q ,
	K_VK_R ,
	K_VK_S ,
	K_VK_T ,
	K_VK_U ,
	K_VK_V ,
	K_VK_W ,
	K_VK_X ,
	K_VK_Y ,
	K_VK_Z ,
	K_VK_0,
	K_VK_1,
	K_VK_2,
	K_VK_3 ,
	K_VK_4 ,
	K_VK_5 ,
	K_VK_6 ,
	K_VK_7 ,
	K_VK_8 ,
	K_VK_9,
	K_VK_NUM0 ,K_VK_NUM1 ,K_VK_NUM2 ,K_VK_NUM3 ,K_VK_NUM4 ,K_VK_NUM5 ,K_VK_NUM6 ,K_VK_NUM7 ,K_VK_NUM8 ,K_VK_NUM9  ,
	K_VK_MULTIPLY ,
	K_VK_ADD ,
	K_VK_SUBTRACT ,
	K_VK_DECIMAL ,
	K_VK_DIVIDE ,
	K_VK_CLEAR ,
	K_VK_MENU ,
	K_VK_LWIN ,
	K_VK_RWIN , 
	K_VK_NUMLOCK ,
	K_VK_SCROLL ,
	K_VK_OEM_1,K_VK_OEM_PLUS ,      
	K_VK_OEM_COMMA ,  
	K_VK_OEM_MINUS   ,  
	K_VK_OEM_PERIOD , 
	K_VK_OEM_2     ,     
	K_VK_OEM_3     ,     
	K_VK_OEM_4      ,    
	K_VK_OEM_5     ,     
	K_VK_OEM_6    ,      
	K_VK_OEM_7  ,
	K_VK_END , 
	K_VK_HOME , K_VK_DEL , 
	K_VK_INSERT ,
	K_VK_SNAPSHOT ,
	K_VK_PRIOR ,
	K_VK_NEXT ,
	K_VK_ERROR       
};

struct	KeyStroke
{
	EKeyboardLayout		toucheID;
	unsigned short		KeyCode;
};


//extern	KeyStroke			KeyboardLayout[LAYOUTSIZE] ;





struct	sFinger	
{
	short		x,y ;
	bool		pressed ;
};

class KInput
{
private:
	
	
	static sFinger			finger[2] ;
	
	static int		KeyboardLayout[LAYOUTSIZE];

	
	static bool		_screenPressed;
	static int		_xScreenHit;
	static int		_yScreenHit;
	static int		_tapeCount;
	static float	_xAcc;
	static float	_yAcc;
	static float	_zAcc;
	
	static int		_screen_width;
	static int		_screen_height;
	
	static int		midX, midY;
	static			bool	_btPressed ;
	static			bool	_leftMousePressed;
	static			bool	_rightMousePressed;

	static			float	_mx,_my ;
	
public:
	
	//static KJoypad * joypad;
	
public:
		
	static void		initInput(int screen_width, int screen_height);
	static long		readJoypad();
	static void		showPointer();
	static void		hidePointer();
	static int		getScreenWidth();
	static int		getScreenHeight();
	static void		setKeyPressed(int keycode);
	static void		setKeyUnPressed(int keycode);
	static bool		isPressed(int keyCode);
	static void		mousePointerTo(int x, int y);
	static void 	setMousePos(float valX, float valY);
	static void 	setMouseX(float val);
	static void 	setMouseY(float val);
	static int		getMouseX();
	static int		getMouseY();		
	static bool		getLeftButtonState();
	static	bool	getRightButtonState();
	static	bool	getMiddleButtonState();
	static void 	setLeftButtonState(bool value);
	static void 	setRightButtonState(bool value);
	
	// iPhone specific 
	static float	getAccelerationX();
	static float	getAccelerationY();
	static float	getAccelerationZ();
	static void		setAccelerometer(float x, float y, float z);
	static int		getScreenTap();
	static bool		isScreenPressed(int *hitX=0, int *hitY=0);
	static bool		isScreenReleased(int *hitX, int *hitY);
	//max 2 fingers  : 0 or 1
	static	sFinger*		getFinger( short fingerid ) ;
	

	//do not use unless you know what you're doing
	static void		setFingerPosition( int fingerid , short	x , short y , bool pressed ) ;
	static void		swapFingerPosition(int fingerid1, int fingerid2);
	
	static void		setScreenPressed(int tapCount, int x, int y);
	static void		setScreenMoving(int x, int y);
	static void		setScreenReleased();
	

	
};

#endif // API_KINPUT__
