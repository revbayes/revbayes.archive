#import "AnalysisView.h"
#import "GuiTree.h"
#import "InOutlet.h"
#import "Node.h"
#import "RevBayes.h"
#import "ToolTreeSet.h"
#import "WindowControllerTreeSet.h"
#import "WindowControllerTreeViewer.h"




@implementation ToolTreeSet

@synthesize myTrees;
@synthesize outgroupIdx;

- (void)addTreeToSet:(GuiTree*)t {

    [myTrees addObject:t];
    if (hasInspectorInfo == NO)
        {
        hasInspectorInfo = YES;
        [myAnalysisView setNeedsDisplay:YES];
        }
}

- (void)awakeFromNib {

}

- (void)closeControlPanel {

    [NSApp stopModal];
	[controlWindow close];
}

- (void)closeInspectorPanel {

    //[NSApp stopModal];
	[treeInspector close];
}

- (void)encodeWithCoder:(NSCoder*)aCoder {

    [aCoder encodeObject:myTrees  forKey:@"myTrees"];
    [aCoder encodeInt:outgroupIdx forKey:@"outgroupIdx"];
    
	[super encodeWithCoder:aCoder];
}

- (BOOL)execute {

    // TO DO: Instantiate trees in core
    
    return YES;
}

- (NSString*)getOutgroupName {
    
    NSString* og = nil;
    if ([myTrees count] > 0)
        {
        GuiTree* t = [myTrees objectAtIndex:0];
        for (int i=0; i<[t numberOfNodes]; i++)
            {
            Node* p = [t downPassNodeIndexed:i];
            if ([p index] == outgroupIdx && [p isLeaf] == YES)
                {
                return [NSString stringWithString:[p name]];
                }
            }
        }
    return og;
}

- (GuiTree*)getTreeIndexed:(int)idx {

    if ([myTrees count] == 0 || idx >= [myTrees count])
        return nil;
    return [myTrees objectAtIndex:idx];
}

- (int)indexOfTaxon:(NSString*)name {

    if ([myTrees count] > 0)
        {
        GuiTree* t = [myTrees objectAtIndex:0];
        for (int i=0; i<[t numberOfNodes]; i++)
            {
            Node* p = [t downPassNodeIndexed:i];
            if ([p isLeaf] == YES)
                {
                if ( [[p name] isEqualToString:name] == YES )
                    return [p index];
                }
            }
        }
    return -1;
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
        numberOfInlets = 1;
		[self addInletOfColor:[NSColor redColor]];
		[self addOutletOfColor:[NSColor redColor]];
        [self setInletLocations];
        [self setOutletLocations];
        [self setOutgroupIdx:0];
        
        // allocate an array to hold the trees
        myTrees = [[NSMutableArray alloc] init];
        
        controlWindow = [[WindowControllerTreeSet alloc] initWithTool:self];
        treeInspector = [[WindowControllerTreeViewer alloc] initWithTool:self];
		}
    return self;
}

- (id)initWithCoder:(NSCoder*)aDecoder {

    if ( (self = [super initWithCoder:aDecoder]) ) 
		{
		// initialize the tool image
		[self initializeImage];
        [self setImageWithSize:itemSize];
        
        // get the set of trees
        outgroupIdx = (int)[aDecoder decodeIntegerForKey:@"outgroupIdx"];
        myTrees = [aDecoder decodeObjectForKey:@"myTrees"];
        if ([myTrees count] > 0)
            hasInspectorInfo = YES;

        controlWindow = [[WindowControllerTreeSet alloc] initWithTool:self];
        treeInspector = [[WindowControllerTreeViewer alloc] initWithTool:self];
		}
	return self;
}

- (void)initializeImage {

    itemImage[0] = [NSImage imageNamed:@"Tool_TreeSummarize25.icns"];
    itemImage[1] = [NSImage imageNamed:@"Tool_TreeSummarize50.icns"];
    itemImage[2] = [NSImage imageNamed:@"Tool_TreeSummarize75.icns"];
    itemImage[3] = [NSImage imageNamed:@"Tool_TreeSummarize100.icns"];
    itemImage[4] = [NSImage imageNamed:@"Tool_TreeSummarize125.icns"];
    itemImage[5] = [NSImage imageNamed:@"Tool_TreeSummarize150.icns"];
    itemImage[6] = [NSImage imageNamed:@"Tool_TreeSummarize200.icns"];
    itemImage[7] = [NSImage imageNamed:@"Tool_TreeSummarize400.icns"];

	float s[8] = { 0.25, 0.50, 0.75, 1.0, 1.25, 1.50, 2.0, 4.0 };
	for (int i=0; i<8; i++)
        [itemImage[i] setSize:NSMakeSize(ITEM_IMAGE_SIZE*s[i], ITEM_IMAGE_SIZE*s[i])];
}

- (int)numberOfTreesInSet {

    return (int)[myTrees count];
}

- (BOOL)readTreesFile {
	
	return NO;
}


- (NSMutableAttributedString*)sendTip {

    NSString* myTip = @" Tree Set Tool ";
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

- (IBAction)okButtonAction:(id)sender {
    
}

- (void)removeAllTreesFromSet {

    [myTrees removeAllObjects];
    hasInspectorInfo = NO;
}

- (int)numberOfInlets {

    return numberOfInlets;
}

- (void)rerootOnTaxonIndexed:(int)newRootIdx {

    [self setOutgroupIdx:newRootIdx];
    

    if ([myTrees count] > 0)
        {
        for (GuiTree* t in myTrees)
            {
            [t setOutgroupIdx:newRootIdx];
            }
        }
}

- (void)setNumberOfInlets:(int)x {

    if (numberOfInlets != x)
        {
        numberOfInlets = x;
        [self removeAllInlets];
        for (int i=0; i<[self numberOfInlets]; i++)
            {
            [self addInletOfColor:[NSColor redColor]];
            [self setInletLocations];
            }
        }
}

- (void)showControlPanel {

    NSPoint p = [self originForControlWindow:[controlWindow window]];
    [[controlWindow window] setFrameOrigin:p];
	[controlWindow showWindow:self];    
	[[controlWindow window] makeKeyAndOrderFront:nil];
    [NSApp runModalForWindow:[controlWindow window]];
}

- (void)showInspectorPanel {

    NSPoint p = [self originForControlWindow:[treeInspector window]];
    [[treeInspector window] setFrameOrigin:p];
	[treeInspector showWindow:self];    
	[[treeInspector window] makeKeyAndOrderFront:nil];
    //[NSApp runModalForWindow:[treeInspector window]];
}

- (NSString*)toolName {

    return @"Tree Set";
}

- (void)updateForChangeInUpstreamState {

    isResolved = NO;
    Tool* parentTool = [self getParentToolOfInletIndexed:0];
    if (parentTool != nil)
        {
        if ([parentTool isResolved] == YES)
            isResolved = YES;
        }
}

- (BOOL)writeTreesFile {
    
	return NO;
}

@end
