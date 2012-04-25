#import "Node.h"
#import "Tree.h"



@implementation Tree

- (void)dealloc {

	[nodes release];
	[super dealloc];
}

- (Node*)downPassNodeIndexed:(int)idx {

    return [downPassSequence objectAtIndex:idx];
}

- (void)encodeWithCoder:(NSCoder*)aCoder {

	[aCoder encodeObject:nodes             forKey:@"nodes"];
    [aCoder encodeObject:downPassSequence  forKey:@"downPassSequence"];
    [aCoder encodeBool:initializedDownPass forKey:@"initializedDownPass"];
}

- (id)init {

    if ( (self = [super init]) ) 
		{
        nodes = [[NSMutableArray alloc] init];
        downPassSequence = [[NSMutableArray alloc] init];
        initializedDownPass = NO;
        root  = nil;
		}
    return self;
}

- (void)initializeDownPassSequence {

    if (initializedDownPass == NO)
        {
        [downPassSequence removeAllObjects];
        [self passDown:root];
        initializedDownPass = YES;
        }
}

- (id)initWithCoder:(NSCoder*)aDecoder {

    if ( (self = [super init]) ) 
		{
		nodes = [aDecoder decodeObjectForKey:@"name"];
        downPassSequence = [aDecoder decodeObjectForKey:@"downPassSequence"];
        initializedDownPass = [aDecoder decodeBoolForKey:@"initializedDownPass"];
		[nodes retain];
        [downPassSequence retain];
		}
	return self;
}

- (int)numberOfNodes {

    return (int)[nodes count];
}

- (void)passDown:(Node*)p {

    for (int i=0; i<[p numberOfDescendants]; i++)
        [self passDown:[p descendantIndexed:i]];
    [downPassSequence addObject:p];
}

- (void)setCoordinates {
		
	if (initializedDownPass == NO)
		[self initializeDownPassSequence];
        
    NSLog(@"nodes = %@", nodes);

	// set x coordinates
	double x = 0.0;
	double maximumX = 0.0;
	double maximumY = 0.0;
	NSEnumerator *enumerator1 = [downPassSequence objectEnumerator];
	Node* p;
	while (p = [enumerator1 nextObject]) 
		{
		if ( [p isLeaf] == YES )
			{
			[p setX:x];
			x += 1.0;
			Node* q = p;
			double depth = 0.0;
			while (q != NULL)
				{
				depth += 1.0;
				q = [q ancestor];
				}
			if (depth > maximumY)
				maximumY = depth;
			}
		else 
			{
			double xMin = 1000000000.0;
			double xMax = 0.0;
            for (int i=0; i<[p numberOfDescendants]; i++)
                {
                Node* q = [p descendantIndexed:i];
				double qX = [q x];
				if (qX < xMin)
					xMin = qX;
				else if (qX > xMax)
					xMax = qX;
                }
			[p setX:(xMin+(xMax-xMin)*0.5)];
			}
		if ([p x] > maximumX)
			maximumX = [p x];
		}
	
	// set y coordinates
	NSEnumerator *enumerator2 = [downPassSequence objectEnumerator];
	while (p = [enumerator2 nextObject]) 
		{
		double depth = 0.0;
		Node* q = p;
		while (q != NULL)
			{
			depth += 1.0;
			q = [q ancestor];
			}
		[p setY:(depth/maximumY)];
		[p setX:([p x]/maximumX)];
		
		}
}

- (void)setNodesToArray:(NSMutableArray*)n {

    nodes = n;
}

- (void)setRoot:(Node*)r {

    root = r;
}

@end
