
#ifndef INCLUDE_DISPLAY__
#define INCLUDE_DISPLAY__


#include "alloween.h"


#define	HACKFONT				1.7f //2.0f



typedef struct fnttag
{
	float	fpoints[257][2];
	float	foffsetX;
	float	foffsetY;
	int		xdecal[64];
	int		ftextid;
	int		fsize;
	int		OrthoProjX;
	int		OrthoProjY;

} Fnt_t;

typedef struct colortag
{
	float	R;
	float	G;
	float	B;
	float	A;

} Color_t;

typedef struct rendertag
{
	int		nvert;

	hBool	is_depthtest;
	hBool	is_depthmask;
	hBool	is_draw2D;
	int		ortho_w;
	int		ortho_h;

	hBool	is_texture;
	int		texture_id;

	hBool	is_blend;
	int		blend_src_mode;
	int		blend_dst_mode;

	hBool	is_color;
	Color_t	color;

	int		begin_mode;
	float	uv[10][2];
	float	xyz[10][3];

} Render_t;


void text_Disp(int fontsize, char *text, int xpos, int ypos, int R, int G, int B, hBool centered);

void	disp_Init();
void	disp_InitMillisecond( void );
long 	disp_GetClock();
void 	disp_GetFrameTime();
void 	disp_World();
void 	disp_Scene();
void 	disp_StringFont(Color_t color, int xpos, int ypos, int xoff, int yoff, char *s);
void 	disp_CharFont(Color_t color, Fnt_t fnt, char c, int x, int y);
int 	disp_GetCarIndex(char car);
void 	disp_DrawSprite(int id_texture, int sx, int sy, int sw, int sh, hBool tr);
void	disp_DrawSprite2Aplha(int id_texture, int sx, int sy, int sw, int sh, hBool tr, float alpha);
void 	disp_DrawSprite_2(int id_texture, int sx, int sy, int sw, int sh, hBool tr);
void 	disp_DrawLine(int x1, int y1, int x2, int y2, float size, int color);
void 	disp_DrawLine_2(int x1, int y1, int x2, int y2, float size, int color);
void 	disp_DrawNumber_leus(int number, int digit, int x, int y, int w, int h, int R, int G, int B, hBool tr);
void 	disp_DrawNumber(int number, int digit, int x, int y, int w, int h, int R, int G, int B, hBool tr);
void 	disp_Line2D(vect_t ortho, vect_t v1, vect_t v2, Color_t color, float size);
void 	disp_Sprite2D(int SpriteTextID, vect_t ortho, vect_t pos, vect_t siz, hBool IsColor, Color_t color, hBool transparent);
void 	disp_Particule(vect_t pos, int color, float size);
void 	disp_FaceList(pFace List);
void 	disp_Face(pFace face);
void	disp_SetFading2D_In();
void	disp_SetFading2D_Out(int next_pstate);
hBool	disp_IsFadeDone();
void	disp_Fading2D_Process();


#endif  //INCLUDE_DISPLAY__

