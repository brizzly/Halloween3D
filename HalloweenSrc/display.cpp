
#include "alloween.cpp"
#include "display.h"

#ifdef H_MAC
#include "KMiscTools.h"
#endif


TEXLIST		*TextureList = NULL;

int		txid_FONT=0;
int		txid_NICEFONT=0;
int		txid_TARGET=0;
int		txid_RAYURE=0;
int		txid_LOGO=0;
int		txid_LOGO2=0;
int		txid_COPY=0;
int		txid_FLECHE=0;
int		txid_0=0;
int		txid_1=0;
int		txid_2=0;
int		txid_3=0;
int		txid_4=0;
int		txid_5=0;
int		txid_6=0;
int		txid_7=0;
int		txid_8=0;
int		txid_9=0;
int		txid_HEART=0;
int		txid_STARTGAME_G=0;
int		txid_ORDERGAME_G=0;
int		txid_OPTION_G=0;
int		txid_HELPORDER_G=0;
int		txid_QUITGAME_G=0;
int		txid_STARTGAME=0;
int		txid_STARTGAME2=0;
int		txid_CONFIG=0;
int		txid_CONFIG2=0;
int		txid_CONFIG3=0;
int		txid_CONFIG4=0;
int		txid_HELPORDER=0;
int		txid_HELPORDER2=0;
int		txid_QUITGAME=0;
int		txid_QUITGAME2=0;
int		txid_ORDERGAME=0;
int		txid_ORDERGAME2=0;
int		txid_CURSOR=0;
int		txid_GAMEOPTIONCONFIG=0;
int		txid_INPUTCONFIG=0;
int		txid_VIDEOCONFIG=0;
int		txid_SOUNDCONFIG=0;
int		txid_GAMEOPTIONCONFIG2=0;
int		txid_INPUTCONFIG2=0;
int		txid_VIDEOCONFIG2=0;
int		txid_SOUNDCONFIG2=0;
int		txid_CLOCK=0;
int		txid_BOMBE=0;
int		txid_BULLET=0;
int		txid_BUTTON=0;
int		txid_BUTTON_P=0;
int		txid_BUTTON_M=0;
int		txid_BUTTON_P_SEL=0;
int		txid_BUTTON_M_SEL=0;
int		txid_BUTTON_DEL=0;
int		txid_BUTTON2_DEL=0;
int		txid_RESUME=0;
int		txid_RESUME2=0;
int		txid_LEAVE=0;
int		txid_LEAVE2=0;
int		txid_EXIT=0;
int		txid_EXIT2=0;
int		txid_BUTTONGO=0;
int		txid_BUTTONGO2=0;
int		txid_BUTTONNEXT=0;
int		txid_BUTTONNEXT2=0;
int		txid_BUTTONBACK=0;
int		txid_BUTTONBACK2=0;
int		txid_CONSOLE=0;
int		txid_FLAME[8];
int		txid_LIGHTMAP_N[64];
int		txid_LOADINGMAP=0;
int		txid_CITROUILLE=0;
int		txid_GRIS=0;
int		txid_ORDER=0;
int		txid_HELP=0;
int		txid_FIOLE1_ICONE=0;
int		txid_FIOLE2_ICONE=0;
int		txid_FIOLE3_ICONE=0;
int		txid_BONUS1=0;
int		txid_CREDIT=0;
int		txid_BG1=0;
int		txid_BG2=0;
int		txid_BG2_G=0;
int		txid_NEWTITLE[30/*25*/]={0};
int		txid_TNN[2]={0};
int		txid_GAMEANIM[25]={0};
int		txid_HELPANIM[15]={0};
int		txid_OPTIONANIM[14]={0};
int		txid_ORDERANIM[19]={0};
int		txid_QUITANIM[25]={0};
int		txid_DEPLA_ICON[2];
int		txid_MORBAK_ICON[2];
int		txid_SUB_ICON[2];
int		txid_BAT_ICON[2];
int		txid_MOMY_ICON[2];
int		txid_JADETEAM=0;
int		txid_DEFAULTMAP=0;
int		txid_SELECTGAMESLOT=0;
int		txid_EASY=0;
int		txid_NORMAL=0;
int		txid_HARD=0;
int		txid_EMPTY=0;
int		txid_EASY_G=0;
int		txid_NORMAL_G=0;
int		txid_HARD_G=0;
int		txid_EMPTY_G=0;
int		txid_TDM=0;
int		txid_TDM_G=0;
int		txid_YES=0;
int		txid_YES_G=0;
int		txid_NO=0;
int		txid_NO_G=0;
int		txid_TOILE=0;
int		txid_OMBRAGE=0;



float	gFrameTime_msec = 16.667f;
float	gTotalTime_msec;
float	gFPSshowed;
Fnt_t	NormalFont, NiceFont;
hBool	gForceNoDepthTest = false;
float	gFadingTime;
int		gFadingDir;


void disp_Init()
{
	gFadingTime = 0;
	gFadingDir = -1;

	WorldColorR = 1;
	WorldColorG = 1;
	WorldColorB = 1;
}


#ifdef H_WINDOWS
LARGE_INTEGER _unitsPerSecond;
#endif

void disp_InitMillisecond( void ) 
{ 
#ifdef H_WINDOWS
	QueryPerformanceFrequency(&_unitsPerSecond );  
	// unitsPerSecond = N unités par secondes
#endif
}

long disp_GetClock()
{
	long	clk;

#ifdef H_WINDOWS
	clk = clock();
	/*
	LARGE_INTEGER performanceCount ; 
	double microseconds; 
  
	if( QueryPerformanceCounter( &performanceCount ) == false )
		return 0;

	microseconds = (double)performanceCount.LowPart  /  (double)_unitsPerSecond.LowPart;
	return (long)(microseconds*1000);
	*/
#endif

#ifdef H_LINUX
	clk = SDL_GetTicks();
#endif

#ifdef H_MAC
	clk = KMiscTools::getMilliseconds();
#endif

	if(clk < 0)
	{
		return 16;
	}
	return clk;
}

void disp_GetFrameTime()
{
	static	long			start=-1l;
	static	unsigned int	nbFrame=0;
	float					FPS;

	nbFrame++;
	if(start==-1l)
		start = disp_GetClock();
	gFrameTime_msec = (disp_GetClock() - start)/(float)nbFrame;

	// 166FPS limitation
	while(gFrameTime_msec<6.0f)
		gFrameTime_msec = (disp_GetClock() - start)/(float)nbFrame;

	gTotalTime_msec += gFrameTime_msec;
	FPS = 1000.0f/gFrameTime_msec;

	if(gTotalTime_msec > 250.0f)
	{
		gFPSshowed = FPS;
		gTotalTime_msec=0;

		nbFrame = 0;
		start = disp_GetClock();
	}
}

void disp_World()
{
	float	s_WorldColorR;
	float	s_WorldColorG;
	float	s_WorldColorB;

	if(ProgramState == PS_GAME ||
		ProgramState == PS_DIE ||
		ProgramState == PS_WIN ||
		ProgramState == PS_ENDSCORE ||
		ProgramState == PS_PAUSEDGAME)
	{
		s_WorldColorR = WorldColorR;
		s_WorldColorG = WorldColorG;
		s_WorldColorB = WorldColorB;
		
		// HACK
		if(gMapIntro && MapDataFormat == MAP_POLYGONS)
		{
			WorldColorR *= 0.700f; // *= plutot que =
			WorldColorG *= 0.700f;
			WorldColorB *= 0.700f;
		}


		fx_DrawParticule(gCamera.Pos);
		if(MapDataFormat == MAP_BSP)
		{
			frustrum_ExtractPlanes(gFrustrumPlanes);
		}
		bsp_DrawMap(&gCamera);
		ab_Draw(gCamera.Pos);
		o_RenderEntitiesList();
		o_RendCautchFlask();
		o_RendCautchletters();
		DrawGameStatutBar();

		// HACK
		if(developer.value)
		{
		//	g_DispString(14,"gNumberOfMonsters: %d",gNumberOfMonsters);
			g_DispString(14,"gFrameTime_msec: %f",gFrameTime_msec);
			g_DispString(14,"gTotalTime_msec: %f",gTotalTime_msec);
			g_DispString(14,"clock: %f",(float)disp_GetClock());
		}
			
		if(draw_fps.value)
			g_DispString(14,"%d FPS",(int)gFPSshowed);
                        
		// hack
		//g_DispString(14,"%d MEM",(int)gMemoryUsage);
		
		if(gIsMultiplayer)
		{
			//net_DispNetInfos();
		}

		WorldColorR = s_WorldColorR;
		WorldColorG = s_WorldColorG;
		WorldColorB = s_WorldColorB;
	}
	else
	{
		WorldColorR = 1;
		WorldColorG = 1;
		WorldColorB = 1;
	}

	if(ProgramState == PS_GAMEOVER)
	{
		o_RenderEntitiesList();
		gForceNoDepthTest = false;
	}
}

void disp_Scene()
{
	if(gIsServer && net_dedicated.value)
		return;
	gl_CameraView();
	disp_World();
	disp_GetFrameTime();
}

void disp_StringFont(Color_t color, int xpos, int ypos, int xoff, int yoff, char *s)
{
	int x = xpos;
	int y = ypos;
	int carrage = 0;
	int	index;

	NormalFont.fsize = gFontSize;
	NormalFont.ftextid = txid_FONT;
	NormalFont.OrthoProjX = 640;
	NormalFont.OrthoProjY = 480;
    carrage = g_fontGetCharHits(s, '\n');

	for(index=0 ; s[index] ; index++, x += (int)(NormalFont.fsize/HACKFONT))
	{
		switch(s[index])
		{
			case 0:
			break;

			case '\n':
			y -= NormalFont.fsize;
            x = xpos - (int)(NormalFont.fsize/HACKFONT);
			continue;

            default :		
			disp_CharFont(color, NormalFont, s[index], x+xoff, y+yoff);
            break;
		}
	}
}

void disp_CharFont(Color_t color, Fnt_t fnt, char c, int x, int y)
{
	int			carnum;
	Render_t	rend;

	carnum = (int)c;

	rend.is_texture = true;
	rend.texture_id = fnt.ftextid;
	rend.nvert = 4;
	rend.is_blend = true;
	rend.blend_src_mode = GL_SRC_ALPHA;
	rend.blend_dst_mode = GL_ONE_MINUS_SRC_ALPHA;
	rend.is_color = true;
	rend.color = color;
	rend.begin_mode = GL_QUADS;
	rend.is_depthtest = false;
	rend.is_depthmask = true;
	rend.is_draw2D = true;
	rend.ortho_w = fnt.OrthoProjX;
	rend.ortho_h = fnt.OrthoProjY;

	rend.xyz[0][0] = (float)x;
	rend.xyz[0][1] = (float)y;
	rend.xyz[0][2] = 0;
	rend.uv[0][0] = fnt.fpoints[carnum][0];
	rend.uv[0][1] = fnt.fpoints[carnum][1];

	rend.xyz[1][0] = (float)x+fnt.fsize;
	rend.xyz[1][1] = (float)y;
	rend.xyz[1][2] = 0;
	rend.uv[1][0] = fnt.fpoints[carnum][0]+fnt.foffsetX;
	rend.uv[1][1] = fnt.fpoints[carnum][1];

	rend.xyz[2][0] = (float)x+fnt.fsize;
	rend.xyz[2][1] = (float)y+fnt.fsize;
	rend.xyz[2][2] = 0;
	rend.uv[2][0] = fnt.fpoints[carnum][0]+fnt.foffsetX;
	rend.uv[2][1] = fnt.fpoints[carnum][1]+fnt.foffsetY;

	rend.xyz[3][0] = (float)x;
	rend.xyz[3][1] = (float)y+fnt.fsize;
	rend.xyz[3][2] = 0;
	rend.uv[3][0] = fnt.fpoints[carnum][0];
	rend.uv[3][1] = fnt.fpoints[carnum][1]+fnt.foffsetY;

	gl_RenderPoly(rend);
}

void text_Disp(int fontsize, char *text, int xpos, int ypos, int R, int G, int B, hBool centered)
{
	float	xscale;
	int		car;
	char	*c;
	int		text_screensize;
	int		centered_offset;
	Color_t		color;

	color.R = R/255.0f;
	color.G = G/255.0f;
	color.B = B/255.0f;
	color.A = 1;

	if(xpos > 640)
		xpos = 640;
	else if(xpos<0)
		xpos = 0;

	if(ypos>480)
		ypos = 480;
	else if(ypos<0)
		ypos = 0;

	NiceFont.fsize = fontsize;
	NiceFont.ftextid = txid_NICEFONT;
	NiceFont.OrthoProjX = 640;
	NiceFont.OrthoProjY = 480;
	xscale = (float)fontsize / 32.0f;

	if(centered)
	{
		text_screensize = 0;
		for(c=text ; *c ; c++)
		{
			if(xpos >= 640)
				break;
			car = disp_GetCarIndex(*c);
			text_screensize += (int)((NiceFont.xdecal[car] * xscale));
		}
		centered_offset = (640 - text_screensize) / 2;
		xpos += centered_offset;
	}

	for(c=text ; *c ; c++)
	{
		if(xpos >= 640)
			break;

		car = disp_GetCarIndex(*c);
		disp_CharFont(color, NiceFont, car, xpos, ypos);
		xpos += (int)((NiceFont.xdecal[car] * xscale));
	}
}

int disp_GetCarIndex(char car)
{
	if(car >= 'a' && car <= 'z')
	{
		return car - 'a';
	}
	else if(car >= 'A' && car <= 'Z')
	{
		return car - 'A' + 26;
	}
	else if(car >= '0' && car <= '9')
	{
		return car - '0' + 52;
	}
	else if(car == '-')
	{
		return 62;
	}
	return 63;
}

void disp_DrawSpriteCol(int id_texture, int sx, int sy, int sw, int sh, hBool tr, float R, float G, float B)
{
	vect_t	pos;
	vect_t	siz;
	vect_t	ortho;
	Color_t	color;

	pos.X = (float)sx;
	pos.Y = (float)sy;
	siz.X = (float)sw;
	siz.Y = (float)sh;
	ortho.X = 640;
	ortho.Y = 480;
	color.R = R;
	color.G = G;
	color.B = B;
	color.A = 1;
	disp_Sprite2D(id_texture, ortho, pos, siz, true, color, tr);
}

void disp_DrawSprite(int id_texture, int sx, int sy, int sw, int sh, hBool tr)
{
	vect_t	pos;
	vect_t	siz;
	vect_t	ortho;
	Color_t	color;

	pos.X = (float)sx;
	pos.Y = (float)sy;
	siz.X = (float)sw;
	siz.Y = (float)sh;
	ortho.X = 640;
	ortho.Y = 480;
	color.R = WorldColorR;
	color.G = WorldColorG;
	color.B = WorldColorB;
	disp_Sprite2D(id_texture, ortho, pos, siz, false, color, tr);
}

void disp_DrawSprite2Aplha(int id_texture, int sx, int sy, int sw, int sh, hBool tr, float alpha)
{
	vect_t	pos;
	vect_t	siz;
	vect_t	ortho;
	Color_t	color;

	pos.X = (float)sx;
	pos.Y = (float)sy;
	siz.X = (float)sw;
	siz.Y = (float)sh;
	ortho.X = 800;
	ortho.Y = 600;
	color.R = color.G = color.B = alpha;
	color.A = 1;
	disp_Sprite2D(id_texture, ortho, pos, siz, true, color, tr);
}

void disp_DrawSprite_2(int id_texture, int sx, int sy, int sw, int sh, hBool tr)
{
	vect_t	pos;
	vect_t	siz;
	vect_t	ortho;
	Color_t	color;

	pos.X = (float)sx;
	pos.Y = (float)sy;
	siz.X = (float)sw;
	siz.Y = (float)sh;
	ortho.X = 800;
	ortho.Y = 600;
	color.R = color.G = color.B = 1;
	disp_Sprite2D(id_texture, ortho, pos, siz, false, color, tr);
}

void disp_DrawLine(int x1, int y1, int x2, int y2, float size, int color)
{
	vect_t	v1;
	vect_t	v2;
	vect_t	ortho;
	Color_t	col;

	v1.X = (float)x1;
	v1.Y = (float)y1;
	v2.X = (float)x2;
	v2.Y = (float)y2;
	ortho.X = 640;
	ortho.Y = 480;
	switch(color)
	{
		case GLRED:
		col.R = 1;
		col.G = 0;
		col.B = 0;
		break;

		default:
		col.R = 1;
		col.G = 1;
		col.B = 1;
		break;
	}
	disp_Line2D(ortho, v1, v2, col, size);
}

void disp_DrawLine_2(int x1, int y1, int x2, int y2, float size, int color)
{
	vect_t	v1;
	vect_t	v2;
	vect_t	ortho;
	Color_t	col;

	v1.X = (float)x1;
	v1.Y = (float)y1;
	v2.X = (float)x2;
	v2.Y = (float)y2;
	ortho.X = 800;
	ortho.Y = 600;
	col.R = col.G = col.B = 1;
	disp_Line2D(ortho, v1, v2, col, size);
}

void disp_DrawNumber_leus(int number, int digit, int x, int y, int w, int h, int R, int G, int B, hBool tr)
{
	int	val;
	int	dec;
	int	i;
	int	w2;

	if(gIsServer && net_dedicated.value)
		return;

	w2 = (int)((float)w * 0.75f);
	dec=digit*w2;

	// HACK
	for(i=digit ; i>1 ; i--)
	if(number < (int)pow(10.0,i-1))
		dec -= w2;

	for(i=0 ; i<digit ; i++)
	{
		val = number % 10;
		number /= 10;
		disp_DrawSpriteCol(txid_0+val,x+dec,y,w,h,tr,(float)R/255.0f,(float)G/255.0f,(float)B/255.0f);
		dec -= w2;
		if(!number)
			break;
	}
}

void disp_DrawNumber(int number, int digit, int x, int y, int w, int h, int R, int G, int B, hBool tr)
{
	int	val;
	int	dec;
	int	i;
	Color_t	color;

	if(gIsServer && net_dedicated.value)
		return;

	color.R = (float)R/255.0f;
	color.G = (float)G/255.0f;
	color.B = (float)B/255.0f;
	gl_SetGlobalColor(color);

	dec=digit*w;
	for(i=0 ; i<digit ; i++)
	{
		val = number % 10;
		number /= 10;
		disp_DrawSprite(txid_0+val,x+dec,y,w,h,tr);
		dec-=w;
		if(!number)
			break;
	}

	color.R = WorldColorR;
	color.G = WorldColorG;
	color.B = WorldColorB;
	gl_SetGlobalColor(color);
}

void disp_Line2D(vect_t ortho, vect_t v1, vect_t v2, Color_t color, float size)
{
	Render_t	rend;

	rend.is_texture = false;
	rend.nvert = 2;
	rend.is_blend = false;
	rend.is_color = true;
	rend.color = color;
	rend.begin_mode = GL_LINE_LOOP;
	rend.is_depthtest = false;
	rend.is_depthmask = true;
	rend.is_draw2D = true;
	rend.ortho_w = (int)ortho.X;
	rend.ortho_h = (int)ortho.Y;

	rend.xyz[0][0] = v1.X;
	rend.xyz[0][1] = v1.Y;
	rend.xyz[0][2] = 0;

	rend.xyz[1][0] = v2.X;
	rend.xyz[1][1] = v2.Y;
	rend.xyz[1][2] = 0;

	// HACK
	glLineWidth(size);
	gl_RenderPoly(rend);
	glLineWidth(1);
}

void disp_SetFading2D_Out(int next_pstate)
{
	NextProgramState = next_pstate;
	gFadingDir = 1;
	gFadingTime = 0;
}

void disp_SetFading2D_Int()
{
//	gl_ClearScreen(); // 
//	gl_SwapBuffer();	//
//	gl_ClearScreen(); // 

	gFadingDir = 0;
	gFadingTime = 1;
	ProgramState = NextProgramState;
}

hBool disp_IsFadeDone()
{
	if(gFadingTime == 0 && gFadingDir == -1)
		return true;
	return false;
}

void disp_Fading2D_Process()
{
	float	speed = 0.0020f;
/*
	if(ProgramState == PS_GAME)
	{
		gFadingTime = 0;
		gFadingDir = -1;
		return;
	}
*/
	switch(gFadingDir)
	{
		case 0:
		if(gFadingTime > 0)
		{
			gFadingTime -= speed * gFrameTime_msec;
			if(gFadingTime <= 0)
			{
				gFadingTime = 0;
				gFadingDir = -1;
			}
		}
		break;

		case 1:
		if(gFadingTime < 1)
		{
			gFadingTime += speed * gFrameTime_msec;
			if(gFadingTime >= 1)
			{
				gFadingTime = 1;
				disp_SetFading2D_Int();
			}
		}
		break;

		default:
		gFadingTime = 0;
		break;
	}
}

void disp_Sprite2D(int SpriteTextID, vect_t ortho, vect_t pos, vect_t siz, hBool IsColor, Color_t color, hBool transparent)
{
	Render_t	rend;

	rend.is_texture = true;
	rend.texture_id = SpriteTextID;
	rend.nvert = 4;
	rend.begin_mode = GL_QUADS;
	rend.is_blend = transparent;
	rend.blend_src_mode = GL_SRC_ALPHA;
	rend.blend_dst_mode = GL_ONE_MINUS_SRC_ALPHA;
	rend.is_color = IsColor;
	rend.color = color;
	rend.is_depthtest = false;
	rend.is_depthmask = true;
	rend.is_draw2D = true;
	rend.ortho_w = (int)ortho.X;
	rend.ortho_h = (int)ortho.Y;

//	if(gFadingDir != -1)
	{
		rend.is_color = true;
		rend.color.R = (1-gFadingTime);
		rend.color.G = (1-gFadingTime);
		rend.color.B = (1-gFadingTime);
		rend.color.A = 1;
	}
	
	rend.xyz[0][0] = pos.X;
	rend.xyz[0][1] = pos.Y;
	rend.xyz[0][2] = 0;
	rend.uv[0][0] = 0;
	rend.uv[0][1] = 0;

	rend.xyz[1][0] = pos.X;
	rend.xyz[1][1] = pos.Y+siz.Y;
	rend.xyz[1][2] = 0;
	rend.uv[1][0] = 0;
	rend.uv[1][1] = 1;

	rend.xyz[2][0] = pos.X+siz.X;
	rend.xyz[2][1] = pos.Y+siz.Y;
	rend.xyz[2][2] = 0;
	rend.uv[2][0] = 1;
	rend.uv[2][1] = 1;

	rend.xyz[3][0] = pos.X+siz.X;
	rend.xyz[3][1] = pos.Y;
	rend.xyz[3][2] = 0;
	rend.uv[3][0] = 1;
	rend.uv[3][1] = 0;

	gl_RenderPoly(rend);
}

void disp_Particule(vect_t pos, int color, float size)
{
	Render_t	rend;
	float		R=0, G=0, B=0;

	if(gIsServer && net_dedicated.value)
		return;

	switch(color)
	{
		case GLRED:
		R = 1;
		G = 0;
		B = 0;
		break;
		
		case GLBLUE:
		R = 0;
		G = 0;
		B = 1;
		break;
		
		case GLGREEN:
		R = 0;
		G = 1;
		B = 0;
		break;
		
		case GLYELLOW:
		R = 0.952f;
		G = 0.713f;
		B = 0.07f;
		break;
	}

	if(IsColorScreen)
	{
		R *= WorldColorR;
		G *= WorldColorG;
		B *= WorldColorB;
	}
	
	rend.is_blend = false;
	rend.is_color = true;
	rend.color.R = R;
	rend.color.G = G;
	rend.color.B = B;
	rend.color.A = 1;
	rend.is_texture = false;
	rend.nvert = 1;
	rend.begin_mode = GL_POINTS;
	rend.is_depthtest = true;
	rend.is_depthmask = true;
	rend.is_draw2D = false;
	rend.xyz[0][0] = pos.X;
	rend.xyz[0][1] = pos.Y;
	rend.xyz[0][2] = pos.Z;
	gl_RenderPoly(rend);
}

void disp_FaceList(pFace List)
{
	pFace face;

	for(face=List ; face ; face=face->Next)
		disp_Face(face);
}

void disp_Face(pFace face)
{
	int			index;
	Render_t	rend;

	rend.nvert = face->NumberOfVertices;
	rend.is_texture = true;
	rend.texture_id = face->TextureID;
	rend.is_blend = false;

	rend.is_color = true;
	rend.color.R = WorldColorR;
	rend.color.G = WorldColorG;
	rend.color.B = WorldColorB;

	rend.begin_mode = GL_POLYGON;
	rend.is_depthtest = true;
	rend.is_depthmask = true;
	rend.is_draw2D = false;
        
	if(draw_face.value == false)
	{
                rend.is_depthtest = false;
                rend.is_depthmask = false;
		rend.is_texture = false;
	}

	if(face->Type == ACTIONBRUSH)
	{
		if(IsColorScreen == false)
		{
			rend.color.R = ActionBrushTab[face->actionbrush_id].R;
			rend.color.G = ActionBrushTab[face->actionbrush_id].G;
			rend.color.B = ActionBrushTab[face->actionbrush_id].B;
		}
		else
		{
			rend.color.R *= ActionBrushTab[face->actionbrush_id].R;
			rend.color.G *= ActionBrushTab[face->actionbrush_id].G;
			rend.color.B *= ActionBrushTab[face->actionbrush_id].B;
		}
	}

	for(index=0 ; index<face->NumberOfVertices ; index++)
	{
		rend.uv[index][0] = face->Vertice[index].U;
		rend.uv[index][1] = face->Vertice[index].V;
		rend.xyz[index][0] = face->Vertice[index].X;
		rend.xyz[index][1] = face->Vertice[index].Y;
		rend.xyz[index][2] = face->Vertice[index].Z;
	}
	gl_RenderPoly(rend);
}




