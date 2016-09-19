#import "AlignmentTaskTcoffee.h"
#import "ToolAlign.h"


@implementation AlignmentTaskTcoffee

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
    NSString* tcoffeePath       = [[[NSBundle mainBundle] resourcePath] stringByAppendingPathComponent:@"t_coffee"];
    NSString* unalignedFilePath = [[NSString stringWithString:unalignedDir] stringByAppendingPathComponent:fileName];
    NSString* workingDirectory  = [[NSString stringWithString:alignedDir] stringByAppendingPathComponent:tempDir];
    NSString* alignedFilePath   = [[NSString stringWithString:workingDirectory] stringByAppendingPathComponent:fileName];
    NSString* guideTreeFilePath = [[NSString stringWithString:workingDirectory] stringByAppendingPathComponent:@"tcoffeetree.dnd"];
    
    /*NSLog(@"tcoffeePath       = \"%@\"", tcoffeePath);
    NSLog(@"unalignedFilePath = \"%@\"", unalignedFilePath);
    NSLog(@"alignedDir        = \"%@\"", alignedDir);
    NSLog(@"workingDirectory  = \"%@\"", workingDirectory);
    NSLog(@"alignedFilePath   = \"%@\"", alignedFilePath);
    NSLog(@"guideTreeFilePath = \"%@\"", guideTreeFilePath);*/
    
    // collect the tcoffee arguments
    NSString* tcoffeeInArg1                  = @"-in";
    NSString* tcoffeeInArg2                  = [NSString stringWithString:unalignedFilePath];
    NSString* tcoffeeOutArg1                 = @"-outfile";
    NSString* tcoffeeOutArg2                 = [NSString stringWithString:alignedFilePath];
    NSString* tcoffeeFmtArg                  = @"-output=fasta_aln";
    NSString* tcoffeeGapOpenArg              = [NSString stringWithFormat:@"-gapopen=%f", [myAlignmentTool tcoffeeGapOpenPenalty]];
    NSString* tcoffeeGapExtensionArg         = [NSString stringWithFormat:@"-gapext=%f", [myAlignmentTool tcoffeeGapExtensionCost]];
    
    // set up an array with the tcoffee arguments
    NSArray* tcoffeeArguments = [NSArray arrayWithObjects:
                                 tcoffeeInArg1,
                                 tcoffeeInArg2,
                                 tcoffeeOutArg1,
                                 tcoffeeOutArg2,
                                 tcoffeeFmtArg,
                                 tcoffeeGapOpenArg,
                                 tcoffeeGapExtensionArg,
                                 nil];
    
    // create a the temporary directory for the alignments
    NSFileManager* tcoffeeFileManager = [[NSFileManager alloc] init];
    NSDictionary* tcoffeeTemporaryDirectoryAttributes = [NSDictionary dictionaryWithObject:NSFileTypeDirectory forKey:@"tcoffeeTemporaryDirectory"];
    [tcoffeeFileManager createDirectoryAtPath:workingDirectory withIntermediateDirectories:NO attributes:tcoffeeTemporaryDirectoryAttributes error:NULL];

    // allocate a task for tcoffee
    alignTask = [[NSTask alloc] init];
    outputPipe = [[NSPipe alloc] init];
    [alignTask setCurrentDirectoryPath:workingDirectory];
    [alignTask setLaunchPath:tcoffeePath];
    [alignTask setArguments:tcoffeeArguments];
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
