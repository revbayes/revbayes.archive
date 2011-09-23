#import "InOutlet.h"
#import "RevBayes.h"
#import "ToolAlign.h"
#import "WindowControllerAlign.h"


@implementation ToolAlign

@synthesize alignClustalAlign;
@synthesize alignClustalWordLength;
@synthesize alignClustalWindow;
@synthesize alignClustalScoreType;
@synthesize alignClustalNumberDiagonals;
@synthesize alignClustalPairGapPenalty;
@synthesize alignClustalMatrix;
@synthesize alignClustalGapOpenPenalty;
@synthesize alignClustalEndGaps;
@synthesize alignClustalGapExtensionCost;
@synthesize alignClustalGapSeparationPenalty;
@synthesize alignClustalIteration;
@synthesize alignClustalNumberOfIterations;


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
            
        // initialize Clustal variables here
        // Default values taken from http://www.ebi.ac.uk/Tools/msa/clustalw2/help/
            
        [self setAlignClustalAlign: @"Full"];
        [self setAlignClustalWordLength: 1];
        [self setAlignClustalWindow: 5];
        [self setAlignClustalScoreType: @"Percent"];
        [self setAlignClustalNumberDiagonals: 5];
        [self setAlignClustalPairGapPenalty: 3];
        [self setAlignClustalMatrix: @"Gonnet"];
        [self setAlignClustalGapOpenPenalty: 10.0];
        [self setAlignClustalEndGaps: @"No"];
        [self setAlignClustalGapExtensionCost: 0.20];
        [self setAlignClustalGapSeparationPenalty: 5];
        [self setAlignClustalIteration: @"None"];
        [self setAlignClustalNumberOfIterations: 1];
       
        
		
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
            
        // resuscitate Clustal variables here before recreating new windowcontroller

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

- (void)helperRunClustal
{
    NSFileManager *alignClustalFileManager;
    alignClustalFileManager = [[NSFileManager alloc] init];
    
    NSDictionary *alignClustalTemporaryDirectoryAttributes;
    alignClustalTemporaryDirectoryAttributes = [NSDictionary dictionaryWithObject:NSFileTypeDirectory forKey:@"alignClustalTemporaryDirectory"];
    
    NSString *alignClustalUserTemporaryDirectory;
    alignClustalUserTemporaryDirectory = NSTemporaryDirectory();
    
    [alignClustalFileManager createDirectoryAtPath:alignClustalUserTemporaryDirectory withIntermediateDirectories:NO attributes:alignClustalTemporaryDirectoryAttributes error:NULL];
    

    NSString *alignClustalPath = @"";
    NSString *alignClustalExecutable = @"clustalw2";
    
    
    alignClustalPath = [[NSBundle mainBundle] pathForResource:alignClustalExecutable ofType:nil];
    NSLog (@"alignClustalPath = %@", alignClustalPath);
    
// String values for clustal arguments
    alignClustalReduceConsoleOutputAr = [NSString stringWithString: @"-QUIET"];

    alignClustalMultipleAlignAr = [NSString stringWithString: @"-ALIGN"];
    
    alignClustalInfileAr = [NSString stringWithString: @"-INFILE=/Users/edna/Documents/Cocoa_projects/Clustal/nYAL001C.fas"];
    
    alignClustalOutfileAr = [NSString stringWithString: @"-OUTFILE="];
    alignClustalOutfileAr = [alignClustalOutfileAr stringByAppendingString: alignClustalUserTemporaryDirectory];
    alignClustalOutfileAr = [alignClustalOutfileAr stringByAppendingString: @"clustaloutput.fasta"];
    
    alignClustalOutputAr = [NSString stringWithString: @"-OUTPUT=FASTA"];    

   
    alignClustalGuideTreeAr = [NSString stringWithString: @"-NEWTREE="];
    alignClustalGuideTreeAr = [alignClustalGuideTreeAr stringByAppendingString: alignClustalUserTemporaryDirectory];
    alignClustalGuideTreeAr = [alignClustalGuideTreeAr stringByAppendingString: @"clustaltree.dnd"];

    
    NSString *alignClustalAlignAr = [NSString stringWithString: @""];
 
    if ([alignClustalAlign isEqualToString: @"Fast"] == YES)
        {
            alignClustalAlignAr = [alignClustalAlignAr stringByAppendingString: @"-QUICKTREE"];
        }
    
    
    NSString *alignClustalWordLengthAr = [NSString stringWithString: @"-KTUPLE="];
    alignClustalWordLengthAr = [alignClustalWordLengthAr stringByAppendingFormat: @"%i", alignClustalWordLength];
    
    NSString *alignClustalWindowAr = [NSString stringWithString: @"-WINDOW="];
    alignClustalWindowAr = [alignClustalWindowAr stringByAppendingFormat: @"%i", alignClustalWindow];
    
    NSString *alignClustalScoreTypeAr = [NSString stringWithString: @"-SCORE="];
    alignClustalScoreTypeAr = [alignClustalScoreTypeAr stringByAppendingString: alignClustalScoreType];
        
    NSString *alignClustalNumberDiagonalsAr = [NSString stringWithString: @"-TOPDIAGS="];
    alignClustalNumberDiagonalsAr = [alignClustalNumberDiagonalsAr stringByAppendingFormat: @"%i", alignClustalNumberDiagonals];

    NSString *alignClustalPairGapPenaltyAr = [NSString stringWithString: @"-PAIRGAP="];
    alignClustalPairGapPenaltyAr = [alignClustalPairGapPenaltyAr stringByAppendingFormat: @"%i", alignClustalPairGapPenalty];

    NSString *alignClustalMatrixAr = [NSString stringWithString: @"-PWMATRIX="];
    alignClustalMatrixAr = [alignClustalMatrixAr stringByAppendingString: alignClustalMatrix];

    NSString *alignClustalGapOpenPenaltyAr = [NSString stringWithString: @"-PWGAPEXT="];
    alignClustalGapOpenPenaltyAr = [alignClustalGapOpenPenaltyAr stringByAppendingFormat: @"%f", alignClustalGapOpenPenalty];

    NSString *alignClustalEndGapsAr = [NSString stringWithString: @"-ENDGAPS="];
    alignClustalEndGapsAr = [alignClustalEndGapsAr stringByAppendingString: alignClustalEndGaps];

    NSString *alignClustalGapExtensionCostAr = [NSString stringWithString: @"-GAPEXT="];
    alignClustalGapExtensionCostAr = [alignClustalGapExtensionCostAr stringByAppendingFormat: @"%f", alignClustalGapExtensionCost];
    
    NSString *alignClustalGapSeparationPenaltyAr = [NSString stringWithString: @"-GAPDIST="];
    alignClustalGapSeparationPenaltyAr = [alignClustalGapSeparationPenaltyAr stringByAppendingFormat: @"%i", alignClustalGapSeparationPenalty];

    NSString *alignClustalIterationAr = [NSString stringWithString: @"-ITERATION="];
    alignClustalIterationAr = [alignClustalIterationAr stringByAppendingString: alignClustalIteration];
    
    NSString *alignClustalNumberOfIterationsAr = [NSString stringWithString: @"-NUMITER="];
    alignClustalNumberOfIterationsAr = [alignClustalNumberOfIterationsAr stringByAppendingFormat: @"%i", alignClustalNumberOfIterations];
    


    alignClustalArguments = [NSArray arrayWithObjects: 
                             alignClustalReduceConsoleOutputAr, 
                             alignClustalInfileAr, 
                             alignClustalOutfileAr, 
                             alignClustalOutputAr,
                             alignClustalGuideTreeAr, 
                             alignClustalAlignAr, 
                             alignClustalWordLengthAr, 
                             alignClustalWindowAr, 
                             alignClustalScoreTypeAr, 
                             alignClustalNumberDiagonalsAr, 
                             alignClustalPairGapPenaltyAr, 
                             alignClustalMatrixAr, 
                             alignClustalGapOpenPenaltyAr, 
                             alignClustalEndGapsAr,
                             alignClustalGapExtensionCostAr,
                             alignClustalGapSeparationPenaltyAr,
                             alignClustalIterationAr,
                             alignClustalNumberOfIterationsAr,
                             alignClustalMultipleAlignAr,
                             nil];
    
    NSLog(@"array: %@", alignClustalArguments);
    
    alignClustalTask = [[NSTask alloc] init];
    [alignClustalTask setLaunchPath: alignClustalPath];
    [alignClustalTask setCurrentDirectoryPath: alignClustalPath];
    [alignClustalTask setArguments: alignClustalArguments];
    
    alignClustalFromPipe = [NSPipe pipe];
    alignClustalFromClustal = [alignClustalFromPipe fileHandleForReading];
    [alignClustalTask setStandardOutput: alignClustalFromPipe];
    
    NSNotificationCenter *defaultCenter = [NSNotificationCenter defaultCenter];
    
    [defaultCenter addObserver: self
                      selector: @selector(receiveData:)
                          name: NSFileHandleReadCompletionNotification
                        object: alignClustalFromClustal];
    
    alignClustalToPipe = [NSPipe pipe];
    alignClustalToClustal = [alignClustalToPipe fileHandleForWriting];
    [alignClustalTask setStandardInput: alignClustalToPipe];
    
    alignClustalErrorPipe = [NSPipe pipe];
    alignClustalErrorData = [alignClustalErrorPipe fileHandleForReading];
    [alignClustalTask setStandardError: alignClustalErrorPipe];
    
    [defaultCenter addObserver: self
                      selector: @selector(alignClustalErrorDataAvailable:)
                          name: NSFileHandleReadCompletionNotification
                        object: alignClustalErrorData];
    
    
    [alignClustalTask launch];
    
    [alignClustalFromClustal readInBackgroundAndNotify];
    //    [alignClustalErrorData readInBackgroundAndNotify];

    
    NSLog(@"Running Clustal from ToolAlign");
}

/* Pipe Clustal error messages to an alert box */
- (void)errorDataAvailable: (NSNotification *) aNotification
{
	NSData *incomingError;
	
	incomingError = [[aNotification userInfo] objectForKey: NSFileHandleNotificationDataItem];
    
    NSString *incomingText;
    
    incomingText = [[NSString alloc] initWithData: incomingError
                                         encoding: NSASCIIStringEncoding];
    
    NSLog (@"%@", incomingText);
    
    
     NSAlert *errorAlert = [NSAlert alertWithMessageText: incomingText 
     defaultButton: @"OK" 
     alternateButton: @"Help"
     otherButton: nil
     informativeTextWithFormat: @""];
     
     NSInteger answer = [errorAlert runModal];
     if (answer ==  NSAlertDefaultReturn)
     {
     NSLog (@"default return");
     }
     
     else
     {
     NSLog (@"not default return");
     NSString *lastLinePlusReturn = @"h\n";
     
     NSData *sendData;
     
     sendData = [lastLinePlusReturn dataUsingEncoding: NSASCIIStringEncoding];
     
     [alignClustalToClustal writeData: sendData];				
     
     }
     
    [incomingText release];
    [alignClustalErrorData readInBackgroundAndNotify];
    return;
}



/* Receive Clustal data */ 
- (void)receiveData: (NSNotification *) aNotification
{
    NSLog(@"Clustal data received");
    /*	NSData *incomingData;
     NSString *incomingText;
     
     incomingData = [[aNotification userInfo] objectForKey: NSFileHandleNotificationDataItem];
     
     incomingText = [[NSString alloc] initWithData: incomingData
     encoding: NSASCIIStringEncoding];
     
     [clustalView appendString: incomingText];
     [clustalView scrollRangeToVisible: NSMakeRange ([[clustalView textStorage] length], 0)];
     [clustalView setNeedsDisplay: YES];
     
     [self returnFocus];
     
     [fromClustal readInBackgroundAndNotify];
     
     [incomingText release];
*/
}
    


@end
