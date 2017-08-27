
#import "MainController.h"
#import "MyOpenGLView.h"
#import "Scene.h"

#import "KInput.h"


@implementation MainController

- (id) init
{
	if(self = [super init])
	{
		//[self performSelector:@selector(goFullScreenEx) withObject:nil afterDelay:0.5];
	}
	return self;
}

- (void) goFullScreenEx
{
	[self goFullScreen:nil];
}

- (IBAction) goFullScreen:(id)sender
{
	if(isInFullScreenMode == YES) {
		[self goWindow];
		return;
	}

	isInFullScreenMode = YES;
	
	// Pause the non-fullscreen view
	[openGLView stopAnimation];
	[openGLView setFullscreen:YES];
	[[openGLView window] setAcceptsMouseMovedEvents:YES];
	KInput::hidePointer();
	
	// Mac OS X 10.6 and later offer a simplified mechanism to create full-screen contexts
#if MAC_OS_X_VERSION_MIN_REQUIRED > MAC_OS_X_VERSION_10_5
	
	NSRect mainDisplayRect, viewRect;
	
	// Create a screen-sized window on the display you want to take over
	// Note, mainDisplayRect has a non-zero origin if the key window is on a secondary display
	mainDisplayRect = [[NSScreen mainScreen] frame];
	fullScreenWindow = [[NSWindow alloc] initWithContentRect:mainDisplayRect styleMask:NSBorderlessWindowMask 
													 backing:NSBackingStoreBuffered defer:YES];
	
	// Set the window level to be above the menu bar
	[fullScreenWindow setLevel:NSMainMenuWindowLevel+1];
	
	// Perform any other window configuration you desire
	[fullScreenWindow setOpaque:YES];
	[fullScreenWindow setHidesOnDeactivate:YES];
		
	// Create a view with a double-buffered OpenGL context and attach it to the window
	// By specifying the non-fullscreen context as the shareContext, we automatically inherit the OpenGL objects (textures, etc) it has defined
	viewRect = NSMakeRect(0.0, 0.0, mainDisplayRect.size.width, mainDisplayRect.size.height);
	fullScreenView = [[MyOpenGLView alloc] initWithFrame:viewRect shareContext:[openGLView openGLContext]];
	[fullScreenWindow setContentView:fullScreenView];
	
	// Show the window
	[fullScreenWindow makeKeyAndOrderFront:self];
	
	// Set the scene with the full-screen viewport and viewing transformation
	[scene setViewportRect:viewRect];
	
	// Assign the view's MainController to self
	[fullScreenView setMainController:self];
	
	if (!isAnimating) {
		// Mark the view as needing drawing to initalize its contents
		[fullScreenView setNeedsDisplay:YES];
	}
	else {
		// Start playing the animation
		[fullScreenView startAnimation];
	}
	
#else
	// Mac OS X 10.5 and eariler require additional work to capture the display and set up a special context
	// This demo uses CGL for full-screen rendering on pre-10.6 systems. You may also use NSOpenGL to achieve this.
	
	CGLPixelFormatObj pixelFormatObj;
	GLint numPixelFormats;
	
	// Capture the main display
	CGDisplayCapture(kCGDirectMainDisplay);
	
	// Set up an array of attributes
	CGLPixelFormatAttribute attribs[] = {
		
		// The full-screen attribute
		kCGLPFAFullScreen,
		
		// The display mask associated with the captured display
		// We may be on a multi-display system (and each screen may be driven by a different renderer), so we need to specify which screen we want to take over. For this demo, we'll specify the main screen.
		kCGLPFADisplayMask, CGDisplayIDToOpenGLDisplayMask(kCGDirectMainDisplay),
		
		// Attributes common to full-screen and non-fullscreen
		kCGLPFAAccelerated,
		kCGLPFANoRecovery,
		kCGLPFADoubleBuffer,
		kCGLPFAColorSize, 24,
		kCGLPFADepthSize, 16,
        0
    };
	
	// Create the full-screen context with the attributes listed above
	// By specifying the non-fullscreen context as the shareContext, we automatically inherit the OpenGL objects (textures, etc) it has defined
	CGLChoosePixelFormat(attribs, &pixelFormatObj, &numPixelFormats);
	CGLCreateContext(pixelFormatObj, [[openGLView openGLContext] CGLContextObj], &fullScreenContextObj);
	CGLDestroyPixelFormat(pixelFormatObj);
	
	if (!fullScreenContextObj) {
        NSLog(@"Failed to create full-screen context");
		CGReleaseAllDisplays();
		[self goWindow];
        return;
    }
	
	// Set the current context to the one to use for full-screen drawing
	CGLSetCurrentContext(fullScreenContextObj);
	
	// Attach a full-screen drawable object to the current context
	CGLSetFullScreen(fullScreenContextObj);
	
    // Lock us to the display's refresh rate
    GLint newSwapInterval = 1;
    CGLSetParameter(fullScreenContextObj, kCGLCPSwapInterval, &newSwapInterval);
	
	// Tell the scene the dimensions of the area it's going to render to, so it can set up an appropriate viewport and viewing transformation
    [scene setViewportRect:NSMakeRect(0, 0, CGDisplayPixelsWide(kCGDirectMainDisplay), CGDisplayPixelsHigh(kCGDirectMainDisplay))];
	
	// Perform the application's main loop until exiting full-screen
	// The shift here is from a model in which we passively receive events handed to us by the AppKit (in window mode)
	// to one in which we are actively driving event processing (in full-screen mode)
	while (isInFullScreenMode)
	{
		NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
		
		// Check for and process input events
        NSEvent *event;
        while (event = [NSApp nextEventMatchingMask:NSAnyEventMask untilDate:[NSDate distantPast] inMode:NSDefaultRunLoopMode dequeue:YES])
		{
            switch ([event type])
			{
                case NSLeftMouseDown:
                    [self mouseDown:event];
                    break;
					
                case NSLeftMouseUp:
                    [self mouseUp:event];
                    break;
					
                case NSLeftMouseDragged:
                    [self mouseDragged:event];
                    break;
					
                case NSKeyDown:
                    [self keyDown:event];
                    break;
					
				case NSMouseMoved:
					[self mouseDown:event];
					break;
					
                default:
                    break;
            }
        }
		
		// Update our animation
        CFAbsoluteTime currentTime = CFAbsoluteTimeGetCurrent();
        if (isAnimating) {
            [scene advanceTimeBy:(currentTime - renderTime)];
        }
        renderTime = currentTime;
		
		// Delegate to the scene object for rendering
		[scene render];
		CGLFlushDrawable(fullScreenContextObj);
		
		[pool release];
	}
	
#endif
}

- (void) goWindow
{
	if(isInFullScreenMode == NO) {
		return;
	}
	isInFullScreenMode = NO;
	
#if MAC_OS_X_VERSION_MIN_REQUIRED > MAC_OS_X_VERSION_10_5
	
	// Release the screen-sized window and view
	[fullScreenWindow release];
	[fullScreenView release];
	
#else
	
	// Set the current context to NULL
	CGLSetCurrentContext(NULL);
	// Clear the drawable object
	CGLClearDrawable(fullScreenContextObj);
	// Destroy the rendering context
	CGLDestroyContext(fullScreenContextObj);
	// Release the displays
	CGReleaseAllDisplays();
	
#endif
	
	// Switch to the non-fullscreen context
	[[openGLView openGLContext] makeCurrentContext];
	
	if (!isAnimating) {
		// Mark the view as needing drawing
		// The animation has advanced while we were in full-screen mode, so its current contents are stale
		[openGLView setNeedsDisplay:YES];
	}
	else {
		// Continue playing the animation
		[openGLView startAnimation];
	}
	
	[openGLView setFullscreen:NO];
	[openGLView updateTrackingAreas];
	KInput::showPointer();
}

- (void) awakeFromNib
{
	// Allocate the scene object
	scene = [[Scene alloc] init];
		
	// Assign the view's MainController to self
	[openGLView setMainController:self];
	
	// Activate the display link now
	[openGLView startAnimation];
	isAnimating = YES;
}

- (void) dealloc
{
	[scene release];
	[super dealloc];
}

- (Scene*) scene
{
	return scene;
}

- (CFAbsoluteTime) renderTime
{
	return renderTime;
}

- (void) setRenderTime:(CFAbsoluteTime)time
{
	renderTime = time;
}

- (void) startAnimation
{
	if (!isAnimating)
	{
		if (!isInFullScreenMode)
			[openGLView startAnimation];
#if MAC_OS_X_VERSION_MIN_REQUIRED > MAC_OS_X_VERSION_10_5
		else
			[fullScreenView startAnimation];
#endif
		isAnimating = YES;
	}
}

- (void) stopAnimation
{
	if (isAnimating)
	{
		if (!isInFullScreenMode)
			[openGLView stopAnimation];
#if MAC_OS_X_VERSION_MIN_REQUIRED > MAC_OS_X_VERSION_10_5
		else
			[fullScreenView stopAnimation];
#endif
		isAnimating = NO;
	}
}

- (void) toggleAnimation
{
	if (isAnimating)
		[self stopAnimation];
	else
		[self startAnimation];
}

- (void) keyUp:(NSEvent *)event
{
    unichar c = [[event charactersIgnoringModifiers] characterAtIndex:0];
	//printf("keyup: %c %d\n", c, (int)c);
	switch (c) 
	{
		case NSUpArrowFunctionKey:
			//NSLog(@"NSUpArrowFunctionKey");
			KInput::setKeyUnPressed(K_VK_UP);
			break;
			
		case NSDownArrowFunctionKey:
			//NSLog(@"NSDownArrowFunctionKey");
			KInput::setKeyUnPressed(K_VK_DOWN);
			break;
			
		case NSLeftArrowFunctionKey:
			//NSLog(@"NSLeftArrowFunctionKey");
			KInput::setKeyUnPressed(K_VK_LEFT);
			break;
			
		case NSRightArrowFunctionKey:
			//NSLog(@"NSRightArrowFunctionKey");
			KInput::setKeyUnPressed(K_VK_RIGHT);
			break;
		
		case 13:
			KInput::setKeyUnPressed(K_VK_RETURN);
			break;
			
		case 27:
			KInput::setKeyUnPressed(K_VK_ESCAPE);
			break;
			
		case 32:
			KInput::setKeyUnPressed(K_VK_SPACE);
			break;
			
		case 127:
			KInput::setKeyUnPressed(K_VK_BACK);
			break;
			
		default:
		{
			if('a' <= c && c <= 'z') {
				KInput::setKeyUnPressed(K_VK_A + c - 'a');
			}
			else if('A' <= c && c <= 'Z') {
				KInput::setKeyUnPressed(K_VK_A + c - 'A');
			}
			else if('0' <= c && c <= '9') {
				KInput::setKeyUnPressed(K_VK_0 + c - '0');
			}
		}
		break;
		
	}
}

- (void) keyDown:(NSEvent *)event
{
    unichar c = [[event charactersIgnoringModifiers] characterAtIndex:0];
	//printf("keydown: %c %d\n", c, (int)c);
	switch (c) 
	{
		case NSUpArrowFunctionKey:
			//NSLog(@"NSUpArrowFunctionKey");
			KInput::setKeyPressed(K_VK_UP);
			break;
			
		case NSDownArrowFunctionKey:
			//NSLog(@"NSDownArrowFunctionKey");
			KInput::setKeyPressed(K_VK_DOWN);
			break;
			
		case NSLeftArrowFunctionKey:
			//NSLog(@"NSLeftArrowFunctionKey");
			KInput::setKeyPressed(K_VK_LEFT);
			break;
			
		case NSRightArrowFunctionKey:
			//NSLog(@"NSRightArrowFunctionKey");
			KInput::setKeyPressed(K_VK_RIGHT);
			break;
			
		case 13:
			KInput::setKeyPressed(K_VK_RETURN);
			break;
			
		case 27:
			KInput::setKeyPressed(K_VK_ESCAPE);
			break;
			
		case 32:
			KInput::setKeyPressed(K_VK_SPACE);
			break;
			
		case 127:
			KInput::setKeyPressed(K_VK_BACK);
			break;
			
		default:
		{
			if('a' <= c && c <= 'z') {
				KInput::setKeyPressed(K_VK_A + c - 'a');
			}
			else if('A' <= c && c <= 'Z') {
				KInput::setKeyPressed(K_VK_A + c - 'A');
			}
			else if('0' <= c && c <= '9') {
				KInput::setKeyPressed(K_VK_0 + c - '0');
			}
		}
		break;
    }
}

- (void) flagsChanged:(NSEvent *)theEvent
{
	static unsigned int oldFlags = 0;
    unsigned int newFlags = [theEvent modifierFlags];
	if(newFlags & NSAlphaShiftKeyMask)
	{
		NSLog(@"Alpha");
	}
	else if(newFlags & NSShiftKeyMask)
	{
		NSLog(@"Shift");
		KInput::setKeyPressed(K_VK_R_SHIFT);
		KInput::setKeyPressed(K_VK_L_SHIFT);
	}
	else if(newFlags & NSControlKeyMask)
	{
		NSLog(@"Control");
		KInput::setKeyPressed(K_VK_R_CONTROL);
		KInput::setKeyPressed(K_VK_L_CONTROL);
	}
	else if(newFlags & NSAlternateKeyMask)
	{
		NSLog(@"Alt");
	}
	else if(newFlags & NSCommandKeyMask)
	{
		NSLog(@"Command");
	}
	else if(oldFlags)
	{
		NSLog(@"unpressed flag ");
		if(oldFlags & NSShiftKeyMask)
		{
			NSLog(@"Shift");
			KInput::setKeyUnPressed(K_VK_R_SHIFT);
			KInput::setKeyUnPressed(K_VK_L_SHIFT);
		}
		else if(oldFlags & NSControlKeyMask)
		{
			NSLog(@"Control");
			KInput::setKeyUnPressed(K_VK_R_CONTROL);
			KInput::setKeyUnPressed(K_VK_L_CONTROL);
		}
		newFlags = 0;
	}
	oldFlags = newFlags;
}	

- (void) mouseUp:(NSEvent *)theEvent
{
	KInput::setScreenReleased();
}

- (void) mouseDown:(NSEvent *)theEvent
{
    BOOL dragging = YES;
    NSPoint windowPoint;	
	NSPoint lastWindowPoint;
	if(isInFullScreenMode == YES)
	{
		lastWindowPoint = [NSEvent mouseLocation];
	}
	else
	{
		lastWindowPoint = [theEvent locationInWindow];
	}
	
	KInput::setScreenPressed(1, KInput::getMouseX(), KInput::getMouseY());
	
	//NSLog(@"Type %d", [theEvent type]);
    while (dragging)
	{
        theEvent = [[openGLView window] nextEventMatchingMask:NSLeftMouseUpMask | NSLeftMouseDraggedMask];
		if(isInFullScreenMode == YES)
		{
			windowPoint = [NSEvent mouseLocation];
		}
		else
		{
			windowPoint = [theEvent locationInWindow];
        }

		switch ([theEvent type])
		{
			//NSLog(@"Type2 %d", [theEvent type]);
				
			case NSLeftMouseDown:
				break;
				
            case NSLeftMouseUp:
                dragging = NO;
                break;
				
            case NSLeftMouseDragged:
                lastWindowPoint = windowPoint;
				
				[openGLView mouseMoved:theEvent];				
				
				if (isInFullScreenMode) {
#if MAC_OS_X_VERSION_MIN_REQUIRED > MAC_OS_X_VERSION_10_5
					[fullScreenView display];
#else
                    [scene render];
                    CGLFlushDrawable(fullScreenContextObj);
#endif
                }
				else {
					[openGLView display];
				}
				 
                break;
				
            default:
                break;
        }
    }
	
	[self mouseUp:theEvent];
}

- (void) setMouseMoved:(NSPoint)pos
{
	if(isInFullScreenMode)
	{
		NSRect mainDisplayRect = [[NSScreen mainScreen] frame];
		float sw = (float)KInput::getScreenWidth();
		float sh = (float)KInput::getScreenHeight();
		float rx = sw / mainDisplayRect.size.width;
		float ry = sh / mainDisplayRect.size.height;
		pos.x *= rx;
		pos.y *= ry;		
	}
	pos.y = (float)KInput::getScreenHeight() - pos.y;
	KInput::setScreenMoving((int)pos.x, (int)pos.y);
}

- (void) enterWindow
{
	KInput::hidePointer();
}

- (void) exitWindow
{
	KInput::showPointer();
}

- (BOOL) applicationShouldTerminateAfterLastWindowClosed:(NSApplication *)theApplication
{
    return YES;
}






@end
