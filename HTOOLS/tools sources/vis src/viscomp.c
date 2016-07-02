
#include "viscomp.h"
#include "bspfile.h"
#include "maths.h"
#include "pvs.h"

void main(int argc, char *argv[])
{
	Nb_portals=0;
	Nb_leafs=0;
	Nb_faces=0;
	Nb_entities=0;
	Nb_textures=0;
	Nb_Lights=0;
	Nb_nodes=0;

	printf("\nVISCOMP [Version 1.11]\n");
	printf("(C) Copyright 2001 Jadeware Corp.\n");
	printf("=================================\n");

	if(argc<2)
		Quit("Mettre en paramettre un fichier *.MAP.\n");

	strcpy(name,argv[1]);

	if(!LoadBSPFromFile(name))
		Quit("Not a valid BSP file\n");
	
//	PrintMemory();

	TimeStart = FloatTime();

	PVSData=malloc(Nb_leafs*Nb_leafs);
	memset(PVSData,'2',Nb_leafs*Nb_leafs);
	CalculatePVS();

	WritePVS(name);

	Rename(name,"map");
	remove(name);
	strcpy(tempname,name);
	Rename(tempname,"tmp");
	rename(tempname,name);

	PrintMemory();

	DispTimeElapsed(TimeStart);
	Quit(NULL);
}

void Quit(char *QuitMessage)
{
	if(QuitMessage)
		printf("%s",QuitMessage);
	printf("\7");

	#ifdef AUTO
	while(true);
	#endif
	exit(0);
}

void PrintMemory()
{
	FILE *file;

	printf("==============\n");
	printf("%5d FACES\n",Nb_faces);
	printf("%5d LEAFS\n",Nb_leafs);
	printf("%5d PORTALS\n",Nb_portals);
	printf("%5d ENTITIES\n",Nb_entities);
	printf("%5d TEXTURES\n",Nb_textures);
	printf("%5d LIGHTS\n",Nb_Lights);
	printf("==============\n");

	file = fopen("output.txt","w");
	fprintf(file,"%5d FACES\n",Nb_faces);
	fprintf(file,"%5d LEAFS\n",Nb_leafs);
	fprintf(file,"%5d PORTALS\n",Nb_portals);
	fprintf(file,"%5d ENTITIES\n",Nb_entities);
	fprintf(file,"%5d TEXTURES\n",Nb_textures);
	fprintf(file,"%5d LIGHTS\n",Nb_Lights);
	fclose(file);
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
