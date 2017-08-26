//
//  KJoypad.h
//  Grizzly
//
//  Created by Julien Meyer on 23/06/2016.
//
//


#import <Foundation/Foundation.h>
#import <GameController/GameController.h>
#import <Cocoa/Cocoa.h>
#import "DDHidLib.h"

/*
#define PAD_NONE		0
#define PAD_UP			0x00000001l
#define PAD_DOWN		0x00000002l
#define PAD_LEFT		0x00000004l
#define PAD_RIGHT		0x00000008l
#define PAD_BUTTON1		0x00000010l
#define PAD_BUTTON2		0x00000020l
#define PAD_BUTTON3		0x00000040l
#define PAD_BUTTON4		0x00000080l
#define PAD_BUTTON5		0x00000100l
#define PAD_BUTTON6		0x00000200l
#define PAD_BUTTON7		0x00000400l
#define PAD_BUTTON8		0x00000800l
#define PAD_BUTTON9		0x00001000l
#define PAD_BUTTON10	0x00002000l
*/

@interface KJoypad : NSObject
{
	NSArray* joySticks;
	
	NSArray * mJoysticks;
	NSMutableArray * mJoystickButtons;
	int mXAxis;
	int mYAxis;
	NSUInteger mJoystickIndex;
	
	// Don't retain these
	DDHidJoystick * mCurrentJoystick;
}

- (void) initJoypad;

+ (void) Set_HasJoypad:(BOOL)value;
+ (BOOL) Get_HasJoypad;

+ (long) Get_PadStatus;
+ (void) Set_PadStatus:(int)dir :(BOOL)pressed;


//@property (strong, nonatomic) GCController * controller;

@end
