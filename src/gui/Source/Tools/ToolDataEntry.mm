#import "AnalysisView.h"
#import "InOutlet.h"
#import "RbData.h"
#import "RevBayes.h"
#import "ToolDataEntry.h"
#import "WindowControllerDataEntry.h"



@implementation ToolDataEntry

@synthesize numberOfCharacters;
@synthesize numberOfTaxa;

- (void)awakeFromNib {

}

- (void)closeControlPanel {

    [NSApp stopModal];
	[controlWindow close];
}

- (void)dealloc {

	[controlWindow release];

	[super dealloc];
}

- (void)encodeWithCoder:(NSCoder*)aCoder {

	[aCoder encodeInt:numberOfCharacters   forKey:@"numberOfCharacters"];
	[aCoder encodeInt:numberOfTaxa         forKey:@"numberOfTaxa"];

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
		[self addOutletOfColor:[NSColor greenColor]];
        [self setOutletLocations];

		// initialize the data
		numberOfTaxa       = 3;
		numberOfCharacters = 1;
		
		// initialize the control window and the data inspector
		controlWindow = [[WindowControllerDataEntry alloc] initWithTool:self];
		}
    return self;
}

- (id)initWithCoder:(NSCoder *)aDecoder {

    if ( (self = [super initWithCoder:aDecoder]) ) 
		{
		// initialize the tool image
		[self initializeImage];
        [self setImageWithSize:itemSize];
		
		// read from file
		numberOfCharacters = [aDecoder decodeIntForKey:@"numberOfCharacters"];
		numberOfTaxa       = [aDecoder decodeIntForKey:@"numberOfTaxa"];
        
		// initialize the control window
		controlWindow = [[WindowControllerDataEntry alloc] initWithTool:self];
		}
	return self;
}

- (void)initializeImage {

    itemImage[0] = [NSImage imageNamed:@"Tool_DataEntry25.icns"];
    itemImage[1] = [NSImage imageNamed:@"Tool_RDataEntry50.icns"];
    itemImage[2] = [NSImage imageNamed:@"Tool_DataEntry75.icns"];
    itemImage[3] = [NSImage imageNamed:@"Tool_DataEntry100.icns"];
    itemImage[4] = [NSImage imageNamed:@"Tool_DataEntry125.icns"];
    itemImage[5] = [NSImage imageNamed:@"Tool_DataEntry150.icns"];
    itemImage[6] = [NSImage imageNamed:@"Tool_DataEntry200.icns"];
    itemImage[7] = [NSImage imageNamed:@"Tool_DataEntry400.icns"];
    
	float s[8] = { 0.25, 0.50, 0.75, 1.0, 1.25, 1.50, 2.0, 4.0 };
	for (int i=0; i<8; i++)
        [itemImage[i] setSize:NSMakeSize(ITEM_IMAGE_SIZE*s[i], ITEM_IMAGE_SIZE*s[i])];
}

- (BOOL)isFullyConnected {

    // we check the number of connections for the inlets and outlets
    // if any inlet or outlet has 0 connections, then the tool is not fully
    // connected
	NSEnumerator* enumerator = [inlets objectEnumerator];
	id element;
	while ( (element = [enumerator nextObject]) )
        {
        if ([element numberOfConnections] == 0)
            return NO;
        }
    
	enumerator = [outlets objectEnumerator];
	while ( (element = [enumerator nextObject]) )
        {
        if ([element numberOfConnections] == 0)
            {
            if ( [element toolColor] == [NSColor greenColor] )
                {
                if ( [self numAlignedMatrices] > 0 )
                    return NO;
                }
            }
        }

    return YES;
}

- (NSMutableAttributedString*)sendTip {

    NSString* myTip = [NSString stringWithString:@" Character Data Entry Tool "];
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

- (void)showControlPanel {

    NSPoint p = [self originForControlWindow:[controlWindow window]];
    [[controlWindow window] setFrameOrigin:p];
	[controlWindow showWindow:self];
	[[controlWindow window] makeKeyAndOrderFront:nil];
    [NSApp runModalForWindow:[controlWindow window]];
}

- (void)updateForChangeInState {

    NSLog(@"updateForChangeInState in %@", self);
}

@end
