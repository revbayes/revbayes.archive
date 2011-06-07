#import "InOutlet.h"
#import "Tool.h"



@implementation InOutlet

@synthesize amInlet;
@synthesize isSelected;
@synthesize inOutletRect;
@synthesize partner;
@synthesize toolColor;
@synthesize toolOwner;

- (void)dealloc {

    [super dealloc];
}

- (void)encodeWithCoder:(NSCoder *)aCoder {

	[aCoder encodeBool:amInlet        forKey:@"amInlet"];
	[aCoder encodeBool:isSelected     forKey:@"isSelected"];
	[aCoder encodeObject:toolColor    forKey:@"toolColor"];
	[aCoder encodeObject:toolOwner    forKey:@"toolOwner"];
	[aCoder encodeRect:inOutletRect   forKey:@"inOutletRect"];
	[aCoder encodePoint:inOutletPoint forKey:@"inOutletPoint"];
	[aCoder encodeObject:partner      forKey:@"partner"];
}

- (void)flipSelected {

	if (isSelected == YES)
		isSelected = NO;
	else 
		isSelected = YES;
}

- (NSPoint)getInOutletPointFromPoint:(NSPoint)p {

	NSPoint mp = inOutletRect.origin;
	mp.x += p.x + inOutletRect.size.width*0.5;
	mp.y += p.y + inOutletRect.size.height*0.5;
	return mp;
}

- (NSRect)getInOutletRectFromPoint:(NSPoint)p {

	NSRect r = inOutletRect;
	r.origin.x += p.x;
	r.origin.y += p.y;
	return r;
}

- (id)init {

	[self initWithTool:nil];
	return self;
}

- (id)initWithCoder:(NSCoder *)aDecoder {

    if ( (self = [super init]) ) 
		{
		amInlet       = [aDecoder decodeBoolForKey:@"amInlet"];
		isSelected    = [aDecoder decodeBoolForKey:@"isSelected"];
		toolColor     = [aDecoder decodeObjectForKey:@"toolColor"];
		toolOwner     = [aDecoder decodeObjectForKey:@"toolOwner"];
		inOutletRect  = [aDecoder decodeRectForKey:@"inOutletRect"];
		inOutletPoint = [aDecoder decodePointForKey:@"inOutletPoint"];
		partner       = [aDecoder decodeObjectForKey:@"partner"];
		}
	return self;
}

- (id)initWithTool:(Tool*)t {

    if ( (self = [super init]) ) 
		{
		toolOwner = t;
		partner   = nil;
		}
    return self;
}

@end
