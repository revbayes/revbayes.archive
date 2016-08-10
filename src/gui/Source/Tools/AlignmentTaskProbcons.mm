#import "AlignmentTaskProbcons.h"
#import "ToolAlign.h"


@implementation AlignmentTaskProbcons

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
    NSString* musclePath        = [[[NSBundle mainBundle] resourcePath] stringByAppendingPathComponent:@"muscle3.8-1.31"];
    NSString* unalignedFilePath = [[NSString stringWithString:unalignedDir] stringByAppendingPathComponent:fileName];
    NSString* workingDirectory  = [[NSString stringWithString:alignedDir] stringByAppendingPathComponent:tempDir];
    NSString* alignedFilePath   = [[NSString stringWithString:workingDirectory] stringByAppendingPathComponent:fileName];
    
    /*NSLog(@"musclePath        = \"%@\"", musclePath);
    NSLog(@"unalignedFilePath = \"%@\"", unalignedFilePath);
    NSLog(@"alignedDir        = \"%@\"", alignedDir);
    NSLog(@"workingDirectory  = \"%@\"", workingDirectory);
    NSLog(@"alignedFilePath   = \"%@\"", alignedFilePath);*/
    
    // collect the muscle arguments
    NSString* muscleInArg1              = @"-in";
    NSString* muscleInArg2              = [NSString stringWithString:unalignedFilePath];
    NSString* muscleOutArg1             = @"-out";
    NSString* muscleOutArg2             = [NSString stringWithString:alignedFilePath];
    NSString* muscleSeqTypeArg1         = @"-seqtype";
    NSString* muscleSeqTypeArg2         = @"auto";
    NSString* muscleAnchorsArg          = @"-anchors";
    NSString* muscleAnchorSpacingArg1   = @"-anchorspacing";
    NSString* muscleAnchorSpacingArg2   = [NSString stringWithFormat:@"%i", [myAlignmentTool muscleAnchorSpacing]];
    NSString* muscleCenterArg1          = @"-center";
    NSString* muscleCenterArg2          = [NSString stringWithFormat: @"%f", [myAlignmentTool muscleCenter]];
    NSString* muscleCluster1Arg1        = @"-cluster1";
    NSString* muscleCluster1Arg2        = [NSString stringWithString:[myAlignmentTool muscleCluster1]];
    NSString* muscleCluster2Arg1        = @"-cluster2";
    NSString* muscleCluster2Arg2        = [NSString stringWithString:[myAlignmentTool muscleCluster2]];
    NSString* muscleDiagLengthArg1      = @"-diaglength";
    NSString* muscleDiagLengthArg2      = [NSString stringWithFormat:@"%i", [myAlignmentTool muscleDiagLength]];
    NSString* muscleDiagMarginArg1      = @"-diagmargin";
    NSString* muscleDiagMarginArg2      = [NSString stringWithFormat:@"%i", [myAlignmentTool muscleDiagMargin]];
    NSString* muscleDistance1Arg1       = @"-distance1";
    NSString* muscleDistance1Arg2       = [NSString stringWithString:[myAlignmentTool muscleDistance1]];
    //NSString* muscleDistance2Arg1       = @"-distance2";
    //NSString* muscleDistance2Arg2       = [NSString stringWithString:[myAlignmentTool muscleDistance2]];
    NSString* muscleGapOpenArg1         = @"-gapopen";
    NSString* muscleGapOpenArg2         = [NSString stringWithFormat:@"%f", [myAlignmentTool muscleGapOpen]];
    NSString* muscleHydroArg1           = @"-hydro";
    NSString* muscleHydroArg2           = [NSString stringWithFormat:@"%i", [myAlignmentTool muscleHydro]];
    NSString* muscleHydroFactorArg1     = @"-hydrofactor";
    NSString* muscleHydroFactorArg2     = [NSString stringWithFormat:@"%f", [myAlignmentTool muscleHydroFactor]];
    //NSString* muscleMaxDiagBreakArg1    = @"-maxdiagbreak";
    //NSString* muscleMaxDiagBreakArg2    = [NSString stringWithFormat:@"%i", [myAlignmentTool muscleMaxDiagBreak]];
    NSString* muscleMaxItersArg1        = @"-maxiters";
    NSString* muscleMaxItersArg2        = [NSString stringWithFormat:@"%i", [myAlignmentTool muscleMaxIters]];
    NSString* muscleMaxTreesArg1        = @"-maxtrees";
    NSString* muscleMaxTreesArg2        = [NSString stringWithFormat:@"%i", [myAlignmentTool muscleMaxTrees]];
    NSString* muscleMinBestColScoreArg1 = @"-minbestcolscore";
    NSString* muscleMinBestColScoreArg2 = [NSString stringWithFormat:@"%f", [myAlignmentTool muscleMinBestColScore]];
    NSString* muscleMinSmoothScoreArg1  = @"-minsmoothscore";
    NSString* muscleMinSmoothScoreArg2  = [NSString stringWithFormat:@"%f", [myAlignmentTool muscleMinSmoothScore]];
    NSString* muscleObjScoreArg1        = @"-objscore";
    NSString* muscleObjScoreArg2        = [NSString stringWithString:[myAlignmentTool muscleObjScore]];
    NSString* muscleRoot1Arg1           = @"-root1";
    NSString* muscleRoot1Arg2           = [NSString stringWithString:[myAlignmentTool muscleRoot1]];
    NSString* muscleRoot2Arg1           = @"-root2";
    NSString* muscleRoot2Arg2           = [NSString stringWithString:[myAlignmentTool muscleRoot2]];
    NSString* muscleSmoothScoreCeilArg1 = @"-smoothscoreceil";
    NSString* muscleSmoothScoreCeilArg2 = [NSString stringWithFormat:@"%f", [myAlignmentTool muscleSmoothScoreCeil]];
    NSString* muscleSmoothWindowArg1    = @"-smoothwindow";
    NSString* muscleSmoothWindowArg2    = [NSString stringWithFormat:@"%i", [myAlignmentTool muscleSmoothWindow]];
    NSString* muscleSUEFFArg1           = @"-SUEFF";
    NSString* muscleSUEFFArg2           = [NSString stringWithFormat:@"%f", [myAlignmentTool muscleSUEFF]];
    NSString* muscleWeight1Arg1         = @"-weight1";
    NSString* muscleWeight1Arg2         = [NSString stringWithString:[myAlignmentTool muscleWeight1]];
    NSString* muscleWeight2Arg1         = @"-weight2";
    NSString* muscleWeight2Arg2         = [NSString stringWithString:[myAlignmentTool muscleWeight2]];
    NSString* muscleQuiet              = @"-quiet";

    // set up an array with the muscle arguments
    NSArray* muscleArguments = [NSArray arrayWithObjects:
                                    muscleInArg1,
                                    muscleInArg2,
                                    muscleOutArg1,
                                    muscleOutArg2,
                                    muscleSeqTypeArg1,
                                    muscleSeqTypeArg2,
                                    muscleAnchorsArg,
                                    muscleAnchorSpacingArg1,
                                    muscleAnchorSpacingArg2,
                                    muscleCenterArg1,
                                    muscleCenterArg2,
                                    muscleCluster1Arg1,
                                    muscleCluster1Arg2,
                                    muscleCluster2Arg1,
                                    muscleCluster2Arg2,
                                    muscleDiagLengthArg1,
                                    muscleDiagLengthArg2,
                                    muscleDiagMarginArg1,
                                    muscleDiagMarginArg2,
                                    muscleDistance1Arg1,
                                    muscleDistance1Arg2,
                                    //muscleDistance2Arg1,
                                    //muscleDistance2Arg2,
                                    muscleGapOpenArg1,
                                    muscleGapOpenArg2,
                                    muscleHydroArg1,
                                    muscleHydroArg2,
                                    muscleHydroFactorArg1,
                                    muscleHydroFactorArg2,
                                    //muscleMaxDiagBreakArg1,
                                    //muscleMaxDiagBreakArg2,
                                    muscleMaxItersArg1,
                                    muscleMaxItersArg2,
                                    muscleMaxTreesArg1,
                                    muscleMaxTreesArg2,
                                    muscleMinBestColScoreArg1,
                                    muscleMinBestColScoreArg2,
                                    muscleMinSmoothScoreArg1,
                                    muscleMinSmoothScoreArg2,
                                    muscleObjScoreArg1,
                                    muscleObjScoreArg2,
                                    muscleRoot1Arg1,
                                    muscleRoot1Arg2,
                                    muscleRoot2Arg1,
                                    muscleRoot2Arg2,
                                    muscleSmoothScoreCeilArg1,
                                    muscleSmoothScoreCeilArg2,
                                    muscleSmoothWindowArg1,
                                    muscleSmoothWindowArg2,
                                    muscleSUEFFArg1,
                                    muscleSUEFFArg2,
                                    muscleWeight1Arg1,
                                    muscleWeight1Arg2,
                                    muscleWeight2Arg1,
                                    muscleWeight2Arg2,
                                    muscleQuiet,
                                    nil];
    
    // create a the temporary directory
    NSFileManager* muscleFileManager = [[NSFileManager alloc] init];
    NSDictionary* muscleTemporaryDirectoryAttributes = [NSDictionary dictionaryWithObject:NSFileTypeDirectory forKey:@"muscleTemporaryDirectory"];
    [muscleFileManager createDirectoryAtPath:workingDirectory withIntermediateDirectories:NO attributes:muscleTemporaryDirectoryAttributes error:NULL];

    // allocate a task for muscle
    alignTask = [[NSTask alloc] init];
    outputPipe = [[NSPipe alloc] init];
    [alignTask setCurrentDirectoryPath:workingDirectory];
    [alignTask setLaunchPath:musclePath];
    [alignTask setArguments:muscleArguments];
    outputFileHandle = [outputPipe fileHandleForReading];
    [alignTask setStandardOutput:outputPipe];
    
    // listen for a notification indicating that the task finished and that data has been sent down the pipe
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
