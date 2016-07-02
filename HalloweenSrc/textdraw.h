
#ifndef TEXTDRAW_INCLUDED
#define TEXTDRAW_INCLUDED

#include "alloween.h"

#ifdef H_LINUX

#include <stdarg.h>


#endif


void	g_InitFont();
void	g_DispString(int format, char *string, ...);
void	g_Disp(int format, int line, int colonne, char *string, ...);
void	g_DispStringLine(int format, int line, char *string, ...);
void	g_DispCenterString(int format, int ligne, char *string, ...);
void	g_fontForeColorReset(void);
void	g_fontShadowColorReset(void);
void	g_fontGradientColorReset(void);
void	g_fontShadowColor(float r, float g, float b);
hBool	g_LoadTextureStruct();
void	g_UnloadTextureStruct();
void	g_ResetTextureStruct();
void	g_fontRegion(int x, int y, int w, int h);
int		g_fontGetCharHits(char *s, char f);

void	fontDrawString(int xpos, int ypos, char *s);



#endif // TEXTDRAW_INCLUDED
