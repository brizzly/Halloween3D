
#ifndef OPENGL_INCLUDED
#define OPENGL_INCLUDED


#include "alloween.h"
#include "mskmodel.h"
#include "display.h"


#define		RENDER_TRIANGLE		GL_TRIANGLES
#define		RENDER_LINELOOP		GL_LINE_LOOP
#define		RENDER_POLYGON		GL_POLYGON


enum
{
	GLRED,
	GLBLUE,
	GLGREEN,
	GLYELLOW
};

void 	gl_SetFog();
void 	gl_StopFog();
void	gl_DrawFaces(Face_t *FaceList, vect_t pos);
void	gl_DrawVectors(Face_t *ListFace);
void	gl_DrawVector(vect_t *pos, vect_t *v, float scale);
void	gl_RenderEntity(pEntity Entity, pEntity PlayerEnt, float R, float G, float B);
void	gl_RenderFace_ENV(Face_t *pFace, int angleY);
void	gl_RenderFace(Face_t *pFace, pEntity PlayerEnt, hBool isBlend, float R, float G, float B, float A);
void	gl_DrawHoldedEntity(pEntity Entity, pEntity PlayerEnt, float R, float G, float B);
void	gl_DrawActionBrush(pFace FaceList, vect_t campos, vect_t Mov, vect_t Rot, vect_t Siz);
void	gl_DrawEntity(pEntity Entity, pEntity PlayerEnt, float R, float G, float B);
void	gl_DrawNormal(Face_t *pCurr);
void	gl_DrawBoundaries(vect_t *Cube);
uint	gl_GetBlendMode(int mode);
void	gl_DrawBoundingBox(pEntity Entity);
hBool	gl_initMultitexture();



int		gl_InitGL();
void	gl_ClearScreen();
void 	gl_Perspective( GLdouble fovY, GLdouble aspect, GLdouble zNear, GLdouble zFar );
void 	gl_ReSizeGLScene(int Width, int Height);
void	gl_ChangeFOV(float fov);
void	gl_SwapBuffer();
void	gl_BindTexture(int id);
void	gl_SetClearColor(Color_t color);
void	gl_SetGlobalColor(Color_t color);
void	gl_SetGlobalColorA(Color_t color);
void	gl_PushMatrix();
void	gl_PopMatrix();
void	gl_TranslateMatrix(vect_t trans);
void	gl_RotateMatrix(vect_t rot);
void	gl_ScaleMatrix(vect_t scale);
void	gl_LoadSurfaceTexture(byte *surf, uint sformat, uint scolor_format1, uint scolor_format2, int w, int h, int id);
void	gl_CameraView();
void	gl_ProjectView(hBool IsProjOn, int ortho_w, int ortho_h);
void	gl_RenderPoly(Render_t render);



#endif // OPENGL_INCLUDED
