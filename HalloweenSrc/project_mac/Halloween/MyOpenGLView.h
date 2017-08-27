
#import <Cocoa/Cocoa.h>
#import <QuartzCore/CVDisplayLink.h>

@class MainController;

@interface MyOpenGLView : NSView 
{	
	NSOpenGLContext *openGLContext;
	NSOpenGLPixelFormat *pixelFormat;
	
	NSTrackingArea * trackingArea;
	Boolean			_mouseInside;
	Boolean			_fullscreen;
	NSRect			_hitRect;
	
	MainController *controller;
	
	CVDisplayLinkRef displayLink;
	BOOL isAnimating;
}

- (void) setFullscreen:(Boolean)value;

- (id) initWithFrame:(NSRect)frameRect;
- (id) initWithFrame:(NSRect)frameRect shareContext:(NSOpenGLContext*)context;

- (NSOpenGLContext*) openGLContext;

- (void) setMainController:(MainController*)theController;
- (void) drawView;
- (void) startAnimation;
- (void) stopAnimation;

@end
