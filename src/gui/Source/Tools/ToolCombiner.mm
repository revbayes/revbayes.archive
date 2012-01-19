#import "RevBayes.h"
#import "ToolCombiner.h"
#import "WindowControllerCombiner.h"



@implementation ToolCombiner

@synthesize numberOfInlets;
@synthesize startingNumberOfInlets;

- (void)closeControlPanel {

    [NSApp stopModal];
	[controlWindow close];
}

- (void)encodeWithCoder:(NSCoder*)aCoder {
    
    //
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
        [self setNumberOfInlets:2];
		[self addInletOfColor:[NSColor greenColor]];
		[self addInletOfColor:[NSColor greenColor]];
		[self addOutletOfColor:[NSColor greenColor]];
        [self setInletLocations];
        [self setOutletLocations];
            
		// initialize the control window
		controlWindow = [[WindowControllerCombiner alloc] initWithTool:self];
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
		//controlWindow = [[WindowControllerAlign alloc] initWithTool:self];
		}
	return self;
}

- (void)initializeImage {

    itemImage[0] = [NSImage imageNamed:@"Tool_Combiner25.icns"];
    itemImage[1] = [NSImage imageNamed:@"Tool_Combiner50.icns"];
    itemImage[2] = [NSImage imageNamed:@"Tool_Combiner75.icns"];
    itemImage[3] = [NSImage imageNamed:@"Tool_Combiner100.icns"];
    itemImage[4] = [NSImage imageNamed:@"Tool_Combiner125.icns"];
    itemImage[5] = [NSImage imageNamed:@"Tool_Combiner150.icns"];
    itemImage[6] = [NSImage imageNamed:@"Tool_Combiner200.icns"];
    itemImage[7] = [NSImage imageNamed:@"Tool_Combiner400.icns"];

	float s[8] = { 0.25, 0.50, 0.75, 1.0, 1.25, 1.50, 2.0, 4.0 };
	for (int i=0; i<8; i++)
        [itemImage[i] setSize:NSMakeSize(ITEM_IMAGE_SIZE*s[i], ITEM_IMAGE_SIZE*s[i])];
}

- (NSMutableAttributedString*)sendTip {

    NSString* myTip = [NSString stringWithString:@" Character Matrix Combiner Tool "];
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

- (void)setInletsAndOutlets {

    [self removeAllInlets];
    for (int i=0; i<[self numberOfInlets]; i++)
        {
		[self addInletOfColor:[NSColor greenColor]];
        [self setInletLocations];
        }
}

- (void)showControlPanel {

    startingNumberOfInlets = [self numberOfInlets];
    [controlWindow setNumberOfInlets:[self numberOfInlets]];
    NSPoint p = [self originForControlWindow:[controlWindow window]];
    [[controlWindow window] setFrameOrigin:p];
	[controlWindow showWindow:self];    
	[[controlWindow window] makeKeyAndOrderFront:nil];
    [NSApp runModalForWindow:[controlWindow window]];
}

@end
