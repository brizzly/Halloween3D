
#import <Cocoa/Cocoa.h>

@interface Scene : NSObject 
{	
   float animationPhase;
}

- (id)init;
- (void)setViewportRect:(NSRect)bounds;
- (void)render;
- (void)advanceTimeBy:(float)seconds;
- (void)setAnimationPhase:(float)newAnimationPhase;

@end
