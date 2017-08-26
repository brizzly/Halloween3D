// Released under MIT License
/*
 Copyright (c) 2010 by Julien Meyer. Web: http://www.jmApp.com
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 */

#include "KWeb.h"
#include "Foundation/Foundation.h"
//#import <UIKit/UIKit.h>
//#import <Cocoa/Cocoa.h>
#include <AppKit/NSWorkspace.h>




@interface KWebAsync : NSObject 
{
@private
	NSURLConnection *	connection;
	NSMutableData	*	data;
	NSString		*	responseString;
	Boolean				completed;
}
- (void) initRequest:(NSString*)myRequestString;
- (NSString*) getAsyncResponse;
@end

@implementation KWebAsync


- (void) initRequest:(NSString*)myRequestString
{
	//1. LOAD DATA FROM WEB
	NSURLSession * session = [NSURLSession sharedSession];
	[[session dataTaskWithURL:[NSURL URLWithString:myRequestString]
			completionHandler:^(NSData *data__, NSURLResponse *response,
								NSError *error) {
				
				if(data__)
				{
					responseString = [[NSString alloc] initWithData:data__ encoding:NSUTF8StringEncoding];
					if(responseString)
					{
						NSLog(@"connection finish : %@", responseString);
						completed = YES;
					}
					
					/*
					// handle response
					//Once data is got, load local object & fire reloadData
					if (data==nil)
					{
						data = [[NSMutableData alloc] init];
					}
					data = [NSJSONSerialization JSONObjectWithData:data__ options:NSJSONReadingMutableContainers error:&error];
					
					//UPDATE CURRENT VIEW FIRST
					//[self setData];
					
					if(data)
					{
						responseString = [[NSString alloc] initWithData:data encoding:NSASCIIStringEncoding];
						if(responseString)
						{
							NSLog(@"connection finish : %@", responseString);
							completed = YES;
						}
					}*/
				}
				
			}] resume];
}


- (void) initRequest_OLD:(NSString*)myRequestString
{
	completed = NO;
	responseString = nil;
	NSURL *url = [NSURL URLWithString:myRequestString];
	NSURLRequest* request = [NSURLRequest requestWithURL:url cachePolicy:NSURLRequestUseProtocolCachePolicy timeoutInterval:60.0];
	connection = [[NSURLConnection alloc] initWithRequest:request delegate:self];
}

- (void) connection:(NSURLConnection *)theConnection didReceiveData:(NSData *)incrementalData 
{
	if (data==nil) 
	{ 
		data = [[NSMutableData alloc] initWithCapacity:2048]; 
	} 
	[data appendData:incrementalData];
}

- (void) connection:(NSURLConnection *)connection didFailWithError:(NSError *)error
{
	NSLog(@"connection failed %@", error);
}

- (void) connectionDidFinishLoading:(NSURLConnection*)theConnection 
{
	if(data)
	{
		responseString = [[NSString alloc] initWithData:data encoding:NSASCIIStringEncoding];
		if(responseString)
		{
			NSLog(@"connection finish : %@", responseString);
			completed = YES;
		}
	}
}

- (NSString*) getAsyncResponse
{
	if(completed == NO) {
		return nil;
	}
	if(responseString)
	{
		return responseString;
	}
	return nil;
}

@end


KWebAsync * myConnection = nil;

KWeb::KWeb( ) 
{
	//hInternet = 0 ;		//handle internet
	//hIurl	= 0 ;
	_dataBuffer = NULL ;
	_dataBufferSize = 0 ;
}

KWeb::~KWeb( ) 
{
	freeInternetHandles( ) ;
	freeDataBuffer( ) ;
}

unsigned long KWeb::getBufferSize( void ) 
{
	return	_dataBufferSize ;
}

void KWeb::freeDataBuffer( void ) 
{
	if ( _dataBuffer != NULL )
	{
		delete	[]	_dataBuffer ;
		_dataBuffer 		= NULL ;
		_dataBufferSize 	= 0 ;
	}
}

void KWeb::freeInternetHandles( void ) 
{
}

void KWeb::showURL(char	*paramURL)
{
	NSString *urlstring = [NSString stringWithUTF8String: paramURL];
	//[[NSApplication sharedApplication] openURL:[NSURL URLWithString: urlstring]];
	[[NSWorkspace sharedWorkspace] openURL:[NSURL URLWithString:urlstring]];
}

char * KWeb::readAsyncResponse()
{
	if(myConnection == nil) {
		return 0;
	}
	NSString * respString = [myConnection getAsyncResponse];
	if(respString == nil) {
		return 0;
	}
	char * resp = (char *)[respString UTF8String];
	//[myConnection release];
	myConnection = nil;
	return resp;
}

char * KWeb::callURL( char	*paramURL , bool 	asynchronous ) 
{
	if(asynchronous) 
	{
		myConnection = [[KWebAsync alloc] init];
		NSString * myRequestString = [NSString stringWithUTF8String: paramURL];
		[myConnection initRequest:myRequestString];
	}
	else
	{
		NSString * myRequestString = [NSString stringWithUTF8String: paramURL];
		NSMutableURLRequest *request = [ [ NSMutableURLRequest alloc ] initWithURL: [ NSURL URLWithString: myRequestString ] ]; 
		[ request setHTTPMethod: @"POST" ];
		
		NSData *returnData = [ NSURLConnection sendSynchronousRequest: request returningResponse: nil error: nil ];
		if(returnData!=0)
		{
			unsigned long	bufferSize=[returnData length]+1;
			_dataBuffer = new		char[bufferSize] ;
			memset( _dataBuffer , 0 , bufferSize ) ;
			
			[returnData getBytes:_dataBuffer];
			_dataBufferSize=bufferSize;
			return _dataBuffer;
		}
		//[request release]; // Correction PTRO		
	}

	return 0;
}







