
#ifndef STORY_INCLUDED
#define STORY_INCLUDED

#include "alloween.h"


void Story_SetText(int TextID);
void Story_Process();
void Story_DispCadreAlpha();
void Story_NextText();
void Story_DispString(int format, int ligne, int col, int nCar, char *string);
void Story_DispN(Color_t color, int xpos, int ypos, int xoff, int yoff, char *s, int N);




#endif // STORY_INCLUDED
