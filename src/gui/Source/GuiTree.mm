#import "Node.h"
#import "GuiTree.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"




@implementation Tree

- (void)buildRandomTreeWithSize:(int)n {

    // find the global random number generator
    RandomNumberGenerator* rng = GLOBAL_RNG;

    // fill in the vector of nodes
    [nodes removeAllObjects];
    int numNodes = 2 * n - 1;
    for (int i=0; i<numNodes; i++)
        {
        Node* nde = [[Node alloc] init];
        [nde setIndex:i];
        NSString* nameStr = [[NSString alloc] initWithFormat:@"Taxon %d", i+1];
        [nde setName:nameStr];
        [nodes addObject:nde];
        }
        
    // build a two-species tree
    NSMutableArray* treeArray = [NSMutableArray arrayWithCapacity:1];
    int nextTip = 0, nextInt = n;
    Node* p = [nodes objectAtIndex:nextTip];
    nextTip++;
    Node* q = [nodes objectAtIndex:nextInt];
    nextInt++;
    [treeArray addObject:p];
    [treeArray addObject:q];
    [p setAncestor:q];
    [q addDescendant:p];
    p = [nodes objectAtIndex:nextTip];
    nextTip++;
    [treeArray addObject:p];
    [p setAncestor:q];
    [q addDescendant:p];
    root = q;
    
    // randomly add the remaining branches to the two-species tree
    for (int i=2; i<n; i++)
        {
        p = [nodes objectAtIndex:nextTip];
        nextTip++;
        q = [nodes objectAtIndex:nextInt];
        nextInt++;
        
        Node* a = [treeArray objectAtIndex:( (int)(rng->uniform01() * [treeArray count]) )];
        if ([a ancestor] == nil)
            {
            [a setAncestor:q];
            [q addDescendant:a];
            [q addDescendant:p];
            [p setAncestor:q];
            root = q;            
            }
        else 
            {
            Node* b = [a ancestor];
            [b removeDescendant:a];
            [b addDescendant:q];
            [q setAncestor:b];
            [q addDescendant:a];
            [q addDescendant:p];
            [a setAncestor:q];
            [p setAncestor:q];
            }
        
        [treeArray addObject:p];
        [treeArray addObject:q];
        }
        
    [self print];
}

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

- (id)initWithTipSize:(int)n {

    if ( (self = [super init]) ) 
		{
        nodes = [[NSMutableArray alloc] init];
        [self buildRandomTreeWithSize:n];
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

- (void)print {

    for (int i=0; i<[nodes count]; i++)
        [[nodes objectAtIndex:i] print];
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
