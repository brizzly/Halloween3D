
#include "alloween.cpp"



#ifdef H_LINUX
 
#include <stdarg.h>

#endif



int				gPrintLine;
int				matrixMode;
int				polyMode[2];
int				lightingOn;
int				blendOn;
int				depthOn;
int				textureOn;
int				scissorOn;
int				blendSrc;
int				blendDst;
int				gFontSize;
float			gAlphaFont;
char			gBuffer[CONSOLE_MAXDISPCHAR];



void g_InitFont()
{
	int	i;
	float	invx, invy;
	int		orig;
	double	x, y;
	double	offsetX, offsetY;

	if(gIsServer && net_dedicated.value)
		return;

	gAlphaFont = 1;

	// Standart Fonte =============================
	offsetX = pow(16.0, -1.0);
	offsetY = pow(16.0, -1.0);
	i = 0;
	for(y=1-offsetY; y>=0; y-=offsetY)
	for(x=0; x<=1-offsetX; x+=offsetX, i++)
	{
		if(i<0 || i>= 257)
			break;
		NormalFont.fpoints[i][0] = (float)x;
		NormalFont.fpoints[i][1] = (float)y;
	}
	m_ConsPrint("Font Init: i max: %d\n", i);

	NormalFont.foffsetX = (float)(15.0 / 256.0);
	NormalFont.foffsetY = (float)(15.0 / 256.0);
	NormalFont.ftextid = txid_FONT;


	// Nice Fonte =============================
	invx = (float)pow (8.0, -1.0);
	invy = (float)pow (8.0, -1.0);
	i = 0;
	for(y=1-invy ; y >= 0 ; y -= invy)
	for(x=0 ; x <= 1-invx ; x += invx, i++)
	{
		NiceFont.fpoints[i][0] = (float)x;
		NiceFont.fpoints[i][1] = (float)y;
	}
	NiceFont.foffsetX = 31.0f/256.0f;
	NiceFont.foffsetY = 31.0f/256.0f;
	NiceFont.ftextid = txid_NICEFONT;

	orig = 'a';
	NiceFont.xdecal['a'-orig] = 19;
	NiceFont.xdecal['b'-orig] = 12;
	NiceFont.xdecal['c'-orig] = 16;
	NiceFont.xdecal['d'-orig] = 15;
	NiceFont.xdecal['e'-orig] = 12;
	NiceFont.xdecal['f'-orig] = 12;
	NiceFont.xdecal['g'-orig] = 16;
	NiceFont.xdecal['h'-orig] = 15;
	NiceFont.xdecal['i'-orig] = 7;
	NiceFont.xdecal['j'-orig] = 7;
	NiceFont.xdecal['k'-orig] = 15;
	NiceFont.xdecal['l'-orig] = 12;
	NiceFont.xdecal['m'-orig] = 20;
	NiceFont.xdecal['n'-orig] = 17;
	NiceFont.xdecal['o'-orig] = 18;
	NiceFont.xdecal['p'-orig] = 13;
	NiceFont.xdecal['q'-orig] = 16;
	NiceFont.xdecal['r'-orig] = 15;
	NiceFont.xdecal['s'-orig] = 12;
	NiceFont.xdecal['t'-orig] = 17;
	NiceFont.xdecal['u'-orig] = 21;
	NiceFont.xdecal['v'-orig] = 21;
	NiceFont.xdecal['w'-orig] = 24;
	NiceFont.xdecal['x'-orig] = 19;
	NiceFont.xdecal['y'-orig] = 18;
	NiceFont.xdecal['z'-orig] = 15;

	orig = 'A' - 26;
	NiceFont.xdecal['A'-orig] = 26;
	NiceFont.xdecal['B'-orig] = 16;
	NiceFont.xdecal['C'-orig] = 20;
	NiceFont.xdecal['D'-orig] = 20;
	NiceFont.xdecal['E'-orig] = 15;
	NiceFont.xdecal['F'-orig] = 14;
	NiceFont.xdecal['G'-orig] = 19;
	NiceFont.xdecal['H'-orig] = 20;
	NiceFont.xdecal['I'-orig] = 9;
	NiceFont.xdecal['J'-orig] = 10;
	NiceFont.xdecal['K'-orig] = 19;
	NiceFont.xdecal['L'-orig] = 16;
	NiceFont.xdecal['M'-orig] = 26;
	NiceFont.xdecal['N'-orig] = 22;
	NiceFont.xdecal['O'-orig] = 24;
	NiceFont.xdecal['P'-orig] = 17;
	NiceFont.xdecal['Q'-orig] = 24;
	NiceFont.xdecal['R'-orig] = 21;
	NiceFont.xdecal['S'-orig] = 15;
	NiceFont.xdecal['T'-orig] = 23;
	NiceFont.xdecal['U'-orig] = 27;
	NiceFont.xdecal['V'-orig] = 27;
	NiceFont.xdecal['W'-orig] = 28;
	NiceFont.xdecal['X'-orig] = 26;
	NiceFont.xdecal['Y'-orig] = 26;
	NiceFont.xdecal['Z'-orig] = 19;

	orig = '0' - 52;
	NiceFont.xdecal['0'-orig] = 24;
	NiceFont.xdecal['1'-orig] = 9;
	NiceFont.xdecal['2'-orig] = 17;
	NiceFont.xdecal['3'-orig] = 17;
	NiceFont.xdecal['4'-orig] = 17;
	NiceFont.xdecal['5'-orig] = 17;
	NiceFont.xdecal['6'-orig] = 17;
	NiceFont.xdecal['7'-orig] = 18;
	NiceFont.xdecal['8'-orig] = 16;
	NiceFont.xdecal['9'-orig] = 17;

	NiceFont.xdecal[62] = 10;	// '-' car
	NiceFont.xdecal[63] = 14;	// ' ' car
}

hBool g_LoadTextureStruct()
{
	int	i;

	TextureList = (TEXLIST*)s_malloc(TEXTURELOADED*sizeof(TEXLIST));
	if(!TextureList)
		return false;
	memset(TextureList,0,TEXTURELOADED*sizeof(TEXLIST));

	for(i=0 ; i<TEXTURELOADED ; i++)
	{
		TextureList[i].name = (char*)s_malloc(TEXTURENAMEMAX*sizeof(char));
		memset(TextureList[i].name,' ',TEXTURENAMEMAX*sizeof(char));
	}

	return true;
}

void g_UnloadTextureStruct()
{
	int	i;

	m_ConsPrint("g_UnloadTextureStruct\n");

	for(i=0 ; i<TEXTURELOADED ; i++)
	{
		if(TextureList && TextureList[i].name) {
			s_free(TextureList[i].name);
			TextureList[i].name = NULL;
		}
	}
	s_free(TextureList);
	TextureList = NULL;

	m_ConsPrint("g_UnloadTextureStruct done.\n");
}

void g_ResetTextureStruct()
{
	int	i;

	m_ConsPrint("g_ResetTextureStruct\n");
	im_UnloadTextures();

	for(i=0 ; i<TEXTURELOADED ; i++)
	{
		TextureList[i].used = false;
		memset(TextureList[i].name,' ',TEXTURENAMEMAX*sizeof(char));
	}

	m_ConsPrint("g_ResetTextureStruct done.\n");
}

void fontDrawString (int xpos, int ypos, char *s)
{
	Color_t	color;

	color.R = color.G = color.B = gAlphaFont;
//	if(gFadingTime > 0 && gFadingTime < 1)
		color.R = color.G = color.B = (1-gFadingTime);
	color.A = 1;
	disp_StringFont(color, xpos, ypos, 0, 0, s);
}

void g_DispCenterString(int format, int ligne, char *string, ...)
{
	va_list	msg;
	int		taille;
	int		offset;
	int		xpos;
	int		ypos;
	
	va_start(msg,string);
	vsprintf(gBuffer, string, msg);	
	va_end(msg);

	taille = (int)(strlen(gBuffer)*format);

	offset = (int)((640.0f-taille)/2.0f);
	offset += (int)((float)taille*0.25f);
	gFontSize = (int)(format);

	xpos = offset;
	ypos = (int)(480.0f-(ligne+1)*format);
	fontDrawString(xpos, ypos, gBuffer);
}

void g_Disp(int format, int line, int colonne, char *string, ...)
{
	va_list	msg;
	int		xpos;
	int		ypos;
	
	va_start(msg,string);
	vsprintf(gBuffer, string, msg);	
	va_end(msg);
		
	xpos = (int)(colonne*format);
	ypos = (int)(480.0f-(line+1)*format);

	gFontSize = (int)(format);
	fontDrawString(xpos, ypos, gBuffer);
}

void g_DispString(int format, char *string, ...)
{
	va_list	msg;
	
	va_start(msg,string);
	vsprintf(gBuffer, string, msg);	
	va_end(msg);

	g_Disp(format, gPrintLine, 0, gBuffer);
	gPrintLine++;
}

void g_DispStringLine(int format, int line, char *string, ...)
{
	va_list	msg;

	va_start(msg,string);
	vsprintf(gBuffer, string, msg);	
	va_end(msg);

	g_Disp(format, line, 0, gBuffer);
}

int g_fontGetCharHits (char *s, char f)
{
	int hits = 0;

	while (*s)
	{
		if (*s == f)
			hits ++;
		*s ++;
	}
	return hits;
}

