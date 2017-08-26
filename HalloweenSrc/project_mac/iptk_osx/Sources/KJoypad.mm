//
//  KJoypad.m
//  Grizzly
//
//  Created by Julien Meyer on 23/06/2016.
//
//

#import "KJoypad.h"
#import "KInput.h"



@implementation KJoypad


KJoypad * singleton = NULL;

IOHIDManagerRef hidManager;
BOOL hasJoypad;
long PadStatus;
float analog_x;
float analog_y;




- (id) init
{
	if(self = [super init])
	{
		singleton = self;
	}
	return self;
}

- (void)dealloc
{
	//[super dealloc];
}


+ (void) Set_HasJoypad:(BOOL)value
{
	hasJoypad = value;
}

+ (BOOL) Get_HasJoypad
{
	return hasJoypad;
}

+ (long) Get_PadStatus
{
	return PadStatus;
}

+ (void) Set_PadStatus:(int)dir :(BOOL)pressed
{
	if(dir == PAD_NONE)
	{
		PadStatus = PAD_NONE;
	}
	else if(dir == PAD_UP)
	{
		if( pressed )
			PadStatus = PAD_UP; //|= PAD_UP;
		else
			PadStatus = PAD_NONE; //  &= ~KEY_UP;
	}
	else if(dir == PAD_DOWN)
	{
		if( pressed )
			PadStatus = PAD_DOWN;
		else
			PadStatus = PAD_NONE;
	}
	else if(dir == PAD_LEFT)
	{
		if( pressed )
			PadStatus = PAD_LEFT;
		else
			PadStatus = PAD_NONE;
	}
	else if(dir == PAD_RIGHT)
	{
		if( pressed )
			PadStatus = PAD_RIGHT;
		else
			PadStatus = PAD_NONE;
	}
}

+ (void) Set_AnalogPad_X:(int)value
{
	analog_x = value;
}
+ (void) Set_AnalogPad_Y:(int)value
{
	analog_x = value;
}


void gamepadWasAdded(void* inContext, IOReturn inResult, void* inSender, IOHIDDeviceRef device) {
	NSLog(@"Gamepad was plugged in");
	[KJoypad Set_HasJoypad:YES];
	if(singleton) {
		[singleton setupJoypad];
	}
}

void gamepadWasRemoved(void* inContext, IOReturn inResult, void* inSender, IOHIDDeviceRef device) {
	NSLog(@"Gamepad was unplugged");
	[KJoypad Set_HasJoypad:NO];
	if(singleton) {
		[singleton endJoypad];
	}
}

void gamepadAction(void* inContext, IOReturn inResult, void* inSender, IOHIDValueRef value)
{
	//NSLog(@"Gamepad talked!");
	IOHIDElementRef element = IOHIDValueGetElement(value);
	NSLog(@"Element: %@", element);
	int elementValue = IOHIDValueGetIntegerValue(value);
	NSLog(@"Element value: %i", elementValue);
}



- (void) initJoypad
{
	[KJoypad Set_HasJoypad:NO];

	// Create an HID Manager
	hidManager = IOHIDManagerCreate( kCFAllocatorDefault, kIOHIDOptionsTypeNone);
	
	
	// Create a Matching Dictionary
	CFMutableDictionaryRef criterion = CFDictionaryCreateMutable(
																 kCFAllocatorDefault,
																 2,
																 &kCFTypeDictionaryKeyCallBacks,
																 &kCFTypeDictionaryValueCallBacks);

//	NSMutableDictionary* criterion = [[NSMutableDictionary alloc] init];
	
/*
	[criterion setObject: [NSNumber numberWithInt: kHIDPage_GenericDesktop] forKey: (NSString*)CFSTR(kIOHIDDeviceUsagePageKey)];
	[criterion setObject: [NSNumber numberWithInt: kHIDUsage_GD_GamePad] forKey: (NSString*)CFSTR(kIOHIDDeviceUsageKey)];
	IOHIDManagerSetDeviceMatching(hidManager,  (__bridge CFDictionaryRef)criterion);
*/
	IOHIDManagerRegisterDeviceMatchingCallback(hidManager, gamepadWasAdded, (void*)self);
	IOHIDManagerRegisterDeviceRemovalCallback(hidManager, gamepadWasRemoved, (void*)self);

	IOHIDManagerScheduleWithRunLoop(hidManager, CFRunLoopGetCurrent(), kCFRunLoopDefaultMode);
	//IOReturn tIOReturn = IOHIDManagerOpen(hidManager, kIOHIDOptionsTypeNone);
	//IOHIDManagerRegisterInputValueCallback(hidManager, gamepadAction, (void*)self);
	
	//[self setupJoypad];
}

- (void) endJoypad
{
	if (mCurrentJoystick != nil)
	{
		[mCurrentJoystick stopListening];
		mCurrentJoystick = nil;
	}
}

- (void) setupJoypad
{
	NSArray * joysticks = [DDHidJoystick allJoysticks];

	mJoystickButtons = [[NSMutableArray alloc] init];
	[joysticks makeObjectsPerformSelector: @selector(setDelegate:)
							   withObject: self];
	[self setJoysticks: joysticks];
	if ([mJoysticks count] > 0) {
		[self setJoystickIndex: 0];
		[KJoypad Set_HasJoypad:YES];
	}
	else {
		[self setJoystickIndex: NSNotFound];
		[KJoypad Set_HasJoypad:NO];
	}
}


//===========================================================
//  joysticks
//===========================================================
- (NSArray *) joysticks
{
	return mJoysticks;
}

- (NSArray *) joystickButtons;
{
	return mJoystickButtons;
}

//===========================================================
//  joystickIndex
//===========================================================
- (NSUInteger) joystickIndex
{
	return mJoystickIndex;
}

- (void) setJoystickIndex: (NSUInteger) theJoystickIndex
{
//	if (mCurrentJoystick != nil)
//	{
//		[mCurrentJoystick stopListening];
//		mCurrentJoystick = nil;
//	}
	
	mJoystickIndex = theJoystickIndex;
//	[mJoysticksController setSelectionIndex: mJoystickIndex];
	if (mJoystickIndex != NSNotFound)
	{
		mCurrentJoystick = [mJoysticks objectAtIndex: mJoystickIndex];
		[mCurrentJoystick startListening];
		
		[self willChangeValueForKey: @"joystickButtons"];
		[mJoystickButtons removeAllObjects];
		NSArray * buttons = [mCurrentJoystick buttonElements];
		NSEnumerator * e = [buttons objectEnumerator];
		DDHidElement * element;
		while (element = [e nextObject])
		{
//			ButtonState * state = [[ButtonState alloc] initWithName: [[element usage] usageName]];
//			[state autorelease];
//			[mJoystickButtons addObject: state];
		}
		[self didChangeValueForKey: @"joystickButtons"];
	}
}


- (void) setJoysticks: (NSArray *) theJoysticks
{
	if (mJoysticks != theJoysticks)
	{
		//[mJoysticks release];
		//mJoysticks = [theJoysticks retain];
		mJoysticks = theJoysticks;
	}
}


- (void) ddhidJoystick: (DDHidJoystick *)  joystick
				 stick: (unsigned) stick
			  xChanged: (int) value;
{
	[self willChangeValueForKey: @"xAxis"];
	mXAxis = value;
	NSLog(@"mXAxis = %d", mXAxis);
	[self didChangeValueForKey: @"xAxis"];
}

- (void) ddhidJoystick: (DDHidJoystick *)  joystick
				 stick: (unsigned) stick
			  yChanged: (int) value;
{
	[self willChangeValueForKey: @"yAxis"];
	mYAxis = value;
	NSLog(@"mYAxis = %d", mYAxis);
	[self didChangeValueForKey: @"yAxis"];
}

- (void) ddhidJoystick: (DDHidJoystick *) joystick
				 stick: (unsigned) stick
			 otherAxis: (unsigned) otherAxis
		  valueChanged: (int) value;
{
	// Somehow display values here
	NSLog(@"Stick: %d, other axis: %d, changed: %d", stick, otherAxis, value);
}

- (void) ddhidJoystick: (DDHidJoystick *) joystick
				 stick: (unsigned) stick
			 povNumber: (unsigned) povNumber
		  valueChanged: (int) value;
{
	// Somehow display values here
	//NSLog(@"Stick: %d, POV number: %d, changed: %d", stick, povNumber, value);
	//return;
	if(value == -1)
	{
		[KJoypad Set_PadStatus:PAD_NONE :NO];
	}
	if(value == 9000)
	{
		[KJoypad Set_PadStatus:PAD_RIGHT :YES];
	}
	if(value == 18000)
	{
		[KJoypad Set_PadStatus:PAD_DOWN :YES];
	}
	if(value == 27000)
	{
		[KJoypad Set_PadStatus:PAD_LEFT :YES];
	}
	if(value == 0)
	{
		[KJoypad Set_PadStatus:PAD_UP :YES];
	}
}

- (void) ddhidJoystick: (DDHidJoystick *) joystick
			buttonDown: (unsigned) buttonNumber;
{
	NSLog(@"Button Down %d", (int)buttonNumber);
//	ButtonState * state = [mJoystickButtons objectAtIndex: buttonNumber];
//	[state setPressed: YES];
	
	if(buttonNumber == 0)
	{
		PadStatus |= PAD_BUTTON1;
	}
	if(buttonNumber == 1)
	{
		PadStatus |= PAD_BUTTON2;
	}
	if(buttonNumber == 2)
	{
		PadStatus |= PAD_BUTTON3;
	}
	if(buttonNumber == 8)
	{
		PadStatus |= PAD_BUTTON9;
	}
	if(buttonNumber == 9)
	{
		PadStatus |= PAD_BUTTON10;
	}
}

- (void) ddhidJoystick: (DDHidJoystick *) joystick
			  buttonUp: (unsigned) buttonNumber;
{
	NSLog(@"Button Up %d", (int)buttonNumber);
//	ButtonState * state = [mJoystickButtons objectAtIndex: buttonNumber];
//	[state setPressed: NO];
	
	if(buttonNumber == 0)
	{
		PadStatus &= ~PAD_BUTTON1;
	}
	if(buttonNumber == 1)
	{
		PadStatus &= ~PAD_BUTTON2;
	}
	if(buttonNumber == 2)
	{
		PadStatus &= ~PAD_BUTTON3;
	}
	if(buttonNumber == 8)
	{
		PadStatus &= ~PAD_BUTTON9;
	}
	if(buttonNumber == 9)
	{
		PadStatus &= ~PAD_BUTTON10;
	}
}


@end
