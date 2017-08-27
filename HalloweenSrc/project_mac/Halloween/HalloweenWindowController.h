/*
 Copyright (C) 2015 Apple Inc. All Rights Reserved.
 See LICENSE.txt for this sample’s licensing information
 
 Abstract:
 Window controller subclass.
 */


#import <Cocoa/Cocoa.h>

@interface HalloweenWindowController : NSWindowController
{
	NSTrackingArea * trackingArea;
	NSRect			_hitRect;
	NSWindowController * controller;
	NSEvent * mouseEventMonitor;

}

@end
