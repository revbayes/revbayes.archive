#import "InOutlet.h"
#import "RevBayes.h"
#import "ToolSimulate.h"


@implementation ToolSimulate

- (void)awakeFromNib {

}

- (void)dealloc {

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
		[self addInletOfColor:[NSColor blueColor]];
		[self addOutletOfColor:[NSColor greenColor]];
		}
    return self;
}

- (id)initWithCoder:(NSCoder *)aDecoder {

    if ( (self = [super initWithCoder:aDecoder]) ) 
		{
		// initialize the tool image
		[self initializeImage];
        [self setImageWithSize:itemSize];
		}
	return self;
}

- (void)initializeImage {

    itemImage[0] = [NSImage imageNamed:@"Tool_Simulate25.icns"];
    itemImage[1] = [NSImage imageNamed:@"Tool_Simulate50.icns"];
    itemImage[2] = [NSImage imageNamed:@"Tool_Simulate75.icns"];
    itemImage[3] = [NSImage imageNamed:@"Tool_Simulate100.icns"];
    itemImage[4] = [NSImage imageNamed:@"Tool_Simulate125.icns"];
    itemImage[5] = [NSImage imageNamed:@"Tool_Simulate150.icns"];
    itemImage[6] = [NSImage imageNamed:@"Tool_Simulate200.icns"];
    itemImage[7] = [NSImage imageNamed:@"Tool_Simulate400.icns"];

	float s[8] = { 0.25, 0.50, 0.75, 1.0, 1.25, 1.50, 2.0, 4.0 };
	for (int i=0; i<8; i++)
        [itemImage[i] setSize:NSMakeSize(ITEM_IMAGE_SIZE*s[i], ITEM_IMAGE_SIZE*s[i])];
}

- (NSMutableAttributedString*)sendTip {

    NSString* myTip = [NSString stringWithString:@" Data Simulation Tool "];
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

@end
