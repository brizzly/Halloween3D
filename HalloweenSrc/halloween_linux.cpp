
//#include "halloween_linux.h"
#include "alloween.h"

#ifdef H_MAC
#include <Carbon/Carbon.h>
#include <ApplicationServices/ApplicationServices.h>
#include "KMiscTools.h"
#endif


#define MAX_NAME	256
#define MAX_KEY		11


int 			videoFlags;
//const SDL_VideoInfo 	*videoInfo;
//enabled by farox
#ifdef H_LINUX
SDL_Surface 		*surface;
#endif


#ifdef H_MAC
KWindow			*fenetre;
#endif

char	ErrorMessage[255];
hBool	done;
int	window;
char	NameText[260];
char	KeyText[260];
int	shareware;
hBool	gIsMultiplayer;
hBool	gIsServer;
int	gMultiMapID = 1;
hBool	gMainProcessDone;
int	shareware2;
int	window_id;
hBool	IsWaitKey = false;
int	GlobalTextureBind = -1;
int	GlobalEntities = 0;
int	ScreenX[] = { 512, 640, 800, 1024, 1152, 1280, 1600 };
int	ScreenY[] = { 384, 480, 600, 768, 864, 960, 1200 };


float swapendf32( float floatValue )
{
   union mapUnion { unsigned char bytes[ 4 ]; float floatVal; } *mapping, newMapping;
   mapping = (union mapUnion *) &floatValue;
   newMapping.bytes[ 0 ] = mapping->bytes[ 3 ];
   newMapping.bytes[ 1 ] = mapping->bytes[ 2 ];
   newMapping.bytes[ 2 ] = mapping->bytes[ 1 ];
   newMapping.bytes[ 3 ] = mapping->bytes[ 0 ];
   return newMapping.floatVal;
}

void sys_AntiPirate()
{
	shareware = 1-shareware2;
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


// HKEY stuff

void key_CheckGame()
{
	if(key_IsValid(NameText,KeyText))
	{
		shareware2 = 56; //'56'; // 1
	}
	else
	{
		shareware2 = 55; //'55'; // 0
	}
	shareware2 -= 55; //'55';
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

int key_IsValid(char *name, char *key)
{
	int		i1,i2;
        FILE		*f;
       
   	#ifdef H_MAC
        f = fopen(KMiscTools::makeFilePath("system\\textures\\systex\\h1m7\\lm_big_002.tga"), "rb");
        if(!f)
            return false;
        fclose(f);
        return true;
  	#endif
        

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



hBool sys_RegisterWindow(char* title)
{
	return true;
}

hBool sys_CreateWindow(char* title, int width, int height, int bits)
{
	return true;
}

void sys_KillWindow()
{
}

hBool sys_CreateGL(int width, int height, int bits)
{
#ifdef H_MAC
	bool res;

	fenetre = new KWindow;
	
	if(fullscreen.value)
	{
		res = fenetre->createGameWindow(	(int)ScreenX[(int)videomode.value],
											(int)ScreenY[(int)videomode.value],
											(int)(colordepth.value),
											false,
											NAME);						
	}
	else
	{
		res = fenetre->createGameWindow(	(int)ScreenX[(int)videomode.value],
											(int)ScreenY[(int)videomode.value],
											(int)(colordepth.value),
											true,
											NAME);	
	}
	
	return res;
#endif

#ifdef H_LINUX
   	if(SDL_Init( SDL_INIT_VIDEO ) < 0)
	{
	    m_ConsPrint("SDL_Initialization failed: %s\n", SDL_GetError());
	    return false;
	}
        
        SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE, (int)(colordepth.value) ); // 16
        SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );
        SDL_GL_SetAttribute( SDL_GL_STENCIL_SIZE, 0 );
        SDL_GL_SetAttribute( SDL_GL_ALPHA_SIZE, 0 );
        SDL_GL_SetAttribute( SDL_GL_BUFFER_SIZE, 16 ); // 24
        
    	videoFlags  = SDL_OPENGL;          
    //	videoFlags |= SDL_GL_DOUBLEBUFFER; 
    //	videoFlags |= SDL_HWPALETTE;     
    //    videoFlags |= SDL_HWSURFACE; 
        videoFlags |= SDL_HWACCEL;      
        if(fullscreen.value)
    	{
    		videoFlags |= SDL_FULLSCREEN;      
	}

    	// get a SDL surface
    	surface = SDL_SetVideoMode(	ScreenX[(int)videomode.value],
     				ScreenY[(int)videomode.value],
				0,
				videoFlags );

	//SDL_EnableUNICODE(1);

    	// Verify there is a surface
    	if ( !surface )
	{
	    m_ConsPrint("Video mode set failed: %s\n", SDL_GetError( ) );
	    return false;
	}

	SDL_WM_SetCaption(NAME, NULL);
		
	return true;
#endif

#ifdef H_WINDOWS

    	// Fetch the video info
    	videoInfo = SDL_GetVideoInfo();

    	if(!videoInfo)
	{
	    fprintf( stderr, "Video query failed: %s\n",
		     SDL_GetError( ) );
	    return false;
	}

    	// the flags to pass to SDL_SetVideoMode
    	videoFlags  = SDL_OPENGL;          /* Enable OpenGL in SDL */
    	videoFlags |= SDL_GL_DOUBLEBUFFER; /* Enable double buffering */
    	videoFlags |= SDL_HWPALETTE;       /* Store the palette in hardware */
    	videoFlags |= SDL_RESIZABLE;       /* Enable window resizing */
    	if(fullscreen.value)
    	{
    		videoFlags |= SDL_FULLSCREEN;       /* Enable window resizing */
	}

    /* This checks to see if surfaces can be stored in memory */
    if ( videoInfo->hw_available )
    {
	videoFlags |= SDL_HWSURFACE;
    }
    else
    {
	videoFlags |= SDL_SWSURFACE;
        m_ConsPrint("No Hardware Accelerated Graphic mode found.. quiting\n");
        sys_end();
        return false;
    }

    /* This checks if hardware blits can be done */
    if ( videoInfo->blit_hw )
	videoFlags |= SDL_HWACCEL;

    /* Sets up OpenGL double buffering */
    SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );

    	// get a SDL surface
    	surface = SDL_SetVideoMode(	ScreenX[(int)videomode.value],
     				ScreenY[(int)videomode.value],
				(int)colordepth.value,
				videoFlags );

	//SDL_EnableUNICODE(1);

    	// Verify there is a surface
    	if ( !surface )
	{
	    m_ConsPrint("Video mode set failed: %s\n", SDL_GetError( ) );
	    return false;
	}

	SDL_WM_SetCaption(NAME, NULL);

	return true;
#endif
}

void sys_KillGL()
{
#ifdef H_MAC
	if(fenetre)
	{
		fenetre->terminate();
		delete fenetre;
		fenetre = NULL;
	}
#else
	SDL_FreeSurface(surface);
	SDL_QuitSubSystem(SDL_INIT_VIDEO);
	//killGLWindow();
#endif
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

int sys_setVID()
{
	if(gIsServer && net_dedicated.value)
		return true;

	if(!sys_CreateGL(ScreenX[(int)videomode.value],ScreenY[(int)videomode.value],(int)colordepth.value))
	{
		m_ConsPrint("CreateGLWindow FAILED");
		return false;
	}

	sys_CreateWindow(NAME,ScreenX[(int)videomode.value],ScreenY[(int)videomode.value],(int)colordepth.value);

	if(!gl_InitGL())
	{
		sys_KillGL();
		game_QuitMessage("gl_InitGL Failed.");
		return false;
	}

	if(!IN_Init())
	{
		game_QuitMessage("di_IniDInput FAILED");
		sys_KillGL();
		return false;
	}

	if(!ds_Init())
	{
		m_ConsPrint("No sound available\n");
		soundsys.value = false;
	}
	ds_LoadSounds();
	ds_AdjustSoundVolumes();
	ds_AdjustMusicVolumes();
	
	sys_setGamma(gamma_.value);
	return true;
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

        ds_AdjustSoundVolumes();
        ds_AdjustMusicVolumes();
        
	//et recharge les textures
	im_ReloadTextures();
}

void sys_end()
{
	// le gamma de l'ecran par defaut est rétablit
	//SetDeviceGammaRamp(hDC,gGammaRamp);
//	m_cons_SaveConfigToFile();
	
	sys_setGamma(1);

	Gun1LaunchedFaceList = liste_Delete(Gun1LaunchedFaceList);
	Gun3FaceList = liste_Delete(Gun3FaceList);

	sys_killVID();

	if(gIsMultiplayer)
	{
		if(gIsServer)
			sv_Destroy();
		else
			cl_Destroy();
	}

	IN_UnInit();
	cons_UninitConsole();

	if(ErrorMessage[0] != '\0') {
		printf(ErrorMessage);
	}

#ifdef H_MAC
	return; // no exit(0); on MacOS
#else
	SDL_Quit();
#endif
	
	exit(0);
}

void LoadingError(char *message)
{
	m_ConsPrint("%s\n", message);
	sys_end();
}

void w32_setGammaRamp(unsigned short *newgammaramp)
{
	//hBool res;
	//res = SetDeviceGammaRamp(hDC,newgammaramp);
	//m_ConsPrint("Gamma : %d\n",(int)res);
}

long Min(long a, long b)
{
    if(a < b)
        return a;
    return b;
}

long Max(long a, long b)
{
    if(a > b)
        return a;
    return b;
}

void sys_setGamma(float gamma)
{
	int		i;
	double		div;

#ifdef H_WIN
	unsigned short	gammaramp[3*256];
	int		value;
	hBool		res;
#endif

#ifdef H_MAC
  	float		fvalue;
   	CGGammaValue 	RedTable[256];
    CGGammaValue 	GreenTable[256];
    CGGammaValue 	BlueTable[256];
#endif

//	if(fullscreen.value == false)
//		return;
                
#ifdef H_MAC
    div = 1.0/256.0;
	for(i=0 ; i<256 ; i++)
	{
		fvalue = Min(65535.0,Max(0,pow((double)i*div,(double)gamma)*65535.0)) / 65535.0f;         
                RedTable[i] = fvalue;
                GreenTable[i] = fvalue;
                BlueTable[i] = fvalue;
	}
        CGSetDisplayTransferByTable( 0, 256, RedTable, GreenTable, BlueTable);
#endif                
      
#ifdef H_WINDOWS
	//res = GetDeviceGammaRamp(hDC, gammaramp);
	div = 1/256.0f;
	for(i=0 ; i<256 ; i++)
	{
		//value = (int)min(65535.0,max(0,pow((double)i*div,gamma)*65535.0));
		//m_ConsPrint("%d\n",value);
		//gammaramp[i+512] = gammaramp[i+256] = gammaramp[i] = value;
	}
	//res = SetDeviceGammaRamp(hDC,gammaramp);
#endif

#ifdef H_LINUX

#endif

	m_ConsPrint("Gamma set to : %f\n",(float)gamma);
}

bool sys_GameEvent()
{
#ifdef H_MAC
	
	IN_ReadMouse();
	IN_ReadKeyboard();
	if(fenetre)
	{
		if(fenetre->isQuit() == true)
			return false;
	}
	if(ProgramState == PS_QUIT)
		return false;
	return true;

#else
	SDL_Event event;

	while(SDL_PollEvent(&event))
	{
		switch(event.type)
		{
			case SDL_ACTIVEEVENT:
			    /* Something's happend with our focus
			     * If we lost focus or we are iconified, we
			     * shouldn't draw the screen
			     */
				if ( event.active.gain == 0 )
                                {
                                    printf("active app false");
				//	isActive = FALSE;
				}
                                else
                                {
                                    printf("active app true");
                                    //isActive = TRUE;
                                }
			break;

			case SDL_VIDEORESIZE:
			/*
			surface = SDL_SetVideoMode( event.resize.w, event.resize.h, 16, videoFlags );
			if ( !surface )
			{
			    fprintf( stderr, "Could not get a surface after resize: %s\n", SDL_GetError( ) );
				return false;
			}
			gl_ReSizeGLScene(event.resize.w, event.resize.h);
			gl_ChangeFOV(fov.value);
			*/
			break;

			case SDL_KEYUP:
			IN_SetKeyboard(event.key.keysym.sym, false);
			break;

			case SDL_KEYDOWN:
			//SDL_EnableUNICODE(1);

		//	m_ConsPrint("key: %d Name: %s\n",
		//	 (int)event.key.keysym.scancode,
		//	  SDL_GetKeyName( event.key.keysym.sym ));

			IN_SetKeyboard(event.key.keysym.sym, true);
			break;

			case SDL_MOUSEBUTTONDOWN:
			switch(event.button.button)
			{
				case SDL_BUTTON_LEFT:
				IN_SetMouseButons(1);
				break;

				case SDL_BUTTON_MIDDLE:
				IN_SetMouseButons(2);
				break;

				case SDL_BUTTON_RIGHT:
				IN_SetMouseButons(3);
				break;
			}
			break;

			case SDL_MOUSEBUTTONUP:
			switch(event.button.button)
			{
				case SDL_BUTTON_LEFT:
				IN_SetMouseButons(-1);
				break;

				case SDL_BUTTON_MIDDLE:
				IN_SetMouseButons(-2);
				break;

				case SDL_BUTTON_RIGHT:
				IN_SetMouseButons(-3);
				break;
			}
			break;

			case SDL_MOUSEMOTION:
		//	printf("Mouse(%d, %d) (%d, %d)\n",
		//				event.motion.x,
		//				event.motion.y,
		//				event.motion.xrel,
		//				event.motion.yrel);

			IN_MouseGetOffet(event.motion.x, event.motion.y, event.motion.xrel, event.motion.yrel);
			break;

			case SDL_QUIT:
			return false;

			default:
			break;
		}
	}
	return true;
	#endif
}

const GLfloat EDGE = 50.0;
const GLfloat SQR3 = sqrt(3);
const GLfloat SQR6 = sqrt(6);

void display()
{
	/*
	GLfloat vertices[4][3] = { {0.0, 0.0, 0.0}, {EDGE, 0.0, 0.0}, {EDGE/2, EDGE*SQR3/2, 0.0}, {EDGE/2, EDGE*SQR3/6, EDGE*SQR6/3} };
	GLfloat colors[4][3] = { {1.0, 1.0, 0.5}, {0.5, 1.0, 0.5}, {0.5, 0.8, 0.5}, {0.5, 0.5, 1.0} };
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glBegin(GL_TRIANGLES);
	//divide_triangle(vertices, colors);
	glEnd();
	glFlush();
	*/
	
	if(GameProcess())
	{
	//	break;
	}
	if(sys_GameEvent() == false)
	{
		GameProcess();
//		break;
	}
	gl_SwapBuffer();
	ds_PlayBuffer();
	
}

void MACStarter()
{
	//main_Alternate(0, NULL);
	
	m_ConsPrint("Start game Halloween()\n");

#ifdef H_LINUX
	SDL_Init(SDL_INIT_TIMER);
#endif
	
#ifdef H_MAC
	KMiscTools::initMiscTools(GAMEVERSION_LITE);
#endif
	
	memset(ErrorMessage,0,255);
	
	WorldColorR = 1;
	WorldColorG = 1;
	WorldColorB = 1;
	
	if(!m_InitConsole())
	{
		game_QuitMessage("m_InitConsole FAILED");
		//goto ending;
		return;
	}
	
	key_CheckGame();
	gIsMultiplayer = false;
	sys_AntiPirate();
	if(shareware)
	{
	}
	else
	{
	}
	
	if(!sys_setVID())
	{
		m_ConsPrint("Failed to set the video mode\n");
		//goto ending;
		return;
	}
	
	if(!im_Init())
	{
		LoadingError("im_Init FAILED!!");
	}
	
//	ProgramState = PS_DEBUG;//PS_LOADING;
	ProgramState = PS_LOADING;
}

bool MACLoop()
{
	if(GameProcess())
	{
		return false;
	}
	if(sys_GameEvent() == false)
	{
		GameProcess();
		return false;
	}
	gl_SwapBuffer();
	ds_PlayBuffer();
	return true;
}

void MACend()
{
	sys_end();
}


//by farox
//int main_Alternate(int argc, char **argv)
int main(int argc, char **argv)
{
	m_ConsPrint("main()\n");
	
	/*
#ifdef H_MAC
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE | GLUT_DEPTH);
#endif

	glutInitWindowSize(500, 500*SQR3/2);
	//glutInitWindowPosition(0, 0);
	glutCreateWindow("GLUT Program");
	
	glutDisplayFunc(display);
	*/
	
	
	#ifdef H_LINUX
        SDL_Init(SDL_INIT_TIMER);
	#endif
	
	#ifdef H_MAC
		KMiscTools::initMiscTools(GAMEVERSION_LITE);
	#endif

	memset(ErrorMessage,0,255);

	WorldColorR = 1;
	WorldColorG = 1;
	WorldColorB = 1;

	if(!m_InitConsole())
	{
		game_QuitMessage("m_InitConsole FAILED");
		goto ending;
	}

	key_CheckGame();
	gIsMultiplayer = false;
	sys_AntiPirate();
	if(shareware)
	{
	}
	else
	{
	}
/*
	if(!sys_setVID())
	{
		m_ConsPrint("Failed to set the video mode\n");
		goto ending;
	}
*/
	if(!im_Init())
	{
		LoadingError("im_Init FAILED!!");
	}
        
	ProgramState = PS_LOADING;

/*
	if(gIsServer && net_dedicated.value)
	{
		gMainProcessDone = false;
		while(!gMainProcessDone)
		{
			gMainProcessDone = GameProcess();
		}
		goto ending;
	}
*/

//	fenetre->terminate();
//	delete fenetre;
//	fenetre = NULL;

	//glutMainLoop();
	
	
	while(1)
	{
		if(GameProcess())
		{
			break;
		}
        if(sys_GameEvent() == false)
		{
			GameProcess();
			break;
		}                
		gl_SwapBuffer();
		ds_PlayBuffer();
	}

	
ending:

	sys_end();
	return 0;
}

