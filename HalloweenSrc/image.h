#ifndef IMAGE_INCLUDED
#define IMAGE_INCLUDED

#include "alloween.h"


hBool	im_Init();
void	im_UnloadTextures();
void	im_UnloadMapTextures();
int		im_Load(char *name, int id, int picmip, hBool map_tex);
void	im_ScreenShotTGA();
void	im_ReloadTextures();


#endif  //IMAGE_INCLUDED
