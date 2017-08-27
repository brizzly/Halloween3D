
#import "Scene.h"
#import <OpenGL/glu.h>

#import "halloweenbridge.h"

@implementation Scene

- (id) init
{
    if (self = [super init]) 
	{
        animationPhase = 0;
		initGame();
	} 
    return self;
}

- (void) dealloc
{
	cleanupGame();
	[super dealloc];
}

- (void) advanceTimeBy:(float)seconds
{
    float phaseDelta = seconds - floor(seconds);
    float newAnimationPhase = animationPhase + 0.015625 * phaseDelta;
    newAnimationPhase = newAnimationPhase - floor(newAnimationPhase);
    [self setAnimationPhase:newAnimationPhase];
}

- (void) setAnimationPhase:(float)newAnimationPhase
{
    animationPhase = newAnimationPhase;
}

- (void) setViewportRect:(NSRect)bounds
{
	int x_offset = 0;
	int y_offset = 0;
	int w = bounds.size.width;
	int h = bounds.size.height;
	float currentRatio = bounds.size.width / bounds.size.height;
	float ratio = 4.0f / 3.0f; // (float)ScreenX / (float)ScreenY;
	if(currentRatio > ratio)
	{
		w = bounds.size.height * ratio;
		x_offset = (bounds.size.width - w)/2;
	}
	else if(currentRatio < ratio)
	{
		h = bounds.size.width / ratio;
		y_offset = (bounds.size.height - h)/2;
	}	
	glViewport(x_offset, y_offset, w, h);
}

- (void) render
{
	loopGame();
}

@end
