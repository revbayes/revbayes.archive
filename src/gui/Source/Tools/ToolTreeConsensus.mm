#import "GuiTree.h"
#import "RevBayes.h"
#import "ToolTreeConsensus.h"
#import "TreeTaxonBipartitions.h"
#import "WindowControllerTreeConsensusViewer.h"




@implementation ToolTreeConsensus

@synthesize isConsensusTreeWindowOpen;
@synthesize consensusTree;

- (void)addTree:(GuiTree*)t {

    [myParts addPartitionsForTree:t];
    if (isConsensusTreeWindowOpen == YES)
        {
        [self setConsensusTree:[myParts consensusTree]];
        [treeInspector update];
        }
}

- (void)awakeFromNib {

}

- (BOOL)checkForExecute:(NSMutableDictionary*)errors {

    return YES;
}

- (BOOL)checkForWarning:(NSMutableDictionary*)warnings {

    return YES;
}

- (void)closeControlPanel {

    [NSApp stopModal];
	//[controlWindow close];
}

- (void)closeInspectorPanel {

	[treeInspector close];
    isConsensusTreeWindowOpen = NO;
}

- (void)encodeWithCoder:(NSCoder*)aCoder {

    [aCoder encodeObject:consensusTree forKey:@"consensusTree"];
    //[aCoder encodeObject:outgroupName forKey:@"outgroupName"];
    
	[super encodeWithCoder:aCoder];
}

- (BOOL)execute {

    // instantiate trees in core
    
    return [super execute];
}

- (GuiTree*)getConsensusTree {

    return consensusTree;
}

- (id)init {

    self = [self initWithScaleFactor:1.0];
    return self;
}

- (id)initWithScaleFactor:(float)sf {

    if ( (self = [super initWithScaleFactor:sf]) ) 
		{
        hasInspectorInfo = YES;
        
		// initialize the tool image
		[self initializeImage];
        [self setImageWithSize:itemSize];

		// initialize the inlet/outlet information
		[self addInletOfColor:[NSColor brownColor]];
        [self setInletLocations];
        [self setOutletLocations];
            
        // allocate an array to hold the trees
        myParts = [[TreeTaxonBipartitions alloc] init];
        consensusTree = [[GuiTree alloc] init];
        
        treeInspector = [[WindowControllerTreeConsensusViewer alloc] initWithTool:self];
        isConsensusTreeWindowOpen = NO;
        //controlWindow = [[WindowControllerTreeSet alloc] initWithTool:self];
		}
    return self;
}

- (id)initWithCoder:(NSCoder*)aDecoder {

    if ( (self = [super initWithCoder:aDecoder]) ) 
		{
        hasInspectorInfo = YES;

		// initialize the tool image
		[self initializeImage];
        [self setImageWithSize:itemSize];
        
        myParts = [[TreeTaxonBipartitions alloc] init];
        consensusTree = [aDecoder decodeObjectForKey:@"consensusTree"];
        
        treeInspector = [[WindowControllerTreeConsensusViewer alloc] initWithTool:self];
        isConsensusTreeWindowOpen = NO;
        //controlWindow = [[WindowControllerTreeSet alloc] initWithTool:self];
		}
	return self;
}

- (void)initializeImage {

    itemImage[0] = [NSImage imageNamed:@"Tool_TreeConsensus25.icns"];
    itemImage[1] = [NSImage imageNamed:@"Tool_TreeConsensus50.icns"];
    itemImage[2] = [NSImage imageNamed:@"Tool_TreeConsensus75.icns"];
    itemImage[3] = [NSImage imageNamed:@"Tool_TreeConsensus100.icns"];
    itemImage[4] = [NSImage imageNamed:@"Tool_TreeConsensus125.icns"];
    itemImage[5] = [NSImage imageNamed:@"Tool_TreeConsensus150.icns"];
    itemImage[6] = [NSImage imageNamed:@"Tool_TreeConsensus200.icns"];
    itemImage[7] = [NSImage imageNamed:@"Tool_TreeConsensus400.icns"];

	float s[8] = { 0.25, 0.50, 0.75, 1.0, 1.25, 1.50, 2.0, 4.0 };
	for (int i=0; i<8; i++)
        [itemImage[i] setSize:NSMakeSize(ITEM_IMAGE_SIZE*s[i], ITEM_IMAGE_SIZE*s[i])];
}

- (NSMutableAttributedString*)sendTip {

    NSString* myTip = @" Tree Consensus Tool ";
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

- (IBAction)okButtonAction:(id)sender {
    
}

- (void)prepareForExecution {

    //[self removeAllTreesFromSet];
}

- (void)showControlPanel {

   /* NSPoint p = [self originForControlWindow:[controlWindow window]];
    [[controlWindow window] setFrameOrigin:p];
	[controlWindow showWindow:self];    
	[[controlWindow window] makeKeyAndOrderFront:nil];
    [NSApp runModalForWindow:[controlWindow window]];*/
}

- (void)showInspectorPanel {

    if ([myParts numSamples] != 0)
        {
        [self setConsensusTree:[myParts consensusTree]];
        [treeInspector update];
        }

    NSPoint p = [self originForControlWindow:[treeInspector window]];
    [[treeInspector window] setFrameOrigin:p];
	[treeInspector showWindow:self];    
	[[treeInspector window] makeKeyAndOrderFront:nil];
    isConsensusTreeWindowOpen = YES;
}

- (NSString*)toolName {

    return @"Tree Consensus";
}

- (void)updateForChangeInParent {

    //[self removeAllTreesFromSet];
}


@end
