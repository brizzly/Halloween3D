
#include "alloween.cpp"

int		KeyBuffer[KEY_BUFFER_MAX]={-1,-1,-1,-1,-1,-1,-1,-1};
hBool	*KeyMask=NULL;

float	Step=0;
hBool	MouseKeyA=false;
hBool	MouseKeyB=false;
hBool	MouseKeyA_clicked=false;
hBool	MouseKeyB_clicked=false;
int		MouseX, MouseY;
hBool	gScanKeyActivated=false;
int		gIdGameKey;
int		gIdScannedKey;
hBool	gPlayerCrouch=false;
float	gPlayerCrouchStep=0;
hBool	gPlayerRun=false;

hMOUSEINPUT	MouseInput;

#ifdef H_WINDOWS
//#define	DIRECTINPUT_VERSION 0x800 // 0x500

#include <windows.h>
#include <dinput.h>

LPDIRECTINPUT			lpdi;
LPDIRECTINPUTDEVICE		lpdiMouse;
LPDIRECTINPUTDEVICE		lpdiKeyboard;
DIMOUSESTATE			dims;      // DInput mouse state structure
//DIJOYSTATE			dijs;      // DInput joystick state structure. Ca plante avec NT !?
#endif

byte					diks[256]; // DInput keyboard state buffer
dword					Touche;
int						gGameIdKeys[MAX_GAMEKEY+4];


//==============================================================================================//
//									DIRECT INPUT ROUTINES										//
//==============================================================================================//

hBool IN_Init(HINSTANCE hInstance)
{
	//if( DirectInputCreate(hInstance,DIRECTINPUT_VERSION,&lpdi,NULL) < 0)
	//	return false;

	if (DirectInput8Create(GetModuleHandle(nullptr), DIRECTINPUT_VERSION,
		IID_IDirectInput8, (VOID**)&lpdi, nullptr) < 0)
		return false;


	lpdi->CreateDevice(GUID_SysKeyboard, &lpdiKeyboard, NULL);
	lpdi->CreateDevice(GUID_SysMouse, &lpdiMouse, NULL);

	lpdiKeyboard->SetCooperativeLevel(ghWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
	lpdiMouse->SetCooperativeLevel(ghWnd, DISCL_EXCLUSIVE | DISCL_FOREGROUND);

	lpdiKeyboard->SetDataFormat(&c_dfDIKeyboard);
	lpdiMouse->SetDataFormat(&c_dfDIMouse);

	lpdiKeyboard->Acquire();
	lpdiMouse->Acquire();

	return true;
}

void DI_ReadKeyboard()
{
	HRESULT	hr;

	hr = lpdiKeyboard->GetDeviceState(sizeof(diks), &diks );
	if( hr == DIERR_INPUTLOST )
	{
	     hr = lpdiKeyboard->Acquire();
	     if(SUCCEEDED(hr))
	         DI_ReadKeyboard();
	}
}

void DI_ReadMouse()
{
	HRESULT	hr;

	hr = lpdiMouse->GetDeviceState(sizeof(DIMOUSESTATE), &dims );
	if( hr == DIERR_INPUTLOST )
    {
         hr = lpdiMouse->Acquire();
         if(SUCCEEDED(hr))
             DI_ReadMouse();
    }
}

int di_GetIdKey()
{
	int		index;
	hBool	clicked;
	int		n_mouse;

	gDrawCursor = false;
	DrawGameConfig_INPUT();
	menu_SelectProcess_CONFIG_INPUT();
	gl_SwapBuffer();
	gDrawCursor = true;

	clicked = false;
	while(1)
	{
		ds_PlayBuffer();

		DI_ReadMouse();
		if(!clicked)
		{
			if( dims.rgbButtons[0] & 0x80 )
			{
				n_mouse = 256;
				clicked = true;
			}
			else if( dims.rgbButtons[1] & 0x80 )
			{
				n_mouse = 257;
				clicked = true;
			}
			else if( dims.rgbButtons[2] & 0x80 )
			{
				n_mouse = 258;
				clicked = true;
			}
		}
		else
		{
			if(!(dims.rgbButtons[0] & 0x80) &&  !(dims.rgbButtons[1] & 0x80) &&  !(dims.rgbButtons[2] & 0x80))
				return n_mouse;
		}

		DI_ReadKeyboard();
		for(index=0 ; index<255 ; index++)
		if(diks[index] & 0xFF)
			return index;
	}
	return -1;
}

void IN_WarpMouse()
{

}

int ReadKey()
{
	int		index;
	int		Key=0;
	int		KeyPos;
	hBool	KeyHasBeenPressed;
	int		index2;

	DI_ReadKeyboard();

	// SYSTEM KEY CODE
	if( diks[DIK_ESCAPE] & 0x80 )
		return 1;
	if( diks[DIK_RETURN] & 0x80 )
		return 2;
	if( diks[DIK_NUMPADENTER] & 0x80 )
		return 2;
	if( diks[DIK_BACK] & 0x80 )
		return 3;
	if( diks[DIK_UP] & 0x80 )
		return 5;
	if( diks[DIK_TAB] & 0x80 )
		return 6;

	KeyHasBeenPressed = false;

	for(index=0 ; index<128 ; index++)
	if(diks[index] & 0x80)
	{
		if(index == DIK_GRAVE)
			continue;
		if(index == DIK_TAB)
			continue;
		if(index == K_LSHIFT)
			continue;
		if(index == K_RSHIFT)
			continue;

		if(KeyMask[index])
			continue;
		KeyMask[index]=true;

		ds_PlaySound(15);
		KeyHasBeenPressed = true;
	
		if((diks[K_LSHIFT] & 0x80) || (diks[K_RSHIFT] & 0x80))
			Key = shiftscantokey[index];
		else
			Key = scantokey[index];

		if(!Key)
			break;

		KeyPos=0;
		while( KeyBuffer[KeyPos] != -1 && KeyPos<KEY_BUFFER_MAX )
			KeyPos++;
		if(KeyPos>=KEY_BUFFER_MAX)
			break;

		for(index2=0 ; index2<KEY_BUFFER_MAX ; index2++)
		if(KeyBuffer[index2] == Key)
			break;

		KeyBuffer[KeyPos] = Key;
	}
	else
		KeyMask[index]=0;

	if(KeyHasBeenPressed)
		return 4;

	return false;
}

void DI_ReacquireMouse()
{
    if(lpdiMouse)
    {
      IDirectInputDevice_Acquire(lpdiMouse);
	}
}

void DI_ReacquireKeyboard()
{
    if(lpdiKeyboard)
    {
      IDirectInputDevice_Acquire(lpdiKeyboard);
	}
}

hBool ReacquireInput()
{
	DI_ReacquireMouse();
	DI_ReacquireKeyboard();
	return true;
}

void DI_UnacquireMouse()
{
    if(lpdiMouse)
    {
      IDirectInputDevice_Unacquire(lpdiMouse);
	}
}

void DI_UnacquireKeyboard()
{
    if(lpdiKeyboard)
    {
      IDirectInputDevice_Unacquire(lpdiKeyboard);
	}
}

hBool UnacquireInput()
{
	DI_UnacquireMouse();
	DI_UnacquireKeyboard();
	return true;
}

void IN_UnInit()
{
	UnacquireInput();

    if( lpdiMouse )
    {
        lpdiMouse->Release();
        lpdiMouse = NULL;
    }
    if( lpdiKeyboard )
    {
        lpdiKeyboard->Release();
        lpdiKeyboard = NULL;
    }
    if( lpdi )
	{
        lpdi->Release();
		lpdi = NULL;
	}
}

hBool di_IsKeyActivated(int id, hMOUSEINPUT Mouse)
{
	if(id<0 || id>258)
		return false;

	if(id < 256)
	{
		if(diks[id] & 0xFF)
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

			case 257:
			if(Mouse.button_B)
				return true;
			break;

			case 258:
		//	if(Mouse.button_C)
		//		return true;
			break;
		}
	}
	return false;
}

void m_UpdateCursor(hMOUSEINPUT Mouse, float *pCursor_x, float *pCursor_y)
{
	*pCursor_x += Mouse.offset_X * 5.0f;
	*pCursor_y -= Mouse.offset_Y * 5.0f;

	if(*pCursor_x<0)
		*pCursor_x=0;
	if(*pCursor_x>640.0f)
		*pCursor_x=640.0f;

	if(*pCursor_y<0)
		*pCursor_y=0;
	if(*pCursor_y>480.0f)
		*pCursor_y=480.0f;
}

void m_ProcessInputs(hMOUSEINPUT Mouse)
{
	pEntity		PlayerEnt;

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
			//ds_PlaySound(4);
			game_Init();
		}
		break;

		case PS_STARTMAP:
		m_UpdateCursor(Mouse,&cursor_x,&cursor_y);
		if(MouseKeyA)
			menu_ClickedA_StartMAP();
		if(MouseKeyB)
		{
			ds_PlaySound(4);
			menu_InitSelectMAP();
			ProgramState=PS_SELECTMAP;
			//game_Init();
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
		if(MouseKeyA)
		{
//			avi_StopIntro();
			game_Init();
		}
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
		//else
		if(gScanKeyActivated && MouseKeyA_clicked == false)
		{
			gScanKeyActivated = false;
			gIdScannedKey = di_GetIdKey();
			gGameIdKeys[gIdGameKey] = gIdScannedKey;
			m_ConsPrint("touche: %s id: %d\n", KeyToStringName[gIdScannedKey],gIdScannedKey);
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
						//LaunchPlayer(SPIKE,2.0f);
						LaunchPlayer(SPINE,2.0f);
						break;

						default:
						PlayerSwapWeapon(1);
						break;
					}					
				}

				if(di_IsKeyActivated(gGameIdKeys[4],Mouse))
				{
					PlayerEnt = o_FindEntity(PERSO);
					if(PlayerEnt)
					{
						JumpCreature(PERSO);
						PlayerEnt->JumpPossible = false; 
					}
				}
				else
				{
					PlayerEnt = o_FindEntity(PERSO);
					if(PlayerEnt)
						PlayerEnt->JumpPossible = true;
				}
			}

			r_TourneY(&gCamera,Mouse.offset_X,PlayerEnt);
			r_TourneX(&gCamera,Mouse.offset_Y,PlayerEnt);
		}
		break;
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
/*
	// CLIENT FORWARD
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
	}
	*/

	// SINGLEPLAYER/HOST FORWARD
	if(player_view.value && di_IsKeyActivated(gGameIdKeys[0],*Mouse))
	{
		if(gIsMultiplayer && !gIsServer)
		{
		//	gNetBitKeys = gNetBitKeys | (char)(0x01 << 7);
			CL_IsWalk = true;
		}
		else
		{
			ply_Movedir(PERSO, DIR_FORWARD);
			gPlayerWalk = true;
		}
	}
/*
	// CLIENT BACKWARD
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
	}
	*/

	// SINGLEPLAYER/HOST BACKWARD
	if(player_view.value && di_IsKeyActivated(gGameIdKeys[1],*Mouse))
	{
		if(gIsMultiplayer && !gIsServer)
		{
	//		gNetBitKeys = gNetBitKeys | (char)(0x01 << 6);
			CL_IsWalk = true;
		}
		else
		{
			ply_Movedir(PERSO, DIR_BACKWARD);
			gPlayerWalk = true;
		}
	}
/*
	// CLIENT STRAFELEFT
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
	}
	*/

	// SINGLEPLAYER/HOST STRAFELEFT
	if(player_view.value && di_IsKeyActivated(gGameIdKeys[2],*Mouse))
	{
		if(gIsMultiplayer && !gIsServer)
		{
	//		gNetBitKeys = gNetBitKeys | (char)(0x01 << 5);
			CL_IsWalk = true;
		}
		else
		{
			ply_Movedir(PERSO, DIR_STRAFLEFT);
			gPlayerWalk = true;
		}
	}

	/*
	// CLIENT STRAFERIGHT
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
	//		gNetBitKeys = gNetBitKeys | (char)(0x01 << 4);
			CL_IsWalk = true;
		}
		else
		{
			ply_Movedir(PERSO, DIR_STRAFRIGHT);
			gPlayerWalk = true;
		}
	}

/*
	// le vecteur acceleration du joueur client sur le serveur s'est arreter il faut avertir le client
	// pour une meilleur interpolation du client
	if(gIsMultiplayer && gIsServer)
	{
		if(phy_IsVelocity(MORBAK) == false)
		{
			if(CL_accelerate)
			{
				//net_SendVectorState(o_FindEntity(MORBAK), 0);
			}
		}
		else
		{
			if(!CL_accelerate)
			{
				//net_SendVectorState(o_FindEntity(MORBAK), 1);
			}
		}
	}

	if(gIsMultiplayer)
	{
		if(!gIsServer)
		{
			pEntity	ClientEnt;

			gNetBitKeys_back = 0;
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
*/

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
	if(player_view.value && di_IsKeyActivated(gGameIdKeys[7],*Mouse))
	{
		PlayerSwapWeapon(1);
	}

	if(player_view.value && di_IsKeyActivated(gGameIdKeys[8],*Mouse))
	{
		PlayerSwapWeapon(2);
	}

	if(player_view.value && di_IsKeyActivated(gGameIdKeys[9],*Mouse))
	{
		PlayerSwapWeapon(3);
	}

	if(player_view.value && di_IsKeyActivated(gGameIdKeys[10],*Mouse))
	{
		PlayerSwapWeapon(4);
	}
}


hBool IN_IsPressed(unsigned short keyCode)
{
	if(gIsServer && net_dedicated.value)
		return false;
//	if(!ActiveWindow)
//		return false;

	DI_ReadKeyboard();
	if(diks[keyCode] & 0x80)
		return true;
	return false;
}

hBool IN_IsMousePressed(unsigned short mbutton)
{
	if(gIsServer && net_dedicated.value)
		return false;

	DI_ReadMouse();

	switch(mbutton)
	{
		case 0:
		if(dims.rgbButtons[0] & 0x80)
			return true;
		break;

		case 1:
		if(dims.rgbButtons[2] & 0x80)
			return true;
		break;

		default:
		if(dims.rgbButtons[1] & 0x80)
			return true;
		break;
	}
	return false;
}

void IN_GetMouse(hMOUSEINPUT *Mouse)
{
	if(gIsServer && net_dedicated.value)
		return;

//	if(!ActiveWindow)
//		return;

	//ghWnd = FindWindow(NULL ,NAME);
	DI_ReadMouse();

	MouseX = (int)(2.0 * dims.lX);
	MouseY = (int)(2.0 * dims.lY);


	Mouse->offset_X = (float)MouseX * player_rvel.value;
	Mouse->offset_Y = (float)MouseY * player_rvel.value;

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
			//net_StopMultiplayer();
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
			{
				//MouseKeyB = true;
				game_Init();
				return;
			}
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
			gAlphaFont = 1;
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
					Story_NextText();
				}	
				
				if(game_IsMapOpened(1) == false) // map training pas debloqué
					goto no_cri;

				if(game_InjuryMode == 0)
				{
					game_InjuryMode = 2000.0f;
					ds_PlaySound(76+randInt(0,9));
				}
no_cri:;
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

