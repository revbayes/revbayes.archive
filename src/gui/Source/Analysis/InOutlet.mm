#import "InOutlet.h"
#import "RevBayes.h"
#import "Tool.h"



@implementation InOutlet

@synthesize position;
@synthesize toolColor;
@synthesize toolOwner;

- (BOOL)amInlet {

    return NO;
}

- (BOOL)amOutlet {

    return NO;
}

- (void)dealloc {

    [super dealloc];
}

- (void)encodeWithCoder:(NSCoder *)aCoder {

    [aCoder encodePoint:position   forKey:@"position"];
	[aCoder encodeObject:toolColor forKey:@"toolColor"];
	[aCoder encodeObject:toolOwner forKey:@"toolOwner"];
}

- (id)init {

	[self initWithTool:nil];
	return self;
}

- (id)initWithCoder:(NSCoder *)aDecoder {

    if ( (self = [super init]) ) 
		{
		toolColor = [aDecoder decodeObjectForKey:@"toolColor"];
		toolOwner = [aDecoder decodeObjectForKey:@"toolOwner"];
		position  = [aDecoder decodePointForKey:@"position"];
		}
	return self;
}

- (id)initWithTool:(Tool*)t {

    if ( (self = [super init]) ) 
		{
		toolOwner = t;
        position = NSMakePoint(1.0, 0.5);
		}
    return self;
}

- (BOOL)isOnLeftEdge {

    if ( position.x < 0.00001)
        return YES;
    return NO;
}

- (BOOL)isOnRightEdge {

    if ( position.x > 0.99999)
        return YES;
    return NO;
}

- (BOOL)isOnUpperEdge {

    if ( position.y > 0.99999)
        return YES;
    return NO;
}

- (BOOL)isOnLowerEdge {

    if ( position.y < 0.00001)
        return YES;
    return NO;
}

- (int)numberOfConnections {

    return 0;
}

- (NSPoint)offsetPosition {

    NSPoint myPoint = position;
    if ( position.x < 0.000001 || position.x > 0.999999 )
        myPoint.y += 0.04;
    else if ( position.y < 0.000001 || position.y > 0.999999 )
        myPoint.x += 0.04;
    return myPoint;
}

- (void)pointsForToolWithRect:(NSRect)r atVertex1:(NSPoint*)v1 andVertex2:(NSPoint*)v2 andVertex3:(NSPoint*)v3 {
    
}

- (NSPoint)pointForToolWithRect:(NSRect)r {

#   if 1
    NSPoint myPoint = NSMakePoint( r.origin.x + r.size.width * [self offsetPosition].x, r.origin.y + r.size.height * [self offsetPosition].y );
    return myPoint;
#   else
    NSPoint myPoint = NSMakePoint( r.origin.x + r.size.width * position.x, r.origin.y + r.size.height * position.y );
    return myPoint;
#   endif
}

- (NSRect)rectForToolWithRect:(NSRect)r {

    // the rect that will be returned, containing the inlet/outlet's position
    NSRect myRect;

    // calculate the size of the rectangle's sides
    float mySideLength = r.size.width * 0.2;
    
    // we assume that the coordinate system or r is bottom-left
    // first, set the origin of the rectangle
    myRect.origin.x = r.origin.x + r.size.width * position.x;
    myRect.origin.y = r.origin.y + r.size.height * position.y;
    if (position.x < 0.01)
        myRect.origin.x -= mySideLength * BURY_FRACTION;
    else
        myRect.origin.x -= mySideLength * (1.0 - BURY_FRACTION);
    if (position.y < 0.01)
        myRect.origin.y -= mySideLength * BURY_FRACTION;
    else
        myRect.origin.y -= mySideLength * (1.0 - BURY_FRACTION);
    
    // then, set the size of the rectangle
    myRect.size.width = mySideLength;
    myRect.size.height = mySideLength;
    
    return myRect;
}

@end
