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

	[aCoder encodeObject:nodes            forKey:@"nodes"];
    [aCoder encodeObject:downPassSequence forKey:@"downPassSequence"];
}

- (id)init {

    if ( (self = [super init]) ) 
		{
        nodes = [[NSMutableArray alloc] init];
        downPassSequence = [[NSMutableArray alloc] init];
        root  = nil;
		}
    return self;
}

- (void)initializeDownPassSequence {

    [downPassSequence removeAllObjects];
    [self passDown:root];
}

- (id)initWithCoder:(NSCoder*)aDecoder {

    if ( (self = [super init]) ) 
		{
		nodes = [aDecoder decodeObjectForKey:@"name"];
        downPassSequence = [aDecoder decodeObjectForKey:@"downPassSequence"];
		[nodes retain];
        [downPassSequence retain];
		}
	return self;
}

- (int)numberOfNodes {

    return (int)[nodes count];
}

- (void)passDown:(Node*)p {

    if ([p numberOfDescendants] > 0)
        {
        for (int i=0; i<[p numberOfDescendants]; i++)
            [self passDown:[p descendantIndexed:i]];
        [downPassSequence addObject:p];
        }
}

- (void)setNodesToArray:(NSMutableArray*)n {

    [nodes release];
    nodes = n;
}

- (void)setRoot:(Node*)r {

    root = r;
}

@end
