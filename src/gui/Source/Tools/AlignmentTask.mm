#import "AlignmentTask.h"
#import "ToolAlign.h"


@implementation AlignmentTask

- (void)dealloc {

    [[NSNotificationCenter defaultCenter] removeObserver:self];
}

- (id)init {

    return [self initWithAlignmentTool:nil];
}

- (id)initWithAlignmentTool:(ToolAlign*)t {

    if ( (self = [super init]) ) 
        {
        myAlignmentTool = t;
        alignTask = nil;
        outputPipe = nil;
        }
    return self;
}

- (void)alignFile:(id)fInfo {

    // get the file path and name of the file to align
    NSMutableArray* args         = (NSMutableArray*)fInfo;
    NSString* temporaryDirectory = [NSString stringWithString:[args objectAtIndex:0]];
    NSString* fileName           = [NSString stringWithString:[args objectAtIndex:1]];
    NSString* workingDirectory   = [NSString stringWithString:temporaryDirectory];
              workingDirectory   = [workingDirectory stringByAppendingString:[args objectAtIndex:2]];
    NSString* dFilePath          = [NSString stringWithString:temporaryDirectory];
              dFilePath          = [dFilePath stringByAppendingString:fileName];
    
    // collect the clustal arguments
    NSString* clustalMultipleAlignArg        = @"-ALIGN";
    NSString* clustalInfileArg               = @"-INFILE=";
              clustalInfileArg               = [clustalInfileArg stringByAppendingString:dFilePath];
    NSString* clustalOutfileArg              = @"-OUTFILE=";
              clustalOutfileArg              = [clustalOutfileArg stringByAppendingString:workingDirectory];
              clustalOutfileArg              = [clustalOutfileArg stringByAppendingString:@"/"];
              clustalOutfileArg              = [clustalOutfileArg stringByAppendingString:fileName];
    NSString* clustalOutputArg               = @"-OUTPUT=FASTA";    
    NSString* clustalGuideTreeArg            = @"-NEWTREE=";
              clustalGuideTreeArg            = [clustalGuideTreeArg stringByAppendingString: workingDirectory];
              clustalGuideTreeArg            = [clustalGuideTreeArg stringByAppendingString: @"/clustaltree.dnd"];
    NSString* clustalAlignArg                = @"-QUICKTREE";
    NSString* clustalWordLengthArg           = @"-KTUPLE=";
              clustalWordLengthArg           = [clustalWordLengthArg stringByAppendingFormat:@"%i", [myAlignmentTool clustalWordLength]];
    NSString* clustalWindowArg               = @"-WINDOW=";
              clustalWindowArg               = [clustalWindowArg stringByAppendingFormat: @"%i", [myAlignmentTool clustalWindow]];
    NSString* clustalScoreTypeArg            = @"-SCORE=";
              clustalScoreTypeArg            = [clustalScoreTypeArg stringByAppendingString:[myAlignmentTool clustalScoreType]];
    NSString* clustalNumberDiagonalsArg      = @"-TOPDIAGS=";
              clustalNumberDiagonalsArg      = [clustalNumberDiagonalsArg stringByAppendingFormat: @"%i", [myAlignmentTool clustalNumberDiagonals]];
    NSString* clustalPairGapPenaltyArg       = @"-PAIRGAP=";
              clustalPairGapPenaltyArg       = [clustalPairGapPenaltyArg stringByAppendingFormat: @"%i", [myAlignmentTool clustalPairGapPenalty]];
    NSString* clustalMatrixArg               = @"-PWMATRIX=";
              clustalMatrixArg               = [clustalMatrixArg stringByAppendingString:[myAlignmentTool clustalMatrix]];
    NSString* clustalGapOpenPenaltyAr        = @"-PWGAPEXT=";
              clustalGapOpenPenaltyAr        = [clustalGapOpenPenaltyAr stringByAppendingFormat: @"%f", [myAlignmentTool clustalGapOpenPenalty]];
    NSString* clustalEndGapsArg              = @"-ENDGAPS=";
              clustalEndGapsArg              = [clustalEndGapsArg stringByAppendingString: [myAlignmentTool clustalEndGaps]];
    NSString* clustalGapExtensionCostArg     = @"-GAPEXT=";
              clustalGapExtensionCostArg     = [clustalGapExtensionCostArg stringByAppendingFormat: @"%f", [myAlignmentTool clustalGapExtensionCost]];
    NSString* clustalGapSeparationPenaltyArg = @"-GAPDIST=";
              clustalGapSeparationPenaltyArg = [clustalGapSeparationPenaltyArg stringByAppendingFormat: @"%i", [myAlignmentTool clustalGapSeparationPenalty]];
    NSString* clustalIterationArg            = @"-ITERATION=";
              clustalIterationArg            = [clustalIterationArg stringByAppendingString:[myAlignmentTool clustalIteration]];
    NSString* clustalNumberOfIterationsArg   = @"-NUMITER=";
              clustalNumberOfIterationsArg   = [clustalNumberOfIterationsArg stringByAppendingFormat: @"%i", [myAlignmentTool clustalNumberOfIterations]];

    // set up an array with the clustal arguments
    NSArray* clustalArguments;
    if ( [[myAlignmentTool clustalAlign] isEqualToString: @"Fast"] == YES )
        {
        clustalArguments = [NSArray arrayWithObjects: 
                                 //@"-QUIET", 
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
                                 //@"-QUIET", 
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
        
    // allocate a task for clustal
    alignTask = [[NSTask alloc] init];
    outputPipe = [[NSPipe alloc] init];

    // create a the temporary directory
    NSFileManager* clustalFileManager = [[NSFileManager alloc] init];
    NSDictionary* clustalTemporaryDirectoryAttributes = [NSDictionary dictionaryWithObject:NSFileTypeDirectory forKey:@"clustalTemporaryDirectory"];
    [clustalFileManager createDirectoryAtPath:workingDirectory withIntermediateDirectories:NO attributes:clustalTemporaryDirectoryAttributes error:NULL];
    
    // find the clustal executable in the application bundle
    NSString* clustalPath = [[NSBundle mainBundle] pathForResource:@"clustalw2" ofType:nil];
    
    NSLog(@"clustalArguments=%@", clustalArguments);
    // set variables for the task
    [alignTask setCurrentDirectoryPath:clustalPath];
    [alignTask setLaunchPath:clustalPath];
    [alignTask setArguments:clustalArguments];
    outputFileHandle = [outputPipe fileHandleForReading];
    [alignTask setStandardOutput:outputPipe];
    NSLog(@"here I am\n");
    
    // listen for a notification indicating that the task finished and that data has been sent down the pipe
    NSNotificationCenter* defaultCenter = [NSNotificationCenter defaultCenter];
    [defaultCenter addObserver:self
                      selector:@selector(alignmentTaskDidFinish:)
                          name:NSTaskDidTerminateNotification
                        object:nil];
    [defaultCenter addObserver:self
                      selector:@selector(receiveData:)
                          name:NSFileHandleReadCompletionNotification
                        object:outputFileHandle];

    // launch the task and wait
NSLog(@" 1 \n");
    [alignTask launch];
NSLog(@" 2 \n");
    [outputFileHandle readInBackgroundAndNotify];
NSLog(@" 3 \n");
    [alignTask waitUntilExit];
NSLog(@" 4 \n");
    int status = [alignTask terminationStatus];
    if (status != 0)
        NSLog(@"Problem aligning data file");
}

- (void)alignmentTaskDidFinish:(NSNotification*)notification {

    if ( [notification object] == alignTask )
        {
        [myAlignmentTool decrementTaskCount];
        }
}

- (void)receiveData:(NSNotification*)aNotification {
     
    NSData* incomingData = [[aNotification userInfo] objectForKey:NSFileHandleNotificationDataItem];
    NSString* incomingText = [[NSString alloc] initWithData:incomingData encoding:NSASCIIStringEncoding];
    NSLog(@"task %@ %@", self, incomingText);
}

@end
