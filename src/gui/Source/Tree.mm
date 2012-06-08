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

- (id)initWithSize:(int)n {

    if ( (self = [super init]) ) 
		{
        nodes = [[NSMutableArray alloc] init];
        for (int i=0; i<2*n-1; i++)
            {
            
            }
        downPassSequence = [[NSMutableArray alloc] init];
        initializedDownPass = NO;
        //[self buildRandomTree];
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
        
    // initialize the depth of the nodes
	NSEnumerator *enumerator = [downPassSequence objectEnumerator];
	Node* p;
	while (p = [enumerator nextObject]) 
        {
        if ([p isLeaf] == YES)
            {
            [p setDepthFromTip:0];
            }
        else
            {
            int maxDepth = 0;
            for (int i=0; i<[p numberOfDescendants]; i++)
                {
                if ( [[p descendantIndexed:i] depthFromTip] > maxDepth )
                    maxDepth = [[p descendantIndexed:i] depthFromTip];
                }
            [p setDepthFromTip:(maxDepth+1)];
            }
        }
    int depthOfRoot = [root depthFromTip];

    // set depth (y values)
	enumerator = [downPassSequence objectEnumerator];
	while (p = [enumerator nextObject]) 
        {
        [p setY:(1.0 - (float)[p depthFromTip]/depthOfRoot)];
        }
    
	// set x coordinates
	double x = 0.0;
	double maximumX = 0.0;
	double maximumY = 0.0;
	enumerator = [downPassSequence objectEnumerator];
	while (p = [enumerator nextObject]) 
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

	enumerator = [downPassSequence objectEnumerator];
	while (p = [enumerator nextObject]) 
        {
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
