#import "Connection.h"
#import "Inlet.h"
#import "Node.h"
#import "RbData.h"
#import "RbDataCell.h"
#import "RevBayes.h"
#import "ToolData.h"
#import "ToolParsimony.h"
#import "Tree.h"
#import "WindowControllerParsimony.h"

#include <vector>



@implementation ToolParsimony

- (void)closeControlPanel {

    [NSApp stopModal];
	[controlWindow close];
}

- (void)encodeWithCoder:(NSCoder*)aCoder {
    
	[super encodeWithCoder:aCoder];
}

- (void)execute {

}

- (void)exhaustiveSearch {

    // find the parent of this tool, which should be an instance of ToolData
    ToolData* dataTool = nil;
    for (int i=0; i<[inlets count]; i++)
        {
        Inlet* theInlet = [inlets objectAtIndex:i];
        for (int j=0; j<[theInlet numberOfConnections]; j++)
            {
            Connection* c = [theInlet connectionWithIndex:j];
            Tool* t = [[c outlet] toolOwner];
            if ( [t isKindOfClass:[ToolData class]] == YES )
                dataTool = (ToolData*)t;
            }
        }
    if ( dataTool == nil )
        return;

    // calculate how many aligned data matrices exist
    NSMutableArray* alignedData = [NSMutableArray arrayWithCapacity:1];
    for (int i=0; i<[dataTool numDataMatrices]; i++)
        {
        if ( [[dataTool dataMatrixIndexed:i] isHomologyEstablished] == YES )
            [alignedData addObject:[dataTool dataMatrixIndexed:i]];
        }
    if ( [alignedData count] != 1)
        return;
        
    // get a pointer to the single RbData object
    RbData* d = [alignedData objectAtIndex:0];
    
    // how many taxa/nodes/characters
    int numTaxa = [d numTaxa];
    int numNodes = 2 * numTaxa - 2;
    int numCharacters = [d numCharacters];
    
    // set up vectors of unsigned ints holding the data for parsimony calculations
    stateSets.resize(numNodes);
    for (int i=0; i<stateSets.size(); i++)
        stateSets[i].resize(numCharacters);
    for (int i=0; i<numNodes; i++)
        for (int j=0; j<numCharacters; j++)
            stateSets[i][j] = 0;
    for (int i=0; i<numTaxa; i++)
        {
        for (int j=0; j<numCharacters; j++)
            {
            RbDataCell* c = [d cellWithRow:i andColumn:j];
            stateSets[i][j] = [c unsignedRepresentation];
            }
        }
    
    // allocate the nodes
    NSMutableArray* nodeContainer = [NSMutableArray arrayWithCapacity:numNodes];
    NSMutableArray* availableTips = [NSMutableArray arrayWithCapacity:1];
    NSMutableArray* availableInts = [NSMutableArray arrayWithCapacity:1];
    for (int i=0; i<numNodes; i++)
        {
        Node* nde = [[Node alloc] init];
        [nde setIndex:i];
        if (i < numTaxa)
            {
            [nde setName:[d taxonWithIndex:i]];
            [nde setIsLeaf:YES];
            [availableTips addObject:nde];
            }
        else 
            {
            [availableInts addObject:nde];
            }
        [nodeContainer addObject:nde];
        [nde release];
        }
    
    // set up a simple three species tree
    NSMutableArray* currentTree = [NSMutableArray arrayWithCapacity:3];
    Node* r = [nodeContainer objectAtIndex:numTaxa];
    [availableInts removeObject:r];
    for (int i=0; i<3; i++)
        {
        Node* p = [nodeContainer objectAtIndex:i];
        [availableTips removeObject:p];
        [currentTree addObject:p];
        [p setAncestor:r];
        [r addDescendant:p];
        }

    // set up the tree object (only used to deal with down pass sequences here
    Tree* t = [[Tree alloc] init];
    [t setNodesToArray:nodeContainer];
    [t setRoot:r];
        
    // recursively visit all of the possible trees
    numTreesVisited = 0;
    [self addTaxonFromList:availableTips toTree:currentTree usingSpareNodes:availableInts treeObject:t];
    
    
    
    
    
    
    for (int i=0; i<numNodes; i++)
        {
        Node* p = [nodeContainer objectAtIndex:i];
        [p print];
        }
    
    
}

- (int)parsimonyScoreForTree:(Tree*)t {

    int len = 0;
    
    for (int n=0; n<[t numberOfNodes]; n++)
        {
        Node* p = [t downPassNodeIndexed:n];
        if ([p isLeaf] == NO)
            {
            int c = 0;
            unsigned s0 = stateSets[ [[p descendantIndexed:0] index] ][c];
            BOOL isIntersectionEmpty = NO;
            for (int i=1; i<[p numberOfDescendants]; i++)
                {
                unsigned si = stateSets[ [[p descendantIndexed:i] index] ][c];
                if ( (s0 & si) == 0 )
                    {
                    len++;
                    s0 |= si;
                    isIntersectionEmpty = YES;
                    }
                }
            }
        }

    return len;
}

- (void)addTaxonFromList:(NSMutableArray*)availableTaxa toTree:(NSMutableArray*)nodes usingSpareNodes:(NSMutableArray*)spares treeObject:(Tree*)t {

    if ([availableTaxa count] > 0)
        {
        // get next taxon from list, along with a spare node
        Node* p = [availableTaxa objectAtIndex:0];
        Node* q = [spares objectAtIndex:0];
        
        // make new lists of available taxa and spare nodes
        NSMutableArray* availableTips = [NSMutableArray arrayWithArray:availableTaxa];
        NSMutableArray* availableInts = [NSMutableArray arrayWithArray:spares];
        NSMutableArray* currentTree   = [NSMutableArray arrayWithArray:nodes];
        [availableTips removeObject:p];
        [availableInts removeObject:q];
        [currentTree addObject:p];
        [currentTree addObject:q];
        
        // loop over the branches of the current tree to which p and q will be added
        for (int i=0; i<[nodes count]; i++)
            {
            Node* b = [nodes objectAtIndex:i];
            Node* a = [b ancestor];
            
            // add p and q to b
            [a removeDescendant:b];
            [a addDescendant:q];
            [q setAncestor:a];
            [q addDescendant:b];
            [q addDescendant:p];
            [b setAncestor:q];
            [p setAncestor:p];
            
            if ([availableTips count] == 0)
                {
                [t initializeDownPassSequence];
                //int len = [self parsimonyScoreForTree:t];
                NSLog(@"Visiting tree %d -- %d", ++numTreesVisited, 0);
                }
            
            // add next tip recursively
            [self addTaxonFromList:availableTips toTree:currentTree usingSpareNodes:availableInts treeObject:t];
            
            // undo addition of p and q to b
            [a removeDescendant:q];
            [a addDescendant:b];
            [b setAncestor:a];
            [q removeDescendant:b];
            [q removeDescendant:p];
            }
        }
}


- (id)init {

    self = [self initWithScaleFactor:1.0];
    return self;
}

- (id)initWithScaleFactor:(float)sf {

    if ( (self = [super initWithScaleFactor:sf]) ) 
		{
		// initialize the tool image
		[self initializeImage];
        [self setImageWithSize:itemSize];
		
		// initialize the inlet/outlet information
		[self addInletOfColor:[NSColor greenColor]];
		[self addOutletOfColor:[NSColor redColor]];
        [self setInletLocations];
        [self setOutletLocations];
            
		// initialize the control window
		controlWindow = [[WindowControllerParsimony alloc] initWithTool:self];
		}
    return self;
}

- (id)initWithCoder:(NSCoder*)aDecoder {

    if ( (self = [super initWithCoder:aDecoder]) ) 
		{
        // initialize the tool image
		[self initializeImage];
        [self setImageWithSize:itemSize];
            
        // initialize the control window
		controlWindow = [[WindowControllerParsimony alloc] initWithTool:self];
		}
	return self;
}

- (void)initializeImage {

    itemImage[0] = [NSImage imageNamed:@"Tool_Parsimony25.icns"];
    itemImage[1] = [NSImage imageNamed:@"Tool_Parsimony50.icns"];
    itemImage[2] = [NSImage imageNamed:@"Tool_Parsimony75.icns"];
    itemImage[3] = [NSImage imageNamed:@"Tool_Parsimony100.icns"];
    itemImage[4] = [NSImage imageNamed:@"Tool_Parsimony125.icns"];
    itemImage[5] = [NSImage imageNamed:@"Tool_Parsimony150.icns"];
    itemImage[6] = [NSImage imageNamed:@"Tool_Parsimony200.icns"];
    itemImage[7] = [NSImage imageNamed:@"Tool_Parsimony400.icns"];

	float s[8] = { 0.25, 0.50, 0.75, 1.0, 1.25, 1.50, 2.0, 4.0 };
	for (int i=0; i<8; i++)
        [itemImage[i] setSize:NSMakeSize(ITEM_IMAGE_SIZE*s[i], ITEM_IMAGE_SIZE*s[i])];
}

- (NSMutableAttributedString*)sendTip {

    NSString* myTip = [NSString stringWithString:@" Parsimony Tool "];
    if ([self isResolved] == YES)
        myTip = [myTip stringByAppendingString:@"\n Status: Resolved "];
    else 
        myTip = [myTip stringByAppendingString:@"\n Status: Unresolved "];
    if ([self isFullyConnected] == YES)
        myTip = [myTip stringByAppendingString:@"\n Fully Connected "];
    else 
        myTip = [myTip stringByAppendingString:@"\n Missing Connections "];

    NSDictionary *attr = [NSDictionary 
                 dictionaryWithObjects:[NSArray arrayWithObjects:[NSFont fontWithName:@"Lucida Grande Bold" size:14.0], [[NSColor whiteColor] colorWithAlphaComponent:1.0], nil] 
                               forKeys: [NSArray arrayWithObjects:NSFontAttributeName,NSForegroundColorAttributeName, nil]];

    NSMutableAttributedString* attrString = [[NSMutableAttributedString alloc] initWithString:myTip attributes:attr];

    return attrString;
}

- (void)showControlPanel {

    NSPoint p = [self originForControlWindow:[controlWindow window]];
    [[controlWindow window] setFrameOrigin:p];
	[controlWindow showWindow:self];    
	[[controlWindow window] makeKeyAndOrderFront:nil];
    [NSApp runModalForWindow:[controlWindow window]];
}

@end
