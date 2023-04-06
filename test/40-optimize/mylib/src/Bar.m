//
//  Bar.m
//  mylib
//
//  Copyright (c) 2023 Nat! - Mulle kybernetiK.
//  All rights reserved.
//
//
//  Redistribution and use in source and binary forms, with or without
//  modification, are permitted provided that the following conditions are met:
//
//  Redistributions of source code must retain the above copyright notice, this
//  list of conditions and the following disclaimer.
//
//  Redistributions in binary form must reproduce the above copyright notice,
//  this list of conditions and the following disclaimer in the documentation
//  and/or other materials provided with the distribution.
//
//  Neither the name of Mulle kybernetiK nor the names of its contributors
//  may be used to endorse or promote products derived from this software
//  without specific prior written permission.
//
//  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
//  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
//  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
//  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
//  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
//  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
//  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
//  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
//  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
//  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
//  POSSIBILITY OF SUCH DAMAGE.
//
#import "Bar.h"

#import "import-private.h"



@implementation Bar


- (instancetype) init
{
   // Call [super init], if subclassing something else than NSObject.
   // self = [super init];

   // Release self and return nil, if object can not be inited
   //
   // if( ...)
   // {
   //    [self release];
   //    return( nil);
   // }

   //
   // Initialize ivars and properties without using the setter (preferably)
   //
   //
   //   _others = [NSMutableArray new];
   //   _value  = [@"whatever" retain];

   return( self);
}


- (void) finalize
{
   //
   // Autorelease and nil read-only properties.
   // Remove and nil yourself from relationships (like delegate, dataSource).
   // Remove yourself from NSNotificationCenter and such things.
   //
   //   [_value autorelease];
   //   _value = nil;
   [super finalize]; // call anywhere you like
}


- (void) dealloc
{
   //
   // Release instance variables, that aren't properties.
   //
   //   [_others release];
   [super dealloc];  // call at end
}


// example method
//- (id) method:(id) argument
//{
//   return( self);
//}

@end
