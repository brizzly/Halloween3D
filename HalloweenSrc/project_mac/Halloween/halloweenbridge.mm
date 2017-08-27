

#include <AppKit/NSApplication.h>
#include "alloween.h"
#include "halloween_linux.h"

bool _init = false;
bool _ended = false;


void initGame()
{
	printf("initGame\n");
	
	//CGAssociateMouseAndMouseCursorPosition(false);
	//CGDisplayMoveCursorToPoint (kCGDirectMainDisplay, CGPointZero);

	
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
	if(!MACLoop()) {
		_ended = true;
		[NSApp terminate:nil];
	}
}

void cleanupGame()
{
	_ended = true;
	MACend();
	printf("cleanupGame\n");
}
