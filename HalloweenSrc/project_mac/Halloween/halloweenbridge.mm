

#include "alloween.h"
//#include "console.h"
#include "halloween_linux.h"



//extern void MACStarter();

bool _ended = false;


void initGame()
{
	printf("initGame\n");
	MACStarter();
	//m_ConsPrint("lol\n");
}

void loopGame()
{
	if(_ended) {
		return;
	}
	MACLoop();
	//printf("loopGame\n");
}

void cleanupGame()
{
	_ended = true;
	MACend();
	printf("cleanupGame\n");
}
