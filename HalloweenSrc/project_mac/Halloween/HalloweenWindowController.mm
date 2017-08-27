/*
 Copyright (C) 2015 Apple Inc. All Rights Reserved.
 See LICENSE.txt for this sample’s licensing information
 
 Abstract:
 Window controller subclass.
 */

#import "HalloweenWindowController.h"
#import "HalloweenFullscreenWindow.h"
#import "KInput.h"

@interface HalloweenWindowController ()
{
    // Fullscreen window
    HalloweenFullscreenWindow *_fullscreenWindow;

    // Non-Fullscreen window (also the initial window)
    NSWindow* _standardWindow;
}
@end

@implementation HalloweenWindowController

- (instancetype)initWithWindow:(NSWindow *)window
{
    self = [super initWithWindow:window];

	if (self)
	{
		// Initialize to nil since it indicates app is not fullscreen
		_fullscreenWindow = nil;
		
		
//			trackingArea = [[NSTrackingArea alloc] initWithRect:[self.window bounds]
//														options: (NSTrackingMouseEnteredAndExited | NSTrackingMouseMoved | NSTrackingActiveInKeyWindow )
//														  owner:self userInfo:nil];
//			[self.window addTrackingArea:trackingArea];
		
    }

	return self;
}



- (void) goFullscreen
{
	// If app is already fullscreen...
	if(_fullscreenWindow)
	{
		//...don't do anything
		return;
	}

	// Allocate a new fullscreen window
	_fullscreenWindow = [[HalloweenFullscreenWindow alloc] init];

	// Resize the view to screensize
	NSRect viewRect = [_fullscreenWindow frame];

	// Set the view to the size of the fullscreen window
	[self.window.contentView setFrameSize: viewRect.size];

	// Set the view in the fullscreen window
	[_fullscreenWindow setContentView:self.window.contentView];

	_standardWindow = [self window];

	// Hide non-fullscreen window so it doesn't show up when switching out
	// of this app (i.e. with CMD-TAB)
	[_standardWindow orderOut:self];

	// Set controller to the fullscreen window so that all input will go to
	// this controller (self)
	[self setWindow:_fullscreenWindow];

	// Show the window and make it the key window for input
	[_fullscreenWindow makeKeyAndOrderFront:self];

}

- (void) goWindow
{
	// If controller doesn't have a full screen window...
	if(_fullscreenWindow == nil)
	{
		//...app is already windowed so don't do anything
		return;
	}

	// Get the rectangle of the original window
	NSRect viewRect = [_standardWindow frame];
	
	// Set the view rect to the new size
	[self.window.contentView setFrame:viewRect];

	// Set controller to the standard window so that all input will go to
	// this controller (self)
	[self setWindow:_standardWindow];

	// Set the content of the orginal window to the view
	[[self window] setContentView:_fullscreenWindow.contentView];

	// Show the window and make it the key window for input
	[[self window] makeKeyAndOrderFront:self];

	// Ensure we set fullscreen Window to nil so our checks for 
	// windowed vs. fullscreen mode elsewhere are correct
	_fullscreenWindow = nil;
}


- (void) keyUp:(NSEvent *)event
{
	unichar c = [[event charactersIgnoringModifiers] characterAtIndex:0];
	printf("keyup: %c %d\n", c, (int)c);
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
	//[super keyDown:event];
}

/*
- (void) keyDown:(NSEvent *)event
{
	unichar c = [[event charactersIgnoringModifiers] characterAtIndex:0];

	switch (c)
	{
		// Handle [ESC] key
		case 27:
			if(_fullscreenWindow != nil)
			{
				[self goWindow];
			}
			return;
		// Have f key toggle fullscreen
		case 'f':
			if(_fullscreenWindow == nil)
			{
				[self goFullscreen];
			}
			else
			{
				[self goWindow];
			}
			return;
	}

	// Allow other character to be handled (or not and beep)
	[super keyDown:event];
}
*/

/*
- (void) mouseMoved:(NSEvent *)event
{
	NSLog(@"mouseMoved");

}
*/

- (void) mouseUp:(NSEvent *)theEvent
{
	NSLog(@"mouseUp");
	KInput::setScreenReleased();
}

- (void) mouseDown:(NSEvent *)theEvent
{
	NSLog(@"mouseDown");
	KInput::setScreenPressed(1, KInput::getMouseX(), KInput::getMouseY());
}


@end
