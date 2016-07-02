#ifndef	BSP2_INCLUDED
#define	BSP2_INCLUDED

#define	MAX_ARRAY			20000

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "bsp2math.h"

bool	menu_visibility;
bool	menu_nodetail;
bool	menu_OptimizeBSP;

void	Error(char *QuitMessage);
void	Rename(char *name, char *extension);
void	main(int argc, char **argv);
double	FloatTime();
void	DispTimeElapsed(double TimeStart);

#endif	BSP2_INCLUDED
