#import "Connection.h"
#import "Inlet.h"



@implementation Inlet

- (void)addConnection:(Connection*)c {

    connection = c;
}

- (BOOL)amInlet {

    return YES;
}

- (BOOL)amOutlet {

    return NO;
}

- (Connection*)connection {

    return connection;
}

- (Connection*)connectionWithIndex:(int)idx {

    return connection;
}

- (void)encodeWithCoder:(NSCoder*)aCoder {
    
	[aCoder encodeObject:connection forKey:@"connection"];
	
    [super encodeWithCoder:aCoder];
}

- (Outlet*)getMyOutlet {

    if (connection == nil)
        return nil;
    return [connection outlet];
}

- (BOOL)hasConnection {

    if (connection == nil)
        return NO;
    return YES;
}

- (id)init {

    if ( (self = [super init]) ) 
        {
        connection = nil;
        }
    return self;
}

- (id)initWithCoder:(NSCoder*)aDecoder {

    if ( (self = [super initWithCoder:aDecoder]) ) 
		{
		connection = [aDecoder decodeObjectForKey:@"connection"];
		}
	return self;
}

- (id)initWithTool:(Tool*)t {

    if ( (self = [super initWithTool:t]) ) 
		{
        connection = nil;
		}
    return self;
}

- (int)numberOfConnections {

    if (connection == nil)
        return 0;
    return 1;
}

- (void)pointsForToolWithRect:(NSRect)r atVertex1:(NSPoint*)v1 andVertex2:(NSPoint*)v2 andVertex3:(NSPoint*)v3 {

    NSRect inOutRect = [self rectForToolWithRect:r];
    float altitude = inOutRect.size.width * 0.866025403;
    
    if (position.x < 0.01)
        {
        // left side
        v1->x = inOutRect.origin.x + (inOutRect.size.width - altitude);
        v2->x = v1->x;
        v3->x = v1->x + altitude;
        v1->y = inOutRect.origin.y;
        v2->y = v1->y + inOutRect.size.height;
        v3->y = v1->y + 0.5 * inOutRect.size.height;
        }
    else if (position.x > 0.99)
        {
        // right side
        v1->x = inOutRect.origin.x + altitude;
        v2->x = v1->x;
        v3->x = inOutRect.origin.x;
        v1->y = inOutRect.origin.y;
        v2->y = v1->y + inOutRect.size.height;
        v3->y = v1->y + 0.5 * inOutRect.size.height;
        }
    else if (position.y < 0.01)
        {
        // bottom side
        v1->x = inOutRect.origin.x;
        v2->x = v1->x + inOutRect.size.width;
        v3->x = v1->x + 0.5 * inOutRect.size.width;
        v1->y = inOutRect.origin.y + (inOutRect.size.height - altitude);
        v2->y = v1->y;
        v3->y = v1->y + altitude;
        }
    else if (position.y > 0.99)
        {
        // top side
        v1->x = inOutRect.origin.x;
        v2->x = v1->x + inOutRect.size.width;
        v3->x = v1->x + 0.5 * inOutRect.size.width;
        v1->y = inOutRect.origin.y + altitude;
        v2->y = v1->y;
        v3->y = inOutRect.origin.y;
        }
    else
        {
        NSLog(@"Cannot figure out which edge of the tool the inlet is on");
        }
}

- (void)removeConnection {

    connection = nil;
}

@end
