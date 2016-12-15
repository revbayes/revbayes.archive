#import "AlignmentTaskClustal.h"
#import "ToolAlign.h"


@implementation AlignmentTaskClustal

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
        alignTask       = nil;
        outputPipe      = nil;
        }
    return self;
}

- (void)alignFile:(id)fInfo {

    // get the information from the arguments passed into this task
    NSMutableArray* args   = (NSMutableArray*)fInfo;
    NSString* unalignedDir = [NSString stringWithString:[args objectAtIndex:0]];
    NSString* alignedDir   = [NSString stringWithString:[args objectAtIndex:1]];
    NSString* fileName     = [NSString stringWithString:[args objectAtIndex:2]];
    NSString* tempDir      = [NSString stringWithString:[args objectAtIndex:3]];

    // get file paths needed for this task
    NSString* clustalPath       = [[[NSBundle mainBundle] resourcePath] stringByAppendingPathComponent:@"clustalw2"];
    NSString* unalignedFilePath = [[NSString stringWithString:unalignedDir] stringByAppendingPathComponent:fileName];
    NSString* workingDirectory  = [[NSString stringWithString:alignedDir] stringByAppendingPathComponent:tempDir];
    NSString* alignedFilePath   = [[NSString stringWithString:workingDirectory] stringByAppendingPathComponent:fileName];
    NSString* guideTreeFilePath = [[NSString stringWithString:workingDirectory] stringByAppendingPathComponent:@"clustaltree.dnd"];
    
    /*NSLog(@"clustalPath       = \"%@\"", clustalPath);
    NSLog(@"unalignedFilePath = \"%@\"", unalignedFilePath);
    NSLog(@"alignedDir        = \"%@\"", alignedDir);
    NSLog(@"workingDirectory  = \"%@\"", workingDirectory);
    NSLog(@"alignedFilePath   = \"%@\"", alignedFilePath);
    NSLog(@"guideTreeFilePath = \"%@\"", guideTreeFilePath);*/
    
    // collect the clustal arguments
    NSString* clustalMultipleAlignArg        = @"-ALIGN";
    NSString* clustalInfileArg               = @"-INFILE=";
              clustalInfileArg               = [clustalInfileArg stringByAppendingString:unalignedFilePath];
    NSString* clustalOutfileArg              = @"-OUTFILE=";
              clustalOutfileArg              = [clustalOutfileArg stringByAppendingString:alignedFilePath];
    NSString* clustalOutputArg               = @"-OUTPUT=FASTA";
    NSString* clustalGuideTreeArg            = @"-NEWTREE=";
              clustalGuideTreeArg            = [clustalGuideTreeArg stringByAppendingString: guideTreeFilePath];
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
        
    // create a the temporary directory for the alignments
    NSFileManager* clustalFileManager = [[NSFileManager alloc] init];
    NSDictionary* clustalTemporaryDirectoryAttributes = [NSDictionary dictionaryWithObject:NSFileTypeDirectory forKey:@"clustalTemporaryDirectory"];
    [clustalFileManager createDirectoryAtPath:workingDirectory withIntermediateDirectories:NO attributes:clustalTemporaryDirectoryAttributes error:NULL];

    // allocate a task for clustal
    alignTask = [[NSTask alloc] init];
    outputPipe = [[NSPipe alloc] init];
    [alignTask setCurrentDirectoryPath:workingDirectory];
    [alignTask setLaunchPath:clustalPath];
    [alignTask setArguments:clustalArguments];
    outputFileHandle = [outputPipe fileHandleForReading];
    [alignTask setStandardOutput:outputPipe];

    // listen for a notification indicating that the task finished
    NSNotificationCenter* defaultCenter = [NSNotificationCenter defaultCenter];
    [defaultCenter addObserver:self
                      selector:@selector(alignmentTaskDidFinish:)
                          name:NSTaskDidTerminateNotification
                        object:nil];

    // launch the task and wait
    [alignTask launch];
    [outputFileHandle readInBackgroundAndNotify];
    [alignTask waitUntilExit];
    int status = [alignTask terminationStatus];
    if (status != 0)
        [myAlignmentTool incrementErrorCount];
}

- (void)alignmentTaskDidFinish:(NSNotification*)notification {

    if ( [notification object] == alignTask )
        {
        [myAlignmentTool decrementTaskCount];
        }
}

@end
