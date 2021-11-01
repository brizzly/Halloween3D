
#include "alloween.h"
#include "input.h"

#ifdef H_MAC
#include "KInput.h"
extern KWindow *fenetre;
#endif


int		KeyBuffer[KEY_BUFFER_MAX];
hBool		*KeyMask=NULL;
int		MouseX;
int		MouseY;
int		Last_MouseX;
int		Last_MouseY;
float		Step = 0;
hBool		MouseKeyA=false;
hBool		MouseKeyB=false;
hBool		MouseKeyA_clicked;
hBool		MouseKeyB_clicked;
hBool		gScanKeyActivated = false;
int		gIdGameKey;
int		gIdScannedKey;
hBool		gPlayerCrouch=false;
float		gPlayerCrouchStep = 0;
hBool		gPlayerRun=false;
hBool		gClientJumpKey;
KEYSINPUT	KeysInput[K_MAXKEYS+4];
hMOUSEINPUT	MouseInput;
dword		Touche;
int			gGameIdKeys[MAX_GAMEKEY+4];
hBool		IsMouseCursor = true;
hBool 		msclic_A;
hBool 		msclic_B;
float		msoffsetX;
float		msoffsetY;
bool		gKeyHasBeenPressed;


#ifdef H_MAC
const char * KeyToStringName_mac[128] =
{
    //	0	1	2	3	4	5	6	7	8	9
        "q",	"s",	"d",	"f",	"h",	"g",	"w",	"x",	"c",	"v",	// 0	
        "",	"b",	"a",	"z",	"e",	"r",	"",	"t",	"1",	"2",	// 10
        "3",	"4",	"6",	"5",	"",	"9",	"7",	"",	"8",	"0",	// 20
        "#",	"o",	"u",	"",	"i",	"p",	"return","l",	"j",	"",	// 30
        "k",	"m",	"",	"",	"",	"n",	"",	"",	"tab",	"space",// 40
        "",	"back",	"",	"esc",	"",	"",	"",	"",	"",	"",	// 50
        "",	"",	"",	"",	"",	"",	"",	"*",	"",	"+",	// 60
        "",	"",	"",	"",	"",	"/",	"enter","",	"-",	"",	// 70
        "",	"",	"np0",	"np1",	"np2",	"np3",	"np4",	"np5",	"np6",	"np7",	// 80
        "",	"np8",	"np9",	"",	"",	"",	"",	"",	"",	"",	// 90
        "",	"",	"",	"",	"",	"",	"",	"",	"",	"",	// 100
        "",	"",	"",	"",	"",	"",	"",	"",	"",	"",	// 110
        "",	"",	"",	"left",	"right","down",	"up",	""	//
};

int NomDeTouches[] = {
0, //"LEFT",
0, //"UP",
0, //"DOWN",
0, //"RIGHT"	,
' ', //"SPACE" , 
0, //"L_SHIFT" , 
0, //"R_SHIFT" ,
0, //"RETURN" ,
0, //"R_CONTROL" , 
0, //"L_CONTROL" ,
0, //"F1" ,
0, //"F2" ,
0, //"F3" ,
0, // "F4",
0, // "F5" ,
0, // "F6" ,
0, // "F7" ,
0, // "F8" ,
0, // "F9" ,
0, // "F10" ,
0, // "F11" ,
0, // "F12" ,
0, //"BACK" ,
0, // "TAB" ,
0, //"ESCAPE" ,
'a', //"A" ,
'b', //"B" ,
'c', //"C" ,
'd', //"D" ,
'e', //"E" ,
'f', //"F" ,
'g', //"G" ,
'h', //"H" ,
'i', //"I" ,
'j', //"J" ,
'k', //"K" ,
'l', //"L" ,
'm', //"M" ,
'n', //"N" ,
'o', //"O" ,
'p', //"P" ,
'q', //"Q" ,
'r', //"R" ,
's', //"S" ,
't', //"T" ,
'u', //"U" ,
'v', //"V" ,
'w', //"W" ,
'x', //"X" ,
'y', //"Y" ,
'z', //"Z" ,
'0', //"0",
'1', //"1",
'2', //"2",
'3', //"3" ,
'4', //"4" ,
'5', //"5" ,
'6', //"6" ,
'7', //"7" ,
'8', //"8" ,
'9', //"9",
'0', //"NUM0" ,
'1', //"NUM1" ,
'2', //"NUM2" ,
'3', //"NUM3" ,
'4', //"NUM4" ,
'5', //"NUM5" ,
'6', //"NUM6" ,
'7', //"NUM7" ,
'8', //"NUM8" ,
'9', //"NUM9"  ,
'*', //"MULTIPLY" ,
'+', //"ADD" ,
'-', //"SUBTRACT" ,
0, //"DECIMAL" ,
'/', //"DIVIDE" ,
0, //"CLEAR" ,
0, //"MENU" ,
0, //"LWIN" ,
0, //"RWIN" , 
0, //"NUMLOCK" ,
0, //"SCROLL" ,
0, //"OEM_1,
0, //"OEM_PLUS" ,      
',', //"OEM_COMMA" ,  
0, //"OEM_MINUS"   ,  
'.', //"OEM_PERIOD" ,
0, //"OEM_2"     ,     
0, //"OEM_3"     ,     
0, //"OEM_4"      ,    
0, //"OEM_5"     ,     
0, //"OEM_6",      
0, //"OEM_7"
0, //K_VK_END
0, //K_VK_HOME
0, //K_VK_DEL
0, //K_VK_INSERT
0, //K_VK_SNAPSHOT
0, //K_VK_PRIOR
0, //K_VK_NEXT
0, //K_VK_ERROR
};

char *NomDeTouches2[] = 
{
	"LEFT", "UP", "DOWN", "RIGHT",
	"SPACE" , "L_SHIFT" , "R_SHIFT", "RETURN" ,
	"R_CONTROL" , "L_CONTROL",
	"F1" , "F2" , "F3" , "F4", "F5" , "F6" , "F7" , "F8" , "F9" , "F10" , "F11" , "F12" ,
	"BACK", "TAB", "ESCAPE",
	"A" , "B" , "C" , "D" , "E" , "F" , "G" , "H" , "I" , "J" , "K" , "L" , "M" , "N" , "O" ,
	"P" , "Q" , "R" , "S" , "T" , "U" , "V" , "W" , "X" , "Y" , "Z" , "0", "1", "2", "3" , "4" ,
	"5" , "6" , "7" , "8" , "9",
	"NUM0" ,"NUM1" ,"NUM2" ,"NUM3", "NUM4", "NUM5","NUM6","NUM7","NUM8","NUM9",
	"MULTIPLY" , "ADD" , "SUBTRACT", "DECIMAL", "DIVIDE", "CLEAR", "MENU", "LWIN", "RWIN", 
	"NUMLOCK" , "SCROLL" , "OEM_1", "OEM_PLUS", "OEM_COMMA", "OEM_MINUS", "OEM_PERIOD", 
	"OEM_2", "OEM_3", "OEM_4", "OEM_5", "OEM_6", "OEM_7", "END", "HOME","DEL", "INSERT",
	"SNAPSHOT", "PRIOR", "NEXT", "ERROR"
};

#endif


hBool IN_Init()
{
	int	i;

	if(gIsServer && net_dedicated.value)
		return true;
		
#ifdef H_MAC
	KInput::initInput(fenetre->getWindowWidth(), fenetre->getWindowHeight());
#endif

#ifdef H_LINUX
    SDL_EnableUNICODE(1);
#endif
	gKeyHasBeenPressed = false;
	MouseX = 0;
	MouseY = 0;
	Last_MouseX = 666;
	Last_MouseY = 666;
	msclic_A = false;
	msclic_B = false;
  	MouseKeyA_clicked = false;
	MouseKeyB_clicked = false;
	msoffsetX = 0;
	msoffsetY = 0;
	//if(fullscreen.value == true)
	IN_HideMouse();
   	IN_SetCursorPos(320, 240);
	for(i=0 ; i<K_MAXKEYS ; i++)
	{
		KeysInput[i].key = 0;
		strcpy(KeysInput[i].keyname, "");
	}
	for(i=0 ; i<KEY_BUFFER_MAX ; i++)
	{
		KeyBuffer[i] = -1;
	}
	IN_LoadKeys();

	return true;
}

void IN_UnInit()
{
	if(gIsServer && net_dedicated.value)
		return;

	IN_ShowMouse();
}

void IN_SetCursorPos(int x, int y)
{
#ifdef H_MAC
	KInput::mousePointerTo(x, y);
#endif

#ifdef H_LINUX
	SDL_WarpMouse(x, y);
#endif
	
	Last_MouseX = x;
	Last_MouseY = y;
}

void IN_HideMouse()
{
	//return;
	
	
	if(gIsServer && net_dedicated.value)
		return;
	IsMouseCursor = false;
	
#ifdef H_MAC
	KInput::hidePointer();
#endif

#ifdef H_LINUX
	SDL_ShowCursor(SDL_DISABLE);
#endif
}

void IN_ShowMouse()
{
	if(gIsServer && net_dedicated.value)
		return;
	IsMouseCursor = true;
	
#ifdef H_MAC
	KInput::hidePointer();
#endif

#ifdef H_LINUX	
	SDL_ShowCursor(SDL_DISABLE);
#endif
}

hBool IN_IsPressed(unsigned short keyCode)
{
	if(gIsServer && net_dedicated.value) {
		return false;
	}
	if(KeysInput[keyCode].key != 0) {
		return true;
	}
	return false;
}

hBool IN_IsMousePressed(unsigned short mbutton)
{
	if(gIsServer && net_dedicated.value)
		return false;
                
	switch((int)mbutton)
	{
		case 0:
		if(msclic_A) return true;
		break;

		case 2:
		if(msclic_B) return true;
		break;
	}

	return false;
}

void IN_LoadKeys()
{
	gGameIdKeys[0] = (int)gamekey_up.value;
	gGameIdKeys[1] = (int)gamekey_down.value;
	gGameIdKeys[2] = (int)gamekey_left.value;
	gGameIdKeys[3] = (int)gamekey_right.value;
	gGameIdKeys[4] = (int)gamekey_jump.value;
	gGameIdKeys[5] = (int)gamekey_crouch.value;
	gGameIdKeys[6] = (int)gamekey_attack.value;
	gGameIdKeys[7] = (int)gamekey_axe.value;
	gGameIdKeys[8] = (int)gamekey_arbalet.value;
	gGameIdKeys[9] = (int)gamekey_bombgun.value;
	gGameIdKeys[10] = (int)gamekey_tromblon.value;
	gGameIdKeys[11] = (int)gamekey_action.value;
	gGameIdKeys[12] = (int)gamekey_run.value;
}

void IN_SaveKey(int nKey, int KeyId, char *KeyName)
{
	gGameIdKeys[nKey] = KeyId;
}

void IN_ReadKeyboard()
{
	int i;
	#ifdef H_MAC //todo...farox
	gKeyHasBeenPressed = false;
	for(i=0 ; i<93+8 ; i++)
	{
        //#ifdef H_MAC		
        if(KInput::isPressed( (EKeyboardLayout)i))
        //#endif
		{
			gKeyHasBeenPressed = true;
			KeysInput[i].key = true;
		}
		else
		{
			KeysInput[i].key = false;
		} 
	}
    #endif
}

void IN_SetKeyboard(int keysym, hBool keydir)
{
	int	key;

#ifdef H_MAC

////#else  //farox
	key = keysym.scancode;

	if(keydir)
	{
	//	{
		//m_ConsPrint("key: %d\n", (int)key); //
			KeysInput[key].key = true;
			KeysInput[key].keysym = keysym;
			strncpy(KeysInput[key].keyname, SDL_GetKeyName(keysym.sym), 32); //
	//	}
	}
	else
	{
	//	{
		//m_ConsPrint("Unkey: %d\n", (int)key); //
			KeysInput[key].key = false;
	//	}
	}
#endif
}

void IN_SetMouseButons(int bstate)
{
	switch(bstate)
	{
		case 1:
		msclic_A = true;
          //  m_ConsPrint("mouse btn 1 press\n");
		break;

		case 2:
         //   m_ConsPrint("mouse btn 2 press\n");
		break;

		case 3:
		msclic_B = true;
         //   m_ConsPrint("mouse btn 3 press\n");
		break;

		case -1:
                msclic_A = false;
          //  m_ConsPrint("mouse btn 1 release\n");
		break;

		case -2:
        //    m_ConsPrint("mouse btn 2 release\n");
		break;

		case -3:
		msclic_B = false;
        //    m_ConsPrint("mouse btn 3 release\n");
		break;
	}
}

void IN_WarpMouse()
{
	IN_SetCursorPos(320, 240);
	return;
	
	// wrap mouse, sauf quand on joue pas, en mode fenetre
	if(fullscreen.value == true || ProgramState == PS_GAME)
	{
		if(IsMouseCursor == true)
		{
			IN_HideMouse();
		}

		IN_SetCursorPos(320, 240);
	}
	else
	{
		if(IsMouseCursor == false && fullscreen.value == false)
		{
			IN_ShowMouse();
		}
	}
}

long IN_ReadMouse()
{
#ifdef H_MAC
	float 		msx = (float)(KInput::getMouseX());
	float 		msy = (float)(KInput::getMouseY());
	

	if(0) //ProgramState != PS_GAME)
	{
		// windowed
//		MouseX = msx;
//		MouseY = msy;

//		if(MouseX > 640) 	MouseX = 640;
//		if(MouseX < 0) 		MouseX = 0;
//		if(MouseY > 480) 	MouseY = 480;
//		if(MouseY < 0) 		MouseY = 0;

		msoffsetX = msx - (float)Last_MouseX;
		msoffsetY = msy - (float)Last_MouseY;
		Last_MouseX = (int)msx;
		Last_MouseY = (int)msy;
		
		MouseX += 2.0f * msoffsetX;// * player_rvel.value;
		MouseY -= 2.0f * msoffsetY;// * player_rvel.value;
		
		//printf("msoffsetX=%f ; msoffsetY=%f\n",msoffsetX,msoffsetY);
	}
	else
	{
		//m_ConsPrint("msx=%f ; msy=%f\n",msx,msy);

		if(Last_MouseX == 666) Last_MouseX = msx;
		if(Last_MouseY == 666) Last_MouseY = msy;
		
		msoffsetX = msx - Last_MouseX;
		msoffsetY = msy - Last_MouseY;
		
		IN_SetCursorPos(320, 240);
		
		//Last_MouseX = 320;
		//Last_MouseY = 240;
		//IN_WarpMouse();
		
		MouseX += 5.0f * msoffsetX * player_rvel.value;
		MouseY -= 5.0f * msoffsetY * player_rvel.value;
		
		if(ProgramState != PS_GAME)
		{
			if(MouseX > 640) 	MouseX = 640;
			if(MouseX < 0) 		MouseX = 0;
			if(MouseY > 480) 	MouseY = 480;
			if(MouseY < 0) 		MouseY = 0;
		}
		
//		m_ConsPrint("msoffsetX=%f ; msoffsetY=%f\n",msx,msy);

	}
	
	/*
	if(fullscreen.value == false || ProgramState != PS_GAME)
	{
		MouseX += msoffsetX;
		MouseY -= msoffsetY;
	
		if(MouseX > 640) 	MouseX = 640;
		if(MouseX < 0) 		MouseX = 0;
		if(MouseY > 480) 	MouseY = 480;
		if(MouseY < 0) 		MouseY = 0;       
	}
	else
	{               
		MouseX += 2.0f * msx;// * player_rvel.value;
		MouseY += 2.0f * msy;// * player_rvel.value;
	}
	*/
	
	//printf("MouseX=%d ; MouseY=%d\n",MouseX,MouseY);
	

	if( KInput::getLeftButtonState() == ISDOWN )
	{
		IN_SetMouseButons(1);
	}
	else
	{
		IN_SetMouseButons(-1);
	}

	if( KInput::getRightButtonState() == ISDOWN )
	{
		IN_SetMouseButons(3);
	}
	else
	{
		IN_SetMouseButons(-3);
	}
	return 0;
#endif
}

void IN_MouseGetOffsetFromAbs(int x, int y)
{
	static int msx = 666;
	static int msy = 666;
	int	offx;
	int	offy;

	if(msx == 666) msx = x;
	if(msy == 666) msy = y;

	offx = x - msx;
	offy = y - msy;

	msx = x;
	msy = y;

	IN_MouseGetOffet(x, y, offx, offy);
}

void IN_MouseGetOffet(int x, int y, int relx, int rely)
{
	if(Last_MouseX == 666) Last_MouseX = x;
	if(Last_MouseY == 666) Last_MouseY = y;
        
	msoffsetX = x - Last_MouseX;
	msoffsetY = y - Last_MouseY;

	Last_MouseX = x;
	Last_MouseY = y;

	if(fullscreen.value == false || ProgramState != PS_GAME)
	{
		MouseX = x * (640.0f / ScreenX[(int)videomode.value]);
		MouseY = 480 - (y * (480.0f / ScreenY[(int)videomode.value]));
                
        #ifdef H_MAC
            if(fullscreen.value == false)
            {
                msoffsetY = -msoffsetY;
                MouseY = y * (480.0f / ScreenY[(int)videomode.value]);
            }
        #endif
        
	}
	else
	{               
		MouseX += 2.0f * msoffsetX * player_rvel.value;
		MouseY += 2.0f * msoffsetY * player_rvel.value;
	}
}

void IN_GetMouse(hMOUSEINPUT *Mouse)
{
	if(gIsServer && net_dedicated.value)
		return;
	
	//printf("msoffsetX=%f ; msoffsetY=%f\n", msoffsetX, msoffsetY);

	Mouse->offset_X = 2.0f * msoffsetX * player_rvel.value;
	Mouse->offset_Y = 2.0f * msoffsetY * player_rvel.value;
	msoffsetX = 0;
	msoffsetY = 0;

	//printf("Mouse->offset_X=%f ; Mouse->offset_Y=%f\n", Mouse->offset_X, Mouse->offset_Y);

	
	if(ProgramState == PS_GAME)
	if(invert_mouse.value != 0)
		Mouse->offset_Y = -Mouse->offset_Y;


	if(IN_IsMousePressed(0))
		Mouse->button_A = true;
	else
		Mouse->button_A = false;

	if(IN_IsMousePressed(2))
		Mouse->button_B = true;
	else
		Mouse->button_B = false;
}

int IN_GetIdKey()
{
	int		index;
	hBool		clicked;
	int		n_mouse;

	if(gIsServer && net_dedicated.value)
		return -1;

	gDrawCursor = false;
	DrawGameConfig_INPUT();
	menu_SelectProcess_CONFIG_INPUT();
	gl_SwapBuffer();
	gDrawCursor = true;
	clicked = false;
        n_mouse = 0;
	IsWaitKey = true;
	while(1)
	{
		sys_GameEvent();
	
		if(!clicked)
		{
			if(msclic_A)
			{
				n_mouse = 256;
				clicked = true;
			}
			else if(msclic_B)
			{
				n_mouse = 258;
				clicked = true;
			}
		}
		else
		{
			if(	!(msclic_A) &&
				!(msclic_B) )
			{
				IsWaitKey = false;
				return n_mouse;
			}
		}

		for(index=0 ; index<K_MAXKEYS ; index++)
		if(KeysInput[index].key != 0)
		{
			IsWaitKey = false;
			return index;
		}
	}

	return -1;
}

int ReadKey()
{
	int		index;
	int		Key = 0;
	int		KeyPos;
	hBool	KeyHasBeenPressed;
	int		index2;
	bool	IsCaps;
	
	if(KeysInput[DIK_ESCAPE].key)
	      return 1;
    if(KeysInput[DIK_RETURN].key)
	      return 2;
	if(KeysInput[DIK_NUMPADENTER].key)
	      return 2;
	if(KeysInput[DIK_BACK].key)
	      return 3;
	if(KeysInput[DIK_UP].key)
	      return 5;
	if(KeysInput[DIK_TAB].key)
	      return 6;
	      
	IsCaps = false;
	KeyHasBeenPressed = false;

	for(index=0 ; index<KEYMASK_SIZE ; index++)
	{
		if(KeysInput[index].key != 0)
		{
			if(index == DIK_GRAVE) {
				continue;
			}
			if(index == DIK_LSHIFT)
			{
				IsCaps = true;
				continue;
			}
			if(index == DIK_LSHIFT)
			{
				IsCaps = true;
				continue;
			}
	
			if(KeyMask[index])
					continue;
			KeyMask[index] = true;
			ds_PlaySound(15);
			KeyHasBeenPressed = true;
		#ifdef H_MAC
			Key = NomDeTouches[index];
			if(Key)
			if(IsCaps)
			{
				if(Key == ',')
					Key = '.';
				else
				{
					Key -= 'a';
					Key += 'A';
				}
			}
		#else
			Key = index;
		#endif
			KeyPos = 0;
			while( KeyBuffer[KeyPos] >= 0 && KeyPos < KEY_BUFFER_MAX )
					KeyPos++;
			if(KeyPos>=KEY_BUFFER_MAX)
					break;
			for(index2=0 ; index2<KEY_BUFFER_MAX ; index2++)
			if(KeyBuffer[index2] == Key)
				break;
			KeyBuffer[KeyPos] = Key;
        }
	    else
		{
			KeyMask[index] = false;
		}
	}

     if(KeyHasBeenPressed)
     {
             return 4;
     }

	return 0;
}

hBool di_IsKeyActivated(int id, hMOUSEINPUT Mouse)
{
	if(id<0 || id>258)
		return false;

	if(id < 256)
	{
		if(KeysInput[id].key != 0)
			return true;
	}
	else
	{
		switch(id)
		{
			case 256:
			if(Mouse.button_A)
				return true;
			break;

			case 258:
			if(Mouse.button_B)
				return true;
			break;

			case 257:
		//	if(Mouse.button_C)
		//		return true;
			break;
		}
	}
	return false;
}

char *IN_GetKeyChar(int IdScannedKey)
{
	static char	label[64];
	int		unicode;

#ifdef H_MAC
	return (char*)IdScannedKey;
#else
	//unicode = KeysInput[IdScannedKey].keysym.unicode;
	//sprintf(label, "%c", (char)unicode);
	return label;
#endif
}

char *IN_GetKeyStringName(int IdScannedKey)
{
	switch(IdScannedKey)
	{
		case 256:
		return "LEFTMOUSE";

		case 258:
		return "RIGHTMOUSE";

		default:
		break;
	}
        
    #ifdef H_MAC
        if(IdScannedKey < 0) IdScannedKey = 0;
        if(IdScannedKey > K_MAXKEYS) IdScannedKey = 0;
        return NomDeTouches2[IdScannedKey];
    #else
	return (char*)KeyToStringName[IdScannedKey-8];
    #endif
}

// fin du systeme de gestion des inputs au niveau materiel










//
// ############### GESTION DES INPUTS DANS LE JEU ################################
//



void m_UpdateCursor(hMOUSEINPUT Mouse, float *pCursor_x, float *pCursor_y)
{
	*pCursor_x = MouseX;
	*pCursor_y = MouseY;

	if(*pCursor_x<0)
		*pCursor_x=0;
	if(*pCursor_x>640.0f)
		*pCursor_x=640.0f;

	if(*pCursor_y<0)
		*pCursor_y=0;
	if(*pCursor_y>480.0f)
		*pCursor_y=480.0f;
}

void IN_Process(hMOUSEINPUT Mouse, pEntity PlayerEnt)
{
	if(gIsServer && net_dedicated.value)
		return;
	// Read One click with no repetitions Mouse A
	if(Mouse.button_A)
	{
		if(!MouseKeyA_clicked)
		{
			MouseKeyA_clicked = true;
			MouseKeyA = true;
		}
		else
			MouseKeyA = false;
	}
	else
	{
		MouseKeyA = false;
		MouseKeyA_clicked=false;
	}

	// Read One click with no repetitions Mouse B
	if(Mouse.button_B)
	{
		if(!MouseKeyB_clicked)
		{
			MouseKeyB_clicked = true;
			MouseKeyB = true;
		}
		else
			MouseKeyB = false;
	}
	else
	{
		MouseKeyB = false;
		MouseKeyB_clicked=false;
	}

	switch(ProgramState)
	{
		case PS_HELP:
		m_UpdateCursor(Mouse,&cursor_x,&cursor_y);
		if(MouseKeyA)
			menu_ClickedA_HELP();
		if(MouseKeyB)
		{
			//ds_PlaySound(4);
			game_Init();
		}
		break;

		case PS_ORDER:
		m_UpdateCursor(Mouse,&cursor_x,&cursor_y);
		if(MouseKeyA)
			menu_ClickedA_ORDER();
		if(MouseKeyB)
		{
			//ds_PlaySound(4);
			game_Init();
		}
		break;

		case PS_BEGINGAME:
		m_UpdateCursor(Mouse,&cursor_x,&cursor_y);
		if(MouseKeyA)
			menu_ClickedA_BEGINGAME();
		if(MouseKeyB)
		{
			menu_ClickedB_BEGINGAME();
		//	ds_PlaySound(4);
		//	game_Init();
		}
		break;

		case PS_SETSLOTGAME:
		m_UpdateCursor(Mouse,&cursor_x,&cursor_y);
		if(MouseKeyA)
			menu_ClickedA_SETSLOTGAME();
		if(MouseKeyB)
		{
			//ds_PlaySound(4);
			game_Init();
		}
		break;

		case PS_YESNO:
		m_UpdateCursor(Mouse,&cursor_x,&cursor_y);
		if(MouseKeyA)
			menu_ClickedA_YESNO();
		if(MouseKeyB)
		{
			ds_PlaySound(4);
			game_Init();
		}
		break;

		case PS_SELECTMAP:
		m_UpdateCursor(Mouse,&cursor_x,&cursor_y);
		if(MouseKeyA)
			menu_ClickedA_SelectMAP();
		if(MouseKeyB)
		{
			ds_PlaySound(4);
			game_LaunchMapIntro();
		}
		break;

		case PS_STARTMAP:
		m_UpdateCursor(Mouse,&cursor_x,&cursor_y);
		if(MouseKeyA)
			menu_ClickedA_StartMAP();
		if(MouseKeyB)
		{
			ds_PlaySound(4);
			game_LaunchMapIntro();
		}
		break;

		case PS_PAUSEDGAME:
		m_UpdateCursor(Mouse,&cursor_x,&cursor_y);
		if(MouseKeyA)
			menu_ClickedA_PAUSED();
		if(MouseKeyB)
			menu_ClickedB_PAUSED();
		break;
		
		case PS_INTRO:
		game_Init();
		break;

		case PS_PRES:
		m_UpdateCursor(Mouse,&cursor_x,&cursor_y);
		if(MouseKeyA)
			menu_ClickedA_PRES();
		break;

		case PS_CONFIG:
		m_UpdateCursor(Mouse,&cursor_x,&cursor_y);
		if(MouseKeyA)
			menu_ClickedA_CONFIG();
		if(MouseKeyB)
			menu_ClickedB_CONFIG();
		break;

		case PS_CONFIG_INPUT:
		m_UpdateCursor(Mouse,&cursor_x,&cursor_y);
		if(MouseKeyA)
			menu_ClickedA_CONFIG_INPUT();
		if(gScanKeyActivated && MouseKeyA_clicked == false)
		{
			gScanKeyActivated = false;
			gIdScannedKey = IN_GetIdKey();
			//gGameIdKeys[gIdGameKey] = gIdScannedKey;
			#ifdef H_MAC
			IN_SaveKey(gIdGameKey, gIdScannedKey, NomDeTouches2[gIdScannedKey]);
			#else
			IN_SaveKey(gIdGameKey, gIdScannedKey, KeysInput[gIdScannedKey].keyname);
			#endif
			
//#ifdef H_WINDOWS
//			m_ConsPrint("touche: %s id: %d\n", //KeyToStringName[gIdScannedKey],gIdScannedKey);
//#endif

//#ifdef H_LINUX
			//m_ConsPrint("touche: %s id: %d\n", IN_GetKeyStringName(gIdScannedKey),gIdScannedKey);
//#endif
		}
		if(MouseKeyB)
			menu_ClickedB_CONFIG();
		break;

		case PS_CONFIG_GAMEOPTION:
		m_UpdateCursor(Mouse,&cursor_x,&cursor_y);
		if(MouseKeyA)
			menu_ClickedA_CONFIG_GAMEOPTION();
		if(MouseKeyB)
			menu_ClickedB_CONFIG();
		break;

		case PS_CONFIG_VIDEO:
		m_UpdateCursor(Mouse,&cursor_x,&cursor_y);
		if(MouseKeyA)
			menu_ClickedA_CONFIG_VIDEO();
		if(MouseKeyB)
			menu_ClickedB_CONFIG();
		break;

		case PS_CONFIG_SOUND:
		m_UpdateCursor(Mouse,&cursor_x,&cursor_y);
		if(MouseKeyA)
			menu_ClickedA_CONFIG_SOUND();
		if(MouseKeyB)
			menu_ClickedB_CONFIG();
		break;

		case PS_SHOWSHINFO:
		m_UpdateCursor(Mouse,&cursor_x,&cursor_y);
		if(MouseKeyA)
		{
			ds_PlaySound(4);
			menu_ClickedA_PRES();
		}
		break;

		case PS_GREETINGS:
		m_UpdateCursor(Mouse,&cursor_x,&cursor_y);
		if(MouseKeyA)
		{
			ds_PlaySound(4);
			menu_ClickedA_PRES();
		}
		break;

		case PS_WIN:
		m_UpdateCursor(Mouse,&cursor_x,&cursor_y);
		if(MouseKeyA)
		{
			ds_PlaySound(4);
			menu_ClickedA_YouWin();
			if(ProgramState == PS_GAMEOVER)
				return;
		}
		break;

		case PS_GAME:
		{
			if(player_view.value)
			{
				if(di_IsKeyActivated(gGameIdKeys[6],Mouse))
				{
					// PLAYER SHOOT
					switch(gPlayerHasGun)
					{
						case 1:
						SabreWulf();
						break;

						case 2:
						LaunchPlayer(FLECHE,1.0f);
						break;

						case 3:
						LaunchPlayer(BOMBE,0.5f);
						break;

						case 4:
						LaunchPlayer(SPINE,2.0f);
						break;

						default:
						PlayerSwapWeapon(1);
						break;
					}
				}

				if(!gIsMultiplayer || gIsServer)
				{
					// PLAYER JUMP
					if(di_IsKeyActivated(gGameIdKeys[4],Mouse))
					{
						if(PlayerEnt)
						{
							JumpCreature(PERSO);
							PlayerEnt->JumpPossible = false; 
						}
					}
					else
					{
						if(PlayerEnt)
							PlayerEnt->JumpPossible = true;
					}
				}
				else if(gIsMultiplayer && !gIsServer)
				{
					if(di_IsKeyActivated(gGameIdKeys[4],Mouse))
					{
						if(!gClientJumpKey)
						{
							gClientJumpKey = true;
							//gNetBitKeys = gNetBitKeys | (char)(0x01 << 3);
						}
					}
					else
					{
						gClientJumpKey = false;
					}
				}
			}
			r_TourneY(&gCamera, Mouse.offset_X, PlayerEnt);
			r_TourneX(&gCamera, Mouse.offset_Y, PlayerEnt);
		}
		break;
	}
}

void m_ProcessInputs(hMOUSEINPUT Mouse)
{
	
}

void m_ReadInputs(hMOUSEINPUT *Mouse)
{
	pEntity	PlayerEnt;
	
	IN_GetMouse(Mouse);

	//==================================================//
	//	GESTION DU CLAVIER
	//==================================================//
	if(IN_IsPressed(DIK_ESCAPE))
	{
		if(gIsMultiplayer && gMultiPodium)
		{
			net_StopMultiplayer();
			return;
		}

		if(ProgramState == PS_WIN)
		{
			game_WinTime = GAME_WINTIME;
			return;
		}
		else if(ProgramState == PS_GAMEOVER)
		{
			o_ClearEntityList();
			game_Init();
			return;
		}
		else if(ProgramState == PS_GAME || ProgramState == PS_DIE || ProgramState == PS_ENDSCORE)
		{
			saved_ProgramState = ProgramState;
			menu_InitPaused();
			return;
		}
		else if(ProgramState == PS_INTRO)
		{
			game_Init();
			return;
		}
		else if(ProgramState != PS_PRES && ProgramState != PS_PAUSEDGAME)
		{
			if(!ResumingAlloweed)
				game_Init();
			else
				menu_InitPaused();
		}
	}

	if(developer.value)
	{
		if(IN_IsPressed(DIK_ESCAPE))
		{
			if(ProgramState==PS_GAME)
			{
				if(!player_view.value)
				{
					gCamera.Pos.X = gStart.X;
					gCamera.Pos.Y = gStart.Y;
					gCamera.Pos.Z = gStart.Z;
					gCamera.Rot.X = 0;
					gCamera.Rot.Y = (float)gAngleView_player + 179.9f; //180.0f; // BUG BLOCKED
					gCamera.Rot.Z = 0;
				}
				else
				{
					PlayerResetStartPos(PERSO);
				}
			}
		}
	}
	
	if(Mouse->button_A)
	{
		if(ProgramState == PS_GAME)
		{
			if(player_view.value == 0)
				cam_MoveY(0,&gCamera);
		}
		if(gMapIntro && game_IsCameraShow())
		{
			game_StopCameraShow();
		}
	}
	if(Mouse->button_B)
	{
		if(ProgramState == PS_GAME)
		{
			if(player_view.value == 0)
				cam_MoveY(1,&gCamera);
		}
	}

	//SCREENSHOT KEY
	if(IN_IsPressed(DIK_NUMPAD5))
	{
		if(IsScreenShot==false)
		{
			IsScreenShot=true;
			im_ScreenShotTGA();
		}
	}
	else
		IsScreenShot=false;
	
	// CONSOLE KEY
	if(IN_IsPressed(DIK_GRAVE))
	{
		if(ProgramState != PS_INTRO && ProgramState != PS_OUTRO && ProgramState != PS_CREDIT)
		if(!gKey_Console)
		{
			gKey_Console=true;
			IsConsoleActive=!IsConsoleActive;
		}
	}
	else
		gKey_Console=false;

	// RETURN KEY
	if(IN_IsPressed(DIK_RETURN))
	{
		if(ProgramState == PS_INTRO)
		{
			game_Init();
			return;
		}

		if(gMapIntro && game_IsCameraShow())
		{
			game_StopCameraShow();
		}
	}

	// ACTION KEY
	if(di_IsKeyActivated(gGameIdKeys[11],*Mouse))
	{
		if(ProgramState == PS_GAME)
		{
			if(game_UseFiole() == false)
			{
				if(!gKey_Action)
				{
					gKey_Action = true;
					ab_Activate(gCamera.Pos);
				}

				if(game_InjuryMode == 0)
				{
					game_InjuryMode = 2000.0f;
					ds_PlaySound(76+randInt(0,9));
				}
				
				Story_NextText();
			}
		}
	}
	else
		gKey_Action=false;

	// MOVE Camera_t
	if(gCameraShowTime == 0 && !player_view.value && ProgramState == PS_GAME)
	{
		if(di_IsKeyActivated(gGameIdKeys[0],*Mouse))
		{
			cam_Move(0,&gCamera);
		}

		if(di_IsKeyActivated(gGameIdKeys[1],*Mouse))
		{
			cam_Move(180.0f,&gCamera);
		}

		if(di_IsKeyActivated(gGameIdKeys[2],*Mouse))
		{
			cam_Move(90.0f,&gCamera);
		}

		if(di_IsKeyActivated(gGameIdKeys[3],*Mouse))
		{
			cam_Move(270.0f,&gCamera);
		}
	}

	if(ProgramState == PS_WIN)
		goto no_input_move;

	// MOVE PLAYER
	if(!gIsMultiplayer)
	{
		if(!IsConsoleActive && gCameraShowTime == 0)
		{
			PlayerEnt = o_FindEntity(PERSO);
			IN_PlayerInputMove(Mouse, PlayerEnt);
		}
		else
		{
			PlayerEnt = o_FindEntity(PERSO);
			if(PlayerEnt)
			{
				PlayerEnt->ent_anim.Pos_Acc.X = 0;
				PlayerEnt->ent_anim.Pos_Acc.Y = 0;
				PlayerEnt->ent_anim.Pos_Acc.Z = 0;
			}
		}
	}
	else
	{
		if(ProgramState != PS_GAME && ProgramState != PS_DIE && ProgramState != PS_PAUSEDGAME)
			goto no_input_move;

		if(gIsServer)
		{
		}
		else
		{
			if(ProgramState == PS_PAUSEDGAME || IsConsoleActive)
				goto no_input_move;
		}

		PlayerEnt = o_FindEntity(PERSO);
		IN_PlayerInputMove(Mouse, PlayerEnt);

no_input_move:;
	}
}

void IN_PlayerInputMove(hMOUSEINPUT *Mouse, pEntity PlayerEnt)
{
	pEntity		ClientEnt;
	hBool		CL_IsWalk;
	hBool		CL_accelerate;

	if(!PlayerEnt)
		return;

        CL_accelerate = false; // hack ?
	gPlayerWalk = false;
	CL_IsWalk = false;

	/*
	// ACTIVATE CLIENT JUMP ON THE SERVER
	if(gIsMultiplayer && gIsServer)
	{
		if(gNetBitKeys & (0x01 << 3))
		{		
			if(gMultiSV_clentid != -1)
			{
				ClientEnt = o_FindEntity(gMultiSV_clentid);
				if(ClientEnt)
				{
					if(ClientEnt->Ground_Type)
					{
						JumpCreature(gMultiSV_clentid);
					}
				}
			}
		}
	}
	 */
	
	// met a zero le vecteur acceleration des entitee joueurs
	if(!gIsMultiplayer || gIsServer)
	{
		if(phy_IsVelocity(MORBAK))
		{
			CL_accelerate = true;
		}
		else
		{
			CL_accelerate = false;
		}

		phy_ResetVelocity(PERSO);
		phy_ResetVelocity(MORBAK);
	}

	// CLIENT FORWARD
	/*
	if(gIsMultiplayer && gIsServer)
	{
		if(gNetBitKeys & (0x01 << 7))
		{
			if(gMultiSV_clentid != -1)
			{
				ply_Movedir(gMultiSV_clentid, DIR_FORWARD);
			}
		}
	}
	else if(gIsMultiplayer && !gIsServer)
	{
		if(gNetBitKeys_back & (0x01 << 7))
		{
			if(gMultiSV_clentid != -1)
			{
				ply_Movedir(MORBAK, DIR_FORWARD);	// HACK
			}
		}
	}*/
	
	// SINGLEPLAYER/HOST FORWARD
	if(player_view.value && di_IsKeyActivated(gGameIdKeys[0],*Mouse))
	{
		if(gIsMultiplayer && !gIsServer)
		{
			//gNetBitKeys = gNetBitKeys | (char)(0x01 << 7);
			CL_IsWalk = true;
		}
		else
		{
			ply_Movedir(PERSO, DIR_FORWARD);
			gPlayerWalk = true;
		}
	}

	// CLIENT BACKWARD
	/*
	if(gIsMultiplayer && gIsServer)
	{
		if(gNetBitKeys & (0x01 << 6))
		{
			if(gMultiSV_clentid != -1)
			{
				ply_Movedir(gMultiSV_clentid, DIR_BACKWARD);
			}
		}
	}
	else if(gIsMultiplayer && !gIsServer)
	{
		if(gNetBitKeys_back & (0x01 << 6))
		{
			if(gMultiSV_clentid != -1)
			{
				ply_Movedir(MORBAK, DIR_BACKWARD);
			}
		}
	}*/
	
	// SINGLEPLAYER/HOST BACKWARD
	if(player_view.value && di_IsKeyActivated(gGameIdKeys[1],*Mouse))
	{
		if(gIsMultiplayer && !gIsServer)
		{
			//gNetBitKeys = gNetBitKeys | (char)(0x01 << 6);
			CL_IsWalk = true;
		}
		else
		{
			ply_Movedir(PERSO, DIR_BACKWARD);
			gPlayerWalk = true;
		}
	}

	// CLIENT STRAFELEFT
	/*
	if(gIsMultiplayer && gIsServer)
	{
		if(gNetBitKeys & (0x01 << 5))
		{
			if(gMultiSV_clentid != -1)
			{
				ply_Movedir(gMultiSV_clentid, DIR_STRAFLEFT);
			}
		}
	}
	else if(gIsMultiplayer && !gIsServer)
	{
		if(gNetBitKeys_back & (0x01 << 5))
		{
			if(gMultiSV_clentid != -1)
			{
				ply_Movedir(MORBAK, DIR_STRAFLEFT);
			}
		}		
	}*/
	
	// SINGLEPLAYER/HOST STRAFELEFT
	if(player_view.value && di_IsKeyActivated(gGameIdKeys[2],*Mouse))
	{
		if(gIsMultiplayer && !gIsServer)
		{
			//gNetBitKeys = gNetBitKeys | (char)(0x01 << 5);
			CL_IsWalk = true;
		}
		else
		{
			ply_Movedir(PERSO, DIR_STRAFLEFT);
			gPlayerWalk = true;
		}
	}

	// CLIENT STRAFERIGHT
	/*
	if(gIsMultiplayer && gIsServer)
	{
		if(gNetBitKeys & (0x01 << 4))
		{
			if(gMultiSV_clentid != -1)
			{
				ply_Movedir(gMultiSV_clentid, DIR_STRAFRIGHT);
			}
		}
	}
	else if(gIsMultiplayer && !gIsServer)
	{
		if(gNetBitKeys_back & (0x01 << 4))
		{
			if(gMultiSV_clentid != -1)
			{
				ply_Movedir(MORBAK, DIR_STRAFRIGHT);
			}
		}
	}
	 */
	
	// SINGLEPLAYER/HOST STRAFERIGHT
	if(player_view.value && di_IsKeyActivated(gGameIdKeys[3],*Mouse))
	{
		if(gIsMultiplayer && !gIsServer)
		{
			//gNetBitKeys = gNetBitKeys | (char)(0x01 << 4);
			CL_IsWalk = true;
		}
		else
		{
			ply_Movedir(PERSO, DIR_STRAFRIGHT);
			gPlayerWalk = true;
		}
	}


	// le vecteur acceleration du joueur client sur le serveur s'est arreter il faut avertir le client
	// pour une meilleur interpolation du client
	if(gIsMultiplayer && gIsServer)
	{
		if(phy_IsVelocity(MORBAK) == false)
		{
			if(CL_accelerate)
			{
				net_SendVectorState(o_FindEntity(MORBAK), 0);
			}
		}
		else
		{
			if(!CL_accelerate)
			{
				net_SendVectorState(o_FindEntity(MORBAK), 1);
			}
		}
	}

	if(gIsMultiplayer)
	{
		if(!gIsServer)
		{
			pEntity	ClientEnt;

			//gNetBitKeys_back = 0;
			ClientEnt = o_FindEntity(MORBAK);
			if(ClientEnt)
			if(ClientEnt->net_SendNewAnim == false)
			if(CL_IsWalk)
			{
				if(ClientEnt->Animation_ID != MSK_RUN)
				{
					ClientEnt->Animation_ID = MSK_RUN;
					ClientEnt->net_SendNewAnim = true;
				}
			}
			else
			{
				if(ClientEnt->Animation_ID != MSK_THINK)
				{
					ClientEnt->Animation_ID = MSK_THINK;
					ClientEnt->net_SendNewAnim = true;
				}
			}
		}
		else
		{
			// Gestion des animations des models du SERVER au CLIENT
			PlayerEnt = o_FindEntity(PERSO);
			if(PlayerEnt)
			if(PlayerEnt->net_SendNewAnim == false)
			if(gPlayerWalk)
			{		
				if(PlayerEnt->Animation_ID != MSK_RUN)
				{
					PlayerEnt->Animation_ID = MSK_RUN;
					PlayerEnt->net_SendNewAnim = true;
				}
			}
			else
			{
				if(PlayerEnt->Animation_ID != MSK_THINK)
				{
					PlayerEnt->Animation_ID = MSK_THINK;
					PlayerEnt->net_SendNewAnim = true;
				}
			}
		}
	}



	if(IsConsoleActive)
		goto no_bobroll;

	// PLAYER BOB/ROLL
	if(!gPlayerWalk)
	{
		if(gPlayerWalkTime)
		{
			gPlayerWalkTime -= gFrameTime_msec;
			if(gPlayerWalkTime < 0)
				gPlayerWalkTime = 0;
		}
	}
	else
	{
		gPlayerWalkTime += gFrameTime_msec;
	}

no_bobroll:


	// PLAYER RUN (pas en mode reseau)
	if(!gIsMultiplayer)
	{
		if(player_view.value && di_IsKeyActivated(gGameIdKeys[12],*Mouse))
		{
			gPlayerRun = true;
		}
		else
		{
			gPlayerRun = false;
		}
	}

	// PLAYER CROUCH (pas en mode reseau)
	if(!gIsMultiplayer)
	{
		if(player_view.value && di_IsKeyActivated(gGameIdKeys[5],*Mouse))
		{
			if(gPlayerCrouch == false)
			{
				gPlayerCrouch = true;
				PlayerEnt = o_FindEntity(PERSO);
				if(PlayerEnt)
					gPlayerCrouchStep = PlayerEnt->Radius.Y * 0.5f;
			}
			else if(gPlayerCrouchStep)
			{
				PlayerEnt = o_FindEntity(PERSO);
				if(PlayerEnt)
				if(PlayerEnt->Radius.Y > gPlayerCrouchStep)
				{
					PlayerEnt->Radius.Y -= gFrameTime_msec * 0.10f;
					PlayerEnt->ent_anim.Pos.Y -= gFrameTime_msec * 0.10f;
					if(PlayerEnt->Radius.Y < gPlayerCrouchStep)
					{
						PlayerEnt->ent_anim.Pos.Y += gFrameTime_msec * 0.10f;
						PlayerEnt->Radius.Y = gPlayerCrouchStep;
						gPlayerCrouchStep = 0;
					}
				}								
			}
		}
		else
		{
			if(gPlayerCrouch == true)
			{
				PlayerEnt = o_FindEntity(PERSO);
				if(PlayerEnt)
				{
					gPlayerCrouchStep = PlayerEnt->saveradiusY;
					gPlayerCrouch = false;
				}
			}
			else
			{
				if(gPlayerCrouchStep)
				{
					PlayerEnt = o_FindEntity(PERSO);
					if(PlayerEnt)
					if(PlayerEnt->Radius.Y < gPlayerCrouchStep)
					if(IsGround2_UpCreature(PlayerEnt) == false)
					{
						PlayerEnt->Radius.Y += gFrameTime_msec * 0.10f;
						PlayerEnt->ent_anim.Pos.Y += gFrameTime_msec * 0.10f;
						if(PlayerEnt->Radius.Y > gPlayerCrouchStep)
						{
							PlayerEnt->Radius.Y = gPlayerCrouchStep;
							gPlayerCrouchStep = 0;
						}
					}
				}
			}
		}
	}
			
	// PLAYER CHANGE WEAPON
//	if(gIsMultiplayer && !gIsServer)
//	{
//	}
//	else
//	{
		if(player_view.value && di_IsKeyActivated(gGameIdKeys[7],*Mouse))
		{
			PlayerSwapWeapon(1);
		}
//	}

//	if(gIsMultiplayer && !gIsServer)
//	{
//	}
//	else
//	{
		if(player_view.value && di_IsKeyActivated(gGameIdKeys[8],*Mouse))
		{
			PlayerSwapWeapon(2);
		}
//	}

//	if(gIsMultiplayer && !gIsServer)
//	{
//	}
//	else
//	{
		if(player_view.value && di_IsKeyActivated(gGameIdKeys[9],*Mouse))
		{
			PlayerSwapWeapon(3);
		}
//	}

//	if(gIsMultiplayer && !gIsServer)
//	{
//	}
//	else
//	{
		if(player_view.value && di_IsKeyActivated(gGameIdKeys[10],*Mouse))
		{
			PlayerSwapWeapon(4);
		}
//	}
}
