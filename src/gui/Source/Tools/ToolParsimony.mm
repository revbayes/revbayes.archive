#import "AnalysisView.h"
#import "Connection.h"
#import "Inlet.h"
#import "Node.h"
#import "Outlet.h"
#import "RbData.h"
#import "RbDataCell.h"
#import "RbTaxonData.h"
#import "RevBayes.h"
#import "ToolData.h"
#import "ToolParsimony.h"
#import "ToolTreeSet.h"
#import "GuiTree.h"
#import "WindowControllerParsimony.h"

#include <iomanip>
#include <iostream>
#include <vector>



@implementation ToolParsimony

@synthesize numTreesVisited;

- (void)closeControlPanel {

    [NSApp stopModal];
	[controlWindow close];
}

/* JPHARC - (void)dealloc {

    [bestTrees release];
    [super dealloc];
} */

- (void)encodeWithCoder:(NSCoder*)aCoder {
    
	[super encodeWithCoder:aCoder];
}

- (BOOL)execute {

    BOOL isSuccessful = [self exhaustiveSearch];
    return isSuccessful;
}

- (BOOL)exhaustiveSearch {

    // find the parent of this tool, which should be an instance of ToolData
    ToolData* dataTool = nil;
    for (size_t i=0; i<[inlets count]; i++)
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
        return NO;

    // calculate how many aligned data matrices exist
    NSMutableArray* alignedData = [NSMutableArray arrayWithCapacity:1];
    for (size_t i=0; i<[dataTool numDataMatrices]; i++)
        {
        if ( [[dataTool dataMatrixIndexed:i] isHomologyEstablished] == YES )
            [alignedData addObject:[dataTool dataMatrixIndexed:i]];
        }
        
    // get a pointer to the single RbData object
    RbData* d = nil;
    if ( [alignedData count] == 0 )
        {
        return NO;
        }
    else if ( [alignedData count] == 1 )
        {
        d = [alignedData objectAtIndex:0];
        }
    else
        {
        return NO;
        }
        
    // check to see if a tree container is downstream of this tool. If so, then purge
    // it of trees
    for (size_t i=0; i<[outlets count]; i++)
        {
        Outlet* theOutlet = [outlets objectAtIndex:i];
        for (int j=0; j<[theOutlet numberOfConnections]; j++)
            {
            Connection* c = [theOutlet connectionWithIndex:j];
            Tool* t = [[c inlet] toolOwner];
            if ( [t isKindOfClass:[ToolTreeSet class]] == YES )
                {
                if ( [(ToolTreeSet*)t numberOfTreesInSet] > 0 )
                    [(ToolTreeSet*)t removeAllTreesFromSet];
                }
            }
        }
                
    // how many taxa/nodes/characters
    int numTaxa = [d numTaxa] - [d numExcludedTaxa];
    int numNodes = 2 * numTaxa - 2;
    numCharacters = [d numCharacters] - [d numExcludedCharacters];
    NSLog(@"numTaxa=%d numCharacters=%d numNodes=%d", numTaxa, numCharacters, numNodes);
        
    // set up vectors of unsigned ints holding the data for parsimony calculations
    stateSets = new unsigned*[numNodes];
    stateSets[0] = new unsigned[numNodes*numCharacters];
    stateSetsPtr = new unsigned*[numNodes];
    for (int i=1; i<numNodes; i++)
        stateSets[i] = stateSets[i-1] + numCharacters;
    for (int i=0; i<numNodes; i++)
        stateSetsPtr[i] = &stateSets[i][0];
    for (int i=0; i<numNodes; i++)
        for (int j=0; j<numCharacters; j++)
            stateSets[i][j] = 0;
    for (int i=0, i1=0; i<[d numTaxa]; i++)
        {
        if ([d isTaxonExcluded:i] == NO)
            {
            for (int j=0, j1=0; j<[d numCharacters]; j++)
                {
                if ([d isCharacterExcluded:j] == NO)
                    {
                    RbDataCell* c = [d cellWithRow:i andColumn:j];
                    if ( [c isGapState] == YES || [c isAmbig] == YES )
                        {
                        unsigned x = 0;
                        for (size_t s=0; s<[c numStates]; s++)
                            x += (int)(pow(2.0, (double)s));
                        stateSets[i1][j1] = x;
                        }
                    else
                        {
                        stateSets[i1][j1] = [c unsignedRepresentation];
                        }
                    j1++;
                    }
                }
            i1++;
            }
        }
    
    for (int j=0; j<numCharacters; j++)
        {
        std::cout << std::setw(4) << j << " -- ";
        for (int i=0; i<numTaxa; i++)
            {
            std::cout << std::setw(2) << stateSets[i][j];
            }
        std::cout << std::endl;
        }
    
    // allocate the nodes
    NSMutableArray* nodeContainer = [NSMutableArray arrayWithCapacity:numNodes];
    NSMutableArray* availableTips = [NSMutableArray arrayWithCapacity:1];
    NSMutableArray* availableInts = [NSMutableArray arrayWithCapacity:1];
    for (int i=0, k=0; i<[d numTaxa]; i++)
        {
        if ([d isTaxonExcluded:i] == NO)
            {
            Node* nde = [[Node alloc] init];
            [nde setIndex:k++];
            [nde setName:[d taxonWithIndex:i]];
            [nde setIsLeaf:YES];
            [availableTips addObject:nde];
            [nodeContainer addObject:nde];
            /* JPHARC [nde release]; */
            }
        }
    for (int i=numTaxa; i<numNodes; i++)
        {
        Node* nde = [[Node alloc] init];
        [nde setIndex:i];
        [availableInts addObject:nde];
        [nodeContainer addObject:nde];
        /* JPHARC [nde release]; */
        }
    
    // set up a simple three species tree
    NSMutableArray* currentTree = [NSMutableArray arrayWithCapacity:3];
    Node* r = [nodeContainer objectAtIndex:numTaxa];
    [r setIsRoot:YES];
    [currentTree addObject:r];
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
    GuiTree* t = [[GuiTree alloc] init];
    [t setNodesToArray:nodeContainer];
    [t setRoot:r];
        
    // recursively visit all of the possible trees
    numTreesVisited = 0;
    scoreOfBestTree = 2000000000;
    [bestTrees removeAllObjects];
    [self addTaxonFromList:availableTips toTree:currentTree usingSpareNodes:availableInts treeObject:t];
    
    delete [] stateSets[0];
    delete [] stateSets;
    delete [] stateSetsPtr;
    
    // drop the best tree(s) into the tree set tool, if we are connected to one
    // find the parent of this tool, which should be an instance of ToolData
    ToolTreeSet* treeSetTool = nil;
    for (int i=0; i<[self numOutlets]; i++)
        {
        Outlet* o = [self outletIndexed:i];
        for (int j=0; j<[o numberOfConnections]; j++)
            {
            Connection* c = [o connectionWithIndex:j];
            Tool* daughterTool = [[c inlet] toolOwner];
            if ( [daughterTool isKindOfClass:[ToolTreeSet class]] == YES )
                treeSetTool = (ToolTreeSet*)daughterTool;
            }
        }
    if ( treeSetTool == nil )
        return YES;
        
    for (size_t i=0; i<[bestTrees count]; i++)
        {
        NSMutableArray* treeNodeData = [NSKeyedUnarchiver unarchiveObjectWithData:[bestTrees objectAtIndex:i]];
        /* JPHARC [treeNodeData retain]; */
        GuiTree* newTree = [[GuiTree alloc] init];
        [newTree setNodesToArray:treeNodeData];
        Node* newRoot = nil;
        for (size_t j=0; j<[treeNodeData count]; j++)
            {
            if ( [[treeNodeData objectAtIndex:j] isRoot] == YES )
                newRoot = [treeNodeData objectAtIndex:j];
            }
        if (newRoot == nil)
            NSLog(@"problem finding root!");
        [newTree setRoot:newRoot];
        NSString* treeStr = [NSString stringWithFormat:@"Parsimony length = %d", scoreOfBestTree];
        [newTree setInfo:treeStr];
        [treeSetTool addTreeToSet:newTree];
        }

    /* JPHARC if ( [alignedData count] > 1 )
        [d release]; */
    
    return YES;
}

- (int)parsimonyScoreForTree:(GuiTree*)t {

    // NOTE: We assume that the tree is binary. This is a reasonable assumption, as the exhaustive search enumerates
    //       all of the binary trees.
    int len = 0;
        
    for (int n=0; n<[t numberOfNodes]; n++)
        {
        Node* p = [t downPassNodeIndexed:n];
        if ([p numberOfDescendants] > 0)
            {
#           if 1
            // left-most descendant first
            Node* pU = [p descendantIndexed:0];
            unsigned* s  = &stateSets[[p  index]][0];
            unsigned* sU = &stateSets[[pU index]][0];
            for (int c=0; c<numCharacters; c++)
                {
                (*s) = (*sU);
                s++;
                sU++;
                }
                
            // other descendants
            for (int i=1; i<[p numberOfDescendants]; i++)
                {
                pU = [p descendantIndexed:i];
                s  = &stateSets[[p  index]][0];
                sU = &stateSets[[pU index]][0];
                for (int c=0; c<numCharacters; c++)
                    {
                    if ( ((*s) & (*sU)) == 0 )
                        {
                        len++;
                        unsigned v = ((*s) | (*sU));
                        (*s) = v;
                        }
                    else
                        {
                        (*s) &= (*sU);
                        }
                    s++;
                    sU++;
                    }
                }
                
#           else
            if ([p numberOfDescendants] != 2)
                NSLog(@"Warning: %d descendant nodes! (%d)", [p numberOfDescendants], [t isRoot:p]);
            Node* pL = [p descendantIndexed:0];
            Node* pR = [p descendantIndexed:1];
            unsigned* sL = stateSetsPtr[[pL index]];
            unsigned* sR = stateSetsPtr[[pR index]];
            unsigned* s  = stateSetsPtr[[p index]];
            for (int c=0; c<numCharacters; c++)
                {
                if ( ((*sL) & (*sR)) == 0 )
                    {
                    len++;
                    (*s) = ((*sL) | (*sR));
                    }
                else 
                    {
                    (*s) = ((*sL) & (*sR));
                    }
                s++;
                sL++;
                sR++;
                }
                
            if ( [p ancestor] == nil )
                {
                pR = [p descendantIndexed:2];
                s  = stateSetsPtr[[p index]];
                sR = stateSetsPtr[[pR index]];
                for (int c=0; c<numCharacters; c++)
                    {
                    if ( ((*s) & (*sR)) == 0 )
                        len++;
                    s++;
                    sR++;
                    }
                }
#           endif
            }
        }

    return len;
}

- (void)addTaxonFromList:(NSMutableArray*)availableTaxa toTree:(NSMutableArray*)nodes usingSpareNodes:(NSMutableArray*)spares treeObject:(GuiTree*)t {

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
        for (size_t i=0; i<[nodes count]; i++)
            {
            Node* b = [nodes objectAtIndex:i];
            if ( [b isRoot] == NO )
                {
                Node* a = [b ancestor];
                
                // add p and q to b
                [a removeDescendant:b];
                [a addDescendant:q];
                [q setAncestor:a];
                [q addDescendant:b];
                [q addDescendant:p];
                [b setAncestor:q];
                [p setAncestor:q];
                
                if ([availableTips count] == 0)
                    {
                    [t setInitializedDownPass:NO];
                    [t initializeDownPassSequence];
                    int len = [self parsimonyScoreForTree:t];
                    if (len < scoreOfBestTree)
                        {
                        [bestTrees removeAllObjects];
                        scoreOfBestTree = len;
                        NSData* copiedTree = [NSKeyedArchiver archivedDataWithRootObject:currentTree];
                        [bestTrees addObject:copiedTree];
                        //[self printNodeInformationForTree:t atSite:0];
                        }
                    else if (len == scoreOfBestTree)
                        {
                        NSData* copiedTree = [NSKeyedArchiver archivedDataWithRootObject:currentTree];
                        [bestTrees addObject:copiedTree];
                        }
                    numTreesVisited++;
                    [myAnalysisView setNeedsDisplay:YES];
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
        bestTrees = [[NSMutableArray alloc] init];
		
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
        bestTrees = [[NSMutableArray alloc] init];
            
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

- (void)printNodeInformationForTree:(GuiTree*)t atSite:(int)site {

    for (int n=0; n<[t numberOfNodes]; n++)
        {
        Node* p = [t downPassNodeIndexed:n];
        [p print];
        std::cout << "   state set = " << stateSets[[p index]][site] << " ( ";
        for (int i=0; i<[p numberOfDescendants]; i++)
            {
            Node* q = [p descendantIndexed:i];
            std::cout << stateSets[[q index]][site] << " ";
            }
        std::cout << ")" << std::endl;
        }
}

- (NSMutableAttributedString*)sendTip {

    NSString* myTip = @" Parsimony Tool ";
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

- (NSString*)toolName {

    return @"Parsimony";
}

- (void)updateForChangeInUpstreamState {

    isResolved = YES;
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
        isResolved = NO;

    // calculate how many aligned data matrices exist
    NSMutableArray* alignedData = [NSMutableArray arrayWithCapacity:1];
    for (int i=0; i<[dataTool numDataMatrices]; i++)
        {
        if ( [[dataTool dataMatrixIndexed:i] isHomologyEstablished] == YES )
            [alignedData addObject:[dataTool dataMatrixIndexed:i]];
        }
    if ( [alignedData count] != 1 )
        isResolved = NO;
}

@end
