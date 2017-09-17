
#include "alloween.cpp"


#ifdef H_WINDOWS

#include "misc/resource.h"
extern HWND hWndDialog;

#endif


hBool		DropKeyPressed=false;
hBool		Key_EnterPressed=false;
char		*ConsoleBuffer;
char		*CmdLineArg;
char		ConsoleBufferLine[CONSOLE_WIDTH];
char		ConsoleBufferLine_save[CONSOLE_WIDTH];
int			key_tab_pressed;
char		buffer[CONSOLE_MAXDISPCHAR];


cmd_function_t	*cmd_functions = NULL;
cvar_t			*cvar_vars = NULL;

cvar_t		maxspeed = { MAX_TRAVEL_VELOCITY,"0.23" };
cvar_t		groundacc = {GROUND_ACCFACTOR,"0.03"};
cvar_t		groundslow = {GROUND_SLOWFACTOR,"0.020"}; // 0.020
cvar_t		airacc = {AIR_ACCFACTOR,"0.004"};
cvar_t		airslow = {AIR_SLOWFACTOR,"0.0"};
cvar_t		visent = {VISENT,"0"};
cvar_t		epsilon = {EPSILON,"0.001"};
cvar_t		draw_fps = {DRAWFPS,"0"};
cvar_t		player_vel = {PVEL,"1.0"};
cvar_t		camera_vel = {CVEL,"0.2"};
cvar_t		player_rvel = {PRVEL,"0.10"};
cvar_t		player_look_ud = {PLOOKUD,"90.0"};
cvar_t		draw_face = {DRAWFACE,"1"};
cvar_t		draw_tris = {DRAWTRIS,"0"};
cvar_t		draw_one = {DRAWONE,"0"};
cvar_t		draw_portal = {DRAWPORTAL,"0"};
cvar_t		invert_mouse = {INVERTMOUSE,"0"};
cvar_t		draw_normal = {DRAWNORMAL,"0"};
cvar_t		player_view = {PLAYERVIEW,"1"};
cvar_t		draw_bound = {DRAWBOUND,"0"};
cvar_t		draw_cell = {DRAWCELL,"0"};
cvar_t		noclip_wall = {NOCLIP,"0"};
cvar_t		fov	= {FOV,"120"}; // 90
cvar_t		mapfile	= {MAPFILE,"h1m1.map"};
cvar_t		gravity = {GRAVITY,"8.0"};
cvar_t		drawgun = {DRAWGUN,"1"};
cvar_t		jumpstep = {CREATURE_JUMPSTEP,"0.25"};
cvar_t		soundsys = {SOUNDSYSTEM,"1"};
cvar_t		respawn = {RESPAWNVAR,"1"};
cvar_t		god = {GOD,"0"};
cvar_t		monster = {MONSTER,"1"};
cvar_t		bombslow = {BOMB_SLOWFACTOR,"0.3"};
cvar_t		musicvol = {MUSICVOL,"50.0"};
cvar_t		soundvol = {SOUNDVOL,"50.0"};
cvar_t		autoswitch = {AUTOSWITCH,"1"};
cvar_t		fullscreen = {FULLSCREEN,"0"};
cvar_t		colordepth = {COLORDEPTH,"16"}; // 32
cvar_t		videomode = {VIDEOMODE,"2"}; // 640x480 | 800x600
cvar_t		bms = {BLENDMODESRC,"4"};
cvar_t		bmd = {BLENDMODEDST,"5"};
cvar_t		statutbar = {STATUTBAR,"1"};
cvar_t		animspeed = {ANIMSPEED,"1.0"};
cvar_t		lightmap = {LIGHTMAP,"1"};
cvar_t		culling = {CULLING,"1"};
cvar_t		render_ent = {RENDERENT,"1"};
cvar_t		directstart = {DIRECTSTART,"0"};
cvar_t		developer = {DEVELOPER,"0"};
cvar_t		offset = {OFFSET,"20"};
cvar_t		leaf = {LEAF,"-1"};
cvar_t		record = {RECORD,"0"};
cvar_t		stoprecord = {STOPRECORD,"0"};
cvar_t		play = {PLAY,"0"};
cvar_t		stopplay = {STOPPLAY,"0"};
cvar_t		noscript = {NOSCRIPT,"0"};

#ifdef H_MAC
cvar_t		gamekey_up = {GAMEKEY_UP,"1"};
cvar_t		gamekey_down = {GAMEKEY_DOWN,"2"};
cvar_t		gamekey_left = {GAMEKEY_LEFT,"0"};
cvar_t		gamekey_right = {GAMEKEY_RIGHT,"3"};
cvar_t		gamekey_jump = {GAMEKEY_JUMP,"61"};
cvar_t		gamekey_attack = {GAMEKEY_ATTACK,"256"};
cvar_t		gamekey_axe = {GAMEKEY_AXE,"52"};
cvar_t		gamekey_arbalet = {GAMEKEY_ARBALET,"53"};
cvar_t		gamekey_bombgun = {GAMEKEY_BOMBGUN,"54"};
cvar_t		gamekey_tromblon = {GAMEKEY_TROMBLON,"55"};
cvar_t		gamekey_crouch = {GAMEKEY_CROUCH,"9"};
cvar_t		gamekey_action = {GAMEKEY_ACTION,"7"};
cvar_t		gamekey_run = {GAMEKEY_RUN,"4"};
#else
cvar_t		gamekey_up = {GAMEKEY_UP,"200"};
cvar_t		gamekey_down = {GAMEKEY_DOWN,"208"};
cvar_t		gamekey_left = {GAMEKEY_LEFT,"203"};
cvar_t		gamekey_right = {GAMEKEY_RIGHT,"205"};
cvar_t		gamekey_jump = {GAMEKEY_JUMP,"257"};
cvar_t		gamekey_attack = {GAMEKEY_ATTACK,"256"};
cvar_t		gamekey_axe = {GAMEKEY_AXE,"2"};
cvar_t		gamekey_arbalet = {GAMEKEY_ARBALET,"3"};
cvar_t		gamekey_bombgun = {GAMEKEY_BOMBGUN,"4"};
cvar_t		gamekey_tromblon = {GAMEKEY_TROMBLON,"5"};
cvar_t		gamekey_crouch = {GAMEKEY_CROUCH,"54"}; // 157
cvar_t		gamekey_action = {GAMEKEY_ACTION,"28"};
cvar_t		gamekey_run = {GAMEKEY_RUN,"54"};
#endif

cvar_t		map_letter = {"mapletter","0"};
cvar_t		kill_depla = {"killdepla","0"};
cvar_t		kill_morbak = {"killmorba","0"};
cvar_t		kill_zeubat = {"killzeubat","0"};
cvar_t		kill_subzerat = {"killsubzerat","0"};
cvar_t		kill_zemummy = {"killzemummy","0"};
cvar_t		map_complete = {"mapcomplete","0"};
cvar_t		music_track = {"musictrack","0"};
cvar_t		pres_track = {"prestrack","0"};
cvar_t		resetgame = {RESETGAME,"0"};
cvar_t		picmip = {PICMIP,"1"};
cvar_t		difficulty = {"difficulty","1"};
cvar_t		gameslot = {"gameslot","1"};
cvar_t		gamma_ = {GAMMA,"1"};
cvar_t		tflame = {TFLAME,"0.035"};
cvar_t		ghost = {GHOST,"0"};
cvar_t		terminator = {TERMINATOR,"0"};
cvar_t		net_port = {C_NETPORT, "24650"};
cvar_t		net_ip = {C_NETIP, "localhost"};
cvar_t		net_mapid = {C_NETMAPID, "1"};
cvar_t		net_clname = {C_NETCLIENTNAME, "UnnamedPlayer"};
cvar_t		net_svname = {C_NETSERVERNAME, "HalloweenServer"};
cvar_t		net_password = {C_NETPASSWORD, "SALEDEP"};
cvar_t		net_gametype = {C_NETGAMETYPE, "0"};
cvar_t		net_dedicated = {C_NETDEDICATED, "0"};
cvar_t		notextures = {C_NOTEXTURES,"0"};
cvar_t		bestscore1 = {C_BESTSCORE1,"0"};
cvar_t		bestscore2 = {C_BESTSCORE2,"0"};
cvar_t		bestscore3 = {C_BESTSCORE3,"0"};
cvar_t		bestscore4 = {C_BESTSCORE4,"0"};
cvar_t		bestscore5 = {C_BESTSCORE5,"0"};
cvar_t		bestscore6 = {C_BESTSCORE6,"0"};
cvar_t		bestscore7 = {C_BESTSCORE7,"0"};
cvar_t		bestscore8 = {C_BESTSCORE8,"0"};
cvar_t		bestscore9 = {C_BESTSCORE9,"0"};
cvar_t		fogmode = {C_FOGMODE,"0"};
cvar_t		fogdensity = {C_FOGDENSITY,"0.001f"};


cvar_t	*	cvarArray[1000];
int			numberCvar = 0;


void Cvar_RegisterVariable(cvar_t *variable)
{
	//cvar_t * newVar = new cvar_t;
	//newVar->value = (float)atof(variable->string);
	cvarArray[numberCvar] = variable;
	numberCvar++;

//	char * nameTemp = new char[200];
//	strcpy(nameTemp, variable->name);
//	variable->name = nameTemp;

//	char * stringTemp = new char[200];
//	strcpy(stringTemp, variable->string);
//	variable->string = stringTemp;

	variable->value = (float)atof(variable->string);

	//variable->string = NULL;
	variable->next = cvar_vars;
	cvar_vars = variable;
	m_ConsPrint("New Variable Added: %s %s %f\n", variable->name, variable->string, variable->value);
}

cvar_t *Cvar_FindVar(char *var_name)
{
/*#ifdef H_WINDOWS
	for (int i = 0; i < numberCvar; i++)
	{
		cvar_t * var = cvarArray[i];
		if (!strcmp(var_name, var->name)) {
			return var;
		}
	}
#endif*/
	
	cvar_t	*var;
	
	for (var = cvar_vars; var; var = var->next) {
		//if(var->name != NULL)
		if (!strcmp(var_name, var->name))
			return var;
	}

	return NULL;
}

float Cvar_VariableValue(char *var_name)
{
	cvar_t	*var;
	
	var = Cvar_FindVar (var_name);
	if (!var)
		return 0;
	//return A_atof (var->string);
	return var->value;
}

void Cmd_AddCommand(char *cmd_name, xcommand_t function)
{
	cmd_function_t	*cmd;

	cmd = (cmd_function_t*)s_malloc(sizeof(cmd_function_t));
	cmd->name = cmd_name;
	cmd->function = function;
	cmd->next = cmd_functions;
	cmd_functions = cmd;
	//m_ConsPrint("%s : New Command Added\n",cmd_name);
}

hBool m_InitConsole()
{
	static char	path[255];

	if(!m_InitConsoleBuffer()) {
		return false;
	}

	m_ConsPrint("WELCOME TO (%s)\n",GAMEVERSION);
	
	m_RegisterConsoleCmd();
	m_RegisterConsoleVar();

	sprintf(path,"%s%s",SYSDIR,CONFIG);
	cons_LoadConfig(path);

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

	sys_GetKeyDatas();
	m_ConsPrint("Console initialization done.\n");
	m_ConsPrint("Halloween game engine is ready..\n");
	return true;
}

hBool m_InitConsoleBuffer()
{
	KeyMask = (hBool*)s_malloc(KEYMASK_SIZE*sizeof(hBool));
	if(KeyMask==NULL) {
		return false;
	}
	memset(KeyMask,0,KEYMASK_SIZE*sizeof(hBool));
	
	ConsoleBuffer = (char*)s_malloc(CONSOLE_NB_CHAR*sizeof(char));
	if(ConsoleBuffer==NULL)
		return false;
	memset(ConsoleBuffer,0,CONSOLE_NB_CHAR*sizeof(char));
	memset(ConsoleBufferLine,0,CONSOLE_WIDTH-1);
	memset(ConsoleBufferLine_save,0,CONSOLE_WIDTH-1);
	return true;
}

void cons_UninitConsole()
{
	char	path[255];
	FILE	*file;

	if(developer.value)
	{
		m_FreeConsoleCmd();
		s_free(KeyMask);
		KeyMask = NULL;

		sprintf(path,"%s%s",SYSDIR,"console.log");
			
#ifdef H_MAC
		file = fopen(KMiscTools::makeFilePath(path), "w");
#else
		file=fopen(path,"w");
#endif
		
		fwrite(ConsoleBuffer,strlen(ConsoleBuffer),1,file);
		fclose(file);
		s_free(ConsoleBuffer);
		ConsoleBuffer = NULL;
	}
	else
	{
		m_FreeConsoleCmd();
		
		s_free(KeyMask);
		KeyMask = NULL;
		
		s_free(ConsoleBuffer);
		ConsoleBuffer = NULL;
	}
}

void m_ConsoleProcess()
{
	vect_t	pos;
	vect_t	siz;
	vect_t	ortho;
	Color_t	color;
	int		offset;
	float	ofX, ofY;
	static float	x = 320;
	static float	y = 240;
	static float	w = 640;
	static float	h = 320; //240;

	if(!IsConsoleActive)
		return;

	offset = (int)(disp_GetClock() * 0.008) % ROTMAX_I;
	ofX = (gFrameTime_msec * 0.005f * Sin_i(offset));
	ofY = (gFrameTime_msec * 0.005f * Cos_i(offset));
	x -= ofX;
	h += ofY;
	if( h < 240)
		h = 240;

	//disp_DrawSprite(txid_CONSOLE,0,240,(int)x,(int)y,0);

	pos.X = x-640;
	pos.Y = y;
	siz.X = w;
	siz.Y = h;
	ortho.X = 640;
	ortho.Y = 480;
	color.R = 1;
	color.G = 1;
	color.B = 1;
	disp_Sprite2D(txid_CONSOLE, ortho, pos, siz, false, color, false);

	pos.X = x;
	pos.Y = y;
	siz.X = w;
	siz.Y = h;
	ortho.X = 640;
	ortho.Y = 480;
	color.R = 1;
	color.G = 1;
	color.B = 1;
	disp_Sprite2D(txid_CONSOLE, ortho, pos, siz, false, color, false);

	disp_DrawLine(0,240,640,240,2,GLRED);
	m_DispConsoleText();
	g_Disp(CONSOLE_FONT, 16, 39, GAMEVERSION_LITE);
	m_EnterConsoleString(); 
}

void m_DispConsoleText()
{
	int		nRetourLigne;
	int		i;
	int		line, str;

	gPrintLine=0;

	for(i=0, nRetourLigne=0 ; ConsoleBuffer[i]!='\0' ; i++)
	{
		if(ConsoleBuffer[i] == '\n')
			nRetourLigne++;
	}
	if(nRetourLigne>CONSOLE_NB_LINE)
	{
		for(i=0,str=0 ; i<nRetourLigne-CONSOLE_NB_LINE ; i++)
		{
			while(ConsoleBuffer[str]!='\n')
			{
				if(ConsoleBuffer[str]=='\0')
					break;
				if(str>CONSOLE_NB_CHAR-2)
					break;
				str++;
			}
			str++;
		}
		line=0;
	}
	else
	{
		str=0;
		line = CONSOLE_NB_LINE - nRetourLigne;
	}
	g_DispString(CONSOLE_FONT,&ConsoleBuffer[str]);
	if(disp_GetClock() % 300 < 150)
	{
		g_DispStringLine(CONSOLE_FONT,16,"]%s%s",ConsoleBufferLine,"_");
	}
	else
	{
		g_DispStringLine(CONSOLE_FONT,16,"]%s%s",ConsoleBufferLine," ");
	}
}

void m_ConsoleCmdAddInput()
{
	char	cmdtext[255];

	if(!m_ConsoleFindText(cmdtext))
		return;

	m_ConsoleFindCommand(cmdtext);
}

void m_ConsPrint(char *Text, ...)
{
	int 	str = 0;
	va_list	msg;

	if(!ConsoleBuffer)
		return;

	va_start(msg, Text);
	vsprintf(buffer, Text, msg);
	va_end(msg);

	while(ConsoleBuffer[str]!='\0')
	{
		if(str>CONSOLE_NB_CHAR-2)
			break;
		str++;
	}

	if(strlen(buffer)+str < CONSOLE_NB_CHAR-1)
		memcpy(&ConsoleBuffer[str],buffer,strlen(buffer));

#ifdef H_WINDOWS
	if(hWndDialog)
	{
		char		*c;
		long		l;
		HWND		richy;
		char		temp[CONSOLE_NB_CHAR];

		for(c=buffer ; *c ; *c++)
		{
			if(*c == '\n')
			{
				*c = 0;
				break;
			}
		}
		sprintf(temp, "\n%s", buffer);
		
		richy = GetDlgItem(hWndDialog, IDC_EDITCONS);
		l = SendMessage(richy, WM_GETTEXTLENGTH, 0, 0);
		SendMessage(richy, EM_SETSEL, l ,l);
		SendMessage(richy, EM_REPLACESEL, false, (LPARAM)temp);
		SendMessage(richy, EM_SCROLLCARET, 0, 0);
		SendMessage(richy, EM_SETBKGNDCOLOR, 0, RGB(120,120,120));
	}
#endif

#ifdef H_LINUX
	printf(buffer);
#endif

#ifdef H_MAC
	printf(buffer);
#endif

#ifdef _DEBUG
	printf(buffer);
#endif

}

/*
=========================================================
Analyse et traite la commande tappé dans la console
=========================================================
*/
hBool m_ConsoleFindText(char *cmdtext)
{
	int		i;
	int		str;
	int		nRetourLigne;

	str = 0;
	nRetourLigne = 0;
	for(i=0 ; ; i++)
	{
		if(ConsoleBuffer[i] == '\0')
			break;
		if(i>CONSOLE_NB_CHAR-2)
			return false;
		if(ConsoleBuffer[i] == '\n')
			nRetourLigne++;
	}
	while(nRetourLigne)
	{
		while( ConsoleBuffer[str] != '\n')
		{
			str++;
			if(str>CONSOLE_NB_CHAR-2)
				return false;
		}
		str++;
		nRetourLigne--;
	}

	if(ConsoleBuffer[str] != '\0')
	{
		strcpy(cmdtext, &ConsoleBuffer[str]);
		return true;
	}
	else
		return false;
}

void m_ConsoleFindCommand(char *cmdtext)
{
	hBool			CmdFound=false;
	int				str=0, strcopy=0;
	char			tab[255];
	float			value;
	cmd_function_t	*cmd;
	cvar_t			*var;

	if(!cmdtext)
		return;

	//
	// scan les cheats
	//
	if(!strcmp(&cmdtext[str],"Pas1Dep"))
	{
		gCheatMode_pumpkin = !gCheatMode_pumpkin;
		if(gCheatMode_pumpkin)
			m_ConsPrint("Pumpkin cheatmode ENABLED !!!\n");
		else
			m_ConsPrint("Pumpkin cheatmode DISABLED.\n");
		return;
	}
	else if(!strcmp(&cmdtext[str],"iM-Clan"))
	{
		gCheatMode_monsterbomb = !gCheatMode_monsterbomb;
		if(gCheatMode_monsterbomb)
			m_ConsPrint("MonsterBomb cheatmode ENABLED !!!\n");
		else
			m_ConsPrint("MonsterBomb cheatmode DISABLED.\n");
		return;
	}
	else if(!strcmp(&cmdtext[str],"RoKn"))
	{
		gCheatMode_son = !gCheatMode_son;
		if(gCheatMode_son)
			m_ConsPrint("Msound cheatmode ENABLED !!!\n");
		else
			m_ConsPrint("Msound cheatmode DISABLED.\n");
		return;
	}
	

	//
	// scan les commandes
	//
	cmd = cmd_functions;
	while(cmd)
	{
		if(strncmp(&cmdtext[str],cmd->name,A_strlen(cmd->name))==0)
		{
			//
			// récupère l'argument
			//
			strcopy = str;
			while(cmdtext[strcopy] != ' ')
			{
				strcopy++;
				if(cmdtext[strcopy] == '\n')
					break;
				if(cmdtext[strcopy] == '\0')
					break;
				if(strcopy>CONSOLE_NB_CHAR-2)
					break;
			}
			if(cmdtext[strcopy] == '\n')
				CmdLineArg = NULL;
			else if(cmdtext[strcopy] == '\0')
				CmdLineArg = NULL;
			else
				CmdLineArg = &cmdtext[strcopy+1];

			//
			// éxécute la fonction
			//
			CmdFound = true;
			m_ConsPrint(" >> ");
			cmd->function ();
			break;
		}
		cmd = cmd->next;
	}

	//
	// scan les variables
	//
	if(!CmdFound)
	{
		//for (int i = 0; i < numberCvar; i++)
		//{
			//var = cvarArray[i];

#ifndef H_WINDOWS
		var = cvar_vars;
		while(var)
		{
			if(!strncmp(&cmdtext[str],var->name,A_strlen(var->name)))
			{
				CmdFound = true;
				if(cmdtext[str+A_strlen(var->name)]==' ' &&
					cmdtext[str+A_strlen(var->name)+1]!='\0')
				{
					sscanf(&cmdtext[str+A_strlen(var->name)]," %s", tab); // &tab
					if(tab[0] < '0' || tab[0] > '9')
					{
						strcpy(var->string,tab);
					}
					else
					{
						value = A_atof(tab);
						var->value = value;
					}
				}
				m_ConsPrint(" : Default \"%s\" value %f",var->string,var->value);
				break;
			}
			var = var->next;
		}
#endif
	}

	//
	// network chat
	//
	if(!CmdFound)
	{
		// si on est en reseau alors on envoi le texte a tout le monde
		if( gIsMultiplayer && gMultiConnected)
		{
#ifdef H_WINDOWS // hack
			if(gIsServer)
			{
				//sv_Send(&cmdtext[str], PacketType_ServerSay, true);
			}
			else
			{
				//cl_Send(&cmdtext[str], PacketType_ClientSay, true, false);
			}
#endif
		}
		else
			m_ConsPrint(" : Unknow Command");
	}
}
/*
=========================================================
ENTREE: numéro de la lettre en cours
SORTIE: numéro de la lettre en cours
Le joueur entre une chaine
=========================================================
*/
void m_EnterConsoleString()
{
	int		Key;
	int		KeyPos;
	int		value;
	int		position;
	cmd_function_t	*cmd;
	cvar_t	*var;
	int		nb_occ;
	int		nb2;

	position=0;

	while(ConsoleBufferLine[position]!='\0')
	{
		if(position>CONSOLE_WIDTH-2) //-1
			return;
		position++;
	}

	value = ReadKey();

	switch(value)
	{
		case 0:
		DropKeyPressed=false;
		Key_EnterPressed=false;
		break;

		//	ESC
		case 1:
		break;

		//	RETURN
		case 2:
		if(Key_EnterPressed==false)
		{
			Key_EnterPressed=true;
			//m_ConsoleCmdAddInput();
			
			position=0;
			while(ConsoleBuffer[position] != '\0')
			{
				if(position > CONSOLE_NB_CHAR-1)
					return;
				position++;
			}
			strcpy(ConsoleBufferLine_save, ConsoleBufferLine);
			strcpy(&ConsoleBuffer[position],ConsoleBufferLine);
			memset(ConsoleBufferLine,0,CONSOLE_WIDTH);

			m_ConsoleCmdAddInput();

			sprintf(ConsoleBuffer,"%s\n",ConsoleBuffer);
		}
		return;

		//	BACK
		case 3:
		if(DropKeyPressed==false)
		{
			DropKeyPressed=true;
			if (position > 0) {
				ConsoleBufferLine[position - 1] = '\0';
			}
		}
		break;
			
	//	case 86: // K_VK_OEM_PERIOD
	//		break;

		// recopie l'anciene commande
		case 5:
		strcpy(ConsoleBufferLine, ConsoleBufferLine_save);
		break;

		// autocompletion
		case 6:
		if(key_tab_pressed == 0)
		{
#ifndef H_WINDOWS
			key_tab_pressed = 1;
#endif
		}
		break;
	}

	if(key_tab_pressed == 1)
	{
		key_tab_pressed = 2;

		nb_occ = 0;


		//for (int i = 0; i < numberCvar; i++)
		//{
			//var = cvarArray[i];

		for(var=cvar_vars ; var ; var=var->next)
		{
			if(!strncmp(ConsoleBufferLine, var->name, strlen(ConsoleBufferLine)))
			{
				if(strlen(ConsoleBufferLine) == strlen(var->name))
				{
					// c'est deja complété, passons a la prochaine instruction
					continue;
				}
				else
				{
					nb_occ++;
				}
			}
		}

		if(nb_occ == 0)
		{
			for(cmd=cmd_functions ; cmd ; cmd=cmd->next)
			{
				if(!strncmp(ConsoleBufferLine, cmd->name, strlen(ConsoleBufferLine)))
				{
					if(strlen(ConsoleBufferLine) == strlen(cmd->name))
					{
						// c'est deja complété, passons a la prochaine instruction
						continue;
					}
					else
					{
						nb_occ++;
					}
				}
			}	
		}

		if(nb_occ == 1)
		{
			for(cmd=cmd_functions ; cmd ; cmd=cmd->next)
			{
				if(!strncmp(ConsoleBufferLine, cmd->name, strlen(ConsoleBufferLine)))
				{
					sprintf(ConsoleBufferLine, "%s ", cmd->name);
				}
			}

			//for (int i = 0; i < numberCvar; i++)
			//{
				//var = cvarArray[i];

			for(var=cvar_vars ; var ; var=var->next)
			{
				if(!strncmp(ConsoleBufferLine, var->name, strlen(ConsoleBufferLine)))
				{
					sprintf(ConsoleBufferLine, "%s ", var->name);
				}
			}
		}
		else if(nb_occ > 1)
		{
			m_ConsPrint("Available commands:\n");
			nb2 = nb_occ;
			for(cmd=cmd_functions ; cmd ; cmd=cmd->next)
			{
				if(!strncmp(ConsoleBufferLine, cmd->name, strlen(ConsoleBufferLine)))
				{
					m_ConsPrint("-> %s\n", cmd->name);
					nb2--;
				}
			}
			if(nb2 > 0)
			//for (int i = 0; i < numberCvar; i++)
			//{
				//var = cvarArray[i];
			for(var=cvar_vars ; var ; var=var->next)
			{
				if(!strncmp(ConsoleBufferLine, var->name, strlen(ConsoleBufferLine)))
				{
					m_ConsPrint("-> %s\n", var->name);
				}
			}
		}
	}

	if(value != 6)
	{
		key_tab_pressed = 0;
	}

	KeyPos=0;
	while(KeyBuffer[KeyPos] == -1) // 0
	{
		KeyPos++;
		if(KeyPos>=KEY_BUFFER_MAX)
			return;
	}
	Key = KeyBuffer[KeyPos];
	KeyBuffer[KeyPos] = -1; // 0


	position = 0;
	while(ConsoleBufferLine[position] != '\0')
	{
		position++;
		if(position >= CONSOLE_WIDTH-1)
			break;
	}
	if(position >= CONSOLE_WIDTH-1)
		return;

	ConsoleBufferLine[position] = Key;
}

void m_FreeConsoleCmd()
{
	cmd_function_t *cmd;

	while(cmd_functions)
	{
		cmd = cmd_functions;
		cmd_functions = cmd_functions->next;

	//	if(cmd->name)
	//		s_free(cmd->name);
	//	if(cmd->function)
	//		s_free(cmd->function);

		s_free(cmd);
	}

	//s_free(cmd_functions);
}

void m_RegisterConsoleCmd()
{
	Cmd_AddCommand(VERSION,m_cons_Version);
	Cmd_AddCommand(MAP,m_cons_map);
	Cmd_AddCommand(DRAWFACE,m_cons_DrawFace);
	Cmd_AddCommand(DRAWTRIS,m_cons_DrawTris);
	Cmd_AddCommand(DRAWGUN,m_cons_DrawGun);
	Cmd_AddCommand(DRAWONE,m_cons_DrawOne);
	Cmd_AddCommand(DRAWPORTAL,m_cons_DrawPortal);
	Cmd_AddCommand(DRAWNORMAL,m_cons_DrawNormals);
	Cmd_AddCommand(PLAYERVIEW,m_cons_PlayerView);
	Cmd_AddCommand(DRAWBOUND,m_cons_DrawBound);
	Cmd_AddCommand(DRAWCELL,m_cons_DrawCell);
	Cmd_AddCommand(NOCLIP,m_cons_Noclip);
	Cmd_AddCommand(FOV,m_cons_ChangeFOV);
	Cmd_AddCommand(CREATURE_JUMPSTEP,m_cons_JumpStep);
	Cmd_AddCommand(GROUND_ACCFACTOR,m_cons_groundacc);
	Cmd_AddCommand(GROUND_SLOWFACTOR,m_cons_groundslow);
	Cmd_AddCommand(GRAVITY,m_cons_ChangeGRAVITY);
	Cmd_AddCommand(QUIT,m_Cons_QuitGame);
	Cmd_AddCommand(MAX_TRAVEL_VELOCITY,m_cons_ChangeMaxSpeed);
	Cmd_AddCommand(AIR_ACCFACTOR,m_cons_ChangeAirAcc);
	Cmd_AddCommand(AIR_SLOWFACTOR,m_cons_ChangeAirSlow);
	Cmd_AddCommand(SOUNDSYSTEM,m_cons_ChangeSound);
	Cmd_AddCommand(RESPAWNVAR,m_cons_ChangeRespawn);
	Cmd_AddCommand(GOD,m_cons_ChangeGod);
	Cmd_AddCommand(MONSTER,m_cons_ChangeMonster);
	Cmd_AddCommand(BOMB_SLOWFACTOR,m_cons_ChangeBombslow);
	Cmd_AddCommand(MUSICVOL,m_cons_ChangeMusicVolume);
	Cmd_AddCommand(SOUNDVOL,m_cons_ChangeSoundVolume);
	Cmd_AddCommand(AUTOSWITCH,m_cons_ChangeAutoswitch);
	Cmd_AddCommand(FULLSCREEN,m_cons_ChangeFullscreen);
	Cmd_AddCommand(COLORDEPTH,m_cons_ChangeColordepth);
	Cmd_AddCommand(VIDEOMODE,m_cons_ChangeVideoMode);
	Cmd_AddCommand(BLENDMODESRC,m_cons_ChangeBlendSrc);
	Cmd_AddCommand(BLENDMODEDST,m_cons_ChangeBlendDst);
	Cmd_AddCommand(STATUTBAR,m_cons_ChangeStatutbar);
	Cmd_AddCommand(ANIMSPEED,m_cons_ChangeAnimspeed);
	Cmd_AddCommand(LIGHTMAP,m_cons_ChangeLightmap);
	Cmd_AddCommand(CULLING,m_cons_ChangeCulling);
	Cmd_AddCommand(RENDERENT,m_cons_ChangeRenderEnt);
	Cmd_AddCommand(DIRECTSTART,m_cons_ChangeDirectStart);
	Cmd_AddCommand(EPSILON,m_cons_ChangeEpsilon);
	Cmd_AddCommand(DEVELOPER,m_cons_ChangeDeveloper);
	Cmd_AddCommand(OFFSET,m_cons_ChangeOffset);
	Cmd_AddCommand(LEAF,m_cons_ChangeLeaf);
	Cmd_AddCommand(RECORD,m_cons_Record);
	Cmd_AddCommand(STOPRECORD,m_cons_StopRecord);
	Cmd_AddCommand(PLAY,m_cons_Play);
	Cmd_AddCommand(STOPPLAY,m_cons_StopPlay);
	Cmd_AddCommand(NOSCRIPT,m_cons_Noscript);
	Cmd_AddCommand(RESETGAME,m_cons_ResetGame);
	Cmd_AddCommand(SAVECONFIG,m_cons_SaveConfigToFile);
	Cmd_AddCommand(LOADCONFIG,m_cons_LoadConfigFromFile);
	Cmd_AddCommand(PICMIP,m_cons_ChangePicmip);
	Cmd_AddCommand(GAMMA,m_cons_ChangeGamma);
	Cmd_AddCommand(GHOST,m_cons_ChangeGhost);
	Cmd_AddCommand(TERMINATOR,m_cons_ChangeTerminator);
}

void m_RegisterConsoleVar()
{
	//numberCvar = 0;

	Cvar_RegisterVariable(&fullscreen);
	Cvar_RegisterVariable(&colordepth);
	Cvar_RegisterVariable(&videomode);
	Cvar_RegisterVariable(&visent);
	Cvar_RegisterVariable(&draw_face);
	Cvar_RegisterVariable(&draw_tris);
	Cvar_RegisterVariable(&draw_one);
	Cvar_RegisterVariable(&draw_portal);
	Cvar_RegisterVariable(&invert_mouse);
	Cvar_RegisterVariable(&draw_normal);
	Cvar_RegisterVariable(&draw_bound);
	Cvar_RegisterVariable(&player_view);
	Cvar_RegisterVariable(&draw_cell);
	Cvar_RegisterVariable(&draw_fps);
	Cvar_RegisterVariable(&camera_vel);
	Cvar_RegisterVariable(&player_vel);
	Cvar_RegisterVariable(&player_rvel);
	Cvar_RegisterVariable(&player_look_ud);
	Cvar_RegisterVariable(&noclip_wall);
	Cvar_RegisterVariable(&fov);
	Cvar_RegisterVariable(&jumpstep);
	Cvar_RegisterVariable(&groundacc);
	Cvar_RegisterVariable(&groundslow);
	Cvar_RegisterVariable(&gravity);
	Cvar_RegisterVariable(&mapfile);
	Cvar_RegisterVariable(&drawgun);
	Cvar_RegisterVariable(&maxspeed);
	Cvar_RegisterVariable(&airacc);
	Cvar_RegisterVariable(&airslow);
	Cvar_RegisterVariable(&soundsys);
	Cvar_RegisterVariable(&respawn);
	Cvar_RegisterVariable(&god);
	Cvar_RegisterVariable(&monster);
	Cvar_RegisterVariable(&bombslow);
	Cvar_RegisterVariable(&musicvol);
	Cvar_RegisterVariable(&soundvol);
	Cvar_RegisterVariable(&autoswitch);
	Cvar_RegisterVariable(&bms);
	Cvar_RegisterVariable(&bmd);
	Cvar_RegisterVariable(&statutbar);
	Cvar_RegisterVariable(&animspeed);
	Cvar_RegisterVariable(&lightmap);
	Cvar_RegisterVariable(&culling);
	Cvar_RegisterVariable(&render_ent);
	Cvar_RegisterVariable(&directstart);
	Cvar_RegisterVariable(&epsilon);
	Cvar_RegisterVariable(&developer);
	Cvar_RegisterVariable(&offset);
	Cvar_RegisterVariable(&leaf);
	Cvar_RegisterVariable(&record);
	Cvar_RegisterVariable(&stoprecord);
	Cvar_RegisterVariable(&play);
	Cvar_RegisterVariable(&stopplay);
	Cvar_RegisterVariable(&noscript);
	Cvar_RegisterVariable(&gamekey_up);
	Cvar_RegisterVariable(&gamekey_down);
	Cvar_RegisterVariable(&gamekey_left);
	Cvar_RegisterVariable(&gamekey_right);
	Cvar_RegisterVariable(&gamekey_jump);
	Cvar_RegisterVariable(&gamekey_attack);
	Cvar_RegisterVariable(&gamekey_axe);
	Cvar_RegisterVariable(&gamekey_arbalet);
	Cvar_RegisterVariable(&gamekey_bombgun);
	Cvar_RegisterVariable(&gamekey_tromblon);
	Cvar_RegisterVariable(&gamekey_crouch);
	Cvar_RegisterVariable(&gamekey_action);
	Cvar_RegisterVariable(&gamekey_run);
	Cvar_RegisterVariable(&map_letter);
	Cvar_RegisterVariable(&kill_depla);
	Cvar_RegisterVariable(&kill_morbak);
	Cvar_RegisterVariable(&kill_zeubat);	
	Cvar_RegisterVariable(&kill_subzerat);
	Cvar_RegisterVariable(&kill_zemummy);
	Cvar_RegisterVariable(&map_complete);
	Cvar_RegisterVariable(&music_track);
	Cvar_RegisterVariable(&pres_track);
	Cvar_RegisterVariable(&resetgame);
	Cvar_RegisterVariable(&picmip);
	Cvar_RegisterVariable(&difficulty);
	Cvar_RegisterVariable(&gameslot);
	Cvar_RegisterVariable(&gamma_);
	Cvar_RegisterVariable(&tflame);
	Cvar_RegisterVariable(&ghost);
	Cvar_RegisterVariable(&terminator);
	Cvar_RegisterVariable(&net_port);
	Cvar_RegisterVariable(&net_ip);
	Cvar_RegisterVariable(&net_mapid);
	Cvar_RegisterVariable(&net_clname);
	Cvar_RegisterVariable(&net_svname);
	Cvar_RegisterVariable(&net_password);
	Cvar_RegisterVariable(&net_gametype);
	Cvar_RegisterVariable(&net_dedicated);
	Cvar_RegisterVariable(&notextures);	
	Cvar_RegisterVariable(&bestscore1);	
	Cvar_RegisterVariable(&bestscore2);	
	Cvar_RegisterVariable(&bestscore3);	
	Cvar_RegisterVariable(&bestscore4);	
	Cvar_RegisterVariable(&bestscore5);	
	Cvar_RegisterVariable(&bestscore6);	
	Cvar_RegisterVariable(&bestscore7);	
	Cvar_RegisterVariable(&bestscore8);	
	Cvar_RegisterVariable(&bestscore9);
	Cvar_RegisterVariable(&fogmode);
	Cvar_RegisterVariable(&fogdensity);	
}

void m_cons_ChangeGhost()
{
	if(ghost.value)
		m_ConsPrint("ghost OFF");
	else
		m_ConsPrint("ghost ON");

	ghost.value=(float)!ghost.value;
}

void m_cons_ChangeTerminator()
{
	if(terminator.value)
		m_ConsPrint("terminator OFF");
	else
		m_ConsPrint("terminator ON");

	terminator.value=(float)!terminator.value;
}

void m_cons_ChangeGamma()
{
	float	newval;

	if(CmdLineArg==NULL)
	{
		m_ConsPrint("default: %s value: %f",gamma_.string, gamma_.value);
		return;
	}
	sscanf(CmdLineArg,"%f",&newval);

	if(newval < 0.3f)
		newval = 0.3f;
	if(newval > 1.7f)
		newval = 1.7f;
	gamma_.value = newval;

	sys_setGamma(gamma_.value);
}

void m_cons_ChangePicmip()
{
	float	newval;
	int		newval_int;

	if(CmdLineArg==NULL)
	{
		m_ConsPrint("default: %s value: %d",picmip.string,(int)picmip.value);
		return;
	}
	sscanf(CmdLineArg,"%f",&newval);
	newval_int = (int)newval;
	if(newval_int < 1)
		newval_int = 1;
	if(newval_int > 6)
		newval_int = 6;
	picmip.value = (float)newval_int;

	sys_RestartVID();
}

void m_cons_ResetGame()
{
	m_ConsPrint("*** game has been reseted ***\n");

	map_letter.value = 0;
	kill_depla.value = 0;
	kill_morbak.value = 0;
	kill_zeubat.value = 0;
	kill_subzerat.value = 0;
	map_complete.value = 0;
}

void m_cons_Noscript()
{
	if(noscript.value)
		m_ConsPrint("script ON");
	else
		m_ConsPrint("script OFF");

	noscript.value=(float)!noscript.value;
}

void m_cons_Record()
{
	if(play.value == 1)
	{
		m_ConsPrint("can't record demo, a demo is being played!\n");
		return;
	}
	demo_RecordInit();
	record.value = 1;
	m_ConsPrint(" recording demo1.dem\n");
}

void m_cons_StopRecord()
{
	demo_Clean();
	record.value = 0;
	m_ConsPrint(" stop recording demo\n");
}

void m_cons_Play()
{
	if(record.value == 1)
	{
		m_ConsPrint("can't play demo, a demo is being recorded!\n");
		return;
	}
	demo_PlayInit();
	play.value = 1;
	m_ConsPrint(" playing demo1.dem\n");
}

void m_cons_StopPlay()
{
	demo_Clean();
	play.value = 0;
	m_ConsPrint(" stop playing demo\n");
}

void m_Cons_QuitGame()
{
	char	car;
	int		pos;

	memset(ErrorMessage,0,255);
	if(CmdLineArg)
	{
		pos=0;
		do
		{
			car=CmdLineArg[pos];
			if(car=='\0' || car=='\n' || car==' ')
				break;
			ErrorMessage[pos]=car;
			pos++;
		}
		while(pos<255);
	}
	
	ProgramState = PS_QUIT;
}

void m_cons_ChangeLeaf()
{
	float	newval;

	if(CmdLineArg==NULL)
	{
		m_ConsPrint(" [default: %s value: %d]",leaf.string,(int)leaf.value);
		return;
	}
	sscanf(CmdLineArg,"%f",&newval);
	leaf.value = newval;
	m_ConsPrint(" [default: %s New value: %d]",leaf.string,(int)leaf.value);
}

void m_cons_ChangeOffset()
{
	float	newval;

	if(CmdLineArg==NULL)
	{
		m_ConsPrint("default: %s value: %f",offset.string,offset.value);
		return;
	}
	sscanf(CmdLineArg,"%f",&newval);
	offset.value = (float)newval;
}

void m_cons_ChangeEpsilon()
{
	float	newepsilon;

	if(CmdLineArg==NULL)
	{
		m_ConsPrint("default: %s value: %f",epsilon.string,epsilon.value);
		return;
	}
	sscanf(CmdLineArg,"%f",&newepsilon);
	epsilon.value = (float)newepsilon;
}

void m_cons_ChangeDirectStart()
{
	if(directstart.value)
		m_ConsPrint("directstart OFF");
	else
		m_ConsPrint("directstart ON");

	directstart.value=(float)!directstart.value;
}

void m_cons_ChangeRenderEnt()
{
	if(render_ent.value)
		m_ConsPrint("render_ent OFF");
	else
		m_ConsPrint("render_ent ON");

	render_ent.value=(float)!render_ent.value;
}

void m_cons_ChangeDeveloper()
{
	if(developer.value)
		m_ConsPrint("developer mode OFF");
	else
		m_ConsPrint("developer mode ON");

	developer.value=(float)!developer.value;
}

void m_cons_ChangeCulling()
{
	if(culling.value)
		m_ConsPrint("culling OFF");
	else
		m_ConsPrint("culling ON");

	culling.value=(float)!culling.value;
}

void m_cons_ChangeLightmap()
{
	if(lightmap.value)
		m_ConsPrint("lightmap OFF");
	else
		m_ConsPrint("lightmap ON");

	lightmap.value=(float)!lightmap.value;
	if(lightmap.value)
	{
		sys_RestartVID();
	}
}

void m_cons_ChangeAnimspeed()
{
	float	newval;

	if(CmdLineArg==NULL)
	{
		m_ConsPrint("default: %s value: %f",animspeed.string,animspeed.value);
		return;
	}
	sscanf(CmdLineArg,"%f",&newval);
	animspeed.value = newval;
}

void m_cons_ChangeStatutbar()
{
	if(statutbar.value)
		m_ConsPrint("StatutBar OFF");
	else
		m_ConsPrint("StatutBar ON");

	statutbar.value=(float)!statutbar.value;
}

void m_cons_ChangeBlendSrc()
{
	float	newval;

	if(CmdLineArg==NULL)
	{
		m_ConsPrint("default: %s value: %d",bms.string,(int)bms.value);
		return;
	}
	sscanf(CmdLineArg,"%f",&newval);
	bms.value = newval;
	glBlendFunc(gl_GetBlendMode((int)bms.value), gl_GetBlendMode((int)bmd.value));
}

void m_cons_ChangeBlendDst()
{
	float	newval;

	if(CmdLineArg==NULL)
	{
		m_ConsPrint("default: %s value: %d",bmd.string,(int)bmd.value);
		return;
	}
	sscanf(CmdLineArg,"%f",&newval);
	bmd.value = newval;
	glBlendFunc(gl_GetBlendMode((int)bms.value),gl_GetBlendMode((int)bmd.value));
}

void m_cons_ChangeVideoMode()
{
	float	newval;
	int		newval_int;

	if(CmdLineArg==NULL)
	{
		m_ConsPrint("default: %s value: %d",videomode.string,(int)videomode.value);
		return;
	}
	sscanf(CmdLineArg,"%f",&newval);
	newval_int = (int)newval;
	if(	newval_int != 0 &&
		newval_int != 1 &&
		newval_int != 2 &&
		newval_int != 3 &&
		newval_int != 4 &&
		newval_int != 5 &&
		newval_int != 6)
		newval_int = 1;
	videomode.value = (float)newval_int;

	sys_RestartVID();
}

void m_cons_ChangeColordepth()
{
	float	newval;
	int		newval_int;

	if(CmdLineArg==NULL)
	{
		m_ConsPrint("default: %s value: %d",colordepth.string,(int)colordepth.value);
		return;
	}
	sscanf(CmdLineArg,"%f",&newval);
	newval_int = (int)newval;
	if(newval_int != 16 && newval_int != 32)
		newval_int = 16;
	colordepth.value = (float)newval_int;

	sys_RestartVID();
}

void m_cons_ChangeFullscreen()
{
	if(fullscreen.value)
		m_ConsPrint("Fullscreen mode OFF");
	else
		m_ConsPrint("Fullscreen mode ON");
	
	//sys_RestartVID();

	sys_killVID();	
	fullscreen.value=(float)!fullscreen.value;
	if(!sys_setVID())
	{
		ProgramState = PS_QUIT;
		return;
	}

	if(gIsServer && net_dedicated.value)
		return;

	// remet la musique en route
	if(gNbTrack >= 8  && gNbTrack <= 11)
		game_LaunchPresMusic();
	else
		ds_LoadStreamWAVE(game_GetMusicString((int)music_track.value));

	//et recharge les textures
	im_ReloadTextures();
}

void m_cons_ChangeAutoswitch()
{
	if(autoswitch.value)
		m_ConsPrint("Autoswitch mode OFF");
	else
		m_ConsPrint("Autoswitch mode ON");

	autoswitch.value=(float)!autoswitch.value;
}

void m_cons_ChangeSoundVolume()
{
	float	newval;

	if(CmdLineArg==NULL)
	{
		m_ConsPrint("default: %s value: %d",soundvol.string,(int)soundvol.value);
		return;
	}
	sscanf(CmdLineArg,"%f",&newval);
	soundvol.value = (float)((int)newval);
	ds_AdjustSoundVolumes();
}

void m_cons_ChangeMusicVolume()
{
	float	newval;

	if(CmdLineArg==NULL)
	{
		m_ConsPrint("default: %s value: %d",musicvol.string,(int)musicvol.value);
		return;
	}
	sscanf(CmdLineArg,"%f",&newval);
	musicvol.value = (float)((int)newval);
	ds_AdjustMusicVolumes();
}

void m_cons_ChangeBombslow()
{
	float	newval;

	if(CmdLineArg==NULL)
	{
		m_ConsPrint("default: %s value: %f",bombslow.string,bombslow.value);
		return;
	}
	sscanf(CmdLineArg,"%f",&newval);
	bombslow.value = newval;
}

void m_cons_ChangeMonster()
{
	if(monster.value)
		m_ConsPrint("Monster OFF");
	else
		m_ConsPrint("Monster ON");

	monster.value=(float)!monster.value;
}

void m_cons_ChangeGod()
{
	if(god.value)
		m_ConsPrint("God mode OFF");
	else
		m_ConsPrint("God mode ON");

	god.value=(float)!god.value;	
}

void m_cons_ChangeRespawn()
{
	if(respawn.value)
		m_ConsPrint("Respawn OFF");
	else
		m_ConsPrint("Respawn ON");

	respawn.value=(float)!respawn.value;	
}

void m_cons_ChangeSound()
{
	/*
	if(soundsys.value)
	{
		music_stop();
		music_uninit();
		soundsys.value = 0;
		m_ConsPrint("Sound OFF");
	}
	else
	{
		soundsys.value = 1.0f;
		music_init();
		music_load();
		m_ConsPrint("Sound ON");
	}
	*/
}

void m_cons_ChangeAirAcc()
{
	float	newval;

	if(CmdLineArg==NULL)
	{
		m_ConsPrint("default: %s value: %f",airacc.string,airacc.value);
		return;
	}
	sscanf(CmdLineArg,"%f",&newval);
	airacc.value = newval;
}

void m_cons_ChangeAirSlow()
{
	float	newval;

	if(CmdLineArg==NULL)
	{
		m_ConsPrint("default: %s value: %f",airslow.string,airslow.value);
		return;
	}
	sscanf(CmdLineArg,"%f",&newval);
	airslow.value = newval;
}

void m_cons_ChangeMaxSpeed()
{
	float	newval;

	if(CmdLineArg==NULL)
	{
		m_ConsPrint("default: %s value: %f",maxspeed.string,maxspeed.value);
		return;
	}
	sscanf(CmdLineArg,"%f",&newval);
	maxspeed.value = newval;
}

void m_cons_groundacc()
{
	float	newval;

	if(CmdLineArg==NULL)
	{
		m_ConsPrint("default: %s value: %f",groundacc.string,groundacc.value);
		return;
	}
	sscanf(CmdLineArg,"%f",&newval);
	groundacc.value = newval;
}

void m_cons_groundslow()
{
	float	newval;

	if(CmdLineArg==NULL)
	{
		m_ConsPrint("default: %s value: %f",groundslow.string,groundslow.value);
		return;
	}
	sscanf(CmdLineArg,"%f",&newval);
	groundslow.value = newval;
}

void m_cons_JumpStep()
{
	float	newJumpStep;

	if(CmdLineArg==NULL)
	{
		m_ConsPrint("default: %s value: %f",jumpstep.string,jumpstep.value);
		return;
	}
	sscanf(CmdLineArg,"%f",&newJumpStep);
	jumpstep.value = newJumpStep;
}

void m_cons_ChangeFOV()
{
	float	newfov;

	if(CmdLineArg==NULL)
	{
		m_ConsPrint("default: %s value: %d",fov.string,(int)fov.value);
		return;
	}
	sscanf(CmdLineArg,"%f",&newfov);
	fov.value = (float)(int)newfov;
	gl_ChangeFOV(fov.value);
}

void m_cons_ChangeGRAVITY()
{
	float	newgravity;

	if(CmdLineArg==NULL)
	{
		m_ConsPrint("default: %s value: %f",gravity.string,(float)gravity.value);
		return;
	}
	sscanf(CmdLineArg,"%f",&newgravity);
	gravity.value = (float)newgravity;
}

void m_cons_Version()
{
	m_ConsPrint(GAMEVERSION);
}

void m_cons_Noclip()
{
	if(noclip_wall.value)
		m_ConsPrint("NoClip OFF");
	else
		m_ConsPrint("NoClip ON");

	noclip_wall.value=(float)!noclip_wall.value;	
}

void m_cons_DrawFace()
{
	if(draw_face.value)
		m_ConsPrint("DrawFace OFF");
	else
		m_ConsPrint("DrawFace ON");

	draw_face.value=(float)!draw_face.value;	
}

void m_cons_DrawTris()
{
	if(draw_tris.value)
		m_ConsPrint("DrawTris OFF");
	else
		m_ConsPrint("DrawTris ON");

	draw_tris.value=(float)!draw_tris.value;	
}

void m_cons_DrawGun()
{
	if(drawgun.value)
		m_ConsPrint("drawgun OFF");
	else
		m_ConsPrint("drawgun ON");

	drawgun.value=(float)!drawgun.value;
}

void m_cons_DrawOne()
{
	if(draw_one.value)
		m_ConsPrint("DrawOne OFF");
	else
		m_ConsPrint("DrawOne ON");

	draw_one.value=(float)!draw_one.value;	
}

void m_cons_DrawPortal()
{
	if(draw_portal.value)
		m_ConsPrint("DrawPortal OFF");
	else
		m_ConsPrint("DrawPortal ON");

	draw_portal.value=(float)!draw_portal.value;	
}

void m_cons_InvertMouse()
{
	m_ConsPrint("InvertMouse");
	if(invert_mouse.value == 0)
		invert_mouse.value = 1;
	else
		invert_mouse.value = 0;
}

void m_cons_DrawNormals()
{
	if(draw_normal.value)
		m_ConsPrint("DrawNormals OFF");
	else
		m_ConsPrint("DrawNormals ON");

	draw_normal.value=(float)!draw_normal.value;	
}

void m_cons_PlayerView()
{
	if(player_view.value)
		m_ConsPrint("PlayerView OFF");
	else
		m_ConsPrint("PlayerView ON");

	player_view.value=(float)!player_view.value;
}

void m_cons_DrawBound()
{
	if(draw_bound.value)
		m_ConsPrint("DrawBound OFF");
	else
		m_ConsPrint("DrawBound ON");

	draw_bound.value=(float)!draw_bound.value;	
}

void m_cons_DrawCell()
{
	if(draw_cell.value)
		m_ConsPrint("DrawCell OFF");
	else
		m_ConsPrint("DrawCell ON");

	draw_cell.value=(float)!draw_cell.value;	
}

void m_cons_map()
{
	char	path[255];
	char	savepath[255];
	char	fullpath[255];
	char	car;
	int		pos;

	if(CmdLineArg==NULL)
	{
		m_ConsPrint("Function need a map file name");
		return;
	}

	memset(path,0,255);
	memset(fullpath,0,255);
	pos=0;
	do
	{
		car=CmdLineArg[pos];
		if(car=='\0')
			break;
		if(car=='\n')
			break;
		if(car==' ')
			break;
		path[pos]=car;
		pos++;
	}
	while(pos<255);
	path[pos+0]='.';
	path[pos+1]='\0';

	fs_RenameFileExtension(path,"map");
	strcpy(savepath,mapfile.string);
	strcpy(mapfile.string,path);

	UnloadMap();
	LoadMap();
}

void m_cons_SaveConfigToFile()
{
	char	path[255];
	char	fullpath[255];
	char	car;
	int		pos;

	if(CmdLineArg==NULL)
	{
		m_ConsPrint("Function need a file name argument");
		return;
	}

	memset(path,0,255);
	memset(fullpath,0,255);
	pos=0;
	do
	{
		car=CmdLineArg[pos];
		if(car=='\0')
			break;
		if(car=='\n')
			break;
		if(car==' ')
			break;
		path[pos]=car;
		pos++;
	}
	while(pos<255);
	path[pos] = 0;
    fs_RenameFileExtension(path, "cfg");
	sprintf(fullpath,"%s%s",SYSDIR,path);
	cons_SaveConfig(fullpath);
	m_ConsPrint("\n%s config file has been saved.",path);
}

void m_cons_LoadConfigFromFile()
{
	char	path[255];
	char	fullpath[255];
	char	car;
	int		pos;
	float	old_picmip;
	float	old_screendepth;
	float	old_fullscreen;
	float	new_fullscreen;
	float	old_res;
	float	old_gamma;
	hBool	IsDedicated;
	float	old_map_letter;
	float	old_kill_depla;
	float	old_kill_morbak;
	float	old_kill_zeubat;
	float	old_kill_subzerat;
	float	old_kill_zemummy;
	float	old_map_complete;
	float	old_difficulty;


	if(CmdLineArg == NULL)
	{
		m_ConsPrint("Function need a file name argument");
		return;
	}

	if(gIsServer && net_dedicated.value)
	{
		IsDedicated = true;
	}
	else
	{
		IsDedicated = false;
	}

	memset(path,0,255);
	memset(fullpath,0,255);
	pos=0;
	do
	{
		car=CmdLineArg[pos];
		if(car=='\0')
			break;
		if(car=='\n')
			break;
		if(car==' ')
			break;
		path[pos]=car;
		pos++;
	}
	while(pos<255);
	path[pos] = 0;

	old_picmip = picmip.value;
	old_screendepth = colordepth.value;
	old_res = videomode.value;
	old_fullscreen = fullscreen.value;
	old_gamma = gamma_.value;

	old_map_letter = map_letter.value;
	old_kill_depla = kill_depla.value;
	old_kill_morbak = kill_morbak.value;
	old_kill_zeubat = kill_zeubat.value;
	old_kill_subzerat = kill_subzerat.value;
	old_kill_zemummy = kill_zemummy.value;
	old_map_complete = map_complete.value;
	old_difficulty = difficulty.value;

	fs_RenameFileExtension(path,"cfg");
	sprintf(fullpath,"%s%s",SYSDIR,path);

	if(!cons_LoadConfig((char*)fullpath))
	{
		m_ConsPrint("File not found");
		return;
	}

	map_letter.value = old_map_letter;
	kill_depla.value = old_kill_depla;
	kill_morbak.value = old_kill_morbak;
	kill_zeubat.value = old_kill_zeubat;
	kill_subzerat.value = old_kill_subzerat;
	kill_zemummy.value = old_kill_zemummy;
	map_complete.value = old_map_complete;
	difficulty.value = old_difficulty;

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

	if(IsDedicated)
	{
		gIsMultiplayer = true;
		gIsServer = true;
		net_dedicated.value = true;
		m_ConsPrint("New video settings not applyied in dedicated server\n");
	}
	else
	{
		if(	old_picmip != picmip.value ||
			old_screendepth != colordepth.value ||
			old_fullscreen != fullscreen.value ||
			old_res != videomode.value)
		{
   			//sys_RestartVID();
			new_fullscreen = fullscreen.value;
			fullscreen.value = old_fullscreen;
			sys_killVID();	
			fullscreen.value = new_fullscreen;
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

			// recharge les textures
			im_ReloadTextures();
		}

		if(old_gamma != gamma_.value)
		{
			sys_setGamma(gamma_.value);
		}

		ds_AdjustMusicVolumes();
		ds_AdjustSoundVolumes();
	}

	m_ConsPrint("\n%s config file has been loaded.",path);
}

hBool cons_SaveConfig(char *path)
{
	FILE	*file;
	cvar_t	*var;
	char	*p;
	
#ifdef H_MAC
	// SANDBOXED --------------------------------
	char * pDoc = KMiscTools::currentDocHomePath;
	char fullpath[255];
	sprintf(fullpath, "%s/%s", pDoc, CONFIG);
	m_ConsPrint("#### cons_SaveConfig file: %s\n", fullpath);
	file = fopen(fullpath, "wb");
	//p = KMiscTools::makeFilePath(path);
	//m_ConsPrint("#### cons_SaveConfig file: %s\n", p);
	//file = fopen(p, "w");
#else
	file = fopen(path,"wb");
#endif

	if(file==NULL)
	{
		//m_ConsPrint("cons_SaveConfig file error: %s %s\n", path, KMiscTools::makeFilePath(path) );
		return false;
	}
	
	m_ConsPrint("Saving config file: %s\n", path);

	gamekey_up.value = (float)gGameIdKeys[0];
	gamekey_down.value = (float)gGameIdKeys[1];
	gamekey_left.value = (float)gGameIdKeys[2];
	gamekey_right.value = (float)gGameIdKeys[3];
	gamekey_jump.value = (float)gGameIdKeys[4];
	gamekey_crouch.value = (float)gGameIdKeys[5];
	gamekey_attack.value = (float)gGameIdKeys[6];
	gamekey_axe.value = (float)gGameIdKeys[7];
	gamekey_arbalet.value = (float)gGameIdKeys[8];
	gamekey_bombgun.value = (float)gGameIdKeys[9];
	gamekey_tromblon.value = (float)gGameIdKeys[10];
	gamekey_action.value = (float)gGameIdKeys[11];
	gamekey_run.value = (float)gGameIdKeys[12];

	
	
#ifdef H_WINDOWS

	fprintf(file, "// generated by Halloween. Modify with care\n");

	fprintf(file, "%s %s %f\r\n", maxspeed.name, maxspeed.string, maxspeed.value);
	fprintf(file, "%s %s %f\r\n", groundacc.name, groundacc.string, groundacc.value);
	fprintf(file, "%s %s %f\r\n", groundslow.name, groundslow.string, groundslow.value);
	fprintf(file, "%s %s %f\r\n", airacc.name, airacc.string, airacc.value);
	fprintf(file, "%s %s %f\r\n", airslow.name, airslow.string, airslow.value);
	fprintf(file, "%s %s %f\r\n", visent.name, visent.string, visent.value);
	fprintf(file, "%s %s %f\r\n", epsilon.name, epsilon.string, epsilon.value);
	fprintf(file, "%s %s %f\r\n", draw_fps.name, draw_fps.string, draw_fps.value);
	fprintf(file, "%s %s %f\r\n", player_vel.name, player_vel.string, player_vel.value);
	fprintf(file, "%s %s %f\r\n", camera_vel.name, camera_vel.string, camera_vel.value);
	fprintf(file, "%s %s %f\r\n", player_rvel.name, player_rvel.string, player_rvel.value);
	fprintf(file, "%s %s %f\r\n", player_look_ud.name, player_look_ud.string, player_look_ud.value);
	fprintf(file, "%s %s %f\r\n", draw_face.name, draw_face.string, draw_face.value);
	fprintf(file, "%s %s %f\r\n", draw_tris.name, draw_tris.string, draw_tris.value);
	fprintf(file, "%s %s %f\r\n", draw_one.name, draw_one.string, draw_one.value);
	fprintf(file, "%s %s %f\r\n", draw_portal.name, draw_portal.string, draw_portal.value);
	fprintf(file, "%s %s %f\r\n", invert_mouse.name, invert_mouse.string, invert_mouse.value);
	fprintf(file, "%s %s %f\r\n", draw_normal.name, draw_normal.string, draw_normal.value);
	fprintf(file, "%s %s %f\r\n", player_view.name, player_view.string, player_view.value);
	fprintf(file, "%s %s %f\r\n", draw_bound.name, draw_bound.string, draw_bound.value);
	fprintf(file, "%s %s %f\r\n", draw_cell.name, draw_cell.string, draw_cell.value);
	fprintf(file, "%s %s %f\r\n", noclip_wall.name, noclip_wall.string, noclip_wall.value);
	fprintf(file, "%s %s %f\r\n", fov.name, fov.string, fov.value);
	fprintf(file, "%s %s %f\r\n", mapfile.name, mapfile.string, mapfile.value);
	fprintf(file, "%s %s %f\r\n", gravity.name, gravity.string, gravity.value);
	fprintf(file, "%s %s %f\r\n", drawgun.name, drawgun.string, drawgun.value);
	fprintf(file, "%s %s %f\r\n", jumpstep.name, jumpstep.string, jumpstep.value);
	fprintf(file, "%s %s %f\r\n", soundsys.name, soundsys.string, soundsys.value);
	fprintf(file, "%s %s %f\r\n", respawn.name, respawn.string, respawn.value);
	fprintf(file, "%s %s %f\r\n", god.name, god.string, god.value);
	fprintf(file, "%s %s %f\r\n", monster.name, monster.string, monster.value);
	fprintf(file, "%s %s %f\r\n", bombslow.name, bombslow.string, bombslow.value);
	fprintf(file, "%s %s %f\r\n", musicvol.name, musicvol.string, musicvol.value);
	fprintf(file, "%s %s %f\r\n", soundvol.name, soundvol.string, soundvol.value);
	fprintf(file, "%s %s %f\r\n", autoswitch.name, autoswitch.string, autoswitch.value);
	fprintf(file, "%s %s %f\r\n", fullscreen.name, fullscreen.string, fullscreen.value);
	fprintf(file, "%s %s %f\r\n", colordepth.name, colordepth.string, colordepth.value);
	fprintf(file, "%s %s %f\r\n", videomode.name, videomode.string, videomode.value);
	fprintf(file, "%s %s %f\r\n", bms.name, bms.string, bms.value);
	fprintf(file, "%s %s %f\r\n", bmd.name, bmd.string, bmd.value);
	fprintf(file, "%s %s %f\r\n", statutbar.name, statutbar.string, statutbar.value);
	fprintf(file, "%s %s %f\r\n", animspeed.name, animspeed.string, animspeed.value);
	fprintf(file, "%s %s %f\r\n", lightmap.name, lightmap.string, lightmap.value);
	fprintf(file, "%s %s %f\r\n", culling.name, culling.string, culling.value);
	fprintf(file, "%s %s %f\r\n", render_ent.name, render_ent.string, render_ent.value);
	fprintf(file, "%s %s %f\r\n", directstart.name, directstart.string, directstart.value);
	fprintf(file, "%s %s %f\r\n", developer.name, developer.string, developer.value);
	fprintf(file, "%s %s %f\r\n", offset.name, offset.string, offset.value);
	fprintf(file, "%s %s %f\r\n", leaf.name, leaf.string, leaf.value);
	fprintf(file, "%s %s %f\r\n", record.name, record.string, record.value);
	fprintf(file, "%s %s %f\r\n", stoprecord.name, stoprecord.string, stoprecord.value);
	fprintf(file, "%s %s %f\r\n", play.name, play.string, play.value);
	fprintf(file, "%s %s %f\r\n", stopplay.name, stopplay.string, stopplay.value);
	fprintf(file, "%s %s %f\r\n", noscript.name, noscript.string, noscript.value);

	fprintf(file, "%s %s %f\r\n", gamekey_up.name, gamekey_up.string, gamekey_up.value);
	fprintf(file, "%s %s %f\r\n", gamekey_down.name, gamekey_down.string, gamekey_down.value);
	fprintf(file, "%s %s %f\r\n", gamekey_left.name, gamekey_left.string, gamekey_left.value);
	fprintf(file, "%s %s %f\r\n", gamekey_right.name, gamekey_right.string, gamekey_right.value);
	fprintf(file, "%s %s %f\r\n", gamekey_jump.name, gamekey_jump.string, gamekey_jump.value);
	fprintf(file, "%s %s %f\r\n", gamekey_attack.name, gamekey_attack.string, gamekey_attack.value);
	fprintf(file, "%s %s %f\r\n", gamekey_axe.name, gamekey_axe.string, gamekey_axe.value);
	fprintf(file, "%s %s %f\r\n", gamekey_arbalet.name, gamekey_arbalet.string, gamekey_arbalet.value);
	fprintf(file, "%s %s %f\r\n", gamekey_bombgun.name, gamekey_bombgun.string, gamekey_bombgun.value);
	fprintf(file, "%s %s %f\r\n", gamekey_tromblon.name, gamekey_tromblon.string, gamekey_tromblon.value);
	fprintf(file, "%s %s %f\r\n", gamekey_crouch.name, gamekey_crouch.string, gamekey_crouch.value);
	fprintf(file, "%s %s %f\r\n", gamekey_action.name, gamekey_action.string, gamekey_action.value);
	fprintf(file, "%s %s %f\r\n", gamekey_run.name, gamekey_run.string, gamekey_run.value);
	fprintf(file, "%s %s %f\r\n", noscript.name, noscript.string, noscript.value);
	fprintf(file, "%s %s %f\r\n", noscript.name, noscript.string, noscript.value);
	fprintf(file, "%s %s %f\r\n", noscript.name, noscript.string, noscript.value);
	fprintf(file, "%s %s %f\r\n", noscript.name, noscript.string, noscript.value);
	fprintf(file, "%s %s %f\r\n", noscript.name, noscript.string, noscript.value);
	fprintf(file, "%s %s %f\r\n", noscript.name, noscript.string, noscript.value);
	fprintf(file, "%s %s %f\r\n", noscript.name, noscript.string, noscript.value);
	fprintf(file, "%s %s %f\r\n", noscript.name, noscript.string, noscript.value);
	fprintf(file, "%s %s %f\r\n", noscript.name, noscript.string, noscript.value);
	fprintf(file, "%s %s %f\r\n", noscript.name, noscript.string, noscript.value);



	/*
	cvar_t		map_letter = { "mapletter","0" };
	cvar_t		kill_depla = { "killdepla","0" };
	cvar_t		kill_morbak = { "killmorba","0" };
	cvar_t		kill_zeubat = { "killzeubat","0" };
	cvar_t		kill_subzerat = { "killsubzerat","0" };
	cvar_t		kill_zemummy = { "killzemummy","0" };
	cvar_t		map_complete = { "mapcomplete","0" };
	cvar_t		music_track = { "musictrack","0" };
	cvar_t		pres_track = { "prestrack","0" };
	cvar_t		resetgame = { RESETGAME,"0" };
	cvar_t		picmip = { PICMIP,"1" };
	cvar_t		difficulty = { "difficulty","1" };
	cvar_t		gameslot = { "gameslot","1" };
	cvar_t		gamma_ = { GAMMA,"1" };
	cvar_t		tflame = { TFLAME,"0.035" };
	cvar_t		ghost = { GHOST,"0" };
	cvar_t		terminator = { TERMINATOR,"0" };
	cvar_t		net_port = { C_NETPORT, "24650" };
	cvar_t		net_ip = { C_NETIP, "localhost" };
	cvar_t		net_mapid = { C_NETMAPID, "1" };
	cvar_t		net_clname = { C_NETCLIENTNAME, "UnnamedPlayer" };
	cvar_t		net_svname = { C_NETSERVERNAME, "HalloweenServer" };
	cvar_t		net_password = { C_NETPASSWORD, "SALEDEP" };
	cvar_t		net_gametype = { C_NETGAMETYPE, "0" };
	cvar_t		net_dedicated = { C_NETDEDICATED, "0" };
	cvar_t		notextures = { C_NOTEXTURES,"0" };
	cvar_t		bestscore1 = { C_BESTSCORE1,"0" };
	cvar_t		bestscore2 = { C_BESTSCORE2,"0" };
	cvar_t		bestscore3 = { C_BESTSCORE3,"0" };
	cvar_t		bestscore4 = { C_BESTSCORE4,"0" };
	cvar_t		bestscore5 = { C_BESTSCORE5,"0" };
	cvar_t		bestscore6 = { C_BESTSCORE6,"0" };
	cvar_t		bestscore7 = { C_BESTSCORE7,"0" };
	cvar_t		bestscore8 = { C_BESTSCORE8,"0" };
	cvar_t		bestscore9 = { C_BESTSCORE9,"0" };
	cvar_t		fogmode = { C_FOGMODE,"0" };
	cvar_t		fogdensity = { C_FOGDENSITY,"0.001f" };
	*/


#else
	
	fprintf(file, "// generated by Halloween. Modify with care\n");

	var = cvar_vars;
	while(var)
	{
		fprintf(file,"%s %s %f\n",var->name,var->string,var->value);
		var = var->next;
	}
#endif

	fclose(file);

	return true;
}

hBool cons_LoadConfig(char *path)
{
	FILE	*file;
	cvar_t	*var;
	char	name[255];
	char	def[255];
    char	line[2000];
	float	value;
	char	*p;

	if(!path) {
		return false;
	}
	if(strlen(path) < 5) {
		return false;
	}
	m_ConsPrint("Loading config file: %s\n", path);
	
	
#ifdef H_MAC
	// SANDBOXED --------------------------------
	char * pDoc = KMiscTools::currentDocHomePath;
	char fullpath[255];
	sprintf(fullpath, "%s/%s", pDoc, CONFIG);
	m_ConsPrint("#### cons_LoadConfig file: %s\n", fullpath);
	file = fopen(fullpath, "rb");
	if(file == NULL)
	{
		p = KMiscTools::makeFilePath(path);
		m_ConsPrint("#### cons_LoadConfig file: %s\n", p);
		file = fopen(p, "rb");
	}
	
	//p = KMiscTools::makeFilePath(path);
	//m_ConsPrint("#### cons_LoadConfig file: %s\n", p);
	//file = fopen(p, "rb");
#else
	file = fopen(path,"rb");
#endif

	if(file == NULL)
	{
		m_ConsPrint("cons_LoadConfig error\n");
		return false;
  	}

	//numberCvar = 0;
                
    while(1)
	{
        if(fgets(line, 2000, file) == 0)
            break;
        sscanf(line, "%s %s %f",name,def,&value);
        var = Cvar_FindVar(name);
        if(var != NULL)
        {
           var->value = value;

		  // var->name = new char[200];
		   //var->string = new char[200];

			strcpy(var->name,name);
			strcpy(var->string,def);

	//		cvar_t * newVar = new cvar_t;
	//		newVar->value = value;
	//		strcpy(newVar->name, name);
	//		strcpy(newVar->string, def);

	//		cvarArray[numberCvar] = newVar;

	//		numberCvar++;
        }
    }
    fclose(file);

	//gl_ChangeFOV(fov.value);
	
	// HACK : on saute un morceau de zik de la presentation trop stressant
	//if(pres_track.value == 2)
	//	pres_track.value = 3;

	return true;
}
