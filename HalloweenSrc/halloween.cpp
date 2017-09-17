
#include "halloween.h"
#include "alloween.cpp"


#define MAX_NAME	256
#define MAX_KEY		11


#include "misc/resource.h"


HWND		ghWnd = NULL;
HWND		hWndDialog = NULL;
HINSTANCE	ghInstance;



char	ErrorMessage[255];
hBool	done;
//hBool	gIsDemo;
int		window;
char	NameText[260];
char	KeyText[260];
int		shareware;
hBool	gIsMultiplayer;
hBool	gIsServer;
int		gMultiMapID = 1;
hBool	gMainProcessDone;
int		shareware2;



BOOL CALLBACK DialogProc4(HWND hwndDlg, UINT uMsg, WPARAM wParam,LPARAM lParam)
{
	RECT	r;
	int		dialogWidth;
	int		dialogHeight;
	char	bufferline[256];

	switch(uMsg)
	{
		case WM_INITDIALOG:
		{
			// on place la boite de dialogue au centre de l'ecran
			GetWindowRect(hwndDlg,&r);
			dialogWidth 	= r.right - r.left ;
			dialogHeight 	= r.bottom - r.top ;
			MoveWindow( hwndDlg , 
									(GetSystemMetrics( SM_CXSCREEN ) - dialogWidth )/2  ,
									(GetSystemMetrics( SM_CYSCREEN ) - dialogHeight )/2  ,
									dialogWidth,
									dialogHeight,
									false );
			//SendMessage(hwndDlg, EM_LIMITTEXT, 128, 0);  
			return TRUE;
		}

		/*
		case WM_CTLCOLORSTATIC:
		case WM_CTLCOLORBTN:
		case WM_CTLCOLOREDIT:
		{
			return (long)(CreateSolidBrush(RGB(255,100,0)));
		}
		break;
		*/

		case WM_CTLCOLORSCROLLBAR:
		case WM_CTLCOLORDLG:
		{
			return (long)(CreateSolidBrush(RGB(0,0,0)));
		}
		break;

		/*
		case WM_CTLCOLORSTATIC: //WM_CTLCOLOREDIT:
		{
			int	r = 255; //153;
			int g = 100; //51;
			int b = 0; //51;
			HDC  theDC = (HDC)(wParam);

			if( GetDlgCtrlID( GetDlgItem( hwndDlg , IDC_EDITCONS) ) == IDC_EDITCONS ) 
			{	
			//	SetTextColor(theDC, RGB(255,255,255));
				SetTextColor(theDC, RGB(0,0,0));
				SetBkColor(theDC, RGB(r,g,b));
				return (long)(CreateSolidBrush(RGB(r,g,b)));
			} 
			break;
		}
		break;
		*/		

		case WM_CLOSE:
		EndDialog(hwndDlg, 0);
		gMainProcessDone = true;
		return FALSE;

		case WM_COMMAND:
		{
			switch(LOWORD(wParam))
			{
				case ID_OK:
				GetWindowText(GetDlgItem(hWndDialog, IDC_EDITCMD), bufferline, 256);
				if(strlen(bufferline) == 0)
					return FALSE;
				SetWindowText(GetDlgItem(hwndDlg, IDC_EDITCMD), "");		
				m_ConsPrint("%s\n",bufferline);
				m_ConsoleFindCommand(bufferline);
				break;

				case ID_CLEAR:
				{
					char	info_welcome[255];
					sprintf(info_welcome, "WELCOME TO HALLOWEEN (%s)\n",GAMEVERSION);
					SetWindowText(GetDlgItem(hwndDlg, IDC_EDITCONS), info_welcome);
					return FALSE;
				}
			}
		}  
		default:
		break;
	}
	return FALSE;
}

void sys_AntiPirate()
{
	shareware = 1-shareware2;
}

BOOL CALLBACK DialogProc3(HWND hwndDlg, UINT uMsg, WPARAM wParam,LPARAM lParam)
{
	RECT	r;
	int		dialogWidth;
	int		dialogHeight;
	char	tmpText[MAX_NAME];
	int		MapID;

	switch(uMsg)
	{
		case WM_INITDIALOG:
		{
			gIsMultiplayer = TRUE;
			// on place la boite de dialogue au centre de l'ecran
			GetWindowRect(hwndDlg,&r);
			dialogWidth 	= r.right - r.left ;
			dialogHeight 	= r.bottom - r.top ;
			MoveWindow( hwndDlg , 
									(GetSystemMetrics( SM_CXSCREEN ) - dialogWidth )/2  ,
									(GetSystemMetrics( SM_CYSCREEN ) -dialogHeight )/2  ,
									dialogWidth,
									dialogHeight,
									false ) ;

			gIsServer = true;
			CheckDlgButton( hwndDlg , IDC_CREATESERVER , TRUE );
			CheckDlgButton( hwndDlg , IDC_JOINSERVER , FALSE );
			EnableWindow( GetDlgItem( hwndDlg , IDC_ENTERIP) , FALSE );
			EnableWindow( GetDlgItem( hwndDlg , IDC_SERVERNAME) , TRUE );
			EnableWindow( GetDlgItem( hwndDlg , IDC_CONNECT) , FALSE );
			EnableWindow( GetDlgItem( hwndDlg , IDC_CREATE) , TRUE );
			EnableWindow( GetDlgItem( hwndDlg , IDC_DEDICATED) , TRUE );
			EnableWindow( GetDlgItem( hwndDlg , IDC_COMBO2) , TRUE );
			
			// dialog presets
			SetWindowText(GetDlgItem(hwndDlg, IDC_PLAYERNAME), net_clname.string);		
			SetWindowText(GetDlgItem(hwndDlg, IDC_SERVERNAME), net_svname.string);
			SetWindowText(GetDlgItem(hwndDlg, IDC_MAPID), net_mapid.string);
			SetWindowText(GetDlgItem(hwndDlg, IDC_ENTERIP), net_ip.string);
						
			// on rempli les combos : connection bandwidth
			SendMessage( GetDlgItem( hwndDlg , IDC_COMBO3) , CB_ADDSTRING , 0 , (LPARAM)"Modem" );
			SendMessage( GetDlgItem( hwndDlg , IDC_COMBO3) , CB_ADDSTRING , 0 , (LPARAM)"Cable/xDSL" );
			SendMessage( GetDlgItem( hwndDlg , IDC_COMBO3) , CB_ADDSTRING , 0 , (LPARAM)"LAN" );
			SendMessage( GetDlgItem( hwndDlg , IDC_COMBO3) , CB_SETCURSEL , 2 , (LPARAM)0 );
			
			// on rempli les combos : player model
			SendMessage( GetDlgItem( hwndDlg , IDC_COMBO1) , CB_ADDSTRING , 0 , (LPARAM)"Depla");
			SendMessage( GetDlgItem( hwndDlg , IDC_COMBO1) , CB_ADDSTRING , 0 , (LPARAM)"Morbak");
			SendMessage( GetDlgItem( hwndDlg , IDC_COMBO1) , CB_ADDSTRING , 0 , (LPARAM)"Zemummy");
			SendMessage( GetDlgItem( hwndDlg , IDC_COMBO1) , CB_SETCURSEL , 0 , (LPARAM)0 );
			
			// on rempli les combos : gametype
			SendMessage( GetDlgItem( hwndDlg , IDC_COMBO2) , CB_ADDSTRING , 0 , (LPARAM)"Tournament");
			SendMessage( GetDlgItem( hwndDlg , IDC_COMBO2) , CB_ADDSTRING , 0 , (LPARAM)"Free For All");
			SendMessage( GetDlgItem( hwndDlg , IDC_COMBO2) , CB_ADDSTRING , 0 , (LPARAM)"Capture the Pumpkin");
			SendMessage( GetDlgItem( hwndDlg , IDC_COMBO2) , CB_SETCURSEL , 0 , (LPARAM)0 );
			
			// serveur dedie
			if(net_dedicated.value)
				CheckDlgButton( hwndDlg , IDC_DEDICATED , TRUE);
			else
				CheckDlgButton( hwndDlg , IDC_DEDICATED , FALSE);

			// on grise les items non encore codé
			EnableWindow( GetDlgItem( hwndDlg , IDC_COMBO1) , FALSE );
			EnableWindow( GetDlgItem( hwndDlg , IDC_COMBO2) , FALSE );
			EnableWindow( GetDlgItem( hwndDlg , IDC_COMBO3) , FALSE );
			
			// si on est dans la demo on force le multiplayer sur la map 4 de la mine
			if(shareware || gIsDemo)
			{
				//net_mapid.value = 4;
				SetWindowText(GetDlgItem(hwndDlg, IDC_MAPID), "demomap");
				EnableWindow(GetDlgItem(hwndDlg , IDC_MAPID) , FALSE );
			}

			//numeroItem = SendMessage( hwndCombo, CB_GETCURSEL ,0,0 ) 

			return TRUE;
		}

		case WM_CLOSE:
		EndDialog(hwndDlg, 0);
		return FALSE;

		case WM_COMMAND:
		{
			switch(LOWORD(wParam))
			{
				case IDC_CREATE:
				{
					// l'id de la map a charger
					if(!shareware && !gIsDemo)
					{
						GetWindowText(GetDlgItem(hwndDlg, IDC_MAPID), tmpText, 3);
						sscanf(tmpText,"%d",&MapID);
						if(MapID < 1 || MapID > 9)
						{
							MessageBox(hwndDlg, "Please enter any ID map from 1 to 9", "ERROR", MB_OK);
							break;
						}
					}
					else
					{
						MapID = 4;
					}
					net_mapid.value = (float)MapID;
					strncpy(net_mapid.string, tmpText, 3);
					gMultiMapID = MapID;

					// le nom du serveur a creer
					GetWindowText(GetDlgItem(hwndDlg, IDC_SERVERNAME), tmpText, 255);
					strncpy(net_svname.string, tmpText, 255);
					
					// recupere le nom du joueur client
					GetWindowText(GetDlgItem(hwndDlg, IDC_PLAYERNAME), tmpText, 255);
					strncpy(net_clname.string, tmpText, 255);

					EndDialog(hwndDlg, 1);
					return TRUE;
				}

				case IDC_CONNECT:
				{
					// recupere le nom du joueur client
					GetWindowText(GetDlgItem(hwndDlg, IDC_PLAYERNAME), tmpText, 255);
					strncpy(net_clname.string, tmpText, 255);

					// recupere l'IP du serveur auquel on va essayer de se connecter
					GetWindowText(GetDlgItem(hwndDlg, IDC_ENTERIP), tmpText, 16);
					strncpy(net_ip.string, tmpText, 16);				


					EndDialog(hwndDlg, 1);
					return TRUE;
				}

				case IDC_JOINSERVER:
				gIsServer = false;
				CheckDlgButton( hwndDlg , IDC_CREATESERVER , !TRUE );
				CheckDlgButton( hwndDlg , IDC_JOINSERVER , !FALSE );
				EnableWindow( GetDlgItem( hwndDlg , IDC_ENTERIP) , !FALSE );
				EnableWindow( GetDlgItem( hwndDlg , IDC_SERVERNAME) , !TRUE );
				EnableWindow( GetDlgItem( hwndDlg , IDC_CONNECT) , !FALSE );
				EnableWindow( GetDlgItem( hwndDlg , IDC_CREATE) , !TRUE );
				EnableWindow( GetDlgItem( hwndDlg , IDC_MAPID) , FALSE );
				EnableWindow( GetDlgItem( hwndDlg , IDC_DEDICATED) , !TRUE );
			//	EnableWindow( GetDlgItem( hwndDlg , IDC_COMBO2) , !TRUE );
				break;

				case IDC_CREATESERVER:
				gIsServer = true;
				CheckDlgButton( hwndDlg , IDC_CREATESERVER , TRUE );
				CheckDlgButton( hwndDlg , IDC_JOINSERVER , FALSE );
				EnableWindow( GetDlgItem( hwndDlg , IDC_ENTERIP) , FALSE );
				EnableWindow( GetDlgItem( hwndDlg , IDC_SERVERNAME) , TRUE );
				EnableWindow( GetDlgItem( hwndDlg , IDC_CONNECT) , FALSE );
				EnableWindow( GetDlgItem( hwndDlg , IDC_CREATE) , TRUE );
				if(!shareware && !gIsDemo)
				{
					EnableWindow( GetDlgItem( hwndDlg , IDC_MAPID) , TRUE );
				}
				EnableWindow( GetDlgItem( hwndDlg , IDC_DEDICATED) , TRUE );
			//	EnableWindow( GetDlgItem( hwndDlg , IDC_COMBO2) , TRUE );
				break;

				case IDC_MAPID:
				break;

				case IDC_DEDICATED:
				if(net_dedicated.value == 0)
					net_dedicated.value = 1;
				else 
					net_dedicated.value = 0;
				break;

				case ID_QUIT:
				{
					EndDialog(hwndDlg, 0);
					return FALSE;
				}
			}
		}  
		default: 
		break;
	}
	return FALSE;
}

BOOL CALLBACK DialogProc2( HWND hwndDlg, UINT uMsg, WPARAM wParam,LPARAM lParam)
{
	RECT	r;
	int		dialogWidth;
	int		dialogHeight;
	char	tmpText[MAX_NAME];

	switch(uMsg)
	{
		case WM_INITDIALOG:
		{
			sprintf(tmpText,"%s",KeyText);
			tmpText[5] = '*';
			tmpText[6] = '*';
			tmpText[7] = '*';
			tmpText[8] = '*';
			tmpText[9] = '*';
			tmpText[10] = '*';
			tmpText[11] = '*';
			SetDlgItemText(hwndDlg,IDC_STATIC1,NameText);
			SetDlgItemText(hwndDlg,IDC_STATIC2,""); //tmpText);

	//	EnableWindow( GetDlgItem( hwndDlg , ID_PLAYMULTI) , FALSE );

			// on place la boite de dialogue au centre de l'ecran
			GetWindowRect(hwndDlg,&r);
			dialogWidth 	= r.right - r.left ;
			dialogHeight 	= r.bottom - r.top ;
			MoveWindow( hwndDlg , 
									(GetSystemMetrics( SM_CXSCREEN ) - dialogWidth )/2  ,
									(GetSystemMetrics( SM_CYSCREEN ) -dialogHeight )/2  ,
									dialogWidth,
									dialogHeight,
									false ) ;
			return TRUE;
		}

		case WM_CLOSE:
		EndDialog(hwndDlg, 0);
		return FALSE;

		case WM_COMMAND:
		{
			switch(LOWORD(wParam))
			{
				case ID_PLAY:
				{
					EndDialog(hwndDlg, 1);
					return TRUE;
				}

				case ID_PLAYMULTI:
				{
					EnableWindow(hwndDlg, FALSE);

					if(!DialogBox(ghInstance,MAKEINTRESOURCE(IDD_DIALOG3),NULL,DialogProc3))
					{
						gIsMultiplayer = FALSE;
						BringWindowToTop(hwndDlg);
						EnableWindow(hwndDlg, TRUE);
						break;
					//	EndDialog(hwndDlg, 1);
					//	return DialogBox(ghInstance,MAKEINTRESOURCE(IDD_DIALOG2),NULL,DialogProc2);
					}
					else
					{
						EndDialog(hwndDlg, 1);
						return TRUE;
					}
				}

				case ID_HELPHALLOW:
				ShellExecute( NULL , "open" , "DOCS\\doc.html" ,0,0,SW_SHOW ) ;
				break;

				case ID_QUIT:
				{
					EndDialog(hwndDlg, 0);
					return FALSE;
				}
			}
		}  
		default: 
		break;
	}
	return FALSE;
}

// KEY stuff ===
void sys_GetKeyDatas()
{
	FILE	*f;
	int		i;
	char	c;

	memset(NameText,0,MAX_NAME);
	memset(KeyText,0,MAX_NAME);
	shareware = true;

	f = fopen("system/hkey.ini","r");
	if(!f)
		return;

	fscanf(f,"NAME: ");
	i=0;
	do
	{
		c = fgetc(f);
		if(c==0)
			return;
		if(c=='\n')
			break;
		NameText[i++] = c;
	}
	while(c!='\n');

	fscanf(f,"KEY: %s",KeyText);
	fclose(f);
}
// ===

BOOL CALLBACK DialogProc( HWND hwndDlg, UINT uMsg, WPARAM wParam,LPARAM lParam)
{
	RECT	r;
	int		dialogWidth;
	int		dialogHeight;
	FILE	*f;

	switch(uMsg)
	{	
		case WM_INITDIALOG:
		{
			GetWindowRect(hwndDlg,&r);
			dialogWidth 	= r.right - r.left ;
			dialogHeight 	= r.bottom - r.top ;

	//	EnableWindow( GetDlgItem( hwndDlg , ID_PLAYMULTI) , FALSE );

			MoveWindow( hwndDlg , 
									(GetSystemMetrics( SM_CXSCREEN ) - dialogWidth )/2  ,
									(GetSystemMetrics( SM_CYSCREEN ) -dialogHeight )/2  ,
									dialogWidth,
									dialogHeight,
									false ) ;

			return TRUE;

			break;
		}

		case WM_CLOSE:
		EndDialog(hwndDlg, 0);
		return FALSE;

		case WM_COMMAND:
		{
			switch(LOWORD(wParam))
			{
				case IDC_REGISTER:
				GetDlgItemText(hwndDlg, IDC_EDIT1, NameText, 255);
				GetDlgItemText(hwndDlg, IDC_EDIT2, KeyText, 255);
			    if(key_IsValid(NameText,KeyText))
				{
					EnableWindow( GetDlgItem( hwndDlg , IDC_EDIT1) , FALSE );
					EnableWindow( GetDlgItem( hwndDlg , IDC_EDIT2) , FALSE );
					MessageBox(NULL, "Key OK\n\nYou have unlocked the FULL version", "Thank You!", MB_OK);
					f = fopen("system/hkey.ini","w");
					if(f)
					{
						fprintf(f,"NAME: %s\n",NameText);
						fprintf(f,"KEY: %s\n",KeyText);
						fclose(f);
					}
					shareware = false;
					//game_DeleteAllSlots();
				}
				else
				{
					ShellExecute( NULL , "open" , "https://www.plimus.com/jsp/buynow.jsp?contractId=1641207" ,0,0,SW_SHOW ) ;
				}
				break;

				case ID_PLAY:
				{
					EndDialog(hwndDlg, 1);
					return TRUE;
				}

				case ID_PLAYMULTI:
				{
					EnableWindow(hwndDlg, FALSE);

					if(!DialogBox(ghInstance,MAKEINTRESOURCE(IDD_DIALOG3),NULL,DialogProc3))
					{
						gIsMultiplayer = FALSE;
						BringWindowToTop(hwndDlg);
						EnableWindow(hwndDlg, TRUE);
						break;
						//return DialogBox(ghInstance,MAKEINTRESOURCE(IDD_DIALOG1),NULL,DialogProc);
					}
					else
					{
						EndDialog(hwndDlg, 1);
						return TRUE;
					}
				}

				case ID_HELPHALLOW:
				ShellExecute( NULL , "open" , "DOCS\\doc.html" ,0,0,SW_SHOW ) ;
				break;

				case ID_QUIT:
				{
					EndDialog(hwndDlg, 0);
					return FALSE;
				}
			}
		}
  		default: 
		break;
	} 
	return FALSE; 
}


// HKEY stuff

void key_CheckGame()
{
	shareware2 = '56'; // 1		
	/*
	if(key_IsValid(NameText,KeyText))
	{
		shareware2 = '56'; // 1
	}
	else
	{
		shareware2 = '55'; // 0
	}*/
	shareware2 -= '55';
}

int get_sum(int number)
{
	int sum = 0;

	while(number > 9)
	{
		sum = 0;
		while(number)
		{
			sum += (number % 10);
			number /= 10;
		}
		number = sum;
	}
	return number;
}

BOOL key_IsValid(char *name, char *key)
{
	FILE		*f;
	int		i1,i2;

	f = fopen("system/textures/systex/h1m7/lm_big_002.tga", "rb");
	if(!f)
	{
		gIsDemo = true;
		strcpy(NameText, "DEMO VERSION");
		return true;
	}
	fclose(f);
	gIsDemo = false;

	if(!name || !key)
		return false;
	if(strlen(name) == 0 || strlen(key) == 0)
		return false;

	// lit le nom
	i2 = 0;
	for(i1=0 ; name[i1] ; i1++)
	{
		if(i1 >= MAX_NAME)
			break;
		i2 += name[i1];
	}
	i2 = get_sum(i2);
	if(i2 != key[0]-48)
		return false;

	// lit la clee
	i2 = 0;
	for(i1=0 ; i1<MAX_KEY ; i1++)
	{
		i2 += key[i1];
	}
	i2 = get_sum(i2);
#ifdef DEMO_GAME
	if(i2 == key[5]-48)
#else
	if(i2 == key[MAX_KEY]-48)
#endif
		return true;
	return false;
}

// HKEY stuff





HDC		hDC = NULL;
HGLRC	hRC = NULL;

static	PIXELFORMATDESCRIPTOR pfd=
{
	sizeof(PIXELFORMATDESCRIPTOR),				// Size Of This Pixel Format Descriptor
	1,											// Version Number
	PFD_DRAW_TO_WINDOW |						// Format Must Support Window
	PFD_SUPPORT_OPENGL |						// Format Must Support OpenGL
	PFD_DOUBLEBUFFER,							// Must Support Double Buffering
	PFD_TYPE_RGBA,								// Request An RGBA Format
	24/*32*/,											// Select Our Color Depth
	0, 0, 0, 0, 0, 0,							// Color Bits Ignored
	0,											// No Alpha Buffer
	0,											// Shift Bit Ignored
	0,											// No Accumulation Buffer
	0, 0, 0, 0,									// Accumulation Bits Ignored
	32,											// 16Bit Z-Buffer (Depth Buffer)  
	0,											// No Stencil Buffer
	0,											// No Auxiliary Buffer
	PFD_MAIN_PLANE,								// Main Drawing Layer
	0,											// Reserved
	0, 0, 0										// Layer Masks Ignored
};

hBool	ChangeResolution(dword w, dword h, dword bitdepth);
void	sys_KillGL();



int WINAPI windows_process(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LONG	lRet = 1;

	switch (uMsg)
	{
		case WM_ACTIVATE:
		if( LOWORD(wParam) == WA_INACTIVE )
		{
			//ActiveWindow = false;
			DI_UnacquireMouse();
			DI_UnacquireKeyboard();
		}
		else
		{
			DI_ReacquireMouse();
			DI_ReacquireKeyboard();
			//ActiveWindow = true;
			//ShowWindow(hWnd, SW_RESTORE);
		}
		break;

		case WM_CLOSE:
		PostQuitMessage(0);
		lRet=0;
		break;
	
		default:
		lRet=DefWindowProc(hWnd,uMsg,wParam,lParam);
		break;
	}
	return  lRet;
}

hBool sys_RegisterWindow(char* title)
{
	WNDCLASS		wc;	
	
	if(gIsMultiplayer && gIsServer && net_dedicated.value)
		return true;

	wc.style			= CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc		= (WNDPROC)windows_process;
	wc.cbClsExtra		= 0;
	wc.cbWndExtra		= 0;
	wc.hInstance		= ghInstance;
	wc.hIcon			= 0;
	wc.hCursor			= LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground	= CreateSolidBrush(RGB(0,0,0));
//	wc.hbrBackground	= CreateSolidBrush(RGB(255,100,0));
//	wc.hbrBackground	= (HBRUSH)GetStockObject(DKGRAY_BRUSH); // mettre de l'orange
	wc.lpszMenuName		= 0;
	wc.lpszClassName	= title;

	if(!RegisterClass(&wc))
	{
		m_ConsPrint("Failed To Register The Window Class.");
		return false;
	}
	else
		m_ConsPrint("RegisterClass\n");
	
	return true;
}

hBool sys_CreateWindow(char* title, int width, int height, int bits)
{
	int		w, h, sx, sy;
	RECT	r;
	DWORD	windowStyle;

	if(fullscreen.value)
	{
		if(!ChangeResolution(width,height,(int)colordepth.value))
		{
			m_ConsPrint("ChangeResolution FAILED\n");
			if (MessageBox(NULL,"The Requested Fullscreen Mode Is Not Supported By\nYour Video Card. Default video settings loaded",title,MB_OK|MB_ICONEXCLAMATION)==IDOK)
			{
				colordepth.value = new_colordepth = 32; // 16;
				videomode.value = new_videomode = 5; // 2;

				width = ScreenX[(int)videomode.value];
				height = ScreenY[(int)videomode.value];
				if(!ChangeResolution(width,height,(int)colordepth.value))
				{
					return false;								
				}
			}
		}
		ghWnd = CreateWindowEx(WS_EX_TOPMOST,title, NAME, WS_POPUP | WS_CLIPSIBLINGS,
                        0, 0, width, height, NULL, NULL, ghInstance, NULL);
	}
	else
	{
		r.left = 0;
		r.top = 0;
		r.right  = width;
		r.bottom = height;

		windowStyle = WS_BORDER|WS_CAPTION|WS_SYSMENU;
	//	windowStyle = WS_OVERLAPPED|WS_BORDER|WS_CAPTION|WS_VISIBLE|WS_SYSMENU;
		AdjustWindowRect (&r, windowStyle, FALSE);

		w = r.right - r.left;
		h = r.bottom - r.top;

		sx = (GetSystemMetrics(SM_CXSCREEN)-w)/2;
		sy = (GetSystemMetrics(SM_CYSCREEN)-h)/2;

		ghWnd = CreateWindowEx(	0,//WS_EX_TOPMOST,
								NAME,
								NAME,
								windowStyle, 
								sx,
								sy,
								w,
								h,
								NULL,
								NULL,
								ghInstance,
								NULL);

		if(developer.value)
		{
	//		srand(time(NULL));
	//		SetWindowPos(ghWnd, HWND_TOP, 560*(rand()%2), 350*(rand()%2), w, h, 0);
		}
	}

	if(!ghWnd)
	{
		m_ConsPrint("Window Creation Error.");
		sys_KillGL();
		return false;
	}
	else
		m_ConsPrint("Window creation succeed\n");

	return true;
}

void sys_KillWindow()
{
	if(ghWnd && !DestroyWindow(ghWnd))					
	{
		MessageBox(NULL,"Could Not Release ghWnd.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		ghWnd=NULL;									
	}
	//ShowCursor(true);
}

hBool sys_CreateGL(int width, int height, int bits)
{
	int	PixelFormat;
	PIXELFORMATDESCRIPTOR pfd_new;
	int generic_format;
	int generic_accelerated;

	if (!(hDC = GetDC(ghWnd)))						
	{
		m_ConsPrint("Can't Create A GL Device Context.");
		sys_KillGL();
		return false;
	}
	else
		m_ConsPrint("Create A GL Device Context\n");

	if (!(PixelFormat=ChoosePixelFormat(hDC,&pfd)))
	{
		m_ConsPrint("Can't Find A Suitable PixelFormat.");
		sys_KillGL();
		return false;
	}
	else
		m_ConsPrint("Find A Suitable PixelFormat\n");



	DescribePixelFormat (hDC, PixelFormat, sizeof(PIXELFORMATDESCRIPTOR), &pfd_new);
	generic_format = pfd_new.dwFlags & PFD_GENERIC_FORMAT;
	generic_accelerated = pfd_new.dwFlags & PFD_GENERIC_ACCELERATED;

	if (generic_format && ! generic_accelerated)
	{
	  // software
		m_ConsPrint("OpenGL (TM) is not hardware accelerated. Please check your video card, or install opengl drivers.\n");
		return false;
	}
	else if (generic_format && generic_accelerated)
	{
	   // hardware - MCD
	}
	else if (! generic_format && ! generic_accelerated)
	{
	   // hardware - ICD
	}


	if(!SetPixelFormat(hDC,PixelFormat,&pfd))
	{
		m_ConsPrint("Can't Set The PixelFormat.");
		sys_KillGL();
		return false;
	}
	else
		m_ConsPrint("Set The PixelFormat\n");

	if (!(hRC=wglCreateContext(hDC)))
	{
		m_ConsPrint("Can't Create A GL Rendering Context.");
		sys_KillGL();
		return false;
	}
	else
		m_ConsPrint("Create A GL Rendering Context\n");

	if(!wglMakeCurrent(hDC,hRC))
	{
		m_ConsPrint("Can't Activate The GL Rendering Context.");
		sys_KillGL();
		return false;
	}
	else
		m_ConsPrint("Activate The GL Rendering Context\n");

	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	gl_SwapBuffer();
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	gl_SwapBuffer();

	ShowWindow(ghWnd,SW_SHOW);
	SetForegroundWindow(ghWnd);

	return true;							
}

void sys_KillGL()
{
	if(hRC)										
	{
		if (!wglMakeCurrent(NULL,NULL))					
			MessageBox(NULL,"Release Of DC And RC Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		if (!wglDeleteContext(hRC))					
			MessageBox(NULL,"Release Rendering Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hRC=NULL;										
	}
	if(hDC && !ReleaseDC(ghWnd,hDC))				
	{
		// this sux on voodoo
		//MessageBox(NULL,"Release Device Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hDC=NULL;									
	}
	if(fullscreen.value)									
	{
		ChangeDisplaySettings(NULL,0);		
	}
}

void sys_killVID()
{
	if(gIsServer && net_dedicated.value)
		return;

	IsConsoleActive = false;

	if(gIsMultiplayer && gIsServer && net_dedicated.value)
		return;
	sys_KillGL();
	IN_UnInit();
	ds_Cleanup();
	sys_KillWindow();
}

BOOL sys_setVID()
{
	if(gIsServer && net_dedicated.value)
		return true;

	sys_CreateWindow(NAME,ScreenX[(int)(videomode.value)],ScreenY[(int)(videomode.value)],(int)(colordepth.value));

	if(!sys_CreateGL(ScreenX[(int)videomode.value],ScreenY[(int)videomode.value],(int)colordepth.value))
	{
		m_ConsPrint("CreateGLWindow FAILED");
		return false;
	}
	if(!gl_InitGL())
	{
		sys_KillGL();
		game_QuitMessage("gl_InitGL Failed.");
		return false;
	}
	if(!IN_Init(ghInstance))
	{
		game_QuitMessage("di_IniDInput FAILED");
		sys_KillGL();
		return false;
	}
	if(!ds_Init(ghWnd, ghInstance))
	{
		m_ConsPrint("No sound available\n");
		soundsys.value = false;
	//	game_QuitMessage("ds_Init FAILED");
	//	sys_KillGL();
	//	return false;
	}
	ds_LoadSounds();
	sys_setGamma(gamma_.value);
	return true;
}

void ListDisplaySettings(void)
{
  DEVMODE devMode;
  LONG    i;
  LONG    modeExist;

	modeExist = EnumDisplaySettings(NULL, 0, &devMode);

	for (i=0; modeExist;i++)
	{
		modeExist = EnumDisplaySettings(NULL, i, &devMode);
	}
}

hBool ChangeResolution(dword w, dword h, dword bitdepth)
{
  DEVMODE devMode;
  LONG    modeExist;
  LONG    modeSwitch;
  LONG    closeMode = 0;
  LONG    i;
//  BYTE    buf[256];

	for (i=0; ;i++)
	{
		modeExist = EnumDisplaySettings(NULL, i, &devMode);

		// Check if we ran out of modes
		if (!modeExist)
		{
			ListDisplaySettings();
			break;
		}

		// Check for a matching mode
		if((devMode.dmBitsPerPel == bitdepth) && (devMode.dmPelsWidth == w) && (devMode.dmPelsHeight == h))
		{
			//sprintf(buf, "Trying %ldx%ldx%ld-bit color (%ld hertz)\n", devMode.dmPelsWidth, devMode.dmPelsHeight, devMode.dmBitsPerPel, devMode.dmDisplayFrequency);
			modeSwitch = ChangeDisplaySettings(&devMode, CDS_FULLSCREEN); 
			if(modeSwitch==DISP_CHANGE_SUCCESSFUL)
				return TRUE;
			if(!closeMode)
				closeMode = i;
		}
	}

	//Might be running in Windows95, let's try without the hertz change 
	EnumDisplaySettings(NULL, closeMode, &devMode);
	devMode.dmBitsPerPel = bitdepth; 
	devMode.dmPelsWidth  = w; 
	devMode.dmPelsHeight = h; 
	devMode.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT; 
	//sprintf(buf, "Trying %ldx%ldx%ld-bit color (Win95 Hack)\n", devMode.dmPelsWidth, devMode.dmPelsHeight, devMode.dmBitsPerPel, devMode.dmDisplayFrequency);
	modeSwitch = ChangeDisplaySettings(&devMode, CDS_FULLSCREEN); 
	if(modeSwitch == DISP_CHANGE_SUCCESSFUL)
		return TRUE;

	// try with separate changes to bitdepth and resolution
	devMode.dmFields = DM_BITSPERPEL; 
	//sprintf(buf, "Trying %ldx%ldx%ld-bit color (Separate BPP switch)\n", devMode.dmPelsWidth, devMode.dmPelsHeight, devMode.dmBitsPerPel, devMode.dmDisplayFrequency);
	modeSwitch = ChangeDisplaySettings(&devMode, CDS_FULLSCREEN);
  
	if(modeSwitch == DISP_CHANGE_SUCCESSFUL)
	{
		devMode.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT; 
		//sprintf(buf, "Trying %ldx%ldx%ld-bit color (Separate resolution switch)\n", devMode.dmPelsWidth, devMode.dmPelsHeight, devMode.dmBitsPerPel, devMode.dmDisplayFrequency);
		modeSwitch = ChangeDisplaySettings(&devMode, CDS_FULLSCREEN); 
		if(modeSwitch == DISP_CHANGE_SUCCESSFUL)
			return TRUE;
		ChangeDisplaySettings(NULL, 0);
	}
	return FALSE; 
}




void sys_RestartVID()
{
	sys_killVID();
	if(!sys_setVID())
	{
		ProgramState = PS_QUIT;
		return;
	}

	// remet la musique en route
	if(gNbTrack >= 8  && gNbTrack <= 11)
		game_LaunchPresMusic();
	else
		ds_LoadStreamWAVE(game_GetMusicString((int)music_track.value));

	//et recharge les textures
	im_ReloadTextures();
}

void sys_end()
{
	// le gamma de l'ecran par defaut est rétablit
	//SetDeviceGammaRamp(hDC,gGammaRamp);
	sys_setGamma(1);

	Gun1LaunchedFaceList = liste_Delete(Gun1LaunchedFaceList);
	Gun3FaceList = liste_Delete(Gun3FaceList);

	sys_killVID();

	if(gIsMultiplayer)
	{
		//if(gIsServer)
			//sv_Destroy();
		//else
			//cl_Destroy();
	}

	IN_UnInit();
	cons_UninitConsole();

	if(ErrorMessage[0] != '\0')
		MessageBox(0,ErrorMessage,NAME,0);

#ifdef H_LINUX
	SDL_Quit();
#endif

	exit(0);
}

void LoadingError(char *message)
{
	MSG		msg;

	m_ConsPrint("%s\n", message);

	while(!gMainProcessDone)
	{
		if(PeekMessage(&msg,NULL,0,0,PM_REMOVE))
		{
			if(IsDialogMessage(hWndDialog, &msg) == false)
			{
				if(msg.message==WM_QUIT)
					gMainProcessDone = true;
				else
				{
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
			}
		}
		else
		{
		}
	}
	sys_end();
}

void w32_setGammaRamp(unsigned short *newgammaramp)
{
	hBool res;

	if(!hDC)
		return;

	res = SetDeviceGammaRamp(hDC,newgammaramp);
	m_ConsPrint("Gamma : %d\n",(int)res);
}

void sys_setGamma(double gamma)
{
	int		i;
	double	div;
	unsigned short	gammaramp[3*256];
	int		value;
	hBool	res;

	if(!hDC)
		return;

	if(fullscreen.value == false)
		return;

	res = GetDeviceGammaRamp(hDC, gammaramp);
	div = 1/256.0f;
	for(i=0 ; i<256 ; i++)
	{
		value = (int)min(65535.0,max(0,pow((double)i*div,gamma)*65535.0));
		//m_ConsPrint("%d\n",value);
		gammaramp[i+512] = gammaramp[i+256] = gammaramp[i] = value;
	}
	res = SetDeviceGammaRamp(hDC,gammaramp);
	m_ConsPrint("Gamma set to : %f\n",(float)gamma);
}


//#ifdef H_WINDOWS

int WIN_main(void)
{
	char	filename[260];
	MSG		msg;

	disp_InitMillisecond();
	window = -1;
	done = false;
	memset(ErrorMessage,0,255);
	disp_Init();

	LoadLibrary("Riched32.dll");
	GetModuleFileName(NULL,filename,260);
	ghInstance = GetModuleHandle( NULL );

//	hWndDialog = CreateDialog (ghInstance,MAKEINTRESOURCE(IDD_DIALOG4),NULL,DialogProc4);
//	ShowWindow(hWndDialog, true);

	if(!m_InitConsole())
	{
		game_QuitMessage("m_InitConsole FAILED");
		goto ending;
	}
	key_CheckGame();
	gIsMultiplayer = false;
	EnableWindow(hWndDialog, false);
	
	sys_AntiPirate();
	

	gIsDemo = false; //

/*
	if(shareware)
	{
		if(!DialogBox(ghInstance,MAKEINTRESOURCE(IDD_DIALOG1),NULL,DialogProc))
			goto ending;
	}
	else
	{
		if(!DialogBox(ghInstance,MAKEINTRESOURCE(IDD_DIALOG2),NULL,DialogProc2))
			goto ending;
	}
	EnableWindow(hWndDialog, true);
	if(gIsServer && net_dedicated.value)
		SetFocus(hWndDialog);
	else
	{*/
		ShowWindow(hWndDialog, false);
//	}

	if(!sys_RegisterWindow(NAME))
	{
		if(ErrorMessage[0] == '\0')
			game_QuitMessage("Failed To Register The Window Class.");
		goto ending;
	}

	if(!sys_setVID())
	{
		m_ConsPrint("Failed to set the video mode\n");
		goto ending;
	}

	if(!im_Init())
	{
		LoadingError("im_Init FAILED!!");
	}

	ProgramState = PS_LOADING;

	if(gIsServer && net_dedicated.value)
	{
		hWndDialog = CreateDialog (ghInstance,MAKEINTRESOURCE(IDD_DIALOG4),NULL,DialogProc4);
		ShowWindow(hWndDialog, true);

		gMainProcessDone = false;
		while(!gMainProcessDone)
		{
			if(PeekMessage(&msg,NULL,0,0,PM_REMOVE))
			{
				if(IsDialogMessage(hWndDialog, &msg) == false)
				{
					if(msg.message==WM_QUIT)
						gMainProcessDone = true;
					else
					{
						TranslateMessage(&msg);
						DispatchMessage(&msg);
					}
				}
			}
			else
			{
				gMainProcessDone = GameProcess();
			}
		}
		sys_end();
		return 1;
	}

	DI_ReacquireMouse();
	DI_ReacquireKeyboard();

	while(1)
	{
		if(PeekMessage(&msg,NULL,0,0,PM_REMOVE))
		{
			if(msg.message==WM_QUIT)
				done = true;
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			done = GameProcess();
			if(done)
			{
				sys_end();
				return 1;
			}
			gl_SwapBuffer();
			ds_PlayBuffer();
		}
	}

ending:

	sys_end();
	return 1;
}


#ifndef _DEBUG

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPreInst, LPSTR lpszCmdLine, int nCmdShow)
{
	return WIN_main();
}

#else

int main(int argc, char **argv)
{
	return WIN_main();
}

#endif





