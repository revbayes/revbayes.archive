#import "RevBayes.h"
#import "ToolLoop.h"
#import "WindowControllerLoop.h"


@implementation ToolLoop

@synthesize indexLetter;
@synthesize indexValue;
@synthesize loopRect;
@synthesize numElements;
@synthesize minLoopSize;
@synthesize indexSource;
@synthesize italicsRange;

- (void)awakeFromNib {

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

- (NSString*)getEndingRangeForLoop {

	italicsRange = NSMakeRange(0, 0);
	NSString* pStr = [NSString stringWithFormat:@"%d", numElements];
    return pStr;
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
		
		// initialize some variables
        isLoop = YES;
        isPlate = YES;
        numElements = 1;
        loopRect = NSMakeRect(0.0, 0.0, 200.0*sf, 200.0*sf);
        indexLetter = 'i';

        [self setImageWithSize:loopRect.size];
		
		// initialize the control window
		controlWindow = [[WindowControllerLoop alloc] initWithTool:self];
		}
    return self;
}

- (id)initWithCoder:(NSCoder *)aDecoder {

    if ( (self = [super initWithCoder:aDecoder]) ) 
		{
		// initialize the tool image
		[self initializeImage];
        [self setImageWithSize:itemSize];
        [self setImageWithSize:loopRect.size];

		// initialize the control window
		controlWindow = [[WindowControllerLoop alloc] initWithTool:self];
		}
	return self;
}

- (void)initializeImage {

    itemImage[0] = [NSImage imageNamed:@"Tool_Loop25.icns"];
    itemImage[1] = [NSImage imageNamed:@"Tool_Loop50.icns"];
    itemImage[2] = [NSImage imageNamed:@"Tool_Loop75.icns"];
    itemImage[3] = [NSImage imageNamed:@"Tool_Loop100.icns"];
    itemImage[4] = [NSImage imageNamed:@"Tool_Loop125.icns"];
    itemImage[5] = [NSImage imageNamed:@"Tool_Loop150.icns"];
    itemImage[6] = [NSImage imageNamed:@"Tool_Loop200.icns"];
    itemImage[7] = [NSImage imageNamed:@"Tool_Loop400.icns"];

	float s[8] = { 0.25, 0.50, 0.75, 1.0, 1.25, 1.50, 2.0, 4.0 };
	for (int i=0; i<8; i++)
        [itemImage[i] setSize:NSMakeSize(ITEM_IMAGE_SIZE*s[i], ITEM_IMAGE_SIZE*s[i])];
}

- (NSMutableAttributedString*)sendTip {

    NSString* myTip = @" Loop ";
              myTip = [myTip stringByAppendingFormat:@"\n Number of repeats: %d ", 0];

    NSDictionary *attr = [NSDictionary 
                 dictionaryWithObjects:[NSArray arrayWithObjects:[NSFont fontWithName:@"Lucida Grande Bold" size:14.0], [[NSColor whiteColor] colorWithAlphaComponent:1.0], nil] 
                               forKeys: [NSArray arrayWithObjects:NSFontAttributeName,NSForegroundColorAttributeName, nil]];

    NSMutableAttributedString* attrString = [[NSMutableAttributedString alloc] initWithString:myTip attributes:attr];

    return attrString;
}

- (void)showControlPanel {

    NSPoint p = [self originForControlWindow:[controlWindow window]];
    [[controlWindow window] setFrameOrigin:p];
	[controlWindow showWindow:self];    
	[[controlWindow window] makeKeyAndOrderFront:nil];
    [NSApp runModalForWindow:[controlWindow window]];
}

- (NSString*)toolName {

    return @"Loop";
}

- (void)updateForChangeInParent {

}

@end
