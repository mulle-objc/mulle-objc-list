#import "mylib.h"

#import "import-private.h"



@implementation mylib


- (instancetype) init
{
   return( self);
}


- (void) finalize
{
   [super finalize]; // call anywhere you like
}


- (void) dealloc
{
   [super dealloc];  // call at end
}


- (void) coverMe
{
}

- (void) uncoverMe
{
}

@end
