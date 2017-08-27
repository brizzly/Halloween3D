/*
 Copyright (C) 2015 Apple Inc. All Rights Reserved.
 See LICENSE.txt for this sample’s licensing information
 
 Abstract:
 OpenGL view subclass.
 */


#import <Cocoa/Cocoa.h>
#import <QuartzCore/CVDisplayLink.h>

#import "modelUtil.h"
#import "imageUtil.h"

@interface HalloweenGLView : NSOpenGLView
{
	CVDisplayLinkRef displayLink;
	
	NSTrackingArea * trackingArea;
	NSRect			_hitRect;
	NSWindowController * controller;
}

- (void) setMainController:(NSWindowController*)theController;


@end
