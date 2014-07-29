#import "Connection.h"
#import "Inlet.h"
#import "Outlet.h"



@implementation Connection

@synthesize inlet;
@synthesize outlet;
@synthesize isSelected;
@synthesize path1;
@synthesize path2;

/* JPHARC - (void)dealloc {

    [path1 release];
    [path2 release];
    [super dealloc];
} */

- (void)encodeWithCoder:(NSCoder*)aCoder {

    [aCoder encodeBool:isSelected forKey:@"isSelected"];
	[aCoder encodeObject:inlet    forKey:@"inlet"];
	[aCoder encodeObject:outlet   forKey:@"outlet"];
    [aCoder encodeObject:path1    forKey:@"path1"];
    [aCoder encodeObject:path2    forKey:@"path2"];
}

- (void)flipSelected {

	if (isSelected == YES)
		isSelected = NO;
	else 
		isSelected = YES;
}

- (id)init {

    if ( (self = [super init]) ) 
        {
        inlet = nil;
        outlet = nil;
        isSelected = NO;
        
        path1 = [[NSBezierPath alloc] init];
        path2 = [[NSBezierPath alloc] init];
        }
    
    return self;
}

- (id)initWithCoder:(NSCoder *)aDecoder {

    if ( (self = [super init]) ) 
		{
		isSelected = [aDecoder decodeBoolForKey:@"isSelected"];
		inlet      = [aDecoder decodeObjectForKey:@"inlet"];
		outlet     = [aDecoder decodeObjectForKey:@"outlet"];
        path1      = [aDecoder decodeObjectForKey:@"path1"];
        path2      = [aDecoder decodeObjectForKey:@"path2"];
        /* JPHARC [path1 retain];
        [path2 retain]; */
		}
	return self;
}

@end
