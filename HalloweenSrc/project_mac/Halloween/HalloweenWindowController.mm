/*
 Copyright (C) 2015 Apple Inc. All Rights Reserved.
 See LICENSE.txt for this sample’s licensing information
 
 Abstract:
 Window controller subclass.
 */

#import "HalloweenWindowController.h"


@implementation HalloweenWindowController



- (void) windowDidLoad
{
	[super windowDidLoad];
	
	_fullscreen = NO;
	
	_standardWindow = [[HalloweenNormalscreenWindow alloc] init];
	_fullscreenWindow = nil;
	
	[self setWindow:_standardWindow];

	[self goFullscreen];
	
	//[self performSelector:@selector(goFullscreen) withObject:nil afterDelay:1];
}

- (void) goFullscreen
{
	if(_fullscreen) {
		return;
	}
	_fullscreen = YES;

	// Allocate a new fullscreen window
	_fullscreenWindow = [[HalloweenFullscreenWindow alloc] init];

	// Resize the view to screensize
	NSRect viewRect = [_fullscreenWindow frame];

	// Set the view to the size of the fullscreen window
	[self.window.contentView setFrameSize: viewRect.size];

	// Set the view in the fullscreen window
	[_fullscreenWindow setContentView:self.window.contentView];

	//_standardWindow = [self window];
	_standardRect = [_standardWindow frame];

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
	if(!_fullscreen) {
		return;
	}
	_fullscreen = NO;

	// Set controller to the standard window so that all input will go to this controller (self)
	//_standardWindow = [self window];
	[self setWindow:_standardWindow];

	
	// Get the rectangle of the original window
	NSRect viewRect = _standardRect; // [_standardWindow frame];
//	NSRect viewRect = [_standardWindow frame];
	[self.window.contentView setFrame:viewRect];


	// Set the content of the orginal window to the view
	[[self window] setContentView:_fullscreenWindow.contentView];

	// Show the window and make it the key window for input
	[[self window] makeKeyAndOrderFront:self];

	// Ensure we set fullscreen Window to nil so our checks for 
	// windowed vs. fullscreen mode elsewhere are correct
	//_fullscreenWindow = nil;
	
	[_fullscreenWindow release];
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
			
		case 9:
			KInput::setKeyUnPressed(K_VK_TAB);
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
		
		case 46:
			KInput::setKeyUnPressed(K_VK_OEM_PERIOD);
			break;
			
		case 96:
			KInput::setKeyUnPressed(K_VK_INSERT);
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


- (void) keyDown:(NSEvent *)event
{
	unichar c = [[event charactersIgnoringModifiers] characterAtIndex:0];
	printf("keyDown: %c %d\n", c, (int)c);
	switch (c)
	{
			/*
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
			*/
			
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
			
		case 9:
			KInput::setKeyPressed(K_VK_TAB);
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
			
		case 46:
			KInput::setKeyPressed(K_VK_OEM_PERIOD);
			break;
			
		case 96:
			KInput::setKeyPressed(K_VK_INSERT);
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

	/*
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
	}*/

	// Allow other character to be handled (or not and beep)
	//[super keyDown:event];
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
	NSLog(@"mouseUp");
	BOOL keepOn = YES;
	BOOL isInside = YES;
	NSPoint mouseLoc;
	
//	while (keepOn) {
//		theEvent = [[self window] nextEventMatchingMask: NSLeftMouseUpMask | NSRightMouseUpMask];
		// mouseLoc = [self convertPoint:[theEvent locationInWindow] fromView:nil];
		//	isInside = [self mouse:mouseLoc inRect:[self bounds]];
		
		switch ([theEvent type]) {
			case NSLeftMouseUp:
				//if (isInside) [self doSomethingSignificant];
				KInput::setLeftButtonState(false);
				keepOn = NO;
				break;
			case NSRightMouseUp:
				KInput::setRightButtonState(false);
				break;
			default:
				/* Ignore any other kind of event. */
				break;
		}
//	};
	KInput::setScreenReleased();
}

- (void) mouseDown:(NSEvent *)theEvent
{
	NSLog(@"mouseDown");
	BOOL keepOn = YES;
	BOOL isInside = YES;
	NSPoint mouseLoc;
	
//	while (keepOn) {
//		theEvent = [[self window] nextEventMatchingMask: NSLeftMouseDownMask | NSRightMouseDownMask];
	// mouseLoc = [self convertPoint:[theEvent locationInWindow] fromView:nil];
	//	isInside = [self mouse:mouseLoc inRect:[self bounds]];
		
		switch ([theEvent type]) {
			case NSLeftMouseDown:
				//if (isInside) [self doSomethingSignificant];
				KInput::setLeftButtonState(true);
				keepOn = NO;
				break;
			case NSRightMouseDown:
				KInput::setRightButtonState(true);
				break;
			default:
				/* Ignore any other kind of event. */
				break;
		}
//	};
}

@end
