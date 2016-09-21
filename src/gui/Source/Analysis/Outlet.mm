#import "Connection.h"
#import "Inlet.h"
#import "Outlet.h"
#import "Tool.h"



@implementation Outlet

- (void)addConnectionWithInlet:(Inlet*)theInlet {

    // decide if we already have a connection with that inlet
    BOOL alreadyConnected = NO;
	NSEnumerator* enumerator = [connections objectEnumerator];
	Connection* element;
	while ( (element = [enumerator nextObject]) )
		{
		if ([element inlet] == theInlet)
			alreadyConnected = YES;
		}
        
    // the connection does not already exist, make it...
    if (alreadyConnected == NO)
        {
        Connection* c = [[Connection alloc] init];
        [c setOutlet:self];
        [c setInlet:theInlet];
        [connections addObject:c];
        [theInlet addConnection:c];
        [[self toolOwner] updateChildrenTools];
        }
}

- (BOOL)amInlet {

    return NO;
}

- (BOOL)amOutlet {

    return YES;
}

- (Connection*)connectionWithIndex:(int)idx {

    return [connections objectAtIndex:idx];
}

- (void)encodeWithCoder:(NSCoder*)aCoder {
    
	[aCoder encodeObject:connections forKey:@"connections"];
	
    [super encodeWithCoder:aCoder];
}

- (id)init {

    if ( (self = [super init]) ) 
        {
        connections = [[NSMutableArray alloc] init];
        }
    return self;
}

- (id)initWithCoder:(NSCoder*)aDecoder {

    if ( (self = [super initWithCoder:aDecoder]) ) 
		{
		connections = [aDecoder decodeObjectForKey:@"connections"];
		}
	return self;
}

- (id)initWithTool:(Tool*)t {

    if ( (self = [super initWithTool:t]) ) 
		{
        connections = [[NSMutableArray alloc] init];
		}
    return self;
}

- (int)numberOfConnections {

    return (int)[connections count];
}

- (void)pointsForToolWithRect:(NSRect)r atVertex1:(NSPoint*)v1 andVertex2:(NSPoint*)v2 andVertex3:(NSPoint*)v3 {

    NSRect inOutRect = [self rectForToolWithRect:r];
    float altitude = inOutRect.size.width * 0.866025403;
    
    if (position.x < 0.01)
        {
        // left side
        v3->x = inOutRect.origin.x + (inOutRect.size.width - altitude);
        v1->x = v3->x + altitude;
        v2->x = v1->x;
        v1->y = inOutRect.origin.y;
        v2->y = v1->y + inOutRect.size.height;
        v3->y = v1->y + 0.5 * inOutRect.size.height;
        }
    else if (position.x > 0.99)
        {
        // right side
        v3->x = inOutRect.origin.x + altitude;
        v1->x = inOutRect.origin.x;
        v2->x = v1->x;
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
        v3->y = inOutRect.origin.y + (inOutRect.size.height - altitude);
        v1->y = v3->y + altitude;
        v2->y = v1->y;
        }
    else if (position.y > 0.99)
        {
        // top side
        v1->x = inOutRect.origin.x;
        v2->x = v1->x + inOutRect.size.width;
        v3->x = v1->x + 0.5 * inOutRect.size.width;
        v1->y = inOutRect.origin.y;
        v2->y = v1->y;
        v3->y = inOutRect.origin.y + altitude;
        }
    else
        {
        NSLog(@"Cannot figure out which edge of the tool the inlet is on");
        }
}

- (void)removeConnection:(Connection*)c {

    if ( [connections containsObject:c] == YES )
        {
        [[c inlet] removeConnection];
        [connections removeObject:c];
        }
}

@end
