#import "Connection.h"
#import "Inlet.h"
#import "RbData.h"
#import "RbDataCell.h"
#import "RbTaxonData.h"
#import "RevBayes.h"
#import "ToolConcatenate.h"
#import "ToolData.h"
#import "WindowControllerConcatenate.h"




@implementation ToolConcatenate

@synthesize useMinimalSet;
@synthesize matchUsingNames;

- (void)awakeFromNib {

}

- (void)assembleNames:(NSMutableArray*)names usingMethod:(int)overlapMethod fromMatrices:(NSMutableArray*)alignedData {

    [names removeAllObjects];
    
    if (overlapMethod == MINIMAL_OVERLAP_METHOD)
        {
        // find the set of names that are found in all of the data matrices
        // (i.e., find the intersection of the names among all of the data matrices
        RbData* d0 = [alignedData objectAtIndex:0];
        for (int n=0; n<[d0 numTaxa]; n++)
            {
            if ([d0 isTaxonExcluded:n] == NO)
                {
                NSString* queryName = [d0 taxonWithIndex:n];
                BOOL taxonPresentInAllMatrices = YES;
                for (int i=1; i<[alignedData count]; i++)
                    {
                    RbData* di = [alignedData objectAtIndex:i];
                    if ([di isTaxonNamePresent:queryName] == NO)
                        taxonPresentInAllMatrices = NO;
                    }
                if (taxonPresentInAllMatrices == YES)
                    [names addObject:queryName];
                }
            }
        }
    else
        {
        // find the names that are found in at least one of the data matrices
        // (i.e., find the union of the names among all of the data matrices
        for (int i=0; i<[alignedData count]; i++)
            {
            RbData* di = [alignedData objectAtIndex:i];
            for (int n=0; n<[di numTaxa]; n++)
                {
                NSString* queryName = [di taxonWithIndex:n];
                BOOL foundTaxon = NO;
                for (NSString* str in [names objectEnumerator])
                    {
                    if ([str isEqualToString:queryName] == YES)
                        foundTaxon = YES;
                    }
                if (foundTaxon == NO)
                    [names addObject:queryName];
                }
            }
        }
}

- (void)closeControlPanel {

    [NSApp stopModal];
	[controlWindow close];
}

- (NSMutableArray*)concatenateMatrices:alignedData forTaxa:(NSMutableArray*)names usingMergeMethod:(int)mergeMethod {

    NSMutableArray* arrayWithMatrices = [[NSMutableArray alloc] init];
    if (mergeMethod == MERGE_BY_DATA_TYPE)
        {
        // merge by data type
        BOOL dataTypePresent[6] = { NO, NO, NO, NO, NO, NO };
        for (RbData* d in [alignedData objectEnumerator])
            {
            int dt = [d dataType];
            if (dt >= 0 && dt < 6)
                dataTypePresent[dt] = YES;
            else
                NSLog(@"Problem concatenating data because of a matrix of unknown type");
            }
            
        for (int i=0; i<6; i++)
            {
            int nc = -1;
            if (dataTypePresent[i] == YES)
                {
                RbData* newD = [[RbData alloc] init];
                for (NSString* name in [names objectEnumerator])
                    {
                    RbTaxonData* td = [[RbTaxonData alloc] init];
                    for (RbData* d in [alignedData objectEnumerator])
                        {
                        if ([d dataType] == i)
                            {
                            // begin
                            RbTaxonData* tdToCopy = [d getDataForTaxonWithName:name];
                            if (tdToCopy != nil)
                                {
                                for (int i=0; i<[tdToCopy numCharacters]; i++)
                                    {
                                    RbDataCell* c = [tdToCopy dataCellIndexed:i];
                                    RbDataCell* newC = [[RbDataCell alloc] initWithCell:c];
                                    [td addObservation:newC];
                                    }
                                }
                            else
                                {
                                for (int i=0; i<[d numCharactersForTaxon:0]; i++)
                                    {
                                    RbDataCell* newC = [[RbDataCell alloc] init];
                                    [newC setDiscreteStateTo:(-1)];
                                    [td addObservation:newC];
                                    }
                                }
                                // end
                            }
                        }
                    [newD addTaxonData:td];
                    if (nc < 0)
                        {
                        nc = [td numCharacters];
                        [newD setNumCharacters:nc];
                        }
                    else
                        {
                        if (nc != [td numCharacters])
                            NSLog(@"problem with inconsistent sizes of rows in data matrix");
                        }
                   }
                }
            }
        
        }
    else
        {
        // unconditionally merge
        RbData* newD = [[RbData alloc] init];
        [newD setNumTaxa:(int)([names count])];
        [newD setName:@"Concatenated Data Matrix"];
        int nc = -1;
        for (NSString* name in [names objectEnumerator])
            {
            RbTaxonData* td = [[RbTaxonData alloc] init];
            [td setTaxonName:name];
            [newD addTaxonName:name];
            for (RbData* d in [alignedData objectEnumerator])
                {
                RbTaxonData* tdToCopy = [d getDataForTaxonWithName:name];
                if (tdToCopy != nil)
                    {
                    for (int i=0; i<[tdToCopy numCharacters]; i++)
                        {
                        RbDataCell* c = [tdToCopy dataCellIndexed:i];
                        RbDataCell* newC = [[RbDataCell alloc] initWithCell:c];
                        [td addObservation:newC];
                        }
                    }
                else
                    {
                    for (int i=0; i<[d numCharactersForTaxon:0]; i++)
                        {
                        RbDataCell* newC = [[RbDataCell alloc] init];
                        [newC setDiscreteStateTo:(-1)];
                        [td addObservation:newC];
                        }
                    }
                }
            [newD addTaxonData:td];
            if (nc < 0)
                {
                nc = [td numCharacters];
                [newD setNumCharacters:nc];
                }
            else
                {
                if (nc != [td numCharacters])
                    NSLog(@"problem with inconsistent sizes of rows in data matrix");
                }
            }
            
        // determine data type for this matrix
        int dt = -1;
        BOOL allSame = YES;
        for (RbData* d in [alignedData objectEnumerator])
            {
            if (dt < 0)
                {
                dt = [d dataType];
                }
            else
                {
                if ( [d dataType] != dt )
                    allSame = NO;
                }
            }
        if (allSame == YES)
            [newD setDataType:dt];
        else
            [newD setDataType:MIXED];
        
        [newD setNumCharacters:nc];
        [newD print];
        [arrayWithMatrices addObject:newD];
        }
        
    return arrayWithMatrices;
}

- (void)concatenateWithOverlap:(int)overlapMethod andMergeMethod:(int)mergeMethod  {

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
    [self removeDataInspector];

    // calculate how many aligned data matrices exist
    NSMutableArray* alignedData = [NSMutableArray arrayWithCapacity:1];
    for (int i=0; i<[dataTool numDataMatrices]; i++)
        {
        if ( [[dataTool dataMatrixIndexed:i] isHomologyEstablished] == YES )
            [alignedData addObject:[dataTool dataMatrixIndexed:i]];
        }
                
    // merge the data and put it into a single RbData object
    if ( [alignedData count] == 0 )
        {
        // no data object to concatenate
        return;
        }
    else if ( [alignedData count] == 1 )
        {
        // only a single data matrix to concatenate
        RbData* d = [[RbData alloc] initWithRbData:[alignedData objectAtIndex:0]];
        [self addMatrix:d];
        }
    else
        {
        // concatenating at least two matrices
        NSMutableArray* names = [NSMutableArray arrayWithCapacity:0];
        [self assembleNames:names usingMethod:overlapMethod fromMatrices:alignedData];
        NSMutableArray* concatenatedMatrices = [self concatenateMatrices:alignedData forTaxa:names usingMergeMethod:mergeMethod];
        for (RbData* d in [concatenatedMatrices objectEnumerator])
            [self addMatrix:d];
        NSLog(@"%@", concatenatedMatrices);
        [concatenatedMatrices release];
        }
    NSLog(@"%@", dataMatrices);
    
    [self makeDataInspector];
}

- (void)dealloc {

	[controlWindow release];
	[super dealloc];
}

- (void)encodeWithCoder:(NSCoder *)aCoder {

	[aCoder encodeBool:useMinimalSet   forKey:@"useMinimalSet"];
	[aCoder encodeBool:matchUsingNames forKey:@"matchUsingNames"];

	[super encodeWithCoder:aCoder];
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
		[self addOutletOfColor:[NSColor greenColor]];
        [self setInletLocations];
        [self setOutletLocations];

		// initialize some variables
        useMinimalSet   = YES;
        matchUsingNames = YES;
		
		// initialize the control window
		controlWindow = [[WindowControllerConcatenate alloc] initWithTool:self];
		}
    return self;
}

- (id)initWithCoder:(NSCoder *)aDecoder {

    if ( (self = [super initWithCoder:aDecoder]) ) 
		{
		// initialize the tool image
		[self initializeImage];
        [self setImageWithSize:itemSize];
        
        useMinimalSet   = [aDecoder decodeBoolForKey:@"useMinimalSet"];
        matchUsingNames = [aDecoder decodeBoolForKey:@"matchUsingNames"];

		// initialize the control window
		controlWindow = [[WindowControllerConcatenate alloc] initWithTool:self];
		}
	return self;
}

- (void)initializeImage {

    itemImage[0] = [NSImage imageNamed:@"Tool_Concatenate25.icns"];
    itemImage[1] = [NSImage imageNamed:@"Tool_Concatenate50.icns"];
    itemImage[2] = [NSImage imageNamed:@"Tool_Concatenate75.icns"];
    itemImage[3] = [NSImage imageNamed:@"Tool_Concatenate100.icns"];
    itemImage[4] = [NSImage imageNamed:@"Tool_Concatenate125.icns"];
    itemImage[5] = [NSImage imageNamed:@"Tool_Concatenate150.icns"];
    itemImage[6] = [NSImage imageNamed:@"Tool_Concatenate200.icns"];
    itemImage[7] = [NSImage imageNamed:@"Tool_Concatenate400.icns"];

	float s[8] = { 0.25, 0.50, 0.75, 1.0, 1.25, 1.50, 2.0, 4.0 };
	for (int i=0; i<8; i++)
        [itemImage[i] setSize:NSMakeSize(ITEM_IMAGE_SIZE*s[i], ITEM_IMAGE_SIZE*s[i])];
}

- (int)numberOfIncomingAlignments {

    return 0;
}

- (int)mininumNumberOfIncomingSequences {

    return 0;
}

- (int)maximumNumberOfIncomingSequences {

    return 0;
}

- (NSMutableAttributedString*)sendTip {

    NSString* myTip = @" Sequence Concatenation Tool ";
    if ([self isResolved] == YES)
        myTip = [myTip stringByAppendingFormat:@"\n Status: Resolved \n # Matrices: %d ", [self numDataMatrices]];
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
	//[controlWindow setMatrixList];
	[controlWindow showWindow:self];    
	[[controlWindow window] makeKeyAndOrderFront:nil];
    [NSApp runModalForWindow:[controlWindow window]];
}

- (void)updateForChangeInState {

    [self startProgressIndicator];
    
    // set the tool state to unresolved
    [self setIsResolved:NO];
    
    // set up an array of outlets from parent tools
    NSMutableArray* dataOutlets = [NSMutableArray arrayWithCapacity:1];
    for (int i=0; i<[self numInlets]; i++)
        {
        Inlet* theInlet = [self inletIndexed:i];
        Tool* t = [self getParentToolOfInlet:theInlet];
        if (t != nil)
            {
            if ( [t isKindOfClass:[ToolData class]] == YES )
                {
                [dataOutlets addObject:[theInlet getMyOutlet]];
                }
            }
        }
        
	// update the state of this tool depending upon the state/presence of the parent tool
	if ( [dataOutlets count] == 0 )
		{
		// we don't have a parent tool that contains data
		[self removeAllDataMatrices];
		}
	else 
		{
NSLog(@" 1 ");
		// we have parent data-containing tool(s)
        
        // for the hell of it, we'll delete everything and freshly concatenate the sequences
NSLog(@" 2 ");
		//[self removeAllDataMatrices];
NSLog(@" 3 ");
        
        // concatenate
        [self concatenateWithOverlap:[controlWindow matchingMethod] andMergeMethod:[controlWindow mergeMethod]];
NSLog(@" 4 ");
        
        if ( [dataMatrices count] > 0 )
            {
            [self setIsResolved:YES];
            [self makeDataInspector];
            }
NSLog(@" 5 ");
		}
                
    [self stopProgressIndicator];
}

@end
