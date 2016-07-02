/*
 *	BSP MAKER (C) Copyright 2001, by Jadeware Corp.
 *
 *	ALL RIGHTS RESERVED
 */

#include "bsp2.h"
#include "bsp2math.h"
#include "bsp2algo.h"
#include "bsp2file.h"
#include "bsp2geo.h"
#include "bsp2list.h"
#include "bsp2solid.h"


void main(int argc, char **argv)
{
	double	StartTime = FloatTime();
	int		args;

	printf("\nBSP-MAKER [Version 1.37]\n");
	printf("(C) Copyright 2001 Jadeware Corp.\n");
	printf("=================================\n");

	if(argc < 2)
		Error("You miss to put an argument");

	menu_visibility = false;
	menu_nodetail = false;
	menu_OptimizeBSP = true;

	for(args=1 ; args<argc ; args++)
	{
		if(!strncmp(argv[args],"-v",2))
		{
			printf("Indoor Visibility Processing\n");
			menu_visibility = true;
		}
		if(!strncmp(argv[args],"-d",2))
		{
			printf("Ignoring all details\n");
			menu_nodetail = true;
		}
		if(!strncmp(argv[args],"-n",2))
		{
			printf("No BSP optimisations\n");
			menu_OptimizeBSP = false;
		}
	}

	if(!ProcessFile(argv[argc-1]))
		Error("Invalid Argument File");

	DispTimeElapsed(StartTime);
}

void Error(char *QuitMessage)
{
	if(QuitMessage)
		printf("%s\n",QuitMessage);
	printf("\7");
	exit(0);
}

void Rename(char *name, char *extension)
{
	int	Ptr=0;

	while(name[Ptr]!='.')
	{
		if(name[Ptr] == '\0')
			return;
		Ptr++;
	}
	strncpy(&name[Ptr+1],extension,3);
}

double FloatTime()
{
	time_t	t;
	
	time (&t);
	
	return t;
}

void DispTimeElapsed(double TimeStart)
{
	int	hour, min, sec;

	sec = (int)(FloatTime()-TimeStart);
	min = sec/60;
	sec %= 60;
	hour = min/60;
	min %= 60;
	printf("- %d:%d'%d -\n",hour,min,sec);
}

