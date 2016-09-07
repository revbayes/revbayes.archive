#import "AnalysisView.h"
#import "Inlet.h"
#import "InOutlet.h"
#import "RbData.h"
#import "RevBayes.h"
#import "ToolMatrixFilter.h"
#import "ToolReadData.h"
#import "WindowControllerCharacterMatrix.h"
#import "WindowControllerMatrixFilter.h"


@implementation ToolMatrixFilter

- (void)awakeFromNib {

}

- (BOOL)checkForExecute:(NSMutableDictionary*)errors {

    // find the parent tool
    NSMutableArray* parents = [self getParentTools];
    if ([parents count] == 0)
        {
        NSString* obId = [NSString stringWithFormat:@"%p", self];
        [errors setObject:@"Matrix Filter Tool does not have a parent" forKey:obId];
        return NO;
        }
    else if ([parents count] > 1)
        {
        NSString* obId = [NSString stringWithFormat:@"%p", self];
        [errors setObject:@"Matrix Filter Tool has too many parents" forKey:obId];
        return NO;
        }
    if ( [[parents objectAtIndex:0] isKindOfClass:[ToolData class]] == NO )
        {
        NSString* obId = [NSString stringWithFormat:@"%p", self];
        [errors setObject:@"Matrix Filter Tool does not have a data tool as a parent" forKey:obId];
        return NO;
        }
    ToolData* dataTool = (ToolData*)[parents objectAtIndex:0];
    
    // check the data matrices in the parent tool
    if ( [dataTool numAligned] == 0)
        {
        NSString* obId = [NSString stringWithFormat:@"%p", self];
        [errors setObject:@"The parent of the Matrix Filter Tool does not have any data" forKey:obId];
        return NO;
        }
    if ( [dataTool numUnaligned] > 0)
        {
        NSString* obId = [NSString stringWithFormat:@"%p", self];
        [errors setObject:@"The parent of the Matrix Filter Tool contains unaligned data" forKey:obId];
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

- (void)encodeWithCoder:(NSCoder *)aCoder {

	[super encodeWithCoder:aCoder];
}

- (BOOL)execute {

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
		controlWindow = nil;
		
		// initialize the control window
		controlWindow = [[WindowControllerMatrixFilter alloc] initWithTool:self];
		}
    return self;
}

- (id)initWithCoder:(NSCoder *)aDecoder {

    if ( (self = [super initWithCoder:aDecoder]) ) 
		{
		// initialize the tool image
		[self initializeImage];
        [self setImageWithSize:itemSize];

		// initialize the control window
		controlWindow = [[WindowControllerMatrixFilter alloc] initWithTool:self];
		}
	return self;
}

- (void)initializeImage {

    itemImage[0] = [NSImage imageNamed:@"Tool_MatrixFilter25.icns"];
    itemImage[1] = [NSImage imageNamed:@"Tool_MatrixFilter50.icns"];
    itemImage[2] = [NSImage imageNamed:@"Tool_MatrixFilter75.icns"];
    itemImage[3] = [NSImage imageNamed:@"Tool_MatrixFilter100.icns"];
    itemImage[4] = [NSImage imageNamed:@"Tool_MatrixFilter125.icns"];
    itemImage[5] = [NSImage imageNamed:@"Tool_MatrixFilter150.icns"];
    itemImage[6] = [NSImage imageNamed:@"Tool_MatrixFilter200.icns"];
    itemImage[7] = [NSImage imageNamed:@"Tool_MatrixFilter400.icns"];

	float s[8] = { 0.25, 0.50, 0.75, 1.0, 1.25, 1.50, 2.0, 4.0 };
	for (int i=0; i<8; i++)
        [itemImage[i] setSize:NSMakeSize(ITEM_IMAGE_SIZE*s[i], ITEM_IMAGE_SIZE*s[i])];
}

- (void)prepareForExecution {

}

- (BOOL)resolveStateOnWindowOK {

    [analysisView markAllToolsAsClean];
    [self setIsDirty:YES];
    [self updateChildrenTools];
    return YES;
}

- (NSMutableAttributedString*)sendTip {

    NSString* myTip = @" Matrix Filter Tool ";
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

    [controlWindow updateButtons];
    NSPoint p = [self originForControlWindow:[controlWindow window]];
    [[controlWindow window] setFrameOrigin:p];
	[controlWindow setMatrixList];
	[controlWindow showWindow:self];    
	[[controlWindow window] makeKeyAndOrderFront:nil];
    [NSApp runModalForWindow:[controlWindow window]];
}

- (NSString*)toolName {

    return @"Matrix Filter";
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
		}
	else 
		{
		// we have parent data-containing tool(s)
        
        // check to see if our current data is simply a copy of the data in the parents, in which case
        // we don't need to do anything
        NSMutableArray* parentDataMatrices = [NSMutableArray arrayWithCapacity:1];
        for (int i=0; i<[dataOutlets count]; i++)
            {
            Outlet* ol = [dataOutlets objectAtIndex:i];
            ToolData* t = (ToolData*)[ol toolOwner];
            if ( [ol toolColor] == [NSColor greenColor] )
                {
                [parentDataMatrices addObjectsFromArray:[t getAlignedData]];
                }
            else if ( [ol toolColor] == [NSColor cyanColor] )
                {
                [parentDataMatrices addObjectsFromArray:[t getUnalignedData]];
                }
            }
        int numFound = 0;
        for (int i=0; i<[dataMatrices count]; i++)
            {
            RbData* myDataMatrix = [dataMatrices objectAtIndex:i];
            for (int j=0; j<[parentDataMatrices count]; j++)
                {
                RbData* parentDataMatrix = [parentDataMatrices objectAtIndex:j];
                if ( parentDataMatrix == [myDataMatrix copiedFrom] )
                    {
                    numFound++;
                    break;
                    }
                }
            }
            
        // remove all of the data matrices if each and every data matrix in this tool is not
        // a copy of the data matrices in the parents
        if ( [parentDataMatrices count] != numFound || [parentDataMatrices count] != [dataMatrices count] )
            {
            [self removeAllDataMatrices];
            for (int i=0; i<[parentDataMatrices count]; i++)
                {
				RbData* d = [parentDataMatrices objectAtIndex:i];
				RbData* nd = [[RbData alloc] initWithRbData:d];
				[self addMatrix:nd];
                }
            }
            
        if ( [dataMatrices count] > 0 )
            {
            [self makeDataInspector];
            }
		}
                
    [self stopProgressIndicator];
}

@end
