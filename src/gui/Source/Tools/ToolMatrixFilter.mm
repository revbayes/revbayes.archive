#import "InOutlet.h"
#import "RbData.h"
#import "RevBayes.h"
#import "ToolMatrixFilter.h"
#import "ToolReadData.h"
#import "WindowControllerCharacterMatrix.h"
#import "WindowControllerMatrixFilter.h"


@implementation ToolMatrixFilter

@synthesize dataMatrices;

- (void)addMatrix:(RbData*)m {

	[dataMatrices addObject:m];
    hasInspectorInfo = YES;
}

- (void)awakeFromNib {

}

- (void)closeControlPanel {

    [NSApp stopModal];
	[controlWindow close];
    [self setInletsAndOutlets];
	[self signalDownstreamTools];
	//[self updateForConnectionChange];
}

- (RbData*)dataMatrixIndexed:(int)i {

	if (i > [dataMatrices count])
		return nil;
	return [dataMatrices objectAtIndex:i];
}

- (void)dealloc {

	[controlWindow release];
	[dataMatrices release];
	[super dealloc];
}

- (void)encodeWithCoder:(NSCoder *)aCoder {

	[aCoder encodeObject:dataMatrices forKey:@"dataMatrices"];
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
		
		// initialize the array holding the data matrices
		dataMatrices = [[NSMutableArray alloc] init];

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
		dataMatrices = [aDecoder decodeObjectForKey:@"dataMatrices"];
		[dataMatrices retain];

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

- (int)numDataMatrices {

	return (int)[dataMatrices count];
}

- (void)removeAllDataMatrices {

	[dataMatrices removeAllObjects];
    hasInspectorInfo = NO;
}

- (void)setOutlets {

	if ( [controlWindow numberOfOutlets] != [self numOutlets] )
		{
		[self removeAllOutlets];
		for (int i=0; i<[controlWindow numberOfOutlets]; i++)
			{
			[self addOutletOfColor:[NSColor greenColor]];
			}
		}
}

- (NSMutableAttributedString*)sendTip {

    NSString* myTip = [NSString stringWithString:@" Matrix Filter Tool "];
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

- (void)setInletsAndOutlets {

}

- (void)showControlPanel {

    NSPoint p = [self originForControlWindow:[controlWindow window]];
    [[controlWindow window] setFrameOrigin:p];
	[controlWindow setMatrixList];
	[controlWindow showWindow:self];    
	[[controlWindow window] makeKeyAndOrderFront:nil];
    [NSApp runModalForWindow:[controlWindow window]];
}

- (void)showInspectorPanel {

    if (dataInspector != nil)
        [dataInspector release];
    dataInspector = [[WindowControllerCharacterMatrix alloc] initWithTool:self];
    [[dataInspector window] center];
    [dataInspector showWindow:self];
}

- (void)updateForConnectionChange {

    // set the tool state to unresolved
    [self setIsResolved:NO];
    
	// attempt to get a pointer to the parent tool
    ToolReadData* t = (ToolReadData*)[self getToolOfInletIndexed:0];
	
	// update the state of this tool depending upon the state/presence of the parent tool
	if (t == nil)
		{
		// we don't have a parent tool
		[self removeAllDataMatrices];
		}
	else 
		{
		// there is a parent tool 
		
		// check that the tool is a ReadData tool...the only connection possible should be to a ReadData tool
		NSString* className = NSStringFromClass([t class]); 
		if ( [className isEqualToString:@"ToolReadData"] == NO )
			return;
			
		// remove any data matrices the filter may contain
		[self removeAllDataMatrices];
		
		// copy the data matrices to this control window's tool
		if ( [t numDataMatrices] > 0 )
			{
			for (int i=0; i<[t numDataMatrices]; i++)
				{
				RbData* d = [t dataMatrixIndexed:i];
				RbData* nd = [[RbData alloc] initWithRbData:d];
				[self addMatrix:nd];
				}
			}
        [self setIsResolved:YES];
		}
        
    // call the update for the super class, to pass on the call to downstream tools
	[super updateForConnectionChange];
}

@end
