
#include "halloween_linux.h"
#include "alloween.cpp"


char    ErrorMessage[255];
hBool   done;
int     window;
char    NameText[260];
char    KeyText[260];
int     shareware;
hBool   gIsMultiplayer;
hBool   gIsServer;
int     gMultiMapID = 1;
hBool   gMainProcessDone;
int     shareware2;


void    sys_RestartVID()
{
}

int    sys_setVID()
{
	return true;
}

void    sys_killVID()
{
}

void    sys_GetKeyDatas()
{
}

void    sys_AntiPirate()
{
}

void    LoadingError(char *message)
{
}

void    key_CheckGame()
{
}

void    end()
{
}

void    w32_setGammaRamp(unsigned short *newgammaramp)
{
}

void    w32_setGamma(double gamma)
{
}



int main(int argc, char **argv)
{
	printf("Halloween, The new nightmare! V1.999 - Linux\n");
 
	/*
//	gIsMultiplayer = false;   
 	
	
//	done = false;
        if(!m_InitConsole())
	{
	  //   game_QuitMessage("m_InitConsole FAILED");
	     goto ending;
	}
	
	
	ProgramState = PS_LOADING;

	while(1)
	{
	//	done = GameProcess();
	//	if(done)
		{
	//		end();			
	//		return 1;
		}
	//	gl_SwapBuffer();
		
	}

ending:
	end();
	*/
	return 1;
}


