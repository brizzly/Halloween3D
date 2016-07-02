
#include "alloween.cpp"


int		gTextOffset;
char	*gStoryText = NULL;
float	gStoryDisp_time;
int		gLastStoryCharPos;
hBool	gIsWaitingForNext;
hBool	gTextIsDone;
char	*StoryTextes[] = 
{
//	"J'aurais jamais du accepter ce pari..@ Ces ruines ressemblent a rien, trop trop malsain ici..@ C'est quoi ce truc brillant devant moi, on dirait genre une lettre en 3D comme dans le jeu Tonyhawk, c'est quoi cette blague encore.. ?@\n\n ..je chie dans mon bene !",
	"Welcome to Halloween !@ Grab the H-A-L-L-O-W-E-E-N letters and follow you destiny.\n\n DeeePLAh.",
	"Sale Dep.@ Hahaha\n enoooooooooorme!"
};


void Story_SetText(int TextID)
{
	gTextIsDone = false;
	gIsWaitingForNext = false;
	gTextOffset = 0;
	gStoryDisp_time = 0;
	gLastStoryCharPos = 0;
	gStoryText = StoryTextes[TextID];
}

void Story_NextText()
{
	if(gTextIsDone)
	{
		gStoryText = NULL;
		return;
	}

	if(gIsWaitingForNext)
	{
		gStoryDisp_time = 0;
		gTextOffset = gLastStoryCharPos;
		gIsWaitingForNext = false;
	}
}

void Story_Process()
{
	if(gIsMultiplayer)
		return;
	if(!gStoryText)
		return;

	gStoryDisp_time += gFrameTime_msec;

	if(gMapIntro)
	{
		if(game_IsMapOpened(1) == false)
		{
			Story_DispCadreAlpha();
			Story_DispString(16, 23, 6, (int)(gStoryDisp_time/50.0f), gStoryText);
		}
	}
}

void Story_DispCadreAlpha()
{
	disp_DrawSprite2Aplha(txid_OMBRAGE, 100, 30, 600, 120, true, 0.3f);
}

void Story_DispString(int format, int ligne, int col, int nCar, char *string)
{
	int		xpos;
	int		ypos;
	Color_t	color;

	if(!string)
		return;
	if(nCar <= 0)
		return;

	xpos = (int)(col*format);
	ypos = (int)(480.0f-(ligne+1)*format);
	gFontSize = (int)(format);
	color.R = color.G = color.B = color.A = 1;
	Story_DispN(color, xpos, ypos, 0, 0, string, nCar);
}

void Story_DispN(Color_t color, int xpos, int ypos, int xoff, int yoff, char *s, int N)
{
	int x = xpos;
	int y = ypos;
	int	ind;
	int	car;
	int carrage = 0;
	int	index;
	int	nCar;
	int	nLine;
	int	maxCar;
	int	TailleMot;
	int	LastSpace;
	int	OffsetDebut;
	
	NormalFont.fsize = gFontSize;
	NormalFont.ftextid = txid_FONT;
	NormalFont.OrthoProjX = 640;
	NormalFont.OrthoProjY = 480;
    carrage = g_fontGetCharHits(s, '\n');

	maxCar = 50;
	LastSpace = 0;
	nCar = 0;
	nLine = 1;

	// cherche l'offset de debut de ligne selon scroll du joueur
	OffsetDebut = gTextOffset;

	for(index=OffsetDebut ; s[index] ; index++, x += (int)(NormalFont.fsize/HACKFONT))
	{
		if(index >= N+OffsetDebut)
			break;

		// calcul la taille du prochain mot
		ind = 0;
		TailleMot = 0;
		while(1)
		{
			car = s[LastSpace+ind+1];
			if(	car == ' ' || car == '\n' || car == '@' || car == 0)
			{
				TailleMot = ind;
				break;
			}			
			ind++;
		}

		// si la phrase ou le mot va deborder on fait un retour a la ligne
		if(s[index] == ' ' || s[index] == '\n' || s[index] == '@')
		if(nCar + TailleMot >= maxCar)
		{
			nLine++;
			nCar = 0;
			y -= NormalFont.fsize;
            x = xpos - (int)(NormalFont.fsize/HACKFONT);
			continue;
		}

		if(s[index] == ' ' || s[index] == '\n' || s[index] == '@')
			LastSpace = index;

		// si le premier caractere d'une ligne est un espace on le saute
		if(nCar == 0)
		{
			if(s[index] == ' ')
			{
				nCar++;
				continue;
			}
		}

		// si ya trop de lignes 
		if(nLine >= 6 || s[index] == '@')
		{
			if(nLine >= 6)
				gLastStoryCharPos = TailleMot + index;
			else if(s[index] == '@')
				gLastStoryCharPos = index+1;

			if(s[gLastStoryCharPos] == ' ')
				gLastStoryCharPos++;

			gIsWaitingForNext = true;
			if(disp_GetClock() % 800 < 400)
			{
				g_DispCenterString(14, 33, "Press Action Key");
			}
			return;
		}

		// si le texte est fini
		if(s[index+1] == 0)
		{
			gTextIsDone = true;
		}

		switch(s[index])
		{
			case '\n':
			nLine++;
			nCar = 0;
			y -= NormalFont.fsize;
            x = xpos - (int)(NormalFont.fsize/HACKFONT);
			continue;

            default:
			nCar++;
			disp_CharFont(color, NormalFont, s[index], x+xoff, y+yoff);
            break;
		}
	}
}



