#import "InOutlet.h"
#import "RbData.h"
#import "RbDataCell.h"
#import "RbTaxonData.h"
#import "RevBayes.h"
#import "ToolDataEntry.h"
#import "WindowControllerDataEntry.h"




@implementation ToolDataEntry

- (void)awakeFromNib {

}

- (void)closeControlPanel {

    [NSApp stopModal];
	[controlWindow close];
}

- (RbData*)dataMatrix {

    RbData* d = [self dataMatrixIndexed:0];
    return d;
}

- (void)encodeWithCoder:(NSCoder*)aCoder {

	[super encodeWithCoder:aCoder];
}

- (id)init {

    self = [self initWithScaleFactor:1.0];
    return self;
}

- (void)initializeDataMatrix:(RbData*)dm {

    [dm setNumTaxa:3];
    [dm setNumCharacters:1];
    [dm setDataType:STANDARD];
    [dm setIsHomologyEstablished:YES];
    [dm setName:@"User-Entered Data Matrix"];
    for (int i=0; i<[dm numTaxa]; i++)
        {
        RbTaxonData* td = [[RbTaxonData alloc] init];
        [td setDataType:STANDARD];
        for (int j=0; j<[dm numCharacters]; j++)
            {
            RbDataCell* c = [[RbDataCell alloc] init];
            [c setIsDiscrete:YES];
            [c setRow:i];
            [c setColumn:j];
            [c setDataType:STANDARD];
            [c setNumStates:10];
            [c setIsAmbig:YES];
            [c setIsGapState:NO];
            [c setVal:[NSNumber numberWithInt:1]];
            [td addObservation:c];
            }
        [dm addTaxonName:[NSString stringWithFormat:@"Taxon %d", i+1]];
        [dm addTaxonData:td];
        }
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
        
        // instantiate the data object
        RbData* m = [[RbData alloc] init];
        [self initializeDataMatrix:m];
        [self addMatrix:m];
        [self makeDataInspector];
        hasInspectorInfo = NO;
        isResolved = YES;
		
		// initialize the control window and the data inspector
		controlWindow = [[WindowControllerDataEntry alloc] initWithTool:self];
		}
    return self;
}

- (id)initWithCoder:(NSCoder*)aDecoder {

    if ( (self = [super initWithCoder:aDecoder]) ) 
		{
		// initialize the tool image
		[self initializeImage];
        [self setImageWithSize:itemSize];

		// initialize the control window and the data inspector
		controlWindow = [[WindowControllerDataEntry alloc] initWithTool:self];

        hasInspectorInfo = NO;
        isResolved = YES;
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

	NSEnumerator* enumerator = [outlets objectEnumerator];
	id element;
	while ( (element = [enumerator nextObject]) )
        {
        if ([element numberOfConnections] == 0)
            {
            if ( [element toolColor] == [NSColor greenColor] )
                {
                return NO;
                }
            }
        }

    return YES;    
}

- (NSMutableAttributedString*)sendTip {

    NSString* myTip = @" Character Data Entry Tool ";
    if ([self isResolved] == YES)
        myTip = [myTip stringByAppendingFormat:@"\n Status: Resolved \n # Matrices: %d ", 0];
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

- (NSString*)toolName {

    return @"Morphological Data Entry";
}

- (void)updateForChangeInParent {

}

@end
