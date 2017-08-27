

#include "alloween.h"
//#include "console.h"
#include "halloween_linux.h"



//extern void MACStarter();

bool _init = false;
bool _ended = false;


void initGame()
{
	printf("initGame\n");
	MACStarter();
	_init = true;
}

void loopGame()
{
	if(!_init) {
		return;
	}
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
