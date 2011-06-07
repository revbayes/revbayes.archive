#import "InOutlet.h"
#import "RevBayes.h"
#import "ToolAlign.h"
#import "WindowControllerAlign.h"


@implementation ToolAlign

- (void)awakeFromNib {

}

- (void)closeControlPanel {

    [NSApp stopModal];
	[controlWindow close];
    [self setInletsAndOutlets];
}

- (void)dealloc {

	[controlWindow release];
	[super dealloc];
}

- (void)encodeWithCoder:(NSCoder *)aCoder {

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
		[self addInletOfColor:[NSColor cyanColor]];
		[self addOutletOfColor:[NSColor greenColor]];
		
		// initialize some variables
		controlWindow = nil;
		
		// initialize the control window
		controlWindow = [[WindowControllerAlign alloc] initWithTool:self];
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
		controlWindow = [[WindowControllerAlign alloc] initWithTool:self];
		}
	return self;
}

- (void)initializeImage {

    itemImage[0] = [NSImage imageNamed:@"Tool_Align25.icns"];
    itemImage[1] = [NSImage imageNamed:@"Tool_Align50.icns"];
    itemImage[2] = [NSImage imageNamed:@"Tool_Align75.icns"];
    itemImage[3] = [NSImage imageNamed:@"Tool_Align100.icns"];
    itemImage[4] = [NSImage imageNamed:@"Tool_Align125.icns"];
    itemImage[5] = [NSImage imageNamed:@"Tool_Align150.icns"];
    itemImage[6] = [NSImage imageNamed:@"Tool_Align200.icns"];
    itemImage[7] = [NSImage imageNamed:@"Tool_Align400.icns"];

	float s[8] = { 0.25, 0.50, 0.75, 1.0, 1.25, 1.50, 2.0, 4.0 };
	for (int i=0; i<8; i++)
        [itemImage[i] setSize:NSMakeSize(ITEM_IMAGE_SIZE*s[i], ITEM_IMAGE_SIZE*s[i])];
}

- (NSMutableAttributedString*)sendTip {

    NSString* myTip = [NSString stringWithString:@" Sequence Alignment Tool "];
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

}

- (void)showControlPanel {

    NSPoint p = [self originForControlWindow:[controlWindow window]];
    [[controlWindow window] setFrameOrigin:p];
	[controlWindow showWindow:self];    
	[[controlWindow window] makeKeyAndOrderFront:nil];
    [NSApp runModalForWindow:[controlWindow window]];
}

@end
