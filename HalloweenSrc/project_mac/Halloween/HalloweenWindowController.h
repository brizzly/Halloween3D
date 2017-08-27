/*
 Copyright (C) 2015 Apple Inc. All Rights Reserved.
 See LICENSE.txt for this sample’s licensing information
 
 Abstract:
 Window controller subclass.
 */


#import <Cocoa/Cocoa.h>
#import "HalloweenNormalscreenWindow.h"
#import "HalloweenFullscreenWindow.h"
#import "KInput.h"

@interface HalloweenWindowController : NSWindowController
{
	HalloweenNormalscreenWindow * _standardWindow;
	HalloweenFullscreenWindow * _fullscreenWindow;

	//NSWindow * _standardWindow;		// Non-Fullscreen window (also the initial window)

	
	BOOL 	_fullscreen;
	
	NSTrackingArea * trackingArea;
	NSRect			_hitRect;
	NSWindowController * controller;
	NSEvent * mouseEventMonitor;
	NSRect _standardRect;

}




@end
