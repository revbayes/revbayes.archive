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
    NSString* probconsPath      = [[[NSBundle mainBundle] resourcePath] stringByAppendingPathComponent:@"probcons"];
    NSString* unalignedFilePath = [[NSString stringWithString:unalignedDir] stringByAppendingPathComponent:fileName];
    NSString* workingDirectory  = [[NSString stringWithString:alignedDir] stringByAppendingPathComponent:tempDir];
    NSString* alignedFilePath   = [[NSString stringWithString:workingDirectory] stringByAppendingPathComponent:fileName];
    
    NSLog(@"probconsPath      = \"%@\"", probconsPath);
    NSLog(@"unalignedFilePath = \"%@\"", unalignedFilePath);
    NSLog(@"alignedDir        = \"%@\"", alignedDir);
    NSLog(@"workingDirectory  = \"%@\"", workingDirectory);
    NSLog(@"alignedFilePath   = \"%@\"", alignedFilePath);
    
    // collect the probcons arguments
    NSString* probconsConsistencyArg1         = @"-c";
    NSString* probconsConsistencyArg2         = [NSString stringWithFormat:@"%d", [myAlignmentTool probconsConsistency]];
    NSString* probconsIterativeRefinementArg1 = @"-ir";
    NSString* probconsIterativeRefinementArg2 = [NSString stringWithFormat:@"%d", [myAlignmentTool probconsIterativeRefinement]];
    NSString* probconsInArg                   = [NSString stringWithString:unalignedFilePath];
    NSString* probconsCarotArg                = @">";
    NSString* probconsOutArg                  = [NSString stringWithString:alignedFilePath];

    // set up an array with the probcons arguments
    NSArray* probconsArguments = [NSArray arrayWithObjects:
                                    probconsConsistencyArg1,
                                    probconsConsistencyArg2,
                                    probconsIterativeRefinementArg1,
                                    probconsIterativeRefinementArg2,
                                    probconsInArg,
                                    //probconsCarotArg,
                                    //probconsOutArg,
                                    nil];
    
    // create a the temporary directory
    NSFileManager* probconsFileManager = [[NSFileManager alloc] init];
    NSDictionary* probconsTemporaryDirectoryAttributes = [NSDictionary dictionaryWithObject:NSFileTypeDirectory forKey:@"probconsTemporaryDirectory"];
    [probconsFileManager createDirectoryAtPath:workingDirectory withIntermediateDirectories:NO attributes:probconsTemporaryDirectoryAttributes error:NULL];

    // create the task and launch
    alignTask = [[NSTask alloc] init];
    outputPipe = [[NSPipe alloc] init];
    outputFileHandle = [outputPipe fileHandleForReading];

    [alignTask setLaunchPath:probconsPath];
    [alignTask setCurrentDirectoryPath:workingDirectory];
    [alignTask setArguments:probconsArguments];
    [alignTask setStandardOutput:outputPipe];

    // listen for a notification indicating that the task finished and that data has been sent down the pipe
    NSNotificationCenter* defaultCenter = [NSNotificationCenter defaultCenter];
    [defaultCenter addObserver:self
                      selector:@selector(alignmentTaskDidFinish:)
                          name:NSTaskDidTerminateNotification
                        object:nil];
    
    [alignTask launch];
    outputFileHandle = [outputPipe fileHandleForReading];

    // read the fasta file output
    NSData* inData = nil;
    NSMutableData* outData = [[NSMutableData alloc] init];
    while ( (inData = [outputFileHandle availableData]) && [inData length] )
        {
        [outData appendData:inData];
        /*NSString* tempString = [[NSString alloc] initWithData:inData encoding:NSASCIIStringEncoding];
        //[outputView insertText:tempString];
        NSLog(@"tempString: %@\n", tempString);
        NSFileHandle* fh = [NSFileHandle fileHandleForWritingAtPath:alignedFilePath];
        [fh seekToEndOfFile];
        [fh writeData:[tempString dataUsingEncoding:NSASCIIStringEncoding]];
        [fh closeFile];*/
        }
    NSString* outString = [[NSString alloc] initWithData:outData encoding:NSASCIIStringEncoding];
    if ( [outString writeToFile:alignedFilePath atomically:YES encoding:NSASCIIStringEncoding error:nil] == NO )
        {
        [myAlignmentTool incrementErrorCount];
        }

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
