#import "Connection.h"
#import "RbData.h"
#import "Inlet.h"
#import "InOutlet.h"
#import "RevBayes.h"
#import "ToolAlign.h"
#import "ToolReadData.h"
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

    if (alignClustalTask != nil)
        [alignClustalTask release];
        
    [alignClustalAlign release];
    [alignClustalScoreType release];
    [alignClustalMatrix release];
    [alignClustalEndGaps release];
    [alignClustalIteration release];

	[controlWindow release];
	[super dealloc];
}

- (void)encodeWithCoder:(NSCoder*)aCoder {
    
	[aCoder encodeObject:alignClustalAlign forKey:@"alignClustalAlign"];
	[aCoder encodeInt:alignClustalWordLength forKey:@"alignClustalWordLength"];
 	[aCoder encodeInt:alignClustalWindow forKey:@"alignClustalWindow"];
	[aCoder encodeObject:alignClustalScoreType forKey:@"alignClustalScoreType"];
	[aCoder encodeInt:alignClustalNumberDiagonals forKey:@"alignClustalNumberDiagonals"];
	[aCoder encodeInt:alignClustalPairGapPenalty forKey:@"alignClustalPairGapPenalty"];
	[aCoder encodeObject:alignClustalMatrix forKey:@"alignClustalMatrix"];
	[aCoder encodeFloat:alignClustalGapOpenPenalty forKey:@"alignClustalGapOpenPenalty"];
	[aCoder encodeObject:alignClustalEndGaps forKey:@"alignClustalEndGaps"];
	[aCoder encodeFloat:alignClustalGapExtensionCost forKey:@"alignClustalGapExtensionCost"];
	[aCoder encodeInt:alignClustalGapSeparationPenalty forKey:@"alignClustalGapSeparationPenalty"];
	[aCoder encodeObject:alignClustalIteration forKey:@"alignClustalIteration"];
	[aCoder encodeInt:alignClustalNumberOfIterations forKey:@"alignClustalNumberOfIterations"];
       
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
        [self setInletLocations];
        [self setOutletLocations];
            
        // initialize clustal variables here
        // Default values taken from http://www.ebi.ac.uk/Tools/msa/clustalw2/help/
        alignClustalTask = nil;
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
        [self setAlignClustalIteration: @"none"];
        [self setAlignClustalNumberOfIterations: 1];
            		
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
        alignClustalAlign = [aDecoder decodeObjectForKey:@"alignClustalAlign"];
            [alignClustalAlign retain];
        alignClustalWordLength = [aDecoder decodeIntForKey:@"alignClustalWordLength"];
        alignClustalWindow = [aDecoder decodeIntForKey:@"alignClustalWindow"];
        alignClustalScoreType = [aDecoder decodeObjectForKey:@"alignClustalScoreType"];
            [alignClustalScoreType retain];
        alignClustalNumberDiagonals = [aDecoder decodeIntForKey:@"alignClustalNumberDiagonals"];
        alignClustalPairGapPenalty = [aDecoder decodeIntForKey:@"alignClustalPairGapPenalty"];
        alignClustalMatrix = [aDecoder decodeObjectForKey:@"alignClustalMatrix"];
            [alignClustalMatrix retain];
        alignClustalGapOpenPenalty = [aDecoder decodeFloatForKey:@"alignClustalGapOpenPenalty"];
        alignClustalEndGaps = [aDecoder decodeObjectForKey:@"alignClustalEndGaps"];
            [alignClustalEndGaps retain];
        alignClustalGapExtensionCost = [aDecoder decodeFloatForKey:@"alignClustalGapExtensionCost"];
        alignClustalGapSeparationPenalty = [aDecoder decodeIntForKey:@"alignClustalGapSeparationPenalty"];
        alignClustalIteration = [aDecoder decodeObjectForKey:@"alignClustalIteration"];
            [alignClustalIteration retain];
        alignClustalNumberOfIterations = [aDecoder decodeIntForKey:@"alignClustalNumberOfIterations"];
        
            
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

- (void)helperRunClustal: (id)sender {

    // find the parent of this tool, which should be an instance of ToolReadData
    ToolReadData* dataTool = nil;
    for (int i=0; i<[inlets count]; i++)
        {
        Inlet* theInlet = [inlets objectAtIndex:i];
        for (int j=0; j<[theInlet numberOfConnections]; j++)
            {
            Connection* c = [theInlet connectionWithIndex:j];
            Tool* t = [[c outlet] toolOwner];
            NSString* className = NSStringFromClass([t class]); 
            if ( [className isEqualToString:@"ToolReadData"] == YES )
                dataTool = (ToolReadData*)t;
            }
        }
    if ( dataTool == nil )
        return;
        
    // calculate how many unaligned data matrices exist
    NSMutableArray* unalignedData = [NSMutableArray arrayWithCapacity:1];
    for (int i=0; i<[dataTool numDataMatrices]; i++)
        {
        if ( [[dataTool dataMatrixIndexed:i] isHomologyEstablished] == NO )
            [unalignedData addObject:[dataTool dataMatrixIndexed:i]];
        }
    if ( [unalignedData count] == 0)
        return;
        
    // align each of the unaligned data files in the ToolReadData object
    NSString* temporaryDirectory = NSTemporaryDirectory();
    for (int i=0; i<[unalignedData count]; i++)
        {
        // have the data object save a fasta file to the temporary directory
        RbData* d = [unalignedData objectAtIndex:i];
        NSString* dFilePath = [NSString stringWithString:temporaryDirectory];
        dFilePath = [dFilePath stringByAppendingString:[d name]];
        [d writeToFile:dFilePath];
        
        // collect the clustal arguments
        NSString* clustalReduceConsoleOutputArg  = [NSString stringWithString: @"-QUIET"];
        NSString* clustalMultipleAlignArg        = [NSString stringWithString: @"-ALIGN"];
        NSString* clustalInfileArg               = [NSString stringWithString: @"-INFILE="];
        clustalInfileArg                         = [clustalInfileArg stringByAppendingString:dFilePath];
        NSString* clustalOutfileArg              = [NSString stringWithString: @"-OUTFILE="];
        clustalOutfileArg                        = [clustalOutfileArg stringByAppendingString: temporaryDirectory];
        clustalOutfileArg                        = [clustalOutfileArg stringByAppendingString: @"clustaloutput.fasta"];
        NSString* clustalOutputArg               = [NSString stringWithString: @"-OUTPUT=FASTA"];    
        NSString* clustalGuideTreeArg            = [NSString stringWithString: @"-NEWTREE="];
        clustalGuideTreeArg                      = [clustalGuideTreeArg stringByAppendingString: temporaryDirectory];
        clustalGuideTreeArg                      = [clustalGuideTreeArg stringByAppendingString: @"clustaltree.dnd"];
        NSString* clustalAlignArg                = [NSString stringWithString: @"-QUICKTREE"];
        NSString* clustalWordLengthArg           = [NSString stringWithString:@"-KTUPLE="];
        clustalWordLengthArg                     = [clustalWordLengthArg stringByAppendingFormat:@"%i", alignClustalWordLength];
        NSString* clustalWindowArg               = [NSString stringWithString: @"-WINDOW="];
        clustalWindowArg                         = [clustalWindowArg stringByAppendingFormat: @"%i", alignClustalWindow];
        NSString* clustalScoreTypeArg            = [NSString stringWithString: @"-SCORE="];
        clustalScoreTypeArg                      = [clustalScoreTypeArg stringByAppendingString: alignClustalScoreType];
        NSString* clustalNumberDiagonalsArg      = [NSString stringWithString: @"-TOPDIAGS="];
        clustalNumberDiagonalsArg                = [clustalNumberDiagonalsArg stringByAppendingFormat: @"%i", alignClustalNumberDiagonals];
        NSString* clustalPairGapPenaltyArg       = [NSString stringWithString: @"-PAIRGAP="];
        clustalPairGapPenaltyArg                 = [clustalPairGapPenaltyArg stringByAppendingFormat: @"%i", alignClustalPairGapPenalty];
        NSString* clustalMatrixArg               = [NSString stringWithString: @"-PWMATRIX="];
        clustalMatrixArg                         = [clustalMatrixArg stringByAppendingString: alignClustalMatrix];
        NSString* clustalGapOpenPenaltyAr        = [NSString stringWithString: @"-PWGAPEXT="];
        clustalGapOpenPenaltyAr                  = [clustalGapOpenPenaltyAr stringByAppendingFormat: @"%f", alignClustalGapOpenPenalty];
        NSString* clustalEndGapsArg              = [NSString stringWithString: @"-ENDGAPS="];
        clustalEndGapsArg                        = [clustalEndGapsArg stringByAppendingString: alignClustalEndGaps];
        NSString* clustalGapExtensionCostArg     = [NSString stringWithString: @"-GAPEXT="];
        clustalGapExtensionCostArg               = [clustalGapExtensionCostArg stringByAppendingFormat: @"%f", alignClustalGapExtensionCost];
        NSString* clustalGapSeparationPenaltyArg = [NSString stringWithString: @"-GAPDIST="];
        clustalGapSeparationPenaltyArg           = [clustalGapSeparationPenaltyArg stringByAppendingFormat: @"%i", alignClustalGapSeparationPenalty];
        NSString* clustalIterationArg            = [NSString stringWithString: @"-ITERATION="];
        clustalIterationArg                      = [clustalIterationArg stringByAppendingString: alignClustalIteration];
        NSString* clustalNumberOfIterationsArg   = [NSString stringWithString: @"-NUMITER="];
        clustalNumberOfIterationsArg             = [clustalNumberOfIterationsArg stringByAppendingFormat: @"%i", alignClustalNumberOfIterations];
        
        // set up an array with the clustal arguments
        NSArray* clustalArguments;
        if ( [alignClustalAlign isEqualToString: @"Fast"] == YES )
            {
            clustalArguments = [NSArray arrayWithObjects: 
                                     clustalReduceConsoleOutputArg, 
                                     clustalInfileArg, 
                                     clustalOutfileArg, 
                                     clustalOutputArg,
                                     clustalGuideTreeArg, 
                                     clustalAlignArg, 
                                     clustalWordLengthArg, 
                                     clustalWindowArg, 
                                     clustalScoreTypeArg, 
                                     clustalNumberDiagonalsArg, 
                                     clustalPairGapPenaltyArg, 
                                     clustalMatrixArg, 
                                     clustalGapOpenPenaltyAr, 
                                     clustalEndGapsArg,
                                     clustalGapExtensionCostArg,
                                     clustalGapSeparationPenaltyArg,
                                     clustalIterationArg,
                                     clustalNumberOfIterationsArg,
                                     clustalMultipleAlignArg,
                                     nil];
            }
        else
            {
            clustalArguments = [NSArray arrayWithObjects: 
                                     clustalReduceConsoleOutputArg, 
                                     clustalInfileArg, 
                                     clustalOutfileArg, 
                                     clustalOutputArg,
                                     clustalGuideTreeArg, 
                                     clustalWordLengthArg, 
                                     clustalWindowArg, 
                                     clustalScoreTypeArg, 
                                     clustalNumberDiagonalsArg, 
                                     clustalPairGapPenaltyArg, 
                                     clustalMatrixArg, 
                                     clustalGapOpenPenaltyAr, 
                                     clustalEndGapsArg,
                                     clustalGapExtensionCostArg,
                                     clustalGapSeparationPenaltyArg,
                                     clustalIterationArg,
                                     clustalNumberOfIterationsArg,
                                     clustalMultipleAlignArg,
                                     nil];
            }
            
        NSLog(@"clustalAgurments = %@", clustalArguments);
            
            
        currentHelper = @"clustal";
        
        // allocate a task for clustal
        alignClustalTask = [[NSTask alloc] init];

        // find the temporary directory
        NSFileManager* clustalFileManager = [[NSFileManager alloc] init];
        NSDictionary* clustalTemporaryDirectoryAttributes = [NSDictionary dictionaryWithObject:NSFileTypeDirectory forKey:@"alignClustalTemporaryDirectory"];
        [clustalFileManager createDirectoryAtPath:temporaryDirectory withIntermediateDirectories:NO attributes:clustalTemporaryDirectoryAttributes error:NULL];
            
        // find the clustal executable in the application bundle
        NSString* clustalPath = [[NSBundle mainBundle] pathForResource:@"clustalw2" ofType:nil];
        NSLog(@"clustalPath = %@", clustalPath);
        
        // set the launch path for the task
        [alignClustalTask setCurrentDirectoryPath:clustalPath];
        [alignClustalTask setLaunchPath:clustalPath];

        [alignClustalTask setArguments:clustalArguments];
        
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
        
        NSNotification *taskLaunchedNotification;
        taskLaunchedNotification = [NSNotification notificationWithName:@"taskLaunchedNotification" object:currentHelper];
        [defaultCenter postNotification:taskLaunchedNotification];

        [alignClustalTask launch];
        
        [alignClustalFromClustal readInBackgroundAndNotify];
        [alignClustalErrorData readInBackgroundAndNotify];
        }
        
        
        
        
        
        
    //return;




#if 0




    // set the current helper app to clustal, which lives in the application bundle
    currentHelper = @"clustal";
    
    // allocate a task for clustal
    if (alignClustalTask == nil)
        alignClustalTask = [[NSTask alloc] init];
    
    // find the temporary directory
    NSFileManager* alignClustalFileManager = [[NSFileManager alloc] init];
    NSDictionary* alignClustalTemporaryDirectoryAttributes = [NSDictionary dictionaryWithObject:NSFileTypeDirectory forKey:@"alignClustalTemporaryDirectory"];
    NSString* alignClustalUserTemporaryDirectory = NSTemporaryDirectory();
    [alignClustalFileManager createDirectoryAtPath:alignClustalUserTemporaryDirectory withIntermediateDirectories:NO attributes:alignClustalTemporaryDirectoryAttributes error:NULL];
        
    // find the clustal executable in the application bundle
    NSString* alignClustalPath = [[NSBundle mainBundle] pathForResource:@"clustalw2" ofType:nil];
    NSLog (@"alignClustalPath = %@", alignClustalPath);
    
    // set the launch path for the task
    [alignClustalTask setCurrentDirectoryPath:alignClustalPath];
    [alignClustalTask setLaunchPath:alignClustalPath];
    
    // set the arguments for clustal
    
    // string values for clustal arguments (set in the tool)
        
    // arguments that come from the Window Controller for this tool
    
    
    
    

    
    if ( [alignClustalAlignAr isEqualToString:@"-QUICKTREE"] == YES )
        {
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
        }
    else
        {
        alignClustalArguments = [NSArray arrayWithObjects: 
                                 alignClustalReduceConsoleOutputAr, 
                                 alignClustalInfileAr, 
                                 alignClustalOutfileAr, 
                                 alignClustalOutputAr,
                                 alignClustalGuideTreeAr, 
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
        }
    
    NSLog(@"array: %@", alignClustalArguments);
    
    [alignClustalTask setArguments:alignClustalArguments];
    
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
    
    NSNotification *taskLaunchedNotification;
    taskLaunchedNotification = [NSNotification notificationWithName:@"taskLaunchedNotification" object:currentHelper];
    [defaultCenter postNotification:taskLaunchedNotification];

    [alignClustalTask launch];
    
    [alignClustalFromClustal readInBackgroundAndNotify];
    [alignClustalErrorData readInBackgroundAndNotify];

    
    NSLog(@"Running Clustal from ToolAlign");
#   endif
}





/* Receive Clustal data */ 
- (void)receiveData:(NSNotification*)aNotification {

    NSData *incomingData;
    NSString *incomingText;
     
    incomingData = [[aNotification userInfo] objectForKey: NSFileHandleNotificationDataItem];
     
    incomingText = [[NSString alloc] initWithData: incomingData
    encoding: NSASCIIStringEncoding];
    
    NSLog(@"%@", incomingText);
     
    [alignClustalFromClustal readInBackgroundAndNotify];
    
    NSString *completionText;
    completionText = @"FASTA file created!";
    
    if ([incomingText rangeOfString: completionText].length > 0)
        {
        [self taskCompleted];
        }
     
    [incomingText release];
}
    

- (void)taskCompleted {

    NSLog (@"Clustal task completed");
}


@end
