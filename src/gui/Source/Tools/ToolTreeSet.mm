#import "InOutlet.h"
#import "RevBayes.h"
#import "ToolTreeSet.h"
#import "WindowControllerTreeSet.h"
#import "WindowControllerTreeViewer.h"




@implementation ToolTreeSet

- (void)addTreeToSet:(Tree*)t {

    NSLog(@"adding %@ to tree set", t);
    [myTrees addObject:t];
    hasInspectorInfo = YES;
}

- (void)awakeFromNib {

}

- (void)closeControlPanel {

    [NSApp stopModal];
	[controlWindow close];
}

- (void)closeInspectorPanel {

    [NSApp stopModal];
	[treeInspector close];
}

- (void)dealloc {
	
	[myTrees release];
    [controlWindow release];
    [treeInspector release];
	[super dealloc];
}

- (void)encodeWithCoder:(NSCoder*)aCoder {

    [aCoder encodeObject:myTrees forKey:@"myTrees"];
	[super encodeWithCoder:aCoder];
}

- (void)execute {

    NSLog(@"Executing tool %@", self);
    
    [self startProgressIndicator];
    
    [self stopProgressIndicator];
}

- (Tree*)getTreeIndexed:(int)idx {

    if ([myTrees count] == 0 || idx >= [myTrees count])
        return nil;
    return [myTrees objectAtIndex:idx];
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
        
        // allocate an array to hold the trees
        myTrees = [[NSMutableArray alloc] init];
        
        controlWindow = [[WindowControllerTreeSet alloc] initWithTool:self];
        treeInspector = [[WindowControllerTreeViewer alloc] initWithTool:self];
		}
    return self;
}

- (id)initWithCoder:(NSCoder *)aDecoder {

    if ( (self = [super initWithCoder:aDecoder]) ) 
		{
		// initialize the tool image
		[self initializeImage];
        [self setImageWithSize:itemSize];
        
        // get the set of trees
        myTrees = [aDecoder decodeObjectForKey:@"myTrees"];
        [myTrees retain];
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

    NSString* myTip = [NSString stringWithString:@" Tree Set Tool "];
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

    [treeInspector initializeTreeInformation];
    NSPoint p = [self originForControlWindow:[treeInspector window]];
    [[treeInspector window] setFrameOrigin:p];
	[treeInspector showWindow:self];    
	[[treeInspector window] makeKeyAndOrderFront:nil];
    [NSApp runModalForWindow:[treeInspector window]];
}

- (BOOL)writeTreesFile {
    
	return NO;
}

@end
