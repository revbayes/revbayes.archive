#import "AlignmentMuscleTask.h"
#import "ToolAlign.h"


@implementation AlignmentMuscleTask

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
    NSString* guideTreeFilePath = [[NSString stringWithString:workingDirectory] stringByAppendingPathComponent:@"clustaltree.dnd"];
    
    /*NSLog(@"musclePath        = \"%@\"", clustalPath);
    NSLog(@"unalignedFilePath = \"%@\"", unalignedFilePath);
    NSLog(@"alignedDir        = \"%@\"", alignedDir);
    NSLog(@"workingDirectory  = \"%@\"", workingDirectory);
    NSLog(@"alignedFilePath   = \"%@\"", alignedFilePath);
    NSLog(@"guideTreeFilePath = \"%@\"", guideTreeFilePath);*/
    
    // collect the muscle arguments
    NSString* muscleInArg              = @"-in";
              muscleInArg              = [muscleInArg stringByAppendingString:unalignedFilePath];
    NSString* muscleOutArg             = @"-out";
              muscleOutArg             = [muscleOutArg stringByAppendingString:alignedFilePath];
    NSString* muscleSeqTypeArg         = @"-seqtype";
              muscleSeqTypeArg         = [muscleSeqTypeArg stringByAppendingString:@"auto"];
    NSString* muscleAnchorSpacingArg   = @"-anchorspacing";
              muscleAnchorSpacingArg   = [muscleAnchorSpacingArg stringByAppendingFormat:@"%i", [myAlignmentTool muscleAnchorSpacing]];
    NSString* muscleCenterArg          = @"-center";
              muscleCenterArg          = [muscleCenterArg stringByAppendingFormat: @"%f", [myAlignmentTool muscleCenter]];
    NSString* muscleCluster1Arg        = @"-cluster1";
              muscleCluster1Arg        = [muscleCluster1Arg stringByAppendingString: [myAlignmentTool muscleCluster1]];
    NSString* muscleCluster2Arg        = @"-cluster2";
              muscleCluster2Arg        = [muscleCluster2Arg stringByAppendingString: [myAlignmentTool muscleCluster2]];
    NSString* muscleDiagLengthArg      = @"-diaglength";
              muscleDiagLengthArg      = [muscleDiagLengthArg stringByAppendingFormat:@"%i", [myAlignmentTool muscleDiagLength]];
    NSString* muscleDiagMarginArg      = @"-diagmargin";
              muscleDiagMarginArg      = [muscleDiagMarginArg stringByAppendingFormat:@"%i", [myAlignmentTool muscleDiagMargin]];
    NSString* muscleDistance1Arg       = @"-distance1";
              muscleDistance1Arg       = [muscleDistance1Arg stringByAppendingString: [myAlignmentTool muscleDistance1]];
    NSString* muscleDistance2Arg       = @"-distance2";
              muscleDistance2Arg       = [muscleDistance2Arg stringByAppendingString: [myAlignmentTool muscleDistance2]];
    NSString* muscleGapOpenArg         = @"-gapopen";
              muscleGapOpenArg         = [muscleGapOpenArg stringByAppendingFormat: @"%f", [myAlignmentTool muscleGapOpen]];
    NSString* muscleHydroArg           = @"-hydro";
              muscleHydroArg           = [muscleHydroArg stringByAppendingFormat:@"%i", [myAlignmentTool muscleHydro]];
    NSString* muscleHydroFactorArg     = @"-hydrofactor";
              muscleHydroFactorArg     = [muscleHydroFactorArg stringByAppendingFormat: @"%f", [myAlignmentTool muscleHydroFactor]];
    NSString* muscleMaxDiagBreakArg    = @"-maxdiagbreak";
              muscleMaxDiagBreakArg    = [muscleMaxDiagBreakArg stringByAppendingFormat:@"%i", [myAlignmentTool muscleMaxDiagBreak]];
    NSString* muscleMaxItersArg        = @"-maxiters";
              muscleMaxItersArg        = [muscleMaxItersArg stringByAppendingFormat:@"%i", [myAlignmentTool muscleMaxIters]];
    NSString* muscleMaxTreesArg        = @"-maxtrees";
              muscleMaxTreesArg        = [muscleMaxTreesArg stringByAppendingFormat:@"%i", [myAlignmentTool muscleMaxTrees]];
    NSString* muscleMinBestColScoreArg = @"-minbestcolscore";
              muscleMinBestColScoreArg = [muscleMinBestColScoreArg stringByAppendingFormat: @"%f", [myAlignmentTool muscleMinBestColScore]];
    NSString* muscleMinSmoothScoreArg  = @"-minsmoothscore";
              muscleMinSmoothScoreArg  = [muscleMinSmoothScoreArg stringByAppendingFormat: @"%f", [myAlignmentTool muscleMinSmoothScore]];
    NSString* muscleObjScoreArg        = @"-objscore";
              muscleObjScoreArg        = [muscleObjScoreArg stringByAppendingString: [myAlignmentTool muscleObjScore]];
    NSString* muscleRoot1Arg           = @"-root1";
              muscleRoot1Arg           = [muscleRoot1Arg stringByAppendingString: [myAlignmentTool muscleRoot1]];
    NSString* muscleRoot2Arg           = @"-root2";
              muscleRoot2Arg           = [muscleRoot2Arg stringByAppendingString: [myAlignmentTool muscleRoot2]];
    NSString* muscleSmoothScoreCeilArg = @"-smoothscoreceil";
              muscleSmoothScoreCeilArg = [muscleSmoothScoreCeilArg stringByAppendingFormat: @"%f", [myAlignmentTool muscleSmoothScoreCeil]];
    NSString* muscleSmoothWindowArg    = @"-smoothwindow";
              muscleSmoothWindowArg    = [muscleSmoothWindowArg stringByAppendingFormat:@"%i", [myAlignmentTool muscleSmoothWindow]];
    NSString* muscleSUEFFArg           = @"-SUEFF";
              muscleSUEFFArg           = [muscleSUEFFArg stringByAppendingFormat: @"%f", [myAlignmentTool muscleSUEFF]];
    NSString* muscleWeight1Arg         = @"-weight1";
              muscleWeight1Arg         = [muscleWeight1Arg stringByAppendingString: [myAlignmentTool muscleWeight1]];
    NSString* muscleWeight2Arg         = @"-weight2";
              muscleWeight2Arg         = [muscleWeight2Arg stringByAppendingString: [myAlignmentTool muscleWeight2]];

    // set up an array with the muscle arguments
    NSArray* muscleArguments = [NSArray arrayWithObjects:
                                    muscleInArg,
                                    muscleOutArg,
                                    muscleSeqTypeArg,
                                    muscleAnchorSpacingArg,
                                    muscleCenterArg,
                                    muscleCluster1Arg,
                                    muscleCluster2Arg,
                                    muscleDiagLengthArg,
                                    muscleDiagMarginArg,
                                    muscleDistance1Arg,
                                    muscleDistance2Arg,
                                    muscleGapOpenArg,
                                    muscleHydroArg,
                                    muscleHydroFactorArg,
                                    muscleMaxDiagBreakArg,
                                    muscleMaxItersArg,
                                    muscleMaxTreesArg,
                                    muscleMinBestColScoreArg,
                                    muscleMinSmoothScoreArg,
                                    muscleObjScoreArg,
                                    muscleRoot1Arg,
                                    muscleRoot2Arg,
                                    muscleSmoothScoreCeilArg,
                                    muscleSmoothWindowArg,
                                    muscleSUEFFArg,
                                    muscleWeight1Arg,
                                    muscleWeight2Arg,
                                    nil];
    
    // create a the temporary directory
    NSFileManager* clustalFileManager = [[NSFileManager alloc] init];
    NSDictionary* clustalTemporaryDirectoryAttributes = [NSDictionary dictionaryWithObject:NSFileTypeDirectory forKey:@"clustalTemporaryDirectory"];
    [clustalFileManager createDirectoryAtPath:workingDirectory withIntermediateDirectories:NO attributes:clustalTemporaryDirectoryAttributes error:NULL];

    // allocate a task for clustal
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
    [defaultCenter addObserver:self
                      selector:@selector(receiveData:)
                          name:NSFileHandleReadCompletionNotification
                        object:outputFileHandle];

    // launch the task and wait
    [alignTask launch];
    [outputFileHandle readInBackgroundAndNotify];
    [alignTask waitUntilExit];
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
}

@end
