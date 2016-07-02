#ifndef __ASECOMP__
#define	__ASECOMP__

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef enum { false, true } bool;

#define	MAX_STRING_LENGTH	8000000
#define	true				1
#define	false				0
#define	EPSILON				0.00001f //0.01f
#define	EQUAL_EPSILON		0.00001f //0.001f
#define	CONFONDU			0
#define	DEVANT				1
#define	DERRIERE			2
#define	AUMILIEU			3
#define	TEXTURENAMEMAX		255
#define TEXTURELOADED		255

extern int	gVertice;
extern int	gNobsp_count;
extern int	gLava_count;
extern int	gInvisible;
extern int	gGun1;
extern int	gGun2;
extern int	gGun3;
extern int	gMonster1;
extern int	gAmmo1;
extern int	gAmmo2;
extern int	gAmmo3;
extern int	gPumpkin;
extern int	gFiole;
extern int	gLettre;
extern int	gLampe;
extern int	gDetail;
extern int	Nb_Textures_Loaded;

bool	menu_makemap;
bool	menu_groupface;
int		gNb_Faces;
int		gNb_Brush;
int		gNb_ActionBrush;
int		Nb_textures;
int		Nb_entities;
int		Nb_object;
int		Nb_Lights;
char	TextureList2[TEXTURELOADED][TEXTURENAMEMAX];
float	gStartX,gStartY,gStartZ;
int		PlayerAngle;
int		ActionNumber;


void Error(char *QuitMessage, char *data);
void Quit(char *data);
void PrintMemory();
void Rename(char *name, char *extension);

#endif __ASECOMP__
