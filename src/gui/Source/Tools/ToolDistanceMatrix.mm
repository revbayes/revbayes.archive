#import "RevBayes.h"
#import "ToolDistanceMatrix.h"
#import "WindowControllerDistanceMatrix.h"



@implementation ToolDistanceMatrix

@synthesize distanceType;
@synthesize gammaRateVariation;
@synthesize baseFreqTreatment;
@synthesize proportionInvariableSites;
@synthesize gammaShape;

- (void)awakeFromNib {

}

- (void)calculateDistances {

    [self startProgressIndicator];

    NSLog(@"distanceType              = %d", distanceType);
    NSLog(@"gammaRateVariation        = %d", gammaRateVariation);
    NSLog(@"baseFreqTreatment         = %d", baseFreqTreatment);
    NSLog(@"proportionInvariableSites = %lf", proportionInvariableSites);
    NSLog(@"gammaShape                = %lf", gammaShape);

    /*const char* cmdAsCStr = [alnDirectory UTF8String];
    std::string cmdAsStlStr = cmdAsCStr;
    std::string line = variableName + " <- read(\"" + cmdAsStlStr + "\")";
    int coreResult = Parser::getParser().processCommand(line);
    if (coreResult != 0)
        {
        [self readDataError:@"Data could not be read" forVariableNamed:nsVariableName];
        [self stopProgressIndicator];
        return;
        }*/

    [self stopProgressIndicator];
}

- (void)closeControlPanel {

    [NSApp stopModal];
	[controlWindow close];
}

- (void)dealloc {

    [controlWindow release];
	[super dealloc];
}

- (void)encodeWithCoder:(NSCoder *)aCoder {

	[aCoder encodeInt:distanceType                 forKey:@"distanceType"];
	[aCoder encodeBool:gammaRateVariation          forKey:@"gammaRateVariation"];
	[aCoder encodeInt:baseFreqTreatment            forKey:@"baseFreqTreatment"];
	[aCoder encodeDouble:proportionInvariableSites forKey:@"proportionInvariableSites"];
	[aCoder encodeDouble:gammaShape                forKey:@"gammaShape"];

	[super encodeWithCoder:aCoder];
}

- (void)execute {

    NSLog(@"Executing tool %@", self);
    
    [self startProgressIndicator];
    
    [self stopProgressIndicator];
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
		[self addOutletOfColor:[NSColor purpleColor]];
        [self setInletLocations];
        [self setOutletLocations];
        
        // initialize the distance variables
        distanceType              = JC69;
        gammaRateVariation        = NO;
        baseFreqTreatment         = EQUAL_FREQS;
        proportionInvariableSites = 0.0;
        gammaShape                = 0.5;
		
		// initialize the control window
		controlWindow = [[WindowControllerDistanceMatrix alloc] initWithTool:self];
		}
    return self;
}

- (id)initWithCoder:(NSCoder *)aDecoder {

    if ( (self = [super initWithCoder:aDecoder]) ) 
		{
		// initialize the tool image
		[self initializeImage];
        [self setImageWithSize:itemSize];

        // initialize the distance variables
		distanceType              = [aDecoder decodeIntForKey:@"distanceType"];
		gammaRateVariation        = [aDecoder decodeBoolForKey:@"gammaRateVariation"];
		baseFreqTreatment         = [aDecoder decodeIntForKey:@"baseFreqTreatment"];
		proportionInvariableSites = [aDecoder decodeDoubleForKey:@"proportionInvariableSites"];
		gammaShape                = [aDecoder decodeDoubleForKey:@"gammaShape"];

		// initialize the control window
		controlWindow = [[WindowControllerDistanceMatrix alloc] initWithTool:self];
		}
	return self;
}

- (void)initializeImage {

    itemImage[0] = [NSImage imageNamed:@"Tool_Distance25.icns"];
    itemImage[1] = [NSImage imageNamed:@"Tool_Distance50.icns"];
    itemImage[2] = [NSImage imageNamed:@"Tool_Distance75.icns"];
    itemImage[3] = [NSImage imageNamed:@"Tool_Distance100.icns"];
    itemImage[4] = [NSImage imageNamed:@"Tool_Distance125.icns"];
    itemImage[5] = [NSImage imageNamed:@"Tool_Distance150.icns"];
    itemImage[6] = [NSImage imageNamed:@"Tool_Distance200.icns"];
    itemImage[7] = [NSImage imageNamed:@"Tool_Distance400.icns"];

	float s[8] = { 0.25, 0.50, 0.75, 1.0, 1.25, 1.50, 2.0, 4.0 };
	for (int i=0; i<8; i++)
        [itemImage[i] setSize:NSMakeSize(ITEM_IMAGE_SIZE*s[i], ITEM_IMAGE_SIZE*s[i])];
}

- (NSMutableAttributedString*)sendTip {

    NSString* myTip = [NSString stringWithString:@" Distance Matrix Tool "];
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

- (void)showControlPanel {

    [controlWindow setDistanceType:distanceType];
    [controlWindow setGammaRateVariation:gammaRateVariation];
    [controlWindow setBaseFreqTreatment:baseFreqTreatment];
    [controlWindow setProportionInvariableSites:proportionInvariableSites];
    [controlWindow setGammaShape:gammaShape];

    NSPoint p = [self originForControlWindow:[controlWindow window]];
    [[controlWindow window] setFrameOrigin:p];
	[controlWindow showWindow:self];    
	[[controlWindow window] makeKeyAndOrderFront:nil];
    [NSApp runModalForWindow:[controlWindow window]];
}

@end
