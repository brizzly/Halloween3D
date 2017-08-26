#include "alloween.h"

//==================== GLOBALS ========================
//=====================================================

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

FILE	*gDemoFile;


void demo_RecordInit()
{
	char	path[256];

	sprintf(path,"%s%s%s",SYSDIR,DEMDIR,"demo1.dem");

	remove(path);
	gDemoFile = fopen(path,"a");
}

void demo_Clean()
{
	if(!gDemoFile)
		return;
	fclose(gDemoFile);
}

void demo_RecordAddKey(char key)
{
	if(!gDemoFile)
		return;
	fprintf(gDemoFile,"%c ",key);
}


void demo_PlayInit()
{
	char	path[256];

	sprintf(path,"%s%s%s",SYSDIR,DEMDIR,"demo1.dem");

	gDemoFile = fopen(path,"r");
}

char demo_PlayReadKey()
{
	char	key;

	if(!gDemoFile)
		return 0;
	fscanf(gDemoFile,"%c ",&key);
	if(feof(gDemoFile))
	{
		play.value = 0;
		m_ConsPrint("fin de demo.\n");
	}
	return key;
}


