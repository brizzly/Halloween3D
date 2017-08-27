
#import "MyOpenGLView.h"
#import "MainController.h"
#import "Scene.h"
#import "KInput.h"

@implementation MyOpenGLView




- (NSOpenGLContext*) openGLContext
{
	return openGLContext;
}

- (NSOpenGLPixelFormat*) pixelFormat
{
	return pixelFormat;
}

- (void) setMainController:(MainController*)theController;
{
	controller = theController;
}

- (CVReturn) getFrameForTime:(const CVTimeStamp*)outputTime
{
	// There is no autorelease pool when this method is called because it will be called from a background thread
	// It's important to create one or you will leak objects
	NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
	
	// Update the animation
	CFAbsoluteTime currentTime = CFAbsoluteTimeGetCurrent();
	[[controller scene] advanceTimeBy:(currentTime - [controller renderTime])];
	[controller setRenderTime:currentTime];
	
	[self drawView];
	
	[pool release];
    return kCVReturnSuccess;
}

// This is the renderer output callback function
static CVReturn MyDisplayLinkCallback(CVDisplayLinkRef displayLink, const CVTimeStamp* now, const CVTimeStamp* outputTime, CVOptionFlags flagsIn, CVOptionFlags* flagsOut, void* displayLinkContext)
{
    CVReturn result = [(MyOpenGLView*)displayLinkContext getFrameForTime:outputTime];
    return result;
}

- (void) setupDisplayLink
{
	// Create a display link capable of being used with all active displays
	CVDisplayLinkCreateWithActiveCGDisplays(&displayLink);
	
	// Set the renderer output callback function
	CVDisplayLinkSetOutputCallback(displayLink, &MyDisplayLinkCallback, self);
	
	// Set the display link for the current renderer
	CGLContextObj cglContext = [[self openGLContext] CGLContextObj];
	CGLPixelFormatObj cglPixelFormat = [[self pixelFormat] CGLPixelFormatObj];
	CVDisplayLinkSetCurrentCGDisplayFromOpenGLContext(displayLink, cglContext, cglPixelFormat);
}

- (void) setFullscreen:(Boolean)value { 
	_fullscreen = value;
}

- (id) initWithFrame:(NSRect)frameRect
{
	trackingArea = nil;
	_mouseInside = NO;
	_fullscreen = NO;
	_hitRect = [self bounds];
	if(self = [self initWithFrame:frameRect shareContext:nil])
	{
	}
	return self;
}

- (id) initWithFrame:(NSRect)frameRect shareContext:(NSOpenGLContext*)context
{
    NSOpenGLPixelFormatAttribute attribs[] =
    {
		kCGLPFAAccelerated,
		kCGLPFANoRecovery,
		kCGLPFADoubleBuffer,
	//	kCGLPFAColorSize, 24,
		kCGLPFADepthSize, 24,
		0
    };
	
    pixelFormat = [[NSOpenGLPixelFormat alloc] initWithAttributes:attribs];
	
    if (!pixelFormat)
		NSLog(@"No OpenGL pixel format");
	
	// NSOpenGLView does not handle context sharing, so we draw to a custom NSView instead
	openGLContext = [[NSOpenGLContext alloc] initWithFormat:pixelFormat shareContext:context];
	
	if (self = [super initWithFrame:frameRect]) 
	{
		[[self openGLContext] makeCurrentContext];
		
		// Synchronize buffer swaps with vertical refresh rate
		GLint swapInt = 1;
		[[self openGLContext] setValues:&swapInt forParameter:NSOpenGLCPSwapInterval]; 
		
		[self setupDisplayLink];
		
		// Look for changes in view size
		// Note, -reshape will not be called automatically on size changes because NSView does not export it to override 
		[[NSNotificationCenter defaultCenter] addObserver:self 
												 selector:@selector(reshape) 
													 name:NSViewGlobalFrameDidChangeNotification
												   object:self];
	}
	
	if(trackingArea == nil)
	{
		//NSRect r = [self bounds];
		//NSLog(@"debug", r);
		
		trackingArea = [[NSTrackingArea alloc] initWithRect:[self bounds]
												options: (NSTrackingMouseEnteredAndExited | NSTrackingMouseMoved | NSTrackingActiveInKeyWindow )
												  owner:self userInfo:nil];
		[self addTrackingArea:trackingArea];
	}
	
	return self;
}

- (void) updateTrackingAreas 
{
	NSLog(@"updateTrackingAreas");
 	if(trackingArea)
	{
		[self removeTrackingArea:trackingArea];
		[trackingArea release];
	}
 	_hitRect = [self bounds];
	NSLog(@"updateTrackingAreas bounds %f %f %f %f", _hitRect.origin.x, _hitRect.origin.y, _hitRect.size.width, _hitRect.size.height);
	
    trackingArea = [[NSTrackingArea alloc] initWithRect:[self bounds]
												options: (NSTrackingMouseEnteredAndExited | NSTrackingMouseMoved | NSTrackingActiveInKeyWindow)
												  owner:self userInfo:nil];
    [self addTrackingArea:trackingArea];
}

- (void) lockFocus
{
	[super lockFocus];
	if ([[self openGLContext] view] != self)
		[[self openGLContext] setView:self];
}

- (void) reshape
{
	// This method will be called on the main thread when resizing, but we may be drawing on a secondary thread through the display link
	// Add a mutex around to avoid the threads accessing the context simultaneously
	CGLLockContext([[self openGLContext] CGLContextObj]);
	
	// Delegate to the scene object to update for a change in the view size
	[[controller scene] setViewportRect:[self bounds]];
	[[self openGLContext] update];
	
	CGLUnlockContext([[self openGLContext] CGLContextObj]);
}

- (void) drawRect:(NSRect)dirtyRect
{
	// Ignore if the display link is still running
	if (!CVDisplayLinkIsRunning(displayLink))
		[self drawView];
}

- (void) drawView
{
	// This method will be called on both the main thread (through -drawRect:) and a secondary thread (through the display link rendering loop)
	// Also, when resizing the view, -reshape is called on the main thread, but we may be drawing on a secondary thread
	// Add a mutex around to avoid the threads accessing the context simultaneously
	CGLLockContext([[self openGLContext] CGLContextObj]);
	
	// Make sure we draw to the right context
	[[self openGLContext] makeCurrentContext];
	
	// Delegate to the scene object for rendering
    [[controller scene] render];
	[[self openGLContext] flushBuffer];
	
	CGLUnlockContext([[self openGLContext] CGLContextObj]);
}
 
- (BOOL) acceptsFirstResponder
{
    // We want this view to be able to receive key events
    return YES;
}

- (BOOL) acceptsMouseMovedEvents
{
	return YES;
}

- (void) mouseEntered:(NSEvent *)theEvent 
{
	NSLog(@"mouse entered");
	[controller enterWindow];
}

- (void) mouseMoved:(NSEvent *)theEvent 
{
	NSPoint eyeCenter;
	if(_fullscreen == YES)
	{
		eyeCenter = [NSEvent mouseLocation];
	}
	else 
	{
		eyeCenter = [self convertPoint:[theEvent locationInWindow] fromView:nil];
		//NSLog(@"mouseMoved bounds %f %f %f %f - %f %f", _hitRect.origin.x, _hitRect.origin.y, _hitRect.size.width, _hitRect.size.height, eyeCenter.x, eyeCenter.y);			
		if(eyeCenter.x < _hitRect.origin.x || eyeCenter.x > _hitRect.origin.x+_hitRect.size.width || 
		   eyeCenter.y < _hitRect.origin.y || eyeCenter.y > _hitRect.origin.y+_hitRect.size.height)
		{
			return;
		}
	}
	
	KInput::setScreenMoving((int)eyeCenter.x, (int)eyeCenter.y);
	
	[controller setMouseMoved:eyeCenter];
}

- (void) mouseExited:(NSEvent *)theEvent 
{
	NSLog(@"mouse exited");
	[controller exitWindow];
}


- (void) keyUp:(NSEvent *)theEvent
{
    [controller keyUp:theEvent];
}

- (void) keyDown:(NSEvent *)theEvent
{
    [controller keyDown:theEvent];
}

- (void) flagsChanged:(NSEvent *)event
{
	[controller flagsChanged:event];
}

- (void) mouseUp:(NSEvent *)theEvent
{
}

- (void) mouseDown:(NSEvent *)theEvent
{
    [controller mouseDown:theEvent];
	[super mouseDown:theEvent];
}

- (void) startAnimation
{
	if (displayLink && !CVDisplayLinkIsRunning(displayLink))
		CVDisplayLinkStart(displayLink);
}

- (void) stopAnimation
{
	if (displayLink && CVDisplayLinkIsRunning(displayLink))
		CVDisplayLinkStop(displayLink);
}

- (void) dealloc
{
	// Stop and release the display link
	CVDisplayLinkStop(displayLink);
    CVDisplayLinkRelease(displayLink);
	
	// Destroy the context
	[openGLContext release];
	[pixelFormat release];
	
	[[NSNotificationCenter defaultCenter] removeObserver:self 
													name:NSViewGlobalFrameDidChangeNotification
												  object:self];
	[super dealloc];
}	

@end
