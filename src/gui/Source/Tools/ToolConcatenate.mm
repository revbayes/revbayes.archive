#import "Connection.h"
#import "Inlet.h"
#import "Outlet.h"
#import "RbData.h"
#import "RbDataCell.h"
#import "RbTaxonData.h"
#import "RevBayes.h"
#import "ToolConcatenate.h"
#import "ToolData.h"
#import "WindowControllerConcatenate.h"




@implementation ToolConcatenate

@synthesize matchMethod;
@synthesize mergeMethod;

- (void)awakeFromNib {

}

- (void)assembleNames:(NSMutableArray*)names usingMethod:(int)overlapMethod fromMatrices:(NSMutableArray*)alignedData {

    [names removeAllObjects];
    
    if (overlapMethod == INTERSECTION_OVERLAP_METHOD)
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
                for (size_t i=1; i<[alignedData count]; i++)
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
        for (size_t i=0; i<[alignedData count]; i++)
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

- (BOOL)checkForExecute:(NSMutableDictionary*)errors {

    // find the parent tool
    NSMutableArray* parents = [self getParentTools];
    if ([parents count] == 0)
        {
        NSString* obId = [NSString stringWithFormat:@"%p", self];
        [errors setObject:@"Concatenate Tool does not have a parent" forKey:obId];
        return NO;
        }
    else if ([parents count] > 1)
        {
        NSString* obId = [NSString stringWithFormat:@"%p", self];
        [errors setObject:@"Concatenate Tool has too many parents" forKey:obId];
        return NO;
        }
    if ( [[parents objectAtIndex:0] isKindOfClass:[ToolData class]] == NO )
        {
        NSString* obId = [NSString stringWithFormat:@"%p", self];
        [errors setObject:@"Concatenate Tool does not have a data tool as a parent" forKey:obId];
        return NO;
        }
    ToolData* dataTool = (ToolData*)[parents objectAtIndex:0];
    
    // check the data matrices in the parent tool
    if ( [dataTool numAligned] == 0)
        {
        NSString* obId = [NSString stringWithFormat:@"%p", self];
        [errors setObject:@"The parent of the Concatenate Tool does not have any data" forKey:obId];
        return NO;
        }
    if ( [dataTool numUnaligned] > 0)
        {
        NSString* obId = [NSString stringWithFormat:@"%p", self];
        [errors setObject:@"The parent of the Concatenate Tool contains unaligned data" forKey:obId];
        return NO;
        }

    return YES;
}

- (BOOL)checkForWarning:(NSMutableDictionary*)warnings {

    return YES;
}

- (void)closeControlPanel {

    [NSApp stopModal];
	[controlWindow close];
}

- (NSMutableArray*)concatenateMatrices:alignedData forTaxa:(NSMutableArray*)names usingMergeMethod:(int)mm {

    NSMutableArray* arrayWithMatrices = [[NSMutableArray alloc] init];
    if (mm == MERGE_BY_DATA_TYPE)
        {
        // merge by data type
        
        // find which types of data are present
        BOOL dataTypePresent[6] = { NO, NO, NO, NO, NO, NO };
        for (RbData* d in [alignedData objectEnumerator])
            {
            int dt = [d dataType];
            if (dt >= 0 && dt < 6)
                dataTypePresent[dt] = YES;
            else
                NSLog(@"Problem concatenating data because of a matrix of unknown type");
            }
            
        // merge matrices of the same data type
        for (size_t i=0; i<6; i++)
            {
            int nc = -1;
            if (dataTypePresent[i] == YES)
                {
                RbData* newD = [[RbData alloc] init];
                [arrayWithMatrices addObject:newD];
                [newD setIsHomologyEstablished:YES];
                [newD setNumTaxa:(int)([names count])];
                [newD setDataType:(int)i];
                [newD setName:@"Matrices concatenated by data type"];
                [newD setStateLabels:@""];
                
                for (NSString* name in [names objectEnumerator])
                    {
                    RbTaxonData* td = [[RbTaxonData alloc] init];
                    NSString* tdName = [[NSString alloc] initWithString:name];
                    [td setTaxonName:tdName];
                    [td setDataType:(int)i];
                    int k = 0;
                    for (RbData* d in [alignedData objectEnumerator])
                        {
                        if ([d dataType] == i)
                            {
                            // begin
                            if ([[newD stateLabels] isEqualToString:@""] == YES)
                                [newD setStateLabels:[d stateLabels]];
                            
                            RbTaxonData* tdToCopy = [d getDataForTaxonWithName:name];
                            if (tdToCopy != nil)
                                {
                                for (size_t j=0; j<[tdToCopy numCharacters]; j++)
                                    {
                                    RbDataCell* c = [tdToCopy dataCellIndexed:(int)j];
                                    RbDataCell* newC = [[RbDataCell alloc] initWithCell:c];
                                    if ([d isCharacterExcluded:(int)j] == YES)
                                        [newD excludeCharacterIndexed:k];
                                    [td addObservation:newC];
                                    k++;
                                    }
                                }
                            else
                                {
                                for (size_t j=0; j<[d numCharactersForTaxon:0]; j++)
                                    {
                                    RbDataCell* newC = [[RbDataCell alloc] init];
                                    [newC setDiscreteStateTo:(-1)];
                                    if ([d isCharacterExcluded:(int)j] == YES)
                                        [newD excludeCharacterIndexed:k];
                                    [td addObservation:newC];
                                    k++;
                                    }
                                }
                            if ([d isHomologyEstablished] == NO)
                                [newD setIsHomologyEstablished:NO];
                            // end
                            }
                        }
                        
                    [newD addTaxonName:name];
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
        [newD setIsHomologyEstablished:YES];
        [newD setNumTaxa:(int)([names count])];
        [newD setName:@"Matrices unconditionally concatenated"];
        int nc = -1;
        for (NSString* name in [names objectEnumerator])
            {
            RbTaxonData* td = [[RbTaxonData alloc] init];
            [td setTaxonName:name];
            [newD addTaxonName:name];
            int k = 0;
            for (RbData* d in [alignedData objectEnumerator])
                {
                RbTaxonData* tdToCopy = [d getDataForTaxonWithName:name];
                if (tdToCopy != nil)
                    {
                    for (int i=0; i<[tdToCopy numCharacters]; i++)
                        {
                        RbDataCell* c = [tdToCopy dataCellIndexed:i];
                        RbDataCell* newC = [[RbDataCell alloc] initWithCell:c];
                        if ([d isCharacterExcluded:i] == YES)
                            [newD excludeCharacterIndexed:k];
                        [td addObservation:newC];
                        k++;
                        }
                    }
                else
                    {
                    for (int i=0; i<[d numCharactersForTaxon:0]; i++)
                        {
                        RbDataCell* newC = [[RbDataCell alloc] init];
                        [newC setDiscreteStateTo:(-1)];
                        if ([d isCharacterExcluded:i] == YES)
                            [newD excludeCharacterIndexed:k];
                        [td addObservation:newC];
                        k++;
                        }
                    }
                if ([d isHomologyEstablished] == NO)
                    [newD setIsHomologyEstablished:NO];
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
        //[newD print];
        [arrayWithMatrices addObject:newD];
        }
    
    return arrayWithMatrices;
}

- (BOOL)concatenateWithOverlap:(int)match andMergeMethod:(int)merge  {
    
    // find the parent of this tool, which should be an instance of ToolData
    ToolData* dataTool = nil;
    for (size_t i=0; i<[inlets count]; i++)
        {
        Inlet* theInlet = [inlets objectAtIndex:i];
        for (size_t j=0; j<[theInlet numberOfConnections]; j++)
            {
            Connection* c = [theInlet connectionWithIndex:(int)j];
            Tool* t = [[c outlet] toolOwner];
            if ( [t isKindOfClass:[ToolData class]] == YES )
                dataTool = (ToolData*)t;
            }
        }
    if ( dataTool == nil )
        {
        [self removeAllDataMatrices];
        [self removeDataInspector];
        return NO;
        }
    
    // calculate how many aligned data matrices exist
    NSMutableArray* alignedData = [NSMutableArray arrayWithCapacity:1];
    for (size_t i=0; i<[dataTool numDataMatrices]; i++)
        {
        if ( [[dataTool dataMatrixIndexed:i] isHomologyEstablished] == YES )
            [alignedData addObject:[dataTool dataMatrixIndexed:i]];
        }
    
    // we make a new concatenated matrix, so we remove the old matrices and inspector
    [self removeAllDataMatrices];
    [self removeDataInspector];

    // merge the data and put it into a single RbData object
    if ( [alignedData count] == 0 )
        {
        // no data object to concatenate
        return NO;
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
        [self assembleNames:names usingMethod:match fromMatrices:alignedData];
        if ( [names count] < 3 )
            {
            NSAlert* alert = [[NSAlert alloc] init];
            [alert setMessageText:@"Error Concatenating Sequences"];
            [alert setInformativeText:@"Too few taxa would result in the concatenated data matrix with the current settings."];
            [alert runModal];
            return NO;
            }
        NSMutableArray* concatenatedMatrices = [self concatenateMatrices:alignedData forTaxa:names usingMergeMethod:merge];
        for (RbData* d in [concatenatedMatrices objectEnumerator])
            [self addMatrix:d];
        }
    
    [self makeDataInspector];
    
    return YES;
}

- (void)encodeWithCoder:(NSCoder *)aCoder {

	[aCoder encodeInt:matchMethod forKey:@"matchMethod"];
    [aCoder encodeInt:mergeMethod forKey:@"mergeMethod"];

	[super encodeWithCoder:aCoder];
}

- (BOOL)execute {

    return [self concatenateWithOverlap:matchMethod andMergeMethod:mergeMethod];
    return [super execute];
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
        matchMethod = INTERSECTION_OVERLAP_METHOD;
        mergeMethod = MERGE_BY_DATA_TYPE;
		
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
        
        matchMethod = [aDecoder decodeIntForKey:@"matchMethod"];
        mergeMethod = [aDecoder decodeIntForKey:@"mergeMethod"];

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

    // find the parent data tool
    ToolData* dataTool = nil;
    for (size_t i=0; i<[inlets count]; i++)
        {
        Inlet* theInlet = [inlets objectAtIndex:i];
        for (size_t j=0; j<[theInlet numberOfConnections]; j++)
            {
            Connection* c = [theInlet connectionWithIndex:(int)j];
            Tool* t = [[c outlet] toolOwner];
            if ( [t isKindOfClass:[ToolData class]] == YES )
                dataTool = (ToolData*)t;
            }
        }
    if ( dataTool == nil )
        return -1;

    int na = 0;
    for (size_t i=0; i<[dataTool numDataMatrices]; i++)
        {
        if ( [[dataTool dataMatrixIndexed:i] isHomologyEstablished] == YES )
            na++;
        }

    return na;
}

- (int)mininumNumberOfIncomingSequences {

    // find the parent data tool
    ToolData* dataTool = nil;
    for (size_t i=0; i<[inlets count]; i++)
        {
        Inlet* theInlet = [inlets objectAtIndex:i];
        for (size_t j=0; j<[theInlet numberOfConnections]; j++)
            {
            Connection* c = [theInlet connectionWithIndex:(int)j];
            Tool* t = [[c outlet] toolOwner];
            if ( [t isKindOfClass:[ToolData class]] == YES )
                dataTool = (ToolData*)t;
            }
        }
    if ( dataTool == nil )
        return -1;

    int n = -1;
    for (size_t i=0; i<[dataTool numDataMatrices]; i++)
        {
        if ( [[dataTool dataMatrixIndexed:i] isHomologyEstablished] == YES )
            {
            int x = [[dataTool dataMatrixIndexed:i] numTaxa];
            if (n == -1)
                n = x;
            else if ( x < n )
                n = x;
            }
        }

    return n;
}

- (int)maximumNumberOfIncomingSequences {

    // find the parent data tool
    ToolData* dataTool = nil;
    for (size_t i=0; i<[inlets count]; i++)
        {
        Inlet* theInlet = [inlets objectAtIndex:i];
        for (size_t j=0; j<[theInlet numberOfConnections]; j++)
            {
            Connection* c = [theInlet connectionWithIndex:(int)j];
            Tool* t = [[c outlet] toolOwner];
            if ( [t isKindOfClass:[ToolData class]] == YES )
                dataTool = (ToolData*)t;
            }
        }
    if ( dataTool == nil )
        return -1;

    int n = 0;
    for (size_t i=0; i<[dataTool numDataMatrices]; i++)
        {
        if ( [[dataTool dataMatrixIndexed:i] isHomologyEstablished] == YES )
            {
            int x = [[dataTool dataMatrixIndexed:i] numTaxa];
            if ( x > n )
                n = x;
            }
        }

    return n;
}

- (void)prepareForExecution {

}

- (BOOL)resolveStateOnWindowOK {

    BOOL isSuccessful = [self concatenateWithOverlap:matchMethod andMergeMethod:mergeMethod];
    [self updateChildrenTools];
    if (isSuccessful == NO)
        return NO;
    return YES;
}

- (NSMutableAttributedString*)sendTip {

    NSString* myTip = @" Sequence Concatenation Tool ";
    myTip = [myTip stringByAppendingFormat:@"\n # Matrices: %d ", (int)[self numDataMatrices]];
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

- (NSString*)toolName {

    return @"Concatenate Data";
}

- (void)updateForChangeInParent {
    
    [self startProgressIndicator];
    
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
        //[self updateChildrenTools];
		}
	else 
		{
		// we have parent data-containing tool(s)
        
        // for the hell of it, we'll delete everything and freshly concatenate the sequences
		[self removeAllDataMatrices];
        
        // concatenate
        [self concatenateWithOverlap:[self matchMethod] andMergeMethod:[self mergeMethod]];
        
        if ( [dataMatrices count] > 0 )
            {
            [self makeDataInspector];
            }
        //[self updateChildrenTools];
		}
                
    [self stopProgressIndicator];
}

@end
