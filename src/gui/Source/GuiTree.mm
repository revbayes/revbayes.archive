#define __ASSERT_MACROS_DEFINE_VERSIONS_WITHOUT_UNDERSCORES 0
#import "Node.h"
#import "GuiTree.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"




@implementation GuiTree

@synthesize info;
@synthesize initializedDownPass;
@synthesize root;
@synthesize numberOfTaxa;
@synthesize nodes;
@synthesize outgroupName;
@synthesize weight;

- (Node*)addNode {

    Node* n = [[Node alloc] init];
    [nodes addObject:n];
    return n;
}

- (void)addNodeToTree:(Node*)n {

    [nodes addObject:n];
}

- (void)addTaxonToRandomBranch {

    // find the global random number generator
    RevBayesCore::RandomNumberGenerator* rng = RevBayesCore::GLOBAL_RNG;

    // pick a branch at random to add the new branch
    Node* p = nil;
    do
        {
        int whichNode = (int)(rng->uniform01() * [self numberOfNodes]);
        if ( [[nodes objectAtIndex:whichNode] ancestor] != nil )
            p = [nodes objectAtIndex:whichNode];
        } while (p == nil);
        
    // make two new nodes for the new branch
    Node* a = [[Node alloc] init];
    Node* b = [[Node alloc] init];
    [a setIndex:[self numberOfNodes]];
    [b setIndex:([self numberOfNodes]+1)];
    [a setIsLeaf:YES];
    [b setIsLeaf:NO];
    [a setY:1.0];
    [b setY:([[p ancestor] y] + ([p y] - [[p ancestor] y]) * 0.5)];
    [a setName:[[NSString alloc] initWithFormat:@"Taxon %d", ([self getNumberOfTaxa]+1)]];
    [b setName:@"InteriorNode"];
    [nodes addObject:a];
    [nodes addObject:b];
        
    // add the new branch to the tree
    [a setAncestor:b];
    [b addDescendant:a];
    Node* anc = [p ancestor];
    [anc removeDescendant:p];
    [anc addDescendant:b];
    [b addDescendant:p];
    [b setAncestor:anc];
    [p setAncestor:b];
    
    // get the new downpass sequence
    initializedDownPass = NO;
    [self initializeDownPassSequence];
    
    // set the x-coordinates
    [self setXCoordinates];
}

- (void)buildRandomTreeWithSize:(int)n {

    // find the global random number generator
    RevBayesCore::RandomNumberGenerator* rng = RevBayesCore::GLOBAL_RNG;

    // fill in the vector of nodes
    [nodes removeAllObjects];
    int numNodes = 2 * n - 1;
    for (int i=0; i<numNodes; i++)
        {
        Node* nde = [[Node alloc] init];
        [nde setIndex:i];
        NSString* nameStr;
        if (i < n)
            {
            [nde setIsLeaf:YES];
            nameStr = [[NSString alloc] initWithFormat:@"Taxon %d", i+1];
            }
        else 
            {
            nameStr = @"Interior Node";
            }
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

- (Node*)downPassNodeIndexed:(int)idx {

    return [downPassSequence objectAtIndex:idx];
}

- (void)deroot {

    if ([root numberOfDescendants] == 2)
        {
        initializedDownPass = NO;
        Node* pL = [root descendantIndexed:0];
        Node* pR = [root descendantIndexed:1];
        if ([pL isLeaf] == NO)
            {
            [root removeDescendant:pL];
            for (size_t i=0; i<[pL numberOfDescendants]; i++)
                {
                Node* d = [pL descendantIndexed:i];
                [d setAncestor:root];
                [root addDescendant:d];
                }
            [nodes removeObject:pL];
            [self initializeDownPassSequence];
            }
        else if ([pR isLeaf] == NO)
            {
            [root removeDescendant:pR];
            for (size_t i=0; i<[pR numberOfDescendants]; i++)
                {
                Node* d = [pR descendantIndexed:i];
                [d setAncestor:root];
                [root addDescendant:d];
                }
            [nodes removeObject:pR];
            [self initializeDownPassSequence];
            }
        else
            {
            // error
            }
        }
}

- (void)deselectAllNodes {

	NSEnumerator* enumerator = [nodes objectEnumerator];
	Node* p;
	while ( (p = [enumerator nextObject]) ) 
        {
        [p setIsNodeSelected:NO];
        [p setIsBranchSelected:NO];
        }
}

- (void)encodeWithCoder:(NSCoder*)aCoder {

	[aCoder encodeObject:nodes             forKey:@"nodes"];
    [aCoder encodeObject:downPassSequence  forKey:@"downPassSequence"];
    [aCoder encodeBool:initializedDownPass forKey:@"initializedDownPass"];
    [aCoder encodeInt:numberOfTaxa         forKey:@"numberOfTaxa"];
    [aCoder encodeObject:info              forKey:@"info"];
    [aCoder encodeObject:root              forKey:@"root"];
    [aCoder encodeObject:outgroupName      forKey:@"outgroupName"];
    [aCoder encodeFloat:weight             forKey:@"weight"];
}

- (int)getNumberOfTaxa {

	NSEnumerator* enumerator = [nodes objectEnumerator];
	Node* p = nil;
    int n = 0;
	while ( (p = [enumerator nextObject]) )
        {
        if ([p numberOfDescendants] == 0 )
            n++;
        }
    return n;
}

- (Node*)getRoot {

    return root;
}

- (id)init {

    if ( (self = [super init]) ) 
		{
        nodes = [[NSMutableArray alloc] init];
        downPassSequence = [[NSMutableArray alloc] init];
		info = [[NSString alloc] init];
        initializedDownPass = NO;
        root = nil;
        numberOfTaxa = 0;
        outgroupName = @"";
        weight = 1.0;
		}
    return self;
}

- (id)initWithTipSize:(int)n {

    if ( (self = [super init]) ) 
		{
        nodes = [[NSMutableArray alloc] init];
        downPassSequence = [[NSMutableArray alloc] init];
		info = [[NSString alloc] init];
        initializedDownPass = NO;
        root = nil;
        numberOfTaxa = n;
        outgroupName = @"";
        weight = 1.0;

        [self buildRandomTreeWithSize:n];
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
		nodes               = [aDecoder decodeObjectForKey:@"nodes"];
        downPassSequence    = [aDecoder decodeObjectForKey:@"downPassSequence"];
        initializedDownPass = [aDecoder decodeBoolForKey:@"initializedDownPass"];
        numberOfTaxa        = [aDecoder decodeIntForKey:@"numberOfTaxa"];
        info                = [aDecoder decodeObjectForKey:@"info"];
        root                = [aDecoder decodeObjectForKey:@"root"];
        outgroupName        = [aDecoder decodeObjectForKey:@"outgroupName"];
        weight              = [aDecoder decodeFloatForKey:@"weight"];
		}
	return self;
}

- (id)initWithTree:(GuiTree*)t {

    if ( (self = [super init]) ) 
        {
        
        }
	return self;
}

- (BOOL)isRoot:(Node*)p {

    if (p == root)
        return YES;
    return NO;
}

- (float)maxNameHeightForFont:(NSFont*)f {

    NSDictionary* attrs = [NSDictionary dictionaryWithObjects:[NSArray arrayWithObjects:f, nil]
                                                      forKeys:[NSArray arrayWithObjects:NSFontAttributeName, nil]];
    float heightOfHighestName = 0.0;
    for (int i=0; i<[self numberOfNodes]; i++)
        {
        Node* p = [self downPassNodeIndexed:i];
        if ([p isLeaf] == YES)
            {
            NSString* taxonName = [p name];
            NSAttributedString* attrString = [[NSAttributedString alloc] initWithString:taxonName attributes:attrs];
            NSRect textSize = [attrString boundingRectWithSize:NSMakeSize(1e10, 1e10) options:NSStringDrawingUsesLineFragmentOrigin];
            if (textSize.size.height > heightOfHighestName)
                heightOfHighestName = textSize.size.height;
            }
        }
    return heightOfHighestName;
}

- (float)maxNameWidthForFont:(NSFont*)f {

    NSDictionary* attrs = [NSDictionary dictionaryWithObjects:[NSArray arrayWithObjects:f, nil]
                                                      forKeys:[NSArray arrayWithObjects:NSFontAttributeName, nil]];
    float lengthOfWidestName = 0.0;
    for (int i=0; i<[self numberOfNodes]; i++)
        {
        Node* p = [self downPassNodeIndexed:i];
        if ([p isLeaf] == YES)
            {
            NSString* taxonName = [p name];
            NSAttributedString* attrString = [[NSAttributedString alloc] initWithString:taxonName attributes:attrs];
            NSRect textSize = [attrString boundingRectWithSize:NSMakeSize(1e10, 1e10) options:NSStringDrawingUsesLineFragmentOrigin];
            if (textSize.size.width > lengthOfWidestName)
                lengthOfWidestName = textSize.size.width;
            }
        }
    return lengthOfWidestName;
}

- (void)moveFromBranch:(Node*)fromBranch toBranch:(Node*)toBranch forTreeYCoordinates:(float)c {

    // check that there are from and to branches
    if (fromBranch == nil || toBranch == nil)
        return;
    
    // make certain they aren't the same branch
    if (fromBranch == toBranch)
        return;
        
    // make certain that the part of the tree to be rearranged is binary
    if ([[fromBranch ancestor] numberOfDescendants] != 2)
        return;
        
    // make certain that there is an ancestor for each branch
    if ([fromBranch ancestor] == nil || [toBranch ancestor] == nil)
        return;
        
    // make certain that the from and to branches aren't sister
    if ( [fromBranch ancestor] == [toBranch ancestor] )
        return;
        
    // check to see if we are moving a branch to its ancestral branch, in which
    // case we simply change the time of the ancestor
    if ( [fromBranch ancestor] == toBranch )
        {
        [[fromBranch ancestor] setY:c];
        return;
        }
        
    // remove 'from' branch
    Node* a = [fromBranch ancestor];
    [a removeDescendant:fromBranch];
    [fromBranch setAncestor:nil];
    Node* aa = [a ancestor];
    if (aa != nil)
        {
        [aa removeDescendant:a];
        for (int i=0; i<[a numberOfDescendants]; i++)
            {
            Node* d = [a descendantIndexed:i];
            [aa addDescendant:d];
            [d setAncestor:aa];
            }
        [a removeAllDescendants];
        }
    else 
        {
        if ( [a numberOfDescendants] == 1 )
            {
            Node* d = [a descendantIndexed:0];
            [a removeAllDescendants];
            [d setAncestor:nil];
            [self setRoot:d];
            }
        else
            {
            NSLog(@"Problem here");
            }
        }
    
    // add to the 'to' branch
    Node* b = [toBranch ancestor];
    [b removeDescendant:toBranch];
    [b addDescendant:a];
    [a addDescendant:fromBranch];
    [a addDescendant:toBranch];
    [fromBranch setAncestor:a];
    [toBranch setAncestor:a];
    [a setAncestor:b];
    
    // set the time for the new node
    [a setY:c];
    
    // get the new downpass sequence
    initializedDownPass = NO;
    [self initializeDownPassSequence];
    
    // check to see if we need to rescale all of the y-coordinates
    if ( [root y] > 0.001 )
        {
        double curDepth = [root y];
        NSEnumerator* enumerator = [downPassSequence objectEnumerator];
        Node* p;
        while ( (p = [enumerator nextObject]) ) 
            {
            if ([p isLeaf] == NO)
                {
                float x = 1.0 - [p y];
                float newX = 1.0 - (x / (1.0-curDepth));
                [p setY:newX];
                }
            }
        }
        
    // reset the x-coordinates
    [self setXCoordinates];
}

- (NSString*)newickString {

    NSMutableString* tStr = [[NSMutableString alloc] init];
    [self traverseNewick:root andString:tStr];
    return tStr;
}

- (void)traverseNewick:(Node*)p andString:(NSMutableString*)s {

    if (p != nil)
        {
		if ([p isLeaf] == YES)
			{
            //[s appendFormat:@"%@", [p name]];
            [s appendFormat:@"%d", [p index]+1];
			}
		else
			{
            [s appendString:@"("];
            for (int i=0; i<[p numberOfDescendants]; i++)
                {
                Node* d = [p descendantIndexed:i];
                [self traverseNewick:d andString:s];
                if (i + 1 != [p numberOfDescendants])
                    {
                    [s appendString:@","];
                    }
                }
            [s appendString:@")"];
            }
        }
}

- (Node*)nodeWithIndex:(int)idx {

    for (int j=0; j<[self numberOfNodes]; j++)
        {
        Node* p = (Node*)[nodes objectAtIndex:j];
        if ([p index] == idx)
            return p;
        }
    return nil;
}

- (Node*)nodeWithOffset:(int)idx {

    return [nodes objectAtIndex:idx];
}

- (Node*)nodeWithName:(NSString*)str {

    // find the node in the tree with the name str
    for (int j=0; j<[self numberOfNodes]; j++)
        {
        Node* p = [self downPassNodeIndexed:j];
        NSString* nodeName = [p name];
        if ( [nodeName isEqualToString:str] == YES )
            return p;
        }
    return nil;
}

- (int)numbeOfDownPassNodes {

    return (int)[downPassSequence count];
}

- (int)numberOfNodes {

    return (int)[nodes count];
}

- (void)passDown:(Node*)p {

    if (p != nil)
        {
        for (int i=0; i<[p numberOfDescendants]; i++)
            [self passDown:[p descendantIndexed:i]];
        [downPassSequence addObject:p];
        }
}

- (void)print {

    for (size_t i=0; i<[nodes count]; i++)
        [[nodes objectAtIndex:i] print];
}

- (void)removeSubtreeAboveNode:(Node*)p {
    
    // check if this is reasonable
    if ([p ancestor] == nil)
        return;

    // mark nodes for removal
    NSMutableArray* nodesToRemove = [NSMutableArray arrayWithCapacity:1];
    [self setAllFlagsTo:NO];
    [p setFlag:YES];
    for ( Node* nde in [downPassSequence reverseObjectEnumerator] )
        {
        if ( [nde ancestor] != nil )
            {
            if ( [[nde ancestor] flag] == YES )
                [nde setFlag:YES];
            }
        if ( [nde flag] == YES )
            [nodesToRemove addObject:nde];
        }
        
    // how many taxa will be removed
    int numNodesToRemove = 0;
    for (Node* nde in [nodesToRemove objectEnumerator])
        {
        if ([nde numberOfDescendants] == 0)
            numNodesToRemove++;
        }
    
    // check that too many nodes are not removed
    if ( [self getNumberOfTaxa] - numNodesToRemove <= 2)
        return;
        
    // remove the clade defined by p from the tree;
    Node* anc = [p ancestor];
    Node* ancAnc = [anc ancestor];
    if (ancAnc != nil)
        {
        [anc removeDescendant:p];
        if ([anc numberOfDescendants] == 1)
            {
            Node* d = [anc descendantIndexed:0];
            [ancAnc addDescendant:d];
            [d setAncestor:ancAnc];
            [ancAnc removeDescendant:anc];
            [nodesToRemove addObject:anc];
            }
        else
            {
            NSLog(@"expecting binary tree");
            }
        }
    else
        {
        
        }
        
    // remove the nodes
    for (Node* nde in [nodesToRemove objectEnumerator])
        {
        [nodes removeObject:nde];
        [downPassSequence removeObject:nde];
        /* JPHARC [nde release]; */
        }
        
    // get the new downpass sequence
    initializedDownPass = NO;
    [self initializeDownPassSequence];
    
    // reset the x-coordinates
    [self setXCoordinates];
    
    // re-index the nodes
    int idx = 0;
    for (Node* nde in [nodes objectEnumerator])
        [nde setIndex:idx++];
        
}

- (void)rootTreeOnNodeNamed:(NSString*)name {

    Node* p = [self nodeWithName:name];
    if (p != nil)
        [self rootTreeOnNode:p];
}

- (void)rootTreeOnNode:(Node*)p {

    // set the flags from node p to the root to YES
    [self setAllFlagsTo:NO];
    [p setFlag:YES];
    Node* q = p;
    while ([q ancestor] != nil)
        {
        q = [q ancestor];
        [q setFlag:YES];
        }
        
    // starting from the root, rearrange the nodes, bringing
    // node p successively closer to the root
    BOOL continueRotatingTree = YES;
    do {
        // set q to be the root of the tree
        Node* d = root;
        
        // set u to be the only marked node above q
        Node* u = nil;
        int numFlaggedDescendants = 0;
        for (int i=0; i<[d numberOfDescendants]; i++)
            {
            Node* n = [d descendantIndexed: i];
            if ( [n flag] == YES && n != p )
                {
                u = n;
                numFlaggedDescendants++;
                }
            }
            
        // rearrange nodes, making u the root and d the descendant
        if (u == nil || numFlaggedDescendants != 1)
            {
            continueRotatingTree = NO;
            }
        else
            {
            [d removeDescendant:u];
            [u addDescendant:d];
            [d setAncestor:u];
            [u setAncestor:nil];
            [d setFlag:NO];
            root = u;
            }
        
        } while (continueRotatingTree == YES);
    
    // make certain that the outgroup is the left-most node
    [root removeDescendant:p];
    [root addDescendantAtLeftMostPosition:p];
    
    // determine the downpass sequence
    initializedDownPass = NO;
    [self initializeDownPassSequence];
}

- (void)setAllFlagsTo:(BOOL)tf {

	NSEnumerator* enumerator = [nodes objectEnumerator];
	Node* p = nil;
	while ( (p = [enumerator nextObject]) ) 
        [p setFlag:tf];
}

- (void)setCoordinates:(BOOL)drawMonophyleticWrOutgroup {
		
	if (initializedDownPass == NO)
		[self initializeDownPassSequence];
        
    // initialize the depth of the nodes
	NSEnumerator* enumerator = [downPassSequence objectEnumerator];
	Node* p;
	while ( (p = [enumerator nextObject]) ) 
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
    if (drawMonophyleticWrOutgroup == YES)
        depthOfRoot++;

    // set depth (y values)
	enumerator = [downPassSequence objectEnumerator];
	while ( (p = [enumerator nextObject]) ) 
        {
        [p setY:(1.0 - (float)[p depthFromTip]/depthOfRoot)];
        }
    
	// set x coordinates
	double x = 0.0;
	double maximumX = 0.0;
	double maximumY = 0.0;
	enumerator = [downPassSequence objectEnumerator];
	while ( (p = [enumerator nextObject]) ) 
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
            int startI = 0;
            if (p == root && drawMonophyleticWrOutgroup == YES)
                startI = 1;
			double xMin = 1000000000.0;
			double xMax = 0.0;
            for (int i=startI; i<[p numberOfDescendants]; i++)
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
	while ( (p = [enumerator nextObject]) ) 
        {
        [p setX:([p x]/maximumX)];
        }
}

- (void)setOutgroupName:(NSString*)oName {

    outgroupName = oName;
    [self rootTreeOnNodeNamed:outgroupName];
}

- (void)setXCoordinates {
		
	if (initializedDownPass == NO)
		[self initializeDownPassSequence];
            
	// set x coordinates
	double x = 0.0;
	double maximumX = 0.0;
	NSEnumerator*  enumerator = [downPassSequence objectEnumerator];
    Node* p = nil;
	while ( (p = [enumerator nextObject]) ) 
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
	while ( (p = [enumerator nextObject]) ) 
        {
        [p setX:([p x]/maximumX)];
        }
}

- (void)setNodesToArray:(NSMutableArray*)n {

    [self setNodes:n];
}

- (NSMutableArray*)taxaNames {

    // taxa should be indexed from 0, 1, ..., N-1
    NSMutableArray* names = [[NSMutableArray alloc] init];
    for (size_t i=0; i<[self numberOfTaxa]; i++)
        {
        Node* p = [self nodeWithIndex:(int)i];
        if ([p numberOfDescendants] != 0)
            {
            NSLog(@"Error that should be trapped");
            }
        [names addObject:[p name]];
        }
    /*for (int i=0; i<[self numberOfNodes]; i++)
        {
        Node* p = [self downPassNodeIndexed:i];
        if ([p numberOfDescendants] == 0)
            [names addObject:[p name]];
        }*/
    return names;
}

@end
