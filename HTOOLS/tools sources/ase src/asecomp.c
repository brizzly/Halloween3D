/*
 *
 *	ASE compiler - Radiosity Compiler for lightmaps
 *	(C) Jadeware 2001-2002. ALL RIGHTS RESERVED
 *
 */

#include "asecomp.h"
#include "asereader.h"
#include "asetool.h"
#include "asemath.h"


int filesize(FILE *file)
{
	int size=0;

	while(fgetc(file)!=EOF)
	{
		size++;
	}
	rewind(file);
	return size;
}

void Error(char *QuitMessage, char *data)
{
	if(QuitMessage)
		printf("%s",QuitMessage);
	printf("\7");

	exit(0);
	Quit(data);
}

void Quit(char *data)
{
	printf("\7");
	free(data);
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

void main(int nb_arg, char **argv)
{
	int		args;
	char	*data;
	char	name[255];
	bool	hValue;
	int		Removed=0;
	int		k=0;

	printf("\nASECOMP [%s]\n","Version 1.27");
	printf("(C) Copyright 2001 Jadeware Corp.\n");
	printf("=================================\n");

	menu_makemap=true;
	menu_groupface=false;
	
	data=(char*)malloc(MAX_STRING_LENGTH);
	if(!data)
	{
		printf("Memory allocation failed\n");
		Quit(data);
	}
	memset(data,0,MAX_STRING_LENGTH);

	if(nb_arg<2)
	{
		printf("Mettre en paramettre un fichier ASE.\n");
		Quit(data);
	}

	for(args=1 ; args<nb_arg ; args++)
	{
		
		if(!strncmp(argv[args],"-o",2))
		{
	//		menu_makemap=false;
		}
	}

	Nb_Lights=0;

	strcpy(name,(char*)argv[nb_arg-1]);
	hValue=f_ChargerASE(name,0);
	if(!hValue)
	{
		printf("Erreur avec le fichier: %s\n",name);
		Quit(data);
	}

	if(menu_makemap)
		printf("Making Polygon Map\n");
	else
		printf("Making Entity/Model\n");

	gNb_Faces=0;
	gNb_Brush=0;
	Nb_textures=0;
	Nb_entities=0;
	Nb_object=0;

	gPolyList = Load_XYZ(name,data);
//	gPolyList = AddBoundFaces(gPolyList);
	PutPolyToFile(name,gPolyList);
	PrintMemory();

	Rename(name,"xyz");
	remove(name);

	Quit(data);
}

void PrintMemory()
{
	FILE	*file;

	if(!menu_makemap)
		gNb_Brush = 0;

	gNb_Faces = CountFaces(gPolyList);

	printf("==============\n");
	//if(gNb_ActionBrush != -4)
		printf("%5d ACTIONBRUSH\n",gNb_ActionBrush);
	if(gMonster1)
		printf("%5d MONSTER\n",gMonster1);
	if(gAmmo1)
		printf("%5d AMO1\n",gAmmo1);
	if(gAmmo2)
		printf("%5d AMO2\n",gAmmo2);
	if(gAmmo3)
		printf("%5d AMO3\n",gAmmo3);
	if(gPumpkin)
		printf("%5d PUMPKIN\n",gPumpkin);
	if(gLampe)
		printf("%5d LAMPE\n",gLampe);
	if(gLettre)
		printf("%5d LETTER\n",gLettre);
	if(gFiole)
		printf("%5d FIOLE\n",gFiole);
	if(gGun1)
		printf("%5d GUN1\n",gGun1);
	if(gGun2)
		printf("%5d GUN2\n",gGun2);
	if(gGun3)
		printf("%5d GUN3\n",gGun3);
	if(gLava_count)
		printf("%5d LAVA\n",gLava_count);
	if(gInvisible)
		printf("%5d INVISIBLE\n",gInvisible);
	if(gDetail)
		printf("%5d DETAIL\n",gDetail);
	if(gNobsp_count)
		printf("%5d NOBSP\n",gNobsp_count);
	if(gNb_Brush)
		printf("%5d BRUSH\n",gNb_Brush);
	if(Nb_entities)
		printf("%5d ENTITIES\n",Nb_entities);
	if(Nb_Textures_Loaded)
		printf("%5d TEXTURES\n",Nb_Textures_Loaded);
	if(Nb_Lights)
		printf("%5d LIGHTS\n",Nb_Lights);
	printf("%5d FACES\n",gNb_Faces);
	printf("==============\n");

	file = fopen("output.txt","w");
	//if(gNb_ActionBrush != -4)
		fprintf(file,"%5d ACTIONBRUSH\n",gNb_ActionBrush);
	if(gMonster1)
		fprintf(file,"%5d MONSTER\n",gMonster1);
	if(gAmmo1)
		fprintf(file,"%5d AMO1\n",gAmmo1);
	if(gAmmo2)
		fprintf(file,"%5d AMO2\n",gAmmo2);
	if(gAmmo3)
		fprintf(file,"%5d AMO3\n",gAmmo3);
	if(gPumpkin)
		fprintf(file,"%5d PUMPKIN\n",gPumpkin);
	if(gLampe)
		fprintf(file,"%5d LAMPE\n",gLampe);
	if(gLettre)
		fprintf(file,"%5d LETTER\n",gLettre);
	if(gFiole)
		fprintf(file,"%5d FIOLE\n",gFiole);
	if(gGun1)
		fprintf(file,"%5d GUN1\n",gGun1);
	if(gGun2)
		fprintf(file,"%5d GUN2\n",gGun2);
	if(gGun3)
		fprintf(file,"%5d GUN3\n",gGun3);
	if(gLava_count)
		fprintf(file,"%5d LAVA\n",gLava_count);
	if(gInvisible)
		fprintf(file,"%5d INVISIBLE\n",gInvisible);
	if(gDetail)
		fprintf(file,"%5d DETAIL\n",gDetail);
	if(gNobsp_count)
		fprintf(file,"%5d NOBSP\n",gNobsp_count);
	if(gNb_Brush)
		fprintf(file,"%5d BRUSH\n",gNb_Brush);
	if(Nb_entities)
		fprintf(file,"%5d ENTITIES\n",Nb_entities);
	if(Nb_Textures_Loaded)
		fprintf(file,"%5d TEXTURES\n",Nb_Textures_Loaded);
	fprintf(file,"%5d FACES\n",gNb_Faces);
	if(Nb_Lights)
		fprintf(file,"%5d LIGHTS\n",Nb_Lights);
	fclose(file);
}
