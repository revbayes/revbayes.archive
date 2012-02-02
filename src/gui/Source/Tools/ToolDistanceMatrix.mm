#import "Connection.h"
#import "Inlet.h"
#import "Outlet.h"
#import "RbData.h"
#import "RevBayes.h"
#import "ToolData.h"
#import "ToolDistanceMatrix.h"
#import "WindowControllerDistanceMatrix.h"

#include "Parser.h"
#include "Workspace.h"
#include <string>


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
        
    // find the parent of this tool, which should be an instance of ToolReadData
    ToolData* dataTool = nil;
    for (int i=0; i<[inlets count]; i++)
        {
        Inlet* theInlet = [inlets objectAtIndex:i];
        for (int j=0; j<[theInlet numberOfConnections]; j++)
            {
            Connection* c = [theInlet connectionWithIndex:j];
            Tool* t = [[c outlet] toolOwner];
            if ( [t isKindOfClass:[ToolData class]] == YES )
                dataTool = (ToolData*)t;
            }
        }
    if ( dataTool == nil )
        return;

    // calculate how many aligned data matrices exist
    NSMutableArray* alignedData = [NSMutableArray arrayWithCapacity:1];
    for (int i=0; i<[dataTool numDataMatrices]; i++)
        {
        if ( [[dataTool dataMatrixIndexed:i] isHomologyEstablished] == YES )
            [alignedData addObject:[dataTool dataMatrixIndexed:i]];
        }
    if ( [alignedData count] == 0 )
        return;
        
    // remove all of the files from the temporary directory
    [self removeFilesFromTemporaryDirectory];
        
    // and make a temporary directory to contain the alignments
    NSString* temporaryDirectory = NSTemporaryDirectory();
    NSFileManager* fm = [[[NSFileManager alloc] init] autorelease];
    NSString* alnDirectory = [NSString stringWithString:temporaryDirectory];
              alnDirectory = [alnDirectory stringByAppendingString:@"myAlignments/"];
    NSDictionary* dirAttributes = [NSDictionary dictionaryWithObject:NSFileTypeDirectory forKey:@"dirAttributes"];
    [fm createDirectoryAtPath:alnDirectory withIntermediateDirectories:NO attributes:dirAttributes error:NULL];

    // write the alignment files to the temporary directory
    for (int i=0; i<[alignedData count]; i++)
        {
        // have the data object save a file to the temporary directory
        RbData* d = [alignedData objectAtIndex:i];
        NSString* dFilePath = [NSString stringWithString:alnDirectory];
                  dFilePath = [dFilePath stringByAppendingString:[d name]];
        if ( [d isHomologyEstablished] == YES )
            dFilePath = [dFilePath stringByAppendingString:@".nex"];
        else
            dFilePath = [dFilePath stringByAppendingString:@".fas"];
        [d writeToFile:dFilePath];
        }

    // get a variable name from the workspace for the data and distance matrix
    std::string dataName = Workspace::userWorkspace()->generateUniqueVariableName();
    std::string distName = Workspace::userWorkspace()->generateUniqueVariableName();
		    
    // format a string command to read the data file(s) and send the
    // formatted string to the parser
    const char* cmdAsCStr = [alnDirectory UTF8String];
    std::string cmdAsStlStr = cmdAsCStr;
    std::string line = dataName + " <- read(\"" + cmdAsStlStr + "\")";
    int coreResult = Parser::getParser().processCommand(line);
    if (coreResult != 0)
        {
        [self stopProgressIndicator];
        return;
        }
        
    // get string for specifying distance commands
    std::string cmdStr = distName;
    cmdStr += " <- distances(data=";
    cmdStr += dataName;
    cmdStr += ", model=\"";
    if (distanceType == P_DISTANCE)
        cmdStr += "p";
    else if (distanceType == JC69)
        cmdStr += "jc69";
    else if (distanceType == F81)
        cmdStr += "f81";
    else if (distanceType == TN93)
        cmdStr += "tn93";
    else if (distanceType == K2P)
        cmdStr += "k2p";
    else if (distanceType == HKY85)
        cmdStr += "hky85";
    else if (distanceType == K3P)
        cmdStr += "k3p";
    else if (distanceType == GTR)
        cmdStr += "gtr";
    else if (distanceType == LOGDET)
        cmdStr += "logdet";
    cmdStr += "\", freqs=\"";
    if (baseFreqTreatment == EQUAL_FREQS)
        cmdStr += "equal";
    else if (baseFreqTreatment == EMPIRICAL_FREQS)
        cmdStr += "empirical";
    cmdStr += "\", asrv=\"";
    if (gammaRateVariation == EQUAL_RATES)
        cmdStr += "equal";
    else if (gammaRateVariation == GAMMA_RATES)
        cmdStr += "gamma";
    cmdStr += "\", shape=";
    char tempC[20];
    sprintf(tempC, "%lf", gammaShape);
    std::string tempStr = tempC;
    cmdStr += tempStr;
    cmdStr += ", pinvar=";
    sprintf(tempC, "%lf", proportionInvariableSites);
    tempStr = tempC;
    cmdStr += tempStr;
    cmdStr += ");";
    std::cout << cmdStr << std::endl;

    // have the core process the command
    coreResult = Parser::getParser().processCommand(cmdStr);
    if (coreResult != 0)
        {
        // possibly return, after erasing the temporary variable names
        }





    // remove the variables from the core
    Workspace::userWorkspace()->eraseVariable(dataName);
    Workspace::userWorkspace()->eraseVariable(distName);

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
